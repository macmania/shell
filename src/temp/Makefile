CFLAGS=-DUNIX -lreadline -lcurses 
DEBUG=-g
#DEBUG=

all: shell parser


shell: ../Shell.c ../Shell.h ../JobControl.c ../JobControl.h ../Parser.c ../Parser.h ../main.c
	gcc $(CFLAGS) $(DEBUG)  ../Shell.c ../JobControl.c ../Parser.c ../main.c -o shell

parser: ../JobControl.c ../JobControl.h ../Parser.c ../Parser.h ../test_parser.c
	gcc $(CFLAGS) $(DEBUG)  ../JobControl.c ../Parser.c ../test_parser.c -o parser

clean:
	rm -f shell parser *~

