all:rsa multiple
rsa:rsa.o rsareference.o rsa.h 
	gcc rsareference.o rsa.o -o rsa -w
rsareference.o:rsareference.c rsa.h
	gcc -c rsareference.c -w
rsa.o:rsa.c rsa.h
	gcc -c rsa.c -w
multiple:multiple.o
	gcc multiple.o -o multiple -w
multiple.o:multiple.c
	gcc -c multiple.c -w
clean:
	rm -f rsa multiple *.o
