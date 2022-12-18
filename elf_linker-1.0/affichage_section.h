
#include <stdint.h>

typedef struct {
    uint32_t name_adr;
    uint32_t type;
    uint32_t flags;
    uint32_t adress;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} SectionEntree;

typedef struct {
    SectionEntree entree;
    char name[30];
} Section ;

typedef struct {
    Section* section_table;
    int section_adress;
    int section_header;
    int section_number;
    int section_header_symbole;
} SectionHeaderStruct;


/* Recupere les valeurs de toutes les sections du fichier nom_fichier */
SectionHeaderStruct* valeur_section(char* nom_fichier);

/* Affiche les valeurs du fichier nom_fichier */
void affichage_section(char* nom_fichier);