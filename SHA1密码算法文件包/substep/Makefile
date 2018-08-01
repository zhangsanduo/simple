#TESTCFLAGS=-g -Wall -Werror
TESTCFLAGS=-g -Wall
TESTLINKER=-lcunit
TESTSOURCES=sha1_code.c sha1.c debug.c
TESTBINARY=sha1

.FORCE: main
all:
	make main

clean:
	rm -rf *.o $(TESTBINARY)

main:
	$(CC) $(TESTCFLAGS) -o $(TESTBINARY) $(TESTSOURCES) $(TESTLINKER)
#	./$(TESTBINARY)	
