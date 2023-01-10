#ifndef _AFFICHAGE_TR_H_
#define _AFFICHAGE_TR_H_


#include "affichage_table_symboles.h"


/* Fonction qui renvoie le type accordé a la valeur info de l'entree */
char *reloc_type(uint32_t valeur);

/* Fonction qui renvoie le nombre de reimplementation */
int getNbRelocSec(Elf* elf);

/* Fonction qui prend un ELF et un fichier ouvert en parametre et renvoie l'ELF completer des tables de reimplementation */
Elf* getTableRelocation(Elf *elf, FILE* f_bin);

/* Fonction d'affichage des tables de réimplémentation */
void affichageTableReimplentation(Elf *elf);

/* Fonction main qui gere l'ouverture ainsi que l'affichage de la table de réimplémentation */
void affichage_table_reimplentation(char *nom_fichier);

#endif