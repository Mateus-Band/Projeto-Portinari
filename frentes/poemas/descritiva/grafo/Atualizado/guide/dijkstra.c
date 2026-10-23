#include "dijkstra.h"


//*********************************************************************************
//
//   calculate set of neighbors for the specified vertes
//
// UPDATE nbr[] USING BFS TREES 
// RETURN: ARRAY OF 3 CLOSEST VERTICES TO root 
//
//*********************************************************************************
graphDist *nbr_dijkstra(Graph *graph, size_tt root, graphDist ***nbrs, size_tt *nbr, size_tt *vertDepth)
{
  int count;
  int i;
  int *color = (int *)malloc(graph->numOfVert * sizeof(int));  //0 is white, -1 is gray, 1 is black
  PQueue *vertQ = new_PQ(graph->numOfVert);  //manages gray vertices & their dist from root
  coord_t distVertToRoot,distVertToNbr;
  size_tt vert;
  size_tt bottomNbrsLayer = 0; // the index i of the first free nbrs[root][i]

  // memory allocation for rootNbrs and nbrCounter
  size_tt *nbrCounter = (size_tt *)malloc((vertDepth[root] + 1) * sizeof(size_tt));

  /* Setting up variables for determining set of 3 closest neighbors for placement of vertex 'root' */
  size_tt numOfCloseVert = 3; /* we allocate an extra index for storing the HEIGHT OF BFS */
  graphDist *closeVert;
  size_tt closeVertItr = 0;
  int closeVertDone = 0;
  size_tt deg;

  /* closeVert[x].pos will hold vertex at pos x*/
  /* closeVert[x].dist will hold distance of closeVert[x][0] from root*/
  closeVert = (graphDist *)malloc(numOfCloseVert * sizeof(graphDist));

  debug("Entering nbr_dijkstra\n");

  nbrs[root] = (graphDist **)malloc((vertDepth[root]+2) * sizeof(graphDist*));
  for(i=0; i <= vertDepth[root]; i++)
  {
    nbrs[root][i] = (graphDist *)malloc((nbr[i]) * sizeof(graphDist));
    nbrCounter[i] = 0;
  }
  for (count = 0; count < graph->numOfVert; count ++)
  {
    color[count] = 0; //0 is white
  }

  enqueuePQ(vertQ,root,0);

//  printf("root: %d\n",root);

  while(pqueue_size(vertQ) > 0 &&  (!closeVertDone || (bottomNbrsLayer <= vertDepth[root])))
  {
	  vert = dequeueMin(vertQ,&distVertToRoot);
	  if(color[vert] == 1)
	  	continue;
	  if(vert != root)
	  {
//	  	printf("distance to %d: %f\n",vert,distVertToRoot);
	  	for(i = bottomNbrsLayer; i <= min(vertDepth[vert], vertDepth[root]); i++)
 			{
 		 		if(nbrCounter[i] < nbr[i])
 				{
 			//debug4("%d's neighborhood: %d (level %d) added\n",root,adjVert,i);
 			  	nbrs[root][i][nbrCounter[i]].vert = vert;
 		    	nbrs[root][i][nbrCounter[i]].dist = distVertToRoot;
 		    	nbrCounter[i]++;
 		  	}
 		 		else
 		   		bottomNbrsLayer = i+1;
   		}

      if( !closeVertDone && vertDepth[vert] > vertDepth[root])
     	{
      	//debug4("vertDepth[%d]=%d, vertDepth[root]=%d\n",adjVert, vertDepth[adjVert],vertDepth[root]);
        closeVert[closeVertItr].dist = distVertToRoot;
        closeVert[closeVertItr].vert = vert;	
        closeVertItr++;
    		if( closeVertItr == numOfCloseVert )
        	closeVertDone = 1;
     	}
    }
    color[vert] = 1;  //vertex is processed, set color to black

    deg = graph->adjList[0][vert];
    for(count = 0; count < deg; count ++)
 		{
   		size_tt adjVert = graph->adjList[vert+1][count];

   		//debug2("next adjacent vertex is %d\n",adjVert);
   		if (color[adjVert] != 1) //if color is white
     	{
	   		distVertToNbr = graph->weightList[vert+1][count];
       	//add the new vertex and its distance from root to queue
       	enqueuePQ(vertQ, adjVert,distVertToRoot+distVertToNbr);
  	  }
   	}
  }
 	debug("Leaving nbr_dijkstra\n");
  
  free_pqueue(vertQ);
  free(color);
  free(nbrCounter);
  return closeVert;
}








