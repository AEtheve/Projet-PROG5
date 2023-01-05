
// typedef struct{
//     int start_section;
//     int nb_sections;
//     int taille_section_header;
//     int section_header_string_table_index;
// }ElfHeader;
#include "elf32.h"

/* Recupere les valeurs d'entete du fichier au format ELF et renvoi
un pointeur elf contenant ses valeurs */
Elf* valeur_entete(char *nom_fichier);

/* Affiche les élément de l'entete contenu dans elf */
void affichageEntete(Elf* elf);

/* Appel la fonction valeur_entere, recupere la valeur et
la renvoie dans la fonction affichageEntete */
void affichage_entete(char *nom_fichier);