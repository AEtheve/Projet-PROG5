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
#include "gestion_entete.h"
#include "gestion_section.h"
#include "gestion_table_symboles.h"
#include "gestion_contenu_section.h"
#include "gestion_tr.h"

void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ --help ] [ --f <fichier> ] [ --h ] [ --S ] [ --s ] [ --x <section> ] [ --r ]\n", name);
}

int main(int argc, char *argv[]) {
	int opt;
	char *header_flag, *section_flag, *symbol_flag, *relocation_flag, *contenu_section, *file;

	struct option longopts[] = {
		{ "h", no_argument, NULL, '1' },
		{ "S", no_argument, NULL, '2' },
		{ "s", no_argument, NULL, '3' },
		{ "x", required_argument, NULL, '4' },
		{ "r", no_argument, NULL, '5' },
		{ "f", required_argument, NULL, 'f'},
        { "help", no_argument, NULL, 'h'},
		{ NULL, 0, NULL, 0 }
	};
	file = NULL;
	header_flag = NULL;
	section_flag = NULL;
	symbol_flag = NULL;
    contenu_section = NULL;

	while ((opt = getopt_long(argc, argv, "1:2:3:4:5:s:f:h", longopts, NULL)) != -1) {
		switch(opt) {
		case '1':
			header_flag = "true";
			break;
		case '2':
			section_flag = "true";
			break;
        case '3':
			symbol_flag = "true";
			break;
		case '4':
			contenu_section = optarg;
			break;
		case '5':
			relocation_flag = "true";
			break;
		case 'f':
			file = optarg;
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

	if (file == NULL) {
		fprintf(stderr, "Il faut spécifier un fichier objet à charger avec le flag --f !\n\n");
		usage(argv[0]);
		exit(1);
	}

	if (header_flag != NULL) {
		gestion_entete(file);
	}
	if (section_flag != NULL) {
		gestion_section(file, false);
	}
	if (symbol_flag != NULL) {
		gestion_table_symboles(file, false);
	}
	if (contenu_section != NULL) {
		int select_section = atoi(contenu_section);
		gestion_contenu_section(file, select_section);
	}
	if (relocation_flag != NULL) {
		gestion_table_reimplentation(file);
	}
	


	return 0;
}