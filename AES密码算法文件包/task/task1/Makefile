aes:aes.o aes.h aes_code.o
	gcc aes.o aes_code.o -o aes
aes.o:aes.c aes.h
	gcc -c aes.c
aes_code.o:aes_code.c aes.h
	gcc -c aes_code.c
clean:
	rm -f aes *.o 
