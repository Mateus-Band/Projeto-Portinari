#ifndef PQUEUE_H
#define PQUEUE_H

#include "Graph.h"
#include "heap.h"

/* Simple priority queue structure and operations to be used in Dijkstra, it uses a heap */
typedef struct pqueue{
  size_tt size;
  size_tt count;
  VertDistPair* elements;
} PQueue;

/* makes a new Q and initializes all values */
PQueue *new_PQ(unsigned long size);

void enqueuePQ(PQueue *Q, size_tt entry,coord_t distance);

size_tt dequeueMin(PQueue *Q,coord_t * pDistance);

int pqueue_size(PQueue *Q);

void free_pqueue(PQueue *Q);
#endif
