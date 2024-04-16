/*
 * yeroth-erp-ventes-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_CAISSE_WINDOW_HPP_
#define SRC_YEROTH_CAISSE_WINDOW_HPP_


#include "../../ui_yeroth-erp-ventes-window.h"


#include "src/utils/yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;


class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;

class QProcess;

class YerothVentesWindow : public YerothWindowsCommons,
    					   private Ui_YerothVentesWindow,
						   public YerothAbstractClassYerothSearchWindow
{
    Q_OBJECT

	enum TabIndex
    {
        TableauDesVentes = 0,
        AfficherVenteAuDetail,
        RetourDuneVente
    };

public:

    YEROTH_CLASS_OPERATORS

	YerothVentesWindow();

    ~YerothVentesWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_caisseWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                stocksVenduTableModel);

    inline void lister_les_elements_du_tableau()
    {
        lister_les_elements_du_tableau(*_curStocksVenduTableModel);
    }

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_ventes_numero_page_derniere,
    												  label_ventes_numero_page_courante)

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_ventes)

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Vous avez accès aux informations des ventes"
                                        " en utilisant la référence d'un article. Ceci se fait"
                                        " dans le 1er champ de recherche de cette l'interface"
                                        " graphique !\n\n"
                                        "2) Vous pouvez filtrer les résultats de la recherche,"
                                        " sur les informations de ventes, en paramétrant"
                                        " celle-ci avec des dates de 'Début', et de 'Fin'.\n\n"
                                        " Vous pouvez filtrer les résultats de la recherche,"
                                        " en utilisant les paramètres que l'on retrouve"
                                        " tout en bas de l'interface graphique"
                                        " (exemple: nom du caissier, désignation de l'article, etc.) !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void handleTabEnabled();

    void handleCurrentTabChanged(int index);

    inline void retourVentes()
    {
        tabWidget_ventes->setCurrentIndex(TableauDesVentes);
    }

    virtual void deconnecter_utilisateur();

    void refineYerothLineEdits();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    bool afficher_retour_vente();

    bool afficher_vente_detail();

    void resetFilter(YerothSqlTableModel *stocksVenduTableModel);

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void resizeEvent(QResizeEvent *event);

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerothWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    virtual void set_filtrer_font();

private:

    void initialiser_FILTRES();

private slots:

    void private_slot_afficher_historique_du_stock();

    void handle_filtres_checkBox(int state);

    void slot_regenerer_recu_client_vente();

    void afficher_ventes();

    void ENABLE_AfficherAnnulerCetteVente(bool enable);

    void modifier_visibilite_actions_sur_cette_vente();

    bool annuler_cette_vente();

    bool filtrer_ventes();

private:

    void setCurrentUser_NOM_CAISSIER(const QString &aUserNomComplet);

    void disableNomCaissier();

    void enableNomCaissier_ONLY_MANAGER();

    bool handleCompteClient(QString &client_id_IN_OUT,
                            int type_de_vente,
                            double curMontantARembourserAuClient);

    void populateComboBoxes();

    void setupLineEdits();

    void clear_all_fields();

    void setupDateTimeEdits();

    inline void setYerothVentesFilter()
    {
        _curStocksVenduTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);
    }


    QString _retourVenteTabWidgetTitle;

    QWidget *_retourVenteTabWidget;

    YerothLogger *_logger;

    QFont *_pushButton_ventes_filtrer_font;

    QString _ventesDateFilter;

    YerothSqlTableModel *_curStocksVenduTableModel;
};


#endif /* SRC_YEROTH_CAISSE_WINDOW_HPP_ */
