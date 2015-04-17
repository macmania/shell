CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
#DEBUG=

all: shell


shell:	Shell.c Shell.h Parser.c Parser.h 
	gcc $(CFLAGS) $(DEBUG) Shell.c Parser.c -o shell
clean:
	rm -f shell *~

