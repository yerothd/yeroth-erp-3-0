/*
 * yeroth-erp-ui-loader.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-ui-loader.hpp"

#include "src/widgets/yeroth-erp-push-button.hpp"

QWidget *YerothUiLoader::createWidget(const QString &className,
                                      QWidget *parent /* = 0 */,
                                      const QString &name /* = QString() */)
{
    if (className == "YerothPushButton")
    {
        YerothPushButton *w = new YerothPushButton(parent);
        w->setObjectName(name);
        return w;
    }

    return QUiLoader::createWidget(className, parent, name);
}
