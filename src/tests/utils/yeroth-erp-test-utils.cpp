/*
 * yeroth-erp-test-utils.cpp
 *
 *      @author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

#include "yeroth-erp-test-utils.hpp"


#include "src/utils/yeroth-erp-database.hpp"


#include <QtTest/QtTest>


YerothERPWindows *Test_YerothERPTestUtils::_allWindows(0);


void Test_YerothERPTestUtils::TEST_UTILS_truncate_database_all_tables()
{
    YerothDatabase *yeroth_erp_3_0_db = YerothUtils::getDatabase();

    QVERIFY(0 != yeroth_erp_3_0_db);

    YerothUtils::executer_fichier_sql
    ("yeroth-erp-3-0-test_data/truncate_test_yeroth_erp_3_test.sql");
}


void Test_YerothERPTestUtils::setAllWindows(YerothERPWindows *allWindows)
{
    QVERIFY(0 != allWindows);

    _allWindows = allWindows;
}


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */
