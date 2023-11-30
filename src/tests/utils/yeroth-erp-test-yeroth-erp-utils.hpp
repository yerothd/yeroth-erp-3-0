/*
 * yeroth-erp-test-yeroth-erp-utils.hpp
 *
 *      @author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_TEST_YEROTH_ERP_UTILS_WINDOWS_HPP_
#define YEROTH_TEST_YEROTH_ERP_UTILS_WINDOWS_HPP_


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST


#include "src/include/yeroth-erp-3-0-definition-oo-class-operators.hpp"

#include "src/tests/utils/yeroth-erp-test-utils.hpp"


#include <QtCore/QObject>


class YerothUtils;


class Test_YerothUtils:public QObject
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS inline Test_YerothUtils()
    {
    }

    inline ~Test_YerothUtils()
    {
    }

private slots:
    inline void initTestCase()
    {
    }


    void test_CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING();

    void test_REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING();

private:

    YerothUtils *_yeroth_erp_utils;
};


#endif /* YEROTH_ERP_3_0_TESTING_UNIT_TEST */


#endif /* YEROTH_TEST_YEROTH_ERP_UTILS_WINDOWS_HPP_ */
