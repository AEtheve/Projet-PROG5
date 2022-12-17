#include "CuTest/CuTest.h"

void RunTest(char *expected, CuTest *tc);
void compare_files(FILE *expected, FILE *stdout_fopen, CuTest *tc);
FILE *useFile(char *fichier, char *mode);
FILE *writeStdout();
void clearStdout();