#include <stdio.h>
#include <stdlib.h>
#include "affichage_table_symboles.h"
#include "affichage_section.h"
#include "util.h"

char *getName(ElfSymbole *symTab, StrTab strtab, int num)
{
  char *name = strtab + (symTab + num)->name;
  return name;
}

// char *getNameSection(ElfSymbole *symTab,
//                      ElfSection *section_header, int num)
// {
//   char *name = section_header[(symTab + num)->ndx].name;
//   return name;
// }


int sameName(ElfSymbole *symbole1, StrTab strtab1, int i, ElfSymbole *symbole2, StrTab strtab2, int j)
{
  return strcmp(getName(symbole1, strtab1, i), getName(symbole2, strtab2, j)) == 0;
}


void addSymbol(ElfSymbole *symtabFusion, int *index, int i, ElfSymbole *symtab, StrTab strtab, int *indexStrTab, int *indexShstrtab, ElfSection *section_header, StrTab shstrtabFusion, StrTab strtabFusion)
{
  symtabFusion[*index].name = symtab[i].name;
  symtabFusion[*index].value = symtab[i].value;
  symtabFusion[*index].size = symtab[i].size;
  symtabFusion[*index].info = symtab[i].info;
  symtabFusion[*index].other = symtab[i].other;
  symtabFusion[*index].ndx = symtab[i].ndx;

  *index += 1;

  if ((symtab[i].info & 0x0f) == 3) // section
  {
    strcpy(shstrtabFusion + *indexShstrtab, getName(symtab, strtab, i));
    *indexShstrtab += strlen(getName(symtab, strtab, i)) + 1;
    symtabFusion[*index - 1].name = *indexShstrtab - strlen(getName(symtab, strtab, i)) - 1;
  }
  else
  {
    strcpy(strtabFusion + *indexStrTab, getName(symtab, strtab, i));
    *indexStrTab += strlen(getName(symtab, strtab, i)) + 1;
    symtabFusion[*index - 1].name = *indexStrTab - strlen(getName(symtab, strtab, i)) - 1;
  }
}

int exactSameSymbol(ElfSymbole *symbole1, ElfSymbole *symbole2)
{
  if (symbole1->value == symbole2->value && symbole1->size == symbole2->size && symbole1->info == symbole2->info && symbole1->other == symbole2->other && symbole1->ndx == symbole2->ndx)
  {
    return 1;
  }
  return 0;
}


Elf *fusion_table_symboles(Elf *file1, Elf *file2)
{
  StrTab strtab1 = file1->string_header;
  StrTab strtab2 = file2->string_header;

  // FICHIER FUSION
  Elf *fileFusion = allocElf();
  fileFusion->nb_symbol = 0;
  fileFusion->symbol_header = allocElfSymbole(10000);
  StrTab strtabsFusion = allocStrTab(10000);
  StrTab shstrtabsFusion = allocStrTab(10000);

  int index = 0;

  int indexStrTab = 0;
  int indexShstrtab = 0;

  // on ajoute les symboles de symtab1:
  for (int i = 0; i < file1->nb_symbol; i++)
  {
    if (file1->symbol_header[i].info >> 4 == 0) // local
    {
      addSymbol(fileFusion->symbol_header, &index, i, file1->symbol_header, strtab1, &indexStrTab, &indexShstrtab, file1->section_header, shstrtabsFusion, strtabsFusion);
      fileFusion->nb_symbol++;
    }
    else
    {
      if (sameName(file1->symbol_header, strtab1, i, file2->symbol_header, strtab2, i))
      {
        if (file1->symbol_header[i].ndx != 0) // def dans 1
        {
          if (file2->symbol_header[i].ndx != 0) // def dans 2
          {
            printf("Erreur: symbole %s defini dans les deux fichiers", getName(file1->symbol_header, strtab1, i));
            exit(1);
          }
          else // def dans 1 et non def dans 2
          {
            addSymbol(fileFusion->symbol_header, &index, i, file1->symbol_header, strtab1, &indexStrTab, &indexShstrtab, file1->section_header, shstrtabsFusion, strtabsFusion);
            fileFusion->nb_symbol++;
          }
        }
        else
        { // on ajoute dans fusion element de fichier 2
          addSymbol(fileFusion->symbol_header, &index, i, file2->symbol_header, strtab2, &indexStrTab, &indexShstrtab, file2->section_header, shstrtabsFusion, strtabsFusion);
          fileFusion->nb_symbol++;
        }
      }
      else
      {
        addSymbol(fileFusion->symbol_header, &index, i, file1->symbol_header, strtab1, &indexStrTab, &indexShstrtab, file1->section_header, shstrtabsFusion, strtabsFusion);
        fileFusion->nb_symbol++;
      }
    }
  }

  // On va ajouter les locaux de symtab2 qui ne sont pas déjà dans symtabFusion
  for (int i = 0; i < file2->nb_symbol; i++)
  {
    if (file2->symbol_header[i].info >> 4 == 0) // local
    {
      // On vérifie que le symbole n'est pas déjà dans symtabFusion
      int dejaPresent = 0;
      for (int j = 0; j < fileFusion->nb_symbol; j++)
      {
        // if (sameName(file2->symbol_header, strtab2, i, fileFusion->symbol_header, strtabsFusion, j))
        if (exactSameSymbol(file2->symbol_header + i, fileFusion->symbol_header + j))
        {
          dejaPresent = 1;
          break;
        }
      }
      if (dejaPresent == 0)
      {
        addSymbol(fileFusion->symbol_header, &index, i, file2->symbol_header, strtab2, &indexStrTab, &indexShstrtab, file2->section_header, shstrtabsFusion, strtabsFusion);
        fileFusion->nb_symbol++;
      }
    }
    else
    {
      // On vérifie que le symbole n'est pas déjà dans symtabFusion
      int dejaPresent = 0;
      for (int j = 0; j < fileFusion->nb_symbol; j++)
      {
        if (sameName(file2->symbol_header, strtab2, i, fileFusion->symbol_header, strtabsFusion, j))
        {
          dejaPresent = 1;
          break;
        }
      }
      if (dejaPresent == 0)
      {
        addSymbol(fileFusion->symbol_header, &index, i, file2->symbol_header, strtab2, &indexStrTab, &indexShstrtab, file2->section_header, shstrtabsFusion, strtabsFusion);
        fileFusion->nb_symbol++;
      }
    }
  }

  fileFusion->section_header = file1->section_header;
  // printf("sectionheader %d", fileFusion->section_header == NULL);
  // exit(1);
  fileFusion->string_header = strtabsFusion;
  return fileFusion;
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

  Elf *fusion = fusion_table_symboles(elf1, elf2);

  affichageTableSymbole(fusion);

  fermetureFichier(f1);
  fermetureFichier(f2);
  return 0;
}
