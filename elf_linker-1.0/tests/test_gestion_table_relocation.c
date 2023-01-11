#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../gestion_table_relocation.h"

void TestAffichageTr1(CuTest *tc)
{
     if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -r ./tests/file1.o > ./tests/file1_afficher_tr.expected");
    } else {
        system("arm-eabi-readelf -r ./tests/file1.o > ./tests/file1_afficher_tr.expected");
    }
    writeStdout();
    gestion_table_reimplentation("./tests/file1.o");
    RunTest("./tests/file1_afficher_tr.expected", tc);
    clearStdout();
}

void TestAffichageTr2(CuTest *tc)
{
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -r ./tests/file2.o > ./tests/file2_afficher_tr.expected");
    } else {
        system("arm-eabi-readelf -r ./tests/file2.o > ./tests/file2_afficher_tr.expected");
    }
    writeStdout();
    gestion_table_reimplentation("./tests/file2.o");
    RunTest("./tests/file2_afficher_tr.expected", tc);
    clearStdout();
}

int main(void)
{

    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAffichageTr1);
    SUITE_ADD_TEST(suite, TestAffichageTr2);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}
