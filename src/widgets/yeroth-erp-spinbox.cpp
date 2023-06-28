/*
 * yeroth-erp-spinbox.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-spinbox.hpp"


void YerothSpinBox::setYerothEnabled(bool enabled)
{
    _originalPaletteBeforeMissingInformation = palette();

    QDoubleSpinBox::setReadOnly(!enabled);

    if (!isReadOnly())
    {
        setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        setBackgroundRole(QPalette::WindowText);
        setForegroundRole(QPalette::Window);
    }
    else
    {
        setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
        setBackgroundRole(QPalette::Window);
        setForegroundRole(QPalette::WindowText);
    }
}


bool YerothSpinBox::checkField()
{
    if (0 == value())
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);

        return false;
    }

    setPalette(_originalPaletteBeforeMissingInformation);

    return true;
}


void YerothSpinBox::clearField()
{
    clear();

    setPalette(_originalPaletteBeforeMissingInformation);
}


void YerothSpinBox::displayValue(double intValue)
{
    setValue(intValue);
    setYerothEnabled(false);
}
