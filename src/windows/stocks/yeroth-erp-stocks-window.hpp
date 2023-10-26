/*
 * yeroth-erp-stocks-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_STOCKS_WINDOW_HPP_
#define YEROTH_ERP_STOCKS_WINDOW_HPP_

#include "../../../ui_yeroth-erp-stocks-window.h"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothStocksWindow : public YerothWindowsCommons,
						   private Ui_YerothStocksWindow,
						   public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothStocksWindow();

    virtual ~YerothStocksWindow();

    inline virtual QToolBar *getQMainWindowToolBar()
    {
        return toolBar_stocksWindow;
    }

    inline virtual void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_stocks_numero_page_derniere,
    												  label_stocks_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel, bool show_ASSET);

    inline virtual void rendreVisible(YerothSqlTableModel *stocksTableModel)
    {
        rendreVisible(stocksTableModel, _show_ASSET);
    }

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_stocks)

    virtual void imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();

    virtual inline void imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR(int pageFROM,
                                                                         int pageTO)
    {
        YerothProgressBar ((YerothWindowsCommons *) this, QString("rgb(%1)").arg(COLOUR_RGB_STRING_YEROTH_GREEN_0_255_0))
                          ((YerothWindowsCommons *) this,
                            &pageFROM,
                            &pageTO,
                            &YerothWindowsCommons::imprimer_pdf_document_WITH_PAGES_SPECIFICATION);
    }

    void gererChoixStrategieGestionDesStocks();

    inline virtual void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    inline virtual void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("1. Les immobilisations sont des stocks avec des"
                                        " prix de vente égal à 0 (zéro) !\n\n"
                                        "2. Sélectionnez 1 IMMOBILISATION OU 1 stock dans le "
                                        " tableau des stocks, ensuite cliquer sur l'opération"
                                        " que vous souhaitez réaliser !\n\n"
                                        "3. La valeur d'inventaire d'un stock (VI) "
                                        "est calculée comme suit:\n"
                                        " [VI = prix_dachat * quantite_en_stock] !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual void entrer(enum entrer_VISIBLE_CHOICE choice_insert =
                                        set_choice_insert_STANDARD_INVENTORY_STOCK);

    void lister_les_elements_du_tableau(YerothSqlTableModel &sqlTableModel,
                                        QString localVisibleStrategy = YerothUtils::EMPTY_STRING);

    virtual void lister_les_elements_du_tableau(QString strategieGlobale = YerothUtils::EMPTY_STRING);

    virtual void modifier_les_articles();

    void afficher_au_detail();

    void afficher_au_detail(const QModelIndex &modelIndex);

    void EFFACER_MANUELLEMENT_LHISTORIQUE();

    void supprimer_PLUSIEURS_Stocks(YerothSqlTableModel &aStocksTableModel);

    void supprimer_ce_stock_DE_PLUSIEURS(QString aStockID);

    void supprimer_ce_stock(QString aStockID = YerothUtils::EMPTY_STRING,
                            bool _reEntrant = false);

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

    void initialiser_FILTRES();

private slots:

    void SWITCH_QACTION_TEXTS();

    void handle_filtres_checkBox(int state);

    void handle_ACTION_CHOICE_IMMOBILISATION_STOCK();

    void handle_combobox_CHOICE_IMMOBILISATION_STOCK(const QString &choice_immobilisation_stock);

    void private_slot_afficher_historique_du_stock();

    bool filtrer_stocks_en_alerte_de_stock();

    bool filtrer_stocks_perimes_seulement();

    inline void afficher_stocks()
    {
        textChangedSearchLineEditsQCompleters();
    }

    bool filtrer_stocks();

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

    void resetFilter();

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void activerComboBoxStrategieDeGestionDesStocks();

    void desactiverComboBoxStrategieDeGestionDesStocks();

    inline void setComboBoxStrategieDeStocks()
    {
        comboBox_strategie_de_stocks->setCurrentIndex
				(YerothUtils::getComboBoxDatabaseQueryValue(YerothERPConfig::salesStrategy,
															YerothUtils::_strategieindexToUserViewString));
    }

    void disableValeurDinventaire();

    void enableValeurDinventaire_ONLY_MANAGER();

    void populateComboBoxes_TYPE_DINVENTAIRE();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();


    enum TEXT_LANGUAGES
	{
    	ENGLISH = 0,
		FRENCH	= 1
	};


    static const QString _STOCKS_TEXT_STRING[2];

    static const QString _STOCKS_LABEL_TEXT_STRING[2];


    static const QString _IMMOBILISATION_TEXT_STRING[2];

    static const QString _IMMOBILISATION_LABEL_TEXT_STRING[2];



    bool _show_ASSET;


    QString _CURRENT_choice_immobilisation_stock;

    QString _localStrategy;

    YerothLogger *_logger;

    QStringList _historiqueStock;

    QString _stocksDateEntreeFilter;

    QFont *_pushButton_stocks_filtrer_font;

    QList<QAction *> _ALL_QACTIONS;
};

#endif /* YEROTH_ERP_STOCKS_WINDOW_HPP_ */
