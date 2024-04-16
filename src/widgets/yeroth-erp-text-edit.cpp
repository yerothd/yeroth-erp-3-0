/*
 * yeroth-erp-text-edit.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-text-edit.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtGui/QTextBlock>


bool YerothTextEdit::checkField()
{
    if (toPlainText().isEmpty())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerothTextEdit::setYerothPOSTextEditDisplayFormat()
{
    if (!isReadOnly())
    {
        setTextColor(YerothUtils::YEROTH_BLACK_COLOR);

        setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
    }
    else
    {
        setTextColor(YerothUtils::YEROTH_WHITE_COLOR);

        setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    }
}


void YerothTextEdit::setVisible(bool visible)
{
    QTextEdit::setVisible(visible);

    setYerothPOSTextEditDisplayFormat();
}


void YerothTextEdit::clearField()
{
    QTextEdit::clear();

    setPalette(_originalPaletteBeforeMissingInformation);

    setYerothPOSTextEditDisplayFormat();
}


void YerothTextEdit::setYerothEnabled(bool enabled)
{
    QTextEdit::setReadOnly(!enabled);

    setYerothPOSTextEditDisplayFormat();

    YerothUtils::yerothSetWidgetColor(this);
}


QString YerothTextEdit::toPlainTextForLatex() const
{
    QTextDocument *aTextDocument = document();

    if (0 == aTextDocument)
    {
        return YerothUtils::EMPTY_STRING;
    }

    QString result;

    QTextBlock it = aTextDocument->begin();

    while (it != aTextDocument->end())
    {
        result.append(QString("%1\\\\").arg(it.text()));

        it = it.next();
    }

    return YerothUtils::LATEX_IN_OUT_handleForeignAccents(result);
}
