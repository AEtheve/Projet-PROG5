#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../gestion_entete.h"

void TestAffichageEntete1(CuTest *tc)
{    
     if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -h ./tests/file1.o > ./tests/file1.expected");
    } else {
        system("arm-eabi-readelf -h ./tests/file1.o > ./tests/file1.expected");
    }
    writeStdout();
    gestion_entete("./tests/file1.o");
    clearStdout();
}

void TestAffichageEntete2(CuTest *tc)
{
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -h ./tests/file2.o > ./tests/file2.expected");
    } else {
        system("arm-eabi-readelf -h ./tests/file2.o > ./tests/file2.expected");
    }
    writeStdout();
    gestion_entete("./tests/file2.o");
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

