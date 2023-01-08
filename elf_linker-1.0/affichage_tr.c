#include "affichage_tr.h"

char *reloc_type(uint32_t valeur)
{
    char *type;
    switch ((uint8_t)valeur)
    {
    case 0:
        type = "R_ARM_NONE";
        break;
    case 1:
        type = "R_ARM_PC24";
        break;
    case 2:
        type = "R_ARM_ABS32";
        break;
    case 3:
        type = "R_ARM_REL32";
        break;
    case 4:
        type = "R_ARM_LDR_PC_G0";
        break;
    case 5:
        type = "R_ARM_ABS16";
        break;
    case 6:
        type = "R_ARM_ABS12";
        break;
    case 7:
        type = "R_ARM_THM_ABS5";
        break;
    case 8:
        type = "R_ARM_ABS8";
        break;
    case 9:
        type = "R_ARM_SBREL32";
        break;
    case 10:
        type = "R_ARM_THM_CALL";
        break;
    case 11:
        type = "R_ARM_THM_PC8";
        break;
    case 12:
        type = "R_ARM_BREL_ADJ";
        break;
    case 13:
        type = "R_ARM_TLS_DESC";
        break;
    case 14:
        type = "R_ARM_THM_SWI8";
        break;
    case 15:
        type = "R_ARM_XPC25";
        break;
    case 16:
        type = "R_ARM_THM_XPC22";
        break;
    case 17:
        type = "R_ARM_TLS_DTPMOD32";
        break;
    case 18:
        type = "R_ARM_TLS_DTPOFF32";
        break;
    case 19:
        type = "R_ARM_TLS_TPOFF32";
        break;
    case 20:
        type = "R_ARM_COPY";
        break;
    case 21:
        type = "R_ARM_GLOB_DAT";
        break;
    case 22:
        type = "R_ARM_JUMP_SLOT";
        break;
    case 23:
        type = "R_ARM_RELATIVE";
        break;
    case 24:
        type = "R_ARM_GOTOFF";
        break;
    case 25:
        type = "R_ARM_GOTPC";
        break;
    case 26:
        type = "R_ARM_GOT32";
        break;
    case 27:
        type = "R_ARM_PLT32";
        break;
    case 28:
        type = "R_ARM_CALL";
        break;
    case 29:
        type = "R_ARM_JUMP24";
        break;
    case 30:
        type = "R_ARM_THM_JUMP24";
        break;
    case 31:
        type = "R_ARM_BASE_ABS";
        break;
    case 32:
        type = "R_ARM_ALU_PCREL_7_0";
        break;
    case 33:
        type = "R_ARM_ALU_PCREL_15_8";
        break;
    case 34:
        type = "R_ARM_ALU_PCREL_23_15";
        break;
    case 35:
        type = "R_ARM_LDR_SBREL_11_0_NC";
        break;
    case 36:
        type = "R_ARM_ALU_SBREL_19_12_NC";
        break;
    case 37:
        type = "R_ARM_ALU_SBREL_27_20_CK";
        break;
    case 38:
        type = "R_ARM_TARGET1";
        break;
    case 39:
        type = "R_ARM_SBREL31";
        break;
    case 40:
        type = "R_ARM_V4BX";
        break;
    default:
        type = "UNKNOWN";
        break;
    }
    return type;
}

int getNbRelocSec(Elf* elf){
    int compt = 0;
    for (int i = 0; i < elf->header->e_section_header_entry_count; i++){
    
        if (elf->section_header[i].entree.type == 9){
            compt++;
        }
    }
    return compt;
}

Elf* getTableRelocation(Elf *elf, FILE* f_bin){

    int nb_relloc = getNbRelocSec(elf);
    int compt = 0;
    // ElfRelocation* reloc = malloc(nb_relloc * sizeof(ElfRelocation));
    ElfRelocation* reloc = allocElfRelocation(nb_relloc);
    if (reloc==NULL){
        printf("Erreur d'allocation de mémoire");
        exit(1);
    }


    for (int i = 0; i < elf->header->e_section_header_entry_count; i++){
    
        if (elf->section_header[i].entree.type == 9){
            // RelocationHeader* rel_tmp = (RelocationHeader*)malloc(sizeof(RelocationHeader) * elf->section_header[i].entree.size / 8);
            RelocationHeader* rel_tmp = allocRelocationHeader(elf->section_header[i].entree.size / 8);

            fseek(f_bin, elf->section_header[i].entree.offset, SEEK_SET);

            uint32_t valeur;
            for (int j = 0; j < elf->section_header[i].entree.size / 8; j++)
            {
                
                fread(&valeur, 4, 1, f_bin);
                rel_tmp[j].offset = reverse_4(valeur);

                fread(&valeur, 4, 1, f_bin);
                rel_tmp[j].info = reverse_4(valeur);
            }
            (reloc + compt)->entree=rel_tmp;
            compt++;
        }
    }
    elf->nb_reloc = nb_relloc;
    elf->relocation_header = reloc;
    return elf;
}

void affichageTableReimplentation(Elf *elf)
{
    int compt=0;
    for (int i = 0; i < elf->header->e_section_header_entry_count; i++)
    {
        if (elf->section_header[i].entree.type == 9)
        {   
            if (elf->section_header[i].entree.size / 8 > 1)
            {
                printf("\nRelocation section '%s' at offset 0x%x contains %d entries:\n",
                       elf->section_header[i].name,
                       elf->section_header[i].entree.offset,
                       elf->section_header[i].entree.size / 8);
            }
            else
            {
                printf("\nRelocation section '%s' at offset 0x%x contains %d entry:\n",
                       elf->section_header[i].name,
                       elf->section_header[i].entree.offset,
                       elf->section_header[i].entree.size / 8);
            }

            printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");

            for (int j = 0; j < elf->section_header[i].entree.size / 8; j++)
            {
                printf("%-10.8x", elf->relocation_header[compt].entree[j].offset);
                printf("%-9.8x", elf->relocation_header[compt].entree[j].info);

                printf("%-17.16s ", reloc_type(elf->relocation_header[compt].entree[j].info));

                int symval = elf->relocation_header[compt].entree[j].info >> 8;
                printf("%-10.8x", reverse_4((elf->symbol_header + symval)->value));

                if ((elf->symbol_header + symval)->info == 3){
                    printf(" %s", elf->section_header[(elf->symbol_header + symval)->ndx].name);
                } else {
                    printf(" %s", elf->string_header + ((elf->symbol_header + symval)->name));
                }

                printf("\n");
            }
            compt++;
        }
        
    }
}






void affichage_table_reimplentation(char* nom_fichier){
    FILE* f = ouvertureFichier(nom_fichier, "rb");
    Elf* elf = valeurEntete(f);
    elf = valeurSection(elf, f);
    elf = getTableSymboles(elf, f);

    elf = getTableRelocation(elf, f);
    affichageTableReimplentation(elf);

    freeElf(elf);

    fermetureFichier(f);
}

int main(int argc, char** argv){

     affichage_table_reimplentation(argv[1]);
     return 0;
}