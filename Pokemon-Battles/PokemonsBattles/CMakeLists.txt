cmake_minimum_required(VERSION 3.21)
project(PokemonsBattles C)

set(CMAKE_C_STANDARD 99)

add_executable(PokemonsBattles MainSecond.c LinkedList.h LinkedList.c BattleByCategory.h Defs.h BattleByCategory.c MaxHeap.h MaxHeap.c tests.c tests.h Pokemon.c Pokemon.h)
