#ifndef _FUSION_TS_H_
#define _FUSION_TS_H_

#include "gestion_table_symboles.h"
#include "fusion_section.h"

/* Retourne l'offset maximal rencontré dans toutes les sections de la structure ELF passée en argument */
int getOffsetMax(Elf *elf_fusion);

/* Retourne la taille de la section qui a l'offset maximal */
int getSizeofOffsetMax(Elf *elf_fusion);

/* Retourne l'emplacement où placer la section suivante */
int getNextOffset(Elf *elf);

/* Retourne une chaine de caractère (extraite de la string table passée en argument), correspondant à l'élément dans la table des symboles de numéro passé en argument*/
char *getName(ElfSymbole *symTab, StrTab strtab, int num);

/* Retourne le nom de section de numéro passé en argument */
char *getNameSection(ElfSymbole *symTab,ElfSection *section_header, int num);


/* Ajoute a l'ELF dest le symbole symbol (de la string table) avec toute ses informations */
void addSymboleOther(Elf *dest, Elf *source, ElfSymbole symbol, StrTab strtab, int index, int *indexStrTab);

/* Ajoute a l'ELF dest le symbole symbol (de la symbole table [une section]) avec toute ses informations */
void addSymboleSection(Elf *dest, Elf *source, ElfSymbole symbol, StrTab strtab, int index, int section);

/* Fonction qui prend en arguments deux ELF: elf1 et elf2 et fusionne leur table des symboles dans elf_fusion qui est renvoyée */
Elf *fusionTableSymboles(Elf *file1, Elf *file2, Elf *elf_fusion);

/* Fonction d'affichage pour tester la fusion des tables de symboles */
void affichageTestFusionSymbole(char* nom_fichier1, char* nom_fichier2);

#endif