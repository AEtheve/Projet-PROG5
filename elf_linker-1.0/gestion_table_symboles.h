#include "gestion_entete.h"
#include "gestion_section.h"
#include "elf32.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int findSymTab(Elf *elf);

Elf* getTableSymboles(Elf* elf, FILE* f_bin);

void affichageTableSymbole(Elf* elf);

void gestion_table_symboles(char* nom_fichier, bool arm_cmd_version);