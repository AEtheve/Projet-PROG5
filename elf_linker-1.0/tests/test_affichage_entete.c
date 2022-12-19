#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../affichage_entete.h"

void TestAffichageEntete1(CuTest *tc)
{    
    writeStdout();
    affichage_entete("./tests/file1.o");
    RunTest("./tests/file1.expected", tc);
    clearStdout();
}

void TestAffichageEntete2(CuTest *tc)
{
    writeStdout();
    affichage_entete("./tests/file2.o");
    RunTest("./tests/file2.expected", tc);
    clearStdout();
}



int main(void)
{
    
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, TestAffichageEntete1);
    SUITE_ADD_TEST(suite, TestAffichageEntete2);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}

