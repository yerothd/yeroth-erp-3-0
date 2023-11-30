/*
 * yeroth-erp-paiements-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_
#define SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_


#include "../../ui_yeroth-erp-paiements-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class QProcess;

class YerothPaiementsWindow : public YerothWindowsCommons,
    					      private Ui_YerothPaiementsWindow,
							  public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

	enum TabIndex
    {
        TableauDesPaiements = 0,
        AfficherPaiementAuDetail
    };

public:

    YEROTH_CLASS_OPERATORS

	YerothPaiementsWindow();

    ~YerothPaiementsWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_historiquePaiementsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual inline void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }


    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    void lister_les_elements_du_tableau(YerothSqlTableModel &
                                        paiementsTableModel);

    inline void lister_les_elements_du_tableau()
    {
        lister_les_elements_du_tableau(*_curPaiementsTableModel);
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_paiements_numero_page_derniere,
    												  label_paiements_numero_page_courante)

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);


public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_paiements)

	virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Vous avez accès aux informations sur les paiements"
                                        " en utilisant le nom de l'entreprise. Ceci se fait"
                                        " dans le 1er champ de recherche de cette l'interface"
                                        " graphique !\n\n"
                                        "2) Vous pouvez filtrer les résultats de la recherche,"
                                        " sur les informations de paiements, en paramétrant"
                                        " celle-ci avec des dates de 'Début', et de 'Fin'.\n\n"
                                        " Vous pouvez filtrer les résultats de la recherche,"
                                        " en utilisant les paramètres que l'on retrouve"
                                        " tout en bas de l'interface graphique"
                                        " (exemple: numéro du bon de paiement, etc.) !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    virtual bool imprimer_pdf_document();

    void handleCurrentChanged(int index);

    void retourPaiements();

    virtual void deconnecter_utilisateur();

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void afficher_paiements_detail();

    inline void afficher_paiements()
    {
        textChangedSearchLineEditsQCompleters();
    }

    void resetFilter(YerothSqlTableModel *historiquePaiementsTableModel);


protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();


protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void resizeEvent(QResizeEvent *event);

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerothWindowsCommons::
        CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    virtual void set_filtrer_font();


private slots:

    void handle_filtres_checkBox(int state);

    void handleComboBoxClients_Typedepaiement_TextChanged(const QString &
                                                          currentText);

    void handle_ACTION_CHOICE_TYPE_DENTREPRISE();

    void handle_combobox_type_dentreprise(const QString &text);

    bool filtrer_paiements();


private:

    void initialiser_FILTRES();

    void prepare__IN__for_export_and_printing();

    void prepare__OUT__for_export_and_printing();

    void update_clients_specific_payments_type();

    void update_suppliers_specific_payments_type();

    const QString &get_current_table_column_for_company_type_to_HIDE();

    void populateComboBoxes();

    void setupLineEdits();

    void clear_all_fields();

    void setupDateTimeEdits();


    static const QString CLIENT_TEXT_STRING;

    static const QString FOURNISSEUR_TEXT_STRING_ENGLISH;

    static const QString FOURNISSEUR_TEXT_STRING_FRENCH;


    YerothLogger *_logger;

    QMap<QString, uint> _client_fournisseur_visible_string_EXPORT_AND_PRINT_PDF__TO__visible_index;

    int _currentTabView;

    QFont *_pushButton_paiements_filtrer_font;

    QString _curSupplierText;

    QString _paiementsDateFilter;

    YerothSqlTableModel *_curPaiementsTableModel;
};


#endif /* SRC_YEROTH_HISTORIQUE_PAIEMENTS_WINDOW_HPP_ */
