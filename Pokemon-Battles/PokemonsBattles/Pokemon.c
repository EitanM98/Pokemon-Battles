#include "Pokemon.h"

// Creates new PokemonType, inits the parameters to 0/NULL by default.
PokemonType * init_PokemonType(char name[])
{
	if(!name)
		return NULL;
	PokemonType *type=(PokemonType *)malloc(sizeof(PokemonType));
    if(!type){
        printf("No memory available.\n");
        return NULL;
    }
	type->name=(char *)malloc(strlen(name)+1);
    if(!type->name){
        free(type);
        printf("No memory available.\n");
        return NULL;
    }
	strcpy(type->name,name);
	type->num_of_pokemons=0;
	type->effective_against_me=NULL;
	type->effective_against_others=NULL;
	type->count_eaMe=0;
	type->count_eaOthers=0;
	return type;
}
// Creates BioInfo struct. Returns whether the creation was success/failure(status)
BioInfo * init_BioInfo(double height, double weight, int attack)
{
	if(!height || !weight || !attack)
		return NULL;
	BioInfo *bio=(BioInfo *)malloc(sizeof(BioInfo));
    if(!bio){
        printf("No memory available.\n");
        return NULL;
    }
	bio->attack=attack;
	bio->height=height;
	bio->weight=weight;
	return bio;
}

// creates pokemon instance
Pokemon * init_Pokemon(char name[],char species[],BioInfo *bioInfo,PokemonType *type)
{
	if(!bioInfo || !name || !species || !type )
			return NULL;
	Pokemon *pokemon=(Pokemon *)malloc(sizeof(Pokemon));
    if(!pokemon){
        printf("No memory available.\n");
        return NULL;
    }
	pokemon->name=(char *)malloc(strlen(name)+1);
    if(!pokemon->name){
        free(pokemon);
        printf("No memory available.\n");
        return NULL;
    }
	strcpy(pokemon->name,name);
	pokemon->species=(char *)malloc(strlen(species)+1);
    if(!pokemon->species){
        free(pokemon->name);
        free(pokemon);
        printf("No memory available.\n");
        return NULL;
    }
	strcpy(pokemon->species,species);
	pokemon->details=bioInfo;
	pokemon->type=type;
    pokemon->type->num_of_pokemons++;
	return pokemon;
}

// gets 2 params (A,B) both PokemonType structs adds B to A.effective_against_me array. adds to the end of the array
status add_to_effectiveAgainstMe(PokemonType *addTo, PokemonType *addMe)
{
	if(!addTo || !addMe)
		return failure;
	addTo->count_eaMe+=1;
	addTo->effective_against_me=(PokemonType **)realloc(addTo->effective_against_me,addTo->count_eaMe * sizeof(PokemonType *));
    if(!addTo->effective_against_me){
        printf("No memory available.\n");
        return failure;
    }
	(addTo->effective_against_me)[addTo->count_eaMe-1]=addMe;
	return success;
}

// gets 2 params (A,B) both PokemonType structs adds B to A.effective_against_others array. adds to the end of the array
status add_to_effectiveAgainstOthers(PokemonType *addTo, PokemonType *addMe)
{
	if(!addTo || !addMe)
		return failure;
	addTo->count_eaOthers+=1;
	addTo->effective_against_others=(PokemonType **)realloc(addTo->effective_against_others,addTo->count_eaOthers * sizeof(PokemonType));
    if(!addTo->effective_against_others){
        printf("No memory available.\n");
        return failure;
    }
	(addTo->effective_against_others)[addTo->count_eaOthers-1]=addMe;
	return success;
}

// gets 2 params (A,B) both PokemonType structs deletes B from A.effective_against_me array. maintains the array's order.
status delete_from_effectiveAgainstMe(PokemonType *deleteFrom, PokemonType *deleteMe)
{
	if(!deleteFrom || !deleteMe)
		return failure;
	// searching for type to delete pointer
	for(int i=0;i<deleteFrom->count_eaMe;i++){
		PokemonType *curType=(deleteFrom->effective_against_me)[i];
		if(strcmp(curType->name,deleteMe->name) == 0){
			//maintaining the list's order by moving all the the pointers one place backwards
			for(int j=i;j<deleteFrom->count_eaMe-1;j++){
				(deleteFrom->effective_against_me)[j]=(deleteFrom->effective_against_me)[j+1];
			}
			(deleteFrom->effective_against_me)[deleteFrom->count_eaMe-1]=NULL;
			deleteFrom->count_eaMe-=1;
			deleteFrom->effective_against_me=(PokemonType **)realloc(deleteFrom->effective_against_me
					,deleteFrom->count_eaMe * sizeof(PokemonType) );
			if(deleteFrom->count_eaMe!=0)
                if(!deleteFrom->effective_against_me){
                    printf("No memory available.\n");
                    return failure;
                }
			return success;
		}
	}
	return failure;
}

// gets 2 params (A,B) both PokemonType structs deletes B from A.effective_against_others array. maintains the array's order.
status delete_from_effectiveAgainstOthers(PokemonType *deleteFrom, PokemonType *deleteMe)
{
	if(!deleteFrom || !deleteMe)
		return failure;
	// searching for type to delete pointer
	for(int i=0;i<deleteFrom->count_eaOthers;i++){
		if(strcmp((deleteFrom->effective_against_others)[i]->name,deleteMe->name)==0){
			//maintaining the list's order by moving all the the pointers one place backwards
			for(int j=i;j<deleteFrom->count_eaOthers-1;j++){
				(deleteFrom->effective_against_others)[j]=(deleteFrom->effective_against_others)[j+1];
			}
			(deleteFrom->effective_against_others)[deleteFrom->count_eaOthers-1]=NULL;
			deleteFrom->count_eaOthers-=1;
			deleteFrom->effective_against_others=(PokemonType **)realloc(deleteFrom->effective_against_others
					,deleteFrom->count_eaOthers * sizeof(PokemonType) );
			if(deleteFrom->count_eaOthers!=0)
                if(!deleteFrom->effective_against_others){
                    printf("No memory available.\n");
                    return failure;
                }
			return success;
		}
	}
	return failure;
}

status print_Pokemon(Pokemon *pokemon)
{
	if(!pokemon)
		return failure;
	printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n",
			pokemon->name,pokemon->species,pokemon->type->name, pokemon->details->height,
			pokemon->details->weight, pokemon->details->attack);
	return success;
}

status print_PokemonType(PokemonType *type)
{
	if(!type)
		return failure;
	//buffers to print
	char againstMe[300]="";
	char againstOthers[300]="";
	if(type->count_eaMe>0)
	{
		strcat(againstMe,"\tThese types are super-effective against ");
		strcat(againstMe,type->name);
		strcat(againstMe,":");
		for(int i=0;i<type->count_eaMe;i++){
			strcat(againstMe,(type->effective_against_me)[i]->name);
			// for every type but the last, add " ," to seperate types.
			if(i!=type->count_eaMe-1)
				strcat(againstMe," ,");
			else{
						strcat(againstMe,"\n");
					}
		}
	}
	if(type->count_eaOthers>0)
	{
		//againstOthers string
		strcat(againstOthers,"\t");
		strcat(againstOthers,type->name);
		strcat(againstOthers," moves are super-effective against:");
		for(int i=0;i<type->count_eaOthers;i++){
			strcat(againstOthers,(type->effective_against_others)[i]->name);
			// for every type but the last, add " ," to seperate types.
			if(i!=type->count_eaOthers-1)
				strcat(againstOthers," ,");
			else{
				strcat(againstOthers,"\n");
			}
		}
	}
	printf("Type %s -- %d pokemons\n%s%s\n",type->name,type->num_of_pokemons,againstMe,againstOthers);
	return success;
}

//Free memory functions:
//Frees BioInfo struct, called by free_Pokemon
status free_BioInfo(BioInfo *bio)
{
    free(bio);
    bio=NULL;
    return success;
}
// Frees Pokemon struct
status free_Pokemon(Pokemon *pokemon)
{
	free(pokemon->name);
	pokemon->name=NULL;
	free(pokemon->species);
	pokemon->species=NULL;
	free_BioInfo(pokemon->details);
	free(pokemon);
	pokemon=NULL;
    return success;
}

// Frees PokemonType struct
status free_PokemonType(PokemonType *type)
{
	free(type->name);
	type->name=NULL;
	for(int i=0; i<type->count_eaMe;i++){
		(type->effective_against_me)[i]=NULL;
	}
	for (int i=0;i<type->count_eaOthers; i++){
		(type->effective_against_others)[i]=NULL;
	}
	free(type->effective_against_me);
	type->effective_against_me=NULL;
	free(type->effective_against_others);
	type->effective_against_others=NULL;
	free(type);
	type=NULL;
    return success;
}






