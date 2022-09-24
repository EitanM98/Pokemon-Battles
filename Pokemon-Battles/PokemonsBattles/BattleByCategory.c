#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"

struct battle_s{
    LinkedList categoriesList;
    char* allCategories;
    int numOfCategories;
    equalFunction equalFunc;
    copyFunction copyFunc;
    freeFunction freeFunc;
    getCategoryFunction getCategoryFunc;
    getAttackFunction getAttackFunc;
    printFunction printFunc;
};

// Defining how to find a heap in the Linked List, gets a MaxHeap and a name and returns true if heap's name is the same
// as the argument. Otherwise return false
bool findByName(MaxHeap category, char* name) {
    if (!category || !name)
        return false;
    char* cat_name = getHeapId(category);
    if (strcmp(cat_name, name) == 0)
    {
        free(cat_name);
        return true;
    }
    free(cat_name);
    return false;
}

//equal function for LinkedList of MaxHeaps. Checks weather 2 heaps have the same name. which means it is the same heap. Returns 0 if they do, and 1 otherwise.
int equalHeaps(MaxHeap heap1,MaxHeap heap2)
{
    if(!heap1 || !heap2)
        //flag for NULL element's pointer
        return BadArguments;
    char* name1= getHeapId(heap1);
    char* name2= getHeapId(heap2);
    int res= strcmp(name1, name2);
    free(name1);
    free(name2);
    if(res!=0)
        return 1;
    return res;
}

// Gets all the needed functions to be a generic ADT and returns Battle instance. Using a linkedList that holds MaxHeaps
// in the Nodes.
Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement)
{
    if(!categories || !equalElement || !copyElement || !freeElement || !getCategory || !getAttack || !printElement )
        return NULL;
    Battle b=(Battle) malloc(sizeof (struct battle_s));
    if(!b) {
        NoMemoryReturnNULL()
    }
    b->numOfCategories=numberOfCategories;
    b->equalFunc=equalElement;
    b->freeFunc=freeElement;
    b->copyFunc=copyElement;
    b->getAttackFunc=getAttack;
    b->getCategoryFunc=getCategory;
    b->printFunc=printElement;
    b->allCategories= (char*)malloc((strlen(categories))+1);
    if(!b->allCategories){
        free(b);
        NoMemoryReturnNULL()
    }
    strcpy(b->allCategories,categories);
    b->categoriesList= createLinkedList((copyFunction)CopyHeap, (freeFunction) destroyHeap, (CheckKeyFunction)findByName , (printFunction)printHeap, (equalFunction) equalHeaps);
    if(!b->categoriesList){
        free(b->allCategories);
        free(b);
        return NULL;
    }
    //tmp all categories string for creation of heaps
    char* tmp= malloc((strlen(b->allCategories))+1);
    if(!tmp)
    {
        destroyList(b->categoriesList);
        free(b->allCategories);
        return NULL;
    }
    strcpy(tmp,b->allCategories);
    // Deriving all the categories to create matching heaps
    char* catName= strtok(tmp,",\r\n\t\v\f");
    for(int i=0;i<numberOfCategories;i++){
        MaxHeap new_cat= CreateHeap(capacity,catName,equalElement,printElement,copyElement,freeElement);
        if(!new_cat){
            destroyList(b->categoriesList);
            free(b->allCategories);
            free(b);
            return NULL;
        }
        if(appendNode(b->categoriesList, new_cat) != success){
            destroyHeap(new_cat);
            destroyList(b->categoriesList);
            free(b->allCategories);
            free(b);
            free(tmp);
            return NULL;
        }
        destroyHeap(new_cat);
        catName= strtok(NULL,",\r\n\t\v\f");
    }
    free(tmp);
    tmp=NULL;
    return b;
}

// Gets a pointer to Battle and frees all the memory allocated to it.
status destroyBattleByCategory(Battle b)
{
    if(!b)
        return failure;
    free(b->allCategories);
    b->allCategories=NULL;
    destroyList(b->categoriesList);
    b->categoriesList=NULL;
    free(b);
    b=NULL;
    return success;
}

// Gets an element and a Battle and inserts a copy of the element to the battle
status insertObject(Battle b, element elem)
{
    if(!b || !elem)
        return failure;
    char* cat_name=b->getCategoryFunc(elem);
    char* category=(char*)malloc(sizeof (strlen(cat_name))+1);
    if(!category) {
        printf("No memory available.\n");
        return failure;
    }
    strcpy(category,cat_name);
    element found_cat=searchByKeyInList(b->categoriesList,category);
    if(!found_cat)
    {
        free(category);
        return failure;
    }
    if(insertToHeap((MaxHeap)found_cat,elem)!=success){
        free(category);
        return failure;
    }
    free(category);
    return success;
}

// Prints all the Battle's elements by categories and sorted from High to low
void displayObjectsByCategories(Battle b)
{
    if(!b)
        return;
    displayList(b->categoriesList);
}

//Gets category name and removes its biggest element, if the category is empty returns NULL
element removeMaxByCategory(Battle b,char* category)
{
    if(!b || !category)
        return NULL;
    element found_cat=searchByKeyInList(b->categoriesList,category);
    if(!found_cat)
        return NULL;
    return PopMaxHeap((MaxHeap)found_cat);
}

// Gets Battle argument and name of category and returns the number of elements in that category
int getNumberOfObjectsInCategory(Battle b,char* category)
{
    if(!b || !category)
        return BadArguments;
    element found_cat=searchByKeyInList(b->categoriesList,category);
    if(!found_cat)
        return BadArguments;
    return getHeapCurrentSize((MaxHeap)found_cat);
}

// Gets an element and a Battle and compares the strongest element in the Battle ADT to face the element.
// compares by their attacks, returns the stronger element b, (if there is a DRAW the battle element will be returned).
element fight(Battle b,element elem)
{
    if(!b || !elem )
        return NULL;
    char* tmp=(char*)malloc(strlen(b->allCategories)+1);
    if(!tmp){
        NoMemoryReturnNULL()
    }
    strcpy(tmp,b->allCategories);
    //variables init for fight comparison
    int *cur_dif;
    int battle_attack_rate, vs_attack_rate,fight_res;
    int tmp1=0;
    int *battle_best_attack=&tmp1;
    int tmp2=0;
    int  *vs_best_attack=&tmp2;
    //flag for first comparison between opponent to Battle element
    int flag=-999;
    int* best=&flag;
    element strongest=NULL;
    // Iterating through all the categories
    char* cat_name= strtok(tmp,",\r\n\t\v\f");
    for(int i=0;i<b->numOfCategories;i++){
        element found_cat=searchByKeyInList(b->categoriesList,cat_name);
        if(!found_cat){
            free(tmp);
            return NULL;
        }
        element cur_strongest= TopMaxHeap((MaxHeap)found_cat);
        //No elements in the current category
        if(!cur_strongest)
        {
            cat_name= strtok(NULL,",\r\n\t\v\f");
            continue;
        }

        // difference between current category the strongest attack minus opponents' attack
        fight_res=b->getAttackFunc(cur_strongest,elem,&battle_attack_rate,&vs_attack_rate);
        cur_dif=&fight_res;
        if(*best==-999 || *cur_dif>*best){
            *best=*cur_dif;
            *battle_best_attack=battle_attack_rate;
            *vs_best_attack=vs_attack_rate;
            // strongest holds a copy of the current strongest, now we want to change it; We free the current element and copy the stronger element
            if(strongest)
                b->freeFunc(strongest);
            strongest=b->copyFunc(cur_strongest);
        }
        cat_name= strtok(NULL,",\r\n\t\v\f");
        //freeing of TopMaxHeap result (which is a copy of the element)
        b->freeFunc(cur_strongest);
    }
    free(tmp);
    // No elements were found in battle ADT
    if(*best==-999)
        return NULL;
    //Competitors details
    printf("The final battle between:\n");
    b->printFunc(elem);
    printf("In this battle his attack is :%d\n\nagainst ",*vs_best_attack);
    b->printFunc(strongest);
    printf("In this battle his attack is :%d\n\n",*battle_best_attack);
    //Draw
    if(*best==0){
        printf("IT IS A DRAW.\n");
        return strongest;
    }
    printf("THE WINNER IS:\n");
    // Battle element is stronger
    if(*best>0) {
        b->printFunc(strongest);
        return strongest;
    }
    // Opponent element is stronger
    else {
        b->freeFunc(strongest);
        strongest=NULL;
        b->printFunc(elem);
        return elem;
    }
}
