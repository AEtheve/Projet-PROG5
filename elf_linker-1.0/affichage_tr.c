#include "affichage_section.h"
#include "elf32.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

char *reloc_type(uint32_t valeur)
{
    char *type;
    switch ((uint8_t)reverse_4(valeur))
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

int findSymTab(SectionHeaderStruct *Shs)
{
    for (int i = 0; i < Shs->section_number; i++)
    {
        if (Shs->section_table[i].entree.type == 2)
        {
            return i;
        }
    }
    return -1;
}

Section fetchSection(SectionHeaderStruct Shs, int numSec)
{
    return *(Shs.section_table + numSec);
}

typedef struct
{
    Elf_Word_32b name;
    Elf_Addr_32b value;
    Elf_Word_32b size;
    unsigned char info;
    unsigned char other;
    Elf_Half_16b ndx;
} SymboleEntree;

typedef struct
{
    SymboleEntree entree;
    unsigned char *name;
} Symbole;

typedef char *StrTab;

void afficherNameOther(SymboleEntree *symTab, StrTab strtab, int num)
{
    printf(" %s", strtab + reverse_4((symTab + num)->name));
}

void afficherNameSection(SymboleEntree *symTab,
                         SectionHeaderStruct *section_header, int num)
{
    printf(" %s",
           section_header->section_table[reverse_2((symTab + num)->ndx)].name);
}

void affichage_table_reimplentation(char *nom_fichier)
{
    SectionHeaderStruct *section_header = valeur_section(nom_fichier);

    FILE *f_bin;
    f_bin = fopen(nom_fichier, "rb");
    if (f_bin == NULL)
    {
        printf("Erreur ouverture fichier");
        exit(1);
    }

    int index_symtab = findSymTab(section_header);
    int index_strtab = section_header->section_table[index_symtab].entree.link;

    Section symTab = fetchSection(*section_header, index_symtab);
    Section strTab = fetchSection(*section_header, index_strtab);

    fseek(f_bin, strTab.entree.offset, SEEK_SET);
    StrTab strtab = malloc(strTab.entree.size);

    fread(strtab, strTab.entree.size, 1, f_bin);

    fseek(f_bin, symTab.entree.offset, SEEK_SET);
    SymboleEntree *symtab = malloc(symTab.entree.size);
    fread(symtab, symTab.entree.size, 1, f_bin);

    for (int i = 0; i < section_header->section_number; i++)
    {
        if (section_header->section_table[i].entree.type == 9)
        {
            if (section_header->section_table[i].entree.size / 8 > 1)
            {
                printf("\nRelocation section '%s' at offset 0x%x contains %d entries:\n",
                       section_header->section_table[i].name,
                       section_header->section_table[i].entree.offset,
                       section_header->section_table[i].entree.size / 8);
            }
            else
            {
                printf("\nRelocation section '%s' at offset 0x%x contains %d entry:\n",
                       section_header->section_table[i].name,
                       section_header->section_table[i].entree.offset,
                       section_header->section_table[i].entree.size / 8);
            }
            // on se place au début de la section
            fseek(f_bin, section_header->section_table[i].entree.offset, SEEK_SET);

            printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");

            uint32_t valeur;
            for (int j = 0; j < section_header->section_table[i].entree.size / 8; j++)
            {
                fread(&valeur, 4, 1, f_bin);
                printf("%-10.8x", reverse_4(valeur));
                fread(&valeur, 4, 1, f_bin);
                printf("%-9.8x", reverse_4(valeur));

                printf("%-17.16s ", reloc_type(valeur));

                int symval = reverse_4(valeur) >> 8;
                printf("%-10.8x", reverse_4((symtab + symval)->value));

                if ((symtab + symval)->info == 3)
                {
                    afficherNameSection(symtab, section_header, symval);
                }
                else
                {
                    afficherNameOther(symtab, strtab, symval);
                }

                printf("\n");
            }
        }
    }
}
