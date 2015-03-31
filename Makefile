CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
#DEBUG=

all: shell


shell:	shell.c shell.h parser.c parser.h 
	gcc $(CFLAGS) $(DEBUG) shell.c parser.c -o shell
clean:
	rm -f shell *~

