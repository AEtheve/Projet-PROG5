ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -Werror -g
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG

AUTOMAKE_OPTIONS = subdir-objects

SUBDIRS=
SUBDIRS+=. Examples_fusion

bin_PROGRAMS=
bin_PROGRAMS+=Options_and_debug_example


COMMON=debug.h debug.c util.h util.c

EXTRA_DIST=filtre.pl
Options_and_debug_example_SOURCES=$(COMMON) Options_and_debug_example.c

EXTRA_DIST+=

check_PROGRAMS = test_affichage_entete test_affichage_section test_affichage_contenu_section
test_affichage_entete_SOURCES = ./affichage_entete.c tests/test_affichage_entete.c tests/CuTest/CuTest.c tests/utils.c
test_affichage_section_SOURCES = ./affichage_section.c ./affichage_entete.c tests/test_affichage_section.c tests/CuTest/CuTest.c tests/utils.c
test_affichage_contenu_section_SOURCES = ./affichage_contenu_section.c ./affichage_section.c ./affichage_entete.c tests/test_affichage_contenu_section.c tests/CuTest/CuTest.c tests/utils.c
TESTS = $(check_PROGRAMS)
