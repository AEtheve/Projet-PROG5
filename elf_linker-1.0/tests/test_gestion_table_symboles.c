#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../gestion_table_symboles.h"

void TestAffichageTableSymboles1(CuTest *tc)
{
    bool arm_cmd_version = 0;
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -s ./tests/file2.o > ./tests/file2_affiche_tab_symb.expected");
        arm_cmd_version = 1;
    } else {
        system("arm-eabi-readelf -s ./tests/file2.o > ./tests/file2_affiche_tab_symb.expected");
    }
    writeStdout();
    gestion_table_symboles("./tests/file2.o", arm_cmd_version);
    RunTest("./tests/file2_affiche_tab_symb.expected", tc);
    clearStdout();
}

void TestAffichageTableSymboles2(CuTest *tc)
{   
    bool arm_cmd_version = 0;
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -s ./tests/file1.o > ./tests/file1_affiche_tab_symb.expected");
        arm_cmd_version = 1;

    } else {
        system("arm-eabi-readelf -s ./tests/file1.o > ./tests/file1_affiche_tab_symb.expected");
    }
    writeStdout();
    gestion_table_symboles("./tests/file1.o", arm_cmd_version);
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
