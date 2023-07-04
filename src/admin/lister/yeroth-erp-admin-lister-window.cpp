/**
 * yeroth-erp-admin-lister-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-admin-lister-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#ifdef YEROTH_SERVER

	#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"

#endif

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/admin/yeroth-erp-admin-search-form.hpp"

#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtGui/QStandardItemModel>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>


YerothAdminListerWindow::YerothAdminListerWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ lister")),
 _alertCurrentlyFiltered(false),
 _productDepartmentCurrentlyFiltered(false),
 _categoryCurrentlyFiltered(false),
 _LIGNE_BUDGETAIRE_CurrentlyFiltered(false),
 _bankAccountCurrentlyFiltered(false),
 _userCurrentlyFiltered(false),
 _siteCurrentlyFiltered(false),
 _charges_financieres_CurrentlyFiltered(false),
 _discountCurrentlyFiltered(false),
 _pushButton_admin_rechercher_font(0),
 _logger(new YerothLogger("YerothAdminListerWindow")),
 _adminSearchForm(0),
 _curSearchSqlTableModel(0),
 _lastItemSelectedForModification(0)
{
	//SETS A PREFIX FOR printed pdf document of this page
	_output_print_pdf_latexFileNamePrefix = QObject::tr("ADMINISTRATION_LISTER_PDF");

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_alerte);


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
        << action_parametrer_les_impressions
        << actionAfficherPDF;


    setup_select_configure_dbcolumn(YerothDatabase::ALERTES);


    lineEdit_nombre_de_resultats->setYerothEnabled(false);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _pushButton_admin_rechercher_font =
                    new QFont(pushButton_admin_rechercher->font());

    _adminSearchForm = new YerothAdminSearchForm(_allWindows, this);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    tableView_lister_utilisateur->setSqlTableName(&YerothDatabase::USERS);

    tableView_lister_localisation
		->setSqlTableName(&YerothDatabase::LOCALISATIONS);

    tableView_lister_departements_produits
		->setSqlTableName(&YerothDatabase::DEPARTEMENTS_PRODUITS);

    tableView_lister_categorie->setSqlTableName(&YerothDatabase::CATEGORIES);

    tableView_lister_LIGNE_BUDGETAIRE->setSqlTableName(&YerothDatabase::LIGNES_BUDGETAIRES);

    tableView_lister_compte_bancaire->setSqlTableName(&YerothDatabase::COMPTES_BANCAIRES);

    tableView_lister_remise->setSqlTableName(&YerothDatabase::REMISES);

    tableView_lister_alerte->setSqlTableName(&YerothDatabase::ALERTES);


    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister"));


    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_creer->enable(this, SLOT(creer()));

    pushButton_modifier->enable(this, SLOT(modifier()));

    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));

    pushButton_supprimer->enable(this, SLOT(supprimer()));

    pushButton_admin_reinitialiser->enable(this, SLOT(reinitialiser()));

    pushButton_admin_rechercher->enable(this, SLOT(rechercher()));

    connect(actionAfficher, SIGNAL(triggered()), this,
            SLOT(afficher_au_detail()));


    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible,
    		SIGNAL(triggered()),
			this,
			SLOT(selectionner_champs_db_visibles()));


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));


    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));

    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));


    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));

    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));

    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier()));

    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));

    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

    connect(tabWidget_lister,
    		SIGNAL(currentChanged(int)),
			this,
            SLOT(handleCurrentChanged(int)));

    connect(tableView_lister_utilisateur, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_localisation, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_departements_produits,
            SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_categorie, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_LIGNE_BUDGETAIRE, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_compte_bancaire, SIGNAL(clicked(QModelIndex)),
            this, SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_alerte, SIGNAL(clicked(QModelIndex)), this,
            SLOT(handleItemModification(QModelIndex)));

    connect(tableView_lister_utilisateur,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_utilisateur()));

    connect(tableView_lister_localisation,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_localisation()));

    connect(tableView_lister_departements_produits,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_departements_de_produits()));

    connect(tableView_lister_categorie,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_categorie()));

    connect(tableView_lister_LIGNE_BUDGETAIRE,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_LIGNE_BUDGETAIRE()));

    connect(tableView_lister_compte_bancaire,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_compte_bancaire()));

    connect(tableView_lister_remise,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_remise()));

    connect(tableView_lister_alerte,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_detail_alerte()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));

    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));

    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));

    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));

    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));

    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
}


YerothAdminListerWindow::~YerothAdminListerWindow()
{
    delete _pushButton_admin_rechercher_font;
    delete _logger;
    delete _adminSearchForm;
}


void YerothAdminListerWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
}


void YerothAdminListerWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
    														   _curSearchSqlTableModel)
}


void YerothAdminListerWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
    														   _curSearchSqlTableModel)
}


void YerothAdminListerWindow::self_reset_view(int currentIndex)
{
    rendreInvisible();
    rendreVisible(currentIndex);
}


void YerothAdminListerWindow::rendreVisible(unsigned selectedSujetAction)
{
    retranslateUi(this);

	_windowName = _windowName_WITH_NO_MAINTENANCE;

    switch (selectedSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        if (false == isUserCurrentlyFiltered())
        {
            lister_utilisateur();
        }
        break;

    case SUJET_ACTION_LOCALISATION:
        if (false == isSiteCurrentlyFiltered())
        {
            lister_localisation();
        }
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        if (false == isProductDepartmentCurrentlyFiltered())
        {
            lister_departements_de_produits();
        }
        break;

    case SUJET_ACTION_CATEGORIE:
        if (false == isCategoryCurrentlyFiltered())
        {
            lister_categorie();
        }
        break;

    case SUJET_ACTION_ligne_budgetaire:
        if (false == is_LIGNE_BUDGETAIRE_CurrentlyFiltered())
        {
        	lister_LIGNE_BUDGETAIRE();
        }
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        if (false == isBankAccountCurrentlyFiltered())
        {
            lister_compte_bancaire();
        }
        break;

    case SUJET_ACTION_ALERTE:
        if (false == isAlertCurrentlyFiltered())
        {
            lister_alerte();
        }
        break;

    case SUJET_ACTION_REMISE:
        if (false == isDiscountCurrentlyFiltered())
        {
            lister_remise();
        }
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        if (false == isCharge_financieres_CurrentlyFiltered())
        {
        	LISTER_CHARGES_FINANCIERES();
        }
        break;

    default:
        break;
    }

    tabWidget_lister->setCurrentIndex(selectedSujetAction);

    setVisible(true);
}


void YerothAdminListerWindow::rechercher()
{
    _adminSearchForm->rendreVisible(tabWidget_lister->currentIndex());
}


void YerothAdminListerWindow::reinitialiser()
{
    _logger->log("reinitialiser");

    _adminSearchForm->reinitialiser();
    _adminSearchForm->rendreInvisible();

    if (_curSearchSqlTableModel)
    {
        _curSearchSqlTableModel->resetFilter("src/admin/lister/yeroth-erp-admin-lister-window.cpp", 391);
        _curSearchSqlTableModel->easySelect("src/admin/lister/yeroth-erp-admin-lister-window.cpp", 392);
    }

    lister_utilisateur();

    lister_localisation();

    lister_categorie();

    lister_LIGNE_BUDGETAIRE();

    lister_compte_bancaire();

    lister_departements_de_produits();

    lister_alerte();

    lister_remise();

    setCurSearchSqlTableModel(0);

    set_admin_rechercher_font();
}


void YerothAdminListerWindow::set_admin_rechercher_font()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	MACRO_SET_ADMIN_RECHERCHER_FONT(_userCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_utilisateur"));

		break;

    case SUJET_ACTION_LOCALISATION:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_localisations();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_siteCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_localisation"));

        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_departements_produits();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_productDepartmentCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_departements_de_produits"));

		break;

    case SUJET_ACTION_CATEGORIE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_categorie);
    	setup_select_configure_dbcolumn(YerothDatabase::CATEGORIES);
    	tableView_lister_categorie->_currentViewWindow = this;

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_categories();

    	MACRO_SET_ADMIN_RECHERCHER_FONT(_categoryCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_categorie"));

        break;

    case SUJET_ACTION_ligne_budgetaire:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_LIGNE_BUDGETAIRE_CurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_LIGNE_BUDGETAIRE"));

        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_bankAccountCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_compte_bancaire"));

        break;

    case SUJET_ACTION_REMISE:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_remises();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_discountCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_remise"));

        break;

    case SUJET_ACTION_ALERTE:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_alertes();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_alertCurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_alerte"));

        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_charges_financieres();
        MACRO_SET_ADMIN_RECHERCHER_FONT(_charges_financieres_CurrentlyFiltered)
		setWindowTitle(_LISTER_tab_TO_tabTitle.value("LISTER_CHARGES_FINANCIERES"));

        break;

    default:
    	_curSearchSqlTableModel = 0;
    	setWindowTitle(_windowName_WITH_NO_MAINTENANCE);
        break;
    }

    pushButton_admin_rechercher->setFont(*_pushButton_admin_rechercher_font);
}


int YerothAdminListerWindow::
		LIST_SHOW_TABLE_VIEW_WITH_PAGINATION
			(YerothTableViewWITHpagination  &a_table_view_to_list_show,
			 YerothSqlTableModel 			&aSqlTableModel)
{
	//I UPDATE A CURRENT DATABASE COLUMN SECTION DIALOG
	_selectExportDBQDialog =
			GET_ERP_GENERIC_SELECTDBFIELD_DIALOG(aSqlTableModel.sqlTableName());

    a_table_view_to_list_show
		.queryYerothTableViewCurrentPageContentRow(aSqlTableModel);

    tableView_show_or_hide_columns(a_table_view_to_list_show);

    int rowCount = a_table_view_to_list_show.rowCount();

    lineEdit_nombre_de_resultats->setText(GET_NUM_STRING(rowCount));

    return rowCount;
}


YerothTableViewWITHpagination *YerothAdminListerWindow::GET_CURRENT_TABLEVIEW()
{
	YerothTableViewWITHpagination *result = 0;

	switch (tabWidget_lister->currentIndex())
	    {
	    case SUJET_ACTION_COMPTE_UTILISATEUR:
	    	result = tableView_lister_utilisateur;
	        break;

	    case SUJET_ACTION_LOCALISATION:
	    	result = tableView_lister_localisation;
	        break;

	    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
	    	result = tableView_lister_departements_produits;
	        break;

	    case SUJET_ACTION_CATEGORIE:
	    	result = tableView_lister_categorie;
	        break;

	    case SUJET_ACTION_ligne_budgetaire:
	    	result = tableView_lister_LIGNE_BUDGETAIRE;
	        break;

	    case SUJET_ACTION_COMPTE_BANCAIRE:
	    	result = tableView_lister_compte_bancaire;
	        break;

	    case SUJET_ACTION_ALERTE:
	    	result = tableView_lister_alerte;
	        break;

	    case SUJET_ACTION_REMISE:
	    	result = tableView_lister_remise;
	        break;

	    case SUJET_ACTION_CHARGE_FINANCIERE:
	    	result = tableView_lister_charges_financieres;
	        break;

	    default:
	        break;
	    }

	return result;
}


void YerothAdminListerWindow::SETUP_PRINT()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	tableView_lister_utilisateur->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_utilisateur);

    	setup_select_configure_dbcolumn(YerothDatabase::USERS);

    	{
    		_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.clear();

    		if (!_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.contains(YerothDatabaseTableColumn::MOT_PASSE))
    		{
        		_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
    				.append(YerothDatabaseTableColumn::MOT_PASSE);
    		}


    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    				<< YerothDatabaseTableColumn::PRENOM
    				<< YerothDatabaseTableColumn::NOM
    				<< YerothDatabaseTableColumn::TITRE
    				<< YerothDatabaseTableColumn::ROLE
    				<< YerothDatabaseTableColumn::LOCALISATION;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_users();
        break;

    case SUJET_ACTION_LOCALISATION:
    	tableView_lister_localisation->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_localisation);

    	setup_select_configure_dbcolumn(YerothDatabase::LOCALISATIONS);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    				<< YerothDatabaseTableColumn::NUMERO_UNIQUE
    	            << YerothDatabaseTableColumn::ADRESSE_IP
    				<< YerothDatabaseTableColumn::NOM_LOCALISATION
    				<< YerothDatabaseTableColumn::QUARTIER
    				<< YerothDatabaseTableColumn::VILLE
    				<< YerothDatabaseTableColumn::NUMERO_TELEPHONE_1;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_localisations();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
    	tableView_lister_departements_produits->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_departements_produits);

    	setup_select_configure_dbcolumn(YerothDatabase::DEPARTEMENTS_PRODUITS);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
    	            << YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_departements_produits();
        break;

    case SUJET_ACTION_CATEGORIE:
    	tableView_lister_categorie->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_categorie);

    	setup_select_configure_dbcolumn(YerothDatabase::CATEGORIES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::NOM_CATEGORIE
    	            << YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
    	            << YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_categories();
        break;

    case SUJET_ACTION_ligne_budgetaire:
    	tableView_lister_LIGNE_BUDGETAIRE->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_LIGNE_BUDGETAIRE);

    	setup_select_configure_dbcolumn(YerothDatabase::LIGNES_BUDGETAIRES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire
    	            << YerothDatabaseTableColumn::montant_de_la_ligne_budgetaire
    	            << YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE
    				<< YerothDatabaseTableColumn::DESCRIPTION_ligne_budgetaire;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
    	tableView_lister_compte_bancaire->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_compte_bancaire);

    	setup_select_configure_dbcolumn(YerothDatabase::COMPTES_BANCAIRES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE
    	            << YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE
    	            << YerothDatabaseTableColumn::INSTITUT_BANCAIRE
    				<< YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
        break;

    case SUJET_ACTION_ALERTE:
    	tableView_lister_alerte->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_alerte);

    	setup_select_configure_dbcolumn(YerothDatabase::ALERTES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::STOCKS_ID
    	            << YerothDatabaseTableColumn::DESIGNATION_ALERTE
    				<< YerothDatabaseTableColumn::DESIGNATION
    				<< YerothDatabaseTableColumn::DATE_DEBUT
    				<< YerothDatabaseTableColumn::DATE_FIN
    				<< YerothDatabaseTableColumn::DESTINATAIRE
    				<< YerothDatabaseTableColumn::DATE_CREATION
    				<< YerothDatabaseTableColumn::ALERTE_RESOLUE;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_alertes();
        break;

    case SUJET_ACTION_REMISE:
    	tableView_lister_remise->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_remise);

    	setup_select_configure_dbcolumn(YerothDatabase::REMISES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    	            << YerothDatabaseTableColumn::ID
    	            << YerothDatabaseTableColumn::STOCKS_ID
    	            << YerothDatabaseTableColumn::DESIGNATION_ALERTE
    				<< YerothDatabaseTableColumn::DESIGNATION
    				<< YerothDatabaseTableColumn::DATE_DEBUT
    				<< YerothDatabaseTableColumn::DATE_FIN
    				<< YerothDatabaseTableColumn::DESTINATAIRE
    				<< YerothDatabaseTableColumn::DATE_CREATION
    				<< YerothDatabaseTableColumn::ALERTE_RESOLUE;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_remises();
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	tableView_lister_charges_financieres->_currentViewWindow = this;

    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_charges_financieres);

    	setup_select_configure_dbcolumn(YerothDatabase::CHARGES_FINANCIERES);

    	{
    	    _visibleDBColumnNameStrList.clear();

    	    _visibleDBColumnNameStrList
    				<< YerothDatabaseTableColumn::DESIGNATION
					<< YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR
					<< YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT
    				<< YerothDatabaseTableColumn::PRIX_DACHAT
    				<< YerothDatabaseTableColumn::PRIX_UNITAIRE
    				<< YerothDatabaseTableColumn::DATE_DE_RECEPTION
    				<< YerothDatabaseTableColumn::LOCALISATION;
    	}

    	_curSearchSqlTableModel = &_allWindows->getSqlTableModel_charges_financieres();
        break;

    default:
        break;
    }
}


void YerothAdminListerWindow::
	LISTER_CHARGES_FINANCIERES(YerothSqlTableModel *aSqlTableModel /* = 0 */)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ CHARGES FINANCIÈRES"));

    //In case, for e.g. there is filtering applied to aSqlTableModel
    if (0 != aSqlTableModel &&
        true == YerothUtils::isEqualCaseInsensitive(YerothDatabase::CHARGES_FINANCIERES,
                                                    aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_charges_financieres,
    										 *aSqlTableModel);
    }
    else
    {
        setUserCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_charges_financieres();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_charges_financieres,
    										 sqlTableModel);
    }

    _LISTER_tab_TO_tabTitle.insert("LISTER_CHARGES_FINANCIERES", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("LISTER_CHARGES_FINANCIERES"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_charges_financieres->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_utilisateur(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ utilisateurs"));

    //In case, for e.g. there is filtering applied to aSqlTableModel
    if (0 != aSqlTableModel &&
        true == YerothUtils::isEqualCaseInsensitive(YerothDatabase::USERS,
                                                    aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_utilisateur,
    										 *aSqlTableModel);
    }
    else
    {
        setUserCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_users();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_utilisateur,
    										 sqlTableModel);
    }

    _LISTER_tab_TO_tabTitle.insert("lister_utilisateur", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_utilisateur"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_utilisateur->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_localisation(YerothSqlTableModel *
                                                  aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ localisations"));

    if (aSqlTableModel &&
    	YerothUtils::isEqualCaseInsensitive(YerothDatabase::LOCALISATIONS,
                                            aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_utilisateur,
    										 *aSqlTableModel);
    }
    else
    {
        setSiteCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_localisations();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_localisation,
    										 sqlTableModel);
    }

    _LISTER_tab_TO_tabTitle.insert("lister_localisation", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_localisation"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_localisation->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_departements_de_produits(YerothSqlTableModel
                                                              * aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ départements"));


    if (0 != aSqlTableModel &&
        YerothUtils::isEqualCaseInsensitive(YerothDatabase::DEPARTEMENTS_PRODUITS,
                                            aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_departements_produits,
    										 *aSqlTableModel);

#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(aSqlTableModel->sqlTableName(),
									 "src/admin/lister/yeroth-erp-admin-lister-window.cpp",
									 QString::number(865)));
        }
#endif

        //qDebug() << QString("++ lister_departements_de_produits | setting new _curSearchSqlTableModel| sql table filter: %1")
        //                              .arg(_curSearchSqlTableModel->filter());
    }
    else
    {
        setProductDepartmentCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_departements_produits();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_departements_produits,
    										 sqlTableModel);

#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(sqlTableModel.sqlTableName(),
									 "src/admin/lister/yeroth-erp-admin-lister-window.cpp",
									 QString::number(890)));
        }
#endif

    }


    _LISTER_tab_TO_tabTitle.insert("lister_departements_de_produits", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_departements_de_produits"));

    //qDebug() << QString("++ lister_categorie | sql table filter: %1")
    //                      .arg(_curSearchSqlTableModel->filter());


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_departements_produits->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_categorie(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ catégories"));

    if (0 != aSqlTableModel &&
        true == YerothUtils::isEqualCaseInsensitive(YerothDatabase::CATEGORIES,
                                                    aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_categorie,
    										 *aSqlTableModel);

//        _curSearchSqlTableModel = aSqlTableModel;
//        //qDebug() << QString("++ lister_categorie | setting new _curSearchSqlTableModel| sql table filter: %1")
//        //                              .arg(_curSearchSqlTableModel->filter());
    }
    else
    {
        setCategoryCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_categories();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_categorie,
    										 sqlTableModel);
    }

    _LISTER_tab_TO_tabTitle.insert("lister_categorie", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_categorie"));

    //qDebug() << QString("++ lister_categorie | sql table filter: %1")
    //                      .arg(_curSearchSqlTableModel->filter());


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_categorie->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_LIGNE_BUDGETAIRE(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ LIGNES BUDÉGTAIRES"));


	if (0 != aSqlTableModel &&
		true == YerothUtils::isEqualCaseInsensitive(YerothDatabase::LIGNES_BUDGETAIRES,
													aSqlTableModel->sqlTableName()))
	{
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_LIGNE_BUDGETAIRE,
    										 *aSqlTableModel);
		//qDebug() << QString("++ lister_LIGNE_BUDGETAIRE | setting new _curSearchSqlTableModel| sql table filter: %1")
	        		//                              .arg(_curSearchSqlTableModel->filter());
	}
	else
	{
		setLIGNE_BUDGETAIRE_CurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_lignes_budgetaires();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_LIGNE_BUDGETAIRE,
    										 sqlTableModel);
	}


	_LISTER_tab_TO_tabTitle.insert("lister_LIGNE_BUDGETAIRE", _windowName);

	setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_LIGNE_BUDGETAIRE"));

	//qDebug() << QString("++ lister_LIGNE_BUDGETAIRE | sql table filter: %1")
	//                      .arg(_curSearchSqlTableModel->filter());


	_lastItemSelectedForModification = 0;

	set_admin_rechercher_font();

	tableView_lister_LIGNE_BUDGETAIRE->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_compte_bancaire(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ lister ~ comptes bancaires"));

    if (0 != aSqlTableModel &&
        true == YerothUtils::isEqualCaseInsensitive(YerothDatabase::COMPTES_BANCAIRES,
                                                	aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_compte_bancaire,
    										 *aSqlTableModel);
    }
    else
    {
        setBankAccountCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_comptes_bancaires();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_compte_bancaire,
    										 sqlTableModel);
    }


    _LISTER_tab_TO_tabTitle.insert("lister_compte_bancaire", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_compte_bancaire"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_compte_bancaire->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_alerte(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                	.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ lister ~ alertes"));

    if (aSqlTableModel &&
    	YerothUtils::isEqualCaseInsensitive(YerothDatabase::ALERTES,
                                            aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_alerte,
    										 *aSqlTableModel);
    }
    else
    {
        setAlertCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_alertes();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_alerte,
    										 sqlTableModel);
    }

    _LISTER_tab_TO_tabTitle.insert("lister_alerte", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_alerte"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_alerte->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::lister_remise(YerothSqlTableModel *aSqlTableModel)
{
	SETUP_PRINT();

    _windowName = QString("%1 - %2")
                	.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ lister ~ remises"));

    if (aSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::REMISES,
                                                   aSqlTableModel->sqlTableName()))
    {
    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_remise,
    										 *aSqlTableModel);
    }
    else
    {
        setDiscountCurrentlyFiltered(false);

        YerothSqlTableModel &sqlTableModel = _allWindows->getSqlTableModel_remises();

    	LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*tableView_lister_remise,
    										 sqlTableModel);
    }


    _LISTER_tab_TO_tabTitle.insert("lister_remise", _windowName);

    setWindowTitle(_LISTER_tab_TO_tabTitle.value("lister_remise"));


    _lastItemSelectedForModification = 0;

    set_admin_rechercher_font();

    tableView_lister_remise->selectRow(_lastItemSelectedForModification);
}


void YerothAdminListerWindow::handleCurrentChanged(int index)
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	lister_utilisateur(_curSearchSqlTableModel);

		break;

    case SUJET_ACTION_LOCALISATION:
    	lister_localisation(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
    	lister_departements_de_produits(_curSearchSqlTableModel);

		break;

    case SUJET_ACTION_CATEGORIE:
    	lister_categorie(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_ligne_budgetaire:
    	lister_LIGNE_BUDGETAIRE(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
    	lister_compte_bancaire(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_REMISE:
    	lister_remise(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_ALERTE:
    	lister_alerte(_curSearchSqlTableModel);

        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	LISTER_CHARGES_FINANCIERES(_curSearchSqlTableModel);
        break;

    default:
        break;
    }
}


void YerothAdminListerWindow::creer()
{
    _allWindows->_adminCreateWindow->rendreVisible(tabWidget_lister->
                                                   currentIndex());
    rendreInvisible();
}


void YerothAdminListerWindow::handleItemModification(const QModelIndex &index)
{
    //_logger->debug("handleItemModification", QString("model index: %1").arg(index.row()));
    _lastItemSelectedForModification = index.row();
}


void YerothAdminListerWindow::modifier()
{
    _logger->log("modifier",
                 QString("current index: %1").arg(tabWidget_lister->
                                                  currentIndex()));

    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        if (tableView_lister_utilisateur->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_LOCALISATION:
        if (tableView_lister_localisation->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        if (tableView_lister_departements_produits->rowCount() > 0)
        {
            //qDebug() << "++ YerothAdminListerWindow::modifier | categorie";

            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_CATEGORIE:
        if (tableView_lister_categorie->rowCount() > 0)
        {
            //qDebug() << "++ YerothAdminListerWindow::modifier | categorie";

            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_ligne_budgetaire:
        if (tableView_lister_LIGNE_BUDGETAIRE->rowCount() > 0)
        {
            //qDebug() << "++ YerothAdminListerWindow::modifier | LIGNE BUDGÉTAIRE";

            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        if (tableView_lister_compte_bancaire->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_ALERTE:
        if (tableView_lister_alerte->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_REMISE:
        if (tableView_lister_remise->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_REMISE);
            rendreInvisible();
        }
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        if (tableView_lister_charges_financieres->rowCount() > 0)
        {
            _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
            rendreInvisible();
        }
        break;

    default:
        break;
    }
}


void YerothAdminListerWindow::afficher_au_detail()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        afficher_detail_utilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        afficher_detail_localisation();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        afficher_detail_departements_de_produits();
        break;

    case SUJET_ACTION_CATEGORIE:
        afficher_detail_categorie();
        break;

    case SUJET_ACTION_ligne_budgetaire:
    	afficher_detail_LIGNE_BUDGETAIRE();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        afficher_detail_compte_bancaire();
        break;

    case SUJET_ACTION_REMISE:
        afficher_detail_remise();
        break;

    case SUJET_ACTION_ALERTE:
        afficher_detail_alerte();
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	afficher_detail_CHARGE_FINANCIERE();
        break;

    default:
        break;
    }
}

void YerothAdminListerWindow::afficher_detail_CHARGE_FINANCIERE()
{
//    _allWindows->_adminDetailWindow
//		->rendreVisibleChargeFinanciere(lastSelectedItemForModification());
//
//    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_utilisateur()
{
    _allWindows->_adminDetailWindow->rendreVisibleCompteUtilisateur
    (lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_localisation()
{
    _allWindows->_adminDetailWindow->rendreVisibleLocalisation
    (lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_departements_de_produits()
{
    //_logger->debug("afficher_detail_categorie",
    //  QString("lastSelectedItemForModification: %1").arg(lastSelectedItemForModification()));
    _allWindows->_adminDetailWindow->rendreVisibleDepartementsDeProduits
    (lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_categorie()
{
    //_logger->debug("afficher_detail_categorie",
    //  QString("lastSelectedItemForModification: %1").arg(lastSelectedItemForModification()));
    _allWindows->_adminDetailWindow->rendreVisibleCategorie
    (lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_LIGNE_BUDGETAIRE()
{
    //_logger->debug("afficher_detail_categorie",
    //  QString("lastSelectedItemForModification: %1").arg(lastSelectedItemForModification()));
    _allWindows->_adminDetailWindow->rendreVisibleLigneBudgetaire(lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_compte_bancaire()
{
    _allWindows->_adminDetailWindow->rendreVisibleCompteBancaire
    (lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_remise()
{
    _allWindows->_adminDetailWindow->
    rendreVisibleRemise(lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::afficher_detail_alerte()
{
    _allWindows->_adminDetailWindow->
    rendreVisibleAlerte(lastSelectedItemForModification());
    rendreInvisible();
}


void YerothAdminListerWindow::supprimer()
{
    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        supprimer_utilisateur();
        break;

    case SUJET_ACTION_LOCALISATION:
        supprimer_localisation();
        break;

    case SUJET_ACTION_CATEGORIE:
        supprimer_categorie();
        break;

    case SUJET_ACTION_ligne_budgetaire:
    	supprimer_LIGNE_BUDGETAIRE();
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        supprimer_compte_bancaire();
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        supprimer_departement_de_produit();
        break;

    case SUJET_ACTION_REMISE:
        supprimer_remise();
        break;

    case SUJET_ACTION_ALERTE:
        supprimer_alerte();
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        break;

    default:
        break;
    }
}


void YerothAdminListerWindow::supprimer_utilisateur()
{
    _logger->log("supprimer_utilisateur");

    YerothSqlTableModel *usersTableModel = 0;

    if (_curSearchSqlTableModel &&
    	YerothUtils::isEqualCaseInsensitive(YerothDatabase::USERS,
                                            _curSearchSqlTableModel->sqlTableName()))
    {
        usersTableModel = _curSearchSqlTableModel;
    }
    else
    {
        usersTableModel = &_allWindows->getSqlTableModel_users();
    }

    QSqlRecord record =
                    usersTableModel->record(lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull("nom_utilisateur"))
    {
        return;
    }

    QString prenom(GET_SQL_RECORD_DATA(record, "prenom"));
    QString nom(GET_SQL_RECORD_DATA(record, "nom"));
    QString id(GET_SQL_RECORD_DATA(record, "nom_utilisateur"));

    prenom.append(QString(" %1 (%2)").arg(nom, id));

    QString
    msgConfirmation(QObject::tr("Supprimer l'utilisateur '%1' ?'").arg
                    (prenom));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool success =
                        usersTableModel->removeRow(lastSelectedItemForModification());

        if (success)
        {
            QString msg(QObject::tr
                        ("L'utilisateur '%1' a été supprimée de la base de données !").
                        arg(prenom));

            YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_COMPTE_UTILISATEUR);
        }
        else
        {
            QString msg(QObject::tr
                        ("L'utilisateur '%1 %2' n'a pas été supprimée de la base de données !").
                        arg(prenom, nom));

            YerothQMessageBox::warning(this, QObject::tr("supprimer"),
                                       msg, QMessageBox::Ok);
        }
    }
}


void YerothAdminListerWindow::supprimer_localisation()
{
    _logger->log("supprimer_localisation");

    YerothSqlTableModel *localisationsTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::LOCALISATIONS,
                                                   _curSearchSqlTableModel->
                                                   sqlTableName()))
    {
        localisationsTableModel = _curSearchSqlTableModel;
    }
    else
    {
        localisationsTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }

    QSqlRecord record =
                    localisationsTableModel->record(lastSelectedItemForModification());

    if (record.isEmpty() || record.isNull("nom_localisation"))
    {
        return;
    }

    QString nom_localisation(GET_SQL_RECORD_DATA(record, "nom_localisation"));

    QString msgConfirmation(QObject::tr("Supprimer la localisation '%1' ?").
                            arg(nom_localisation));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool success =
                        localisationsTableModel->removeRow(lastSelectedItemForModification
                                                           ());

        QString msg(QObject::tr("La localisation '%1").
                    arg(nom_localisation));

        if (success)
        {
            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_LOCALISATION);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::warning(this, QObject::tr("supprimer"),
                                       msg, QMessageBox::Ok);
        }
    }
}


void YerothAdminListerWindow::supprimer_categorie()
{
    _logger->log("supprimer_categorie");

    YerothSqlTableModel *categoriesTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::CATEGORIES,
                                                   _curSearchSqlTableModel->
                                                   sqlTableName()))
    {
        categoriesTableModel = _curSearchSqlTableModel;
    }
    else
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }

    QSqlRecord record =
                    categoriesTableModel->record(lastSelectedItemForModification());

    if (record.isEmpty()
            || record.isNull(YerothDatabaseTableColumn::NOM_CATEGORIE))
    {
        return;
    }

    QString
    categorie(GET_SQL_RECORD_DATA
              (record, YerothDatabaseTableColumn::NOM_CATEGORIE));

    QString msgConfirmation(QObject::tr("Supprimer la catégorie '%1' ?").
                            arg(categorie));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
    	/*
    	 * CHECK UNDER STOCK TABLE
    	 */
        QString
        SEARCH_IF_STOCK_UNDER_TO_REMOVE_CATEGORIE_STILL_EXIST(QString
                                                              ("select %1 from %2 where %3='%4'")
															  .arg(YerothDatabaseTableColumn::ID,
																   YerothDatabase::STOCKS,
																   YerothDatabaseTableColumn::CATEGORIE,
																   categorie));


        int existing_stock_row_count =
                        YerothUtils::execQueryRowCount
                        (SEARCH_IF_STOCK_UNDER_TO_REMOVE_CATEGORIE_STILL_EXIST);


        if (existing_stock_row_count > 0)
        {
            YerothQMessageBox::information(this,
                                           QObject::tr("supprimer"),
                                           QObject::tr
                                           ("1. La catégorie '%1' comporte"
                                            " encore '%2' stocks !\n\n"
                                            "2. SVP, supprimer ou ecouler d'abord"
                                            " tous ces stocks !").arg
                                           (categorie,
                                            QString::number
                                            (existing_stock_row_count)),
                                           QMessageBox::Ok);

            return;
        }

    	/*
    	 * CHECK UNDER "charges_financieres" TABLE
    	 */
        QString
        SEARCH_IF_STOCK_UNDER_TO_REMOVE_charges_financieres_STILL_EXIST(QString
                                                              ("select %1 from %2 where %3='%4'")
															  .arg(YerothDatabaseTableColumn::ID,
																   YerothDatabase::CHARGES_FINANCIERES,
																   YerothDatabaseTableColumn::CATEGORIE,
																   categorie));


        int existing_charges_financieres_row_count =
                        YerothUtils::execQueryRowCount
                        (SEARCH_IF_STOCK_UNDER_TO_REMOVE_charges_financieres_STILL_EXIST);


        if (existing_charges_financieres_row_count > 0)
        {
            YerothQMessageBox::information(this,
                                           QObject::tr("supprimer"),
                                           QObject::tr
                                           ("1. La catégorie '%1' comporte"
                                            " encore '%2' charges financières !\n\n"
                                            "2. SVP, supprimer d'abord tous ces charges financières !").arg
                                           (categorie,
                                            QString::number(existing_charges_financieres_row_count)),
                                            QMessageBox::Ok);

            return;
        }



        bool success =
                        categoriesTableModel->removeRow(lastSelectedItemForModification());

        QString msg(QObject::tr("La catégorie '%1").arg(categorie));

        if (success)
        {
            msg.append(QObject::tr
                       ("' a été supprimé de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_CATEGORIE);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas pu été supprimé de la base de données !"));

            YerothQMessageBox::warning(this, tr("supprimer"), msg,
                                       QMessageBox::Ok);
        }
    }
}


void YerothAdminListerWindow::supprimer_LIGNE_BUDGETAIRE()
{
	_logger->log("supprimer_LIGNE_BUDGETAIRE");

	YerothSqlTableModel *lignesBudgetairesTableModel = 0;

	if (_curSearchSqlTableModel &&
		YerothUtils::isEqualCaseInsensitive(YerothDatabase::LIGNES_BUDGETAIRES,
											_curSearchSqlTableModel->sqlTableName()))
	{
		lignesBudgetairesTableModel = _curSearchSqlTableModel;
	}
	else
	{
		lignesBudgetairesTableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
	}

	QSqlRecord record = lignesBudgetairesTableModel->record(lastSelectedItemForModification());

	if (record.isEmpty() ||
		record.isNull(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire))
	{
		return;
	}

	QString intitule_de_la_ligne_budgetaire(
				GET_SQL_RECORD_DATA(record,
								    YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire));

	QString msgConfirmation(QObject::tr("Supprimer la LIGNE BUDGÉTAIRE '%1' ?")
								.arg(intitule_de_la_ligne_budgetaire));

	if (QMessageBox::Ok ==
			YerothQMessageBox::question(this, QObject::tr("supprimer"),
					msgConfirmation, QMessageBox::Cancel,
					QMessageBox::Ok))
	{
		/*
		 * CHECK UNDER CHARGES_FINANCIERES TABLE
		 *
		 * column 'categorie' of table charges_financieres corresponds
		 * to column 'INTITULE_DE_LA_ligne_budgetaire' of table LIGNES_BUDGETAIRES !
		 */
		QString SEARCH_IF_financial_expense_UNDER_TO_REMOVE_BUDGET_LINE_STILL_EXIST(
				QString("select %1 from %2 where %3='%4'")
					.arg(YerothDatabaseTableColumn::ID,
						 YerothDatabase::CHARGES_FINANCIERES,
						 YerothDatabaseTableColumn::CATEGORIE,
						 intitule_de_la_ligne_budgetaire));

		int existing_financial_expense_row_count =
				YerothUtils::execQueryRowCount
				(SEARCH_IF_financial_expense_UNDER_TO_REMOVE_BUDGET_LINE_STILL_EXIST);


		if (existing_financial_expense_row_count > 0)
		{
			YerothQMessageBox::information(this,
										   QObject::tr("supprimer"),
										   QObject::tr("1. La LIGNE BUDGÉTAIRE '%1' comporte"
												   	   	   " encore '%2' charges financières !\n\n"
												   	   	   "2. SVP, supprimer d'abord toutes ces "
												   	   	   " charges financières !")
												.arg(intitule_de_la_ligne_budgetaire,
													 QString::number(existing_financial_expense_row_count)),
													 QMessageBox::Ok);

			return;
		}


		bool success = lignesBudgetairesTableModel->removeRow(lastSelectedItemForModification());


		/*
		 * IL FAUT AUSSI SUPPRIMER LA CATÉGORIE CORRESPONDANT !
		 */
		QString DELETE_CORRESPONDING_STOCK_CATEGORY =
				QString("delete from %1 where %2='%3'")
					.arg(YerothDatabase::CATEGORIES,
						 YerothDatabaseTableColumn::NOM_CATEGORIE,
						 intitule_de_la_ligne_budgetaire);

		YerothUtils::execQuery(DELETE_CORRESPONDING_STOCK_CATEGORY);


		QString msg(QObject::tr("La LIGNE BUDGÉTAIRE '%1")
							.arg(intitule_de_la_ligne_budgetaire));

		if (success)
		{
			msg.append(QObject::tr
					("' a été supprimé de la base de données !"));

			YerothQMessageBox::information(this, QObject::tr("supprimer"),
					msg, QMessageBox::Ok);

			self_reset_view(SUJET_ACTION_ligne_budgetaire);
		}
		else
		{
			msg.append(QObject::tr
					(" n'a pas pu été supprimé de la base de données !"));

			YerothQMessageBox::warning(this, tr("supprimer"), msg,
					QMessageBox::Ok);
		}
	}
}


void YerothAdminListerWindow::supprimer_compte_bancaire()
{
    _logger->log("supprimer_compte_bancaire");

    YerothSqlTableModel *comptesBancairesTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::
                                                   COMPTES_BANCAIRES,
                                                   _curSearchSqlTableModel->
                                                   sqlTableName()))
    {
        comptesBancairesTableModel = _curSearchSqlTableModel;
    }
    else
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record =
                    comptesBancairesTableModel->record(lastSelectedItemForModification());

    if (record.isEmpty()
            || record.
            isNull(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE))
    {
        return;
    }

    QString
    reference_du_compte_bancaire(GET_SQL_RECORD_DATA
                                 (record,
                                  YerothDatabaseTableColumn::
                                  REFERENCE_DU_COMPTE_BANCAIRE));

    QString msgConfirmation(QObject::tr
                            ("Supprimer le compte bancaire avec la référence '%1' ?").
                            arg(reference_du_compte_bancaire));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool success =
                        comptesBancairesTableModel->removeRow(lastSelectedItemForModification
                                                              ());

        QString
        msg(QObject::tr
            ("Le compte bancaire avec la référence '%1").arg
            (reference_du_compte_bancaire));

        if (success)
        {
            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_COMPTE_BANCAIRE);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::warning(this, QObject::tr("supprimer"), msg,
                                       QMessageBox::Ok);
        }
    }
}


void YerothAdminListerWindow::supprimer_departement_de_produit()
{
    _logger->log("supprimer_departement_de_produit");

    YerothSqlTableModel *departements_produits_TableModel = 0;

    if (0 != _curSearchSqlTableModel &&
            YerothUtils::isEqualCaseInsensitive(YerothDatabase::
                                                DEPARTEMENTS_PRODUITS,
                                                _curSearchSqlTableModel->
                                                sqlTableName()))
    {
        departements_produits_TableModel = _curSearchSqlTableModel;
    }
    else
    {
        departements_produits_TableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }


    QSqlRecord record =
                    departements_produits_TableModel->record(lastSelectedItemForModification
                                                             ());


    if (record.isEmpty()
            || record.isNull(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT))
    {
        return;
    }

    QString
    nom_departement_produit(GET_SQL_RECORD_DATA
                            (record,
                             YerothDatabaseTableColumn::
                             NOM_DEPARTEMENT_PRODUIT));

    QString msgConfirmation(QObject::tr
                            ("Supprimer le département de produits '%1' ?").
                            arg(nom_departement_produit));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgConfirmation, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {

    	  //TESTING FOR TRIGGERING failure DELETE WITHOUT SELECT
#ifdef TESTING_YR_DB_RUNTIME_VERIF_ICTSS_ONE
    	QString
        SEARCH_IF_STOCK_UNDER_TO_REMOVE_DEPARTMENT_STILL_EXIST
				(QString("select %1 from %2 where %3='%4'")
					.arg(YerothDatabaseTableColumn::ID,
						 YerothDatabase::STOCKS,
						 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
						 nom_departement_produit));


        int existing_stock_row_count =
                        YerothUtils::execQueryRowCount
                        (SEARCH_IF_STOCK_UNDER_TO_REMOVE_DEPARTMENT_STILL_EXIST);


        if (existing_stock_row_count > 0)
        {
#ifdef YEROTH_SERVER
        YEROTH_RUNTIME_VERIFIER *dbusServer = _allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1.%2;%3;%4")
								.arg(YerothDatabase::STOCKS,
									 nom_departement_produit,
									 "src/admin/lister/yeroth-erp-admin-lister-window.cpp",
									 QString::number(1570)));
        }
#endif
            YerothQMessageBox::information(this,
                                           QObject::tr("supprimer"),
                                           QObject::tr
                                           ("1. Le département de produits '%1' comporte"
                                            " encore '%2' stocks !\n\n"
                                            "2. SVP, supprimer ou ecouler d'abord"
                                            " tous ces  stocks !").arg
                                           (nom_departement_produit,
                                            QString::number
                                            (existing_stock_row_count)),
                                           QMessageBox::Ok);

            return;
        }*/
#endif //TESTING_YR_DB_RUNTIME_VERIF_ICTSS_ONE

        bool successRemoveDEPARTEMENT = departements_produits_TableModel
        									->removeRow(lastSelectedItemForModification());

#ifdef YEROTH_SERVER
        YEROTH_RUNTIME_VERIFIER *dbusServer = _allWindows->dbusServer();
        if (dbusServer && successRemoveDEPARTEMENT)
        {
        	dbusServer->YR_slot_refresh_DELETE_DB_MYSQL__CALLED
							(QString("%1.%2;%3;%4")
								.arg(YerothDatabase::DEPARTEMENTS_PRODUITS,
									 nom_departement_produit,
									 "src/admin/lister/yeroth-erp-admin-lister-window.cpp",
									 QString::number(1603)));
        }
#endif

        QString STR_QUERY_REMOVE_DEPARTEMENT_DE_PRODUITS_DU_TABLEAU_MARCHANDISES
        (QString("DELETE FROM %1 WHERE %2='%3'").arg
         (YerothDatabase::MARCHANDISES,
          YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
          nom_departement_produit));


        bool successRemoveDEPARTEMENT_marchandises =
        		YerothUtils::execQuery(STR_QUERY_REMOVE_DEPARTEMENT_DE_PRODUITS_DU_TABLEAU_MARCHANDISES);

        successRemoveDEPARTEMENT = successRemoveDEPARTEMENT &&
        		successRemoveDEPARTEMENT_marchandises;

        QString msg(QObject::tr("Le département de produits '%1").arg
                    (nom_departement_produit));

        if (successRemoveDEPARTEMENT_marchandises)
        {
#ifdef YEROTH_SERVER
        YEROTH_RUNTIME_VERIFIER *dbusServer = _allWindows->dbusServer();
        if (dbusServer && successRemoveDEPARTEMENT_marchandises)
        {
        	dbusServer->YR_slot_refresh_DELETE_DB_MYSQL__CALLED
							(QString("%1.%2;%3;%4")
								.arg(YerothDatabase::MARCHANDISES,
									 nom_departement_produit,
									 "src/admin/lister/yeroth-erp-admin-lister-window.cpp",
									 QString::number(1659)));
        }
#endif
            msg.append(QObject::tr
                       ("' a été supprimé de la base de données !"));

            YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas pu été supprimé de la base de données !"));

            YerothQMessageBox::warning(this, tr("supprimer"), msg,
                                       QMessageBox::Ok);
        }
    }
}


void YerothAdminListerWindow::supprimer_remise()
{

}


void YerothAdminListerWindow::supprimer_alerte()
{
    _logger->log("supprimer_alerte");
    YerothSqlTableModel *alertesTableModel = 0;

    if (_curSearchSqlTableModel
            && YerothUtils::isEqualCaseInsensitive(YerothDatabase::ALERTES,
                                                   _curSearchSqlTableModel->
                                                   sqlTableName()))
    {
        alertesTableModel = _curSearchSqlTableModel;
    }
    else
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record =
                    alertesTableModel->record(lastSelectedItemForModification());

    if (record.isEmpty()
            || record.isNull(YerothDatabaseTableColumn::DESIGNATION_ALERTE))
    {
        return;
    }

    QString
    designation_alerte(GET_SQL_RECORD_DATA
                       (record,
                        YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    QString msgConfirmation(QObject::tr("Supprimer l'alerte '%1' ?").arg
                            (designation_alerte));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("supprimer"),
                                        msgConfirmation,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool success =
                        alertesTableModel->removeRow(lastSelectedItemForModification());

        QString msg("L'alerte '");
        msg.append(designation_alerte);

        if (success)
        {
            msg.append(QObject::tr
                       ("' a été supprimée de la base de données !"));

            YerothQMessageBox::information(this,
                                           QObject::tr("supprimer"),
                                           msg, QMessageBox::Ok);

            self_reset_view(SUJET_ACTION_ALERTE);
        }
        else
        {
            msg.append(QObject::tr
                       (" n'a pas été supprimée de la base de données !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr("supprimer"),
                                       msg, QMessageBox::Ok);
        }
    }
}


QString YerothAdminListerWindow::get_latex_template_print_pdf_content()
{
    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();

    switch (tabWidget_lister->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_utilisateur);


    	_DBFieldNamesToPrintLeftAligned.insert(5);
    	_DBFieldNamesToPrintLeftAligned.insert(6);


    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"User");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"USER LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Utilisateurs");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE D'UTILISATEURS");
        }

		break;

    case SUJET_ACTION_LOCALISATION:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_localisation);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Site");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"SITE LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Localisations");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE LOCALISATIONS");
        }

        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_departements_produits);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Department");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"DEPARTMENT LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Départements");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE DÉPARTEMENTS");
        }

		break;

    case SUJET_ACTION_CATEGORIE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_categorie);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Category");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"CATEGORY LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Catégories");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE CATÉGORIES");
        }

        break;

    case SUJET_ACTION_ligne_budgetaire:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_LIGNE_BUDGETAIRE);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"BUDGET LINE");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"BUDGET LINE LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"LIGNES BUDÉTAIRES");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE LIGNES BUDÉTAIRES");
        }

        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_compte_bancaire);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Bank account");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"BANK ACCOUNT LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Comptes bancaires");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE COMPTES BANCAIRES");
        }

        break;

    case SUJET_ACTION_REMISE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_remise);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Discount");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"DISCOUNT LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Remises");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE REMISES");
        }

        break;

    case SUJET_ACTION_ALERTE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_alerte);

    	if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Alert");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"ALERT LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Alertes");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE D'ALERTES");
        }

        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	setYerothTableView_FROM_WINDOWS_COMMONS(tableView_lister_charges_financieres);

        if (YerothMainWindow::LANGUE_ANGLAISE)
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Financial expense");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"FINANCIAL EXPENSE LISTING");
        }
        else
        {
    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHOBJECT",
						"Charges financières");

    		_documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHSUBJECT",
						"1 LISTE DE CHARGES FINANCIÈRES");
        }
        break;

    default:
        break;
    }


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_lister_admin_objects_TEX;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_lister_admin_objects_TEX;
    }


    return _latex_template_print_pdf_content;
}



