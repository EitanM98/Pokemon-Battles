PokemonsBattles: LinkedList.o MaxHeap.o Pokemon.o BattleByCategory.o MainSecond.o
	gcc LinkedList.o MaxHeap.o Pokemon.o BattleByCategory.o MainSecond.o -o PokemonsBattles
MainSecond.o: MainSecond.c Pokemon.h Defs.h MaxHeap.h LinkedList.h BattleByCategory.h	
	gcc -c MainSecond.c
LinkedList.o: LinkedList.c Defs.h LinkedList.h
	gcc -c LinkedList.c
MaxHeap.o: MaxHeap.c Defs.h MaxHeap.h
	gcc -c MaxHeap.c
Pokemon.o: Pokemon.c Pokemon.h Defs.h
	gcc -c Pokemon.c
BattleByCategory.o: BattleByCategory.c BattleByCategory.h MaxHeap.h LinkedList.h Defs.h
	gcc -c BattleByCategory.c
clean: 
	rm -f *.o PokemonsBattles
