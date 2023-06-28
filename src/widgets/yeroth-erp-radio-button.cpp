/*
 * yeroth-erp-radio-button.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/yeroth-erp-radio-button.hpp"


void YerothRadioButton::setYerothEnabled(bool enabled)
{
    setVisible(enabled);
    setEnabled(enabled);
}
