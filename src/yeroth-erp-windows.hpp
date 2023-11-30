/*
 * yeroth-erp-windows.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_WINDOWS_HPP_
#define SRC_YEROTH_WINDOWS_HPP_

#include <QtCore/QObject>

#include "src/windows/yeroth-erp-main-window.hpp"


#include "src/windows/yeroth-erp-recherche-conditionnelle-window.hpp"
#include "src/windows/yeroth-erp-paiements-window.hpp"
#include "src/windows/yeroth-erp-tableaux-de-bord-window.hpp"
#include "src/windows/yeroth-erp-comptabilite-window.hpp"
#include "src/windows/yeroth-erp-alertes-window.hpp"
#include "src/windows/yeroth-erp-ventes-window.hpp"


//** STOCKS
#include "src/windows/stocks/yeroth-erp-mouvements_de_stocks-window.hpp"
#include "src/windows/yeroth-erp-entrer-window.hpp"
#include "src/windows/stocks/yeroth-erp-historique-du-stock-window.hpp"
#include "src/windows/stocks/yeroth-erp-marchandises-window.hpp"
#include "src/windows/stocks/yeroth-erp-modifier-window.hpp"
#include "src/windows/stocks/yeroth-erp-stock-detail-window.hpp"
#include "src/windows/stocks/yeroth-erp-sortir-liststocks-window.hpp"
#include "src/windows/stocks/yeroth-erp-stocks-window.hpp"
#include "src/windows/stocks/yeroth-erp-sortir-window.hpp"


//** DIALOGS
#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"
#include "src/dialogs/yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp"
#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-carte-credit-dialog.hpp"
#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-comptant-entree-dialog.hpp"
#include "src/dialogs/yeroth-erp-pointdevente-methode-paiement-dialog.hpp"


//** POS
#include "src/windows/pos/yeroth-erp-pointdevente-liststocks-window.hpp"
#include "src/windows/pos/yeroth-erp-pointdevente-window.hpp"

//** CHARGES FINANCIÈRES
#include "src/windows/yeroth-erp-charges-financieres-window.hpp"
#include "src/windows/yeroth-erp-charges-financieres-detail-window.hpp"


//** PURCHASE
#include "src/windows/purchase/yeroth-erp-achats-window.hpp"
#include "src/windows/purchase/yeroth-erp-achats-detail-window.hpp"


//** SUPPLIER
#include "src/windows/supplier/yeroth-erp-tableau-des-transactions-du-fournisseur-window.hpp"
#include "src/windows/supplier/yeroth-erp-fournisseur-details-window.hpp"
#include "src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.hpp"

#include "src/windows/supplier/yeroth-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp"

#include "src/windows/supplier/yeroth-erp-creer-GROUPE-DE-PAIE-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-creer-groupe-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-creer-groupe-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-GROUPES-DEMPLOYES-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-GROUPES-DE-PAIE-hr-window.hpp"
#include "src/windows/supplier/yeroth-erp-fournisseurs-window.hpp"
#include "src/windows/supplier/yeroth-erp-creer-fournisseur-window.hpp"
#include "src/windows/supplier/yeroth-erp-modifier-fournisseur-window.hpp"


//** CRM
#include "src/windows/crm/creer/yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp"
#include "src/windows/crm/creer/yeroth-erp-creer-groupe-de-clients-window.hpp"
#include "src/windows/crm/creer/yeroth-erp-creer-compte-client-window.hpp"

#include "src/windows/crm/details/yeroth-erp-details-dun-groupe-de-clients-window.hpp"
#include "src/windows/crm/details/yeroth-erp-details-dun-programme-de-fidelite-clients-window.hpp"
#include "src/windows/crm/details/yeroth-erp-clients-detail-window.hpp"

#include "src/windows/crm/modifier/yeroth-erp-modifier-compte-client-window.hpp"

#include "src/windows/crm/yeroth-erp-programmes-de-fidelite-clients-window.hpp"
#include "src/windows/crm/yeroth-erp-groupes-de-clients-window.hpp"
#include "src/windows/crm/yeroth-erp-groupes-dun-client-window.hpp"
#include "src/windows/crm/yeroth-erp-clients-window.hpp"
#include "src/windows/crm/yeroth-erp-tableau-des-transactions-du-client-window.hpp"
#include "src/windows/crm/yeroth-erp-payer-compte-client-window.hpp"




//** ADMINISTRATION
#include "src/admin/yeroth-erp-admin-window.hpp"
#include "src/admin/creer/yeroth-erp-admin-create-window.hpp"
#include "src/admin/lister/yeroth-erp-admin-lister-window.hpp"
#include "src/admin/detail/yeroth-erp-admin-detail-window.hpp"
#include "src/admin/modifier/yeroth-erp-admin-modifier-window.hpp"


//** UTILS
#include "src/utils/yeroth-erp-database.hpp"
#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlDatabase>


class YerothSqlTableModel;
class YerothMainWindow;
class YerothRechercheConditionnelleWindow;
class YerothPaiementsWindow;
class YerothTableauDesTransactionsDuFournisseurWindow;
class YerothTableauDesTransactionsDuClientWindow;
class YerothHistoriqueDuStockWindow;
class YerothPOSChangerUtilisateurDialog;
class YerothCreerFournisseurWindow;
class YerothDetailsDunProgrammeDeFideliteClientsWindow;
class YerothCreerUnProgrammeDeFideliteClientsWindow;
class YerothCreerGroupeDeClientsWindow;
class YerothCreerCompteClientWindow;
class YerothModifierFournisseurWindow;
class YerothModifierCompteClientWindow;
class YerothAchatsWindow;
class YerothStocksWindow;
class YerothMarchandisesWindow;
class YerothComptabiliteWindow;
class YerothAlertesWindow;
class YerothVentesWindow;
class YerothModifierWindow;
class YerothSortirWindow;
class YerothTableauxDeBordWindow;
class YerothMouvementsDeStocksWindow;
class YerothDetailsDunGroupeDeClientsWindow;
class YerothPointDeVenteWindow;
class YerothPOSVenteMethodePaiementCarteCreditDialog;
class YerothPOSVenteMethodePaiementComptantEntreeDialog;
class YerothPOSVenteMethodePaiementDialog;
class YerothFournisseurDetailsWindow;
class YerothGroupesDunClientWindow;
class YerothClientsDetailWindow;
class YerothChargesFinancieresWindow;
class YerothChargesFinancieresDetailsWindow;
class YerothAchatsDetailWindow;
class YerothStockDetailWindow;
class YerothEntrerWindow;
class YerothAdminWindow;
class YerothAdminCreateWindow;
class YerothAdminListerWindow;
class YerothAdminDetailWindow;
class YerothAdminModifierWindow;
class YerothPOSUser;
class YerothInfoEntreprise;
class YerothLogger;
class YerothDatabase;
class YerothPointDeVenteListStocksWindow;
class YerothPayerCompteClientWindow;
class YerothGROUPES_DUN_EMPLOYE_Window;
class YerothCreerUnGroupeDePaieWindow;
class YerothCreerGroupeDemployesWindow;
class YerothDetailsDunGroupeDePaieWindow;
class YerothERPGroupesDemployesHRWindow;
class YerothERPGROUPES_DE_PAIE_Window;
class YerothERPFournisseursWindow;
class YerothERPClientsWindow;
class YerothSortirListStocksWindow;
class YerothUtils;

#ifdef YEROTH_SERVER
class YEROTH_RUNTIME_VERIFIER;
#endif

#ifdef YEROTH_CLIENT
class YerothDBusClient;
#endif

class QString;
class QDesktopWidget;

class YerothERPWindows : public QObject
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS YerothERPWindows(QDesktopWidget *desktopWidget);

    virtual ~YerothERPWindows();

    void createAllYerothPosUserWindows();

    void reinitialiseSqlTableModels();

    void setupSqlTableModelFromName(const QString       &aSqlTableName,
                                    YerothSqlTableModel **aYerothSqlTableModel);

    void setupConfiguration();


    YerothSqlTableModel &getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();

    YerothSqlTableModel &getSqlTableModel_imprimantereseau_recus_petits();

    YerothSqlTableModel &getSqlTableModel_types_doperations_financieres();

    YerothSqlTableModel &getSqlTableModel_operations_comptables();

    YerothSqlTableModel &getSqlTableModel_comptes_bancaires();

    YerothSqlTableModel &getSqlTableModel_entreprise_info();

    YerothSqlTableModel &getSqlTableModel_paiements();

    YerothSqlTableModel &getSqlTableModel_users();

    YerothSqlTableModel &getSqlTableModel_titres();

    YerothSqlTableModel &getSqlTableModel_localisations();

    YerothSqlTableModel &getSqlTableModel_departements_produits();

    YerothSqlTableModel &getSqlTableModel_categories();

    YerothSqlTableModel &getSqlTableModel_lignes_budgetaires();

    YerothSqlTableModel &getSqlTableModel_alertes();

    YerothSqlTableModel &getSqlTableModel_remises();

    YerothSqlTableModel &getSqlTableModel_conditions_alertes();

    YerothSqlTableModel &getSqlTableModel_courriers_alertes();

    YerothSqlTableModel &getSqlTableModel_programmes_de_fidelite_clients();

    YerothSqlTableModel &getSqlTableModel_clients();

    YerothSqlTableModel &getSqlTableModel_groupes_demployes_hr();

    YerothSqlTableModel &getSqlTableModel_groupes_de_paie_hr();

    YerothSqlTableModel &getSqlTableModel_groupes_de_clients();

    YerothSqlTableModel &getSqlTableModel_fournisseurs();

    YerothSqlTableModel &getSqlTableModel_charges_financieres();

    YerothSqlTableModel &getSqlTableModel_achats();

    YerothSqlTableModel &getSqlTableModel_stocks();

    YerothSqlTableModel &getSqlTableModel_services_completes();

    YerothSqlTableModel &getSqlTableModel_marchandises();

    YerothSqlTableModel &getSqlTableModel_stocks_vendu();

    YerothSqlTableModel &getSqlTableModel_stocks_sorties();

    YerothSqlTableModel &getSqlTableModel_configurations();

    YerothSqlTableModel &getSqlTableModel_init_configurations();

#ifdef YEROTH_SERVER
    inline YEROTH_RUNTIME_VERIFIER *dbusServer()
    {
        return _dbusServer;
    }
#endif

#ifdef YEROTH_CLIENT
    inline YerothDBusClient *dbusClient()
    {
        return _dbusClient;
    }
#endif

    inline static const QString &get_last_lister_selected_row_db_ID()
    {
        return YerothWindowsCommons::get_last_lister_selected_row_db_ID();
    }

    inline void setDesktopWidget(QDesktopWidget *desktopWidget)
    {
        _desktopWidget = desktopWidget;
    }

    inline QDesktopWidget &desktopWidget()
    {
        return *_desktopWidget;
    }

    static inline int getNextIdSqlTableModel_periodes_dappartenance_groupes_de_paie_hr()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);
    }

    static inline int getNextIdSqlTableModel_users()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::USERS);
    }

    static inline int getNextIdSqlTableModel_titres()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::TITRES);
    }

    static inline int getNextIdSqlTableModel_localisations()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    LOCALISATIONS);
    }

    static inline int getNextIdSqlTableModel_departements_produits()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    DEPARTEMENTS_PRODUITS);
    }

    static inline int getNextIdSqlTableModel_categories()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::CATEGORIES);
    }

    static inline int getNextIdSqlTableModel_LIGNES_BUDGETAIRES()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::LIGNES_BUDGETAIRES);
    }

    static inline int getNextIdSqlTableModel_alertes()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::ALERTES);
    }

    static inline int getNextIdSqlTableModel_conditions_alertes()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    CONDITIONS_ALERTES);
    }

    static inline int getNextIdSqlTableModel_courriers_alertes()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    COURRIERS_ALERTES);
    }

    static inline int getNextIdSqlTableModel_programme_de_fidelite_clients()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    PROGRAMMES_DE_FIDELITE_CLIENTS);
    }

    static inline int getNextIdSqlTableModel_groupes_demployes_hr()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::GROUPES_DEMPLOYES_hr);
    }

    static inline int getNextIdSqlTableModel_groupes_de_paie_hr()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::GROUPES_DE_PAIE_hr);
    }

    static inline int getNextIdSqlTableModel_clients()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::CLIENTS);
    }

    static inline int getNextIdSqlTableModel_groupes_de_clients()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    GROUPES_DE_CLIENTS);
    }

    static inline int getNextIdSqlTableModel_fournisseurs()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    FOURNISSEURS);
    }

    static inline int getNextIdSqlTableModel_comptes_bancaires()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    COMPTES_BANCAIRES);
    }


    static inline int getNextIdSqlTableModel_imprimantereseau_recus_petits()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    IMPRIMANTERESEAU_RECUS_PETITS);
    }


    static inline int getNextIdSqlTableModel_types_doperations_financieres()
    {
        return YerothERPWindows::getNextIdFromTable
                (YerothDatabase::TYPE_DOPERATIONS_FINANCIERES);
    }


    static inline int getNextIdSqlTableModel_operations_comptables()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    OPERATIONS_COMPTABLES);
    }


    static inline int getNextIdSqlTableModel_charges_financieres()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    CHARGES_FINANCIERES);
    }

    static inline int getNextIdSqlTableModel_achats()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::ACHATS);
    }

    static inline int getNextIdSqlTableModel_marchandises()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    MARCHANDISES);
    }

    static inline int getNextIdSqlTableModel_paiements()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::PAIEMENTS);
    }

    static inline int getNextIdSqlTableModel_stocks()
    {
        return YerothERPWindows::NEXT_TABLE_ID_MYSQL_FOR_STOCKS();
    }

    static inline int getNextIdSqlTableModel_services_completes()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    SERVICES_COMPLETES);
    }

    static inline int getNextIdSqlTableModel_stocks_vendu()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    STOCKS_VENDU);
    }

    static inline int getNextIdSqlTableModel_stocks_sorties()
    {
        return YerothERPWindows::getNextIdFromTable(YerothDatabase::
                                                    STOCKS_SORTIES);
    }

    inline void setCurrentPosition(QPoint *currentPosition)
    {
        _currentPosition = currentPosition;
    }

    void setCurrentPosition(int xPos, int yPos);

    inline QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

    inline YerothPOSUser *getUser()
    {
        return _user;
    }

    inline YerothDatabase &getDatabase()
    {
        return *_database;
    }

    inline void setDatabase(YerothDatabase *database)
    {
        _database = database;
    }

    inline YerothInfoEntreprise &getInfoEntreprise()
    {
        return *_infoEntreprise;
    }

    static inline YerothSqlTableModel *getSqlTableModelFromName(const QString &
                                                                aSqlTableName)
    {
        return _sqltablenameTOsqltablemodel.value(aSqlTableName);
    }

    bool move(QMainWindow &aWindow);

    bool updateUserData(int userRecordRow);

    void setUser(YerothPOSUser *user);

    void definirAdministrateur();

    void definirMagasinier();

    void definirCaissier();

    void definirManager();

    void definirVendeur();

    void definirGestionaireDesStocks();

    void definirPasDeRole();

    void hideAllWindows();

    static QTranslator *_FRENCH_TRANSLATOR;

    static QTranslator *_ENGLISH_TRANSLATOR;

    static QApplication *_YEROTH_ERP_PGI_APPLICATION;

    static bool CURRENTLY_CHANGING_USER;


    YerothMainWindow *_mainWindow;

    YerothRechercheConditionnelleWindow *_recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;
    YerothPOSChangerUtilisateurDialog *_changerUtilisateurDialog;
    YerothTableauDesTransactionsDuFournisseurWindow
    *_transactionsDunFournisseurWindow;
    YerothTableauDesTransactionsDuClientWindow *_transactionsDunClientWindow;
    YerothHistoriqueDuStockWindow *_historiqueDuStockWindow;
    YerothCreerFournisseurWindow *_creerFournisseurWindow;
    YerothDetailsDunProgrammeDeFideliteClientsWindow
    *_detailsDunProgrammeDeFideliteClientsWindow;
    YerothCreerUnProgrammeDeFideliteClientsWindow
    *_creerUnProgrammeDeFideliteClientsWindow;
    YerothCreerGroupeDeClientsWindow *_creerGroupeDeClientsWindow;
    YerothCreerCompteClientWindow *_creerCompteClientWindow;
    YerothModifierFournisseurWindow *_modifierFournisseurWindow;
    YerothModifierCompteClientWindow *_modifierCompteClientWindow;
    YerothAchatsWindow *_achatsWindow;
    YerothStocksWindow *_stocksWindow;
    YerothMarchandisesWindow *_marchandisesWindow;
    YerothComptabiliteWindow *_comptabiliteWindow;
    YerothAlertesWindow *_listerAlertesWindow;
    YerothPaiementsWindow *_paiementsWindow;
    YerothVentesWindow *_ventesWindow;
    YerothModifierWindow *_modifierWindow;
    YerothSortirWindow *_sortirWindow;
    YerothTableauxDeBordWindow *_statistiquesWindow;
    YerothMouvementsDeStocksWindow *_mouvementsDeStocksWindow;
    YerothPointDeVenteWindow *_pdVenteWindow;
    YerothFournisseurDetailsWindow *_fournisseurDetailsWindow;
    YerothERPProgrammesDeFideliteClientsWindow
    *_programmesDeFideliteClientsWindow;
    YerothDetailsDunGroupeDeClientsWindow *_detailsGroupeDeClientsWindow;
    YerothGroupesDeClientsWindow *_groupesDeClientsWindow;
    YerothGroupesDunClientWindow *_groupesDunClientWindow;
    YerothClientsDetailWindow *_clientsDetailWindow;
    YerothChargesFinancieresWindow *_charges_financieresWindow;
    YerothChargesFinancieresDetailsWindow *_charges_financieres_detailsWindow;
    YerothAchatsDetailWindow *_achatsDetailWindow;
    YerothStockDetailWindow *_detailWindow;
    YerothEntrerWindow *_entrerWindow;
    YerothIMPRESSION_DE_DOCUMENT_Dialog *_impressionDeDocumentDialog;
    YerothPointDeVenteListStocksWindow *_pdVenteListStocksWindow;
    YerothPOSVenteMethodePaiementCarteCreditDialog
    *_pdVenteMethodePaiementCarteCreditDialog;
    YerothPOSVenteMethodePaiementComptantEntreeDialog
    *_pdVenteMethodePaiementComptantEntreeDialog;
    YerothPOSVenteMethodePaiementDialog *_pdVenteMethodePaiementDialog;
    YerothGROUPES_DUN_EMPLOYE_Window *_groupes_DUN_EMPLOYE_window;
    YerothCreerUnGroupeDePaieWindow *_CREER_GROUPE_DE_PAIE_hr_window;
    YerothCreerGroupeDemployesWindow *_CREER_groupe_demployes_window;
    YerothDetailsDunGroupeDemployesWindow *_DETAILS_DUN_GROUPE_DEMPLOYES_hr;
    YerothDetailsDunGroupeDePaieWindow *_DETAILS_dun_groupe_de_PAIE_hr;
    YerothERPGroupesDemployesHRWindow *_groupe_demployes_window;
    YerothERPGROUPES_DE_PAIE_Window *_GROUPES_DE_PAIE_Window;
    YerothERPFournisseursWindow *_fournisseursWindow;
    YerothERPClientsWindow *_clientsWindow;
    YerothPayerCompteClientWindow *_payerAuCompteClientWindow;
    YerothSortirListStocksWindow *_sortirListStocksWindow;
    YerothAdminWindow *_adminWindow;
    YerothAdminCreateWindow *_adminCreateWindow;
    YerothAdminListerWindow *_adminListerWindow;
    YerothAdminDetailWindow *_adminDetailWindow;
    YerothAdminModifierWindow *_adminModifierWindow;

private:

    static int NEXT_TABLE_ID_MYSQL_FOR_STOCKS();

    static int getNextIdFromTable(const QString &tableName);

    YerothDatabase *_database;

    static QMap<QString, YerothSqlTableModel *> _sqltablenameTOsqltablemodel;

    YerothSqlTableModel *_tableModel_periodes_dappartenance_groupes_de_paie_hr;
    YerothSqlTableModel *_tableModel_imprimantereseau_recus_petits;
    YerothSqlTableModel *_tableModel_types_doperations_financieres;
    YerothSqlTableModel *_tableModel_operations_comptables;
    YerothSqlTableModel *_tableModel_comptes_bancaires;
    YerothSqlTableModel *_tableModel_entreprise_info;
    YerothSqlTableModel *_tableModel_historique_paiements;
    YerothSqlTableModel *_tableModel_users;
    YerothSqlTableModel *_tableModel_titres;
    YerothSqlTableModel *_tableModel_localisations;
    YerothSqlTableModel *_tableModel_departements_produits;
    YerothSqlTableModel *_tableModel_categories;
    YerothSqlTableModel *_tableModel_ligne_budgetaire;
    YerothSqlTableModel *_tableModel_programmes_de_fidelite_clients;
    YerothSqlTableModel *_tableModel_groupes_demployes_hr;
    YerothSqlTableModel *_tableModel_groupes_de_paie_hr;
    YerothSqlTableModel *_tableModel_clients;
    YerothSqlTableModel *_tableModel_groupes_de_clients;
    YerothSqlTableModel *_tableModel_fournisseurs;
    YerothSqlTableModel *_tableModel_alertes;
    YerothSqlTableModel *_tableModel_remises;
    YerothSqlTableModel *_tableModel_conditions_alertes;
    YerothSqlTableModel *_tableModel_courriers_alertes;
    YerothSqlTableModel *_tableModel_charges_financieres;
    YerothSqlTableModel *_tableModel_achats;
    YerothSqlTableModel *_tableModel_stocks;
    YerothSqlTableModel *_tableModel_services_completes;
    YerothSqlTableModel *_tableModel_marchandises;
    YerothSqlTableModel *_tableModel_stocks_vendu;
    YerothSqlTableModel *_tableModel_stocks_sorties;
    YerothSqlTableModel *_tableModel_configurations;
    YerothSqlTableModel *_tableModel_init_configurations;

#ifdef YEROTH_SERVER
    YEROTH_RUNTIME_VERIFIER *_dbusServer;
#endif

#ifdef YEROTH_CLIENT
    YerothDBusClient *_dbusClient;
#endif


    YerothLogger *_logger;

    YerothInfoEntreprise *_infoEntreprise;
    YerothPOSUser *_user;

    QPoint *_currentPosition;
    QDesktopWidget *_desktopWidget;
};

#endif /* SRC_YEROTH_WINDOWS_HPP_ */
