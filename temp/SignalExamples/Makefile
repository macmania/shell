CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
#DEBUG=

all: SigHandler

SigHandler: signalEX5.c
	gcc $(CFLAGS) $(DEBUG)  signalEX5.c -o SigHandler
clean:
	rm -f SigHandler *~

