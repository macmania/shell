#!/bin/sh
AC_INIT([Shell], [1.0], [jojofabe@gmail.com], 
		[Shell], [http://macmania.github.io/])
AC_PREREQ([2.65])
AM_PROG_AR

AC_CONFIG_AUX_DIR([build-aux])
#AC_CONFIG_SRCDIR([src/main.c src/Shell.c])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC
AC_PROG_LIBTOOL
PKG_CHECK_MODULES([CHECK], [check >= 0.9.6])
AC_HEADER_STDC
echo "Testing for a C compiler"
AC_LANG(C)
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT

