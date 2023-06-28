/*
 * yeroth-erp-qcheckbox.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-qcheckbox.hpp"


#include <QtCore/QDebug>


YerothERPQCheckBox::YerothERPQCheckBox(QWidget *parent):QCheckBox(parent)
{
}


void YerothERPQCheckBox::setReadOnly(bool isReadOnly)
{
    if (isReadOnly)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setFocusPolicy(Qt::NoFocus);
    }
    else
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, false);
        setFocusPolicy(Qt::StrongFocus);
    }
}


void YerothERPQCheckBox::setYerothChecked(bool checked, const QString &text
                                          /*= YerothUtils::EMPTY_STRING*/)
{
    if (!YerothUtils::isEqualCaseInsensitive(text, YerothUtils::EMPTY_STRING))
    {
        setText(text);
    }

    setChecked(checked);
}


void YerothERPQCheckBox::setYerothEnabled(bool isEnabled)
{
    if (isEnabled)
    {
        setReadOnly(false);
        setEnabled(true);
    }
    else
    {
        setReadOnly(true);
        setEnabled(false);
    }
}
