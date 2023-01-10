#ifndef __ELF32_H__
#define __ELF32_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>



#define HEADER_IDENT_SIZE 16

typedef uint32_t Elf_Addr_32b;
typedef uint16_t Elf_Half_16b;

typedef uint32_t Elf_Word_32b;
typedef uint32_t Elf_Off_32b;

typedef struct {
    unsigned char e_ident[HEADER_IDENT_SIZE];
    Elf_Half_16b e_type;
    Elf_Half_16b e_machine;
    Elf_Word_32b e_version;
    Elf_Addr_32b e_entry;
    Elf_Off_32b e_program_header_off;
    Elf_Off_32b e_section_header_off;
    Elf_Word_32b e_flags;
    Elf_Half_16b e_header_size;
    Elf_Half_16b e_program_header_entry_size;
    Elf_Half_16b e_program_header_entry_count;
    Elf_Half_16b e_section_header_entry_size;
    Elf_Half_16b e_section_header_entry_count;
    Elf_Half_16b e_section_header_string_table_index;
} ElfHeader;


typedef struct {
    Elf_Word_32b name;
    Elf_Word_32b type;
    Elf_Word_32b flags;
    Elf_Addr_32b adress;
    Elf_Off_32b offset;
    Elf_Word_32b size;
    Elf_Word_32b link;
    Elf_Word_32b info;
    Elf_Word_32b addralign;
    Elf_Word_32b entsize;
} SectionHeader;

typedef struct {
    SectionHeader entree;
    uint8_t* data;
    char name[30];
} ElfSection;

typedef struct {
  Elf_Word_32b name;
  Elf_Addr_32b value;
  Elf_Word_32b size;
  unsigned char info;
  unsigned char other;
  Elf_Half_16b ndx;
} ElfSymbole;

typedef struct {
    Elf_Word_32b offset;
    Elf_Word_32b info;
} RelocationHeader;

typedef struct {
    RelocationHeader* entree;
} ElfRelocation;

typedef char *StrTab;

typedef struct {
    ElfHeader* header;
    ElfSection* section_header;
    ElfSymbole* symbol_header;
    StrTab string_header;
    ElfRelocation* relocation_header;
    int nb_reloc;
    int nb_symbol;
} Elf;


/* Fonction qui renvoie un Elf* alloue dans la memoire avec tout ses attribus NULL */ 
Elf* allocElf();

/* Fonction qui renvoie un ElfHeader* alloue dans la memoire */
ElfHeader* allocElfHeader();

/* Fonction qui renvoie un SectionHeader* alloue dans la memoire */
SectionHeader* allocElfSectionHeader();

/* Fonction qui renvoie un ElfSection* alloue dans la memoire avec la section initialiser a la taille passer en parametre */
ElfSection* allocElfSection(int size);

/* Fonction qui renvoie un ElfSymbole* alloue dans la memoire avec sa taille passer en parametre */
ElfSymbole* allocElfSymbole(int size);

/* Fonction qui renvoie un StrTab alloue dans la memoire avec sa taille passer en parametre */
StrTab allocStrTab(int size);

/* Fonction qui alloue un tableau de réimplantation de la taille passée en argument et retourne un pointeur vers cette zone allouée */
ElfRelocation* allocElfRelocation(int size);

/* Fonction qui renvoie un tableau de RelocationHeader de la taille passée en arguments pour stocker toute les relocation et renvoie un pointeur */
RelocationHeader* allocRelocationHeader(int size);

/* Ajoute une section à la table des sections de la structure ELF */
Elf* addSection(Elf* elf, ElfSection section);

/* Ajoute un symbole à la table des symboles de la structure ELF */
Elf* addSymbol(Elf* elf, ElfSymbole symbol);

/* Fonction permettant la libération de la zone mémoire contenant l'entete */
void freeElfHeader(ElfHeader* elf_header);

/* Fonction permettant la libération de la zone mémoire contenant la table des symboles */
void freeSymbolHeader(ElfSymbole *elf_symbole);

/* Fonction permettant la libération de la zone mémoire contenant la table des sections */
void freeElfSectionHeader(SectionHeader* section_header);

/* Fonction permettant la libération de la zone mémoire contenant une section */
void freeElfSection(ElfSection* section, Elf_Half_16b count);

/* Fonction permettant la libération de la zone mémoire contenant la string table */
void freeStrTab(StrTab string);

/* Fonction permettant la libération de la zone mémoire contenant une entrée de la table de réimplantation */
void freeRelocationHeader(RelocationHeader* rel);

/* Fonction permettant la libération de la zone mémoire contenant la table de réimplantation */
void freeElfRelocation(ElfRelocation* rel, int count);

/* Fonction qui s'occupe de libérer toute les zones memoires utilisés par nos structures */
void freeElf(Elf* elf);

#endif
