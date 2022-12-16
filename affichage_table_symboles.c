#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint32_t num;
    uint32_t value;
    uint32_t size;
    uint32_t type;
    uint32_t bind;
    uint32_t vis;
    uint32_t ndx;
    unsigned char *name;
} SymboleEntree;

//SymboleEntree symbole_table

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

void traiter_symtab(Section s){
    /*
    Prend une section symtab et récupère les symboles pour créer la table des symboles
    */

}

void find_symtab(Section *s){
    /*
    Prend une liste des sections et traite chaque symtab qui se trouve dedans
    */
    int taille; // à init avec le nb de sections visibles dans le header
    Section tmp;
    for (int i=0;i<taille-1;i++){
        tmp=*(s+i);
        if (s->entree.type==0x12){ //La section est de type symtab
            traiter_symtab(tmp); //Traite la symtab 
        }
    }
}


void affichageNum(uint32_t num){
    printf("%d ",num);
}

void affichageValue(uint32_t value){
    printf("%d ",value);
}

void affichageSize(uint32_t size){
    printf("%d ",size);
}

void affichageType(uint32_t type){
    switch (type){
    case 0:
        printf("NOTYPE ");
        break;
    case 1:
        printf("OBJECT ");
        break;
    case 2:
        printf("FUNC ");
        break;
    case 3:
        printf("SECTION ");
        break;
    case 4:
        printf("FILE ");
        break;
    case 13:
        printf("LOPROC ");
        break;
    case 15:
        printf("HIPROC ");
        break;
    default:
        printf("INCONNU ");
        break;
    }
}

void affichageBind(uint32_t bind){
    switch (bind){
    case 0:
        printf("LOCAL ");
        break;
    case 1:
        printf("GLOBAL ");
        break;
    case 2:
        printf("WEAK ");
        break;
    case 13:
        printf("LOPROC ");
        break;
    case 15:
        printf("HIPROC ");
        break;
    default:
        printf("INCONNU ");
        break;
    }
}

int main(int argc, char* argv[]){
    FILE *f_bin;

    f_bin = fopen(argv[1], "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", argv[1]);
        exit(1);
    }

    SymboleEntree *ts;
    int t=20;//nb de symboles (taille de section/taille de entsize)
    ts=(SymboleEntree*)malloc(sizeof(SymboleEntree)*t);
    printf("Programme terminé\n");
}



