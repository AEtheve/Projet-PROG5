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

int getIndexSymVal(Elf* res, Elf* elf, int index){
    char* name;
    if((elf->symbol_header + index)->info == 3){
        name = getNameSection(elf->symbol_header,elf->section_header,index);
    } else {
        name = getName(elf->symbol_header,elf->string_header,index);
    }

    for(int i = 0; i <= res->nb_symbol; i++){
        if((res->symbol_header + i)->info == 3){
            if (!strcmp(name,getNameSection(res->symbol_header,res->section_header,i))){
                return i;
            }
        } else {
            if (!strcmp(name,getName(res->symbol_header,res->string_header,i))){
                return i;
            }
        }
    }
    printf("Erreur, le symbole n'existe pas\n");
    exit(1);
}

int getSectionSize(Elf *elf,char *name){
    for (int i=0;i<elf->header->e_section_header_entry_count; i++){
      if (!strcmp(name,elf->section_header[i].name)){
        return i;
      }
    }
    return 0;
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
            if (testElf1 == -1) { 
                /* Ajout de la section de ELF2 dans ELF1 */
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

                section2->entree.offset = getNextOffset(result);

                section2->data = malloc(section2->entree.size);
                memcpy(section2->data, tmp, section2->entree.size);

                addSection(result, *section2);
            } else { 
                /* Fusion des sections de ELF1 et ELF2 */
                int size1 = elf1->section_header[testElf1].entree.size;
                int size2 = section2->entree.size;
                elf1->section_header[testElf1].entree.size = size1 + size2;


                int index_relocation_elf1 = whereIsBryansSection(elf1, section2->name);

                RelocationHeader* tmp;
                tmp = (RelocationHeader*)malloc((size1 + size2));

                if (tmp == NULL){
                    printf("Erreur\n");
                    exit(1);
                }
                
                memcpy(tmp, elf1->relocation_header[index_relocation_elf1].entree, size1);


                int cmp_tmp = 0;
                int ndx;
                for(int j = size1/8 ; j < (size1 + size2)/8 ; j ++){
                    tmp[j] = elf2->relocation_header[compt].entree[cmp_tmp];


                    ndx=getIndexSymVal(result, elf2, tmp[j].info>>8);
                    tmp[j].info = (tmp[j].info & 0xFF) | (ndx << 8);


                    int val = getSectionSize(elf1, getNameSection(result->symbol_header,result->section_header,ndx));
                    tmp[j].offset += val;
                    

                    cmp_tmp++;
                }

                result->relocation_header[index_relocation_elf1].entree = tmp;
                elf1->section_header[testElf1].entree.link = findSymTab(result);
                elf1->section_header[testElf1].entree.info = positionSectionWithoutRel(result, elf1->section_header[testElf1].name);
                
                elf1->section_header[testElf1].entree.offset = getNextOffset(result);

                elf1->section_header[testElf1].data = malloc(size1+size2);
                memcpy(elf1->section_header[testElf1].data, tmp, size1+size2);
                addSection(result, elf1->section_header[testElf1]);
            }
            compt++;
        }
    }

    



    return result;
}

void affichageTestFusionRelocation (char* nom_fichier1, char* nom_fichier2){
    FILE* f1 = ouvertureFichier(nom_fichier1, "rb");
    Elf* elf1 = valeurEntete(f1);
    elf1 = valeurSection(elf1, f1);
    elf1 = getTableSymboles(elf1, f1);
    elf1 = getTableRelocation(elf1, f1);

    FILE* f2 = ouvertureFichier(nom_fichier2, "rb");
    Elf* elf2 = valeurEntete(f2);
    elf2 = valeurSection(elf2, f2);
    elf2 = getTableSymboles(elf2, f2);
    elf2 = getTableRelocation(elf2, f2);

    Elf* result;
    result = fusionSection(elf1, elf2);
    result = fusionTableSymboles(elf1, elf2, result);

    result = fusionRelocation(result, elf1, elf2);
    
    affichageTableReimplentation(result);

    freeElf(elf1);
    freeElf(elf2);
    free(result);
    fermetureFichier(f1);
    fermetureFichier(f2);
}