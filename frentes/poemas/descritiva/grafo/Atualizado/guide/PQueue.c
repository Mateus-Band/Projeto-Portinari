#include "PQueue.h"


/* makes a new Q and initializes all values */
PQueue *new_PQ(unsigned long size)
{
  PQueue *newQ = (PQueue *)malloc(sizeof(PQueue));
  newQ->size = size;
  newQ->count = 0;
	newQ->elements = (VertDistPair*)malloc(size * sizeof(VertDistPair));
  return newQ;
}

void enqueuePQ(PQueue *Q, size_tt entry,coord_t distance)
{
  if (Q->count >= Q->size) // we need to enlarge the array
  {
    VertDistPair* A;
    Q->size *=2;
    A = (VertDistPair*)malloc(Q->size * sizeof(VertDistPair));
    copyHeap(Q->elements,A,Q->count);
    free(Q->elements);
    Q->elements=A;
  }
  insert(Q->elements,&(Q->count),entry,distance);
}

size_tt dequeueMin(PQueue *Q,coord_t * pDistance)
{
  if (Q->count > 0 )
    return extractMin(Q->elements,&(Q->count),pDistance);
  return -1;
}

int pqueue_size(PQueue *Q)
{
  return Q->count;
}

void free_pqueue(PQueue *Q)
{
  free(Q->elements);
  free(Q);
}
