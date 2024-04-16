/*
 * yeroth-erp-spell-numbers.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_SPELL_NUMBERS_HPP_
#define SRC_YEROTH_SPELL_NUMBERS_HPP_


#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QObject>


class YerothUtils_SPELL_NUMBERS:public QObject
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS inline YerothUtils_SPELL_NUMBERS()
    {
    }

    inline ~YerothUtils_SPELL_NUMBERS()
    {
    }

    static QString YEROTH__spell_numbers(unsigned long n);

    static QString YEROTH__spell_numbers___ENGLISH_ONLY(unsigned long n);

private:

    static QString yeroth_spell_hundreds(unsigned n);

    static QString yeroth_spell_hundreds___ENGLISH_ONLY(unsigned n);


    static const char *yeroth_small_numbers[];

    static const char *yeroth_decades[];

    static const char *yeroth_thousand_powers[];


    static const char *yeroth_small_numbers_THREE_D_BARDIAGRAM[];

    static const char *yeroth_decades_THREE_D_BARDIAGRAM[];

    static const char *yeroth_thousand_powers_THREE_D_BARDIAGRAM[];
};


#endif /* SRC_YEROTH_SPELL_NUMBERS_HPP_ */
