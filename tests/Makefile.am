TESTS = check_shell check_job_control check_parser
check_PROGRAMS = check_shell check_job_control check_parser

check_shell_SOURCES = check_shell.c $(top_builddir)/src/Shell.h $(top_builddir)/src/Parser.h $(top_builddir)/src/JobControl.h 
check_shell_CFLAGS = @CHECK_CFLAGS@
check_shell_LDADD = $(top_builddir)/src/libShell.la @CHECK_LIBS@

check_job_control_SOURCES = check_job_control.c $(top_builddir)/src/JobControl.h $(top_builddir)/src/Parser.h
check_job_control_CFLAGS = @CHECK_CFLAGS@
check_job_control_LDADD = $(top_builddir)/src/libParser.la $(top_builddir)/src/libJobControl.la  @CHECK_LIBS@

check_parser_SOURCES = check_parser.c $(top_builddir)/src/Parser.h
check_parser_CFLAGS = @CHECK_CFLAGS@
check_parser_LDADD = $(top_builddir)/src/libParser.la @CHECK_LIBS@

