/*
 * yeroth-erp-achats-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_ERP_ACHATS_WINDOW_HPP_
#define YEROTH_ERP_ACHATS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-achats-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "src/windows/yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothAchatsWindow : public YerothWindowsCommons,
    					   private Ui_YerothAchatsWindow,
						   public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothAchatsWindow();

    virtual ~YerothAchatsWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_achatsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_achats_numero_page_derniere,
    												  label_achats_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:


	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_achats)


    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Sélectionner un achat dans le tableau des achats, ensuite cliquer "
                                    		   	   "sur l'opération que vous souhaitez réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &achatSqlTableModel);

    inline void lister_les_elements_du_tableau()
    {
        lister_les_elements_du_tableau(*_curAchatSqlTableModel);
    }

    void afficher_stock_selectioner(const QString &stockName);

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerothWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    void resetFilter(YerothSqlTableModel *achatsTableModel);

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private slots:

    void handle_ACTION_CHOICE_IMMOBILISATION_STOCK();

    void handle_RADIO_BUTTONS_immobilisations_STOCKS(bool is_toggle);

    void handle_filtres_checkBox(int state);

    inline void afficher_achats()
    {
        textChangedSearchLineEditsQCompleters();
    }

    bool filtrer_achats();

private:

    void initialiser_FILTRES();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    static unsigned int PDF_LISTING_COLUMN_STOCKS_ID;

    static unsigned int PDF_LISTING_COLUMN_REFERENCE;

    static unsigned int PDF_LISTING_COLUMN_DESIGNATION;

    static unsigned int PDF_LISTING_COLUMN_CATEGORIE;

    YerothLogger *_logger;

    QFont *_pushButton_achats_filtrer_font;

    bool _show_ASSET;

    QString _achatsDateFilter;

    YerothSqlTableModel *_curAchatSqlTableModel;
};

#endif /* YEROTH_ERP_ACHATS_WINDOW_HPP_ */
