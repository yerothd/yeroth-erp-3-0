/*
 * yeroth-erp-spell-numbers.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "src/utils/yeroth-erp-spell-numbers.hpp"


#ifdef YEROTH_FRANCAIS_LANGUAGE

const char *
YerothUtils_SPELL_NUMBERS::yeroth_small_numbers[] =
{
    "zéro", "un", "deux", "trois", "quatre", "cinq",
    "six", "sept", "huit", "neuf", "dix",
    "onze", "douze", "treize", "quatorze", "quinze",
    "seize", "dix sept", "dix huit", "dix neuf"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_decades[] =
{
    "", "", "vingt", "trente", "quarante",
    "cinquante", "soixante", "soixante dix", "quatre vingt", "quatre vingt dix"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_thousand_powers[] =
{
    " milliard(s)", " million(s)", " mille(s)", ""
};

#else

const char *
YerothUtils_SPELL_NUMBERS::yeroth_small_numbers[] =
{
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_decades[] =
{
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_thousand_powers[] =
{
    " billion(s)", " million(s)", " thousand(s)", ""
};

#endif


const char *
YerothUtils_SPELL_NUMBERS::yeroth_small_numbers_THREE_D_BARDIAGRAM[] =
{
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen",
    "sixteen", "seventeen", "eighteen", "nineteen"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_decades_THREE_D_BARDIAGRAM[] =
{
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char *
YerothUtils_SPELL_NUMBERS::yeroth_thousand_powers_THREE_D_BARDIAGRAM[] =
{
    " billion(s)", " million(s)", " thousand(s)", ""
};



QString YerothUtils_SPELL_NUMBERS::YEROTH__spell_numbers(unsigned long n)
{
    if (n < 20)
    {
        return QString(yeroth_small_numbers[n]);
    }

    QString res;

    const char **pScaleName = yeroth_thousand_powers;

    //1 MILLIARD
    unsigned long scaleFactor = 1000000000;

    while (scaleFactor > 0)
    {
        if (n >= scaleFactor)
        {
            unsigned long h = n / scaleFactor;

#ifdef YEROTH_FRANCAIS_LANGUAGE

            unsigned first_characters = (n / 100);

            if (h < 2)
            {
                res.append(*pScaleName);
            }
            else
            {
                res.append(yeroth_spell_hundreds(h) + *pScaleName);
            }

#else

            res.append(yeroth_spell_hundreds(h) + *pScaleName);

#endif

            n %= scaleFactor;

            if (0 != n)
            {
                res.append(" ");
            }
        }

        scaleFactor /= 1000;

        ++pScaleName;
    }

    return res;
}


QString YerothUtils_SPELL_NUMBERS::YEROTH__spell_numbers___ENGLISH_ONLY(unsigned
                                                                        long n)
{
    if (n < 20)
    {
        return QString(yeroth_small_numbers_THREE_D_BARDIAGRAM[n]);
    }

    QString res;

    const char **pScaleName = yeroth_thousand_powers_THREE_D_BARDIAGRAM;

    //1 MILLIARD
    unsigned long scaleFactor = 1000000000;

    while (scaleFactor > 0)
    {
        if (n >= scaleFactor)
        {
            unsigned long h = n / scaleFactor;

            res.append(yeroth_spell_hundreds___ENGLISH_ONLY(h) + *pScaleName);

            n %= scaleFactor;

            if (0 != n)
            {
                res.append(" ");
            }
        }

        scaleFactor /= 1000;

        ++pScaleName;
    }

    return res;
}


QString YerothUtils_SPELL_NUMBERS::yeroth_spell_hundreds(unsigned n)
{
    QString res;

    if (n > 99)
    {
        res = yeroth_small_numbers[n / 100];

#ifdef YEROTH_FRANCAIS_LANGUAGE

        unsigned first_characters = (n / 100);

        if (first_characters < 2)
        {
            res.clear();
        }

        res.append(" cent(s)");
#else
        res.append(" hundred(s)");
#endif

        n %= 100;

        if (n)
        {
#ifdef YEROTH_FRANCAIS_LANGUAGE
            res.append(" ");
#else
            res.append(" and ");
#endif
        }
    }

    if (n >= 20)
    {
        res.append(yeroth_decades[n / 10]);

        n %= 10;

        if (n)
        {
            res.append("-");
        }
    }

    if (0 < n && n < 20)
    {
        res.append(yeroth_small_numbers[n]);
    }

    return res;
}


QString YerothUtils_SPELL_NUMBERS::yeroth_spell_hundreds___ENGLISH_ONLY(unsigned n)
{
    QString res;

    if (n > 99)
    {
        res = yeroth_small_numbers_THREE_D_BARDIAGRAM[n / 100];

        res.append(" hundred(s)");

        n %= 100;

        if (n)
        {
            res.append(" and ");
        }
    }

    if (n >= 20)
    {
        res.append(yeroth_decades_THREE_D_BARDIAGRAM[n / 10]);

        n %= 10;

        if (n)
        {
            res.append("-");
        }
    }

    if (0 < n && n < 20)
    {
        res.append(yeroth_small_numbers_THREE_D_BARDIAGRAM[n]);
    }

    return res;
}
