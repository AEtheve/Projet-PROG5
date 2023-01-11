#ifndef _gestion_SECTION_H_
#define _gestion_SECTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gestion_entete.h"
#include "util.h"
#include "elf32.h"


/* Recupere les valeurs de toutes les sections et de l'entete du fichier
nom_fichier et renvoie leurs valeur au format Elf */
Elf* valeurSection(Elf* elf, FILE* f_bin);

/* Affiche les sections contenu dans le fichier elf */
void affichageSection(Elf* elf, bool arm_cmd_version);

/* Recupere les valeurs de section avec valeur_section puis les affiches
avec affichageSection */
void gestion_section(char* nom_fichier, bool arm_cmd_version);

#endif