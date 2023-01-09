#ifndef _FUSION_H_
#define _FUSION_H_

#include <stdlib.h>
#include <stdio.h>
#include "elf32.h"
#include "util.h"
#include "fusion_section.h"
#include "fusion_relocation.h"

Elf* fusionElf(Elf* elf1, Elf* elf2);

void writeElf(FILE* f_out, Elf* content);

#endif