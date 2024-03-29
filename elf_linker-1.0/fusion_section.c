#include "fusion_section.h"

int findSection(Elf* elf, const char* name) {

    int sec_num = elf->header->e_section_header_entry_count;

    for(int i=0; i<sec_num; i++) {
        if (!strcmp(elf->section_header[i].name, name)) {
            return i;
        }
    }
    return 0;
}

Elf* fusionSection(Elf* elf1, Elf* elf2) {

    Elf* elf_o = allocElf();
    elf_o->header = allocElfHeader();
    elf_o->header->e_section_header_entry_count=0;

    int sec_num1 = elf1->header->e_section_header_entry_count;
    int strtab_offset;
    int offset=0x34;

    ElfSection null_section = {
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        NULL,
        "NULL"
    };

    addSection(elf_o, null_section);

    // Boucle sur les sections de File1
    for(int i=0; i<sec_num1; i++) {
        // si la section est fusionnable
        switch(elf1->section_header[i].entree.type) {
            case 3:
                if(!strcmp(elf1->section_header[i].name, ".shstrtab")) {
                    strtab_offset = elf1->section_header[i].entree.size;
                }
            case 1:
            case 8:
            case 0x70000003:
            {
                // Chercher la section correspondante dans elf2
                int j = findSection(elf2, elf1->section_header[i].name);

                ElfSection new_section;

                // Si on trouve une correspondance
                if (j) {
                    // On fusionne les deux sections
                    new_section.entree.addralign = elf1->section_header[i].entree.addralign;
                    new_section.entree.adress = elf1->section_header[i].entree.adress;
                    new_section.entree.entsize = elf1->section_header[i].entree.entsize;
                    new_section.entree.flags = elf1->section_header[i].entree.flags;
                    new_section.entree.info = elf1->section_header[i].entree.info;
                    new_section.entree.link = elf1->section_header[i].entree.link;
                    new_section.entree.name = elf1->section_header[i].entree.name;
                    new_section.entree.offset = offset; 
                    new_section.entree.size = elf1->section_header[i].entree.size + elf2->section_header[j].entree.size;
                    new_section.entree.type = elf1->section_header[i].entree.type;

                    strcpy(new_section.name, elf1->section_header[i].name);

                    new_section.data = (uint8_t *)malloc(sizeof(uint8_t)*new_section.entree.size);
                    memcpy(new_section.data, elf1->section_header[i].data, elf1->section_header[i].entree.size);
                    memcpy(new_section.data+elf1->section_header[i].entree.size, elf2->section_header[j].data, elf2->section_header[j].entree.size);
                } else {
                    // Sinon on copie la section trouvée
                    memcpy(&(new_section.entree), &(elf1->section_header[i].entree), sizeof(SectionHeader));
                    new_section.data = (uint8_t *)malloc(sizeof(uint8_t)*new_section.entree.size);
                    memcpy(new_section.data, elf1->section_header[i].data, new_section.entree.size);
                    strcpy(new_section.name, elf1->section_header[i].name);
                    new_section.entree.offset=offset;
                }
                // Et on l'ajoute au fichier de sortie
                elf_o = addSection(elf_o, new_section);
                offset+=new_section.entree.size;
                break;
            }
        }
    }

    // On rajoute les sections de elf2 qui ne correspondent à aucune section  de elf1
    int sec_num2 = elf2->header->e_section_header_entry_count;

    for(int j=0; j<sec_num2; j++) {
        // si la section est fusionnable
        switch(elf2->section_header[j].entree.type) {
            case 1:
            case 3:
            case 8:
            {
                int i = findSection(elf1, elf2->section_header[j].name);

                // Si on trouve une correspondance, on ne traite pas la section
                if (i) {
                    break;
                } else {
                    ElfSection new_section;
                    // Sinon on copie la section trouvée
                    memcpy(&(new_section.entree), &(elf2->section_header[j].entree), sizeof(SectionHeader));
                    new_section.entree.name+=strtab_offset;
                    new_section.data = (uint8_t *)malloc(sizeof(uint8_t)*new_section.entree.size);
                    memcpy(new_section.data, elf2->section_header[j].data, new_section.entree.size);
                    strcpy(new_section.name, elf2->section_header[j].name);
                    new_section.entree.offset=offset;
                    
                    // Et on l'ajoute au fichier de sortie
                    elf_o = addSection(elf_o, new_section);
                    offset+=new_section.entree.size;
                }
                break;
            }
        }
    }

    return elf_o;
}