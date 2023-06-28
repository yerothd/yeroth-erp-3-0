/*
 * yeroth-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE-hr_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE-hr_WINDOW_HPP_

#include "../../../ui_yeroth-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailsDunGroupeDePaieWindow : public YerothWindowsCommons,
    									   private Ui_YerothDetailsDunGroupeDePaieWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothDetailsDunGroupeDePaieWindow();

    inline ~YerothDetailsDunGroupeDePaieWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerothDetailsDunGroupeDePaieWindow;
    }

    inline virtual void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    inline virtual void definirVendeur()
    {
        definirManager();
    }

    inline virtual void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
                               YerothSqlTableModel *stocksTableModel);

public slots:

    inline virtual void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    inline virtual void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Vous pouvez modifier les détails de ce programme de fidélité clients "
                                    		   	   "en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showPayGroup_PROGRAM_DETAIL();


    YerothLogger *_logger;

    YerothSqlTableModel *_curPAY_GROUP_TableModel;
};


#endif /* SRC_WINDOWS_YEROTH_SUPPLIER_DETAILS_DUN_GROUPE_DE_PAIE-hr_WINDOW_HPP_ */
