#include <stdio.h>
#include <stdlib.h>

void printMagic(unsigned char *entete){
    printf("Magic: ");
    for (int i = 0; i < 16; i++){
        printf("%02x ", entete[i]);
    }
    printf("\n");
}
    


void printClass(unsigned char *entete){
    printf("Class: ");
    if (entete[4] == 1){
        printf("ELF32\n");
    } else if (entete[4] == 2){
        printf("ELF64\n");
    } else {
        printf("Aucun\n");
    }
}

void printData(unsigned char *entete){
    printf("Data: ");
    if (entete[5] == 1){
        printf("LSB\n");
    } else if (entete[5] == 2){
        printf("MSB\n");
    } else {
        printf("Aucun\n");
    }
}

void printVersion(unsigned char *entete){
    printf("Version: ");
    if (entete[6] == 1){
        printf("1 (current)\n");
    } else {
        printf("Aucun\n");
    }
}

void printAbi(int val){
    printf("ABI: ");
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
    printf("ABI Version: %d\n", entete[8]);
}

void printType(unsigned char *entete){
    printf("Type: ");
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
    printf("Machine: ");
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

void affichage_entete(char *nom_fichier){
    FILE *f_bin;

    f_bin = fopen(nom_fichier, "rb");
    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", nom_fichier);
        exit(1);
    }


    unsigned char entete[32];

    fread(entete, 1, 32, f_bin);

    printMagic(entete);

    printClass(entete);
    printData(entete);
    printVersion(entete);
    printAbi(entete[7]);
    printAbiVersion(entete);

    printType(entete);
    printMachine(entete);
    printVersion(entete);

    fclose(f_bin);
}