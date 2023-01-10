#ifndef _FUSION_SECTION_H_
#define _FUSION_SECTION_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "affichage_section.h"
#include "elf32.h"


Elf* fusionSection(Elf* elf1, Elf* elf2);

int findSection(Elf* elf, const char* name);



#endif