/*
 * yeroth-erp-windows.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/admin/yeroth-erp-admin-window.hpp"


#ifdef YEROTH_SERVER
#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"
#endif

#ifdef YEROTH_CLIENT
#include "src/dbus/yeroth-erp-dbus-client.hpp"
#endif


#include <QtCore/QList>

#include <QtCore/QMap>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlDriver>

#include <QtSql/QSqlQuery>

#include <QtWidgets/QDesktopWidget>


QTranslator *YerothERPWindows::_FRENCH_TRANSLATOR = 0;

QTranslator *YerothERPWindows::_ENGLISH_TRANSLATOR = 0;

QApplication *YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION = 0;

bool YerothERPWindows::CURRENTLY_CHANGING_USER(false);


QMap <QString, YerothSqlTableModel *> YerothERPWindows::_sqltablenameTOsqltablemodel;


YerothERPWindows::YerothERPWindows(QDesktopWidget *desktopWidget):_mainWindow(0),
    _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW(0),
    _transactionsDunFournisseurWindow(0),
    _transactionsDunClientWindow(0),
    _historiqueDuStockWindow(0),
    _changerUtilisateurDialog(0),
    _creerFournisseurWindow(0),
    _detailsDunProgrammeDeFideliteClientsWindow(0),
    _creerUnProgrammeDeFideliteClientsWindow(0),
    _creerGroupeDeClientsWindow(0),
    _creerCompteClientWindow(0),
    _modifierFournisseurWindow(0),
    _modifierCompteClientWindow(0),
    _achatsWindow(0),
    _stocksWindow(0),
    _marchandisesWindow(0),
    _comptabiliteWindow(0),
    _listerAlertesWindow(0),
    _paiementsWindow(0),
    _ventesWindow(0),
    _modifierWindow(0),
    _sortirWindow(0),
    _statistiquesWindow(0),
    _mouvementsDeStocksWindow(0),
    _pdVenteWindow(0),
    _fournisseurDetailsWindow(0),
    _programmesDeFideliteClientsWindow(0),
    _detailsGroupeDeClientsWindow(0),
    _groupesDeClientsWindow(0),
    _groupesDunClientWindow(0),
    _clientsDetailWindow(0),
    _charges_financieresWindow(0),
    _achatsDetailWindow(0),
    _detailWindow(0),
    _entrerWindow(0),
    _pdVenteListStocksWindow(0),
    _impressionDeDocumentDialog(0),
    _pdVenteMethodePaiementCarteCreditDialog(0),
    _pdVenteMethodePaiementComptantEntreeDialog(0),
    _pdVenteMethodePaiementDialog(0),
	_groupes_DUN_EMPLOYE_window(0),
	_CREER_GROUPE_DE_PAIE_hr_window(0),
	_CREER_groupe_demployes_window(0),
	_DETAILS_DUN_GROUPE_DEMPLOYES_hr(0),
	_DETAILS_dun_groupe_de_PAIE_hr(0),
	_groupe_demployes_window(0),
	_GROUPES_DE_PAIE_Window(0),
    _fournisseursWindow(0),
    _clientsWindow(0),
    _payerAuCompteClientWindow(0),
    _sortirListStocksWindow(0),
    _adminWindow(0),
    _adminCreateWindow(0),
    _adminListerWindow(0),
    _adminDetailWindow(0),
    _adminModifierWindow(0),
    _database(0),
    _tableModel_periodes_dappartenance_groupes_de_paie_hr(0),
    _tableModel_imprimantereseau_recus_petits(0),
    _tableModel_types_doperations_financieres(0),
    _tableModel_operations_comptables(0),
    _tableModel_comptes_bancaires(0),
    _tableModel_entreprise_info(0),
    _tableModel_historique_paiements(0),
    _tableModel_users(0),
    _tableModel_titres(0),
    _tableModel_localisations(0),
    _tableModel_departements_produits(0),
    _tableModel_categories(0),
	_tableModel_ligne_budgetaire(0),
    _tableModel_programmes_de_fidelite_clients(0),
	_tableModel_groupes_demployes_hr(0),
	_tableModel_groupes_de_paie_hr(0),
    _tableModel_clients(0),
	_tableModel_groupes_de_clients(0),
    _tableModel_fournisseurs(0),
    _tableModel_alertes(0),
    _tableModel_remises(0),
    _tableModel_conditions_alertes(0),
    _tableModel_courriers_alertes(0),
    _tableModel_charges_financieres(0),
    _tableModel_achats(0),
    _tableModel_stocks(0),
    _tableModel_services_completes(0),
    _tableModel_stocks_vendu(0),
    _tableModel_stocks_sorties(0),
    _tableModel_marchandises(0),
    _tableModel_configurations(0),
    _tableModel_init_configurations(0),
#ifdef YEROTH_SERVER
    _dbusServer(0),
#endif
#ifdef YEROTH_CLIENT
    _dbusClient(0),
#endif
    _logger(0),
    _infoEntreprise(0),
    _user(0),
    _currentPosition(0),
    _desktopWidget(desktopWidget)
{
    _logger = new YerothLogger("YerothWindows");

    YerothERPWindows::setupConfiguration();

    static YerothDatabaseTableColumn aYerothDatabaseTableColumnStaticInstance;

    setupSqlTableModelFromName(YerothDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR,
                               &_tableModel_periodes_dappartenance_groupes_de_paie_hr);

    setupSqlTableModelFromName(YerothDatabase::IMPRIMANTERESEAU_RECUS_PETITS,
                               &_tableModel_imprimantereseau_recus_petits);

    setupSqlTableModelFromName(YerothDatabase::TYPE_DOPERATIONS_FINANCIERES,
                               &_tableModel_types_doperations_financieres);

    setupSqlTableModelFromName(YerothDatabase::OPERATIONS_COMPTABLES,
                               &_tableModel_operations_comptables);

    setupSqlTableModelFromName(YerothDatabase::COMPTES_BANCAIRES,
                               &_tableModel_comptes_bancaires);

    setupSqlTableModelFromName(YerothDatabase::ENTREPRISE_INFO,
                               &_tableModel_entreprise_info);

    setupSqlTableModelFromName(YerothDatabase::PAIEMENTS,
                               &_tableModel_historique_paiements);

    setupSqlTableModelFromName(YerothDatabase::USERS, &_tableModel_users);

    setupSqlTableModelFromName(YerothDatabase::TITRES, &_tableModel_titres);

    setupSqlTableModelFromName(YerothDatabase::LOCALISATIONS,
                               &_tableModel_localisations);

    setupSqlTableModelFromName(YerothDatabase::DEPARTEMENTS_PRODUITS,
                               &_tableModel_departements_produits);

    setupSqlTableModelFromName(YerothDatabase::CATEGORIES,
                               &_tableModel_categories);

    setupSqlTableModelFromName(YerothDatabase::LIGNES_BUDGETAIRES,
                               &_tableModel_ligne_budgetaire);

    setupSqlTableModelFromName(YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                               &_tableModel_programmes_de_fidelite_clients);

    setupSqlTableModelFromName(YerothDatabase::GROUPES_DEMPLOYES_hr,
                                   &_tableModel_groupes_demployes_hr);

    setupSqlTableModelFromName(YerothDatabase::GROUPES_DE_PAIE_hr,
                                   &_tableModel_groupes_de_paie_hr);

    setupSqlTableModelFromName(YerothDatabase::CLIENTS, &_tableModel_clients);

    setupSqlTableModelFromName(YerothDatabase::GROUPES_DE_CLIENTS,
                               &_tableModel_groupes_de_clients);

    setupSqlTableModelFromName(YerothDatabase::FOURNISSEURS,
                               &_tableModel_fournisseurs);

    setupSqlTableModelFromName(YerothDatabase::ALERTES, &_tableModel_alertes);
    setupSqlTableModelFromName(YerothDatabase::REMISES, &_tableModel_remises);
    setupSqlTableModelFromName(YerothDatabase::CONDITIONS_ALERTES,
                               &_tableModel_conditions_alertes);
    setupSqlTableModelFromName(YerothDatabase::COURRIERS_ALERTES,
                               &_tableModel_courriers_alertes);
    setupSqlTableModelFromName(YerothDatabase::CHARGES_FINANCIERES,
                               &_tableModel_charges_financieres);
    setupSqlTableModelFromName(YerothDatabase::ACHATS, &_tableModel_achats);
    setupSqlTableModelFromName(YerothDatabase::STOCKS, &_tableModel_stocks);
    setupSqlTableModelFromName(YerothDatabase::SERVICES_COMPLETES,
                               &_tableModel_services_completes);
    setupSqlTableModelFromName(YerothDatabase::STOCKS_VENDU,
                               &_tableModel_stocks_vendu);
    setupSqlTableModelFromName(YerothDatabase::STOCKS_SORTIES,
                               &_tableModel_stocks_sorties);
    setupSqlTableModelFromName(YerothDatabase::MARCHANDISES,
                               &_tableModel_marchandises);
    setupSqlTableModelFromName(YerothDatabase::CONFIGURATIONS,
                               &_tableModel_configurations);
    setupSqlTableModelFromName(YerothDatabase::INIT_CONFIGURATIONS,
                               &_tableModel_init_configurations);

    /*
     * This is to initialize constants useful for 'INTERNATIONALIZATION'.
     */
    YerothUtils
    yerothUtilsDummy;

#ifdef YEROTH_SERVER
    _dbusServer = new YEROTH_RUNTIME_VERIFIER;
#endif

#ifdef YEROTH_CLIENT
    _dbusClient = new YEROTH_RUNTIME_VERIFIER(this);
#endif
}

YerothERPWindows::~YerothERPWindows()
{
    delete _changerUtilisateurDialog;

    delete _mainWindow;
    delete _sortirWindow;
    delete _statistiquesWindow;
    delete _mouvementsDeStocksWindow;
    delete _impressionDeDocumentDialog;
    delete _pdVenteMethodePaiementCarteCreditDialog;
    delete _pdVenteMethodePaiementComptantEntreeDialog;
    delete _pdVenteMethodePaiementDialog;
    delete _pdVenteWindow;
    delete _detailWindow;
    delete _achatsDetailWindow;
    delete _charges_financieres_detailsWindow;
    delete _charges_financieresWindow;
    delete _clientsDetailWindow;
    delete _groupesDunClientWindow;
    delete _groupesDeClientsWindow;
    delete _detailsGroupeDeClientsWindow;
    delete _programmesDeFideliteClientsWindow;
    delete _fournisseurDetailsWindow;
    delete _modifierWindow;
    delete _entrerWindow;
    delete _creerCompteClientWindow;
    delete _creerGroupeDeClientsWindow;
    delete _creerUnProgrammeDeFideliteClientsWindow;
    delete _detailsDunProgrammeDeFideliteClientsWindow;
    delete _creerFournisseurWindow;
    delete _modifierCompteClientWindow;
    delete _modifierFournisseurWindow;
    delete _stocksWindow;
    delete _achatsWindow;
    delete _marchandisesWindow;
    delete _comptabiliteWindow;
    delete _listerAlertesWindow;
    delete _paiementsWindow;
    delete _ventesWindow;
    delete _pdVenteListStocksWindow;
    delete _historiqueDuStockWindow;
    delete _transactionsDunClientWindow;
    delete _transactionsDunFournisseurWindow;
    delete _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW;
    delete _clientsWindow;
    delete _GROUPES_DE_PAIE_Window;
    delete _groupe_demployes_window;
    delete _DETAILS_dun_groupe_de_PAIE_hr;
    delete _DETAILS_DUN_GROUPE_DEMPLOYES_hr;
    delete _CREER_groupe_demployes_window;
    delete _groupes_DUN_EMPLOYE_window;
    delete _fournisseursWindow;
    delete _payerAuCompteClientWindow;
    delete _sortirListStocksWindow;

    delete _adminWindow;
    delete _adminCreateWindow;
    delete _adminListerWindow;
    delete _adminDetailWindow;
    delete _adminModifierWindow;

#ifdef YEROTH_SERVER
    delete _dbusServer;
#endif

#ifdef YEROTH_CLIENT
    delete _dbusClient;
#endif

    delete _logger;
    delete _infoEntreprise;
}

void YerothERPWindows::createAllYerothPosUserWindows()
{
    _infoEntreprise = new YerothInfoEntreprise;

    _changerUtilisateurDialog = new YerothPOSChangerUtilisateurDialog(this);

    _mainWindow = new YerothMainWindow;

    _currentPosition = new QPoint;

    YerothUtils::getCenterPosition(_desktopWidget->width(),
                                   _desktopWidget->height(),
                                   *_mainWindow, *_currentPosition);

    YerothWindowsCommons::setCenterPosition(_currentPosition);

    YerothPOSAdminWindowsCommons::setCenterPosition(_currentPosition);

    _adminWindow = new YerothAdminWindow;
    _adminCreateWindow = new YerothAdminCreateWindow;
    _adminListerWindow = new YerothAdminListerWindow;
    _adminDetailWindow = new YerothAdminDetailWindow;
    _adminModifierWindow = new YerothAdminModifierWindow;

    _recherche_CONDITIONNELLE_SEARCH_FILTER_WINDOW =
                    new YerothRechercheConditionnelleWindow;
    _transactionsDunFournisseurWindow =
                    new YerothTableauDesTransactionsDuFournisseurWindow;
    _transactionsDunClientWindow =
                    new YerothTableauDesTransactionsDuClientWindow;
    _historiqueDuStockWindow = new YerothHistoriqueDuStockWindow;
    _sortirWindow = new YerothSortirWindow;
    _statistiquesWindow = new YerothTableauxDeBordWindow;
    _mouvementsDeStocksWindow = new YerothMouvementsDeStocksWindow;

    _impressionDeDocumentDialog = new YerothIMPRESSION_DE_DOCUMENT_Dialog;

    _pdVenteMethodePaiementCarteCreditDialog
        = new YerothPOSVenteMethodePaiementCarteCreditDialog(_pdVenteWindow);

    _pdVenteMethodePaiementComptantEntreeDialog
        =
                        new YerothPOSVenteMethodePaiementComptantEntreeDialog(this,
                                                                              _pdVenteWindow);

    _pdVenteMethodePaiementDialog =
                    new YerothPOSVenteMethodePaiementDialog(this, _pdVenteWindow);

    _pdVenteWindow = new YerothPointDeVenteWindow;
    _fournisseurDetailsWindow = new YerothFournisseurDetailsWindow;
    _programmesDeFideliteClientsWindow =
                    new YerothERPProgrammesDeFideliteClientsWindow;
    _detailsGroupeDeClientsWindow = new YerothDetailsDunGroupeDeClientsWindow;
    _groupesDeClientsWindow = new YerothGroupesDeClientsWindow;
    _groupesDunClientWindow = new YerothGroupesDunClientWindow;
    _clientsDetailWindow = new YerothClientsDetailWindow;
    _charges_financieresWindow = new YerothChargesFinancieresWindow;
    _charges_financieres_detailsWindow =
                    new YerothChargesFinancieresDetailsWindow;
    _achatsDetailWindow = new YerothAchatsDetailWindow;
    _detailWindow = new YerothStockDetailWindow;
    _modifierWindow = new YerothModifierWindow;
    _entrerWindow = new YerothEntrerWindow;
    _creerFournisseurWindow = new YerothCreerFournisseurWindow;
    _detailsDunProgrammeDeFideliteClientsWindow =
                    new YerothDetailsDunProgrammeDeFideliteClientsWindow;
    _creerUnProgrammeDeFideliteClientsWindow =
                    new YerothCreerUnProgrammeDeFideliteClientsWindow;
    _creerGroupeDeClientsWindow = new YerothCreerGroupeDeClientsWindow;
    _creerCompteClientWindow = new YerothCreerCompteClientWindow;
    _modifierFournisseurWindow = new YerothModifierFournisseurWindow;
    _modifierCompteClientWindow = new YerothModifierCompteClientWindow;
    _achatsWindow = new YerothAchatsWindow;
    _stocksWindow = new YerothStocksWindow;
    _marchandisesWindow = new YerothMarchandisesWindow;
    _comptabiliteWindow = new YerothComptabiliteWindow;
    _listerAlertesWindow = new YerothAlertesWindow;
    _paiementsWindow = new YerothPaiementsWindow;
    _ventesWindow = new YerothVentesWindow;
    _pdVenteListStocksWindow = new YerothPointDeVenteListStocksWindow;
    _groupes_DUN_EMPLOYE_window = new YerothGROUPES_DUN_EMPLOYE_Window;
    _CREER_GROUPE_DE_PAIE_hr_window = new YerothCreerUnGroupeDePaieWindow;
    _CREER_groupe_demployes_window = new YerothCreerGroupeDemployesWindow;
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr = new YerothDetailsDunGroupeDemployesWindow;
    _DETAILS_dun_groupe_de_PAIE_hr = new YerothDetailsDunGroupeDePaieWindow;
    _groupe_demployes_window = new YerothERPGroupesDemployesHRWindow;
    _GROUPES_DE_PAIE_Window = new YerothERPGROUPES_DE_PAIE_Window;
    _fournisseursWindow = new YerothERPFournisseursWindow;
    _clientsWindow = new YerothERPClientsWindow;
    _payerAuCompteClientWindow = new YerothPayerCompteClientWindow;
    _sortirListStocksWindow = new YerothSortirListStocksWindow;

    YerothERPWindows::move(*_mainWindow);
}

void YerothERPWindows::reinitialiseSqlTableModels()
{
    delete _tableModel_periodes_dappartenance_groupes_de_paie_hr;
    delete _tableModel_imprimantereseau_recus_petits;
    delete _tableModel_types_doperations_financieres;
    delete _tableModel_operations_comptables;
    delete _tableModel_comptes_bancaires;
    delete _tableModel_entreprise_info;
    delete _tableModel_historique_paiements;
    delete _tableModel_users;
    delete _tableModel_titres;
    delete _tableModel_localisations;
    delete _tableModel_departements_produits;
    delete _tableModel_categories;
    delete _tableModel_ligne_budgetaire;
    delete _tableModel_programmes_de_fidelite_clients;
    delete _tableModel_groupes_demployes_hr;
    delete _tableModel_groupes_de_paie_hr;
    delete _tableModel_clients;
    delete _tableModel_groupes_de_clients;
    delete _tableModel_fournisseurs;
    delete _tableModel_alertes;
    delete _tableModel_remises;
    delete _tableModel_conditions_alertes;
    delete _tableModel_courriers_alertes;
    delete _tableModel_stocks;
    delete _tableModel_achats;
    delete _tableModel_marchandises;
    delete _tableModel_services_completes;
    delete _tableModel_stocks_vendu;
    delete _tableModel_stocks_sorties;
    delete _tableModel_configurations;
    delete _tableModel_init_configurations;

    _tableModel_periodes_dappartenance_groupes_de_paie_hr =
        new YerothSqlTableModel(YerothDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);

    _tableModel_imprimantereseau_recus_petits =
        new YerothSqlTableModel(YerothDatabase::IMPRIMANTERESEAU_RECUS_PETITS);

    _tableModel_types_doperations_financieres =
        new YerothSqlTableModel(YerothDatabase::TYPE_DOPERATIONS_FINANCIERES);

    _tableModel_operations_comptables =
        new YerothSqlTableModel(YerothDatabase::OPERATIONS_COMPTABLES);

    _tableModel_comptes_bancaires =
        new YerothSqlTableModel(YerothDatabase::COMPTES_BANCAIRES);

    _tableModel_entreprise_info =
                    new YerothSqlTableModel(YerothDatabase::ENTREPRISE_INFO);
    _tableModel_historique_paiements =
                    new YerothSqlTableModel(YerothDatabase::PAIEMENTS);
    _tableModel_users = new YerothSqlTableModel(YerothDatabase::USERS);
    _tableModel_titres = new YerothSqlTableModel(YerothDatabase::TITRES);
    _tableModel_localisations =
                    new YerothSqlTableModel(YerothDatabase::LOCALISATIONS);
    _tableModel_departements_produits =
                    new YerothSqlTableModel(YerothDatabase::DEPARTEMENTS_PRODUITS);
    _tableModel_categories =
                    new YerothSqlTableModel(YerothDatabase::CATEGORIES);
    _tableModel_ligne_budgetaire =
                    new YerothSqlTableModel(YerothDatabase::LIGNES_BUDGETAIRES);
    _tableModel_programmes_de_fidelite_clients =
                    new YerothSqlTableModel(YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    _tableModel_groupes_demployes_hr = new YerothSqlTableModel(YerothDatabase::GROUPES_DEMPLOYES_hr);

    _tableModel_groupes_de_paie_hr = new YerothSqlTableModel(YerothDatabase::GROUPES_DE_PAIE_hr);

    _tableModel_clients = new YerothSqlTableModel(YerothDatabase::CLIENTS);

    _tableModel_groupes_de_clients =
                    new YerothSqlTableModel(YerothDatabase::GROUPES_DE_CLIENTS);
    _tableModel_fournisseurs =
                    new YerothSqlTableModel(YerothDatabase::FOURNISSEURS);
    _tableModel_alertes = new YerothSqlTableModel(YerothDatabase::ALERTES);
    _tableModel_remises = new YerothSqlTableModel(YerothDatabase::REMISES);
    _tableModel_conditions_alertes =
                    new YerothSqlTableModel(YerothDatabase::CONDITIONS_ALERTES);
    _tableModel_courriers_alertes =
                    new YerothSqlTableModel(YerothDatabase::COURRIERS_ALERTES);
    _tableModel_achats = new YerothSqlTableModel(YerothDatabase::ACHATS);
    _tableModel_stocks = new YerothSqlTableModel(YerothDatabase::STOCKS);
    _tableModel_services_completes =
                    new YerothSqlTableModel(YerothDatabase::SERVICES_COMPLETES);
    _tableModel_marchandises =
                    new YerothSqlTableModel(YerothDatabase::MARCHANDISES);
    _tableModel_stocks_vendu =
                    new YerothSqlTableModel(YerothDatabase::STOCKS_VENDU);
    _tableModel_stocks_sorties =
                    new YerothSqlTableModel(YerothDatabase::STOCKS_SORTIES);
    _tableModel_configurations =
                    new YerothSqlTableModel(YerothDatabase::CONFIGURATIONS);
    _tableModel_init_configurations =
                    new YerothSqlTableModel(YerothDatabase::INIT_CONFIGURATIONS);
}

void YerothERPWindows::setUser(YerothPOSUser *user)
{
    _user = user;

    if (0 != _user)
    {
        _user->create_user_personal_settings_file();
    }

    switch (_user->role())
    {

    case YerothUtils::ROLE_ADMINISTRATEUR:
        definirAdministrateur();
        break;

    case YerothUtils::ROLE_MANAGER:
        definirManager();
        break;

    case YerothUtils::ROLE_VENDEUR:
        definirVendeur();
        break;

    case YerothUtils::ROLE_GESTIONNAIREDESTOCK:
        definirGestionaireDesStocks();
        break;

    case YerothUtils::ROLE_MAGASINIER:
        definirMagasinier();
        break;

    case YerothUtils::ROLE_CAISSIER:
        definirCaissier();
        break;

    default:
        break;
    }
}


void YerothERPWindows::setupSqlTableModelFromName(const QString &aSqlTableName,
                                                  YerothSqlTableModel **
                                                  aYerothSqlTableModel)
{
    *aYerothSqlTableModel = new YerothSqlTableModel(aSqlTableName);

    _sqltablenameTOsqltablemodel.insert(aSqlTableName, *aYerothSqlTableModel);
}


void YerothERPWindows::setupConfiguration()
{
    _logger->log("setupConfiguration");
}

void YerothERPWindows::definirAdministrateur()
{
    _adminWindow->definirAdministrateur();
    _adminCreateWindow->definirAdministrateur();
    _adminListerWindow->definirAdministrateur();
    _adminDetailWindow->definirAdministrateur();
    _adminModifierWindow->definirAdministrateur();
}

void YerothERPWindows::definirMagasinier()
{
    _mainWindow->definirMagasinier();
    _sortirWindow->definirMagasinier();
    _statistiquesWindow->definirMagasinier();
    _mouvementsDeStocksWindow->definirMagasinier();
    _pdVenteWindow->definirMagasinier();
    _fournisseurDetailsWindow->definirMagasinier();
    _programmesDeFideliteClientsWindow->definirMagasinier();
    _detailsGroupeDeClientsWindow->definirMagasinier();
    _groupesDeClientsWindow->definirMagasinier();
    _groupesDunClientWindow->definirMagasinier();
    _clientsDetailWindow->definirMagasinier();
    _charges_financieresWindow->definirMagasinier();
    _charges_financieres_detailsWindow->definirMagasinier();
    _achatsDetailWindow->definirMagasinier();
    _detailWindow->definirMagasinier();
    _modifierWindow->definirMagasinier();
    _entrerWindow->definirMagasinier();
    _creerFournisseurWindow->definirMagasinier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerUnProgrammeDeFideliteClientsWindow->definirMagasinier();
    _creerGroupeDeClientsWindow->definirMagasinier();
    _creerCompteClientWindow->definirMagasinier();
    _modifierFournisseurWindow->definirMagasinier();
    _modifierCompteClientWindow->definirMagasinier();
    _achatsWindow->definirMagasinier();
    _stocksWindow->definirMagasinier();
    _marchandisesWindow->definirMagasinier();
    _listerAlertesWindow->definirMagasinier();
    _paiementsWindow->definirMagasinier();
    _ventesWindow->definirMagasinier();
    _groupes_DUN_EMPLOYE_window->definirMagasinier();
    _CREER_GROUPE_DE_PAIE_hr_window->definirMagasinier();
    _CREER_groupe_demployes_window->definirMagasinier();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirMagasinier();
    _DETAILS_dun_groupe_de_PAIE_hr->definirMagasinier();
    _groupe_demployes_window->definirMagasinier();
    _GROUPES_DE_PAIE_Window->definirMagasinier();
    _fournisseursWindow->definirMagasinier();
    _clientsWindow->definirMagasinier();
    _payerAuCompteClientWindow->definirMagasinier();
}

void YerothERPWindows::definirCaissier()
{
    _mainWindow->definirCaissier();
    _sortirWindow->definirCaissier();
    _statistiquesWindow->definirCaissier();
    _mouvementsDeStocksWindow->definirCaissier();
    _pdVenteWindow->definirCaissier();
    _fournisseurDetailsWindow->definirCaissier();
    _programmesDeFideliteClientsWindow->definirCaissier();
    _detailsGroupeDeClientsWindow->definirCaissier();
    _groupesDeClientsWindow->definirCaissier();
    _groupesDunClientWindow->definirCaissier();
    _clientsDetailWindow->definirCaissier();
    _charges_financieresWindow->definirCaissier();
    _charges_financieres_detailsWindow->definirCaissier();
    _achatsDetailWindow->definirCaissier();
    _detailWindow->definirCaissier();
    _modifierWindow->definirCaissier();
    _entrerWindow->definirCaissier();
    _creerFournisseurWindow->definirCaissier();
    _detailsDunProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerUnProgrammeDeFideliteClientsWindow->definirCaissier();
    _creerGroupeDeClientsWindow->definirCaissier();
    _creerCompteClientWindow->definirCaissier();
    _modifierFournisseurWindow->definirCaissier();
    _modifierCompteClientWindow->definirCaissier();
    _achatsWindow->definirCaissier();
    _stocksWindow->definirCaissier();
    _marchandisesWindow->definirCaissier();
    _listerAlertesWindow->definirCaissier();
    _paiementsWindow->definirCaissier();
    _ventesWindow->definirCaissier();
    _groupes_DUN_EMPLOYE_window->definirCaissier();
    _CREER_GROUPE_DE_PAIE_hr_window->definirCaissier();
    _CREER_groupe_demployes_window->definirCaissier();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirCaissier();
    _DETAILS_dun_groupe_de_PAIE_hr->definirCaissier();
    _groupe_demployes_window->definirCaissier();
    _GROUPES_DE_PAIE_Window->definirCaissier();
    _fournisseursWindow->definirCaissier();
    _clientsWindow->definirCaissier();
    _payerAuCompteClientWindow->definirCaissier();
}

void YerothERPWindows::definirManager()
{
    _mainWindow->definirManager();
    _sortirWindow->definirManager();
    _statistiquesWindow->definirManager();
    _mouvementsDeStocksWindow->definirManager();
    _pdVenteWindow->definirManager();
    _fournisseurDetailsWindow->definirManager();
    _programmesDeFideliteClientsWindow->definirManager();
    _detailsGroupeDeClientsWindow->definirManager();
    _groupesDeClientsWindow->definirManager();
    _groupesDunClientWindow->definirManager();
    _clientsDetailWindow->definirManager();
    _charges_financieresWindow->definirManager();
    _charges_financieres_detailsWindow->definirManager();
    _achatsDetailWindow->definirManager();
    _detailWindow->definirManager();
    _modifierWindow->definirManager();
    _entrerWindow->definirManager();
    _creerFournisseurWindow->definirManager();
    _detailsDunProgrammeDeFideliteClientsWindow->definirManager();
    _creerUnProgrammeDeFideliteClientsWindow->definirManager();
    _creerGroupeDeClientsWindow->definirManager();
    _creerCompteClientWindow->definirManager();
    _modifierFournisseurWindow->definirManager();
    _modifierCompteClientWindow->definirManager();
    _achatsWindow->definirManager();
    _stocksWindow->definirManager();
    _marchandisesWindow->definirManager();
    _comptabiliteWindow->definirManager();
    _listerAlertesWindow->definirManager();
    _paiementsWindow->definirManager();
    _ventesWindow->definirManager();
    _groupes_DUN_EMPLOYE_window->definirManager();
    _CREER_GROUPE_DE_PAIE_hr_window->definirManager();
    _CREER_groupe_demployes_window->definirManager();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirManager();
    _DETAILS_dun_groupe_de_PAIE_hr->definirManager();
    _groupe_demployes_window->definirManager();
    _GROUPES_DE_PAIE_Window->definirManager();
    _fournisseursWindow->definirManager();
    _clientsWindow->definirManager();
    _payerAuCompteClientWindow->definirManager();

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    _comptabiliteWindow->definirManager();
#endif

    _adminWindow->definirManager();
    _adminCreateWindow->definirManager();
    _adminListerWindow->definirManager();
    _adminDetailWindow->definirManager();
    _adminModifierWindow->definirManager();
}


void YerothERPWindows::definirVendeur()
{
    _mainWindow->definirVendeur();
    _sortirWindow->definirVendeur();
    _statistiquesWindow->definirVendeur();
    _mouvementsDeStocksWindow->definirVendeur();
    _pdVenteWindow->definirVendeur();
    _fournisseurDetailsWindow->definirVendeur();
    _programmesDeFideliteClientsWindow->definirVendeur();
    _detailsGroupeDeClientsWindow->definirVendeur();
    _groupesDeClientsWindow->definirVendeur();
    _groupesDunClientWindow->definirVendeur();
    _clientsDetailWindow->definirVendeur();
    _charges_financieresWindow->definirVendeur();
    _charges_financieres_detailsWindow->definirVendeur();
    _achatsDetailWindow->definirVendeur();
    _detailWindow->definirVendeur();
    _modifierWindow->definirVendeur();
    _entrerWindow->definirVendeur();
    _creerFournisseurWindow->definirVendeur();
    _detailsDunProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerUnProgrammeDeFideliteClientsWindow->definirVendeur();
    _creerGroupeDeClientsWindow->definirVendeur();
    _creerCompteClientWindow->definirVendeur();
    _modifierFournisseurWindow->definirVendeur();
    _modifierCompteClientWindow->definirVendeur();
    _achatsWindow->definirVendeur();
    _stocksWindow->definirVendeur();
    _marchandisesWindow->definirVendeur();
    _listerAlertesWindow->definirVendeur();
    _paiementsWindow->definirVendeur();
    _ventesWindow->definirVendeur();
    _groupes_DUN_EMPLOYE_window->definirVendeur();
    _CREER_GROUPE_DE_PAIE_hr_window->definirVendeur();
    _CREER_groupe_demployes_window->definirVendeur();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirVendeur();
    _DETAILS_dun_groupe_de_PAIE_hr->definirVendeur();
    _groupe_demployes_window->definirVendeur();
    _GROUPES_DE_PAIE_Window->definirVendeur();
    _fournisseursWindow->definirVendeur();
    _clientsWindow->definirVendeur();
    _payerAuCompteClientWindow->definirVendeur();

    _adminWindow->definirVendeur();
    _adminCreateWindow->definirVendeur();
    _adminListerWindow->definirVendeur();
    _adminDetailWindow->definirVendeur();
    _adminModifierWindow->definirVendeur();
}


void YerothERPWindows::definirGestionaireDesStocks()
{
    _mainWindow->definirGestionaireDesStocks();
    _sortirWindow->definirGestionaireDesStocks();
    _statistiquesWindow->definirGestionaireDesStocks();
    _mouvementsDeStocksWindow->definirGestionaireDesStocks();
    _pdVenteWindow->definirGestionaireDesStocks();
    _fournisseurDetailsWindow->definirGestionaireDesStocks();
    _programmesDeFideliteClientsWindow->definirGestionaireDesStocks();
    _detailsGroupeDeClientsWindow->definirGestionaireDesStocks();
    _groupesDeClientsWindow->definirGestionaireDesStocks();
    _groupesDunClientWindow->definirGestionaireDesStocks();
    _clientsDetailWindow->definirGestionaireDesStocks();
    _charges_financieresWindow->definirGestionaireDesStocks();
    _charges_financieres_detailsWindow->definirGestionaireDesStocks();
    _achatsDetailWindow->definirGestionaireDesStocks();
    _detailWindow->definirGestionaireDesStocks();
    _modifierWindow->definirGestionaireDesStocks();
    _entrerWindow->definirGestionaireDesStocks();
    _creerFournisseurWindow->definirGestionaireDesStocks();
    _detailsDunProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerUnProgrammeDeFideliteClientsWindow->definirGestionaireDesStocks();
    _creerGroupeDeClientsWindow->definirGestionaireDesStocks();
    _creerCompteClientWindow->definirGestionaireDesStocks();
    _modifierFournisseurWindow->definirGestionaireDesStocks();
    _modifierCompteClientWindow->definirGestionaireDesStocks();
    _achatsWindow->definirGestionaireDesStocks();
    _stocksWindow->definirGestionaireDesStocks();
    _marchandisesWindow->definirGestionaireDesStocks();
    _listerAlertesWindow->definirGestionaireDesStocks();
    _paiementsWindow->definirGestionaireDesStocks();
    _ventesWindow->definirGestionaireDesStocks();
    _groupes_DUN_EMPLOYE_window->definirGestionaireDesStocks();
    _CREER_GROUPE_DE_PAIE_hr_window->definirGestionaireDesStocks();
    _CREER_groupe_demployes_window->definirGestionaireDesStocks();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirGestionaireDesStocks();
    _DETAILS_dun_groupe_de_PAIE_hr->definirGestionaireDesStocks();
    _groupe_demployes_window->definirGestionaireDesStocks();
    _GROUPES_DE_PAIE_Window->definirGestionaireDesStocks();
    _fournisseursWindow->definirGestionaireDesStocks();
    _clientsWindow->definirGestionaireDesStocks();
    _payerAuCompteClientWindow->definirGestionaireDesStocks();

    _adminWindow->definirGestionaireDesStocks();
    _adminCreateWindow->definirGestionaireDesStocks();
    _adminListerWindow->definirGestionaireDesStocks();
    _adminDetailWindow->definirGestionaireDesStocks();
    _adminModifierWindow->definirGestionaireDesStocks();
}


/**
 * We only need to call the method 'definirPasDe Role'
 * on the _mainWindow instance because all other widget
 * instances are not yet instantiated.
 */
void YerothERPWindows::definirPasDeRole()
{
    _mainWindow->definirPasDeRole();
    _sortirWindow->definirPasDeRole();
    _statistiquesWindow->definirPasDeRole();
    _mouvementsDeStocksWindow->definirPasDeRole();
    _pdVenteWindow->definirPasDeRole();
    _fournisseurDetailsWindow->definirPasDeRole();
    _programmesDeFideliteClientsWindow->definirPasDeRole();
    _detailsGroupeDeClientsWindow->definirPasDeRole();
    _groupesDeClientsWindow->definirPasDeRole();
    _groupesDunClientWindow->definirPasDeRole();
    _clientsDetailWindow->definirPasDeRole();
    _charges_financieresWindow->definirPasDeRole();
    _charges_financieres_detailsWindow->definirPasDeRole();
    _achatsDetailWindow->definirPasDeRole();
    _detailWindow->definirPasDeRole();
    _modifierWindow->definirPasDeRole();
    _entrerWindow->definirPasDeRole();
    _creerFournisseurWindow->definirPasDeRole();
    _detailsDunProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerUnProgrammeDeFideliteClientsWindow->definirPasDeRole();
    _creerGroupeDeClientsWindow->definirPasDeRole();
    _creerCompteClientWindow->definirPasDeRole();
    _modifierFournisseurWindow->definirPasDeRole();
    _modifierCompteClientWindow->definirPasDeRole();
    _achatsWindow->definirPasDeRole();
    _stocksWindow->definirPasDeRole();
    _marchandisesWindow->definirPasDeRole();
    _comptabiliteWindow->definirPasDeRole();
    _listerAlertesWindow->definirPasDeRole();
    _paiementsWindow->definirPasDeRole();
    _ventesWindow->definirPasDeRole();
    _groupes_DUN_EMPLOYE_window->definirPasDeRole();
    _CREER_GROUPE_DE_PAIE_hr_window->definirPasDeRole();
    _CREER_groupe_demployes_window->definirPasDeRole();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->definirPasDeRole();
    _DETAILS_dun_groupe_de_PAIE_hr->definirPasDeRole();
    _groupe_demployes_window->definirPasDeRole();
    _GROUPES_DE_PAIE_Window->definirPasDeRole();
    _fournisseursWindow->definirPasDeRole();
    _clientsWindow->definirPasDeRole();
    _payerAuCompteClientWindow->definirPasDeRole();

    _adminWindow->definirPasDeRole();
    _adminCreateWindow->definirPasDeRole();
    _adminListerWindow->definirPasDeRole();
    _adminDetailWindow->definirPasDeRole();
    _adminModifierWindow->definirPasDeRole();
}

void YerothERPWindows::hideAllWindows()
{
    _sortirWindow->rendreInvisible();
    _transactionsDunFournisseurWindow->rendreInvisible();
    _transactionsDunClientWindow->rendreInvisible();
    _historiqueDuStockWindow->rendreInvisible();
    _statistiquesWindow->rendreInvisible();
    _mouvementsDeStocksWindow->rendreInvisible();
    _pdVenteWindow->rendreInvisible();
    _pdVenteListStocksWindow->rendreInvisible();
    _impressionDeDocumentDialog->rendreInvisible();
    _pdVenteMethodePaiementCarteCreditDialog->rendreInvisible();
    _pdVenteMethodePaiementComptantEntreeDialog->rendreInvisible();
    _pdVenteMethodePaiementDialog->rendreInvisible();
    _sortirListStocksWindow->rendreInvisible();
    _fournisseurDetailsWindow->rendreInvisible();
    _programmesDeFideliteClientsWindow->rendreInvisible();
    _detailsGroupeDeClientsWindow->rendreInvisible();
    _groupesDeClientsWindow->rendreInvisible();
    _groupesDunClientWindow->rendreInvisible();
    _clientsDetailWindow->rendreInvisible();
    _charges_financieresWindow->rendreInvisible();
    _charges_financieres_detailsWindow->rendreInvisible();
    _achatsDetailWindow->rendreInvisible();
    _detailWindow->rendreInvisible();
    _creerFournisseurWindow->rendreInvisible();
    _detailsDunProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerUnProgrammeDeFideliteClientsWindow->rendreInvisible();
    _creerGroupeDeClientsWindow->rendreInvisible();
    _creerCompteClientWindow->rendreInvisible();
    _modifierFournisseurWindow->rendreInvisible();
    _modifierCompteClientWindow->rendreInvisible();
    _achatsWindow->rendreInvisible();
    _stocksWindow->rendreInvisible();
    _marchandisesWindow->rendreInvisible();
    _comptabiliteWindow->rendreInvisible();
    _listerAlertesWindow->rendreInvisible();
    _paiementsWindow->rendreInvisible();
    _ventesWindow->rendreInvisible();
    _groupes_DUN_EMPLOYE_window->rendreInvisible();
    _CREER_GROUPE_DE_PAIE_hr_window->rendreInvisible();
    _CREER_groupe_demployes_window->rendreInvisible();
    _DETAILS_DUN_GROUPE_DEMPLOYES_hr->rendreInvisible();
    _DETAILS_dun_groupe_de_PAIE_hr->rendreInvisible();
    _groupe_demployes_window->rendreInvisible();
    _GROUPES_DE_PAIE_Window->rendreInvisible();
    _fournisseursWindow->rendreInvisible();
    _clientsWindow->rendreInvisible();
    _payerAuCompteClientWindow->rendreInvisible();
    _modifierWindow->rendreInvisible();
    _entrerWindow->rendreInvisible();
    _adminWindow->rendreInvisible();
    _adminCreateWindow->rendreInvisible();
    _adminListerWindow->rendreInvisible();
    _adminDetailWindow->rendreInvisible();
    _adminModifierWindow->rendreInvisible();

    _mainWindow->rendreInvisible();
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr()
{
    _tableModel_periodes_dappartenance_groupes_de_paie_hr->resetFilter("src/yeroth-erp-windows.cpp", 948);
    return *_tableModel_periodes_dappartenance_groupes_de_paie_hr;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_imprimantereseau_recus_petits()
{
    _tableModel_imprimantereseau_recus_petits->resetFilter("src/yeroth-erp-windows.cpp", 905);
    return *_tableModel_imprimantereseau_recus_petits;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_types_doperations_financieres()
{
    _tableModel_types_doperations_financieres->resetFilter("src/yeroth-erp-windows.cpp", 970);
    return *_tableModel_types_doperations_financieres;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_operations_comptables()
{
    _tableModel_operations_comptables->resetFilter("src/yeroth-erp-windows.cpp", 912);
    return *_tableModel_operations_comptables;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_comptes_bancaires()
{
    _tableModel_comptes_bancaires->resetFilter("src/yeroth-erp-windows.cpp", 926);
    return *_tableModel_comptes_bancaires;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_entreprise_info()
{
    _tableModel_entreprise_info->resetFilter("src/yeroth-erp-windows.cpp", 933);
    return *_tableModel_entreprise_info;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_paiements()
{
    _tableModel_historique_paiements->resetFilter("src/yeroth-erp-windows.cpp", 940);
    return *_tableModel_historique_paiements;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_users()
{
    _tableModel_users->resetFilter("src/yeroth-erp-windows.cpp", 947);
    return *_tableModel_users;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_titres()
{
    _tableModel_titres->resetFilter("src/yeroth-erp-windows.cpp", 979);
    return *_tableModel_titres;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_localisations()
{
    _tableModel_localisations->resetFilter("src/yeroth-erp-windows.cpp", 986);
    return *_tableModel_localisations;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_departements_produits()
{
    _tableModel_departements_produits->resetFilter("src/yeroth-erp-windows.cpp", 992);
    return *_tableModel_departements_produits;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_categories()
{
    _tableModel_categories->resetFilter("src/yeroth-erp-windows.cpp", 973);
    return *_tableModel_categories;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_lignes_budgetaires()
{
	_tableModel_ligne_budgetaire->resetFilter("src/yeroth-erp-windows.cpp", 979);
    return *_tableModel_ligne_budgetaire;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_alertes()
{
    _tableModel_alertes->resetFilter("src/yeroth-erp-windows.cpp", 985);
    return *_tableModel_alertes;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_remises()
{
    _tableModel_remises->resetFilter("src/yeroth-erp-windows.cpp", 991);
    return *_tableModel_remises;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_conditions_alertes()
{
    _tableModel_conditions_alertes->resetFilter("src/yeroth-erp-windows.cpp", 998);
    return *_tableModel_conditions_alertes;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_courriers_alertes()
{
    _tableModel_courriers_alertes->resetFilter("src/yeroth-erp-windows.cpp", 1004);
    return *_tableModel_courriers_alertes;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_programmes_de_fidelite_clients()
{
    _tableModel_programmes_de_fidelite_clients->resetFilter("src/yeroth-erp-windows.cpp", 1010);
    return *_tableModel_programmes_de_fidelite_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_groupes_demployes_hr()
{
	_tableModel_groupes_demployes_hr->resetFilter("src/yeroth-erp-windows.cpp", 1016);
    return *_tableModel_groupes_demployes_hr;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_groupes_de_paie_hr()
{
	_tableModel_groupes_de_paie_hr->resetFilter("src/yeroth-erp-windows.cpp", 1022);
    return *_tableModel_groupes_de_paie_hr;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_clients()
{
    _tableModel_clients->resetFilter("src/yeroth-erp-windows.cpp", 1028);
    return *_tableModel_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_groupes_de_clients()
{
    _tableModel_groupes_de_clients->resetFilter("src/yeroth-erp-windows.cpp", 1034);
    return *_tableModel_groupes_de_clients;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_fournisseurs()
{
    _tableModel_fournisseurs->resetFilter("src/yeroth-erp-windows.cpp", 1040);
    return *_tableModel_fournisseurs;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_charges_financieres()
{
    _tableModel_charges_financieres->resetFilter("src/yeroth-erp-windows.cpp", 1047);
    return *_tableModel_charges_financieres;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_achats()
{
    _tableModel_achats->resetFilter("src/yeroth-erp-windows.cpp", 1054);
    return *_tableModel_achats;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks()
{
    _tableModel_stocks->resetFilter("src/yeroth-erp-windows.cpp", 1061);
    return *_tableModel_stocks;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_services_completes()
{
    _tableModel_services_completes->resetFilter("src/yeroth-erp-windows.cpp", 1068);
    return *_tableModel_services_completes;
}


YerothSqlTableModel &YerothERPWindows::getSqlTableModel_marchandises()
{
    _tableModel_marchandises->resetFilter("src/yeroth-erp-windows.cpp", 1075);
    return *_tableModel_marchandises;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks_vendu()
{
    _tableModel_stocks_vendu->resetFilter("src/yeroth-erp-windows.cpp", 1081);
    return *_tableModel_stocks_vendu;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_stocks_sorties()
{
    _tableModel_stocks_sorties->resetFilter("src/yeroth-erp-windows.cpp", 1087);
    return *_tableModel_stocks_sorties;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_configurations()
{
    _tableModel_configurations->resetFilter("src/yeroth-erp-windows.cpp", 1093);
    return *_tableModel_configurations;
}

YerothSqlTableModel &YerothERPWindows::getSqlTableModel_init_configurations()
{
    _tableModel_init_configurations->resetFilter("src/yeroth-erp-windows.cpp", 1099);
    return *_tableModel_init_configurations;
}

void YerothERPWindows::setCurrentPosition(int xPos, int yPos)
{
    if (_currentPosition)
    {
        _currentPosition->setX(xPos);
        _currentPosition->setY(yPos);
    }
    else
    {
        _logger->log("YerothWindows::setCurrentPosition(int, int)",
                     "_currentPosition is NULL!");
    }
}

bool YerothERPWindows::move(QMainWindow &aWindow)
{
    if (0 != _currentPosition)
    {
        //qDebug() << "pos: " << *_currentPosition;
        aWindow.move(*_currentPosition);
        return true;
    }

    //qDebug() << "no move: ";
    return false;
}


/**
 * !!! DO NOT MODIFY AT ALL !!!
 *
 *  !!! THIS IS A VERY IMPORTANT PIECE OF CODE !!!
 */
int YerothERPWindows::NEXT_TABLE_ID_MYSQL_FOR_STOCKS()
{
    int max_id = 0;

    QString MAX_ID_QUERY_FOR_STOCKS(QString("SELECT IFNULL(MAX(id), 0)"
                                            " FROM"
                                            " ("
                                            " SELECT id FROM %1"
                                            " UNION ALL"
                                            " SELECT %2 as id FROM %3"
                                            " UNION ALL"
                                            " SELECT %4 as id FROM %5"
                                            " ) a").
                                    arg(YerothDatabase::STOCKS,
                                        YerothDatabaseTableColumn::STOCKS_ID,
                                        YerothDatabase::STOCKS_VENDU,
                                        YerothDatabaseTableColumn::STOCKS_ID,
                                        YerothDatabase::SERVICES_COMPLETES));

//      QDEBUG_STRING_OUTPUT_2("MAX_ID_QUERY_FOR_STOCKS", MAX_ID_QUERY_FOR_STOCKS);

    QSqlQuery aQuery;

    int querySize = YerothUtils::execQuery(aQuery, MAX_ID_QUERY_FOR_STOCKS);

    if (querySize > 0 && aQuery.next())
    {
        max_id = aQuery.value(0).toInt();

        max_id += 1;
    }

    return max_id;
}


int YerothERPWindows::getNextIdFromTable(const QString &tableName)
{
    //qDebug() << "[YerothUtils::getLastIdFromTable()] : next id from table '" << tableName << "'";
    QString
    strQuery(QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 0, 1").arg
             (YerothDatabaseTableColumn::ID, tableName,
              YerothDatabaseTableColumn::ID));

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    if (query.last())
    {
        int lastId = query.value(0).toInt();
        ++lastId;
//        qDebug() << "\t next id 1: " << lastId;
        return lastId;
    }

    return 0;
}
