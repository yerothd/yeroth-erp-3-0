/*
 * yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-details-dun-programme-de-fidelite-clients-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothDetailsDunProgrammeDeFideliteClientsWindow : public YerothWindowsCommons,
    												     private Ui_YerothDetailsDunProgrammeDeFideliteClientsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothDetailsDunProgrammeDeFideliteClientsWindow();

    inline ~YerothDetailsDunProgrammeDeFideliteClientsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerothDetailsDunProgrammeDeFideliteClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
        definirManager();
    }

    virtual inline void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
                               YerothSqlTableModel *stocksTableModel);

public slots:

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Vous pouvez modifier les détails de ce programme de fidélité clients "
                                        "en cliquant sur 'Modifier' !"));
    }

protected:

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showClientLOYALTY_PROGRAM_DETAIL();


    YerothLogger *_logger;

    YerothSqlTableModel *_curClient_LOYALTY_PROGRAM_TableModel;
};


#endif /* SRC_WINDOWS_YEROTH_CLIENTS_DETAILS_DUN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
