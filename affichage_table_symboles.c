#include <stdio.h>
#include <stdlib.h>

typedef struct {
    u_int32_t num;
    u_int32_t value;
    u_int32_t size;
    u_int32_t type;
    u_int32_t bind;
    u_int32_t vis;
    u_int32_t ndx;
    unsigned char *name;

} SymboleEntree;

SymboleEntree symbole_table;

void affichageNum(u_int32_t num){
    printf("%d ",num);
}

void affichageValue(u_int32_t value){
    printf("%d ",value);
}

void affichageSize(u_int32_t size){
    printf("%d ",size);
}

void affichageType(u_int32_t type){
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

void affichageBind(u_int32_t bind){
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





