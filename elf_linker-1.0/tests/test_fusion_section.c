#include "../fusion_section.h"

int main(int argc, char **argv) {
    if (argc!=3) {
        printf("Usage: fusion <file1> <file2>\n");
        exit(1);
    }

    FILE* file1 = ouvertureFichier(argv[1], "rb");
    FILE* file2 = ouvertureFichier(argv[2], "rb");

    Elf* elf1 = valeurEntete(file1);
    Elf* elf2 = valeurEntete(file2);    

    valeurSection(elf1, file1);
    valeurSection(elf2, file2);

    Elf* result = fusionSection(elf1, elf2);

    affichageSection(result, 1);

    return 0;
} 