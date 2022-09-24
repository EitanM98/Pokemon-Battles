#include <stdio.h>
#include "BattleByCategory.h"
#include "Pokemon.h"

// helper functions section:

//gets 2 pointers to Pokemon Types returns true if the first is in the second's effective against others list, and false otherwise
bool isType_In_EffectiveAgainstOthers(PokemonType* checkedType,PokemonType* checkMeType)
{
    if(!checkMeType || !checkedType)
        return false;
    for(int i=0;i<checkMeType->count_eaOthers;i++){
        if(strcmp(checkedType->name,checkMeType->effective_against_others[i]->name)==0)
            return true;
    }
    return false;
}

//gets 2 pointers to Pokemon Types returns true if the first is in the second's effective against me list, and false otherwise
bool isType_In_EffectiveAgainstMe(PokemonType* checkedType,PokemonType* checkMeType)
{
    if(!checkMeType || !checkedType)
        return false;
    for(int i=0;i<checkMeType->count_eaMe;i++){
        if(strcmp(checkedType->name,checkMeType->effective_against_me[i]->name)==0)
            return true;
    }
    return false;
}

// helper function, gets Pokemon Type's name and all the PokemonTypes array and returns a pointer the found PokemonType, or NULL if not found
PokemonType* isInTypes(char* type_name, PokemonType** all_types, int numberOfTypes)
{
    if(!type_name || !all_types)
        return NULL;
    for(int i=0;i<numberOfTypes;i++){
        if(strcmp(type_name,all_types[i]->name)==0)
            return all_types[i];
    }
    return NULL;
}

// Pokemon functions for Battle ADT section:

// gets a pointer to a pokemon and returns its Type's name
char* getPokemonsType(Pokemon* pokemon)
{
    if(!pokemon)
        return NULL;
    return pokemon->type->name;
}

// gets a pointer to a pokemon and returns a copy(deep copied) of it, should be freed afterwards
Pokemon* copyPokemon(Pokemon* pokemon)
{
    if(!pokemon)
        return NULL;
    Pokemon* new_pok=(Pokemon*) malloc(sizeof (Pokemon));
    if(!new_pok)
        return NULL;
    new_pok->name=(char*) malloc(strlen(pokemon->name)+1);
    if(!new_pok->name){
        free(new_pok);
        return NULL;
    }
    strcpy(new_pok->name,pokemon->name);
    new_pok->species=(char*) malloc(strlen(pokemon->species)+1);
    if(!new_pok->species){
        free(new_pok->name);
        free(new_pok);
        return NULL;
    }
    strcpy(new_pok->species,pokemon->species);
    new_pok->details= init_BioInfo(pokemon->details->height,pokemon->details->weight,pokemon->details->attack);
    if(!new_pok->details){
        free(new_pok->name);
        free(new_pok->species);
        free(new_pok);
        return NULL;
    }
    new_pok->type=pokemon->type;
    return new_pok;
}


//gets 2 pointers to pokemons and compares their attacks'. returns 1 if first is stronger, -1 if second is stronger and 0 if equal
int comparePokemons(Pokemon* pokemon1,Pokemon* pokemon2)
{
    if(!pokemon1 || !pokemon2)
        return BadArguments;
    int attack1=pokemon1->details->attack;
    int attack2=pokemon2->details->attack;
    if(attack1>attack2)
        return 1;
    if(attack1<attack2)
        return -1;
    return 0;
}

// Gets 2 pointers to pokemons, returns true if they are totally equal (By all attributes) , and false otherwise
bool isEqual(Pokemon* pokemon1,Pokemon* pokemon2)
{
    if(!pokemon1 || !pokemon2)
        return false;
    if(strcmp(pokemon1->name,pokemon2->name)!=0 || strcmp(pokemon1->species,pokemon2->species)!=0 || strcmp(pokemon1->type->name,pokemon2->type->name)!=0)
        return false;
    if(pokemon1->details->weight!= pokemon2->details->weight || pokemon1->details->height!= pokemon2->details->height)
        return false;
    if(comparePokemons(pokemon1,pokemon2)!=0)
        return false;
    return true;
}



// Gets 2 pointers to Pokemons and updates their attack values when matching each other in pointers attack1, attack2.
// Returns attack1-attack2
int getAttack(Pokemon* pokemon1,Pokemon* pokemon2,int* attack1,int* attack2)
{
    if(!pokemon1 || !pokemon2 || !attack1 || !attack2)
        return BadArguments;
    *attack1= pokemon1->details->attack;
    *attack2=pokemon2->details->attack;
    PokemonType* type1=pokemon1->type;
    PokemonType* type2= pokemon2->type;
    // flags to decrease at most 10 points of pokemon's attack in the fight
    bool max10Flag1=false;
    bool max10Flag2=false;
    if(isType_In_EffectiveAgainstOthers(type1,type2))
    {
        *attack1-=10;
        max10Flag1=true;
    }

    if(isType_In_EffectiveAgainstOthers(type2,type1))
    {
        *attack2-=10;
        max10Flag2=true;
    }
    if(!max10Flag2 && isType_In_EffectiveAgainstMe(type1,type2))
        *attack2-=10;
    if(!max10Flag1 && isType_In_EffectiveAgainstMe(type2,type1))
        *attack1-=10;
    return (*attack1-*attack2);
}


// Function to handle reading the data from the data file and Creating our data structure
status read_file(char *file_path, PokemonType** all_types, int num_of_types,int capacityInTypes, Battle* b)
{
    FILE *file;
    file=fopen(file_path,"r");
    if(!file){
        return failure;
    }
    // reading the file by getting line by line to a buffer and iterating through it with a pointer p
    // line iteration done with fgets(), attributes and names extract is being done by strtok() and strtok_r
    char buffer[300];
    char *p;
    //line maximum size
    int max_size=300;
    char *typeName=NULL;
    //detect the word "Types:", to start adding pokemon types
    fgets(buffer,max_size,file);
    while(strcmp(buffer,"Types\r\n")!=0 && strcmp(buffer,"Types\n")!=0)
    {
        fgets(buffer,max_size,file);
    }
    fgets(buffer,max_size,file);
    // detect Types' names
    p=buffer;
    //Creating Battle b with all the Pokemon Types as categories
    *b = createBattleByCategory(capacityInTypes,num_of_types,buffer,(equalFunction )comparePokemons,(copyFunction) copyPokemon,(freeFunction )free_Pokemon,(getCategoryFunction) getPokemonsType, (getAttackFunction) getAttack, (printFunction) print_Pokemon);
    if(!*b)
        return failure;
    // adding every type of the types separated by ","
    for(int i=0;i<num_of_types;i++)
    {
        char *d=",";
        if(i==num_of_types-1)
            d="\r\n";
        if(i==0)
            typeName=strtok(p,d);
        else
            typeName=strtok(NULL,d);
        // Creating a new type and adding it to the all_types array
        all_types[i]=init_PokemonType(typeName);
        if(!all_types[i])
            return failure;
    }

    //effective against me/others lines
    fgets(buffer,max_size,file);
    p=buffer;
    char *against_who=NULL;
    int addTo=0;
    int addMe=0;
    while(strcmp(buffer,"Pokemons\n")!=0 && strcmp(buffer,"Pokemons\r\n")!=0)
    {
        //format: 	Fire effective-against-me:Water,Ground
        //extracting Type to Add by index in all_types dynamic array, and which list to add (effective-against-me/others)
        while(*p=='\t' || *p=='\n' || *p=='\r')
            p++;
        typeName=strtok(p," ");
        for(int i=0;i<num_of_types;i++){
            if(strcmp(all_types[i]->name,typeName)==0){
                addTo=i;
                break;
            }
        }
        strtok(NULL,"-");
        strtok(NULL,"-");
        against_who=strtok(NULL,":");
        //extracting which types to add to the lists by indexes in all_types array
        char *add_us=strtok(NULL,"\0");
        typeName=strtok_r(add_us,"\r\n,",&add_us);
        // keeps adding till the end of the line
        while(typeName && strcmp(typeName,"\r\n")!=0 && strcmp(typeName,"\n")!=0)
        {

            for(int i=0;i<num_of_types;i++){
                if(strcmp(all_types[i]->name,typeName)==0){
                    addMe=i;
                    break;
                }
            }
            if(strcmp(against_who,"me")==0){
                add_to_effectiveAgainstMe(all_types[addTo], all_types[addMe]);
            }
            else {
                add_to_effectiveAgainstOthers(all_types[addTo], all_types[addMe]);
            }
            typeName=strtok_r(add_us,"\r\n,",&add_us);
        }
        fgets(buffer,max_size,file);
        p=buffer;
    }

    // got to line:"Pokemons", now we are extracting Pokemons
    char *name=NULL,*species=NULL,*height=NULL,*weight=NULL,*attack=NULL;
    int typeIndex=0;
    while(fgets(buffer,max_size,file)){
        p=buffer;
        //for each pokemon's attribute
        name=strtok(p,",");
        species=strtok(NULL,",");
        height=strtok(NULL,",");
        weight=strtok(NULL,",");
        attack=strtok(NULL,",");
        typeName=strtok(NULL,"\r\n");
        //find right PokemonType
        for(int j=0;j<num_of_types;j++){
            if(strcmp(all_types[j]->name,typeName)==0){
                typeIndex=j;
                break;
            }
        }
        //Creating new pokemon instance and adding it to the ADT
        Pokemon* new_pok=init_Pokemon(name, species, init_BioInfo(atof(height),atof(weight),atoi(attack)), all_types[typeIndex]);
        //Inserting the pokemon to Battle ADT
        if(!new_pok ||  insertObject(*b,new_pok)==failure)
            return failure;
        free_Pokemon(new_pok);
    }
    fclose(file);
    file=NULL;
    return success;
}

//Main menu helper functions for program functionality

// Gets pointer to Battle, all pokemon types struct and number of types and frees all the memory allocated for the data structure.
status freeMemory(Battle b,PokemonType** all_Types, int numberOfTypes)
{
    if(destroyBattleByCategory(b)==failure)
        return failure;
    for(int i=0;i<numberOfTypes;i++){
        if(free_PokemonType(all_Types[i])==failure)
            return failure;
    }
    free(all_Types);
    all_Types=NULL;
    return success;
}

//gets the quantity of PokemonTypes and Types array and prints each type by the format in pokemon.c
status printAllTypes(int numberOfTypes,PokemonType** all_types)
{
    if(!all_types)
        return failure;
    for(int i=0;i<numberOfTypes;i++)
        print_PokemonType(all_types[i]);
    return success;
}

// Gets input from the user and returns the type if it exists, NULL if not found.
PokemonType* getUsersType(PokemonType** all_Types,int numberOfTypes)
{
    char typesName[300];
    scanf("%s",typesName);
    PokemonType* pokemon_type=isInTypes(typesName,all_Types,numberOfTypes);
    return pokemon_type;
}

// Gets input from the user and create and returns a pointer to a pokemon
Pokemon* createUsersPokemon(PokemonType** all_Types,int numberOfTypes,PokemonType* pokemon_type){
    if(!all_Types || !pokemon_type)
        return NULL;
    char pokemonName[300];
    char pokemonSpecies[300];
    double pokemonHeight, pokemonWeight;
    int pokemonAttack;
    printf("Please enter Pokemon name:\n");
    scanf("%s",pokemonName);
    printf("Please enter Pokemon species:\n");
    scanf("%s",pokemonSpecies);
    printf("Please enter Pokemon height:\n");
    scanf("%lf",&pokemonHeight);
    printf("Please enter Pokemon weight:\n");
    scanf("%lf",&pokemonWeight);
    printf("Please enter Pokemon attack:\n");
    scanf("%d",&pokemonAttack);
    return init_Pokemon(pokemonName,pokemonSpecies, init_BioInfo(pokemonHeight,pokemonWeight,pokemonAttack),pokemon_type);
}

int main(int argc,char* argv[]) {
    //Arguments and Variables init
    int numberOfTypes=atoi(argv[1]);
    int capacityInType=atoi(argv[2]);
    Battle b=NULL;
    // Holds all the pokemon types
    PokemonType** all_Types=(PokemonType**) malloc(sizeof (PokemonType*)*numberOfTypes);
    if(!all_Types){
        printf("No memory available.\n");
        return EXIT_FAILURE;
    }
    // File reading and creating the Data structure
    if(read_file(argv[3],all_Types,numberOfTypes,capacityInType,&b)==failure)
    {
        freeMemory(b,all_Types,numberOfTypes);
        return EXIT_FAILURE;
    }
    //Menu section:
    char choice='1';
    while(choice!='6')
    {
        printf("Please choose one of the following numbers:\n");
        printf("1 : Print all Pokemons by types\n");
        printf("2 : Print all Pokemons types\n");
        printf("3 : Insert Pokemon to battles training camp\n");
        printf("4 : Remove strongest Pokemon by type\n");
        printf("5 : Fight\n");
        printf("6 : Exit\n");
        scanf("%s",&choice);
        Pokemon* usersPokemon=NULL;
        PokemonType* usersType=NULL;
        Pokemon* strongestByCat=NULL;
        Pokemon* fightWinner=NULL;
        numberOfTypes=atoi(argv[1]);
        capacityInType=atoi(argv[2]);
        switch (choice) {
            case '1':
                displayObjectsByCategories(b);
                break;
            case '2':
                printAllTypes(numberOfTypes,all_Types);
                break;
            case '3':
                printf("Please enter Pokemon type name:\n");
                usersType= getUsersType(all_Types,numberOfTypes);
                // Type not exists
                if(!usersType){
                    printf("Type name doesn't exist.\n");
                    break;
                }
                // Type at full capacity:
                if(usersType->num_of_pokemons>=capacityInType){
                    printf("Type at full capacity.\n");
                    break;
                }
                usersPokemon= createUsersPokemon(all_Types,numberOfTypes,usersType);
                if(!usersPokemon){
                    freeMemory(b,all_Types,numberOfTypes);
                    return EXIT_FAILURE;
                }

                if(insertObject(b,usersPokemon)==failure)
                {
                    free_Pokemon(usersPokemon);
                    usersPokemon=NULL;
                    freeMemory(b,all_Types,numberOfTypes);
                    return EXIT_FAILURE;
                }
                //successful insertion
                printf("The Pokemon was successfully added.\n");
                print_Pokemon(usersPokemon);
                free_Pokemon(usersPokemon);
                usersPokemon=NULL;
                break;
            case '4':
                printf("Please enter type name:\n");
                usersType=getUsersType(all_Types,numberOfTypes);
                // Type not exists
                if(!usersType){
                    printf("Type name doesn't exist.\n");
                    break;
                }
                strongestByCat=removeMaxByCategory(b,usersType->name);
                if(!strongestByCat)
                {
                    printf("There is no Pokemon to remove.\n");
                    usersType=NULL;
                    break;
                }
                //successfull removal
                strongestByCat->type->num_of_pokemons--;
                printf("The strongest Pokemon was removed:\n");
                print_Pokemon(strongestByCat);
                free_Pokemon(strongestByCat);
                break;
            case '5':
                printf("Please enter Pokemon type name:\n");
                usersType= getUsersType(all_Types,numberOfTypes);
                // Type not exists
                if(!usersType){
                    printf("Type name doesn't exist.\n");
                    break;
                }
                usersPokemon= createUsersPokemon(all_Types,numberOfTypes,usersType);
                if(!usersPokemon){
                    freeMemory(b,all_Types,numberOfTypes);
                    return EXIT_FAILURE;
                }
                usersPokemon->type->num_of_pokemons--;
                printf("You choose to fight with:\n");
                print_Pokemon(usersPokemon);
                // Fight functionality
                fightWinner=fight(b,usersPokemon);
                // No pokemons in the data structure
                if(!fightWinner){
                    printf("There are no Pokemons to fight against you.\n");
                    free_Pokemon(usersPokemon);
                    break;
                }

                //In case of Winner is from Battle ADT
                if(!isEqual(fightWinner,usersPokemon))
                {
                    free_Pokemon(fightWinner);
                    fightWinner=NULL;
                }
                free_Pokemon(usersPokemon);
                break;
            case '6':
                freeMemory(b,all_Types,numberOfTypes);
                printf("All the memory cleaned and the program is safely closed.\n");
                break;
            default:
                printf("Please choose a valid number.\n");
        }
    }
    return EXIT_SUCCESS;
}
