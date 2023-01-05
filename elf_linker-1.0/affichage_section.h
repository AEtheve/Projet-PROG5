
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "elf32.h"

/* Recupere les valeurs de toutes les sections et de l'entete du fichier
nom_fichier et renvoie leurs valeur au format Elf */
Elf* valeur_section(char* nom_fichier);

/* Affiche les sections contenu dans le fichier elf */
void affichageSection(Elf* elf, bool arm_cmd_version);

/* Recupere les valeurs de section avec valeur_section puis les affiches
avec affichageSection */
void affichage_section(char* nom_fichier, bool arm_cmd_version);