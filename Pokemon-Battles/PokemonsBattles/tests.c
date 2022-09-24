#include "tests.h"


int test_num=0;

element copyInt(element num)
{
    int *newNum=(int*) malloc(sizeof (int ));
    if(!newNum)
        return NULL;
    *newNum=*(int *)num;
    return newNum;
};

status freeInt(element num){
    if(!num)
        return failure;
    free((int*) num);
    num=NULL;
    return success;
};

status printInt(element num){
    if(!num)
        return failure;
    printf("%d \n",*(int*)num);
    return success;
};

int compInt(element num1,element num2){
    if(!num1 || !num2)
        return -999;
    if(*(int*)num1==*(int*)num2)
        return 0;
    if(*(int*)num1<*(int*)num2)
        return -1;
    else
        return 1;
};

bool checkKey(element num,element key){
    if(!num || !key)
        return false;
    if(*(int*)num == *(int*)key)
        return true;
    return false;
};

//vars declaration
int x1=8;
int x2=10;
int x3=13;
int x4=15;
int x5=3;
int notFound=-1;

void LinkedListTest()
{

    LinkedList ll=createLinkedList(copyInt,freeInt,checkKey,printInt,compInt);
    test_num++;
    if(!ll)
        printf("test number: %d failed\n", test_num);
    appendNode(ll,&x1);
    appendNode(ll,&x2);
    appendNode(ll,&x3);
    appendNode(ll,&x4);
    appendNode(ll,&x5);
//    displayList(ll);
    test_num++;
    if(deleteNode(ll,&x3)!=success)
        printf("test number: %d failed\n", test_num);
    displayList(ll);
    test_num++;
    int* search_res=searchByKeyInList(ll,&notFound);
    int* search_res2=searchByKeyInList(ll,&x5);
    printf("found key is:\n");
    printInt(search_res2);
    test_num++;
    if(search_res)
        printf("test number: %d failed\n", test_num);
    destroyList(ll);
};

void testMaxHeap()
{
    int max_size=7;
    char* heap_name="Integers";
    MaxHeap h1=CreateHeap(max_size,heap_name,compInt,printInt,copyInt,freeInt);
    test_num++;
    if(!h1)
        printf("test number: %d failed\n", test_num);
    insertToHeap(h1,&x2);
    insertToHeap(h1,&x3);
    insertToHeap(h1,&x1);
    insertToHeap(h1,&x4);
    insertToHeap(h1,&x5);
//    printHeap(h1);
//    printf("heap name is: %s\n",getHeapId(h1));
//    printf("Top element is: ");
//    printInt(TopMaxHeap(h1));
//    printf("Pop element is: ");
//    printInt(PopMaxHeap(h1));
//    printf("heap size is: %d\n",getHeapCurrentSize(h1));
    free(PopMaxHeap(h1));
    free(PopMaxHeap(h1));
    printHeap(h1);
    destroyHeap(h1);
}

void mainTest()
{
//    LinkedListTest();
    testMaxHeap();
    printf("Finished testing");
};