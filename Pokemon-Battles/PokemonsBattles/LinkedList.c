#include "LinkedList.h"


// Node is a pointer to Node_t struct
typedef struct Node_t* Node;

//Node_t struct of the LinkedList has its data and pointer to the next node and prev node
struct Node_t {
    element data;
    Node next;
    Node prev;
};
// Init node gets an element and creates and returns a node with the element as data and pointer to next element as NULL
Node init_Node(element lm, copyFunction copyFunc)
{
    if(!lm || !copyFunc)
        return NULL;
    Node node= (Node)malloc(sizeof (struct Node_t));
    if(!node){
        NoMemoryReturnNULL()
    }
    node->data=copyFunc(lm);
    node->next=NULL;
    node->prev=NULL;
    return node;
}
//gets a node and frees all its memory allocations
void freeNode(Node node,freeFunction freeFunc)
{
    if(!node)
        return;
    freeFunc(node->data);
    node->data=NULL;
    free(node);
    node=NULL;
}

//Linked List struct has: first and last Nodes' pointers,
// 4 basic functions(free,copy,print,compare) and counter for its size
struct LinkedList_t{
    Node first;
    Node last;
    copyFunction copyFunc;
    freeFunction freeFunc;
    equalFunction compFunc;
    printFunction printFunc;
    CheckKeyFunction checkKeyFunc;
    int list_size;
};

LinkedList createLinkedList(copyFunction copyFunc, freeFunction freeFunc, CheckKeyFunction checkKeyFunction, printFunction printFunc,equalFunction compFunc)
{
    if(!copyFunc || !freeFunc || !compFunc || !checkKeyFunction || !printFunc )
        return NULL;
    LinkedList l=(LinkedList) malloc(sizeof(struct LinkedList_t));
    if(!l) {
        NoMemoryReturnNULL()
    }
    l->freeFunc=freeFunc;
    l->copyFunc=copyFunc;
    l->compFunc=compFunc;
    l->printFunc=printFunc;
    l->checkKeyFunc=checkKeyFunction;
    l->first=NULL;
    l->last=NULL;
    l->list_size=0;
    return l;
}

void destroyList(LinkedList l)
{
    if(!l)
        return;
    Node cur=l->first;
    // Free all the nodes in the List
    while(cur!=NULL)
    {
        Node tmp=cur->next;
        freeNode(cur,l->freeFunc);
        cur=tmp;
    }
    free(l);
    l=NULL;
}

status appendNode(LinkedList l, element element)
{
    if(!l || !element)
        return failure;
    Node newNode= init_Node(element,l->copyFunc);
    if(!newNode)
        return failure;
    if(!l->first){
        l->first=newNode;
        l->last=newNode;
        l->list_size++;
        return success;
    }
    newNode->prev=l->last;
    l->last->next=newNode;
    l->last=newNode;
    l->list_size++;
    return success;
}

status deleteNode(LinkedList l, element element)
{
    if(!l || !element || !l->first)
        return failure;
    Node cur=l->first;
    if(l->compFunc(cur->data,element)== 0){
        l->first=cur->next;
        freeNode(cur,l->freeFunc);
        cur=NULL;
        l->list_size--;
        if(l->list_size==0)
            l->last=NULL;
        return success;
    }
    while(cur!=NULL)
    {
        if(l->compFunc(cur->data,element)== 0){
            if(cur->next==NULL){
                l->last=cur->prev;
                l->list_size--;
                if(l->list_size==1)
                    l->first=cur->prev;
                freeNode(cur,l->freeFunc);
                cur=NULL;
                return success;
            }
            cur->prev->next=cur->next;
            freeNode(cur,l->freeFunc);
            cur=NULL;
            l->list_size--;
            return success;
        }
        cur=cur->next;
    }
    return failure;
}

void displayList(LinkedList l)
{
    if(!l)
        return;
    Node cur=l->first;
    while(cur !=NULL)
    {
        l->printFunc(cur->data);
        cur=cur->next;
    }
}


element searchByKeyInList(LinkedList l,element key)
{
    if(!l || !key)
        return NULL;
    Node cur=l->first;
    while(cur!=NULL)
    {
        if(l->checkKeyFunc(cur->data,key)== true)
            return cur->data;
        cur=cur->next;
    }
    return NULL;
}
