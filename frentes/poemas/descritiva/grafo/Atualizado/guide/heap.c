#include "heap.h"
#include "math.h"
#include "string.h"

#define prev(i) (i) >> 1

void createHeap(VertDistPair* A, size_tt heapSize)
{
  size_tt i;
  for(i=0;i <= heapSize/2; i++)
    heapify(A,heapSize,heapSize/2-i);
}

void heapify(VertDistPair* A, size_tt heapSize, size_tt index)
{
  size_tt maxIndex=index;
  VertDistPair dummy;
  if(2*index < heapSize && A[index].dist > A[2*index].dist)
    maxIndex=2*index;
  if(2*index+1 < heapSize && A[maxIndex].dist > A[2*index+1].dist)
    maxIndex=2*index+1;
  if(maxIndex!=index)
  {
	  dummy=A[index];
	  A[index]=A[maxIndex];
	  A[maxIndex]=dummy;
	  heapify(A,heapSize,maxIndex);
	}
}

size_tt extractMin(VertDistPair* A, size_tt* pHeapSize,coord_t *oderElem)
{
  size_tt erg=A[0].vert;
  *oderElem=A[0].dist;
  (*pHeapSize)--;
  A[0]=A[*pHeapSize];
  heapify(A,*pHeapSize,0);
  return erg;
}

void insert(VertDistPair* A, size_tt* pHeapSize, size_tt element,coord_t orderElem)
{
  size_tt i=*pHeapSize;
  while((i!=0)&&(A[prev(i)].dist>orderElem))
    {
      A[i]=A[prev(i)];
      i=prev(i);
    }
  A[i].vert=element;
  A[i].dist=orderElem;
  (*pHeapSize)++;
}

void copyHeap(VertDistPair* oldA, VertDistPair* newA, size_tt heapSize)
{
  memcpy(newA,oldA,heapSize*sizeof(VertDistPair));
}

