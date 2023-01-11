#include "fusion.h"


Elf* fusionElf(Elf* elf1, Elf* elf2) {
    // fusion sections
    Elf* result = fusionSection(elf1, elf2);

    // fusion table des symboles
    result = fusionTableSymboles(elf1, elf2, result);

    // fusion tables de reimplantation
    result = fusionRelocation(result, elf1, elf2);

    affichageSection(result, false);


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

WriteError writeElf(FILE* f_out, Elf* content, bool strict_mode) {
    if (f_out==NULL) {
        return INVALID_FILE;
    }
    
    // Ecriture du header
    if (strict_mode && (content->header==NULL)) {
        return MISSING_HEADER;
    }
    writeHeader(content->header, f_out);

    // Ecriture du contenu des sections

    if (strict_mode && (content->section_header==NULL)) {
        return MISSING_SECTION_HEADER;
    }
    for (int i=0; i<content->header->e_section_header_entry_count; i++) {
        if (strict_mode && (content->section_header[i].data==NULL) && (content->section_header[i].entree.size > 0)) {
            return MISSING_SECTION_DATA;
        }
        fwrite(content->section_header[i].data, content->section_header[i].entree.size, 1, f_out);
    }

    // Ecriture de la table des sections
    for (int i=0; i<content->header->e_section_header_entry_count; i++) {
        writeSectionHeader(&(content->section_header[i].entree), f_out);
    }

    return WRITE_OK;
}

int main(int argc, char **argv) {
    if (argc != 3)
    {
        printf("Usage: %s <fichier1> <fichier2> \r \n", argv[0]);
        exit(1);
    }

    FILE *f1 = ouvertureFichier(argv[1], "rb");
    FILE *f2 = ouvertureFichier(argv[2], "rb");

    Elf *elf1 = valeurEntete(f1);
    Elf *elf2 = valeurEntete(f2);

    elf1 = valeurSection(elf1, f1);
    elf2 = valeurSection(elf2, f2);

    elf1 = getTableSymboles(elf1, f1);
    elf2 = getTableSymboles(elf2, f2);

    elf1 = getTableRelocation(elf1, f1);
    elf2 = getTableRelocation(elf2, f2);



    Elf *fusion = fusionElf(elf1, elf2);

    FILE* f_out = ouvertureFichier("out.o", "wb");
    WriteError err =writeElf(f_out, fusion, false);

    if (err!=WRITE_OK) {
        printf("ERROR WHILE WRITING TO FILE\n");
        exit(1);
    }

    fermetureFichier(f1);
    fermetureFichier(f2);
    fermetureFichier(f_out);
    return 0;
}
