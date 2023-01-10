#ifndef _AFFICHAGE_SECTION_H_
#define _AFFICHAGE_SECTION_H_

#include <stdbool.h>
#include <string.h>
#include "affichage_entete.h"


/* Fonction qui affiche le nom de la section */
void affichageName(char name[], bool arm_cmd_version);

/* Fonction qui affiche le type de la section */
void affichageType(uint32_t type);

/* Fonction qui affiche l'adresse de la section */
void affichageAddr(uint32_t adress);

/* Fonction qui affiche l'offset de la section */
void affichageOff(uint32_t offset);

/* Fonction qui affiche la taille de la section */
void affichageSize(uint32_t size);

/* Fonction qui affiche la taille des entrees de la section */
void affichageES(uint32_t entsize);

/* Fonction qui affiche le flag de la section */
void affichageFlg(uint32_t flags);

/* Fonction qui affiche link de la section */
void affichageLk(uint32_t link);

/* Fonction qui affiche l'info de la section */  
void affichageInf(uint32_t info);
    
/* Fonction qui affiche l'alignement de la section */
void affichageAl(uint32_t addralign);


/* Recupere les valeurs de toutes les sections et de l'entete du fichier
nom_fichier et renvoie leurs valeur au format Elf */
Elf* valeurSection(Elf* elf, FILE* f_bin);

/* Affiche les sections contenu dans le fichier elf */
void affichageSection(Elf* elf, bool arm_cmd_version);

/* Recupere les valeurs de section avec valeur_section puis les affiches
avec affichageSection */
void affichage_section(char* nom_fichier, bool arm_cmd_version);

#endif