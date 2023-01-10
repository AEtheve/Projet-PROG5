#ifndef _FUSION_RELOCATION_H_
#define _FUSION_RELOCATION_H_

#include "util.h"
#include "elf32.h"
#include "stdbool.h"
#include "string.h"
#include "affichage_tr.h"
#include "fusion_section.h"
#include "fusion_ts.h"

Elf* fusionRelocation(Elf* result, Elf* elf1, Elf* elf2);




#endif