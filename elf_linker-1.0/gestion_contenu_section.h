#ifndef _GESTION_CONTENU_SECTION_H_
#define _GESTION_CONTENU_SECTION_H_

#include "elf32.h"

void affichageContenuSection(Elf* elf, FILE* f_bin, int select_section);

void gestion_contenu_section(char* nom_fichier, int select_section);

#endif