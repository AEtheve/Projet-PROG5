#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef union {
    uint32_t adr;
    char* name;
} NameSection ;

typedef struct {
    NameSection name_adr;
    uint32_t type;
    uint32_t flags;
    uint32_t adress;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
} SectionEntree;


SectionEntree section_table;

void changeName(){
    
}


void affichageName(uint32_t name){
    printf("%06d\t",name);
}

void affichageType(uint32_t type){
    int i = 0;
    int max = 0;
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
    default:
        type_string = "INCONNU";
        break;
    }
    printf("%-10s", type_string);
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
    switch (flags){
    case 1:
        printf("W ");
        break;
    case 2:
        printf("A ");
        break;
    case 4:
        printf("X ");
        break;    
    default:
        printf("xx ");
        break;
    }
}

void affichageLk(uint32_t link){
    printf("%x ",link);
}
        
void affichageInf(uint32_t info){
    printf("%x ",info);
}
    
void affichageAl(uint32_t addralign){
    printf("%x",addralign);
}


int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage: %s <fichier>\n", argv[0]);
        return 1;
    }
    FILE *f_bin;

    f_bin = fopen(argv[1], "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", argv[1]);
        return 1;
    }
    
    int section_adress = 0x5b8;
    int section_header = 40;
    int section_number = 23;
    SectionEntree section_table[section_number];

    fseek(f_bin, section_adress, SEEK_SET);    
    fread(section_table, section_header, section_number, f_bin);

    printf("There are %d section headers, starting at offset 0x%x:\n\nSection Headers:\n",section_number, section_adress);

    for(int i = 0; i < section_number; i++){
            printf("  [%.*d] ", 2, i);
        affichageName(section_table[i].name_adr.adr);
        affichageType(section_table[i].type);
        affichageAddr(section_table[i].adress);
        affichageOff(section_table[i].offset);
        affichageSize(section_table[i].size);
        affichageES(section_table[i].entsize);
        affichageFlg(section_table[i].flags);
        affichageLk(section_table[i].link);
        affichageInf(section_table[i].info);
        affichageAl(section_table[i].addralign);
        printf("\n");
    }

    printf("Key to Flags:\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n  L (link order), O (extra OS processing required), G (group), T (TLS),\n  C (compressed), x (unknown), o (OS specific), E (exclude),\n  y (purecode), p (processor specific)\n");

    fclose(f_bin);
    return 0;
}