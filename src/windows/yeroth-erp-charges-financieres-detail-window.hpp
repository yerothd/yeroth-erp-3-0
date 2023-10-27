/*
 * yeroth-erp-charges-financieres-detail-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_

#include "../../ui_yeroth-erp-charges-financieres-detail-window.h"

#include "src/utils/yeroth-erp-logger.hpp"

#include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class YerothChargesFinancieresDetailsWindow : public YerothWindowsCommons,
    										  private Ui_YerothChargesFinancieresDetailsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothChargesFinancieresDetailsWindow();

    inline ~YerothChargesFinancieresDetailsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_CHARGE_FINANCIERE_detailWindow;
    }

    virtual inline void definirCaissier()
    {
    	definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
    	definirPasDeRole();
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


    void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *achatStocksTableModel,
                       YerothSqlTableModel *stocksTableModel);


public slots:

    virtual bool imprimer_pdf_document();

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Vous pouvez retourner à la page précédente en "
                                        "cliquant sur 'Retour'!"));
    }

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

private:

    void setupLineEdits();

    void showItem();

    void checkCourrierAlerts();

    int _achatLastSelectedRow;

    YerothSqlTableModel *_cur_CHARGES_FINANCIERESTableModel;

    YerothLogger *_logger;
};


#endif /* SRC_WINDOWS_YEROTH_CHARGES_FINANCIERES_DETAIL_WINDOW_HPP_ */
