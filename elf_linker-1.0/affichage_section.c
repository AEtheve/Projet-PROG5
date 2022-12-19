#include <stdio.h>
#include <stdlib.h>
#include "affichage_section.h"
#include "affichage_entete.h"

uint32_t big_e_to_little_e(uint32_t i) {
	uint32_t o = 0;
	o = o | ((i << 24) & 0xFF000000);
	o = o | ((i << 8) & 0x00FF0000);
	o = o | ((i >> 8) & 0x0000FF00);
	o = o | ((i >> 24) & 0x000000FF);
	return o;
}

void affichageNameAddr(uint32_t name){
    printf("%06d ",name);
}

void affichageName(char name[]){
    printf("%-17.*s ", 17, name);
}

void affichageType(uint32_t type){
    char *type_string = (char *)malloc(sizeof(char)*9);

    switch (type){
    case 0:
        type_string = "NULL";
        break;
    case 1:
        type_string = "PROGBITS";
        break;
    case 2:
        type_string = "SYMTAB";
        break;
    case 3:
        type_string = "STRTAB";
        break;
    case 4:
        type_string = "RELA";
        break;
    case 5:
        type_string = "HASH";
        break;
    case 6:
        type_string = "DYNAMIC";
        break;
    case 7:
        type_string = "NOTE";
        break;
    case 8:
        type_string = "NOBITS";
        break;
    case 9:
        type_string = "REL";
        break;
    case 10:
        type_string = "SHLIB";
        break;
    case 11:
        type_string = "DYNSYM";
        break;
    case 0x70000000:
        type_string = "LOPROC";
        break;
    case 0x7fffffff:
        type_string = "HIPROC";
        break;
    case 0x80000000:
        type_string = "LOUSER";
        break;
    case 0xffffffff:
        type_string = "HIUSER";
        break;
    case 0x70000003:
        type_string = "ARM_ATTRIBUTES";
        break;
    default:
        type_string = "INCONNU";
        break;
    }
    printf("%-16s", type_string);
}

void affichageAddr(uint32_t adress){
    printf("%08x ", adress);
}

void affichageOff(uint32_t offset){
    printf("%06x ",offset);
}

void affichageSize(uint32_t size){
    printf("%06x ",size);
}

void affichageES(uint32_t entsize){
    printf("%02x ",entsize);
}
        
void affichageFlg(uint32_t flags){
    char *flag_string = (char *)malloc(sizeof(char)*33);
    int index = 0;
    for (int i=0; i<32; i++) {
        if (flags & (1<<i)) {
            switch(i) {
                case 0:
                    flag_string[index++] = 'W';
                    break;
                case 1:
                    flag_string[index++] = 'A';
                    break;
                case 2:
                    flag_string[index++] = 'X';
                    break;
                case 4:
                    flag_string[index++] = 'M';
                    break;
                case 5:
                    flag_string[index++] = 'S';
                    break;
                case 6:
                    flag_string[index++] = 'I';
                    break;
            }
        }
    }
    flag_string[index]='\0';
    printf("%3s ", flag_string);
}

void affichageLk(uint32_t link){
    printf("%2d ",link);
}
        
void affichageInf(uint32_t info){
    printf("%3d ",info);
}
    
void affichageAl(uint32_t addralign){
    printf("%2d",addralign);
}

SectionHeaderStruct* valeur_section(char* nom_fichier){
    FILE *f_bin;

    f_bin = fopen(nom_fichier, "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", nom_fichier);
        exit(1);
    }

    ElfHeader* h = valeur_entete(nom_fichier);
    int section_adress = h->start_section;
    int section_header = h->taille_section_header;
    int section_number = h->nb_sections;
    int section_header_symbole = h->section_header_string_table_index;
    
    SectionHeaderStruct* table = (SectionHeaderStruct*)malloc(sizeof(SectionHeaderStruct));
    Section* section_table = (Section*)malloc(sizeof(Section)*section_number);
    SectionEntree section_temp[section_number];
    

    fseek(f_bin, section_adress, SEEK_SET);    
    fread(section_temp, section_header, section_number, f_bin);

	for(int i = 0; i < section_number; i++){
		section_temp[i].name_adr = big_e_to_little_e(section_temp[i].name_adr);
		section_temp[i].type = big_e_to_little_e(section_temp[i].type);
		section_temp[i].flags = big_e_to_little_e(section_temp[i].flags);
		section_temp[i].adress = big_e_to_little_e(section_temp[i].adress);
		section_temp[i].offset = big_e_to_little_e(section_temp[i].offset);
		section_temp[i].size = big_e_to_little_e(section_temp[i].size);
		section_temp[i].link = big_e_to_little_e(section_temp[i].link);
		section_temp[i].info = big_e_to_little_e(section_temp[i].info);
		section_temp[i].addralign = big_e_to_little_e(section_temp[i].addralign);
		section_temp[i].entsize = big_e_to_little_e(section_temp[i].entsize);
	}

    for(int i = 0; i < section_number; i++){
        section_table[i].entree = section_temp[i];
        uint8_t lettre;
        int j = 0;
        fseek(f_bin, section_temp[section_header_symbole].offset + section_temp[i].name_adr, SEEK_SET);
        fread(&lettre, 1, 1, f_bin);
        while (lettre != 0){
            section_table[i].name[j] = lettre;
            fread(&lettre, 1, 1, f_bin);
            j++;
        }
    }
    fclose(f_bin);
    table->section_table = section_table;
    table->section_adress = section_adress;
    table->section_header = section_header;
    table->section_number = section_number;
    table->section_header_symbole = section_header_symbole;
    return table;
}

void affichage(SectionHeaderStruct* table){
    Section* section_table = table->section_table;
    printf("There are %d section headers, starting at offset 0x%x:\n\nSection Headers:\n",table->section_number, table->section_adress);
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for(int i = 0; i < table->section_number; i++){
        // printf("%x\n", section_table[i].entree.flags);
        printf("  [%2d] ",i);

        affichageName(section_table[i].name);
        affichageType(section_table[i].entree.type);
        affichageAddr(section_table[i].entree.adress);
        affichageOff(section_table[i].entree.offset);
        affichageSize(section_table[i].entree.size);
        affichageES(section_table[i].entree.entsize);
        affichageFlg(section_table[i].entree.flags);
        affichageLk(section_table[i].entree.link);
        affichageInf(section_table[i].entree.info);
        affichageAl(section_table[i].entree.addralign);
        printf("\n");
    }

    printf("Key to Flags:\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n  L (link order), O (extra OS processing required), G (group), T (TLS),\n  C (compressed), x (unknown), o (OS specific), E (exclude),\n  y (purecode), p (processor specific)\n");
    
}

void affichage_section(char* nom_fichier){
    SectionHeaderStruct* table;
    table = valeur_section(nom_fichier);
    affichage(table);
}