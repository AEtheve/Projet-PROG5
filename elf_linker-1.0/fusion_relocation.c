#include "fusion_relocation.h"


int isTheSectionInSecondTable(Elf* elf, char *name){
    for (int i=0;i<elf->header->e_section_header_entry_count; i++){
        if (!strcmp(elf->section_header[i].name,name)){
            return i;
        }
    }
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

    for(int i = 0; i < elf2->header->e_section_header_entry_count; i++){
        if (elf2->section_header[i].entree.type == 9){
            ElfSection* section2 = &elf2->section_header[i];
            int testElf1 = isTheSectionInSecondTable(elf1, section2->name); 
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
            } else { // Fusion de ELF1 et ELF2
                
                int size1 = sizeof(elf1->section_header[testElf1].data);
                int size2 = sizeof(section2->data);

                result->section_header[testElf1].entree.size = size1 + size2;

                int index_relocation_elf1 = whereIsBryansSection(elf1, section2->name);

                RelocationHeader* tmp = realloc(result->relocation_header[index_relocation_elf1].entree, 
                    (size1 + size2));
                if (tmp == NULL){
                    printf("Erreur\n");
                    exit(1);
                }

                memcpy(tmp + size1, elf2->relocation_header[i].entree, size2);

                result->relocation_header[index_relocation_elf1].entree = tmp;
                
                for(int j = size1 / result->section_header[testElf1].entree.entsize; j < (section2->entree.size / section2->entree.entsize) ; j++){
                    result->relocation_header[index_relocation_elf1].entree[j].offset += size1;
                }


            }
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

    Elf* result = malloc(sizeof(Elf));
    result = fusionSection(elf1, elf2);
    printf("\n\n\nPartie res : \n");
    result = fusionRelocation(result, elf1, elf2);

    
    affichageTableReimplentation(result);

    freeElf(elf1);
    freeElf(elf2);
    fermetureFichier(f1);
    fermetureFichier(f2);
    return 0;
}