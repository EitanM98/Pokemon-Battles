#ifndef ASSINGMENT3_DEFS_H
#define ASSINGMENT3_DEFS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum e_bool { false,true } bool;
//BadArguments is status returned in int function's failure
typedef enum e_status { success, failure,BadArguments=-999} status;
typedef void* element;
typedef element(*copyFunction) (element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);
typedef int(*equalFunction) (element, element);//1 first bigger, 0 - same, -1 second bigger
typedef char*(*getCategoryFunction)(element);
typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond); // return the first attack - second attack . also produce the attack of each element, The function transmits the information through the pointers received in the function.
//Macro for memory allocation that failed
#define NoMemoryReturnNULL() {\
printf("No memory available.\n");\
return NULL;\
}

#endif //ASSINGMENT3_DEFS_H
