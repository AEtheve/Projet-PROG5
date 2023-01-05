#ifndef _AFFICHAGE_CONTENU_SECTION_H_
#define _AFFICHAGE_CONTENU_SECTION_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf32.h"

void affichageContenuSection(Elf* elf, FILE* f_bin, int select_section);

void affichage_contenu_section(char* nom_fichier, int select_section);

#endif