TESTCFLAGS=-g -Wall -Werror
TESTSOURCES=main.c sha1.c debug.c
TESTBINARY=sha1

.FORCE: main
all:
	make main

clean:
	rm -rf *.o $(TESTBINARY)

main:
	$(CC) $(TESTCFLAGS) -o $(TESTBINARY) $(TESTSOURCES)
#	./$(TESTBINARY)	
