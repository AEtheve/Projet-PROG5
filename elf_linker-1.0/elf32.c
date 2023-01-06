#include "elf32.h"

FILE* ouvertureFichier(char* nomFichier, char* mode){
    FILE* fichier = fopen(nomFichier, mode);
    if (fichier == NULL){
        printf("Erreur lors de l'ouverture du fichier %s\n", nomFichier);
        exit(1);
    }
    return fichier;
}

void fermetureFichier(FILE* fichier){
    fclose(fichier);
}

Elf* allocElf(){
    Elf* elf = (Elf*)malloc(sizeof(Elf));
    return elf;
}

ElfHeader* allocElfHeader(){
    ElfHeader* header = (ElfHeader*)malloc(sizeof(ElfHeader));
    return header;
}

SectionHeader* allocElfSectionHeader(){
    SectionHeader* header = (SectionHeader*)malloc(sizeof(SectionHeader));
    return header;
}

ElfSection* allocElfSection(int size){
    ElfSection* section = (ElfSection*)malloc(sizeof(ElfSection)*size);
    return section;
}

ElfSymbole* allocElfSymbole(int size){
    ElfSymbole* symbole = (ElfSymbole*)malloc(sizeof(ElfSymbole)*size);
    return symbole;
}

StrTab allocStrTab(int size){
    StrTab strTab = (StrTab)malloc(sizeof(StrTab)*size);
    return strTab;
}

ElfRelocation* allocElfRelocation(int size){
    ElfRelocation* relocation = (ElfRelocation*)malloc(sizeof(ElfRelocation)*size);
    return relocation;
}

RelocationHeader* allocRelocationHeader(int size){
    RelocationHeader* header = (RelocationHeader*) malloc(sizeof(RelocationHeader) * size);
    return header;
}


Elf* addSection(Elf* elf, ElfSection section) {
    // Recuperer l'indice de la nouvelle section

    int index = elf->header->e_section_header_entry_count;
    elf->header->e_section_header_entry_count++;

    // Allouer la mémoire supplémentaire
    ElfSection* res = realloc(elf->section_header, sizeof(ElfSection)*elf->header->e_section_header_entry_count);
    if (res==NULL) {
        exit(1);
    }
    elf->section_header=res;

    // Copier le contenu de section en bout de tableau
    elf->section_header[index]=section;

    return elf;
}
