#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "../affichage_entete.h"

void TestAffichageEntete(CuTest *tc)
{
    FILE *expected = fopen("./tests/file2.expected", "r");
    if (expected == NULL)
    {
        printf("Impossible d'ouvrir le fichier test1.expected");
        exit(1);
    }
    
    FILE *stdout_f = freopen("./stdout.txt", "w", stdout);

    if (stdout_f == NULL)
    {
        printf("Impossible d'ouvrir le fichier stdout.txt");
        exit(1);
    }

    affichage_entete("./tests/file2.o");

    fclose(stdout_f);

    if (getenv("GITHUB_ACTIONS") == NULL)
    {
        stdout_f = freopen("/dev/tty", "w", stdout);
    }
    
    FILE *stdout_fopen = fopen("./stdout.txt", "r");
    if (stdout_fopen == NULL)
    {
        printf("Impossible d'ouvrir le fichier stdout.txt");
        exit(1);
    }

    char ligne1[1000];
    char ligne2[1000];

    while (fgets(ligne2, 1000, expected) != NULL && fgets(ligne1, 1000, stdout_fopen) != NULL)
    {
        ligne1[strlen(ligne1) - 1] = '\0';
        ligne2[strlen(ligne2) - 1] = '\0';
        CuAssertStrEquals(tc, ligne1, ligne2);
    }

    fclose(stdout_fopen);
    fclose(expected);
    remove("./stdout.txt");
}

int main(void)
{
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestAffichageEntete);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}

