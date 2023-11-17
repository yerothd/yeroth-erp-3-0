/*
 * yeroth-erp-comptabilite-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_
#define SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_

#include "../../ui_yeroth-erp-comptabilite-window.h"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;
class YerothUtils;
class YerothWindowsCommons;
class YerothPOSUser;

class YerothComptabiliteWindow : public YerothWindowsCommons,
    							 private Ui_YerothComptabiliteWindow,
								 public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothComptabiliteWindow();

    virtual ~YerothComptabiliteWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_comptabilite_Window;
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

    virtual inline void definirAdministrateur()
    {
        definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_comptabilite_numero_page_derniere,
    												  label_comptabilite_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_comptes_doperations_comptables)

	virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Vous avez accès aux informations sur tous les"
                                        " comptes bancaires enregistrés dans YEROTH-ERP-3.0,"
                                        " ainsi que toutes les transactions financières"
                                        " éffectuées sur ses comptes bancaires !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual void deconnecter_utilisateur();

    void afficher_comptes_bancaires(YerothSqlTableModel &
                                    aYerothSqlTableModel);

    inline void afficher_comptes_bancaires()
    {
        textChangedSearchLineEditsQCompleters();
    }


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void setupShortcuts();


protected slots:
    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


private slots:
    void handle_change_tab(int);

    void set_filtrer_font();

    void refineYerothLineEdits();


private:

    void updateComboBoxes();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();


    YerothLogger        *_logger;

    YerothSqlTableModel *_curCompte_Lignes_Budgetaires_SqlTableModel;

    QFont               *_pushButton_comptabilite_filtrer_font;
};

#endif /* SRC_YEROTH_COMPTES_BANCAIRES_WINDOW_HPP_ */
