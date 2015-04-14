CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g

all: testSignalHandling

testSignalHandling: ../testSignalHandling.c 
	gcc $(CFLAGS) $(DEBUG) ../testSignalHandling.c  -o testSignalHandling

clean: 
	rm -f testSignalHandling *~



