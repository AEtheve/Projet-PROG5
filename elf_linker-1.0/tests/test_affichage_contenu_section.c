#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CuTest/CuTest.h"
#include "./utils.h"
#include "../affichage_contenu_section.h"

void TestAffichageContenuSection1(CuTest *tc)
{
     if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -x 1 ./tests/file1.o > ./tests/file1_afficher_contenu_section_1.expected");
    } else {
        system("arm-eabi-readelf -x 1 ./tests/file1.o > ./tests/file1_afficher_contenu_section_1.expected");
    }
    writeStdout();
    affichage_contenu_section("./tests/file1.o", 1);
    RunTest("./tests/file1_afficher_contenu_section_1.expected", tc);
    clearStdout();
}

void TestAffichageContenuSection2(CuTest *tc)
{
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -x 4 ./tests/file1.o > ./tests/file1_afficher_contenu_section_4.expected");
    } else {
        system("arm-eabi-readelf -x 4 ./tests/file1.o > ./tests/file1_afficher_contenu_section_4.expected");
    }
    writeStdout();
    affichage_contenu_section("./tests/file1.o", 4);
    RunTest("./tests/file1_afficher_contenu_section_4.expected", tc);
    clearStdout();
}

void TestAffichageContenuSection3(CuTest *tc)
{
    if (system("which arm-none-eabi-readelf") == 0) {
        system("arm-none-eabi-readelf -x 5 ./tests/file1.o > ./tests/file1_afficher_contenu_section_5.expected");
    } else {
        system("arm-eabi-readelf -x 5 ./tests/file1.o > ./tests/file1_afficher_contenu_section_5.expected");
    }
    writeStdout();
    affichage_contenu_section("./tests/file1.o", 5);
    RunTest("./tests/file1_afficher_contenu_section_5.expected", tc);
    clearStdout();
}

int main(void)
{

    CuString *output = CuStringNew();
    CuSuite *suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, TestAffichageContenuSection1);
    SUITE_ADD_TEST(suite, TestAffichageContenuSection2);
    // SUITE_ADD_TEST(suite, TestAffichageContenuSection3);    

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);

    printf("%s", output->buffer);

    return suite->failCount;
}
