#include <stdio.h>
#include <stdlib.h>
#include "affichage_table_symboles.h"
#include "affichage_section.h"
#include "util.h"
#include "fusion_section.h"
#include "fusion_ts.h"

char *getName(ElfSymbole *symTab, StrTab strtab, int num)
{
  char *name = strtab + (symTab + num)->name;
  return name;
}

char *getNameSection(ElfSymbole *symTab,
                     ElfSection *section_header, int num)
{
  char *name = section_header[(symTab + num)->ndx].name;
  return name;
}

void addSymboleOther(Elf *dest, Elf *source, ElfSymbole symbol, StrTab strtab, int index, int *indexStrTab)
  {
  symbol.name = source->symbol_header[index].name;
  symbol.value = source->symbol_header[index].value;
  symbol.size = source->symbol_header[index].size;
  symbol.info = source->symbol_header[index].info;
  symbol.other = source->symbol_header[index].other;
  symbol.ndx = source->symbol_header[index].ndx;

  char *name = getName(source->symbol_header, source->string_header, index);
  strcpy(strtab + *indexStrTab, name);
  symbol.name = *indexStrTab;
  *indexStrTab += strlen(name) + 1;
  addSymbol(dest, symbol);
}

void addSymboleSection(Elf *dest, Elf *source, ElfSymbole symbol, StrTab strtab, int index, int section)
  {
  symbol.name = source->symbol_header[index].name;
  symbol.value = source->symbol_header[index].value;
  symbol.size = source->symbol_header[index].size;
  symbol.info = source->symbol_header[index].info;
  symbol.other = source->symbol_header[index].other;
  symbol.ndx = section;
  addSymbol(dest, symbol);
}

Elf *fusion_table_symboles(Elf *file1, Elf *file2, Elf *elf_fusion)
{
  elf_fusion->nb_symbol = 0;
  StrTab strtab1 = file1->string_header;
  StrTab strtab2 = file2->string_header;
  StrTab strtabFusion = allocStrTab(1000);

  
  ElfSymbole symbol;

  int indexStrTab = 0;

  // on ajoute tous les locaux de file1
  for (int i = 0; i < file1->nb_symbol; i++)
  {
    if (file1->symbol_header[i].info >> 4 == 0) // locaux
    {
        // si c'est une section on ajoute une section sinon on ajoute un symbole
        if (file1->symbol_header[i].info == 3) {
          char *section = getNameSection(file1->symbol_header, file1->section_header, i);
          addSymboleSection(elf_fusion, file1, symbol, strtabFusion, i, findSection(elf_fusion, section));
        }
        else {
          addSymboleOther(elf_fusion, file1, symbol, strtabFusion, i, &indexStrTab);
        }
    } else{
      // les globaux de file1
      // on parcours les globaux de file2, si on trouve un symbole avec le meme nom,
      // si le symbole est défini dans file1 et file2 on affiche une erreur,
      // si le symbole n'est pas défini dans file1 on ajoute celui de file2,
      // sinon on ajoute celui de file1
      for (int j = 0; j < file2->nb_symbol; j++)
      {
        if (file2->symbol_header[j].info >> 4 == 1) // globaux
        {
          char *name1 = getName(file1->symbol_header, strtab1, i);
          char *name2 = getName(file2->symbol_header, strtab2, j);
          if (strcmp(name1, name2) == 0)
          {
            if (file1->symbol_header[i].info == 2 && file2->symbol_header[j].info == 2)
            {
              printf("Erreur : symbole %s défini dans les deux fichiers \r \n", name1);
              exit(1);
            }
            else if (file1->symbol_header[i].info != 2)
            {
              addSymboleOther(elf_fusion, file2, symbol, strtabFusion, j, &indexStrTab);
            }
            else
            {
              addSymboleOther(elf_fusion, file1, symbol, strtabFusion, i, &indexStrTab);
            }
          }
        }
      }
    }
  }

  // on ajoute les locaux de file2 qui ne sont pas des sections
  for (int i = 0; i < file2->nb_symbol; i++)
  {
    if (file2->symbol_header[i].info >> 4 == 0) // locaux
    {
      if (file2->symbol_header[i].info != 3) {
        addSymboleOther(elf_fusion, file2, symbol, strtabFusion, i, &indexStrTab);
      } else{
        char *section = getNameSection(file2->symbol_header, file2->section_header, i);
         // si cette section n'existe pas dans fusion (findsection = 0) on l'ajoute (symbole) dans fusion 
         if (findSection(elf_fusion, section) == 0) {
          addSymboleSection(elf_fusion, file2, symbol, strtabFusion, i, findSection(elf_fusion, section));
         }

      }
    } else{
      // Les globaux de file2:
      // on ajoute les globaux qui n'existe pas dans fusion:
      int present = 0;
      for (int j = 0; j < elf_fusion->nb_symbol; j++)
      {
        if (elf_fusion->symbol_header[j].info >> 4 == 1) // globaux
        {
          char *name1 = getName(elf_fusion->symbol_header, strtabFusion, j);
          char *name2 = getName(file2->symbol_header, strtab2, i);
          if (strcmp(name1, name2) == 0)
          {
            present = 1;
          }
        }
      }
      if (present == 0)
      {
        addSymboleOther(elf_fusion, file2, symbol, strtabFusion, i, &indexStrTab);
      }
    }
  }
  


  elf_fusion->string_header = strtabFusion;
  return elf_fusion;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: %s <fichier1> <fichier2> \r \n", argv[0]);
    exit(1);
  }

  FILE *f1 = ouvertureFichier(argv[1], "rb");
  FILE *f2 = ouvertureFichier(argv[2], "rb");

  Elf *elf1 = valeurEntete(f1);
  Elf *elf2 = valeurEntete(f2);

  elf1 = valeurSection(elf1, f1);
  elf2 = valeurSection(elf2, f2);

  elf1 = getTableSymboles(elf1, f1);
  elf2 = getTableSymboles(elf2, f2);


  Elf* elf_fusion = fusionSection(elf1, elf2);

  Elf *fusion = fusion_table_symboles(elf1, elf2, elf_fusion);

  affichageTableSymbole(fusion);

  fermetureFichier(f1);
  fermetureFichier(f2);
  return 0;
}