#ifndef _FUSION_SECTION_H_
#define _FUSION_SECTION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "affichage_section.h"
#include "elf32.h"

/* Trouve l'indice de la fonction de nom name et renvoie son indice */
int findSection(Elf* elf, const char* name);

/* Retourne une structure ELF contenant la fusion des sections des deux structures ELF passées en argument */
Elf* fusionSection(Elf* elf1, Elf* elf2);

/* Fonction d'affichage de la fusion des sections des fichiers nom_fichier1 et nom_fichier2 */
void affichageTestFusionSection(char* nom_fichier1, char* nom_fichier2, bool arm_cmd_version);

#endif