/*
 * yeroth-erp-dialogs-commons.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_ERP_DIALOGS_COMMONS_HPP_
#define YEROTH_ERP_DIALOGS_COMMONS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtWidgets/QDialog>

class YerothERPWindows;

class YerothPOSDialogCommons : public QDialog
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

    inline YerothPOSDialogCommons(QString dialogName)
    :_dialogName(dialogName)
    {
    }

    inline ~YerothPOSDialogCommons()
    {
    }

    void showAsModalDialogWithParent(QWidget &parent);

    virtual inline void show()
    {
        QDialog::show();
    }

    virtual inline void rendreInvisible()
    {
        close();
    }

    inline QString getDialogName()
    {
        return _dialogName;
    }

    /**
     * This is "pure virtual method".
     *
     * It must be implemented by all
     * sub classes.
     */
    virtual QPoint *getCurrentPosition() = 0;

    inline static void setAllWindows(YerothERPWindows *allWindows)
    {
        _allWindows = allWindows;
    }

    inline static YerothERPWindows *getAllWindows()
    {
        return _allWindows;
    }

protected:

    virtual void closeEvent(QCloseEvent *closeEvent);

    static YerothERPWindows *_allWindows;

    const QString _dialogName;
};

#endif /* YEROTH_ERP_DIALOGS_COMMONS_HPP_ */
