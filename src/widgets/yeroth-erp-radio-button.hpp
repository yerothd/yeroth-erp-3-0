/*
 * yeroth-erp-radio-button.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_RADIO_BUTTON_HPP_
#define YEROTH_ERP_RADIO_BUTTON_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QRadioButton>

class QToolBar;
class YerothPOSAdminWindowsCommons;
class YerothWindowsCommons;

class YerothRadioButton:public QRadioButton
{

public:

    YEROTH_CLASS_OPERATORS inline YerothRadioButton(const QString &text, QWidget *parent = 0):QRadioButton(text,
                                                                                                               parent)
    {
    }

    inline YerothRadioButton(QWidget *parent = 0):QRadioButton(parent)
    {
    }

    inline ~YerothRadioButton()
    {
    }

    void setYerothEnabled(bool enabled);
};


#endif /* YEROTH_ERP_RADIO_BUTTON_HPP_ */
