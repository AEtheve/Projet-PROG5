#include "fusion_relocation.h"


int isTheSectionInSecondTable(Elf* elf, char *name){
    for (int i=0;i<elf->header->e_section_header_entry_count; i++){
        if (!strcmp(elf->section_header[i].name,name)){
            return i;
        }
    }
    return -1;
}

char* removePrefix(char *name){
    char * res=malloc(sizeof(char)*(strlen(name)-4));
    for(int i=3;i<strlen(name);i++){
        res[i-3]=name[i+1];
    }
    return res;

}

int positionSectionWithoutRel(Elf* elf, char* name){
    char * prefix = removePrefix(name);
    for(int i = 0; i < elf->header->e_section_header_entry_count; i++){
        if (!strcmp(elf->section_header[i].name,prefix)){
            free(prefix);
            return i;
        }
    }
    free(prefix);
    return -1;
}

int whereIsBryansSection(Elf *elf, char *name){
    int compt = 0;
    for (int i=0;i<elf->header->e_section_header_entry_count; i++){
        if (!strcmp(elf->section_header[i].name,name)){
            return compt;
        }
        if (elf->section_header[i].entree.type == 9){
            compt++;
        }
    }
    return -1;
}

Elf* fusionRelocation(Elf* result, Elf* elf1, Elf* elf2) {

    result->relocation_header = malloc(elf1->nb_reloc*sizeof(ElfRelocation));
    memcpy(result->relocation_header, elf1->relocation_header, elf1->nb_reloc*sizeof(ElfRelocation));
    result->nb_reloc = elf1->nb_reloc;

    int compt = 0;
    int testElf1 = 0;

    for(int i = 0; i < elf2->header->e_section_header_entry_count; i++){
        if (elf2->section_header[i].entree.type == 9){
            ElfSection* section2 = &elf2->section_header[i];
            testElf1 = isTheSectionInSecondTable(elf1, section2->name); 
            if (testElf1 == -1) { // Si Section de ELF2 n'est pas dans ELF1
                ElfRelocation* tmp;
                tmp = realloc(result->relocation_header, (result->nb_reloc+1)*sizeof(ElfRelocation));
                if (tmp == NULL){
                    printf("Erreur\n");
                    exit(1);
                }
                result->relocation_header = tmp;
                result->relocation_header[result->nb_reloc].entree = elf2->relocation_header[i].entree;
                result->nb_reloc++;
                section2->entree.link = findSymTab(result);
                section2->entree.info = positionSectionWithoutRel(result, section2->name);
                addSection(result, *section2);
            } else { // Fusion de ELF1 et ELF2tmp 

                int size1 = elf1->section_header[testElf1].entree.size;
                int size2 = section2->entree.size;

                elf1->section_header[testElf1].entree.size = size1 + size2;

                int index_relocation_elf1 = whereIsBryansSection(elf1, section2->name);

                RelocationHeader* tmp;
                tmp = (RelocationHeader*)malloc((size1 + size2)*sizeof(RelocationHeader));

                //realloc(result->relocation_header[index_relocation_elf1].entree, (size1 + size2));
                if (tmp == NULL){
                    printf("Erreur\n");
                    exit(1);
                }
                
                memcpy(tmp, elf1->relocation_header[index_relocation_elf1].entree, size1);

                printf("TEST : %x\n",elf2->relocation_header[compt].entree->offset);
                memcpy(tmp + size1, elf2->relocation_header[compt].entree, size2); 

                
                // printf("ICI : %d\n",size1 / elf1->section_header[testElf1].entree.entsize);


                int cmp_tmp = 0;
                for(int j = size1; j < (size1 + size2) ; j += 8){
                    // (tmp + size1) = elf2->relocation_header[compt].entree;
                    (tmp + j)->offset += size1;
                    printf("COMPT : %x\n",(tmp + j)->offset );
                    cmp_tmp++;
                }

                result->relocation_header[index_relocation_elf1].entree = tmp;
                // printf("FINAL : %x\n",result->relocation_header[size1/8].entree->offset);
                elf1->section_header[testElf1].entree.link = findSymTab(result);
                elf1->section_header[testElf1].entree.info = positionSectionWithoutRel(result, elf1->section_header[testElf1].name);
                addSection(result, elf1->section_header[testElf1]);
            }
            compt++;
        }
    }



    return result;
}

int main (int argc, char** argv){
    FILE* f1 = ouvertureFichier(argv[1], "rb");
    Elf* elf1 = valeurEntete(f1);
    elf1 = valeurSection(elf1, f1);
    elf1 = getTableSymboles(elf1, f1);
    elf1 = getTableRelocation(elf1, f1);

    printf("Partie 1 : \n");
    affichageTableReimplentation(elf1);

    FILE* f2 = ouvertureFichier(argv[2], "rb");
    Elf* elf2 = valeurEntete(f2);
    elf2 = valeurSection(elf2, f2);
    elf2 = getTableSymboles(elf2, f2);
    elf2 = getTableRelocation(elf2, f2);

    printf("\n\n\nPartie 2 : \n");
    affichageTableReimplentation(elf2);

    Elf* result;
    printf("\n\n\nPartie res : \n");
    result = fusionSection(elf1, elf2);
    // affichageSection(result,false);
    printf("\n\nFusion section OK\n");

    result = fusion_table_symboles(elf1, elf2, result);
    printf("\n\nFusion table symbole OK\n");
    affichageSection(result,false);
    // affichageTableSymbole(result);

    result = fusionRelocation(result, elf1, elf2);
    printf("\n\nFusion relocation OK\n");
    
    // affichageSection(elf1,false);
    affichageSection(result,false);
    // affichageTableSymbole(result);
    affichageTableReimplentation(result);

    freeElf(elf1);
    freeElf(elf2);
    free(result);
    fermetureFichier(f1);
    fermetureFichier(f2);
    return 0;
}