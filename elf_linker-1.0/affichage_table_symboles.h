#ifndef _AFFICHAGE_TABLE_SYMBOLES_H_
#define _AFFICHAGE_TABLE_SYMBOLES_H_


#include "affichage_section.h"

/* Fonction qui renvoie la position de la table des symboles dans le section_header (table des sections) */
int findSymTab(Elf *elf);

/* Fonction qui renvoie le type du symbole */
char *afficheType(ElfSymbole *ts, int num);

/* Fonction qui renvoie le bind du symbole */
char *afficheBind(ElfSymbole *ts, int num);

/* Fonction qui renvoie le vis du symbole */
char *afficheVis(ElfSymbole *ts, int num);

/* Fonction qui renvoie le ndx du symbole */
char *afficheNdx(int num);

/* Fonction qui prends en parametre en ELF et un fichier ouvert et remplit la table des symbole ainsi que la table des string dans
la structure avant de la renvoyer*/
Elf* getTableSymboles(Elf* elf, FILE* f_bin);

/* Fonction qui prends en parametre un ELF et affiche sa table des symbole */
void affichageTableSymbole(Elf* elf);

/* Fonction main qui gere les test effectué sur la tables des symboles */
void affichage_table_symboles(char* nom_fichier, bool arm_cmd_version);

#endif