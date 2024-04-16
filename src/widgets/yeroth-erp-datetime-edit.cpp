/*
 * yeroth-erp-datetime-edit.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-datetime-edit.hpp"

#include "src/yeroth-erp-windows.hpp"
#include "src/utils/yeroth-erp-logger.hpp"

#include <QtCore/QPoint>

YerothDateTimeEdit::YerothDateTimeEdit(QWidget *parent)
:QDateTimeEdit(parent),
 _logger(new YerothLogger("YerothDateTimeEdit")),
 _calendarWidget(new QCalendarWidget)
{
    _calendarWidget->setMinimumDate(QDate(1900, 1, 1));

    _calendarWidget->setMaximumDate(QDate(70000, 1, 1));

    _calendarWidget->setGridVisible(true);


    setCalendarPopup(true);


    setCalendarWidget(_calendarWidget);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	setLocale(YerothUtils::englishLocale);
    }
    else
    {
    	setLocale(YerothUtils::frenchLocale);
    }


    setYerothERPDateTimeEditDisplayFormat();
}


void YerothDateTimeEdit::setYerothEnabled(bool enabled)
{
    QDateTimeEdit::setReadOnly(!enabled);

    setYerothERPDateTimeEditDisplayFormat();
}


void YerothDateTimeEdit::setYerothERPDateTimeEditDisplayFormat()
{
    if (isReadOnly())
    {
        setAlignment(Qt::AlignCenter);

        setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    }
    else
    {
        setAlignment(Qt::AlignLeft);

        setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    }
}


void YerothDateTimeEdit::setStartDate(QDate startDate)
{
    _startDate = startDate;

    setDate(_startDate);
}
