sm4:sm4.o sm4_code.o sm4.h
	gcc sm4.o sm4_code.o -o sm4 
sm4.o:sm4.c sm4.h
	gcc -c sm4.c -w
sm4_code.o:sm4_code.c sm4.h
	gcc -c sm4_code.c -w
clean:
	rm -f sm4 *.o
