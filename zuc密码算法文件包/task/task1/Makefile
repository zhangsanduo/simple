zuc:zucreference.o zuc.o zuc.h
	gcc zucreference.o zuc.o -o zuc -w
zuc.o:zuc.c zuc.h
	gcc -c zuc.c -w
zucreference.o:zucreference.c zuc.h
	gcc -c zucreference.c -w
clean:
	rm -f zuc *.o
