/*
 * yeroth-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_
#define YEROTH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_

#include "../../../ui_yeroth-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailsDunGroupeDemployesWindow : public YerothWindowsCommons,
    										  private Ui_YerothDetailsDunGroupeDemployesWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothDetailsDunGroupeDemployesWindow();

    inline ~YerothDetailsDunGroupeDemployesWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_detailsDunGroupeDemployesWindow;
    }

    inline virtual void definirCaissier()
    {
    	definirPasDeRole();
    }

    virtual void definirManager();

    inline virtual void definirVendeur()
    {
    	definirPasDeRole();
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

    inline virtual void rendreInvisible()
    {
        YerothWindowsCommons::rendreInvisible();
    }

    virtual void rendreVisible(YerothSqlTableModel *aClientTableModel,
                               YerothSqlTableModel *aClientGroupTableModel,
                               YerothSqlTableModel *stocksTableModel,
                               const QString &clientGroup_db_ID = YerothUtils::EMPTY_STRING);

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
                               YerothSqlTableModel *stocksTableModel,
                               const QString &EMPLOYEE_Group_db_ID = YerothUtils::EMPTY_STRING);

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

private slots:

    void afficher_groupes_dun_EMPLOYE();

private:

    void setupLineEdits();

    void showEmployeeGroup_DETAIL(const QString &clientGroup_db_ID =
                                                YerothUtils::EMPTY_STRING);


    YerothLogger *_logger;

    YerothSqlTableModel *_curEmployeeTableModel;

    YerothSqlTableModel *_curEmployeeGroupTableModel;
};


#endif /* YEROTH_FOURNISSEURS_DETAILS_DUN_GROUPE_DEMPLOYES_HR_WINDOW_HPP_ */
