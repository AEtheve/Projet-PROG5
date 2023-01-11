#include "affichage_table_symboles.h"

int findSymTab(Elf *elf) {
    for (int i = 0; i < elf->header->e_section_header_entry_count; i++)
    {
        if (elf->section_header[i].entree.type == 2)
        {
            return i;
        }
    }
    return -1;
}

char *afficheType(ElfSymbole *ts, int num) {
    char *type = malloc(10);
    switch ((ts + num)->info & 0x0f)
    {
    case 0:
        strcpy(type, "NOTYPE");
        break;
    case 1:
        strcpy(type, "OBJECT");
        break;
    case 2:
        strcpy(type, "FUNC");
        break;
    case 3:
        strcpy(type, "SECTION");
        break;
    case 4:
        strcpy(type, "FILE");
        break;
    case 13:
        strcpy(type, "LOPROC");
        break;
    case 15:
        strcpy(type, "HIPROC");
        break;
    default:
        strcpy(type, "Inconnu");
        break;
    }
    return type;
}

char *afficheBind(ElfSymbole *ts, int num) {
    char *bind = malloc(10);
    switch ((ts + num)->info >> 4)
    {
    case 0:
        strcpy(bind, "LOCAL");
        break;
    case 1:
        strcpy(bind, "GLOBAL");
        break;
    case 2:
        strcpy(bind, "WEAK");
        break;
    case 13:
        strcpy(bind, "LOPROC");
        break;
    case 15:
        strcpy(bind, "HIPROC");
        break;
    default:
        strcpy(bind, "Inconnu");
        break;
    }
    return bind;
}

char *afficheVis(ElfSymbole *ts, int num) {
    char *vis = malloc(10);
    switch ((ts + num)->other)
    {
    case 0:
        strcpy(vis, "DEFAULT");
        break;
    case 1:
        strcpy(vis, "INTERNAL");
        break;
    case 2:
        strcpy(vis, "HIDDEN");
        break;
    case 3:
        strcpy(vis, "PROTECTED");
        break;
    case 4:
        strcpy(vis, "EXPORTED");
        break;
    case 5:
        strcpy(vis, "SINGLETON");
        break;
    case 6:
        strcpy(vis, "ELIMINATE");
        break;
    default:
        strcpy(vis, "Inconnu");
        break;
    }
    return vis;
}

char *afficheNdx(int num) {
    char *ndx = malloc(10);
    switch (num)
    {
    case 0:
        strcpy(ndx, "UND");
        break;
    case 0xfff1:
        strcpy(ndx, "ABS");
        break;
    case 0xfff2:
        strcpy(ndx, "COMMON");
        break;
    default:
        sprintf(ndx, "%d", num);
        break;
    }
    return ndx;
}

Elf *getTableSymboles(Elf *elf, FILE *f_bin)
{
    int index_symbol = findSymTab(elf);
    int index_string = elf->section_header[index_symbol].entree.link;

    if (index_symbol == -1)
    {
        printf("Pas de table de symbole\n");
        return elf;
    }

    SectionHeader *symbol_header = &elf->section_header[index_symbol].entree;
    SectionHeader *string_header = &elf->section_header[index_string].entree;

    ElfSymbole *symbol = allocElfSymbole(symbol_header->size);
    StrTab string = allocStrTab(string_header->size);

    fseek(f_bin, symbol_header->offset, SEEK_SET);
    fread(symbol, symbol_header->size, 1, f_bin);

    for(int i = 0; i < symbol_header->size / symbol_header->entsize; i++)
    {
        symbol[i].name = reverse_4(symbol[i].name);
        symbol[i].value = reverse_4(symbol[i].value);
        symbol[i].size = reverse_4(symbol[i].size);
        symbol[i].ndx = reverse_2(symbol[i].ndx);
    }

    fseek(f_bin, string_header->offset, SEEK_SET);
    fread(string, string_header->size, 1, f_bin);

    elf->symbol_header = symbol;
    elf->nb_symbol = symbol_header->size / symbol_header->entsize;
    elf->string_header = string;

    return elf;
}

void affichageTableSymbole(Elf *elf)
{
    

    printf("\nSymbol table '.symtab' contains %d entries:\n",
           elf->nb_symbol);
    printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
    
    for (int i = 0; i < elf->nb_symbol; i++)
    {
        printf("   %3d: %08x %5d %-7s %-6s %-8s %3s ",
               i,
               elf->symbol_header[i].value,
               elf->symbol_header[i].size,
               afficheType(elf->symbol_header, i),
               afficheBind(elf->symbol_header, i),
               afficheVis(elf->symbol_header, i),
               afficheNdx((elf->symbol_header + i)->ndx)); //   %s
        if ((elf->symbol_header + i)->info == 3)
        {	
            printf("%s", elf->section_header[(elf->symbol_header + i)->ndx].name);
        }
        else
        {
            printf("%s", elf->string_header + ((elf->symbol_header + i)->name));
        }
        printf("\n");
    }
}

void affichage_table_symboles(char *nom_fichier, bool arm_cmd_version)
{
    FILE *f = ouvertureFichier(nom_fichier, "rb");
    Elf *elf = valeurEntete(f);
    elf = valeurSection(elf, f);

    elf = getTableSymboles(elf, f);
    affichageTableSymbole(elf);

    fermetureFichier(f);
}

// int main(int argc, char **argv)
// {
//     affichage_table_symboles(argv[1], false);
//     return 0;
// }
