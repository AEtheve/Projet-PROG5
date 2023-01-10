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
    elf->header=NULL;
    elf->section_header=NULL;
    elf->symbol_header=NULL;
    elf->relocation_header=NULL;
    elf->string_header=NULL;
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
    for(int i=0; i<size; i++) {
        section[i].data=NULL;
    }
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
    // printf("elf relocation alloc size: %d\n", size);
    ElfRelocation* relocation = (ElfRelocation*)malloc(sizeof(ElfRelocation)*size);
    for (int i=0; i<size; i++) {
        relocation[i].entree = NULL;
    }
    return relocation;
}

RelocationHeader* allocRelocationHeader(int size){
    // printf("elf relocation header alloc size: %d\n", size);
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



Elf *addSymbol(Elf *elf, ElfSymbole symbole)
{
    // Recuperer l'indice du nouveau symbole

    int index = elf->nb_symbol;
    elf->nb_symbol++;

    // Allouer la mémoire supplémentaire
    ElfSymbole *res = realloc(elf->symbol_header, sizeof(ElfSymbole) * elf->nb_symbol);
    if (res == NULL)
    {
        exit(1);
    }
    elf->symbol_header = res;

    // Copier le contenu de symbole en bout de tableau
    elf->symbol_header[index] = symbole;

    return elf;
}

void freeElfHeader(ElfHeader* elf_header){
    if(elf_header==NULL) {
        return;
    }
    free(elf_header);
}

void freeSymbolHeader(ElfSymbole *elf_symbole){
    if(elf_symbole==NULL) {
        return;
    }
    free(elf_symbole);
}


void freeElfSectionHeader(SectionHeader* section_header) {
    if(section_header==NULL) {
        return;
    }
    free(section_header);
}

void freeElfSection(ElfSection* section, Elf_Half_16b count){
    if(section==NULL) {
        return;
    }
    for (int i=0; i<count; i++) {
        if(section[i].data!=NULL) {
            free(section[i].data);
        }
    }
    free(section);
}


void freeStrTab(StrTab string){
    if(string==NULL) {
        return;
    }
    free(string);
}

void freeRelocationHeader(RelocationHeader* rel){
    if(rel==NULL) {
        return;
    }
    free(rel);
}

void freeElfRelocation(ElfRelocation* rel, int count){
    if(rel==NULL) {
        return;
    }
    // printf("elf relocation ptr: %p\n", rel);
    for (int i=0; i<count; i++) {
        if(rel[i].entree!=NULL) {
            freeRelocationHeader(rel[i].entree);
        }
    }
    free(rel);
}

void freeElf(Elf* elf){
    if(elf==NULL) {
        return;
    }
    if(elf->string_header!=NULL) {
        freeStrTab(elf->string_header);
    }
    if(elf->relocation_header!=NULL) {
        /* for (int i=elf->nb_reloc-1;i>=0;i--){
            freeElfRelocation(&(elf->relocation_header[i]));
        } */
        freeElfRelocation(elf->relocation_header, elf->nb_reloc);
    }
    if(elf->section_header!=NULL) {
        /* for (int i=elf->header->e_program_header_entry_count-1;i>=0;i--){
            freeElfSection(&(elf->section_header[i]));
        } */
        freeElfSection(elf->section_header, elf->header->e_section_header_entry_count);
    }
    if(elf->header!=NULL) {
        freeElfHeader(elf->header);
    }
    if(elf->symbol_header!=NULL) {
        freeSymbolHeader(elf->symbol_header);
    }
    free(elf);
}