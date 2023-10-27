/*
 * yeroth-erp-tableaux-de-bord-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_BUSINESS_REPORTS_WINDOW_HPP_
#define YEROTH_BUSINESS_REPORTS_WINDOW_HPP_

#include "../../ui_yeroth-erp-tableaux-de-bord-window.h"


#include "yeroth-erp-window-commons.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"


class Yeroth_MAP_COMPLEX_Item;

class QProcess;

class YerothUtils;
class YerothLogger;
class YerothSqlTableModel;
class YerothERPWindows;

class YerothTableauxDeBordWindow : public YerothWindowsCommons,
    							   private Ui_YerothRapportsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothTableauxDeBordWindow();

    virtual inline ~YerothTableauxDeBordWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_rapportsWindow;
    }


    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();


    void setupTab_EVOLUTION_DU_CHIFFRE_DAFFAIRE();

    void setupTab_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    void setupTab_BILAN_COMPTABLE();


    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);


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
                                       ("Sélectionner les critères commerciaux que vous désirez,"
                                        " ensuite cliquez sur le boutton 'générer le fichier PDF'"
                                        " pour obtenir le fichier PDF correspondant !"));
    }

    void reinitialiser();

    void reinitialiser_chiffre_affaire();

    void reinitialiser_bilan_comptable();

    inline void setFilter()
    {
        _curStocksVenduTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);
    }

    virtual inline void PRINT_with_Progress_BAR_generer()
    {
        YerothProgressBar(this, "rgb(255, 255, 255)")
                         (this, &YerothTableauxDeBordWindow::generer);
    }

    virtual void *generer();

    void quantite_max_benefices(QString fileName,
                                QString fieldId,
                                int size);

    void quantite_moindre_benefices(QString fileName,
                                    QString fieldId,
                                    int size);

    void quantite_max_stats(QString fileName,
                            QString fieldId,
                            int size,
                            bool service = false);

    void quantite_moindre_stats(QString fileName,
                                QString fieldId,
                                int size,
                                bool service = false);

    void meilleursStats(QString fileName,
                        QString fieldId,
                        int size,
                        bool service = false);

    //void ZERO_stats(QString fileName, QString fieldId);

    void ZERO_stats_stocks(QString fileName,
                           QString fieldId);

    void derniersStats(QString fileName,
                       QString fieldId,
                       int size,
                       bool service = false);


    //********************************** statsBENEFICESMax *********************************
    inline void statsBENEFICESMaxClients(QString fileName, int size)
    {
        quantite_max_benefices(fileName,
                               YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                               size);
    }

    inline void statsBENEFICESMaxFournisseursVentes(QString fileName, int size)
    {
        quantite_max_benefices(fileName,
                               YerothDatabaseTableColumn::
                               NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsBENEFICESMaxCaissiers(QString fileName, int size)
    {
        quantite_max_benefices(fileName, YerothDatabaseTableColumn::NOM_CAISSIER,
                               size);
    }

    inline void statsBENEFICESMaxCategories(QString fileName, int size)
    {
        quantite_max_benefices(fileName, YerothDatabaseTableColumn::CATEGORIE,
                               size);
    }

    inline void statsBENEFICESMaxArticles(QString fileName, int size)
    {
        quantite_max_benefices(fileName, YerothDatabaseTableColumn::DESIGNATION,
                               size);
    }

    inline void statsBENEFICESMaxQuartiers(QString fileName, int size)
    {
        quantite_max_benefices(fileName,
                               YerothDatabaseTableColumn::
                               QUARTIER_ENTREPRISE_CLIENT, size);
    }

    inline void statsBENEFICESMaxVilles(QString fileName, int size)
    {
        quantite_max_benefices(fileName,
                               YerothDatabaseTableColumn::
                               VILLE_ENTREPRISE_CLIENT, size);
    }

    inline void statsBENEFICESMaxRegionsEtats(QString fileName, int size)
    {
        quantite_max_benefices(fileName,
                               YerothDatabaseTableColumn::
                               REGION_ETAT_ENTREPRISE_CLIENT, size);
    }


    //********************************** statsBENEFICESMoindre *********************************
    inline void statsBENEFICESMoindreClients(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::
                                   NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsBENEFICESMoindreFournisseursVentes(QString fileName,
                                                        int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::
                                   NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsBENEFICESMoindreCaissiers(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::NOM_CAISSIER,
                                   size);
    }

    inline void statsBENEFICESMoindreCategories(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::CATEGORIE, size);
    }

    inline void statsBENEFICESMoindreArticles(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsBENEFICESMoindreQuartiers(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::
                                   QUARTIER_ENTREPRISE_CLIENT, size);
    }

    inline void statsBENEFICESMoindreVilles(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::
                                   VILLE_ENTREPRISE_CLIENT, size);
    }

    inline void statsBENEFICESMoindreRegionsEtats(QString fileName, int size)
    {
        quantite_moindre_benefices(fileName,
                                   YerothDatabaseTableColumn::
                                   REGION_ETAT_ENTREPRISE_CLIENT, size);
    }


    //********************************** statsQuantiteMax *********************************
    inline void statsQuantiteMaxServices(QString fileName, int size)
    {
        quantite_max_stats(fileName, YerothDatabaseTableColumn::DESIGNATION,
                           size, true);
    }

    inline void statsQuantiteMaxClients(QString fileName, int size,
                                        bool service = false)
    {
        quantite_max_stats(fileName,
                           YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                           size);
    }

    inline void statsQuantiteMaxFournisseursVentes(QString fileName, int size,
                                                   bool service = false)
    {
        quantite_max_stats(fileName,
                           YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                           size);
    }

    inline void statsQuantiteMaxCaissiers(QString fileName, int size,
                                          bool service = false)
    {
        quantite_max_stats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER,
                           size);
    }

    inline void statsQuantiteMaxCategories(QString fileName, int size,
                                           bool service = false)
    {
        quantite_max_stats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

    inline void statsQuantiteMaxArticles(QString fileName, int size,
                                         bool service = false)
    {
        quantite_max_stats(fileName, YerothDatabaseTableColumn::DESIGNATION,
                           size);
    }

    inline void statsQuantiteMaxVilles(QString fileName, int size,
                                       bool service = false)
    {
        quantite_max_stats(fileName,
                           YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
                           size);
    }

    inline void statsQuantiteMaxQuartiers(QString fileName, int size,
                                          bool service = false)
    {
        quantite_max_stats(fileName,
                           YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                           size);
    }

    inline void statsQuantiteMaxRegionsEtats(QString fileName, int size,
                                             bool service = false)
    {
        quantite_max_stats(fileName,
                           YerothDatabaseTableColumn::
                           REGION_ETAT_ENTREPRISE_CLIENT, size);
    }


    //********************************** statsQuantiteMoindre *********************************
    inline void statsQuantiteMoindreServices(QString fileName, int size)
    {
        quantite_moindre_stats(fileName, YerothDatabaseTableColumn::DESIGNATION,
                               size, true);
    }

    inline void statsQuantiteMoindreClients(QString fileName, int size,
                                            bool service = false)
    {
        quantite_moindre_stats(fileName,
                               YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                               size);
    }

    inline void statsQuantiteMoindreFournisseursVentes(QString fileName,
                                                       int size, bool service =
                                                                       false)
    {
        quantite_moindre_stats(fileName,
                               YerothDatabaseTableColumn::
                               NOM_ENTREPRISE_FOURNISSEUR, size);
    }

    inline void statsQuantiteMoindreCaissiers(QString fileName, int size,
                                              bool service = false)
    {
        quantite_moindre_stats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER,
                               size);
    }

    inline void statsQuantiteMoindreCategories(QString fileName, int size,
                                               bool service = false)
    {
        quantite_moindre_stats(fileName, YerothDatabaseTableColumn::CATEGORIE,
                               size);
    }

    inline void statsQuantiteMoindreArticles(QString fileName, int size,
                                             bool service = false)
    {
        quantite_moindre_stats(fileName, YerothDatabaseTableColumn::DESIGNATION,
                               size);
    }

    inline void statsQuantiteMoindreVilles(QString fileName, int size,
                                           bool service = false)
    {
        quantite_moindre_stats(fileName,
                               YerothDatabaseTableColumn::
                               VILLE_ENTREPRISE_CLIENT, size);
    }

    inline void statsQuantiteMoindreQuartiers(QString fileName, int size,
                                              bool service = false)
    {
        quantite_moindre_stats(fileName,
                               YerothDatabaseTableColumn::
                               QUARTIER_ENTREPRISE_CLIENT, size);
    }

    inline void statsQuantiteMoindreRegionsEtats(QString fileName, int size,
                                                 bool service = false)
    {
        quantite_moindre_stats(fileName,
                               YerothDatabaseTableColumn::
                               REGION_ETAT_ENTREPRISE_CLIENT, size);
    }



    //********************************** statsMeilleurs *********************************
    inline void statsMeilleurs_QUARTIER(QString fileName, int size,
                                        bool service = false)
    {
        meilleursStats(fileName,
                       YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                       size);
    }

    inline void statsMeilleurs_VILLES(QString fileName, int size,
                                      bool service = false)
    {
        meilleursStats(fileName,
                       YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT, size);
    }

    inline void statsMeilleurs_REGIONS_ETATS(QString fileName, int size)
    {
        meilleursStats(fileName,
                       YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT,
                       size);
    }

    inline void statsMeilleursClients(QString fileName, int size,
                                      bool service = false)
    {
        meilleursStats(fileName,
                       YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT, size);
    }

    inline void statsMeilleursFournisseursVentes(QString fileName, int size)
    {
        meilleursStats(fileName,
                       YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                       size);
    }

    inline void statsMeilleursCaissiers(QString fileName, int size)
    {
        meilleursStats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsMeilleursServices(QString fileName, int size)
    {
        meilleursStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size,
                       true);
    }

    inline void statsMeilleursArticles(QString fileName, int size)
    {
        meilleursStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsMeilleursCategories(QString fileName, int size)
    {
        meilleursStats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }


    //********************************** ZERO_stats_stocks *********************************
    inline void statsZERO_QUARTIER(QString fileName)
    {
        ZERO_stats_stocks(fileName,
                          YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT);
    }

    inline void statsZERO_VILLES(QString fileName)
    {
        ZERO_stats_stocks(fileName,
                          YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT);
    }

    inline void statsZERO_REGIONS_ETATS(QString fileName)
    {
        ZERO_stats_stocks(fileName,
                          YerothDatabaseTableColumn::
                          REGION_ETAT_ENTREPRISE_CLIENT);
    }

    inline void statsZERO_Clients(QString fileName)
    {
        ZERO_stats_stocks(fileName,
                          YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
    }

    inline void statsZERO_FournisseursVentes(QString fileName)
    {
        ZERO_stats_stocks(fileName,
                          YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR);
    }

    inline void statsZERO_Caissiers(QString fileName)
    {
        ZERO_stats_stocks(fileName, YerothDatabaseTableColumn::NOM_CAISSIER);
    }

    inline void statsZERO_Articles(QString fileName)
    {
        ZERO_stats_stocks(fileName, YerothDatabaseTableColumn::DESIGNATION);
    }

    inline void statsZERO_Categories(QString fileName)
    {
        ZERO_stats_stocks(fileName, YerothDatabaseTableColumn::CATEGORIE);
    }


    //********************************** statsDerniers *********************************
    inline void statsDerniers_QUARTIERS(QString fileName, int size)
    {
        derniersStats(fileName,
                      YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                      size);
    }

    inline void statsDerniers_VILLES(QString fileName, int size)
    {
        derniersStats(fileName,
                      YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT, size);
    }

    inline void statsDerniersREGIONS_ETATS(QString fileName, int size)
    {
        derniersStats(fileName,
                      YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT,
                      size);
    }

    inline void statsDerniersClients(QString fileName, int size)
    {
        derniersStats(fileName, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                      size);
    }

    inline void statsDerniersFournisseursVentes(QString fileName, int size)
    {
        derniersStats(fileName,
                      YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                      size);
    }

    inline void statsDerniersCaissiers(QString fileName, int size)
    {
        derniersStats(fileName, YerothDatabaseTableColumn::NOM_CAISSIER, size);
    }

    inline void statsDerniersServices(QString fileName, int size)
    {
        derniersStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size,
                      true);
    }

    inline void statsDerniersArticles(QString fileName, int size)
    {
        derniersStats(fileName, YerothDatabaseTableColumn::DESIGNATION, size);
    }

    inline void statsDerniersCategories(QString fileName, int size)
    {
        derniersStats(fileName, YerothDatabaseTableColumn::CATEGORIE, size);
    }

    void rechercher();

    virtual bool export_csv_file();

    void remove_BAR_PIE_CHART_OPTION_FOR_ZERO_BUSINESS_TURNOVER(const QString &
                                                                comboBoxQualiteCurrentText);

    void changeLineEditEvolutionObjetsTextSetup(const QString &
                                                comboBoxEvolutionObjetsCurrentText);


    virtual inline void PRINT_with_Progress_BAR_bilanComptable()
    {
        YerothProgressBar(this, "rgb(255, 255, 255)")
                         (this, &YerothTableauxDeBordWindow::bilanComptable);
    }

    virtual void *bilanComptable();

    void analyse_comparee_jour_semaine_VENTES_BENEFICES();


    void GEN_THREE_DIMENSION_analyse_comparee_jour_semaine_ACHATS_VENTES(QString
                                                                         yerothFiltre,
                                                                         const
                                                                         QString &
                                                                         textFromLineEditEvolutionSujets,
                                                                         const
                                                                         QString &
                                                                         comboBoxEvolutionObjetsCurrentText);

    void analyse_comparee_jour_semaine_ACHATS_VENTES();


    void analyse_comparee_mensuelle_VENTES_BENEFICES();

    void analyse_comparee_mensuelle_ACHATS_VENTES();

    void analyse_comparee_jour_semaine();

    void analyse_comparee_mensuelle();

    void compterLesArticlesVendusParBENEFICES();

    void compterLesArticlesVendusParQuantite();

    void handle_enabled_chiffre_daffaire_jour_semaine(bool enabled);

    void handle_enabled_chiffre_daffaire_mois(bool enabled);

    void calculer_chiffre_daffaire_jour_semaine();

    void calculer_chiffre_daffaire_mois();


    virtual inline void PRINT_with_Progress_BAR_choisirEvolutionDuChiffreDaffaire()
    {
        YerothProgressBar(this, "rgb(255, 255, 255)")
                         (this, &YerothTableauxDeBordWindow::choisirEvolutionDuChiffreDaffaire);
    }

    virtual void *choisirEvolutionDuChiffreDaffaire();


protected:

    virtual void setupShortcuts();


private slots:

    bool check_fields_FINANCIAL_ACCOUNTING_REPORT();

    bool check_fields_BUSINESS_TURNOVER_COMPARISON();

    bool check_fields_BUSINESS_TURNOVER_PROGRESS();

    inline void handle_comboBox_bilan_comptable_SUCCURSALE_current_text(const
                                                                        QString
                                                                        &
                                                                        current_text)
    {
        _current_succursale_STRING = current_text;
    }

    inline void handle_comboBox_operations_SUCCURSALE_current_text(const
                                                                   QString &
                                                                   current_text)
    {
        _current_succursale_STRING = current_text;
    }

    inline void handle_comboBox_operations_chiffre_SUCCURSALE_current_text(const QString
                                                                           &
                                                                           current_text)
    {
        _current_succursale_STRING = current_text;
    }

    void handle_checkBox_analyse_comparee(int);

    void handle_tab_business_turnover_progress_radio_button(bool);

    void handleTabChanged(int index);


private:

    void populateComboBoxes();

    class StockQteVendueEtRemiseTotalSurVente
    {
    public:
        inline StockQteVendueEtRemiseTotalSurVente():
               _qteVendue(0),
               _remiseTotalSurVentes(0)
        {
        }

        inline StockQteVendueEtRemiseTotalSurVente(double qteVendue,
                                                   double
                                                   remiseTotalSurVentes):_qteVendue
            (qteVendue), _remiseTotalSurVentes(remiseTotalSurVentes)
        {
        }

        double _qteVendue;
        double _remiseTotalSurVentes;
    };

    enum TabIndexes
    {
        SUJET_ACTION_FINANCIAL_ACCOUNTING_REPORT_GENERATION = 0,
        SUJET_ACTION_BUSINESS_TURNOVER_COMPARISON = 1,
        SUJET_ACTION_BUSINESS_TURNOVER_PROGRESS = 2
    };

    static QString GET_YEROTH_DTL_SET_PIE_SEGMENT_COLOR_STRING(uint position);

    static QString GET_YEROTH_DTL_SET_PIE_SEGMENT_COLOR_STRING_for_LATEX(uint size);


    static QString GET_YEROTH_DTL_SET_BAR_COLOR_STRING(uint position);

    static QString GET_YEROTH_DTL_SET_BAR_COLOR_STRING_for_LATEX(uint size);


    void setupDateTimeEdits_EVOLUTION_DES_CHIFFRES_DAFFAIRES();

    void setupDateTimeEdits_COMPARAISON_DES_CHIFFRES_DAFFAIRES();

    void setupDateTimeEdits_BILAN_COMPTABLE();



    static QString ANALYSE_COMPAREE_VENTES_BENEFICES;
    static QString ANALYSE_COMPAREE_ACHATS_VENTES;


    static QString OPERATION_GENERER;
    static QString OPERATION_GENERER_CHIFFRE_DAFFAIRE;
    static QString OPERATION_GENERER_BILAN_COMPTABLE;


    static const QString YEROTH_DTL_SET_BAR_PIE_COLOR_ARRAY[12];


    static const unsigned int QUANTITE_9;

    static QString MOIS_1;
    static QString MOIS_2;
    static QString MOIS_3;
    static QString MOIS_4;
    static QString MOIS_5;
    static QString MOIS_6;
    static QString MOIS_7;
    static QString MOIS_8;
    static QString MOIS_9;
    static QString MOIS_10;
    static QString MOIS_11;
    static QString MOIS_12;

    static QString QUALITE_PLUS_VENDU_PAR_QUANTITE;
    static QString QUALITE_MOINS_VENDU_PAR_QUANTITE;
    static QString QUALITE_MEILLEURS;
    static QString QUALITE_BENEFICES_PLUS_ELEVES;
    static QString QUALITE_BENEFICES_MOINS_ELEVES;
    static QString QUALITE_ZERO;
    static QString QUALITE_DERNIERS;

    static QString OBJET_SERVICES;
    static QString OBJET_ARTICLES;
    static QString OBJET_CATEGORIES;
    static QString OBJET_CAISSIERS;
    static QString OBJET_CLIENTS;
    static QString OBJET_FOURNISSEURS;
    static QString OBJET_QUARTIER;
    static QString OBJET_REGIONS_ETATS;
    static QString OBJET_VILLES;

    static QString GRAPHE_BAR_CHART;
    static QString GRAPHE_PIE_CHART;

    static const double STATS_MIN_VALUE;

    static const unsigned int MAX_YEARS_REPORTS;


    YerothLogger *_logger;

    QString _searchFilter;

    QString _curDetailPDFFileTitleString;

    QString _reportTexFileEndString;

    QString _current_succursale_STRING;

    QMap < QString, int >_moisToNombre;

    int _objetServiceLastIndex;

    int _objetClientLastIndex;

    unsigned int _csvFileItemSize;
    unsigned int _startYear;

    bool _GENERATE_THREE_D_BARD_DIAGRAM;

    YerothSqlTableModel *_curStocksVenduTableModel;
};


#endif /* YEROTH_BUSINESS_REPORTS_WINDOW_HPP_ */
