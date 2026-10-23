#include "DrawGraph.h"
#include "dijkstra.h"


Point *pos;      //array of vertex positions
Point *disp;     //array of displacements for each vertex
Point *oldDisp;  //array of old displacement values for each vertex
coord_t *dispNorm;  //the norm of the displacement of each vertex
coord_t *oldDispNorm;  //the norm of the old displacement of each vertex
double *heat;   //current temperature of each vertex
double *oldCos;
size_tt numOfVert;
Graph *graph;
int misfDepth;
size_tt *misfSize;
size_tt *misf;
size_tt *nbr;
graphDist ***nbrs; /* old comment is propably wrong: array starts at 1, 0 element is ignored */
size_tt numOfInitVert;

coord_t edge = 32;   //ideal edge length
coord_t tinit;
int FR_full;
int FR_levels;
int plot_all_vert;
double heat_fraction;

int initRounds;
int finalRounds;

float r;
float s;

coord_t getAverageWeightOfEdge(Graph* aG, int numComponents);

//////////////////////////////////////////////////////////////////////////////
//
//   This function initializes a draw Graph structure.
//   It makes calls to perform the filtration and then stores the results
//   in the structure
//
//////////////////////////////////////////////////////////////////////////////

DrawGraph *init_DG(Graph *G, int numComponents, int _dim, size_tt _numOfInitVert, int _displayPar, int randBFS, int _FR_full, int _FR_levels, int _plot_all_vert, int _rounds, int _finalRounds, float _heat_fraction, float _r, float _s)
{
  DrawGraph *result;
  MISF *max_ind_set;
  size_tt vert;
  int curr_component;
  result = (DrawGraph *)malloc(numComponents * sizeof(DrawGraph));

  if(G->weightList != 0) // compute average weight of edge
  	edge=getAverageWeightOfEdge(G,numComponents);
  else
    edge = 32;
  
  // process each component in the graph
  for(curr_component = 0; curr_component < numComponents; curr_component ++)
    {
      graph = &G[curr_component];

      //initial temperature must stay above 5 to allow vertices to move in all levels
      tinit = edge/6;

      //initialize all of parameters selected by the user
      FR_full = _FR_full;
      FR_levels = _FR_levels;
      plot_all_vert = _plot_all_vert;
      initRounds = _rounds;
      finalRounds = _finalRounds;
      heat_fraction = _heat_fraction;
      r = _r;     //used for update_Temp_v2()
      s = _s;     //used for update_Temp_v2()
      dim = _dim;

      // perform the filtration and initialize arrays used by drawGraph structure
      numOfVert = graph->numOfVert;
      numOfInitVert = min(_numOfInitVert, numOfVert);
      max_ind_set  = create_misf(graph, randBFS, numOfInitVert, plot_all_vert);
      misfDepth = max_ind_set->depth;
      misfSize = max_ind_set->size;
      misf = max_ind_set->filt;
      pos = (Point *)malloc(numOfVert * sizeof(Point));
      disp = (Point *)malloc(numOfVert * sizeof(Point));
      oldDisp = (Point *)malloc(numOfVert * sizeof(Point));
      dispNorm = (coord_t *)malloc(numOfVert * sizeof(coord_t));
      oldDispNorm = (coord_t *)malloc(numOfVert * sizeof(coord_t));
      heat = (coord_t *)malloc(numOfVert * sizeof(coord_t));
      oldCos = (double *)malloc(numOfVert * sizeof(double));
      
      debug("before set nbr size\n");
      set_nbr_size();
      nbrs =  (graphDist ***)malloc((numOfVert) * sizeof(graphDist **));

      /* initialize heat and oldCos */
      for(vert=0; vert < numOfVert; vert++)
    	{
	      heat[ vert ] = tinit;
	      oldCos[vert] = 1;
    	  initialize_Point(&oldDisp[vert], 0, 0, 0, 0, _dim);
	      initialize_Point(&disp[vert], 0, 0, 0, 0, _dim);
    	  initialize_Point(&pos[vert], 0, 0, 0, 0, _dim);
	      if (plot_all_vert)
	        point_duplicate(&pos[vert], random_point());
        nbrs[vert]=0;
    	}

      //store all fields in the new drawGraph structure
      result[curr_component].diameter = diam;
      result[curr_component].dim = _dim;
      result[curr_component].edge = edge;
      result[curr_component].currLevel =  max_ind_set->depth - 1;
      result[curr_component].misf = max_ind_set;
      result[curr_component].numOfInitVert = numOfInitVert;
      result[curr_component].pos = pos;
      result[curr_component].disp = disp;
      result[curr_component].oldDisp = oldDisp;
      result[curr_component].dispNorm = dispNorm;
      result[curr_component].oldDispNorm = oldDispNorm;
      result[curr_component].heat = heat;
      result[curr_component].oldCos = oldCos;
      result[curr_component].nbr = nbr;
      result[curr_component].nbrs = nbrs;
      result[curr_component].vertDepth = vertDepth;
      result[curr_component].displayPar = _displayPar;
      result[curr_component].createList = 0;
      result[curr_component].createListSwitch = 1;
      result[curr_component].roundsCtr = 0;
      result[curr_component].rounds = initRounds;
      result[curr_component].firstRound = 1;
    }
  return result;
}



//**************************************************************************
//
//    This is the main GRIP engine.  It performs placement and refinement 
//    of vertices.  It uses settings stored in the drawGraph structure 
//    in init_DG().
//
//**************************************************************************
void misf_engine(Graph *G, DrawGraph *dg)
{
  MISF *max_ind_set;  //maximal independent set filtration structure
  int currLevel;   //MISF level currently being processed
  int itr;         //independent iterator
  int roundsCtr = dg->roundsCtr;  // rounds processed for current level
  int rounds = dg->rounds;  //target rounds for current level
  size_tt vert;
  int loop;         // if set to true (1), algorithm can loop through all rounds without stopping to display results
  int displayPar;   //flag indicating whether interactive display is used
  int createList;   //indicates whether processing has completed and final graph is ready
  size_tt *vertDepth;  //stores the smallest level each vertex appears at
  Graph *graph;
  graph = G;

  dim = dg->dim;
	edge = dg->edge;

  tinit = edge/6;
  numOfVert = graph->numOfVert;
  numOfInitVert = dg->numOfInitVert;
  max_ind_set  = dg->misf;
  misfDepth = max_ind_set->depth;
  misfSize = max_ind_set->size;
  misf = max_ind_set->filt;
 
  pos = dg->pos;
  disp = dg->disp;
  oldDisp = dg->oldDisp;
  dispNorm = dg->dispNorm;
  oldDispNorm = dg->oldDispNorm;
  heat = dg->heat;
  oldCos = dg->oldCos;

  displayPar = dg->displayPar;

  /* initialize the seed for the random function */
  sfast_Rand(0);

  nbr = dg->nbr;
  nbrs = dg->nbrs;
  
  currLevel = dg->currLevel;
  vertDepth = dg->vertDepth;

  debug3("drawgraph: roundsCtr %d rounds %d\n",roundsCtr, rounds);
  loop = 1; //True

  /*****************************************************************
   *   This section applies the full Fruchterman Reingold algorithm
   *   to the graph.  GRIP is not used here
   ******************************************************************/	 
  if(plot_all_vert)  
  {
    if (roundsCtr==0)
  	{
	    int i;
	    for (i = 0; i < numOfVert; i ++)
	    {
	      //initialize the heat; heat_fraction is specified by the user
	      heat[i] = edge * heat_fraction;
	    }
	  }

    //processing all vertices for finalRounds number of rounds
    while(roundsCtr < finalRounds && loop)
	  {
	  size_tt i;
	  coord_t totalDisp = 0;
    roundsCtr ++;

	    if (displayPar)
	      loop = 0;
	  
  	  //process each vertex and calculate displacement
	    for(i = 0; i < numOfVert; i ++)
	    {
	      FR_spring_full(misf[i], graph);

	      // calculate the final displacement
	      if ((s - 1.0) >= roundsCtr * 0.08)
		      update_Local_Temp_v2(i, r, s - (roundsCtr * 0.08));
	      else
		      update_Local_Temp_v2(i, r, 2.0);

	      point_duplicate(&oldDisp[misf[i]], &disp[misf[i]]);
	      oldDispNorm[misf[i]] = dispNorm[misf[i]];
	      point_mult_eq(&disp[misf[i]], (coord_t)heat[i]);  
	      debug2(" disp[%d]=",misf[i]);
	      print_point(&disp[misf[i]]);
	      if(dispNorm[misf[i]]!=0)
		    {
		      point_div_eq(&disp[misf[i]], dispNorm[misf[i]]);
		    }
	      totalDisp += dispNorm[misf[i]];
	      debug3("dispNorm[%d]=%d\n",misf[i],dispNorm[misf[i]]);
	      debug2("final disp[%d]=",misf[i]);
	      print_point(&disp[misf[i]]);
	      debug2("heat = %d\n", heat[0]);
	    }

	    //if no displacement takes place, done processing
	    if (totalDisp==0)
	      roundsCtr = finalRounds;

	    for(i = 0; i < numOfVert; i++)
	    {
	      point_plus_eq(&pos[misf[i]], &disp[misf[i]]);
	      debug2("pos[%d]=",misf[i]);
	      print_point(&pos[misf[i]]);
	    }

	  }
  }

  /************************************************************************
   *   This section uses the GRIP algorithm.  It first places the vertices
   *   with respect to 3 closest neighbors and then applies KK and FR
   *   refinement algorithms
   ***********************************************************************/
  else  while(currLevel >= 0 && loop)
  {
    graphDist *closeVert;
    debug3("DrawGraph: current level is %d with size %d\n",currLevel,misfSize[currLevel]);

    if (displayPar)
	    loop = 0;

      /* itr = vertex at which we stopped processing last time */

	  itr = misfSize[currLevel];

      /* highest level, initialize positions of  the first initNum of  vertices */
    if (currLevel == (misfDepth - 1) && dg->firstRound)
	  {
	    size_tt vert[numOfInitVert];
	    size_tt i;
	    Point * baricenter = construct_Point(0, 0, 0, 0, dim);
	    for(i = 0; i < numOfInitVert; i ++)
	      vert[i] = misf[i];
	 
	    dg->firstRound = 0;

  	  /* make all vertices  random points and find their neighbors*/
	    for(i = 0; i < numOfInitVert; i ++)
	    {
	      size_tt currVert = misf[i];
	      point_duplicate(&pos[vert[i]], random_point());
	      debug3("pos[%d](vert%d)=",i,vert[i]);
	      print_point(&pos[vert[i]]);
//	      if(graph->weightList==0)
//					closeVert = nbr_bfs(graph, currVert, nbrs, nbr, vertDepth);
//	      else
					closeVert =nbr_dijkstra(graph,currVert,nbrs,nbr,vertDepth); // graph now is always weighted!
	    }

	    print_point(baricenter);
	    for(i = 0; i < numOfInitVert; i ++)
	    {
	      debug3("pos[%d](vert%d)=",i,vert[i]);
	      print_point(&pos[vert[i]]);
	    }

	    /* center all the vertices around the origin */
  	  for(i = 0; i < itr; i++)
	      point_plus_eq(baricenter, &pos[misf[i]]);
	    point_div_eq(baricenter, 3);
  	  for(i = 0; i < itr; i++)
	      point_minus_eq(&pos[misf[i]], baricenter);


	    debug("baricenter:");
  	  print_point(baricenter);
	    for(i = 0; i < numOfInitVert; i ++)
	    {
	      debug3("pos[%d](vert%d)=",i,vert[i]);
	      print_point(&pos[vert[i]]);
	    }
  	}
      
    if(roundsCtr == rounds) /* a whole level has been processed, need to reinitialize for next level */
  	{
	    int i;
	    debug2("itr %d\n",itr);
  	  for (i = 0; i < itr; i ++)
	    {
	      free(nbrs[misf[i]][currLevel]);
	    }
	  
	    /* Reset all values for next level */
	    currLevel --;
  	  if (itr != numOfVert) /* this is not last level */
	    {
	      roundsCtr = 0;
	      rounds = sched3(misfSize[currLevel - 1], 0, initRounds, numOfVert, finalRounds);
	    }
  	  else  /* last level was just processed */
	      break;

  	  for(vert=0; vert < itr; vert++)
	    {
	      heat[misf[vert]] = tinit;
	    }

	  /* place each vertex at next level that isn't in the current level*/
	    while(itr < misfSize[currLevel])
	    {
	      size_tt currVert = misf[itr];
	      size_tt count;
	      
	      //create the neighbor set for a new vertex
//	      if(graph->weightList==0)
//		      closeVert = nbr_bfs(graph, currVert, nbrs, nbr, vertDepth);
//	      else
      	closeVert =nbr_dijkstra(graph,currVert,nbrs,nbr,vertDepth); // graph now is always weighted

//	      printf("Close Vertices for %d:\n",currVert);
//	      for(vert = 0; vert < 3; vert ++)
//      		printf("Vertex %d with distance %f\n",closeVert[vert].vert,closeVert[vert].dist);

	      // place new vertex at baricenter of 3 closest neighbors
	      point_duplicate( &pos[currVert], &pos[closeVert[0].vert]);
	      point_plus_eq(&pos[currVert], &pos[closeVert[1].vert]);
	      point_plus_eq(&pos[currVert], &pos[closeVert[2].vert]);
	      point_div_eq(&pos[currVert], 3);
	
	      // set oldDisp vector to the average of
              // disp vectors of the closest centers
        point_plus_eq(&oldDisp[currVert], &oldDisp[closeVert[0].vert]);
        point_plus_eq(&oldDisp[currVert], &oldDisp[closeVert[1].vert]);
        point_plus_eq(&oldDisp[currVert], &oldDisp[closeVert[2].vert]);
        point_div_eq(&oldDisp[currVert], 3);
        oldDispNorm[currVert] = norm(&oldDisp[currVert]);

	      count = 0;

	      //perform short refinement of the new vertex
	      while(count++ < 3)
    		{
		      KK_spring_local(currVert, closeVert, 3);
    		  update_Local_Temp(currVert);
		      point_duplicate(&oldDisp[currVert], &disp[currVert]);
    		  oldDispNorm[currVert] = dispNorm[currVert];
		      point_mult_eq(&disp[currVert], (coord_t)heat[currVert]);
    		  if(dispNorm[currVert])
		        point_div_eq(&disp[currVert], dispNorm[currVert]);
    		  point_plus_eq(&pos[currVert], &disp[currVert]);
		    }

	      debug2("------pos[%d]=",currVert);
	      print_point(&pos[currVert]);
	      itr ++;
	    }
  	}

    debug2("rounds: %d\n",rounds);

    //perform 'rounds' number of refinement rounds for current level
    if (roundsCtr ++ < rounds) /* do refinement */
	  {
  	  size_tt i;
	    for(i = 0; i < itr; i++)
	    {
	      //perform local force-directed modifications
	      
	      if(currLevel < FR_levels)
		    {
    		  //use either the localized FR function specified in GRIP, or
		      //the original FR function
    		  if (FR_full)
		        FR_spring_full(misf[i], graph);
		      else
    		    FR_spring(misf[i], nbrs[misf[i]][currLevel], currLevel, graph);
		    }
	      else
      		KK_spring(misf[i], nbrs[misf[i]][currLevel], currLevel);

	      //call the cooling function
	      update_Local_Temp(misf[i]);

	      //store all the values for next round
	      point_duplicate(&oldDisp[misf[i]], &disp[misf[i]]);
	      oldDispNorm[misf[i]] = dispNorm[misf[i]];
	      point_mult_eq(&disp[misf[i]], (coord_t)heat[misf[i]]);  
	      debug2(" disp[%d]=",misf[i]);
	      print_point(&disp[misf[i]]);
	      if(dispNorm[misf[i]])
    		{
    		  point_div_eq(&disp[misf[i]], dispNorm[misf[i]]);
    		}

	      debug3("dispNorm[%d]=%d\n",misf[i],dispNorm[misf[i]]);
	      debug2("final disp[%d]=",misf[i]);
	      print_point(&disp[misf[i]]);
	    }
	    for(i = 0; i < itr; i++)
	    {
	      point_plus_eq(&pos[misf[i]], &disp[misf[i]]);
	      debug2("pos[%d]=",misf[i]);
	      print_point(&pos[misf[i]]);
	    }
  	}

  } //end while(currLevel >= 0) {...

  if( loop && dg->createListSwitch)
  {
    createList = 1;
    dg->createListSwitch = 0;
  }
  else
    createList = 0;


  dg->currLevel = currLevel;
  dg->createList = dg->createList || createList;  //make sure if it's true, it stays true
  dg->roundsCtr = roundsCtr;
  dg->rounds = rounds;
}


/* DETERMINES NUMBER OF NEIGHBORS AT EACH LEVEL THAT ARE TO BE FOUND FOR EACH VERTEX */
void set_nbr_size()
{
    double AvgDeg = 0;
    int maxCxty = 0;
    int initCxty = 10000;
    int smallLevel = 0;
    size_tt itr;
    size_tt vert;
    //computing Avg(deg(G)), maxCxty
    for(vert = 0; vert < numOfVert; vert++)
        AvgDeg += graph->adjList[0][vert];
        
    maxCxty = (unsigned long)AvgDeg;
    if(maxCxty < initCxty)
      maxCxty = initCxty;
    AvgDeg /= numOfVert;
        
     
    // COMPUTING smallLevel i.e. a level so that for each
    // misf level l >= smallLevel
    // misfSize[l] * misfSize[l] <= initCxty
    itr = 0;
    while( itr < misfDepth && misfSize[itr] )
    {
      if( (double)misfSize[itr] * misfSize[itr] - initCxty <= 0)
      {
        smallLevel = itr;
        break;
      }
      itr++;
    }
        
    // compute the number of vertices nbr[i] for local beautification
    // at level i, for each i.
    nbr = (size_tt *)malloc( misfDepth * sizeof(size_tt));
    itr = 0;
    while( itr < misfDepth && misfSize[itr] )
    {
      if( itr >= smallLevel )
	      nbr[itr] = max(misfSize[itr]-1, numOfInitVert - 1);
      else
    	{
        nbr[itr] = min((unsigned long)(sched(itr,0,2,10000,1) *
              maxCxty/misfSize[itr]),(unsigned long)(misfSize[itr]-1));
      }
      itr++;
    }

    // some simple nbr[] tune up
    nbr[0] = min(2*nbr[0], numOfVert-1);
    //for(itr = 0; itr <misfDepth; itr ++)
    //printf("nbr[%d]=%d\n",itr,nbr[itr]);
}



///////////////////////////////////////////////////////////////
//
//     Scheduling function that computes the number of rounds
//     for next level, given results of previous level
//
///////////////////////////////////////////////////////////////

double sched(size_tt x, size_tt max, size_tt maxVal, size_tt min, size_tt minVal)
{      
  if( x <= max )
    return (double)maxVal;
  else if( max <= x && x <= min )
    {
      return ((minVal - maxVal)/(double)(min-max))*(x - max) + maxVal;
    } 
  else
      return (double)minVal;

}

// exponentially decaying schedule function
// it assumes that we are working in the interval [0, min]
// that is max = 0
size_tt sched3(size_tt x, size_tt max, size_tt maxVal, size_tt min, size_tt minVal)
{
  if( x <= max )
    return maxVal;
  else if( max <= x && x <= min )
    {
      double k = -log((double)minVal/maxVal)/min;
      return (size_tt)(ceil(maxVal * exp( -k * x )));
    } 
  else
    return minVal;
}


/////////////////////////////////////////////////////////////////////
//  Creates a new point with random placement within limited boundary
/////////////////////////////////////////////////////////////////////
Point *random_point()
{
  Point *p;
  coord_t boxSize = (coord_t)(edge * diam * 0.5);
  coord_t box2Size = 2 * boxSize + 1;
  debug2("diam: %d\n",diam);
  p   =      construct_Point((coord_t)fmod(fast_Rand(),box2Size) - boxSize,
                             (coord_t)fmod(fast_Rand(),box2Size) - boxSize,
                             (coord_t)fmod(fast_Rand(),box2Size) - boxSize,
                             (coord_t)fmod(fast_Rand(),box2Size) - boxSize, dim);
  /*Point *p = construct_Point(0,0,0,0,dim);*/
  debug("random point = ");
  print_point(p);
  return p;
}


//**************************************************************
//
//    KK_spring_local()
//
//    local beautification used for determining initial positions
//    of "new vertices"
//
//    the force is calculated using the whole bfs tree
//
//**************************************************************
void KK_spring_local(size_tt vert,
                     graphDist *closeVert,
                     size_tt size)
{
    coord_t _norm2;   // its norm squared
    Point *vect = construct_Point(0,0,0,0,dim);
    size_tt i;
    
    debug("Entering KK_spring_local()\n");
    set_to_zero(&disp[vert]);
    for(i = 0; i < size; i++)
      {
        set_to_zero(vect);
	      debug3("looking at close vert %d with dist %d\n",closeVert[i].vert,closeVert[i].vert);
        point_minus(vect, &pos[closeVert[i].vert], &pos[vert]);
        _norm2 = norm2(vect);
        // in the original * 1 was replaced with * edge * edge:
        point_mult_eq(vect,  (_norm2/(closeVert[i].dist * closeVert[i].dist * 1 )-1));
        point_plus_eq(&disp[vert], vect);

      }
    dispNorm[vert] = norm(&disp[vert]);
    debug2("kk_local disp[%d]=",vert);
    print_point(&disp[vert]);

    //free memory
    free(vect);
    debug("Leaving KK_spring_local()\n");
}

//**************************************************************************
//
//         Local refinement method that uses all of the vertex neighbors
//
//**************************************************************************
void KK_spring(const size_tt vert,
	             graphDist *vertNbrs,
	             size_tt misfLayer)
{
  size_tt overt; // other vertex
  double dist2;// square of the graph theoretic dist between vert and overt
  double _norm2;// square of the Eucleadian distance between vert and overt
  Point *vect = construct_Point(0,0,0,0,dim);
  size_tt i;
  double _norm;

  debug("Entering KK_spring()\n");
    
  set_to_zero(&disp[vert]);
  for(i = 0; i < nbr[misfLayer]; i ++)
  {
    overt = vertNbrs[i].vert;
    debug3("looking at nbr %d with dist %f\n",overt,vertNbrs[i].dist);
    dist2 = vertNbrs[i].dist * vertNbrs[i].dist;
    if(dist2)
	  {
	    set_to_zero(vect);
  	  point_plus_eq(vect, &pos[overt]);
	    point_minus_eq(vect, &pos[vert]);
	    _norm2 = (double)norm2(vect);
	    // in the original version * 1 was replaced with * edge * edge
  	  point_mult_eq(vect, (_norm2 / (dist2 * 1) - 1));
 	    point_plus_eq(&disp[vert], vect);
	    debug2("disp[%d]=",vert);
	    print_point(&disp[vert]);
  	}
  }
        
  _norm = norm(&disp[vert]);
  dispNorm[vert] = (_norm);
        
  if(dispNorm[vert]!=0)
  {
    point_mult_eq(&disp[vert], edge / _norm);
    dispNorm[vert] = norm(&disp[vert]);
  }
  debug2("disp[%d]=",vert);
  print_point(&disp[vert]);

  //free memory
    free(vect);
    debug("Leaving KK_spring()\n");
}


//************************************************************************
//
//    Fruchterman-Reingold force calculation
//
//************************************************************************
void FR_spring(const size_tt vert,
               graphDist *vertNbrs,
               size_tt misfLayer,
               Graph *graph)
{
    size_tt overt; // other vertex
    double _norm2;// square of the Eucleadian distance between vert and overt
    double _norm;
    size_tt adjVert;
    size_tt deg;
    size_tt i;
    double fedge2 = 0.05 * edge * edge;
    coord_t weightOfEdge = edge;

    Point *vect = construct_Point(0,0,0,0,dim);

    debug("Entering FR_spring()\n");

    set_to_zero(&disp[vert]);

    // attractive force calculation
    deg = graph->adjList[0][vert];
    for (adjVert = 0; adjVert < deg; adjVert++)
    {
    	overt = graph->adjList[vert+1][adjVert];
      if(nbrs[overt]==0) // position is not initialized
        continue;
    	if(graph->weightList != 0)
    		weightOfEdge = graph->weightList[vert+1][adjVert];
    	debug2("  looking at adj vert %d \n",overt);
    	set_to_zero(vect);
    	point_plus_eq(vect, &pos[overt]);
    	point_minus_eq(vect, &pos[vert]);
    	_norm2 = norm2(vect);
    	point_mult_eq(vect, (_norm2/(weightOfEdge*weightOfEdge)));
    	point_plus_eq(&disp[vert], vect);
    	debug2("attr: disp[%d]=",vert);
    	print_point(&disp[vert]);
     }

    // repulsive force calculation
    for(i = 0; i < nbr[misfLayer]; i ++)
    {
    	overt = vertNbrs[i].vert;
    	debug3("  looking at nbr %d with dist %d\n",overt,*ptr);
    	set_to_zero(vect);
    	point_plus_eq(vect, &pos[vert]);
    	point_minus_eq(vect, &pos[overt]);
    	_norm2 = norm2(vect);
    	if(_norm2==0)
      {
	      int x = rand() % 2;
	      int y = rand() % 2;
	      int z = rand() % 2;
  	    _norm2 = 0.01;

	      initialize_Point(vect,x,y,z,0,dim);
	    }
//	    fedge2=(*ptr)*(*ptr); //changed
    	point_mult_eq(vect,  (fedge2/_norm2));
    	point_plus_eq(&disp[vert], vect);
    	debug2("rep: disp[%d]=",vert);
    	print_point(&disp[vert]);
    }
    _norm = norm(&disp[vert]);
    dispNorm[vert] = _norm;
    debug3("dispNorm[%d]=%d\n",vert,(int)dispNorm[vert]);

    if(dispNorm[vert])
    {
    	point_mult_eq(&disp[vert], edge/_norm);
    	dispNorm[vert] = norm(&disp[vert]);
    }
    debug3("dispNorm[%d]=%d\n",vert,(int)dispNorm[vert]);
    debug2("disp[%d]=",vert);
    print_point(&disp[vert]);

    //free memory
    free(vect);

    debug("Leaving FR_spring()\n");
}


//************************************************************************
//
//   FR force calculation.
//
//   This version uses all the vertices that are in the current level,
//   rather than just neighbors for calculation of repulsive forces.
//
//***********************************************************************    
void FR_spring_full(const size_tt vert, Graph *graph)
{
    size_tt overt; // other vertex
    double _norm2;// square of the Eucleadian distance between vert and overt
    double _norm;
    size_tt adjVert;
    size_tt deg;
    size_tt i;
    double fedge2 = 0.1 * edge * edge;
    coord_t weightOfEdge = edge;

    Point *vect = construct_Point(0,0,0,0,dim);

    debug("Entering FR_spring_full()\n");

    set_to_zero(&disp[vert]);

    // attractive force calculation
    deg = graph->adjList[0][vert];
    for (adjVert = 0; adjVert < deg; adjVert++)
    {
    	overt = graph->adjList[vert+1][adjVert];
    	debug2("  looking at adj vert %d \n",overt);
    	set_to_zero(vect);
    	point_plus_eq(vect, &pos[overt]);
    	point_minus_eq(vect, &pos[vert]);
    	_norm2 = (double)norm2(vect);
    	if(graph->weightList!=0)
    	  weightOfEdge=graph->weightList[vert+1][adjVert];
    	point_mult_eq(vect, (double)(_norm2/(weightOfEdge*weightOfEdge)));
    	point_plus_eq(&disp[vert], vect);
    	debug2("attr: disp[%d]=",vert);
    	print_point(&disp[vert]);
    }

    // repulsive force calculation
    // this version of Fruchterman-Reingold algorithm uses all vertices 
    // to calculate repulsive force
    for(i = 0; i < numOfVert; i += 1)
    {
    	if (i != vert)
	    {
	      overt = i;
  	    debug2("  looking at nbr %d\n",overt);
	      set_to_zero(vect);
	      point_plus_eq(vect, &pos[vert]);
	      point_minus_eq(vect, &pos[overt]);
  	    _norm2 = (double)norm2(vect);
	      if(_norm2==0)
	      {
          int x = rand() % 2;
          int y = rand() % 2;
          int z = rand() % 2;
          _norm2 = 0.01;
          initialize_Point(vect,x,y,z,0,dim);
	      }

	      point_mult_eq(vect,  (double)(fedge2/_norm2));
	      point_plus_eq(&disp[vert], vect);
  	    debug2("rep: disp[%d]=",vert);
	      print_point(&disp[vert]);
	    }
    }
    _norm = norm(&disp[vert]);
    dispNorm[vert] = _norm;
    debug3("dispNorm[%d]=%d\n",vert,(int)dispNorm[vert]);

    if(dispNorm[vert]!=0)
    {
      point_mult_eq(&disp[vert], edge/_norm);
      dispNorm[vert] = norm(&disp[vert]);
    }
    debug3("dispNorm[%d]=%d\n",vert,(int)dispNorm[vert]);
    debug2("disp[%d]=",vert);
    print_point(&disp[vert]);

    //free memory
    free(vect);

    debug("Leaving FR_spring_full()\n");

}


//**************************************************************
//
//	update_Local_Temp
//
//      updating heat[vert]
//      looking at the ratio oldNorm/newNorm and multiplying
//      old heat by this value and cos of the angle
//
//**************************************************************
void update_Local_Temp( size_tt vert )
{
    coord_t temp = heat[vert];
    coord_t normOldDisp = oldDispNorm[vert];
    coord_t normNewDisp = dispNorm[vert];
    
    if( normOldDisp != 0 && normNewDisp != 0 )
      {
        coord_t scalProd = point_scalar_product(&disp[vert], &oldDisp[vert]);
        double cos = (double)scalProd/(normOldDisp * normNewDisp);
        float r = 0.15;
        float s = 3;
        if( oldCos[vert] * cos > 0 )
            temp += (coord_t)(temp * s * cos * r);
        else
            temp += (coord_t)(temp * cos * r);
        
        oldCos[vert] = cos;
        heat[vert] = temp;
    }
    debug3("Heat[%d]=%f\n",vert,temp);
}


//**************************************************************
//
//	update_Local_Temp_v2
//
//      updating heat[vert]
//      looking at the ratio oldNorm/newNorm and multiplying
//      old heat by this value and cos of the angle
//
//**************************************************************
void update_Local_Temp_v2( size_tt vert, float r, float s )
{
    coord_t temp = heat[vert];
    coord_t normOldDisp = oldDispNorm[vert];
    coord_t normNewDisp = dispNorm[vert];
    
    if( normOldDisp != 0 && normNewDisp != 0 )
      {
        coord_t scalProd = point_scalar_product(&disp[vert], &oldDisp[vert]);
        double cos = (double)scalProd/(normOldDisp * normNewDisp);

        if( oldCos[vert] * cos > 0 )
            temp += (coord_t)(temp * s * cos * r);
        else
            temp += (coord_t)(temp * cos * r);
        
        oldCos[vert] = cos;
        heat[vert] = temp;
    }
    debug3("Heat[%d]=%f\n",vert,temp);
}


/*  Prints out the coordinates of the vertex */
void print_point(Point *p)
{
  int i;
  debug2("point:(%f",p->coord[0]);
  for(i = 1; i < p->dim; i ++)
    debug2(",%f",p->coord[i]);
  debug(")\n");
}


/*  Free all memory occupied by the drawGraph structure*/
void free_DrawGraph(DrawGraph *dgraph)
{
  int vert;

  if (!dgraph)
    return;
  
  debug("Entering free_DrawGraph\n");
  for(vert=0; vert < numOfVert; vert++)
    {
      free_Point(&dgraph->pos[vert]);
      free_Point(&dgraph->disp[vert]);
      free_Point(&dgraph->oldDisp[vert]);

      // The nbrs[] array is only used by the GRIP algorith, not FR
      if (!plot_all_vert)
        free(nbrs[vert]);
    }
  free(dgraph->pos);
  free(dgraph->disp);
  free(dgraph->oldDisp);
  free(dgraph->dispNorm);
  free(dgraph->oldDispNorm);
  free(dgraph->heat);
  free(dgraph->oldCos);
  destroy_misf(dgraph->misf);
  free(dgraph->nbr);
  free(dgraph->vertDepth);
  free(dgraph->nbrs);
  free(dgraph);
  debug("Leaving free_DrawGraph\n");
}

coord_t getAverageWeightOfEdge(Graph* aG, int numComponents)
{
int i,j,k;
coord_t sumOfWeight=0;
size_tt numOfWeight=0;
	for(i=0;i<numComponents;i++)
		for(j=0;j<aG[i].numOfVert;j++)
			for(k=0;k<aG[i].adjList[0][j];k++)
			{
				sumOfWeight+=aG[i].weightList[j+1][k];
       	numOfWeight++;
    	}
  printf("average weight: %f\n",  sumOfWeight/numOfWeight);
  return sumOfWeight/numOfWeight; // actually we have counted each weight twice, but it doesn't matter
}
