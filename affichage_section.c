#include <stdio.h>
#include <stdlib.h>

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


    unsigned char entete[32];

    fread(entete, 1, 32, f_bin);
    
    return 0;
}