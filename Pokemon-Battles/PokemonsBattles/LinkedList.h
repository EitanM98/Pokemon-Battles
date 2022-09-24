
#ifndef ASSINGMENT3_LINKEDLIST_H
#define ASSINGMENT3_LINKEDLIST_H
#include "Defs.h"


typedef struct LinkedList_t* LinkedList;
// Function that gets an element and compares its requested attribute to the given key. Returns true/false
typedef bool (*CheckKeyFunction)(element,element);
// gets 5 functions to be able to function as Generic LinkedList and returns a pointer to a LinkedList instance
LinkedList createLinkedList(copyFunction copyFunc, freeFunction freeFunc, CheckKeyFunction checkKeyFunction, printFunction printFunc,equalFunction compFunc);
// Destroys the linkedList and frees all the memory allocated to it
void destroyList(LinkedList l);
//gets a LinkedList and an Element which is appended to the end of the list,Returns success/failure
status appendNode(LinkedList l, element element);
// gets a pointer to a node and deletes it from the linked list. Returns success/failure
status deleteNode(LinkedList l, element element);
//gets a pointer to the linked list and prints all its Nodes
void displayList(LinkedList l);
//gets a pointer to a linked list and a key to filter&search by, returns the first node to match the key or NULL if not found
element searchByKeyInList(LinkedList l,element key);

#endif //ASSINGMENT3_LINKEDLIST_H
