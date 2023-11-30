/*
 * yeroth-erp-marchandises-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_ERP_MARCHANDISES_WINDOW_HPP_
#define YEROTH_ERP_MARCHANDISES_WINDOW_HPP_

#include "../../../ui_yeroth-erp-marchandises-window.h"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


#include "src/windows/yeroth-erp-window-commons.hpp"


class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;


class YerothMarchandisesWindow : public YerothWindowsCommons,
    							 private Ui_YerothERPMarchandisesWindow,
								 public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothMarchandisesWindow();

    virtual ~YerothMarchandisesWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_marchandisesWindow;
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

    inline bool IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS()
    {
        return !_current_filtering_non_empty_stock_SQL_QUERY.isEmpty();
    }

    inline void setCheckBoxServices(bool checked)
    {
        checkBox_services->setChecked(checked);
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_marchandises_numero_page_derniere,
    												  label_marchandises_numero_page_courante)

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);


public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_marchandises)

	virtual inline void set_quantite_totale_label_string(double a_view_string_double)
    {
        lineEdit_nombre_darticles->setText(QString::number(a_view_string_double));
    }

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un marchandise dans le tableau "
                                        "de marchandises, ensuite cliquer "
                                        "sur l'opération que vous souhaitez réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual bool imprimer_pdf_document();

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                aYerothSqlTableModel);

    inline void afficherMarchandises()
    {
        textChangedSearchLineEditsQCompleters();
    }

    void afficher_stock_selectioner(const QString &stockName);

    void modifier_marchandise();

    void supprimer_PLUSIEURS_Marchandises(YerothSqlTableModel &
                                          aMarchandisesTableModel);

    void supprimer_cette_marchandise_DE_PLUSIEURS(QString aMarchandiseID);

    void supprimer_cette_marchandise(QString aMarchandiseID =
                                                     YerothUtils::EMPTY_STRING,
                                     bool _reEntrant = false);

    void reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    inline YerothLogger *getLogger()
    {
        return _logger;
    }

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();

protected slots:
    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void initialiser_FILTRES();

private slots:
    void handle_filtres_checkBox(int state);

    virtual void entrer_un_stock();

    inline void handle_ACTION_CHOICE_SERVICES_MARCHANDISES()
    {
        checkBox_services->setChecked(!checkBox_services->isChecked());
    }

    void handle_services_checkBox(int state);

    bool slot_filtrer_non_empty_product_stock__AVANT_MARCHANDISES_TERMINEES();

    bool slot_filtrer_non_empty_product_stock();

    inline void slot_filtrer_non_empty_product_stock__WITH_PROGRESS_BAR()
    {
        YerothProgressBar(this)(this,
                                &YerothMarchandisesWindow::
                                slot_filtrer_non_empty_product_stock);
    }

    bool slot_filter_empty_product_stock();

    inline void slot_filter_empty_product_stock__WITH_PROGRESS_BAR()
    {
        YerothProgressBar(this)(this,
                                &YerothMarchandisesWindow::
                                slot_filter_empty_product_stock);
    }

    bool slot_filtrer();

public:

    double _qteTotaleDarticlesEnStock;

private:

    void localSetupLineEditsQCompleters();

    void set_filtrer_font();

    void populateMarchandisesComboBoxes();

    void setupLineEdits();


    YerothLogger *_logger;

    bool _NON_TERMINEES_MARCHANDISE_RE_ENTRANT;

    bool _currentlyFiltered;

    int _lastSelectedRow__ID;

    QString _current_filtering_non_empty_stock_SQL_QUERY;

    QFont *_pushButton_filtrer_font;

    YerothSqlTableModel *_curMarchandisesTableModel;
};

#endif /* YEROTH_ERP_MARCHANDISES_WINDOW_HPP_ */
