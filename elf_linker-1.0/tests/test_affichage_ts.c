#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../affichage_table_symboles.h"

void TestAffichageTableSymboles1(CuTest *tc)
{
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -h ./tests/file2.o > ./tests/file2_affiche_tab_symb.expected");
    } else {
        system("arm-eabi-readelf -h ./tests/file2.o > ./tests/file2_affiche_tab_symb.expected");
    }
    writeStdout();
    affichage_table_symboles("./tests/file2.o");
    RunTest("./tests/file2_affiche_tab_symb.expected", tc);
    clearStdout();
}

void TestAffichageTableSymboles2(CuTest *tc)
{   
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -h ./tests/file1.o > ./tests/file1_affiche_tab_symb.expected");
    } else {
        system("arm-eabi-readelf -h ./tests/file1.o > ./tests/file1_affiche_tab_symb.expected");
    }
    writeStdout();
    affichage_table_symboles("./tests/file1.o");
    RunTest("./tests/file1_affiche_tab_symb.expected", tc);
    clearStdout();
}

int main(void)
{

    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAffichageTableSymboles1);
    SUITE_ADD_TEST(suite, TestAffichageTableSymboles2);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}
