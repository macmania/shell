TESTS = check_shell
check_PROGRAMS = check_shell

check_shell_SOURCES = check_shell.c $(top_builddir)/src/Shell.h $(top_builddir)/src/Parser.h $(top_builddir)/src/JobControl.h 
check_shell__CFLAGS = @CHECK_CFLAGS@
check_shell_LDADD = $(top_builddir)/src/libShell.la @CHECK_LIBS@
