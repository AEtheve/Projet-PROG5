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


