
lib_LTLIBRARIES = libShell.la libParser.la libJobControl.la

libShell_la_SOURCES = Shell.c Shell.h
libParser_la_SOURCES = Parser.c Parser.h 
libJobControl_la_SOURCES = JobControl.c JobControl.h

bin_PROGRAMS = main
main_SOURCES = main.c 
main_LDADD = libShell.la libParser.la libJobControl.la
