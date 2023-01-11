#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../gestion_section.h"

void TestAffichageSection(CuTest *tc)
{
    bool arm_cmd_version = 0;
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -S ./tests/file2.o > ./tests/file2_section.expected");
        arm_cmd_version = 1;
    } else {
        system("arm-eabi-readelf -S ./tests/file2.o > ./tests/file2_section.expected");
    }

    writeStdout();
    gestion_section("./tests/file2.o", arm_cmd_version);
    RunTest("./tests/file2_section.expected", tc);
    clearStdout();
}

int main(void)
{

    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAffichageSection);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}
