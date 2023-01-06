#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf32.h"
#include "util.h"
#include "affichage_entete.h"
#include "affichage_table_symboles.h"
#include "affichage_section.h"

Elf* getTableRelocation(Elf *elf, FILE* f_bin);

void affichageTableReimplentation(Elf *elf);

void affichage_table_reimplentation(char *nom_fichier);