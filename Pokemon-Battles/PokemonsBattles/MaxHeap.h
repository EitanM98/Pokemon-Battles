#ifndef ASSINGMENT3_MAXHEAP_H
#define ASSINGMENT3_MAXHEAP_H
#include "Defs.h"
#include <math.h>

typedef struct MaxHeap_t* MaxHeap;
// gets 4 basic function: copy, free, compare, print and heap's name and maximum size and returns a pointer to a MaxHeap
MaxHeap CreateHeap(int max_size,char *name, equalFunction compFunc, printFunction printFunc, copyFunction copyFunc, freeFunction freeFunc);
//gets a pointer to a heap and deletes all of its contents and frees all the memory allocated for it
void destroyHeap(MaxHeap h);
//gets a pointer to a heap and pointer to an element and inserts it to the heap in the requested order, returns success/failure
status insertToHeap(MaxHeap h, element lm);
//gets pointer to the heap and prints all its elements sorted by descending order
status printHeap(MaxHeap h);
// gets a pointer to the heap and extracts in O(1) its max element and returns the element's copy and needs to be freed afterwards
element PopMaxHeap(MaxHeap h);
// gets a pointer to the heap and returns the max element without extracting it, returns a copy that should be freed
element TopMaxHeap(MaxHeap h);
// gets a pointer to the heap and returns a copy of its name, user should free the returned value.
char* getHeapId(MaxHeap h);
// gets a pointer to the heap and returns how many elements are currently in the heap
int getHeapCurrentSize(MaxHeap h);
// gets a pointer to a Maxheap and returns a copy of it (deep-copy), using malloc needs to be freed afterwards.
MaxHeap CopyHeap(MaxHeap heap);

#endif //ASSINGMENT3_MAXHEAP_H
