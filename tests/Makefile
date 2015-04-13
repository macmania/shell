CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
#DEBUG=

all: testJobControl


testJobControl: ../JobControl.c ../JobControl.h testJobControl.c
	gcc $(CFLAGS) $(DEBUG)  ../JobControl.c testJobControl.c -o testJobControl
clean:
	rm -f testJobControl *~

