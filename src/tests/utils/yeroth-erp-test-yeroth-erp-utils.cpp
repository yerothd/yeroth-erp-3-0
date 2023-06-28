/*
 * yeroth-erp-test-yeroth-erp-utils.cpp
 *
 *      @author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include "yeroth-erp-test-yeroth-erp-utils.hpp"


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QStringList>

#include <QtTest/QtTest>


void Test_YerothUtils::test_CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING()
{
    QString test_NO_SPLIT_STAR_SEPARATED_string(YerothUtils::EMPTY_STRING);

    QString test_STR(YerothUtils::EMPTY_STRING);

    bool CONTAINS_NO_DUPLICATE_STRING =
                    YerothUtils::CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING
                    (test_NO_SPLIT_STAR_SEPARATED_string,
                     test_STR);

    QVERIFY(false == CONTAINS_NO_DUPLICATE_STRING);

    test_STR = "trois";

    test_NO_SPLIT_STAR_SEPARATED_string =
                    QString("un*deux*%1*%2").arg(test_STR, test_STR);

    CONTAINS_NO_DUPLICATE_STRING =
                    YerothUtils::CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING
                    (test_NO_SPLIT_STAR_SEPARATED_string, test_STR);

    QVERIFY(true == CONTAINS_NO_DUPLICATE_STRING);
}


void Test_YerothUtils::test_REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING()
{
    QString string_TWO("two");

    QString STRING_TEST_WITH_STARS_NOT_SEPARATED =
                    QString("one*%1*three*four*five").arg(string_TWO);

    QString result_separated_string_WITH_NO_STARS_in_out =
                    STRING_TEST_WITH_STARS_NOT_SEPARATED;

    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
    (result_separated_string_WITH_NO_STARS_in_out, string_TWO);

    QDEBUG_STRING_OUTPUT_2("result_separated_string_WITH_NO_STARS_in_out",
                           result_separated_string_WITH_NO_STARS_in_out);


    QVERIFY(true ==
            !result_separated_string_WITH_NO_STARS_in_out.contains(string_TWO,
                                                                   Qt::
                                                                   CaseInsensitive));
}


#endif //YEROTH_ERP_3_0_TESTING_UNIT_TEST
