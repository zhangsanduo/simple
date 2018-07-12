main:main.o aes.h aes.o
	gcc main.o aes.o -o main -w
main.o:main.c aes.h
	gcc -c main.c -w
aes.o:aes.c aes.h
	gcc -c aes.c -w
clean:
	rm -f main *.o 
