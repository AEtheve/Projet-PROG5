#ifndef _FUSION_RELOCATION_H_
#define _FUSION_RELOCATION_H_

#include "gestion_tr.h"
#include "fusion_ts.h"


/*Retourne l'index de la section de nom passé en argument dans la table des sections de la structure ELF passée en argument */
int isTheSectionInSecondTable(Elf* elf, char *name);

/*Retourne une chaine de caractère sans le préfixe ".rel" */
char* removePrefix(char *name);

/* Retourne l'index de la section de nom passé en argument
Le nom est de la forme ".rel.nom_section" et la section renvoyée est de nom "nom_section" */
int positionSectionWithoutRel(Elf* elf, char* name);

/* Retourne l'index de la section de meme nom dans la 2ème structure elf */
int whereIsBryansSection(Elf *elf, char *name);

/* Renvoie l'index du symbole dans la nouvelles tables des symboles (renumérotation des symboles de la table de réimplantation) */
int getIndexSymVal(Elf* res, Elf* elf, int index);

/* Prend en entrée un ELF et un nom et renvoie la taille de la section de nom name */
int getSectionSize(Elf *elf,char *name);

/* Prend en entrée deux ELF : elf1 et elf2, fusionne leur table de reimplantation, la place dans l'ELF result et le renvoie */
Elf* fusionRelocation(Elf* result, Elf* elf1, Elf* elf2);

/* Fonction d'affichage de la table de rollacation */
void affichageTestFusionRelocation (char* nom_fichier1, char* nom_fichier2);

#endif