#include <stdio.h>
#include <stdlib.h>
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


int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <fichier>\n", argv[0]);
        return 1;
    }
    FILE *f_bin;

    f_bin = fopen(argv[1], "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", argv[1]);
        return 1;
    }
    int retour = 0;
    int section_adress = 0x5b8;
    int section_header = 40;
    int section_number = 23;
    SectionEntree section_table[section_number];

    fseek(f_bin, section_adress, SEEK_SET);    

    retour = fread(section_table, section_header, section_number, f_bin);

    for(int i = 0; i < section_number; i++){
        printf("[%d] %x %x %x %x %x %x %x %x %x %x\n",i, section_table[i].name_adr,
        section_table[i].type, section_table[i].flags, section_table[i].adress,
        section_table[i].offset, section_table[i].size, section_table[i].link,
        section_table[i].info, section_table[i].addralign, section_table[i].entsize);
    }
    printf("Retour : %d\n",retour);
    fclose(f_bin);
    return 0;
}