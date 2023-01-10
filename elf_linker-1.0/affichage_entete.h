#ifndef _AFFICHAGE_ENTETE_H_
#define _AFFICHAGE_ENTETE_H_

#include "util.h"

/* Recupere les valeurs d'entete du fichier au format ELF et renvoi
un pointeur elf contenant ses valeurs */
Elf* valeurEntete(FILE* f_bin);

/* Affiche les élément de l'entete contenu dans elf */
void affichageEntete(Elf* elf);

/* Appel la fonction valeur_entere, recupere la valeur et
la renvoie dans la fonction affichageEntete */
void affichage_entete(char *nom_fichier);

#endif