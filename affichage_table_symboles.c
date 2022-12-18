#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "./elf_linker-1.0/affichage_section.h"
#include "./elf_linker-1.0/affichage_entete.h"

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

void traiterSymtab(Section s){
    /*
    Prend une section symtab et récupère les symboles pour créer la table des symboles
    */

}

int findSymtab(SectionHeaderStruct Shs){
    /*
    Prend une liste des sections et retourne le numero de la section symtab
    */
    int taille;
    taille=Shs.section_number;
    Section *s;
    s=Shs.section_table;
    Section tmp;
    for (int i=0;i<taille-1;i++){
        tmp=*(s+i);
        if (tmp.entree.type==0x2){ //La section est de type symtab
            return i;
        }
    }
    printf("Aucune symtab trouvée\n");
    exit(1);
}

Section fetchSymtab(SectionHeaderStruct Shs, int numSec){
    return *(Shs.section_table+numSec);
}

int getNbSymboles(Section s){
    return (s.entree.size/s.entree.entsize);
}

/*
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
*/

int main(int argc, char* argv[]){
    FILE *f_bin;

    f_bin = fopen(argv[1], "rb");

    if (f_bin == NULL){
        printf("Erreur d'ouverture du fichier %s\n", argv[1]);
        exit(1);
    }

    SymboleEntree *ts;
    SectionHeaderStruct Shs;
    Shs=*(valeur_section(argv[1])); //Récupération de la valeur de la table des sections
    int symtab=findSymtab(Shs); //OK fonctionne //Trouve le numéro de la section Symtab
    Section s=fetchSymtab(Shs,symtab); //OK fonctionne //Recupère la section symtab
    int nb_symboles=getNbSymboles(s); //OK fonctionne //Recupère le nombre de symboles présents dans la symtab
    ts=malloc(sizeof(SymboleEntree)*nb_symboles); //Malloc de la table des symboles de la taille exacte nécessaire
}



