#include "affichage_entete.h"
#include "affichage_section.h"
#include "elf32.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int findSymTab(Elf *elf);

Elf* getTableSymboles(Elf* elf, FILE* f_bin);

void affichageTableSymbole(Elf* elf);

void affichage_table_symboles(char* nom_fichier, bool arm_cmd_version);
