#include "gestion_section.h"


void affichageNameAddr(uint32_t name){
	printf("%06d ",name);
}

void affichageName(char name[], bool arm_cmd_version){
	if (arm_cmd_version) { // arm-none-eabi
		if (strlen(name)>17) {
			printf("%-12.*s", 12, name);
			printf("[...] ");
		} else {
			printf("%-17.*s ", 17, name);
		}
	} else { // arm-eabi
		printf("%-17.*s ", 17, name);
	}
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

Elf* valeurSection(Elf* elf, FILE* f_bin){

    int section_adress = elf->header->e_section_header_off;
    int section_header = elf->header->e_section_header_entry_size;
    int section_number = elf->header->e_section_header_entry_count;
    int section_header_symbole = elf->header->e_section_header_string_table_index;

    ElfSection* section_table = allocElfSection(section_number);
    SectionHeader section_temp[section_number];
    

    fseek(f_bin, section_adress, SEEK_SET);    
    fread(section_temp, section_header, section_number, f_bin);

	for(int i = 0; i < section_number; i++){
        section_table[i].entree.name = reverse_4(section_temp[i].name);
		section_table[i].entree.type = reverse_4(section_temp[i].type);
		section_table[i].entree.flags = reverse_4(section_temp[i].flags);
		section_table[i].entree.adress = reverse_4(section_temp[i].adress);
		section_table[i].entree.offset = reverse_4(section_temp[i].offset);
		section_table[i].entree.size = reverse_4(section_temp[i].size);
		section_table[i].entree.link = reverse_4(section_temp[i].link);
		section_table[i].entree.info = reverse_4(section_temp[i].info);
		section_table[i].entree.addralign = reverse_4(section_temp[i].addralign);
		section_table[i].entree.entsize = reverse_4(section_temp[i].entsize);
	}

    uint8_t lettre;
    for(int i = 0; i < section_number; i++){
        int j = 0;
        fseek(f_bin, section_table[section_header_symbole].entree.offset + section_table[i].entree.name, SEEK_SET);
        fread(&lettre, 1, 1, f_bin);
        while (lettre != 0){
            section_table[i].name[j] = lettre;
            fread(&lettre, 1, 1, f_bin);
            j++;
        }
        section_table[i].name[j] = '\0';
    }

    for(int i = 0; i < section_number; i++) {
        if(section_table[i].entree.size!=0) {
            section_table[i].data = (uint8_t *)malloc(sizeof(uint8_t)*section_table[i].entree.size);
            fseek(f_bin, section_table[i].entree.offset, SEEK_SET);
            fread(section_table[i].data, 1, section_table[i].entree.size, f_bin);
        }
    }

    elf->section_header = section_table;
    return elf;
}

void affichageSection(Elf* elf, bool arm_cmd_version){
    ElfSection* section_table = elf->section_header;

    printf("There are %d section headers, starting at offset 0x%x:\n\nSection Headers:\n",elf->header->e_section_header_entry_count, elf->header->e_section_header_off);
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    for(int i = 0; i < elf->header->e_section_header_entry_count; i++){
        printf("  [%2d] ",i);

        affichageName(section_table[i].name, arm_cmd_version);
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
    printf("Key to Flags:\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n  L (link order), O (extra OS processing required), G (group), T (TLS),\n  C (compressed), x (unknown), o (OS specific), E (exclude),\n  D (mbind), y (purecode), p (processor specific)\n");
    
}

void gestion_section(char* nom_fichier, bool arm_cmd_version){
    FILE* f = ouvertureFichier(nom_fichier, "rb");
    Elf* elf = valeurEntete(f);
    elf = valeurSection(elf, f);
    affichageSection(elf, arm_cmd_version);
    fermetureFichier(f);
}

/* int main(int argc, char* argv[]){
    
    gestion_section(argv[1],false);
    
    return 0;
} */