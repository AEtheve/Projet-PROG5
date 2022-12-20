#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "affichage_section.h"

void printAsciiLine(uint8_t *line, int size) {
  for (int k = 0; k < size; k++) {
    if (line[k] >= 32 && line[k] <= 126) {
      printf("%c", line[k]);
    } else {
      printf(".");
    }
  }
}

void printIndent(int n) {
  for (int k = 0; k < n; k++) {
    printf("  ");
    if (k % 4 == 0) {
      printf(" ");
    }
  }
}

void affichage_contenu_section(char* nom_fichier, int select_section){
  SectionHeaderStruct *section_header = valeur_section(nom_fichier);

  FILE *f_bin;
  f_bin = fopen(nom_fichier, "rb");
  if (f_bin == NULL) {
    printf("Erreur ouverture fichier");
    exit(1);
  }
  fseek(f_bin, section_header->section_table[select_section].entree.offset,
        SEEK_SET);

  uint8_t lettre;
  int j = 0;
  uint8_t ligne[1000];

  if (section_header->section_table[select_section].entree.size == 0) {
    printf("Section '%s' has no data to dump.\n",
           section_header->section_table[select_section].name);
  } else {
    printf("Hex dump of section '%s':\n",
           section_header->section_table[select_section].name);
    for (int i = 0; i < section_header->section_number; i++) {
      if (section_header->section_table[i].entree.type == 9) { // REL
        if (section_header->section_table[i].entree.info == select_section) {
          printf(" NOTE: This section has relocations against it, but these "
                 "have NOT been applied to this dump.\n");
        }
      }
    }
  }

  while (j < section_header->section_table[select_section].entree.size) {
    if (j % 16 == 0) {
      printf("  0x%08x ", j);
    }
    fread(&lettre, 1, 1, f_bin);
    printf("%02x", lettre);
    ligne[j % 16] = lettre;
    j++;
    if (j % 4 == 0) {
      printf(" ");
    }
    if (j % 16 == 0) {
      printAsciiLine(ligne, 16);
      printf("\n");
    }
  }
  if (j % 16 != 0) {
    printIndent(16 - j % 16);
    printAsciiLine(ligne, j % 16);
    printf("\n");
  }
}