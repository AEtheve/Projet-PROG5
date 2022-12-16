
typedef struct{
    int start_section;
    int nb_sections;
    int taille_section_header;
    int section_header_string_table_index;
}ElfHeader;

ElfHeader* valeur_entete(char *nom_fichier);

void affichage_entete(char *nom_fichier);