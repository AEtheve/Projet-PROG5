#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
    uint32_t name_adr;
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

typedef struct {
    SectionEntree entree;
    char name[30];
} Section ;


void affichageNameAddr(uint32_t name){
    printf("%06d ",name);
}

void affichageName(char name[]){
    printf("%-17.*s ", 17, name);
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
    
    int section_adress = 752;
    int section_header = 40;
    int section_number = 23;
    int section_header_symbole = 20;
    Section* section_table = (Section*)malloc(sizeof(Section)*section_number);
    SectionEntree section_temp[section_number];

    fseek(f_bin, section_adress, SEEK_SET);    
    fread(section_temp, section_header, section_number, f_bin);

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

    // TEST

    printf("There are %d section headers, starting at offset 0x%x:\n\nSection Headers:\n",section_number, section_adress);
    printf("  [Nr] Name              Type      Addr     Off    Size   ES Flg Lk Inf Al\n");
    for(int i = 0; i < section_number; i++){
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

    fclose(f_bin);
    return 0;
}