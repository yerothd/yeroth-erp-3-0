/*
 * yeroth-erp-ui-loader.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_UI_LOADER_HPP_
#define SRC_YEROTH_UI_LOADER_HPP_

#include <QtUiTools/QUiLoader>

class YerothUiLoader:public QUiLoader
{

public:

    inline YerothUiLoader(QObject *parent = 0)
    {
    }

    virtual inline ~YerothUiLoader()
    {
    }

    virtual QWidget *createWidget(const QString &className,
                                  QWidget *parent = 0,
                                  const QString &name = QString());

};

#endif /* SRC_YEROTH_UI_LOADER_HPP_ */
