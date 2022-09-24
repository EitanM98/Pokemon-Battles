#include "MaxHeap.h"

struct MaxHeap_t{
    int max_size;
    char *name;
    int cur_size;
    copyFunction copyFunc;
    printFunction printFunc;
    equalFunction compFunc;
    freeFunction freeFunc;
    element *heap;
};

//Heap helper functions:
int get_parent(int i){
    if(i%2==0)
        return ((i/2)-1);
    return (int) (floor(i/2));
}

int get_leftChild(int i)
{
    return (2*i+1);
}

int get_rightChild(int i)
{
    return (2*i+2);
}

// Helper function to insert a key to its appropriate place in the heap
status Heap_Increase_Key(MaxHeap h, int i, element lm)
{
    if(!h || i>=h->max_size || !lm)
        return failure;
    h->heap[i]=lm;
    while(i>0 && (h->compFunc(h->heap[get_parent(i)],h->heap[i])==-1))
    {
        element tmp=h->heap[get_parent(i)];
        h->heap[get_parent(i)]=h->heap[i];
        h->heap[i]=tmp;
        i= get_parent(i);
    }
    return success;
}

// Function to rearrange the heap to meet MaxHeap conditions, used in pop from heap
void Max_Heapify(MaxHeap h,int i)
{
    if(!h || h->cur_size==0)
        return;
    int l= get_leftChild(i);
    int r= get_rightChild(i);
    int largest;
    if(l<h->cur_size && (h->compFunc(h->heap[l],h->heap[i])==1))
        largest=l;
    else
        largest=i;
    if(r<h->cur_size && (h->compFunc(h->heap[r],h->heap[largest])==1))
        largest=r;
    if(largest!=i){
        element tmp=h->heap[i];
        h->heap[i]=h->heap[largest];
        h->heap[largest]=tmp;
        Max_Heapify(h,largest);
    }
}

MaxHeap CreateHeap(int max_size,char *name, equalFunction compFunc, printFunction printFunc, copyFunction copyFunc, freeFunction freeFunc)
{
    if(!max_size || !name || !compFunc || !printFunc || !freeFunc || !copyFunc)
        return NULL;
    MaxHeap h=(MaxHeap)malloc(sizeof (struct MaxHeap_t));
    if(!h) {
        NoMemoryReturnNULL()
    }
    h->copyFunc=copyFunc;
    h->freeFunc=freeFunc;
    h->printFunc=printFunc;
    h->compFunc=compFunc;
    h->max_size=max_size;
    h->name=(char *) malloc(strlen(name)+1);
    if(!h->name){
        printf("No memory available.\n");
        free(h);
        h=NULL;
        return NULL;
    }
    strcpy(h->name,name);
    h->heap=(element *) malloc(sizeof (element)*max_size);
    if(!h->heap){
        printf("No memory available.\n");
        free(h->name);
        h->name=NULL;
        free(h);
        h=NULL;
        return NULL;
    }
    h->cur_size=0;
    return h;
}

void destroyHeap(MaxHeap h)
{
    if(!h)
        return;
    free(h->name);
    h->name=NULL;
    for(int i=0;i<h->cur_size;i++)
    {
//        h->printFunc(h->heap[i]);
        h->freeFunc(h->heap[i]);
        h->heap[i]=NULL;
    }
    free(h->heap);
    h->heap=NULL;
    free(h);
    h=NULL;
}

char* getHeapId(MaxHeap h)
{
    if(!h)
        return NULL;
    char* tmp=(char*) malloc(strlen(h->name)+1);
    strcpy(tmp,h->name);
    return tmp;
}

int getHeapCurrentSize(MaxHeap h)
{
    if(!h)
        return -1;
    return h->cur_size;
}

status insertToHeap(MaxHeap h, element lm)
{
    if(!h || !lm || h->cur_size==h->max_size)
        return failure;
    element new_lm=h->copyFunc(lm);
    if(!new_lm)
        return failure;
    if(Heap_Increase_Key(h,h->cur_size,new_lm)==success)
    {
        h->cur_size++;
        return success;
    }
    return failure;
}

MaxHeap CopyHeap(MaxHeap heap)
{
    if(!heap)
        return NULL;
    MaxHeap new_heap=(MaxHeap)malloc(sizeof (struct MaxHeap_t));
    if(!new_heap){
        NoMemoryReturnNULL()
    }
    new_heap->cur_size=heap->cur_size;
    new_heap->compFunc=heap->compFunc;
    new_heap->copyFunc=heap->copyFunc;
    new_heap->freeFunc=heap->freeFunc;
    new_heap->max_size=heap->max_size;
    new_heap->printFunc=heap->printFunc;
    new_heap->name=(char*) malloc((sizeof (strlen(heap->name))+1));
    if(!new_heap->name){
        free(new_heap);
        NoMemoryReturnNULL()
    }
    strcpy(new_heap->name,heap->name);
    new_heap->heap=(element*)malloc(sizeof (element)*heap->max_size);
    if(!new_heap->heap)
    {
        free(new_heap->name);
        free(new_heap);
        NoMemoryReturnNULL()
    }
    // copying all the elements of the heap to the new heap
    for(int i=0;i<heap->cur_size;i++)
    {
        new_heap->heap[i]=heap->copyFunc(heap->heap[i]);
    }
    return new_heap;
}

// Printing Ordered heap from Max to Min, By creating a copy of the heap and popping all the heap(heap sort)
status printHeap(MaxHeap h)
{
    if(!h)
        return failure;
    if(h->cur_size==0){
        printf("%s:\nNo elements.\n\n",h->name);
        return success;
    }
    MaxHeap heapToPrint= CopyHeap(h);
    if(!heapToPrint)
        return failure;
    printf("%s:\n",h->name);
    for (int i=0;i<h->cur_size;i++){
        element max= PopMaxHeap(heapToPrint);
        printf("%d. ",i+1);
        h->printFunc(max);
        h->freeFunc(max);
    }
    destroyHeap(heapToPrint);
    return success;
}

element TopMaxHeap(MaxHeap h)
{
    if(!h || h->cur_size==0)
        return NULL;
    element max=h->copyFunc(h->heap[0]);
    return max;
}

element PopMaxHeap(MaxHeap h)
{
    if(!h || h->cur_size==0)
        return NULL;
    element max=h->copyFunc(h->heap[0]);
    h->freeFunc(h->heap[0]);
    h->heap[0]=h->heap[h->cur_size-1];
    h->heap[h->cur_size-1]=NULL;
    h->cur_size--;
    Max_Heapify(h,0);
    return max;
}


