/*
 * yeroth-erp-style.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */
#include "src/utils/yeroth-erp-style.hpp"

#include <QtCore/QString>
#include <QtGui/QColor>

QString YerothPosStyle::getColorStyleSheetString(const QColor &aColour)
{
    return QString("QLabel { color : %1; }").arg(aColour.name());
}
