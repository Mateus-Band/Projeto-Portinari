#include "Graph.h"

typedef struct
{
	size_tt vert;
	coord_t dist;
} VertDistPair;

void createHeap(VertDistPair* A, size_tt heapSize);

void heapify(VertDistPair* A, size_tt heapSize, size_tt index);

size_tt extractMin(VertDistPair* A, size_tt* pHeapSize, coord_t * orderElem);

void insert(VertDistPair* A, size_tt* pHeapSize, size_tt element, coord_t orderElem);

void copyHeap(VertDistPair* oldA, VertDistPair* newA, size_tt heapSize);