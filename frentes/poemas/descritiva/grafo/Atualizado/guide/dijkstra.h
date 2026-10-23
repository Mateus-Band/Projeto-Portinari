#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <math.h>
#include <time.h>
#include "Graph.h"
#include "PQueue.h"
#include "DrawGraph.h"

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

//int diam;
//int *create_bfs(Graph *graph, size_tt *vertices, int num_vert, int level);
//int *bfs(Graph *graph, size_tt root, int depthLim);
//int *rand_bfs(size_tt* vertices, int num_vert);
graphDist * nbr_dijkstra(Graph *graph, size_tt root, graphDist ***nbrs, size_tt *nbr, size_tt *vertDepth);

#endif
