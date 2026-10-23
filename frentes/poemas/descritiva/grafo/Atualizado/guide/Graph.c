
#include <assert.h>

#include "Graph.h"



//**************************************************************
//
//      tree()
//
//**************************************************************
Graph * tree(size_tt depth, size_tt base)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  size_tt total = 0;
  int i,j,v;
  int first_leaf;
  size_tt ** adjList;
  size_tt numOfVert;

  assert(graph!=NULL);

  //make sure that the tree is at least binary at depth 2

  if (depth<2) 
    depth=2;
  if (base<2) 
    base=2;
  
  //compute the total number of vertices of the tree
  for(i = 0;i < depth;i++)
    total = total + (size_tt)pow(base,i);
  numOfVert = total;

  adjList = (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
  assert(adjList!=NULL);
  adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
  assert(adjList[0]!=NULL);
  adjList[0][0]=base;
  adjList[1] = (size_tt *)malloc(base * sizeof(size_tt));
  assert(adjList[1]!=NULL);
  
  first_leaf = numOfVert - (size_tt)pow(base,depth-1);

  for(i = 1;i < first_leaf; i++) 
  {
    adjList[0][i]=base+1;
    adjList[i+1] = (size_tt *)malloc((base + 1) * sizeof(size_tt));
    assert(adjList[i+1]!=NULL);
  }
  
  for(i = first_leaf; i < numOfVert; i++) 
  {
    adjList[0][i]=1;
    adjList[i+1] = (size_tt *)malloc(sizeof(size_tt));
    assert(adjList[i+1]!=NULL);
  }

  //setting adjacencies for the root  
  for (i = 0; i < base; i++)
    adjList[1][i]=i+1;

  //setting adjacencies for the internal nodes

  for(i = 1 ; i < first_leaf; i++)
  {
    adjList[i+1][0]=(size_tt)((i-1)/base);
    for(j = 1; j <= base; j++)
      adjList[i+1][j]=base*i+j;
  }
		 
  //setting adjacencies for the leafs
  for(i = first_leaf; i < numOfVert; i++) 
    adjList[i+1][0]=(size_tt)((i-1)/base);

        debug("adjList: \n");
        for(v = 0; v < numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }
  graph->numOfVert = numOfVert;
  graph->adjList = adjList;
  graph->weightList=NULL;
  return graph;
}

//*****************************************************************
//
//  creates a twisted torus
//  t1 specifies twist in the x direction
//  t2 specifies twist in the y direction
//
//******************************************************************

Graph * twistedTorus( size_tt h, size_tt w,  size_tt t1, size_tt t2) {

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  size_tt numOfVert = w * h;
  size_tt ** adjList;
  int i,v,j;
  size_tt a,b,la,lb;
  adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
  adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

  assert(graph!=NULL);
  assert(adjList!=NULL);
  assert(adjList[0]!=NULL);

  //setting degrees for all vertices to 4 and allocating memory
  for(i=0;i<numOfVert;i++) {
    adjList[0][i]=0;
    adjList[i+1] = (size_tt *)malloc(4 * sizeof(size_tt));
    assert(adjList[i+1]!=NULL);
  }

  
  for (a = 0; a < h; a++) {
    for (b = 0; b < w; b++) {
      // For each vertex match with left neighbor
      la = (a + t1) % h;
      lb = (b + 1) % w;
      addEdge(adjList, a+b*h, la+lb*h);

      // and bottom neighbor
      la = (a + 1) % h;
      lb = (b + t2) % w;
      addEdge(adjList, a+b*h, la+lb*h);
    }
  }

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }
  graph->numOfVert = numOfVert;
  graph->adjList = adjList;
  graph->weightList=NULL;
  return graph;
}


//**************************************************************
//
//	torus()
//
//**************************************************************
Graph *  torus( size_tt h, size_tt w )
{
  /*    debug("Entering torus"); */
    
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    size_tt numOfVert = w * h;
    size_tt ** adjList;
    int i;
    int j;
    int twist;
    size_tt v;

    assert(graph!=NULL);

    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    assert(adjList!=NULL);

    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
    assert(adjList[0]!=NULL);

    //setting degrees for all vertices to 4 and allocating memory
    for(i=0;i<numOfVert;i++) 
    {
      adjList[0][i]=4;
      adjList[i+1] = (size_tt *)malloc(4 * sizeof(size_tt));
      assert(adjList[i+1]!=NULL);
   }


    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++)
        {
            adjList[ (v + i*h) + 1][0] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][1] = v-1 + i*h;
            adjList[ (v + i*h) + 1][2] = v + (i+1)*h;
            adjList[ (v + i*h) + 1][3] = v+1 + i*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for( i=1; i < w-1; i++){
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

    // making the leftmost and rightmost columns adjacent
      for(j=0;j<h;j++){
	int qqq=(w-1)*h+j;
	adjList[j+1][adjList[0][j]-1]=qqq;
	adjList[qqq+1][adjList[0][j]-1]=j;
        }

      // making the top and bottom row adjacent
      twist=1;
      for(j=0;j<numOfVert;j+=h){
          int tw=(h*w+j+h*twist-1)%(h*w);
	  adjList[j+1][3]=tw;
	  adjList[tw+1][3]=j;
      }

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }

    debug("Leaving torus");


  graph->numOfVert = numOfVert;
  graph->adjList = adjList;
  graph->weightList=NULL;
  return graph;
}


//**************************************************************
//
//	Method name : pow2
//
//	Description : sequencial doubling of a graph
//
//**************************************************************
Graph * pow2(size_tt exp, Graph *graph)
{

  /*    debug("Entering pow2"); */

    size_tt i;
    for(i = 0; i < exp; i++)
      graph = double_Graph(graph);


    debug("Leaving pow2");

    return graph;
}

//**************************************************************
//
//	Method name : hypercube
//
//	Description : cube of dimension dim
//
//**************************************************************
Graph * hyper_Cube( size_tt dim )
{

  /*    debug("Entering hypercube"); */

    Graph *graph = path_Graph( 2 );
    size_tt i;
    for(i = 0; i < dim - 1; i++)
        graph = double_Graph(graph);

    debug("Leaving hypercube");

	return graph;
}

//**************************************************************
//
//	Method name : double_Graph
//
//	Description : create an adjacency list of a graph obtained
//      from the given one G by creating a clone G' of G and joining
//      each vertex of G' with its twin brother in G.
//
//**************************************************************
Graph * double_Graph(Graph *graph)
{

  /*    debug("Entering double_Graph"); */

    size_tt initSize = graph->numOfVert;
    int numOfVert = 2 * initSize;
    size_tt vert;
    size_tt adjVert;
    size_tt **aList;
    aList =  (size_tt **)malloc((numOfVert+1) * sizeof(size_tt *));
    aList[0] = (size_tt *)malloc(numOfVert * sizeof(size_tt));

    assert(aList!=NULL);
    assert(aList[0]!=NULL);

     // updating degrees
    for( vert = 0; vert < initSize; vert++)
    {
        aList[ 0 ][ vert ] = graph->adjList[0][ vert ] + 1;
        aList[ 0 ][ vert + initSize ] = aList[0][ vert ];
    }

    // allocating memory and setting adjacency lists of all vertices
    for( vert = 0; vert < initSize; vert++)
    {// memory allocation
        aList[ vert+1 ] = (size_tt *)malloc(aList[0][ vert ] * sizeof(size_tt));
	assert(aList[vert+1]!=NULL);
        aList[ vert+1+initSize ] = (size_tt *)malloc(aList[0][ vert ] * sizeof(size_tt));
	assert(aList[vert+1+initSize]!=NULL);
#if DEBUG
        debug("vert = " << vert );
#endif
            // enlisting a twin brother
        aList[ vert+1 ][0] = initSize + vert;
        aList[ vert+1+initSize ][0] = vert;

        for(adjVert = 1; adjVert < aList[0][ vert ];
            adjVert++)
        {
#if DEBUG
            debug("1st, adjVert = " << adjVert );
#endif
            aList[ vert+1 ][ adjVert ] = graph->adjList[ vert+1 ][ adjVert-1];

#if DEBUG
            debug("2nd, adjVert = " << adjVert );
#endif
            aList[ vert+1+initSize ][ adjVert ] = graph->adjList[ vert+1 ][ adjVert-1 ] + initSize;
        }

    }
    /*    for(v = 0; v <= numOfVert; v++)
	  delete [] adjList[v];*/
    					/* WILL HAVE TO FREE SPACE HERE */
    /*delete [] adjList;*/
    /* adjList = aList;*/

    graph->numOfVert = numOfVert;
    graph->adjList = aList;
    return graph;
}

//**************************************************************
//
//	mesh()
//
//**************************************************************
Graph * mesh( size_tt height )
{
#if DEBUG
    debug("Entering");
#endif

    return mesh_Graph(height,height);

#if DEBUG
    debug("Leaving");
#endif
}

//**************************************************************
//
//	Method name : mesh_Graph
//
//      creating a rectangular w x h mesh
//
//**************************************************************
Graph * mesh_Graph(size_tt h, size_tt w)
{
#define DEBUG_MISH_GRAPH 0
#if DEBUG_MISH_GRAPH
    debug("Entering mesh_Graph");
#endif

	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = w * h;
	size_tt v;
	int k;
	int i;
	int j;


#if DEBUG_MISH_GRAPH
    debug("h=" << h<<", w="<< w<<", numOfVert=" << numOfVert );
#endif


    size_tt ** adjList;

    assert(graph!=NULL);

    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    assert(adjList!=NULL);
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
    assert(adjList[0]!=NULL);

    //setting degrees of the first colunm
    adjList[0][0]=2;
    adjList[0][h-1]=2;
    for(k=1; k < h-1; k++)
        adjList[0][k]=3;

    /* for(v=0; v < h; v++)
       debug3("adjList[0][%d]=%d\n",v,adjList[0][v]);*/

    // memory alloc for the adj list of the elements of the first column
    for(v = 0; v < h; v++) {
      adjList[v+1] = (size_tt *)malloc(adjList[0][v] * sizeof(size_tt));
      assert(adjList[v+1]!=NULL);
    }

    // setting degrees and memory alloc of the elements of the remaining columns
    for(i=1; i<w; i++){
        if( i == w-1 ){
            for(v = 0; v < h; v++){
                adjList[ 0 ][ v + i*h ] = adjList[0][v];
                adjList[ (v + i*h) + 1 ] = (size_tt *)malloc(adjList[0][v + i*h] * sizeof(size_tt));
		assert(adjList[ (v + i*h) + 1 ]!=NULL);
            }
        } else {
            for( v = 0; v < h; v++){
                adjList[ 0 ][ v + i*h ] = adjList[0][v]+1;
                adjList[ v+1+ i*h ] = (size_tt *)malloc(adjList[0][ v + i*h] * sizeof(size_tt));
		assert(adjList[ v+1+ i*h ]!=NULL);
	    }
	}
    }
    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++){
            adjList[ (v + i*h) + 1][0] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][1] = v-1 + i*h;
            adjList[ (v + i*h) + 1][2] = v + (i+1)*h;
            adjList[ (v + i*h) + 1][3] = v+1 + i*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }

#if DEBUG_MISH_GRAPH
    debug("Leaving mesh_Graph");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=NULL;	
	return graph;
}


//**************************************************************
//
//      meshT()
//      Creates a triangular mesh
//
//**************************************************************
Graph * meshT(size_tt h)
{

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  size_tt numOfVert=(h+1)*h/2;
  size_tt index=0;
  int i, j, v;

  size_tt ** adjList;
  assert(graph!=NULL);
  adjList =   (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
  assert(adjList!=NULL);
  adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
  assert(adjList[0]!=NULL);
  adjList[1] = (size_tt *)malloc(2 * sizeof(size_tt));
  assert(adjList[1]!=NULL);

  adjList[0][0]=2;
  adjList[1][0]=1;
  adjList[1][1]=2;

  for(i=2;i<=h;i++) 
    {
      for(j=1;j<=i;j++)
	{
	  index=index+1;
	  if (i!=h)
	    {
	      if (j==1) 
		{
		  adjList[0][index]=4;
		  adjList[index+1]= (size_tt *)malloc(4 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-(i-1);
		  adjList[index+1][1]=index+1;
		  adjList[index+1][2]=index+i;
		  adjList[index+1][3]=index+i+1;
		}
	      else if(j==i) 
		{
		  adjList[0][index]=4;
		  adjList[index+1]= (size_tt *)malloc(4 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-i;
		  adjList[index+1][1]=index-1;
		  adjList[index+1][2]=index+i;
		  adjList[index+1][3]=index+i+1;	
		}
	      else 
		{
		  adjList[0][index]=6;
		  adjList[index+1]= (size_tt *)malloc(6 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-i;
		  adjList[index+1][1]=index-i+1;
		  adjList[index+1][2]=index-1;
		  adjList[index+1][3]=index+1;
		  adjList[index+1][4]=index+i;
		  adjList[index+1][5]=index+i+1;
		}
	    }
	  else 
	    {
	      if(j==1) 
		{	
		  adjList[0][index]=2;
		  adjList[index+1]= (size_tt *)malloc(2 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-(i-1);
		  adjList[index+1][1]=index+1;
		}
	      else if(j==i) 
		{
		  adjList[0][index]=2;
		  adjList[index+1]= (size_tt *)malloc(6 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-i;
		  adjList[index+1][1]=index-1;
		}
	      else 
		{
		  adjList[0][index]=4;
		  adjList[index+1]= (size_tt *)malloc(6 * sizeof(size_tt));
		  assert(adjList[index+1]!=NULL);
		  adjList[index+1][0]=index-i;
		  adjList[index+1][1]=index-i+1;
		  adjList[index+1][2]=index-1;
		  adjList[index+1][3]=index+1;
		}
	    }
	}
    }


  debug("adjList: \n");
  for(v=0; v< numOfVert; v++){
    debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
    for(j=0; j < adjList[0][v]; j++){
      debug2("%d ",adjList[v+1][j]);
    }
    debug("\n");
  }
  
  graph->numOfVert = numOfVert;
  graph->adjList = adjList;
  graph->weightList=NULL;
  return graph;
}
//**************************************************************
//
//	square_Cylinder()
//
//**************************************************************
Graph * square_Cylinder( size_tt h, size_tt w)
{
#define DEBUG_MISH_GRAPH 0
#if DEBUG_MISH_GRAPH
    debug("Entering mesh_Graph");
#endif

	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = w * h;
 	size_tt v;
 	int i;
 	int j;
 	int k;

#if DEBUG_MISH_GRAPH
    debug("h=" << h<<", w="<< w<<", numOfVert=" << numOfVert );
#endif

    size_tt ** adjList;
    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

    //setting degrees of the first colunm
    adjList[0][0]=3;
    adjList[0][h-1]=3;
    for(k=1; k < h-1; k++)
        adjList[0][k]=4;


    // memory alloc for the adj list of the elements of the first column
    for(v = 0; v < h; v++)
        adjList[v+1] = (size_tt *)malloc(adjList[0][v] * sizeof(size_tt));

    // setting degrees and memory alloc of the elements of the remaining columns
    for(i=1; i<w; i++)
    {

      for(v = 0; v < h; v++)
    	{
      	adjList[ 0 ][ v + i*h ] = adjList[0][v];
        adjList[ v+1+ i*h ] = (size_tt *)malloc(adjList[0][ v + i*h] * sizeof(size_tt));
	    }
    }
    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++){
            adjList[ (v + i*h) + 1][0] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][1] = v-1 + i*h;
            adjList[ (v + i*h) + 1][2] = v + (i+1)*h;
            adjList[ (v + i*h) + 1][3] = v+1 + i*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }


      for(j=0;j<h;j++){
		int qqq=(w-1)*h+j;
		adjList[j+1][adjList[0][j]-1]=qqq;
		adjList[qqq+1][adjList[0][j]-1]=j;
       }

#if DEBUG_MISH_GRAPH
    debug("Leaving mesh_Graph");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=NULL;
	return graph;
}
//**************************************************************
//
//	sphere()
//
//**************************************************************
Graph * sphere( size_tt h, size_tt w)
{
	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = w * h;
 	size_tt v;
 	int i;
 	int j;
 	int k;
// 	int stdw = 32, sw = 16, ew = 64, iw=(ew-sw)/(h/2-1);
  double radius = 500, deltaParallel =  (radius*2.0*sin(M_PI/(h+1.0)/2.0)); //latitude difference
  double deltaLongitudeConst =  (radius*2.0*sin(2.0*M_PI/w/2.0));  // constant fraction of the longitude difference
    size_tt ** adjList;
    coord_t ** weightList;
    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    weightList =  (coord_t **)malloc((numOfVert + 1) * sizeof(coord_t *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
    //setting degrees of the first colunm
    adjList[0][0]=3;
    adjList[0][h-1]=3;
    for(k=1; k < h-1; k++)
        adjList[0][k]=4;

    // memory alloc for the adj list of the elements of the first column
    for(v = 0; v < h; v++)
    {
        adjList[v+1] = (size_tt *)malloc(adjList[0][v] * sizeof(size_tt));
        weightList[v+1] = (coord_t *)malloc(adjList[0][v] * sizeof(coord_t));
    }

    // setting degrees and memory alloc of the elements of the remaining columns
    for(i=1; i<w; i++)
    {

      for(v = 0; v < h; v++)
    	{
      	adjList[ 0 ][ v + i*h ] = adjList[0][v];
        adjList[ v+1+ i*h ] = (size_tt *)malloc(adjList[0][ v + i*h] * sizeof(size_tt));
        weightList[ v+1+ i*h ] = (coord_t *)malloc(adjList[0][ v + i*h] * sizeof(coord_t));
	    }
    }
    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++)
        {
            adjList[ (v + i*h) + 1][0] = v-1 + i*h;
            adjList[ (v + i*h) + 1][1] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][2] = v+1 + i*h;
            adjList[ (v + i*h) + 1][3] = v + (i+1)*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++)
    {
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++)
    {
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++)
    {
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for(i=1; i < w-1; i++)
    {
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }


    for(j=0;j<h;j++)
    {
			int qqq=(w-1)*h+j;
			adjList[j+1][adjList[0][j]-1]=qqq;
			adjList[qqq+1][adjList[0][j]-1]=j;
   }

   for(i=0; i< w; i++) // rightwards
   {
    // bottom and top of the sphere is handled seperatly
    weightList[ i*h + 1 ][0] = deltaLongitudeConst*sin(M_PI/(h+1.0));
  	weightList[ i*h + 1 ][1] = deltaParallel;
  	weightList[ i*h + 1 ][2] = deltaLongitudeConst*sin(M_PI/(h+1.0));
   	weightList[ (i+1)*h -1 +1][0] = deltaLongitudeConst*sin(M_PI/(h+1.0));
   	weightList[ (i+1)*h -1 +1][1] = deltaParallel;
   	weightList[ (i+1)*h -1 +1][2] = deltaLongitudeConst*sin(M_PI/(h+1.0));
   	for(j = 1; j < (h >> 1); j++) // downwards
   	{
    	weightList[ j + i*h + 1 ][0] = deltaParallel;
  		weightList[ j + i*h + 1 ][2] = deltaParallel;
   	  weightList[ i*h + h-1-j +1][0] = deltaParallel;
   		weightList[ i*h + h-1-j +1][2] = deltaParallel;
   		weightList[ j + i*h + 1 ][1] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(j+1.0)); //sw+j*iw;
   		weightList[ j + i*h + 1 ][3] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(j+1.0)); //sw+(j+1)*iw;
	   	weightList[ i*h + h-1-j +1 ][1] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(j+1.0));// sw+(j+1)*iw;
	   	weightList[ i*h + h-1-j +1 ][3] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(j+1.0)); //sw+j*iw;
   		
   	}

   	if(h%2 != 0) // h is odd
   	{
   	  k = (h>>1); 
      printf("Hallo %d %d %d\n",i,h,k);
   	  weightList[ k + i*h + 1 ][0] = deltaParallel;
  		weightList[ k + i*h + 1 ][2] = deltaParallel;
   		weightList[ k + i*h + 1 ][1] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(k+1.0)); //sw+j*iw;
   		weightList[ k + i*h + 1 ][3] = deltaLongitudeConst*sin(M_PI/(h+1.0)*(k+1.0)); //sw+(j+1)*iw;
   	}
   }

	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=weightList;
	return graph;
}

//**************************************************************
//
//	globus()
//
//**************************************************************
Graph * globus( size_tt h, size_tt w)
{

	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = w * h+2;
 	size_tt v;
 	int i;
 	int j;

//first of all we use the mesh_Graph-Algorithm, then we connect the two vertical edges,
// after that, we connect the poles
    size_tt ** adjList;
    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));


    // setting degrees and memory alloc of the elements of the remaining columns
    for(i=0; i<w; i++)
    {
    	for(v = 0; v < h; v++)
      {
      	adjList[ 0 ][ v + i*h ] = 4;
        adjList[ (v + i*h) + 1 ] = (size_tt *)malloc(4 * sizeof(size_tt));
      }
    }
    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++){
            adjList[ (v + i*h) + 1][0] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][1] = v-1 + i*h;
            adjList[ (v + i*h) + 1][2] = v + (i+1)*h;
            adjList[ (v + i*h) + 1][3] = v+1 + i*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

// connecting grid edges
  for(j=1;j<h-1;j++)
  {
		int qqq=(w-1)*h+j;
		adjList[j+1][3]=qqq;
		adjList[qqq+1][3]=j;
	}
	adjList[1][2]=(w-1)*h;
	adjList[(w-1)*h+1][2]=0;
	adjList[h-1+1][2]=w*h-1;
	adjList[w*h-1+1][2]=h-1;
// connecting poles
	
  adjList[h*w+1]=(size_tt *)malloc(w * sizeof(size_tt));
	adjList[h*w+2]=(size_tt *)malloc(w * sizeof(size_tt));
	adjList[0][h*w]=w;
	adjList[0][h*w+1]=w;
  for(i=0;i<w;i++)
  {
  	adjList[i*h+1][3]=h*w;
  	adjList[h*w+1][i]=i*h;
  	adjList[h*(i+1)][3]=h*w+1;
  	adjList[h*w+2][i]=h*(i+1)-1;
  }

	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=NULL;
	return graph;
}



//**************************************************************
//
//	moebius()
//
//**************************************************************
Graph * moebius( size_tt h, size_tt w)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    size_tt numOfVert = w * h;
    size_tt v;
    int i;
    int j;
    int k;
    size_tt ** adjList;

    //swap h and w to make # be the lenght and T the thickness
    size_tt temp = h;
    h=w;
    w=temp;
    /*#if DEBUG_MOEBIUS_GRAPH
    debug("h=" << h<<", w="<< w<<", numOfVert=" << numOfVert );
    #endif*/

    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

    //setting degrees of the first colunm
    adjList[0][0]=3;
    adjList[0][h-1]=3;
    for(k=1; k < h-1; k++)
        adjList[0][k]=4;

    // memory alloc for the adj list of the elements of the first column
    for( v = 0; v < h; v++)
        adjList[v+1] = (size_tt *)malloc(adjList[0][v] * sizeof(size_tt));

    // setting degrees and memory alloc of the elements of the remaining columns
    for(i=1; i<w; i++){
        if( i == w-1 ){
            for(v = 0; v < h; v++){
                adjList[ 0 ][ v + i*h ] = adjList[0][v];
                adjList[ (v + i*h) + 1 ] = (size_tt *)malloc(adjList[0][v + i*h] * sizeof(size_tt));
            }
        } else {
            for(v = 0; v < h; v++){
                adjList[ 0 ][ v + i*h ] = adjList[0][v];
                adjList[ v+1+ i*h ] = (size_tt *)malloc(adjList[0][ v + i*h] * sizeof(size_tt));
	    }
	}
    }
    //
    //  SETTING ADJACENCY LISTS OF ALL VICES
    //

    // interior vertices
    for(i=1; i < w-1; i++)
        for(v = 1; v < h-1; v++){
            adjList[ (v + i*h) + 1][0] = v + (i-1)*h;
            adjList[ (v + i*h) + 1][1] = v-1 + i*h;
            adjList[ (v + i*h) + 1][2] = v + (i+1)*h;
            adjList[ (v + i*h) + 1][3] = v+1 + i*h;
        }

    // left vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ v + 1][0] = v - 1;
        adjList[ v + 1][1] = v + h;
        adjList[ v + 1][2] = v + 1;
    }

    // right vertical edge (without corners)
    for(v = 1; v < h-1; v++){
        adjList[ (v + (w-1)*h) + 1][0] = v-1 + (w-1)*h;
        adjList[ (v + (w-1)*h) + 1][1] = v   + (w-2)*h;
        adjList[ (v + (w-1)*h) + 1][2] = v+1 + (w-1)*h;
    }

    // top horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ i*h + 1][0] = (i-1)*h;
        adjList[ i*h + 1][1] = 1 + i*h;
        adjList[ i*h + 1][2] = (i+1)*h;
    }

    // bottom horizontal edge
    for(i=1; i < w-1; i++){
        adjList[ h + i*h ][0] = h-1 + (i-1)*h;
        adjList[ h + i*h ][1] = h-2 + i*h;
        adjList[ h + i*h ][2] = h-1 + (i+1)*h;
    }

    // FOUR CORNERS

    // top left
    adjList[1][0] = h;
    adjList[1][1] = 1;

    // bottom left
    adjList[h][0] = 2*h-1;
    adjList[h][1] = h-2;

    // top right
    adjList[(w-1)*h + 1][0] = (w-2)*h;
    adjList[(w-1)*h + 1][1] = 1 + (w-1)*h;

    // bottom right
    adjList[ h + (w-1)*h ][0] = h-1 + (w-2)*h;
    adjList[ h + (w-1)*h ][1] = h-2 + (w-1)*h;

      for(j=0;j<h;j++){
		int qqq=w*h-j-1;
		adjList[j+1][adjList[0][j]-1]=qqq;
		adjList[qqq+1][adjList[0][j]-1]=j;
      }

#if DEBUG_MOEBIUS_GRAPH
    debug("Leaving moebius_Graph");
#endif

	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
  graph->weightList=NULL;
	return graph;
}

//**************************************************************
//
//	wmesh_Graph() - a weighted mesh
//
//**************************************************************
#define LEFT(i) (i)-1
#define RIGHT(i) (i)+1
#define UP(i) (i)-length
#define DOWN(i) (i)+length
Graph * wmesh( size_tt _numOfVert ,size_tt _stdwidth, size_tt  _smallwidth, size_tt _largewidth )
{
#define DEBUG_COMPL 0
#if DEBUG || DEBUG_COMPL
    debug("Entering complete_Graph");
#endif

	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = _numOfVert*_numOfVert; // we will produce a square field
 	size_tt length = _numOfVert;
 	size_tt i,j;
 	size_tt sw=_smallwidth,stdw=_stdwidth, ew=_largewidth;
 	size_tt iw=(ew-sw)/(length-1);

    // the i-th element of the first row holds numbers of adjacent
    // vertices in the i-th row of the adjacency list

    // initializing adjList
    size_tt ** adjList;
    coord_t ** weightList;

    assert(graph!=NULL);


	adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
	assert(adjList!=NULL);
	adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
	assert(adjList[0]!=NULL);
  weightList =  (coord_t **)malloc((numOfVert + 1) * sizeof(coord_t *));
  assert(weightList!=NULL);
  // nooks
  	adjList[0][0] = 2;
  adjList[1] = (size_tt *)malloc((2) * sizeof(size_tt));
  assert(adjList[1]!=NULL);
  adjList[1][0] = RIGHT(0);
  adjList[1][1] = DOWN(0);
  weightList[1] = (coord_t *)malloc((2) * sizeof(coord_t));
  assert(weightList[1]!=NULL);
  weightList[1][0] = stdw;
  weightList[1][1] = sw;
  	adjList[0][length-1] = 2;
  adjList[length] = (size_tt *)malloc((2) * sizeof(size_tt));
  assert(adjList[length]!=NULL);
  adjList[length][0] = LEFT(length-1);
  adjList[length][1] = DOWN(length-1);
  weightList[length] = (coord_t *)malloc((2) * sizeof(coord_t));
  assert(weightList[length]!=NULL);
  weightList[length][0] = stdw;
  weightList[length][1] = ew;
        adjList[0][(length-1)*length] = 2;
  adjList[(length-1)*length+1] = (size_tt *)malloc((2) * sizeof(size_tt));
  assert(adjList[(length-1)*length+1]!=NULL);
  adjList[(length-1)*length+1][0] = RIGHT((length-1)*length);
  adjList[(length-1)*length+1][1] = UP((length-1)*length);
  weightList[(length-1)*length+1] = (coord_t *)malloc((2) * sizeof(coord_t));
  assert(weightList[(length-1)*length+1]!=NULL);
  weightList[(length-1)*length+1][0] = stdw;
  weightList[(length-1)*length+1][1] = sw;
  	adjList[0][length*length-1] = 2;
  adjList[length*length] = (size_tt *)malloc((2) * sizeof(size_tt));
  assert(adjList[length*length]!=NULL);
  adjList[length*length][0] = UP(length*length-1);
  adjList[length*length][1] = LEFT(length*length-1);
  weightList[length*length] = (coord_t *)malloc((2) * sizeof(coord_t));
  assert(weightList[length*length]!=NULL);
  weightList[length*length][0] = ew;
  weightList[length*length][1] = stdw;
  // margin
  for(i= 1; i < length-1; i++)
  {
  		adjList[0][i]=3;
  	adjList[i+1] = (size_tt *)malloc((3) * sizeof(size_tt));
	assert(adjList[i+1]!=NULL);
  	adjList[i+1][0]=RIGHT(i);
  	adjList[i+1][1]=LEFT(i);
  	adjList[i+1][2]=DOWN(i);
        weightList[i+1] = (coord_t *)malloc((3) * sizeof(coord_t));
        assert(weightList[i+1]!=NULL);
  	weightList[i+1][0]=stdw;
  	weightList[i+1][1]=stdw;
  	weightList[i+1][2]=sw+i*iw;
  		adjList[0][i*length]=3;
  	adjList[i*length+1] = (size_tt *)malloc((3) * sizeof(size_tt));
        assert(adjList[i*length+1]!=NULL);
  	adjList[i*length+1][0]=UP(i*length);
  	adjList[i*length+1][1]=RIGHT(i*length);
  	adjList[i*length+1][2]=DOWN(i*length);
  	weightList[i*length+1] = (coord_t *)malloc((3) * sizeof(coord_t));
        assert(weightList[i*length+1]!=NULL);
  	weightList[i*length+1][0]=sw;
  	weightList[i*length+1][1]=stdw;
  	weightList[i*length+1][2]=sw;
  		adjList[0][(i+1)*length-1]=3;
  	adjList[(i+1)*length] = (size_tt *)malloc((3) * sizeof(size_tt));
        assert(adjList[(i+1)*length]!=NULL);
  	adjList[(i+1)*length][0]=UP((i+1)*length-1);
  	adjList[(i+1)*length][1]=LEFT((i+1)*length-1);
  	adjList[(i+1)*length][2]=DOWN((i+1)*length-1);
  	weightList[(i+1)*length] = (coord_t *)malloc((3) * sizeof(coord_t));
        assert(weightList[(i+1)*length]!=NULL);
  	weightList[(i+1)*length][0]=ew;
  	weightList[(i+1)*length][1]=stdw;
  	weightList[(i+1)*length][2]=ew;
  		adjList[0][length*(length-1)+i]=3;
  	adjList[length*(length-1)+i+1] = (size_tt *)malloc((3) * sizeof(size_tt));
        assert(adjList[length*(length-1)+i+1]!=NULL);
  	adjList[length*(length-1)+i+1][0]=RIGHT(length*(length-1)+i);
  	adjList[length*(length-1)+i+1][1]=UP(length*(length-1)+i);
  	adjList[length*(length-1)+i+1][2]=LEFT(length*(length-1)+i);
  	weightList[length*(length-1)+i+1] = (coord_t *)malloc((3) * sizeof(coord_t));
        assert(weightList[length*(length-1)+i+1]!=NULL);
  	weightList[length*(length-1)+i+1][0]=stdw;
  	weightList[length*(length-1)+i+1][1]=sw+i*iw;
  	weightList[length*(length-1)+i+1][2]=stdw;

  }
  // rest
  for(i=1; i < length-1; i ++)   // downwards
  	for(j=1; j < length-1; j++)  // leftwards
  	{
	  	adjList[0][ j+i*length ] = 4;
  		adjList[j+i*length+1] = (size_tt *)malloc((4) * sizeof(size_tt));
		assert(adjList[j+i*length+1]!=NULL);
  		adjList[j+i*length+1][0] = RIGHT(j+i*length);
  		adjList[j+i*length+1][1] = UP(j+i*length);
  		adjList[j+i*length+1][2] = LEFT(j+i*length);
   		adjList[j+i*length+1][3] = DOWN(j+i*length);
   		weightList[j+i*length+1] = (coord_t *)malloc((4) * sizeof(coord_t));
		assert(weightList[j+i*length+1]!=NULL);
  		weightList[j+i*length+1][0] = stdw;
  		weightList[j+i*length+1][1] = sw+j*iw;
  		weightList[j+i*length+1][2] = stdw;
   		weightList[j+i*length+1][3] = sw+j*iw;
   	}


//		for(i=0;i< length*length; i++)
//		{
//			for(j=0;j< adjList[0][i]; j++)
//				printf("%d ",weightList[i+1][j]);
//			printf("\n");
//		}
#if DEBUG	
    debug("Exiting complete_Graph");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=weightList;
	return graph;
}


//**************************************************************
//
//	old_complete_Graph()
//
//**************************************************************
Graph * old_complete_Graph( size_tt _numOfVert )
{
#define DEBUG_COMPL 0
#if DEBUG || DEBUG_COMPL
    debug("Entering complete_Graph");
#endif

	Graph *graph = (Graph *)malloc(sizeof(Graph));
 	size_tt numOfVert = _numOfVert;
 	size_tt vert;
	size_tt adjVert;
 	size_tt i;
 	size_tt index;

	// create an auxiliary array, which holds numbers 0..numOfVert-1
    size_tt *vertices = (size_tt *)malloc(numOfVert * sizeof(size_tt));

    // the i-th element of the first row holds numbers of adjacent
    // vertices in the i-th row of the adjacency list

    // initializing adjList
    size_tt ** adjList;
	adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
	adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

    for(vert = 0; vert < numOfVert; vert++)
        adjList[ 0 ][ vert ] = numOfVert - 1;

    for(vert = 0; vert < numOfVert; vert++)
        adjList[vert+1] = (size_tt *)malloc((numOfVert - 1) * sizeof(size_tt));


    // create an auxiliary array, which holds numbers 0..numOfVert-1
    for(i = 0; i < numOfVert; i++)
        vertices[i] = i;

    index = 0;

    for(vert = 0; vert < numOfVert; vert++)
    {   // locate vert in 'vertices' and then swap it with the last
        // element of 'vertices'
        index = 0;
        while( vertices[index] !=  vert )
            ++index;
        swap(&vertices[index], &vertices[numOfVert-1]);

        for(adjVert = 0; adjVert < numOfVert - 1;
            adjVert++)
            adjList[ vert+1 ][ adjVert ] = vertices[adjVert];
    }

    					/* WILL HAVE TO FREE SPACE HERE */

    /*delete [] vertices;*/

#if DEBUG
    debug("Exiting complete_Graph");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=NULL;
	return graph;
}

//**************************************************************
//
//      path_Graph
//
//**************************************************************
Graph * path_Graph( size_tt _numOfVert )
{
#define DEBUG_PATH 0
#if DEBUG || DEBUG_PATH
    debug("Entering path");
#endif

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    size_tt numOfVert = _numOfVert;
    size_tt ** adjList;
    size_tt vert;
    size_tt adjVert;
    int j,v;

    // initializing adjacency list
    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

    // setting degrees
    // vertex 0 and numOfVert-1 must be dealt with separately
    adjList[0][0] = 1;
    adjList[0][numOfVert-1] = 1;
    for(vert = 1; vert < numOfVert-1; vert++)
      adjList[0][vert] = 2;

    // setting adjLists for each vertex
    // vertex 0 and numOfVert-1 must be considered separately
    adjList[1]    = (size_tt *)malloc(sizeof(size_tt));
    adjList[1][0] = 1;

    adjList[numOfVert]    = (size_tt *)malloc(sizeof(size_tt));
    adjList[numOfVert][0] = numOfVert-2;

    for( vert = 1; vert < numOfVert-1; vert++){
        adjList[ vert+1 ] = (size_tt *)malloc(sizeof(size_tt) * adjList[ 0 ][ vert ]);
        for(adjVert = 0; adjVert < adjList[ 0 ][ vert ];
            adjVert++) {
            adjList[ vert+1 ][ adjVert ] = vert - 1 + 2*adjVert;
        }
    }

        debug("adjList: \n");
        for(v=0; v< numOfVert; v++){
            debug4("%d:(adjList[0][%d]=%d) : ",v, v, adjList[0][v]);
            for(j=0; j < adjList[0][v]; j++){
                debug2("%d ",adjList[v+1][j]);
            }
            debug("\n");
        }
#if DEBUG_PATH
    debug("Exiting path");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	return graph;
  graph->weightList=NULL;
}

//**************************************************************
//
//	cycle_Graph()
//
//**************************************************************
Graph * cycle_Graph( size_tt _numOfVert )
{
#define DEBUGa 0
#if DEBUGa
    debug("Entering cycle");
#endif

  Graph *graph = (Graph *)malloc(sizeof(Graph));
	size_tt numOfVert = _numOfVert;
	size_tt vert;
	size_tt adjVert;
	size_tt ** adjList;

    // initializing adjList
    adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
    adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

    // setting degrees
    for(vert = 0; vert < numOfVert; vert++)
    	adjList[ 0 ][ vert ] = 2;


    // allocating memory
    for(vert = 0; vert < numOfVert; vert++)
        adjList[ vert+1 ] = (size_tt *)malloc(adjList[ 0 ][ vert ] * sizeof(size_tt));

    // setting adjacency lists for each vertex
    // vertices 0 and numOfVert-1 must be dealt with separately
    adjList[ 1 ][ 0 ] = numOfVert - 1;
    adjList[ 1 ][ 1 ] = 1;

    adjList[numOfVert][0] = numOfVert-2;
    adjList[numOfVert][1] = 0;


    for(vert = 1; vert < numOfVert-1; vert++)
        for(adjVert = 0; adjVert < adjList[ 0 ][ vert ];
            adjVert++){
            adjList[ vert+1 ][ adjVert ] = vert-1 + 2*adjVert;
        }

#if DEBUGa
    debug("Exiting cycle");
#endif
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
  graph->weightList=NULL;
	return graph;
}

//****************************************************************
//
//	rand_cpt_Graph()
//
//****************************************************************
size_tt **  rand_cpt_Graph( size_tt _numOfVert )
{
#if DEBUG
    debug("Entering rand_cpt_Graph" );
#endif

    size_tt numOfVert = _numOfVert;

    size_tt adj[numOfVert-1]; // array whose entries count the number of
                          // elements in the corresponding rows of
                          // adjList
	size_tt **cptAdjList =  (size_tt **)malloc(numOfVert * sizeof(size_tt *));
	size_tt adjVertex;
	size_tt vertex;
	size_tt index;

	srand(time(0));

    for( index = 0; index < numOfVert-1; index++){
        // for each vertex vert between 0 and numOfVert-2 generate
        // a random number between 1 and numOfVert - 1 - vert.
        adj[ index ] = 1 + rand() % (numOfVert - index - 1);

    }


    // initializing adjList
    cptAdjList[0] = (size_tt *)malloc( (numOfVert - 1) * sizeof(size_tt));

    for(adjVertex = 0; adjVertex < numOfVert - 1; adjVertex++)
        cptAdjList[ 0 ][ adjVertex ] = adj[ adjVertex ];


    for(vertex = 1; vertex < numOfVert; vertex++) {
        // generate adj[ vertex ] random size_tt without repetition
        // in the range vertex .. numOfVert-1
        size_tt array[numOfVert - vertex];
        size_tt i;
        for(i = 0; i < numOfVert - vertex; i++)
            array[i] = i + vertex;

        rand_Perm(array, (size_tt)(numOfVert - vertex),
                  cptAdjList[ vertex ], adj[ vertex - 1]);

		    	/* WILL HAVE TO FREE SPACE HERE */
/*        delete [] array;*/
    }
#if DEBUG
    debug("Exiting rand_cpt_Graph" );
#endif
    return cptAdjList;
}

//****************************************************************
//
//	rand_Graph()
//
//****************************************************************
Graph * rand_Graph( size_tt h, size_tt w)
{

	Graph *graph = (Graph *)malloc(sizeof(Graph));
	size_tt numOfVert = h;

  	size_tt temp=0;
  	size_tt total=0;
  	size_tt first_leaf;
  	size_tt ** adjList;
  	int i;
	int j;
  	size_tt type=0;
  	if(rand()%2==1)
  	  type=1;
  	srand(time(0));

  	while(total<=h) {
		total=total+(size_tt)pow(2,temp);
		temp++;
  	}
  	if (total> h)
		total=total-(size_tt)pow(2,temp-1);
 	first_leaf=total-(size_tt)pow(2,temp-2);
 	numOfVert=total;


  	if (type==0){
		size_tt base=2;
		adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
		adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));
		adjList[0][0]=base;
		adjList[1] = (size_tt *)malloc((h-1) * sizeof(size_tt));

		for(i=1;i<first_leaf;i++) {
		  adjList[0][i]=base+1;
		  adjList[i+1] = (size_tt *)malloc((h-1) * sizeof(size_tt));
		}
		for(i=first_leaf;i<numOfVert;i++) {
		  adjList[0][i]=1;
		  adjList[i+1] = (size_tt *) malloc((h-1) * sizeof(size_tt));
		}

		//setting adjacencies for the root
		for (i=0;i<base;i++)
		  adjList[1][i]=i+1;

		//setting adjacencies for the internal nodes

		for(i=1;i<first_leaf;i++){
		  adjList[i+1][0]=(size_tt)((i-1)/base);
		  for(j=1;j<=base;j++)
			adjList[i+1][j]=base*i+j;
		}

		//setting adjacencies for the leafs
		for(i=first_leaf;i<numOfVert;i++)
		  adjList[i+1][0]=(size_tt)((i-1)/base);

	  }

	  else
	  {
		size_tt vert;

		// initializing adjList
		adjList =  (size_tt **)malloc((numOfVert + 1) * sizeof(size_tt *));
		adjList[0] = (size_tt *)malloc(numOfVert * sizeof( size_tt));

		// setting degrees
		// vertex 0 and numOfVert-1 must be dealt with separately
		adjList[0][0] = 1;
		adjList[0][numOfVert-1] = 1;
		for(vert = 1; vert < numOfVert-1; vert++)
		  adjList[ 0 ][ vert ] = 2;

		// setting adjLists for each vertex
		// vertex 0 and numOfVert-1 must be considered separately
		adjList[1]    = (size_tt *)malloc( sizeof(size_tt));
		adjList[1][0] = 1;

		adjList[numOfVert]    = (size_tt *)malloc(sizeof(size_tt));
		adjList[numOfVert][0] = numOfVert-2;

		for(vert = 1; vert < numOfVert-1; vert++){
		  adjList[ vert+1 ] = (size_tt *)malloc((numOfVert-1) * sizeof(size_tt));
		  adjList[vert+1][0]=vert-1;
		  adjList[vert+1][1]=vert+1;
		}
	  }

	  for(i=0;i<numOfVert;i++)
	  {
		for(j=0;j<i;j++)
		{
		  if ((i!=j)&&(i!=(j-1))&&(i!=j+1))
		  {
			size_tt th=rand()%(numOfVert*numOfVert);
			if ((i<5)&&(th<=(w*w)))
			{
			  adjList[i+1][adjList[0][i]++]=j;
			  adjList[j+1][adjList[0][j]++]=i;
			}
			else if ((i>h-4)&&(j>h-4)&&(th<=(w*w)))
			{
		  	  adjList[i+1][adjList[0][i]++]=j;
		  	  adjList[j+1][adjList[0][j]++]=i;
			}
			else if (th<=w)
			{
		  	  adjList[i+1][adjList[0][i]++]=j;
		      adjList[j+1][adjList[0][j]++]=i;
			}
		  }
		}
  	  }
	graph->numOfVert = numOfVert;
	graph->adjList = adjList;
	graph->weightList=NULL;
	return graph;
}

//****************************************************************
//
//	Method name : rand_Perm
//
//	Description : perform a permutation of 'array' of 'len'
//      elements (we can set 'len' to be any number <= lenght[array])
//      put the first newLen elements of 'array' size_tto newArray.
//
//****************************************************************
void rand_Perm(size_tt *array,
                      size_tt len,
                      size_tt *newArray,
                      size_tt newLen)
{
    size_tt l = (newLen == 0) ? len : newLen;
    size_tt i;
    for(i = 0; i < l; i++)
        swap(&array[i], &array[i + (fast_Rand() % (len - i))]);

    for(i = 0; i < newLen; i++)
        newArray[i] = array[i];
}

//**************************************************************
//
//      sierpinski()
//
//**************************************************************
static size_tt vert = 4;
Graph *sierpinski(size_tt h, size_tt density)
{

  Graph *graph = (Graph *)malloc(sizeof(Graph));
  size_tt numOfVert;
  size_tt ** adjList = NULL;
  int i;

  if (density == 2) 
    {
      size_tt temp=3;
      for(i=1;i<=h;i++)
	temp=temp+(size_tt)pow(3,i);
      numOfVert=temp;
      adjList =  (size_tt **)malloc( (numOfVert + 1) * sizeof( size_tt*));
      adjList[0]=(size_tt *)malloc(numOfVert * sizeof(size_tt));
      for(i=0;i<numOfVert;i++) {
	adjList[i+1] = (size_tt *)malloc(4 * sizeof(size_tt));
	adjList[0][i]=0;
      }
      
      sierpinski_recurse(adjList,h,0,0,1,2);
    }
  else 
    {
      numOfVert = sierpinski_recurse3D(adjList,h,0,0,1,2,3,0);
      adjList =  (size_tt **)malloc( (numOfVert+1) * sizeof(size_tt*));
      adjList[0]=(size_tt *)malloc(numOfVert * sizeof(size_tt));
      for(i=0;i<numOfVert;i++) {
	adjList[i+1] = (size_tt *)malloc(6 * sizeof(size_tt));
	adjList[0][i]=0;
      }
      vert=4;
      sierpinski_recurse3D(adjList,h,0,0,1,2,3, 1);
    }
  graph->numOfVert = numOfVert;
  graph->adjList = adjList;
  graph->weightList=NULL;
  return graph;
}


void sierpinski_recurse(size_tt **adjList,
			int maxLevel,
			int currentLevel,
			size_tt a, size_tt b, size_tt c)
{
  static size_tt vert = 3;
  if (currentLevel >= maxLevel) 
    {
      addEdge(adjList,a,b);
      addEdge(adjList,a,c);
      addEdge(adjList,b,c);
    } 
  else 
    {
      size_tt d = vert++;
      size_tt e = vert++;
      size_tt f = vert++;
      sierpinski_recurse(adjList,maxLevel,currentLevel+1,a,d,e);
      sierpinski_recurse(adjList,maxLevel,currentLevel+1,d,b,f);
      sierpinski_recurse(adjList,maxLevel,currentLevel+1,e,f,c);
    }
}


size_tt sierpinski_recurse3D(size_tt **adjList,
			     int maxLevel,
			     int currentLevel,
			     size_tt a, size_tt b, 
			     size_tt c,
			     size_tt d,
			     int flag)
{ 
  if (currentLevel >= maxLevel) 
    {
      if (flag) {
	addEdge(adjList,a,b);
	addEdge(adjList,a,c);
	addEdge(adjList,a,d);
	addEdge(adjList,b,c);
	addEdge(adjList,b,d);
	addEdge(adjList,c,d);
      }
    } 
  else 
    {
      size_tt e = vert++;
      size_tt f = vert++;
      size_tt g = vert++;
      size_tt h = vert++;
      size_tt i = vert++;
      size_tt j = vert++;
      sierpinski_recurse3D(adjList,maxLevel,currentLevel+1,a,g,e,f,flag);
      sierpinski_recurse3D(adjList,maxLevel,currentLevel+1,e,b,i,h,flag);
      sierpinski_recurse3D(adjList,maxLevel,currentLevel+1,f,c,j,h,flag);
      sierpinski_recurse3D(adjList,maxLevel,currentLevel+1,g,d,i,j,flag);
    }
  return vert;
}


//****************************************************************
//
//	swap()
//
//	swap two int, coord_t or Point<>
//
//****************************************************************

void swap(size_tt *a, size_tt *b)
{
  size_tt temp = *a;
  *a = *b;
  *b = temp;
}
void addEdge(size_tt **adjList, size_tt a, size_tt b)
{
  adjList[a+1][adjList[0][a]] = b;
  adjList[b+1][adjList[0][b]] = a;
  adjList[0][a]++;
  adjList[0][b]++;
}

//*************************************************************
//
//         fast random function
//
//************************************************************

void sfast_Rand(unsigned int seed)  { idum = seed; }

unsigned long fast_Rand(void)
{
    idum = 1664525L*idum + 1013904223L;
    return idum;
}


//********************************************************
//
//      free all memory used by the Graph structure
//
//********************************************************
void free_Graph(Graph *graph)
{
  int i;
  for (i = 0; i <= graph->numOfVert; i ++)
    free(graph->adjList[i]);
  free(graph->adjList);
  if(graph->weightList!=0)
  {
  	for (i = 1; i <= graph->numOfVert; i ++)
  		free(graph->weightList[i]);
  	free(graph->weightList);
  }
  //free(graph);
}




