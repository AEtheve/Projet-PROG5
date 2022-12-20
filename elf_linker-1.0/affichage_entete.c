#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "affichage_entete.h"


void printMagic(unsigned char *entete){
    printf(" Magic:   ");
    for (int i = 0; i < 16; i++){
        printf("%02x ", entete[i]);
    }
    printf("\n");
}
    


void printClass(unsigned char *entete){
    printf("  Class:                             ");
    if (entete[4] == 1){
        printf("ELF32\n");
    } else if (entete[4] == 2){
        printf("ELF64\n");
    } else {
        printf("Aucun\n");
    }
}

void printData(unsigned char *entete){
    printf("  Data:                              ");
    if (entete[5] == 1){
        printf("2's complement, little endian\n");
    } else if (entete[5] == 2){
        printf("2's complement, big endian\n");
    } else {
        printf("Aucun\n");
    }
}

void printVersion(unsigned char *entete){
    printf("  Version:                           ");
    if (entete[6] == 1){
        printf("1 (current)\n");
    } else {
        printf("Aucun\n");
    }
}

void printAbi(int val){
    printf("  OS/ABI:                            ");
    switch (val){
        case 0:
            printf("UNIX - System V\n");
            break;
        case 1:
            printf("HP-UX\n");
            break;
        case 2:
            printf("NetBSD\n");
            break;
        case 3:
            printf("Linux\n");
            break;
        case 6:
            printf("Solaris\n");
            break;
        case 7:
            printf("IBM AIX\n");
            break;
        case 8:
            printf("SGI IRIX\n");
            break;
        case 9:
            printf("FreeBSD\n");
            break;
        case 10:
            printf("CompaqTRU64\n");
            break;
        case 11:
            printf("Novell Modesto\n");
            break;
        case 12:
            printf("OpenBSD\n");
            break;
        case 64:
            printf("ARM\n");
            break;
        case 97:
            printf("ARM AArch64\n");
            break;
        default:
            printf("Aucun\n");
            break;
    }
}

void printAbiVersion(unsigned char *entete){
    printf("  ABI Version:                       %d\n", entete[8]);
}

void printType(unsigned char *entete){
    printf("  Type:                              ");
    if (entete[17] == 1){
        printf("REL (Relocatable file)\n");
    } else if (entete[17] == 2){
        printf("EX (Executable file)\n");
    } else if (entete[17] == 3){
        printf("SHA (Shared object file)\n");
    } else if (entete[17] == 4){
        printf("Core file\n");
    } else {
        printf("No file type\n");
    }
}


void printMachine(unsigned char *entete){
    printf("  Machine:                           ");
    if (entete[19] == 1){
        printf("AT&T WE 32100\n");
    } else if (entete[19] == 2){
        printf("SPARC\n");
    } else if (entete[19] == 3){
        printf("Intel Architecture\n");
    } else if (entete[19] == 4){
        printf("Motorola 68000\n");
    } else if (entete[19] == 5){
        printf("Motorola 88000\n");
    } else if (entete[19] == 7){
        printf("Intel 80860\n");
    } else if (entete[19] == 8){
        printf("MIPS RS3000 Big-endian\n");
    } else if (entete[19] == 10){
        printf("MIPS RS4000 Big-endian\n");
    }
    else if (entete[19] == 40){
        printf("ARM\n");
    }
    else {
        printf("\n");
    }
}

void printVersion2(unsigned char *entete){
    printf("  Version:                           ");
    printf("0x%x\n",entete[23]);
}

void printPointDentree(unsigned char *entete) {
    printf("  Entry point address:               ");
    printf("0x%x\n",entete[21]);
}

void printStartOfProgramHeaders (unsigned char *entete) {
    printf("  Start of program headers:          ");
    printf("0 (bytes into file)\n");
}

void printStartOfSectionHeaders (unsigned char *entete) {
    printf("  Start of section headers:          ");
    uint32_t v = entete[32] <<24;
    v = v | entete[33] <<16;
    v = v | entete[34] << 8;
    v = v | entete[35];
    printf("%d (bytes into file)\n",v);
}

void printFlags (unsigned char *entete){
    printf("  Flags:                             ");
    uint32_t v = entete[36] <<24;
    v = v | entete[37] <<16;
    v = v | entete[38] << 8;
    v = v | entete[39]; 
    printf("0x%0x, Version5 EABI\n",v);
}

void printsizeOfHeaders (unsigned char *entete)  {
    printf("  Size of this header:               ");
    uint32_t v = entete[40] << 8;
    v = v | entete[41]; 
    printf("%d (bytes)\n",v);
}

void printsizeOfProgramHeaders (unsigned char *entete)  {
    printf("  Size of program headers:           ");
    uint32_t v = entete[42] << 8;
    v = v | entete[43]; 
    printf("%d (bytes)\n",v);
}

void printNumberOfProgramHeaders (unsigned char *entete)  {
    printf("  Number of program headers:         ");
    uint32_t v = entete[44] << 8;
    v = v | entete[45]; 
    printf("%d\n",v);
}

void printSizeOfSectionHeaders (unsigned char *entete)  {
    printf("  Size of section headers:           ");
    uint32_t v = entete[46] << 8;
    v = v | entete[47]; 
    printf("%d (bytes)\n",v);
}

void printNumberOfSectionHeaders (unsigned char *entete)  {
    printf("  Number of section headers:         ");
    uint32_t v = entete[48] << 8;
    v = v | entete[49]; 
    printf("%d\n",v);
}

void printSectionHeaderStringTableIndex (unsigned char *entete)  {
    printf("  Section header string table index: ");
    uint32_t v = entete[50] << 8;
    v = v | entete[51]; 
    printf("%d\n",v);
}

ElfHeader* valeur_entete(char *nom_fichier){
    FILE *f_bin;

    f_bin = fopen(nom_fichier, "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", nom_fichier);
        exit(1);
    }

    unsigned char entete[52];

    fread(entete, 1, 52, f_bin);
    ElfHeader* h = (ElfHeader*)malloc(sizeof(ElfHeader));

    uint32_t v = entete[32] <<24;
    v = v | entete[33] <<16;
    v = v | entete[34] << 8;
    v = v | entete[35]; 
    h->start_section=v;

    v = entete[46] << 8;
    v = v | entete[47]; 
    h->taille_section_header=v;

    v = entete[48] << 8;
    v = v | entete[49]; 
    h->nb_sections=v;

    v = entete[50] << 8;
    v = v | entete[51]; 
    h->section_header_string_table_index=v;

    return h;
}

void affichage_entete(char *nom_fichier){
    FILE *f_bin;

    f_bin = fopen(nom_fichier, "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", nom_fichier);
        exit(1);
    }

    unsigned char entete[52];

    fread(entete, 1, 52, f_bin);

    printf("ELF Header:\n ");
    printMagic(entete);

    printClass(entete);
    printData(entete);
    printVersion(entete);
    printAbi(entete[7]);
    printAbiVersion(entete);

    printType(entete);
    printMachine(entete);
    printVersion2(entete);
    printPointDentree(entete);
    printStartOfProgramHeaders(entete);
    printStartOfSectionHeaders (entete);
    printFlags(entete);
    printsizeOfHeaders(entete);
    printsizeOfProgramHeaders(entete);
    printNumberOfProgramHeaders(entete);
    printSizeOfSectionHeaders(entete);
    printNumberOfSectionHeaders(entete);
    printSectionHeaderStringTableIndex(entete);

}