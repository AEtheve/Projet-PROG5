#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"


FILE *useFile(char *fichier, char *mode)
{
    FILE *f = fopen(fichier, mode);
    if (f == NULL)
    {
        printf("Impossible d'ouvrir le fichier %s", fichier);
        exit(1);
    }
    return f;
}

FILE *writeStdout()
{
    FILE *stdout_f = freopen("./stdout.txt", "w", stdout);
    if (stdout_f == NULL)
    {
        printf("Impossible d'ouvrir le fichier stdout.txt");
        exit(1);
    }
    return stdout_f;
}

void clearStdout()
{
    remove("./stdout.txt");
}

void compare_files(FILE *expected, FILE *stdout_fopen, CuTest *tc)
{
    char ligne1[1000];
    char ligne2[1000];

    if (getenv("GITHUB_ACTIONS") == NULL)
    {
        freopen("/dev/tty", "w", stdout);
    }

    while (fgets(ligne2, 1000, expected) != NULL && fgets(ligne1, 1000, stdout_fopen) != NULL)
    {
        ligne1[strlen(ligne1) - 1] = '\0';
        ligne2[strlen(ligne2) - 1] = '\0';
        CuAssertStrEquals(tc, ligne1, ligne2);
    }
}

void RunTest(char *expected, CuTest *tc)
{
    compare_files(useFile(expected, "r"), useFile("./stdout.txt", "r"), tc);
}
