#include "gestion_entete.h"


void printMagic(ElfHeader *header)
{
    printf(" Magic:   ");
    for (int i = 0; i < 16; i++)
    {
        printf("%02x ", header->e_ident[i]);
    }
    printf("\n");
}

void printClass(ElfHeader *header)
{

    printf("  Class:                             ");
    if (header->e_ident[4] == 1)
    {
        printf("ELF32\n");
    }
    else if (header->e_ident[4] == 2)
    {
        printf("ELF64\n");
    }
    else
    {
        printf("Aucun\n");
    }
}


void printData(ElfHeader *header)
{
    printf("  Data:                              ");
    if (header->e_ident[5] == 1)
    {
        printf("2's complement, little endian\n");
    }
    else if (header->e_ident[5] == 2)
    {
        printf("2's complement, big endian\n");
    }
    else
    {
        printf("Aucun\n");
    }
}

void printVersion(ElfHeader *header)
{
    printf("  Version:                           ");
    if (header->e_ident[6] == 1)
    {
        printf("1 (current)\n");
    }
    else
    {
        printf("Aucun\n");
    }
}


void printAbi(ElfHeader *header)
{
    printf("  OS/ABI:                            ");
    switch (header->e_ident[7])
    {
    case 0:
        printf("UNIX - System V\n");
        break;
    case 1:
        printf("HP-UX\n");
        break;
    case 2:
        printf("NetBSD\n");
        break;
    case 3:
        printf("Linux\n");
        break;
    case 6:
        printf("Solaris\n");
        break;
    case 7:
        printf("IBM AIX\n");
        break;
    case 8:
        printf("SGI IRIX\n");
        break;
    case 9:
        printf("FreeBSD\n");
        break;
    case 10:
        printf("CompaqTRU64\n");
        break;
    case 11:
        printf("Novell Modesto\n");
        break;
    case 12:
        printf("OpenBSD\n");
        break;
    case 64:
        printf("ARM\n");
        break;
    case 97:
        printf("ARM AArch64\n");
        break;
    default:
        printf("Aucun\n");
        break;
    }
}

void printAbiVersion(ElfHeader *header)
{
    printf("  ABI Version:                       %d\n", header->e_ident[8]);
}

void printType(ElfHeader *header)
{
    printf("  Type:                              ");
    switch (header->e_type)
    {
    case 0:
        printf("No file type\n");
        break;
    case 1:
        printf("REL (Relocatable file)\n");
        break;
    case 2:
        printf("EX (Executable file)\n");
        break;
    case 3:
        printf("SHA (Shared object file)\n");
        break;
    case 4:
        printf("Core file\n");
        break;
    case 0xff00:
        printf("Processor-specific\n");
        break;
    case 0xffff:
        printf("Processor-specific\n");
        break;
    default:
        printf("Erreur");
        exit(1);
    }
}

void printMachine(ElfHeader *header)
{
    printf("  Machine:                           ");
    switch (header->e_machine)
    {
    case 1:
        printf("AT&T WE 32100\n");
        break;
    case 2:
        printf("SPARC\n");
        break;
    case 3:
        printf("Intel Architecture\n");
        break;
    case 4:
        printf("Motorola 68000\n");
        break;
    case 5:
        printf("Motorola 88000\n");
        break;
    case 7:
        printf("Intel 80860\n");
        break;
    case 8:
        printf("MIPS RS3000 Big-endian\n");
        break;
    case 10:
        printf("MIPS RS4000 Big-endian\n");
        break;
    case 40:
        printf("ARM\n");
        break;
    default:
        printf("\n");
        break;
    }
}

void printVersion2(ElfHeader *header)
{
    printf("  Version:                           ");
    printf("0x%x\n", header->e_version);
}

void printPointDentree(ElfHeader *header)
{
    printf("  Entry point address:               ");
    printf("0x%x\n", header->e_entry);
}


void printStartOfProgramHeaders(ElfHeader *header)
{
    printf("  Start of program headers:          ");
    printf("%d (bytes into file)\n", header->e_program_header_off);
}

void printStartOfSectionHeaders(ElfHeader *header)
{
    printf("  Start of section headers:          ");
    printf("%d (bytes into file)\n", header->e_section_header_off);
}

void printFlags(ElfHeader *header)
{
    printf("  Flags:                             ");
    printf("0x%x, Version5 EABI\n", header->e_flags);
}

void printsizeOfHeaders(ElfHeader *header)
{
    printf("  Size of this header:               ");
    printf("%d (bytes)\n", header->e_header_size);
}

void printsizeOfProgramHeaders(ElfHeader *header)
{
    printf("  Size of program headers:           ");
    printf("%d (bytes)\n", header->e_program_header_entry_size);
}

void printNumberOfProgramHeaders(ElfHeader *header)
{
    printf("  Number of program headers:         ");
    printf("%d\n", header->e_program_header_entry_count);
}

void printSizeOfSectionHeaders(ElfHeader *header)
{
    printf("  Size of section headers:           ");
    printf("%d (bytes)\n", header->e_section_header_entry_size);
}

void printNumberOfSectionHeaders(ElfHeader *header)
{
    printf("  Number of section headers:         ");
    printf("%d\n", header->e_section_header_entry_count);
}

void printSectionHeaderStringTableIndex(ElfHeader *header)
{
    printf("  Section header string table index: ");
    printf("%d\n", header->e_section_header_string_table_index);
}

Elf *valeurEntete(FILE* f_bin) {
    ElfHeader *h = allocElfHeader();

    fread(h, 1, sizeof(ElfHeader), f_bin);

    h->e_type = reverse_2(h->e_type);
    h->e_machine = reverse_2(h->e_machine);
    h->e_version = reverse_4(h->e_version);
    h->e_entry = reverse_4(h->e_entry);
    h->e_program_header_off = reverse_4(h->e_program_header_off);
    h->e_section_header_off = reverse_4(h->e_section_header_off);
    h->e_flags = reverse_4(h->e_flags);
    h->e_header_size = reverse_2(h->e_header_size);
    h->e_program_header_entry_size = reverse_2(h->e_program_header_entry_size);
    h->e_program_header_entry_count = reverse_2(h->e_program_header_entry_count);
    h->e_section_header_entry_size = reverse_2(h->e_section_header_entry_size);
    h->e_section_header_entry_count = reverse_2(h->e_section_header_entry_count);
    h->e_section_header_string_table_index = reverse_2(h->e_section_header_string_table_index);


    Elf* elf = allocElf();
    elf->header = h;

    return elf;
}

void affichageEntete(Elf* elf){
    printf("ELF Header:\n ");
    printMagic(elf->header);
    printClass(elf->header);
    printData(elf->header);
    printVersion(elf->header);
    printAbi(elf->header);
    printAbiVersion(elf->header);
    printType(elf->header);
    printMachine(elf->header);
    printVersion2(elf->header);
    printPointDentree(elf->header);
    printStartOfProgramHeaders(elf->header);
    printStartOfSectionHeaders (elf->header);
    printFlags(elf->header);
    printsizeOfHeaders(elf->header);
    printsizeOfProgramHeaders(elf->header);
    printNumberOfProgramHeaders(elf->header);
    printSizeOfSectionHeaders(elf->header);
    printNumberOfSectionHeaders(elf->header);
    printSectionHeaderStringTableIndex(elf->header);
}

void gestion_entete(char *nom_fichier){
    FILE* f = ouvertureFichier(nom_fichier, "rb");
    Elf* elf = valeurEntete(f);
    affichageEntete(elf);
    fermetureFichier(f);
}
