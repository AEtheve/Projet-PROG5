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
bin_PROGRAMS+=Options_and_debug_example readelf


COMMON=debug.h debug.c util.h util.c

EXTRA_DIST=filtre.pl
Options_and_debug_example_SOURCES=$(COMMON) Options_and_debug_example.c

EXTRA_DIST+=

readelf_SOURCES = ./readelf.c gestion_entete.c gestion_section.c gestion_table_symboles.c elf32.c gestion_tr.c gestion_contenu_section.c

check_PROGRAMS =
check_PROGRAMS += test_gestion_entete
check_PROGRAMS += test_gestion_section
check_PROGRAMS += test_gestion_contenu_section
check_PROGRAMS += test_gestion_ts
check_PROGRAMS += test_gestion_tr

test_gestion_entete_SOURCES = ./gestion_entete.c tests/test_gestion_entete.c tests/CuTest/CuTest.c tests/utils.c elf32.c
test_gestion_section_SOURCES = ./gestion_section.c ./gestion_entete.c tests/test_gestion_section.c tests/CuTest/CuTest.c tests/utils.c elf32.c
test_gestion_contenu_section_SOURCES = ./gestion_contenu_section.c ./gestion_section.c ./gestion_entete.c tests/test_gestion_contenu_section.c tests/CuTest/CuTest.c tests/utils.c elf32.c
test_gestion_ts_SOURCES = ./gestion_table_symboles.c ./gestion_section.c ./gestion_entete.c tests/test_gestion_ts.c tests/CuTest/CuTest.c tests/utils.c elf32.c
test_gestion_tr_SOURCES = ./gestion_tr.c ./gestion_section.c ./gestion_entete.c tests/test_gestion_tr.c tests/CuTest/CuTest.c tests/utils.c elf32.c gestion_table_symboles.c
TESTS = $(check_PROGRAMS)
