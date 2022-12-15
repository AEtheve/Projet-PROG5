#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
        printf("LSB\n");
    } else if (entete[5] == 2){
        printf("MSB\n");
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
            printf("UNIX System V\n");
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
    if (entete[16] == 1){
        printf("Relocatable file\n");
    } else if (entete[16] == 2){
        printf("Executable file\n");
    } else if (entete[16] == 3){
        printf("Shared object file\n");
    } else if (entete[16] == 4){
        printf("Core file\n");
    } else {
        printf("No file type\n");
    }
}


void printMachine(unsigned char *entete){
    printf("  Machine:                           ");
    if (entete[18] == 1){
        printf("AT&T WE 32100\n");
    } else if (entete[18] == 2){
        printf("SPARC\n");
    } else if (entete[18] == 3){
        printf("Intel Architecture\n");
    } else if (entete[18] == 4){
        printf("Motorola 68000\n");
    } else if (entete[18] == 5){
        printf("Motorola 88000\n");
    } else if (entete[18] == 7){
        printf("Intel 80860\n");
    } else if (entete[18] == 8){
        printf("MIPS RS3000 Big-endian\n");
    } else if (entete[18] == 10){
        printf("MIPS RS4000 Big-endian\n");
    }
    else if (entete[18] == 40){
        printf("ARM\n");
    }
}

void printVersion2(unsigned char *entete){
    printf("  Version:                           ");
    printf("0x%x\n",entete[20]); // TODO  escrot #1 je ne sais pas ce que fait entete[19]
}

void printPointDentree(unsigned char *entete) {
    printf("  Entry point address:               ");
    printf("0x%x\n",entete[21]); // TODO  escrot #2 peut être pas 21
}

void printStartOfProgramHeaders (unsigned char *entete) {
    printf("  Start of program headers:          ");
    printf("0 (bytes into file)\n"); //TODO start of program à faire
}

void printStartOfSectionHeaders (unsigned char *entete) {
    printf("  Start of section headers:          ");
    uint32_t v =entete[33] << 8;
    v = v | entete[32]; 
    printf("%d (bytes into file)\n",v);
}

void printFlags (unsigned char *entete){
    printf("  Flags:                             ");
    printf("0x5000000, Version5 EABI\n"); //TODO Flags à faire
}

void printsizeOfHeaders (unsigned char *entete)  {
    printf("  Size of this header:               ");
    printf("52 (bytes)\n"); //TODO sizeOfHeaders à faire
}

void printsizeOfProgramHeaders (unsigned char *entete)  {
    printf("  Size of program headers:           ");
    printf("0 (bytes)\n"); //TODO sizeOfProgramHeaders à faire
}

void printNumberOfProgramHeaders (unsigned char *entete)  {
    printf("  Number of program headers:         ");
    printf("0\n"); //TODO NumberOfProgramHeaders à faire
}

void printSizeOfSectionHeaders (unsigned char *entete)  {
    printf("  Size of section headers:           ");
    printf("40 (bytes)\n"); //TODO SizeOfSectionHeaders à faire
}

void printNumberOfSectionHeaders (unsigned char *entete)  {
    printf("  Number of section headers:         ");
    printf("21\n"); //TODO NumberOfSectionHeaders à faire
}

void printSectionHeaderStringTableIndex (unsigned char *entete)  {
    printf("  Section header string table index: ");
    printf("20\n"); //TODO SectionHeaderStringTableIndex à faire
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


    unsigned char entete[50];

    fread(entete, 1, 32, f_bin);
    printf(" ELF Header:\n ");
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

    return 0;
}