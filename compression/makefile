testComp.out : testComp.c readComp.o
	gcc testComp.c readComp.o -o testComp.out

readComp.o : readComp.c readComp.h
	gcc -c $<
