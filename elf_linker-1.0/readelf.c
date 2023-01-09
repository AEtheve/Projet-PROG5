/*
ELF Loader - chargeur/implanteur d'exécutables au format ELF à but pédagogique
Copyright (C) 2012 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
         ENSIMAG - Laboratoire LIG
         51 avenue Jean Kuntzmann
         38330 Montbonnot Saint-Martin
*/
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "affichage_entete.h"
#include "affichage_section.h"
#include "affichage_table_symboles.h"
#include "affichage_tr.h"

void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ --help ] [ --h file ] [ --S file ] [ --s file ] [ --x file ]\n\n"
		, name);
}

int main(int argc, char *argv[]) {
	int opt;
	char *option1, *option2, *option3, *option4;

	struct option longopts[] = {
		{ "h", required_argument, NULL, '1' },
		{ "S", required_argument, NULL, '2' },
		{ "s", required_argument, NULL, '3' },
		{ "x", required_argument, NULL, '4' },
        { "help", no_argument, NULL, 'h'},
		{ NULL, 0, NULL, 0 }
	};

	option1 = NULL;
	option2 = NULL;
    option3 = NULL;
    option4 = NULL;
	while ((opt = getopt_long(argc, argv, "1:2:3:4:h", longopts, NULL)) != -1) {
		switch(opt) {
		case '1':
			option1 = optarg;
			affichage_entete(option1);
			break;
		case '2':
			option2 = optarg;
            affichage_section(option2,false);
			break;
        case '3':
            option3 = optarg;
            affichage_table_symboles(option3,false);
            break;
        case '4':
            option4 = optarg;
            affichage_table_reimplentation(option4);
            break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}

	return 0;
}