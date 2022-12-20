#include "affichage_entete.h"
#include "affichage_section.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint32_t name_index;
  uint32_t value;
  uint32_t size;
  unsigned char info;
  unsigned char other;
  uint16_t ndx;
} SymboleEntree;

typedef struct {
  SymboleEntree entree;
  unsigned char *name;
} Symbole;

typedef char *StrTab;

int findSymTab(SectionHeaderStruct *Shs) {
  for (int i = 0; i < Shs->section_number; i++) {
    if (Shs->section_table[i].entree.type == 2) {
      return i;
    }
  }
  return -1;
}

Section fetchSection(SectionHeaderStruct Shs, int numSec) {
  return *(Shs.section_table + numSec);
}

void afficheType(SymboleEntree *ts, int num) {
  switch ((ts + num)->info & 0x0f) {
  case 0:
    printf(" NOTYPE ");
    break;
  case 1:
    printf(" OBJECT ");
    break;
  case 2:
    printf(" FUNC   ");
    break;
  case 3:
    printf(" SECTION");
    break;
  case 4:
    printf(" FILE   ");
    break;
  case 13:
    printf(" LOPROC ");
    break;
  case 15:
    printf(" HIPROC ");
    break;
  default:
    printf(" Inconnu");
    break;
  }
}

void afficheBind(SymboleEntree *ts, int num) {
  switch ((ts + num)->info >> 4) {
  case 0:
    printf(" LOCAL ");
    break;
  case 1:
    printf(" GLOBAL");
    break;
  case 2:
    printf(" WEAK  ");
    break;
  case 13:
    printf(" LOPROC");
    break;
  case 15:
    printf(" HIPROC");
    break;
  default:
    printf(" Inconnu");
    break;
  }
}

void affichageVis(SymboleEntree *ts, int num) {
  switch ((ts + num)->other) {
  case 0:
    printf(" DEFAULT");
    break;
  case 1:
    printf(" INTERNAL");
    break;
  case 2:
    printf(" HIDDEN");
    break;
  case 3:
    printf(" PROTECTED");
    break;
  case 4:
    printf(" EXPORTED");
    break;
  case 5:
    printf(" SINGLETON");
    break;
  case 6:
    printf(" ELIMINATE");
    break;
  default:
    printf(" Inconnu");
    break;
  }
}

void afficherNdx(int num) {
  switch (num) {
  case 0:
    printf("  UND");
    break;
  case 0xfff1:
    printf("  ABS");
    break;
  case 0xfff2:
    printf(" COMMON");
    break;
  default:
    printf(" %4d", num);
    break;
  }
}

void afficherNameOther(SymboleEntree *symTab, StrTab strtab, int num) {
  printf(" %s", strtab + reverse_4((symTab + num)->name_index));
}

void afficherNameSection(SymboleEntree *symTab,
                         SectionHeaderStruct *section_header, int num) {
  printf(" %s",
         section_header->section_table[reverse_2((symTab + num)->ndx)].name);
}

int getNbSymboles(Section s) { return (s.entree.size / s.entree.entsize); }

void afficherNumValueSize(SymboleEntree *symTab, int num) {
  printf("    %2d: %08X  %4d", num, reverse_4((symTab + num)->value),
         reverse_4((symTab + num)->size));
}

int main(int argc, char *argv[]) {
  SectionHeaderStruct *section_header = valeur_section(argv[1]);

  FILE *f_bin;
  f_bin = fopen(argv[1], "rb");
  if (f_bin == NULL) {
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

  printf("Symbol table '%s' contains %d entries:\n", strTab.name,
         section_header->section_table[index_symtab].entree.size / 16);

  printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

  fseek(f_bin, symTab.entree.offset, SEEK_SET);
  SymboleEntree *symtab = malloc(symTab.entree.size);
  fread(symtab, symTab.entree.size, 1, f_bin);

  int nb_symboles = getNbSymboles(symTab);

  for (int i = 0; i < nb_symboles; i++) {
    afficherNumValueSize(symtab, i);
    afficheType(symtab, i);
    afficheBind(symtab, i);
    affichageVis(symtab, i);
    afficherNdx(reverse_2((symtab + i)->ndx));
    if ((symtab + i)->info == 3) {
      afficherNameSection(symtab, section_header, i);
    } else {
      afficherNameOther(symtab, strtab, i);
    }
    printf("\n");
  }
}
