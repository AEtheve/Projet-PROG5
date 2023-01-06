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

void affichageContenuSection(Elf* elf, FILE* f_bin, int select_section){;

  fseek(f_bin, elf->section_header[select_section].entree.offset, SEEK_SET);

  uint8_t lettre;
  int j = 0;
  uint8_t ligne[1000];

  if (elf->section_header[select_section].entree.size == 0) {
    printf("Section '%s' has no data to dump.\n",
           elf->section_header[select_section].name);
  } else {
    printf("\nHex dump of section '%s':\n",
           elf->section_header[select_section].name);
    for (int i = 0; i < elf->header->e_section_header_entry_count; i++) {
      if (elf->section_header[i].entree.type == 9) { // REL
        if (elf->section_header[i].entree.info == select_section) {
          printf(" NOTE: This section has relocations against it, but these "
                 "have NOT been applied to this dump.\n");
        }
      }
    }
  }

  while (j < elf->section_header[select_section].entree.size) {
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

void affichage_contenu_section(char* nom_fichier, int select_section){
  FILE* f = ouvertureFichier(nom_fichier, "rb");
  Elf* elf = valeurEntete(f);
  elf = valeurSection(elf, f);
  affichageContenuSection(elf, f, select_section);
  fermetureFichier(f);
}

// int main(int argc, char** argv){
//   affichage_contenu_section(argv[1], 0);
//   affichage_contenu_section(argv[1], 1);
//   affichage_contenu_section(argv[1], 2);
//   affichage_contenu_section(argv[1], 3);
//   affichage_contenu_section(argv[1], 4);
//   affichage_contenu_section(argv[1], 5);
//   affichage_contenu_section(argv[1], 6);
//   affichage_contenu_section(argv[1], 7);
//   affichage_contenu_section(argv[1], 8);
//   affichage_contenu_section(argv[1], 9);
//   affichage_contenu_section(argv[1], 10);
//   affichage_contenu_section(argv[1], 11);
//   affichage_contenu_section(argv[1], 12);
//   affichage_contenu_section(argv[1], 13);
//   affichage_contenu_section(argv[1], 14);
//   affichage_contenu_section(argv[1], 15);
//   affichage_contenu_section(argv[1], 16);
//   affichage_contenu_section(argv[1], 17);
//   affichage_contenu_section(argv[1], 18);
//   affichage_contenu_section(argv[1], 19);
//   affichage_contenu_section(argv[1], 20);
//   affichage_contenu_section(argv[1], 21);
//   affichage_contenu_section(argv[1], 22);
//   return 0;
// }