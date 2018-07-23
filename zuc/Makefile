main:main.o zuc.o zuc.h
	gcc main.o zuc.o -o main -w
zuc.o:zuc.c zuc.h
	gcc -c zuc.c -w
main.o:main.c zuc.h
	gcc -c main.c -w
clean:
	rm -f main *.o
