#include "fusion.h"


Elf* fusionElf(Elf* elf1, Elf* elf2) {
    // fusion sections
    Elf* result = fusionSection(elf1, elf2);

    // fusion table des symboles
    

    // fusion tables de reimplantation
    result = fusionRelocation(result, elf1, elf2);

    //  Remplissage header
    memcpy(result->header->e_ident, elf1->header->e_ident, HEADER_IDENT_SIZE);
    result->header->e_type=elf1->header->e_type;
    result->header->e_machine=elf1->header->e_machine;
    result->header->e_version=elf1->header->e_version;
    result->header->e_flags=elf1->header->e_flags;
    result->header->e_header_size=elf1->header->e_header_size;
    result->header->e_program_header_entry_size=elf1->header->e_program_header_entry_size;
    result->header->e_section_header_entry_size=elf1->header->e_section_header_entry_size;
    result->header->e_entry=elf1->header->e_entry;

    ElfSection last_section = result->section_header[result->header->e_section_header_entry_count-1];
    int offset = last_section.entree.offset + last_section.entree.size;

    result->header->e_program_header_off = 0;
    result->header->e_section_header_off = offset;

    for (int i=0; i<result->header->e_section_header_entry_count; i++) {
        if (!strcmp(result->section_header[i].name, ".shstrtab")) {
            result->header->e_section_header_string_table_index = i;
            break;
        }
    }

    return result;
}

void writeElf(FILE* f_out, Elf* content) {
    // Ecriture du header
    content->header->e_section_header_string_table_index+=1;
    fwrite(content->header, sizeof(ElfHeader), 1, f_out);

    // Ecriture du contenu des sections
    for (int i=0; i<content->header->e_section_header_entry_count-1; i++) {
        fwrite(content->section_header[i].data, content->section_header[i].entree.size, 1, f_out);
    }

    // Ecriture de la table des sections
    // - Ecriture de la section vide
    uint8_t null_entry = 0;
    fwrite(&null_entry, 1, content->header->e_section_header_entry_size, f_out);
    // - Ecriture des sections du fichier
    for (int i=0; i<content->header->e_section_header_entry_count-1; i++) {
        fwrite(&(content->section_header[i].entree), sizeof(SectionHeader), 1, f_out);
    }
}

