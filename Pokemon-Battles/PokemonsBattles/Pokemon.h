
#ifndef POKEMON_H_
#define POKEMON_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Defs.h"

// Pokemon biological Information struct, height in meters, weight in kgs
typedef struct {
double height;
double weight;
int attack;
} BioInfo;

//struct PokemonType contains name, 2 PokemonType's arrays (plus 2 helper attributes of count), and num of
// pokemons of the type. Helper attributes helps reallocating arrays' sizes to maintain dynamic arrays.
typedef struct PokemonType {
	char *name;
	int num_of_pokemons;
	int count_eaMe;
	int count_eaOthers;
	struct PokemonType **effective_against_me;
	struct PokemonType **effective_against_others;
} PokemonType;

//Pokemon struct contains pointers to its' name,species, pokemon's type and his biological information
typedef struct Pokemon {
	char *name;
	char *species;
	PokemonType *type;
	BioInfo *details;
} Pokemon;

//init functions:

// inits a PokemonType struct. get the type's name and Returns pointer to the created instance.
PokemonType* init_PokemonType(char name[]);
// inits BioInfo struct.Gets 3 params and returns pointer to the created instance.
BioInfo* init_BioInfo(double height, double weight, int attack);
// inits Pokemon struct. Returning pointer to the created instance.
Pokemon* init_Pokemon(char name[],char species[],BioInfo *bioInfo,PokemonType *type);

// add/delete effective-against functions
//gets 2 Pointers to PokemonTypes and adds the later to the first's effective_against_me dynamic array.  Returns status (success/failure)
status add_to_effectiveAgainstMe(PokemonType *addTo, PokemonType *addMe);
//gets 2 Pointers to PokemonTypes and adds the later to the first's effective_against_others dynamic array.  Returns status (success/failure)
status add_to_effectiveAgainstOthers(PokemonType *addTo, PokemonType *addMe);
//gets 2 Pointers to PokemonTypes and deletes the later to the first's effective_against_me dynamic array. Returns status (success/failure)
status delete_from_effectiveAgainstMe(PokemonType *deleteFrom, PokemonType *deleteMe);
//gets 2 Pointers to PokemonTypes and deletes the later to the first's effective_against_others dynamic array. Returns status (success/failure)
status delete_from_effectiveAgainstOthers(PokemonType *deleteFrom, PokemonType *deleteMe);

//Print functions:
// Gets a pointer to a pokemon and prints all its' attributes.  Returns status (success/failure)
status print_Pokemon(Pokemon *pokemon);
// Gets a pointer to a pokemonType and prints all its' attributes.  Returns status (success/failure)
status print_PokemonType(PokemonType *type);

// FREE functions declarations
//frees BioInfo struct incstance, called by free_Pokemon function
status free_BioInfo(BioInfo *bio);
//frees Pokemon struct instance
status free_Pokemon(Pokemon *pokemon);
// frees PokemonType struct. Nullifies all the pointers in effective_against arrays and all_pokemons pointers;
status free_PokemonType(PokemonType *type);

#endif /* POKEMON_H_ */
