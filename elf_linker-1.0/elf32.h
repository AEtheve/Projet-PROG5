#ifndef __ELF32_H__
#define __ELF32_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "util.h"



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

FILE* ouvertureFichier(char* nomFichier, char* mode);

void fermetureFichier(FILE* fichier);

Elf* allocElf();

ElfHeader* allocElfHeader();

SectionHeader* allocElfSectionHeader();

ElfSection* allocElfSection(int size);

ElfSymbole* allocElfSymbole(int size);

StrTab allocStrTab(int size);

ElfRelocation* allocElfRelocation(int size);

RelocationHeader* allocRelocationHeader(int size);

Elf* addSection(Elf* elf, ElfSection section);

Elf* addSymbol(Elf* elf, ElfSymbole symbol);

void freeElfHeader(ElfHeader* elf_header);

void freeSymbolHeader(ElfSymbole *elf_symbole);


void freeElfSectionHeader(SectionHeader* section_header);

void freeElfSection(ElfSection* section, Elf_Half_16b count);


void freeStrTab(StrTab string);

void freeRelocationHeader(RelocationHeader* rel);

void freeElfRelocation(ElfRelocation* rel, int count);

void freeElf(Elf* elf);

void writeHeader(ElfHeader* header, FILE* f_out);

void writeSectionHeader(SectionHeader* section_header, FILE* f_out);

#endif