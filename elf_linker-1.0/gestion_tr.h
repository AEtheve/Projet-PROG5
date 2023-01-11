#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "elf32.h"
#include "util.h"
#include "gestion_entete.h"
#include "gestion_table_symboles.h"
#include "gestion_section.h"

Elf* getTableRelocation(Elf *elf, FILE* f_bin);

void affichageTableReimplentation(Elf *elf);

void gestion_table_reimplentation(char *nom_fichier);