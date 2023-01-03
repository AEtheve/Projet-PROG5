
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "elf32.h"

/* Recupere les valeurs de toutes les sections du fichier nom_fichier */
SectionHeaderStruct* valeur_section(char* nom_fichier);

/* Affiche les valeurs du fichier nom_fichier */
void affichage_section(char* nom_fichier, bool arm_cmd_version);