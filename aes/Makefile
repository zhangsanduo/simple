main:main.o aes.h aes.o
	gcc main.o aes.o -o main
main.o:main.c aes.h
	gcc -c main.c
aes.o:aes.c aes.h
	gcc -c aes.c
clean:
	rm -f main *.o 
