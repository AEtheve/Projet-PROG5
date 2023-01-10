#ifndef _FUSION_H_
#define _FUSION_H_

#include <stdlib.h>
#include <stdio.h>
#include "elf32.h"
#include "util.h"
#include "affichage_table_symboles.h"
#include "fusion_section.h"
#include "fusion_relocation.h"
#include "fusion_ts.h"
#include "error.h"

Elf* fusionElf(Elf* elf1, Elf* elf2);

WriteError writeElf(FILE* f_out, Elf* content, bool strict_mode);

#endif