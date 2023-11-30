/**
 * yeroth-erp-admin-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-admin-window.hpp"


#include "src/imports/yeroth-erp-marchandise-import.hpp"

#include "src/imports/yeroth-erp-stock-import.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/process/yeroth-erp-process-info.hpp"

#include "src/utils/yeroth-erp-database.hpp"

#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"

#include "src/widgets/yeroth-erp-progress-bar.hpp"

#include "src/users/yeroth-erp-users.hpp"


#include <QtSql/QSqlQuery>

#include <QtCore/QTemporaryFile>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QStyleFactory>


const int YerothAdminWindow::MAX_IMPORT_CSV_HEADER_SIZE(16);

const QString YerothAdminWindow::DATABASE_YEROTH_ERP_3_0_KEYWORD("* database * ");


QStringList
YerothAdminWindow::comboBox_impression_sur_CHOICE_list;

QStringList
YerothAdminWindow::comboBox_format_de_facture_CHOICE_list;


#ifdef YEROTH_FRANCAIS_LANGUAGE

const
QString YerothAdminWindow::EFFACER(QObject::tr("effacer"));
const QString YerothAdminWindow::EXPORTER(QObject::tr("exporter"));
const QString YerothAdminWindow::IMPORTER(QObject::tr("importer"));

const QString YerothAdminWindow::DB(QObject::tr("base de données"));
const QString YerothAdminWindow::TABLEAU(QObject::tr("tableau"));

#elif YEROTH_ENGLISH_LANGUAGE

const
QString YerothAdminWindow::EFFACER(QObject::tr("erase"));
const QString YerothAdminWindow::EXPORTER(QObject::tr("export"));
const QString YerothAdminWindow::IMPORTER(QObject::tr("import"));

const QString YerothAdminWindow::DB(QObject::tr("database"));
const QString YerothAdminWindow::TABLEAU(QObject::tr("database table"));

#endif



#ifdef YEROTH_FRANCAIS_LANGUAGE
const QString YerothAdminWindow::CREER(QObject::
                                       trUtf8(ACTION_ADMIN_OPERATIONS_CREATE_FR));
const QString YerothAdminWindow::LISTER(QObject::tr(ACTION_ADMIN_OPERATIONS_LIST_FR));
const QString YerothAdminWindow::MODIFIER(QObject::
                                          trUtf8(ACTION_ADMIN_OPERATIONS_MODIFY_FR));
const QString YerothAdminWindow::SUPPRIMER(QObject::
                                           trUtf8(ACTION_ADMIN_OPERATIONS_DELETE_FR));

const QString YerothAdminWindow::COMPTE_UTILISATEUR(QObject::tr
                                                    (SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_FR));
const QString YerothAdminWindow::LOCALISATION(QObject::
                                              trUtf8(SUBJECT_ADMIN_OPERATIONS_SITE_FR));
const QString YerothAdminWindow::DEPARTEMENTS_DE_PRODUITS(QObject::tr
                                                          (SUBJECT_ADMIN_OPERATIONS_PRODUCT_DEPARTMENT_FR));
const QString YerothAdminWindow::CATEGORIE(QObject::
                                           trUtf8(SUBJECT_ADMIN_OPERATIONS_CATEGORY_FR));
const QString YerothAdminWindow::LIGNE_BUDGETAIRE(QObject::
                                           trUtf8(SUBJECT_ADMIN_OPERATIONS_BUDGET_LINE_FR));
const QString YerothAdminWindow::ALERTE(QObject::
                                        trUtf8(SUBJECT_ADMIN_OPERATIONS_ALERT_FR));
const QString YerothAdminWindow::BON_DE_COMMANDE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_FINANCIAL_EXPENSE_FR));
const QString YerothAdminWindow::COMPTE_BANCAIRE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_BANK_ACCOUNT_FR));
const QString YerothAdminWindow::REMISE(QObject::
                                        trUtf8(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_FR));

#elif YEROTH_ENGLISH_LANGUAGE
const QString YerothAdminWindow::CREER(QObject::tr(ACTION_ADMIN_OPERATIONS_CREATE_EN));
const QString YerothAdminWindow::LISTER(QObject::tr(ACTION_ADMIN_OPERATIONS_LIST_EN));
const QString YerothAdminWindow::MODIFIER(QObject::tr(ACTION_ADMIN_OPERATIONS_MODIFY_EN));
const QString YerothAdminWindow::SUPPRIMER(QObject::
                                           tr(ACTION_ADMIN_OPERATIONS_DELETE_EN));

const QString YerothAdminWindow::COMPTE_UTILISATEUR(QObject::tr
                                                    (SUBJECT_ADMIN_OPERATIONS_USER_ACCOUNT_EN));
const QString YerothAdminWindow::LOCALISATION(QObject::
                                              tr(SUBJECT_ADMIN_OPERATIONS_SITE_EN));
const QString YerothAdminWindow::DEPARTEMENTS_DE_PRODUITS(QObject::tr
                                                          (SUBJECT_ADMIN_OPERATIONS_PRODUCT_DEPARTMENT_EN));
const QString YerothAdminWindow::CATEGORIE(QObject::
                                           tr(SUBJECT_ADMIN_OPERATIONS_CATEGORY_EN));
const QString YerothAdminWindow::LIGNE_BUDGETAIRE(QObject::
                                           tr(SUBJECT_ADMIN_OPERATIONS_BUDGET_LINE_EN));
const QString YerothAdminWindow::ALERTE(QObject::tr(SUBJECT_ADMIN_OPERATIONS_ALERT_EN));
const QString YerothAdminWindow::BON_DE_COMMANDE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_FINANCIAL_EXPENSE_EN));
const QString YerothAdminWindow::COMPTE_BANCAIRE(QObject::tr
                                                 (SUBJECT_ADMIN_OPERATIONS_BANK_ACCOUNT_EN));
const QString YerothAdminWindow::REMISE(QObject::
                                        tr(SUBJECT_ADMIN_OPERATIONS_DISCOUNT_EN));
#endif

YerothAdminWindow::YerothAdminWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ accueil")),
 _logger(new YerothLogger("YerothAdminWindow")),
 _importer_parlant(false),
 _curAdminAction(ACTION_CREER),
 _curAdminSujetAction(SUJET_ACTION_COMPTE_UTILISATEUR),
 _curAdminMaintainAction(ACTION_EXPORTER),
 _curAdminSujetsMaintainAction(SUJET_ACTION_DB),
 _actionsToConst(0),
 _sujetActionsToConst(0)
{
    setupUi(this);

    mySetupUi(this);


    comboBox_impression_sur_CHOICE_list
            << YerothUtils::IMPRIMANTE_PDF
            << YerothUtils::IMPRIMANTE_EPSON_TM_T20ii
            << YerothUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU;


    comboBox_format_de_facture_CHOICE_list
            << YerothERPConfig::RECEIPT_FORMAT_PETIT
            << YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER
            << YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER;


    stop_TESTING_MAINTENANCE();


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    comboBox_localisation->setYerothEditable(true);

    populateComboBoxes();


    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->checkField();

    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE
		->setValidator(&YerothUtils::STRING_FOR_YEROTH_ERP_3_0_IP_ADDRESS_VALIDATOR);


    lineEdit_importer_separation_csv
		->setYerothEnabled(false);

    lineEdit_importer_separation_csv
		->setText(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    lineEdit_localisation_adresse_ip->setYerothEnabled(false);

    lineEdit_this_localisation->setYerothEnabled(false);

    lineEdit_this_localisation
		->setText(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

    _yerothAdminWindowTitleStart.append(windowTitle());


    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE
		->setYerothEnabled(false);

    lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0
		->setYerothEnabled(false);

    lineEdit_administration_maintenance_commandes_effacer_un_tableau
		->setYerothEnabled(false);

    lineEdit_administration_maintenance_commandes_exporter_un_tableau
		->setYerothEnabled(false);

    lineEdit_administration_maintenance_commandes_importer_un_tableau
		->setYerothEnabled(false);

    lineEdit_administration_maintenance_commandes_supprimer_un_tableau
		->setYerothEnabled(false);


    lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0
		->setText(QString("mysqldump --databases -u '%1' -p"));

    lineEdit_administration_maintenance_commandes_exporter_un_tableau
		->setText(QString("mysqldump db -u '%1' -p"));


    setupValidators();

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _actionsToConst = new QMap<QString, AdminAction>;

    _sujetActionsToConst = new QMap<QString, AdminSujetAction>;

    _actionsToConst->insert(CREER, ACTION_CREER);
    _actionsToConst->insert(LISTER, ACTION_LISTER);
    _actionsToConst->insert(MODIFIER, ACTION_MODIFIER);
    _actionsToConst->insert(SUPPRIMER, ACTION_SUPPRIMER);
    _actionsToConst->insert(EXPORTER, ACTION_EXPORTER);
    _actionsToConst->insert(IMPORTER, ACTION_IMPORTER);
    _actionsToConst->insert(EFFACER, ACTION_EFFACER);

    _sujetActionsToConst->insert(ALERTE, SUJET_ACTION_ALERTE);

    _sujetActionsToConst->insert(BON_DE_COMMANDE,
                                 SUJET_ACTION_CHARGE_FINANCIERE);

    _sujetActionsToConst->insert(CATEGORIE, SUJET_ACTION_CATEGORIE);

    _sujetActionsToConst->insert(LIGNE_BUDGETAIRE, SUJET_ACTION_ligne_budgetaire);

    _sujetActionsToConst->insert(COMPTE_BANCAIRE,
                                 SUJET_ACTION_COMPTE_BANCAIRE);

    _sujetActionsToConst->insert(COMPTE_UTILISATEUR,
                                 SUJET_ACTION_COMPTE_UTILISATEUR);

    _sujetActionsToConst->insert(DEPARTEMENTS_DE_PRODUITS,
                                 SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

    _sujetActionsToConst->insert(LOCALISATION, SUJET_ACTION_LOCALISATION);

    _sujetActionsToConst->insert(REMISE, SUJET_ACTION_REMISE);

    _sujetActionsToConst->insert(DB, SUJET_ACTION_DB);

    _sujetActionsToConst->insert(TABLEAU, SUJET_ACTION_TABLEAU);

    comboBox_sujets_actions->addItem(ALERTE);
    comboBox_sujets_actions->addItem(BON_DE_COMMANDE);
    comboBox_sujets_actions->addItem(CATEGORIE);
    comboBox_sujets_actions->addItem(LIGNE_BUDGETAIRE);
    comboBox_sujets_actions->addItem(COMPTE_BANCAIRE);
    comboBox_sujets_actions->addItem(COMPTE_UTILISATEUR);
    comboBox_sujets_actions->addItem(DEPARTEMENTS_DE_PRODUITS);
    comboBox_sujets_actions->addItem(LOCALISATION);
    comboBox_sujets_actions->addItem(REMISE);

    YEROTH_ERP_3_populate_all_tables();

    comboBox_strategie_vente_sortie
		->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    comboBox_strategie_vente_sortie
		->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    comboBox_strategie_vente_sortie
		->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    comboBox_strategie_vente_sortie
		->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    comboBox_actions->addItem(CREER);
    comboBox_actions->addItem(LISTER);
    comboBox_actions->addItem(MODIFIER);
    comboBox_actions->addItem(SUPPRIMER);


    comboBox_operations_maintenance->addItem(EFFACER);
    comboBox_operations_maintenance->addItem(EXPORTER);
    comboBox_operations_maintenance->addItem(IMPORTER);
    comboBox_operations_maintenance->addItem(SUPPRIMER);


    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);


    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);


    pushButton_maintenance_valider
		->enable(this,
				 SLOT(EXECUTER_COMMANDE_MAINTENANCE()));

    pushButton_maintenance_reinitialiser
		->enable(this,
				 SLOT(reinitialiser_AFFICHAGE_COMMANDE_MAINTENANCE()));

    pushButton_entreprise_reinitialiser
		->enable(this,
				 SLOT(read_entreprise_info_database_table()));

    pushButton_entreprise_enregistrer
		->enable(this,
				 SLOT(enregistrer_entreprise_info_database_table()));


    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS()));

    pushButton_START_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(START_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    pushButton_STOP_DB_RUNTIME_VERIFICATOR
		->enable(this,
				 SLOT(STOP_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()));


    pushButton_rafraichir_statut_alert_daemon
		->enable(this,
                 SLOT(set_colour_status_of_alert_daemon_process_buttons()));

    pushButton_demarrer_alert_daemon
		->enable(this,
                 SLOT(start_alert_daemon_process()));

    pushButton_stopper_alert_daemon
		->enable(this,
				 SLOT(stop_alert_daemon_process()));

    label_ON->setVisible(false);
    label_OFF->setVisible(true);

    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);

    pushButton_choose_mariadb_client_full_path_dir->enable(this,
                                                           SLOT
                                                           (choose_path_mariadb_client_dir
                                                            ()));

    pushButton_choose_pdfReader->enable(this, SLOT(choose_path_pdfReader()));

    pushButton_choose_fichier_systeme_imprimante_thermique->enable(this,
                                                                   SLOT
                                                                   (choose_path_thermalPrinterDeviceFile
                                                                    ()));

    pushButton_choose_repertoire_systeme_latex->enable(this,
                                                       SLOT
                                                       (choose_path_latex_system_root_folder
                                                        ()));

    pushButton_choose_repertoire_des_sauvegardes->enable(this,
                                                         SLOT
                                                         (choose_repertoire_des_sauvegardes
                                                          ()));

    pushButton_choose_repertoire_fichiers_temporaires->enable(this,
                                                              SLOT
                                                              (choose_repertoire_fichiers_temporaires
                                                               ()));

    pushButton_yeroth_erp_3_0_system_daemon_parameters_reinitialiser->enable
    (this,
     SLOT(reinitialiser_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration()));

    pushButton_yeroth_erp_3_0_system_daemon_parameters_enregistrer->enable
    (this, SLOT(enregistrer_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration()));

    pushButton_parametres_reinitialiser->enable(this,
                                                SLOT
                                                (reinitialiser_app_parameters_configuration
                                                 ()));

    pushButton_parametres_enregistrer->enable(this,
                                              SLOT
                                              (enregistrer_app_parameters_configuration
                                               ()));

    pushButton_connecter_localisation->enable(this,
                                              SLOT(connecter_localisation_db
                                                   ()));

    pushButton_deconnecter_localisation->enable(this,
                                                SLOT
                                                (deconnecter_localisation_db
                                                 ()));


    connect(comboBox_operations_maintenance,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(handle_changer_commande_MAINTENANCE_OPERATION
                 (const QString &)));

    connect(comboBox_sujets_maintenance,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(handle_changer_commande_MAINTENANCE_SUJET
                 (const QString &)));

    connect(comboBox_tableaux_mariadb_sql,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(handle_changer_tableau_dimportation(const QString &)));

    connect(checkBox_importer_parlant, SIGNAL(stateChanged(int)),
            this, SLOT(handle_importer_parlant_checkBox(int)));


    /*connect(tabWidget_maintenance_verification_DATABASE,
            SIGNAL(currentIndexChanged(const QString &)),
			this,
            SLOT(handle__tabWidget_maintenance_verification_DATABASE(const QString &)));*/


    connect(comboBox_impression_sur,
            SIGNAL(currentIndexChanged(const QString &)), this,
            SLOT(handleThermalPrinterConfiguration(const QString &)));

    connect(checkBox_activer_registre_de_caisse, SIGNAL(stateChanged(int)),
            this, SLOT(handleCheckboxActiverRegistreCaisse(int)));

    connect(comboBox_impression_sur,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(choix_registre_de_caisse(const QString &)));

    connect(actionStocks, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionCreer, SIGNAL(triggered()), this, SLOT(action_creer()));
    connect(actionGo, SIGNAL(triggered()), this,
            SLOT(gerer_choix_action()));
    connect(actionModifier, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionSupprimer, SIGNAL(triggered()), this,
            SLOT(action_lister()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this,
            SLOT(apropos()));
    connect(actionGuide_PRATIQUE_DE_LUTILISATEUR, SIGNAL(triggered()), this,
            SLOT(getManuelUtilisateurPDF()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

    connect(comboBox_sujets_actions, SIGNAL(activated(int)), this,
            SLOT(gerer_choix_action()));

    connect(comboBox_localisation,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(set_localisation_adresse_ip_text()));

    connect(tabWidget_administration, SIGNAL(currentChanged(int)), this,
            SLOT(handleTabChanged(int)));

    initialize_admin_importer_csv_tableau();
}

YerothAdminWindow::~YerothAdminWindow()
{
    delete _logger;
    delete _actionsToConst;
    delete _sujetActionsToConst;
}

void YerothAdminWindow::setupValidators()
{
    lineEdit_sauvegarde_de_yeroth_erp_3_secondes->setValidator(&YerothUtils::
                                                               Int_BACKUP_YEROTH_ERP_3_Validator);

    lineEdit_alert_period_time_interval->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_alert_quantity_time_interval->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setValidator
    (&YerothUtils::
     nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE_Validator);

    lineEdit_longueur_maximale_string->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_taille_de_pagination_par_defaut->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_MAINTENANCE_VERIFICATION_log_size_max->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_annee_depart_rapports_chiffre_affaire->
    setValidator(&YerothUtils::UintValidator);

    lineEdit_annee_de_depart_pour_la_pagination->
    setValidator(&YerothUtils::UintValidator);
}


void YerothAdminWindow::PRINT_SIGNALS_SOURCE_TARGET_info()
{
	int size = YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.size();

	static bool FIRST_TIME_EVER_call = true;

	if (FIRST_TIME_EVER_call)
	{
		if (size < YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.reserve(YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT);
		}

		FIRST_TIME_EVER_call = false;
	}

	QStringList cur_string_list;

	for (uint i = 0; i < size; ++i)
	{
		for (uint k = 0; k < YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT && k < size; ++k)
		{
			cur_string_list =
					YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.at(k)
					.split(";", Qt::KeepEmptyParts);

			tableWidget_LOGGING_runtime_VERIFICATION->addArticle(cur_string_list.at(0),
																 cur_string_list.at(1),
																 cur_string_list.at(2),
																 cur_string_list.at(3),
																 cur_string_list.at(4));
		}

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.clear();

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings.reserve(YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT);

		return ;
	}
}


void YerothAdminWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);

    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_operation_go->disable(this);
}

void YerothAdminWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    pushButton_reinitialiser_le_logo_de_lentreprise->disable(this);

    pushButton_selectionner_logo_de_lentreprise->disable(this);

    pushButton_supprimer_logo_de_lentreprise->disable(this);

    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
}


void YerothAdminWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 true);

    tabWidget_administration->setCurrentIndex(OPERATIONS);


    pushButton_reinitialiser_le_logo_de_lentreprise->enable(this,
                                                            SLOT
                                                            (reinitialiser_la_derniere_selection_du_logo_de_lentreprise
                                                             ()));

    pushButton_selectionner_logo_de_lentreprise->enable(this,
                                                        SLOT
                                                        (selectionner_logo_de_lentreprise
                                                         ()));

    pushButton_supprimer_logo_de_lentreprise->enable(this,
                                                     SLOT
                                                     (supprimer_logo_de_lentreprise
                                                      ()));


    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
    pushButton_operation_go->enable(this, SLOT(gerer_choix_action()));
}


void YerothAdminWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    _allWindows->_mainWindow->write_AFTER_SETUP_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    stop_TESTING_MAINTENANCE();

    groupBox_yeroth_erp_3_0_parametres_serveur->setVisible(false);

    YerothPOSUser *user = _allWindows->getUser();

    if (0 != user && user->isManager())
    {
        if (user->titre() == "Prof." &&
        	user->nom_complet().startsWith("DR.-ING. DIPL.-INF. XAVIER NOUNDOU"))
        {
            start_TESTING_MAINTENANCE();

            PRINT_SIGNALS_SOURCE_TARGET_info();
        }

        YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        QString localIpAddress;

        if (0 != allWindows)
        {
            localIpAddress = allWindows->getDatabase().db_ip_address();
        }

        groupBox_yeroth_erp_3_0_parametres_serveur->setTitle(QObject::tr
                                                             ("paramètres serveur [ \"%1\" ]").
                                                             arg
                                                             (localIpAddress));

        groupBox_yeroth_erp_3_0_parametres_serveur->setVisible(true);
    }

    if (0 != stocksTableModel)
    {
        _curStocksTableModel = stocksTableModel;
    }

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            tabWidget_administration->setTabEnabled(DONNEES_ENTREPRISE, true);
        }
        else
        {
            tabWidget_administration->setTabEnabled(DONNEES_ENTREPRISE, false);
        }
    }

    set_colour_status_of_alert_daemon_process_buttons();

    read_configuration();

    populateComboBoxes();

    if (YerothERPConfig::_distantSiteConnected)
    {
        comboBox_localisation
			->find_AND_SET_CURRENT_INDEX(YerothERPConfig::_connectedSite);

        lineEdit_localisation_adresse_ip
			->setText(YerothERPConfig::_connectedSiteIPAddress);

        label_ONLINE->setVisible(true);

        label_OFFLINE->setVisible(false);
    }
    else
    {
        comboBox_localisation->resetYerothComboBox();

        lineEdit_localisation_adresse_ip->clear();

        label_ONLINE->setVisible(false);

        label_OFFLINE->setVisible(true);
    }

    _allWindows->_adminListerWindow->lister_remise(&_allWindows->getSqlTableModel_remises());

    _allWindows->_adminListerWindow->lister_alerte(&_allWindows->getSqlTableModel_alertes());

    _allWindows->_adminListerWindow
		->lister_compte_bancaire(&_allWindows->getSqlTableModel_comptes_bancaires());

    _allWindows->_adminListerWindow->lister_categorie(&_allWindows->getSqlTableModel_categories());

    _allWindows->_adminListerWindow
		->lister_LIGNE_BUDGETAIRE(&_allWindows->getSqlTableModel_lignes_budgetaires());

    _allWindows->_adminListerWindow->lister_utilisateur(&_allWindows->getSqlTableModel_users());

    _allWindows->_adminListerWindow
		->lister_departements_de_produits(&_allWindows->getSqlTableModel_departements_produits());

    _allWindows->_adminListerWindow
		->lister_localisation(&_allWindows->getSqlTableModel_localisations());

    handleTabChanged(tabWidget_administration->currentIndex());

    read_entreprise_info_database_table(false);

    setVisible(true);
}


void YerothAdminWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}


void YerothAdminWindow::reinitialiser_la_derniere_selection_du_logo_de_lentreprise()
{
    if (!YerothInfoEntreprise::
            load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO
            (label_logo_de_lentreprise))
    {
        label_logo_de_lentreprise->setAutoFillBackground(false);
    }
}


void YerothAdminWindow::supprimer_logo_de_lentreprise()
{
    YerothERPWindows *allWindows = YerothUtils::getAllWindows();

    if (0 == allWindows)
    {
        return;
    }

    if (0 == label_logo_de_lentreprise->pixmap())
    {
        return;
    }


    YerothSqlTableModel &entreprise_info_TableModel =
                    allWindows->getSqlTableModel_entreprise_info();

    int row = 1;

    QSqlRecord entrepriseInfoRecord = entreprise_info_TableModel.record(row);

    if (entrepriseInfoRecord.isEmpty())
    {
        return;
    }


    QVariant
    un_logo_de_lentreprise(entrepriseInfoRecord.value
                           (YerothDatabaseTableColumn::LOGO_ENTREPRISE));

    if (un_logo_de_lentreprise.toByteArray().isEmpty())
    {
        QString
        msg(QObject::tr("L'entreprise n'a pas un logo enregistré !"));

        YerothQMessageBox::information(this, QObject::tr("suppression"), msg);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du logo de l'entreprise ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression d'1 logo de l'entreprise"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        entrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::LOGO_ENTREPRISE,
                 QVariant(QVariant::ByteArray));


        bool recordUpdated =
                        entreprise_info_TableModel.updateRecord(row,
                        										entrepriseInfoRecord,
																"src/admin/yeroth-erp-admin-window.cpp",
																839);

        if (recordUpdated)
        {
            YerothUtils::supprimer_image(*label_logo_de_lentreprise);

            msgSupprimer =
                            QObject::tr("1 logo de l'entreprise a été supprimée !");

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression d'1 logo de l'entreprise - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("1 logo de l'entreprise n'a pas pu être supprimé !");

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression d'1 logo de l'entreprise - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothAdminWindow::YEROTH_ERP_3_populate_all_tables()
{
    comboBox_sujets_maintenance->addItem(QString("%1 %2").arg
                                         (YerothAdminWindow::
                                          DATABASE_YEROTH_ERP_3_0_KEYWORD,
                                          YerothERPConfig::_db_name));

    QString strShowTABLESQuery("SHOW TABLES");

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strShowTABLESQuery);

    for (int k = 0; k < querySize && query.next(); ++k)
    {
        comboBox_sujets_maintenance->addItem(query.value(0).toString());
    }
}


void YerothAdminWindow::EXECUTER_COMMANDE_MAINTENANCE()
{
    if (lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->isEmpty
            ())
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Sélectionner 1 commande à exécuter !"));

        return;
    }

    QString
    maintenanceCommand
    (lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text
     ());

    QString currentProgram_mysql_mysqldump(YerothUtils::MYSQL);

    maintenanceCommand.replace("'", YerothUtils::EMPTY_STRING);

    int lenToRemoveFromMAINTENANCE_COMMAND = 0;

    if (maintenanceCommand.contains(YerothUtils::MYSQL_DUMP))
    {
        currentProgram_mysql_mysqldump = YerothUtils::MYSQL_DUMP;

        lenToRemoveFromMAINTENANCE_COMMAND = YerothUtils::MYSQL_DUMP.length();
    }
    else if (maintenanceCommand.contains(YerothUtils::MYSQL))
    {
        currentProgram_mysql_mysqldump = YerothUtils::MYSQL;

        lenToRemoveFromMAINTENANCE_COMMAND = YerothUtils::MYSQL.length();
    }
    else
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Commande à exécuter INVALIDE !"));

        return;
    }

//      QDEBUG_STRING_OUTPUT_2("maintenanceCommand", maintenanceCommand);

    QString progArgsString(maintenanceCommand.remove(0,
                                                     lenToRemoveFromMAINTENANCE_COMMAND).
                           trimmed());

    progArgsString.replace("--databases",
                           QString("--databases %1").
                           arg(YerothERPConfig::_db_name));

    progArgsString.replace("db",
                           QString("%1 %2").arg(YerothERPConfig::_db_name,
                                                comboBox_sujets_maintenance->
                                                currentText()));

    progArgsString.replace("-p",
                           QString("-p%1").
                           arg(YerothERPConfig::_db_user_pwd));

//      QDEBUG_STRING_OUTPUT_2("progArgsString", progArgsString);


    QString progArgString_NOT_TO_SPLIT;

    QString progArgString_TO_SPLIT(progArgsString);

    if (progArgsString.contains("--execute"))
    {
        QStringList splitted_cmd_string(progArgsString.split("--execute"));

        progArgString_TO_SPLIT = splitted_cmd_string.at(0).trimmed();

        progArgString_NOT_TO_SPLIT = splitted_cmd_string.at(1).trimmed();

//              QDEBUG_STRING_OUTPUT_2("progArgString_NOT_TO_SPLIT", progArgString_NOT_TO_SPLIT);

//              QDEBUG_STRING_OUTPUT_2("progArgString_TO_SPLIT", progArgString_TO_SPLIT);
    }

    QStringList
    progArguments(progArgString_TO_SPLIT.split
                  (YerothUtils::EMPTY_SPACE_REGEXP));

    QString yeroth_erp_3_0_restore_backup_sql_file(QString("%1.sql").arg
                                                   (FILE_NAME_USERID_CURRENT_TIME
                                                    ("yeroth_erp_3_0_BACKUP_RESTORE")));

    QString
    mysqlProcessProgram(QString("%1/%2").arg
                        (YerothERPConfig::pathToMARIA_DB_BASE_DIR,
                         currentProgram_mysql_mysqldump));

    if (progArgsString.contains("--execute"))
    {
        progArguments.append(QString("--execute%1").
                             arg(progArgString_NOT_TO_SPLIT));
    }

//      QDEBUG_STRINGS_OUTPUT_QSTRINGLIST("progArguments", progArguments);

    int output_file_size =
                    YerothERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE
                    (mysqlProcessProgram,
                     YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY,
                     yeroth_erp_3_0_restore_backup_sql_file,
                     progArguments);

    if (!progArgsString.contains("--execute"))
    {
        YerothUtils::GZIP_YEROTH_FILE
        (YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY,
         QString("%1/%2").arg
         (YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY,
          yeroth_erp_3_0_restore_backup_sql_file));
    }


    QString
    userViewPrettyCommand(QString("%1 %2").arg
                          (comboBox_operations_maintenance->currentText(),
                           comboBox_sujets_maintenance->currentText()));

    if (output_file_size <= 0 && !progArgsString.contains("--execute"))
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("EXÉCUTER COMMANDE MAINTENANCE"),
                                   QObject::tr
                                   ("Commande non exécutable: \"%1\" (%2) !\n\n"
//                                                              "%3\n\n"
                                    "Répertoire d'exécution: \"%3\".").arg
                                   (mysqlProcessProgram, userViewPrettyCommand,
//                                                               lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text(),
                                    YerothERPConfig::
                                    fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY));
        return;
    }

    if (output_file_size > 0)
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("EXÉCUTER COMMANDE MAINTENANCE"),
                                       QObject::tr
                                       ("La commande \"%1\" a été exécuter avec succès !\n\n"
//                                                              "%2\n\n"
                                        "Répertoire d'exécution: \"%2\".").
                                       arg(userViewPrettyCommand,
//                                                               lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->text(),
                                           YerothERPConfig::
                                           fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY));
    }
}


void YerothAdminWindow::reinitialiser_AFFICHAGE_COMMANDE_MAINTENANCE()
{
    comboBox_sujets_maintenance->setCurrentIndex(0);
    comboBox_operations_maintenance->setCurrentIndex(0);

    label_administration_exporter_yeroth_erp_3->setVisible(true);
    label_administration_effacer_tableau->setVisible(true);
    label_administration_exporter_tableau->setVisible(true);
    label_administration_importer_tableau->setVisible(true);
    label_administration_supprimer_tableau->setVisible(true);

    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->clear();
    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->setVisible
    (true);
    lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_effacer_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_exporter_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_importer_un_tableau->
    setVisible(true);
    lineEdit_administration_maintenance_commandes_supprimer_un_tableau->
    setVisible(true);
}


void YerothAdminWindow::handleCheckboxActiver_SHOW_DATABASE_RAW(int state)
{
    if (Qt::Checked == state)
    {
        YerothERPConfig::SHOW_DATABASE_RAW = true;
    }
    else
    {
        YerothERPConfig::SHOW_DATABASE_RAW = false;
    }
}


void YerothAdminWindow::handleCheckboxActiverRegistreCaisse(int state)
{
    if (Qt::Checked == state)
    {
        YerothERPConfig::ouvrirRegistreDeCaisse = true;
    }
    else
    {
        YerothERPConfig::ouvrirRegistreDeCaisse = false;
    }
}


void
YerothAdminWindow::

handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database(const
                                                                 QString &
                                                                 commande_MAINTENANCE,
                                                                 const
                                                                 QString &
                                                                 sqlTableName)
{
    if (!YerothUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        comboBox_operations_maintenance->setCurrentIndex
        (comboBox_operations_maintenance->findText(EXPORTER));
    }

    changer_commande_YEROTH_LINE_EDIT
    (*lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0,
     sqlTableName);

    label_administration_exporter_yeroth_erp_3->setVisible(true);

    lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0->
    setVisible(true);

    cacher_autres_commandes_YEROTH_LINE_EDIT
    (lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0);
}


void YerothAdminWindow::handle_changer_commande_MAINTENANCE_OPERATION(const
                                                                      QString &
                                                                      commande_MAINTENANCE)
{
    QString sqlTableName(comboBox_sujets_maintenance->currentText());

    if (sqlTableName.contains
            (YerothAdminWindow::DATABASE_YEROTH_ERP_3_0_KEYWORD))
    {
        handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database
        (commande_MAINTENANCE, sqlTableName);
        return;
    }

    if (YerothUtils::isEqualCaseInsensitive(EFFACER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_effacer_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_exporter_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(IMPORTER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_importer_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(SUPPRIMER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_supprimer_un_tableau,
         sqlTableName);
    }
    else
    {
        QDEBUG_STRING_OUTPUT_1(" ## INVALID COMMAND FOR MAINTENANCE ##");
    }
}


void YerothAdminWindow::handle_changer_commande_MAINTENANCE_SUJET(const QString &
                                                                  sqlTableName)
{
    QString
    commande_MAINTENANCE(comboBox_operations_maintenance->currentText());

    if (sqlTableName.contains
            (YerothAdminWindow::DATABASE_YEROTH_ERP_3_0_KEYWORD))
    {
        handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database
        (commande_MAINTENANCE, sqlTableName);
        return;
    }

    if (YerothUtils::isEqualCaseInsensitive(EFFACER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_effacer_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(EXPORTER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_exporter_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(IMPORTER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_importer_un_tableau,
         sqlTableName);
    }
    else if (YerothUtils::isEqualCaseInsensitive(SUPPRIMER, commande_MAINTENANCE))
    {
        changer_commande_YEROTH_LINE_EDIT
        (*lineEdit_administration_maintenance_commandes_supprimer_un_tableau,
         sqlTableName);
    }
    else
    {
        QDEBUG_STRING_OUTPUT_1(" ## INVALID COMMAND FOR MAINTENANCE ##");
    }
}


void YerothAdminWindow::handle_importer_parlant_checkBox(int aState)
{
    if (checkBox_importer_parlant->isChecked())
    {
        setImporterParlant(true);
    }
    else
    {
        setImporterParlant(false);
    }
}


void YerothAdminWindow::choix_registre_de_caisse(const QString &
                                                 labelImpressionSur)
{
    if (YerothUtils::isEqualCaseInsensitive(labelImpressionSur,
                                            YerothUtils::IMPRIMANTE_EPSON_TM_T20ii))
    {
        label_activer_registre_de_caisse->setEnabled(true);
        checkBox_activer_registre_de_caisse->setEnabled(true);

        comboBox_format_de_facture->clear();
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER);
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->setYerothEnabled(true);
    }
    else if (YerothUtils::isEqualCaseInsensitive(labelImpressionSur,
                                                 YerothUtils::IMPRIMANTE_PDF))
    {
        label_activer_registre_de_caisse->setEnabled(false);
        checkBox_activer_registre_de_caisse->setChecked(false);
        checkBox_activer_registre_de_caisse->setEnabled(false);

        comboBox_format_de_facture->clear();
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_PETIT);
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER);
        comboBox_format_de_facture->
        addItem(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER);
        comboBox_format_de_facture->setYerothEnabled(true);
    }
}


void YerothAdminWindow::reset_import_current_selected_csv_file()
{
    if (!lineEdit_fichier_csv_a_importer->text().isEmpty())
    {
        YerothUtils::import_csv_file_content(lineEdit_fichier_csv_a_importer->
                                             text(),
                                             _curCsvFileToImportContentWordList);

        generate_table_header_mapping_entries_for_csv_import();

        for (int k = 0; k < _csvContentIdxToSQLTableImportHeader.size(); ++k)
        {
            _csvContentIdxToSQLTableImportHeader.value(k)->
            resetYerothComboBox();
        }
    }
    else
    {
        clear_csv_import_panel_content_mapping();
    }
}


void YerothAdminWindow::import_current_selected_csv_file()
{
    if (_curCsvFileToImportContentWordList.size() <= 0)
    {
        return;
    }

    QStringList csvHeaderContent =
                    _curCsvFileToImportContentWordList.at(0).
                    split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    int curCsvFileLineSize = csvHeaderContent.size();

    QString dbFieldName;

    YerothERPDatabaseTableColumnInfo *curDatabaseTableColumnInfo = 0;

    YerothComboBox *aMappedComboBox = 0;

    for (int i = 0; i < curCsvFileLineSize; ++i)
    {
        aMappedComboBox = _csvContentIdxToSQLTableImportHeader.value(i);

        if (0 != aMappedComboBox)
        {
            dbFieldName = aMappedComboBox->currentText();

            curDatabaseTableColumnInfo =
                            new YerothERPDatabaseTableColumnInfo(dbFieldName,
                                                                 _dbTableColumnToType.value
                                                                 (dbFieldName));

            _csvContentIdxToDatabaseTableColumnInfo.insert(i,
                                                           curDatabaseTableColumnInfo);

            _allDatabaseTableColumnInfo.append(curDatabaseTableColumnInfo);
        }
    }

    _current_selected_import_table =
                    comboBox_tableaux_mariadb_sql->currentText();

    int successImportCount = 0;

    if (YerothUtils::isEqualCaseInsensitive(_current_selected_import_table,
                                            YerothDatabase::STOCKS))
    {
        YerothERPStockImport erpStockImport(*this,
                                            _curCsvFileToImportContentWordList,
                                            _csvContentIdxToDatabaseTableColumnInfo);

        YerothProgressBar(this)(&erpStockImport,
                                &YerothERPStockImport::import_csv_file,
                                &successImportCount,
                                _curCsvFileToImportContentWordList.size());
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (_current_selected_import_table, YerothDatabase::MARCHANDISES))
    {
        YerothERPMarchandiseImport stockMarchandiseImport(*this,
                                                          _curCsvFileToImportContentWordList,
                                                          _csvContentIdxToDatabaseTableColumnInfo);

        YerothProgressBar(this)(&stockMarchandiseImport,
                                &YerothERPMarchandiseImport::import_csv_file,
                                &successImportCount,
                                _curCsvFileToImportContentWordList.size());
    }


    QString msg;

    if (successImportCount > 0)
    {
        msg = QObject::tr("'%1' entrée(s) du fichier CSV ont "
                              "été importée(s) avec succès !").arg
              (QString::number(successImportCount));


        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("résultat importation fichier CSV"),
                                       msg);
    }
    else
    {
        msg =
                        QObject::tr
                        ("AUCUNE DONNÉE DU FICHIER (.csv) n'a été importée !");


        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("résultat importation fichier CSV"),
                                       msg);
    }
}


bool YerothAdminWindow::generate_table_header_mapping_entries_for_csv_import()
{
    if (_curCsvFileToImportContentWordList.size() <= 1)
    {
        return false;
    }

    QStringList csvHeaderContent =
                    _curCsvFileToImportContentWordList.at(0).
                    split(YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR);

    bool csvFileHasVisibleContentToImport = false;

    int curCsvFileColumnSize = csvHeaderContent.size();

    QString aCsvHeaderString;

    QLabel *aCsvHeaderLabel = 0;

    YerothComboBox *aMappedComboBox = 0;

    for (int k = 0; k < csvHeaderContent.size(); ++k)
    {
        aCsvHeaderString = csvHeaderContent.at(k);

        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(k);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(true);
            aCsvHeaderLabel->setEnabled(true);
            aCsvHeaderLabel->setText(QString("%1 (%2)").arg(aCsvHeaderString,
                                                            QString::number
                                                            (k + 1)));

            csvFileHasVisibleContentToImport = true;
        }
    }

    QMapIterator < int,
                 YerothComboBox *
                 >itIndexToSQLTableImportHeader(_csvContentIdxToSQLTableImportHeader);

    while (itIndexToSQLTableImportHeader.hasNext())
    {
        itIndexToSQLTableImportHeader.next();

        aMappedComboBox = itIndexToSQLTableImportHeader.value();

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(false);
            aMappedComboBox->setEnabled(false);
            aMappedComboBox->clear();
        }
    }

    YerothUtils::fillDBTableColumnNameToDBTableColumnType_TEST
    (comboBox_tableaux_mariadb_sql->currentText(), _dbTableColumnToType,
     _dbTableColumnToIsNotNULL);

    for (int i = 0; i < curCsvFileColumnSize; ++i)
    {
        aMappedComboBox = _csvContentIdxToSQLTableImportHeader.value(i);

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(true);
            aMappedComboBox->setEnabled(true);
            aMappedComboBox->addItems(_dbTableColumnToType.keys());

            csvFileHasVisibleContentToImport = true;
        }
    }

    for (int j = curCsvFileColumnSize; j < MAX_IMPORT_CSV_HEADER_SIZE; ++j)
    {
        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(j);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(false);
            aCsvHeaderLabel->setEnabled(false);
        }
    }

    YerothERPStockImport::_dbTableColumnToIsNotNULL =
                    &_dbTableColumnToIsNotNULL;

    pushButton_importer_fichier_csv_reinitialiser->enable(this,
                                                          SLOT
                                                          (reset_import_current_selected_csv_file
                                                           ()));

    pushButton_importer_fichier_csv_valider->enable(this,
                                                    SLOT
                                                    (import_current_selected_csv_file
                                                     ()));

    return csvFileHasVisibleContentToImport;
}


void YerothAdminWindow::populateComboBoxes()
{
    comboBox_localisation
    	->populateComboBoxRawString(YerothDatabase::LOCALISATIONS,
                              	    YerothDatabaseTableColumn::NOM_LOCALISATION);
}


void YerothAdminWindow::cacher_autres_commandes_YEROTH_LINE_EDIT(YerothLineEdit *
                                                                 aYerothCommandLineEdit)
{
    if (0 != aYerothCommandLineEdit)
    {
        aYerothCommandLineEdit->setVisible(true);

        if (aYerothCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0)
        {
            label_administration_exporter_yeroth_erp_3->setVisible(false);
            lineEdit_administration_maintenance_commandes_exporter_yerotherp3_0->
            setVisible(false);
        }
        else
        {
            label_administration_exporter_yeroth_erp_3->setVisible(true);
        }

        if (aYerothCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_effacer_un_tableau)
        {
            label_administration_effacer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_effacer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_effacer_tableau->setVisible(true);
        }

        if (aYerothCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_exporter_un_tableau)
        {
            label_administration_exporter_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_exporter_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_exporter_tableau->setVisible(true);
        }

        if (aYerothCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_importer_un_tableau)
        {
            label_administration_importer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_importer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_importer_tableau->setVisible(true);
        }

        if (aYerothCommandLineEdit !=
                lineEdit_administration_maintenance_commandes_supprimer_un_tableau)
        {
            label_administration_supprimer_tableau->setVisible(false);
            lineEdit_administration_maintenance_commandes_supprimer_un_tableau->
            setVisible(false);
        }
        else
        {
            label_administration_supprimer_tableau->setVisible(true);
        }
    }
}


void YerothAdminWindow::changer_commande_YEROTH_LINE_EDIT(YerothLineEdit &
                                                          aYerothCommandLineEdit,
                                                          const QString &
                                                          sqlTableName)
{
    cacher_autres_commandes_YEROTH_LINE_EDIT(&aYerothCommandLineEdit);

    QString curCOMMANDString(aYerothCommandLineEdit.text());

    curCOMMANDString.replace("%1",
                             QString("%1").
                             arg(YerothERPConfig::_db_user_name));

    curCOMMANDString.replace("%3",
                             QString("%1").arg(YerothERPConfig::_db_name));

    if (sqlTableName.contains
            (YerothAdminWindow::DATABASE_YEROTH_ERP_3_0_KEYWORD))
    {
        curCOMMANDString.replace("%2",
                                 QString("%1").
                                 arg(YerothERPConfig::_db_name));
    }
    else
    {
        curCOMMANDString.replace("%2",
                                 QString("%1.%2").
                                 arg(YerothERPConfig::_db_name,
                                     sqlTableName));
    }

//      QDEBUG_STRING_OUTPUT_2("cur_COMMAND_string", curCOMMANDString);

    lineEdit_administration_maintenance_commandes_COMMANDE_ACTUELLE->setText
    (curCOMMANDString);
}


void YerothAdminWindow::clear_csv_import_panel_content_mapping()
{
    QLabel *aCsvHeaderLabel = 0;

    for (int k = 0; k < _csvContentIdxToCsvFileContentImportHeader.size(); ++k)
    {
        aCsvHeaderLabel = _csvContentIdxToCsvFileContentImportHeader.value(k);

        if (0 != aCsvHeaderLabel)
        {
            aCsvHeaderLabel->setVisible(false);
            aCsvHeaderLabel->setEnabled(false);
            aCsvHeaderLabel->setText(YerothUtils::EMPTY_STRING);
        }
    }

    QMapIterator < int,
                 YerothComboBox *
                 >itIndexToSQLTableImportHeader(_csvContentIdxToSQLTableImportHeader);

    YerothComboBox *aMappedComboBox = 0;

    while (itIndexToSQLTableImportHeader.hasNext())
    {
        itIndexToSQLTableImportHeader.next();

        aMappedComboBox = itIndexToSQLTableImportHeader.value();

        if (0 != aMappedComboBox)
        {
            aMappedComboBox->setVisible(false);
            aMappedComboBox->setEnabled(false);
            aMappedComboBox->clear();
        }
    }

    pushButton_importer_fichier_csv_reinitialiser->setEnabled(false);

    pushButton_importer_fichier_csv_valider->setEnabled(false);
}


void YerothAdminWindow::initialize_admin_importer_csv_tableau()
{
    pushButton_fichier_csv_a_importer->enable(this,
                                              SLOT
                                              (choose_fichier_csv_a_importer
                                               ()));

    _csvContentIdxToCsvFileContentImportHeader.insert(0,
                                                      label_importer_fichier_csv_entete_0);
    _csvContentIdxToCsvFileContentImportHeader.insert(1,
                                                      label_importer_fichier_csv_entete_1);
    _csvContentIdxToCsvFileContentImportHeader.insert(2,
                                                      label_importer_fichier_csv_entete_2);
    _csvContentIdxToCsvFileContentImportHeader.insert(3,
                                                      label_importer_fichier_csv_entete_3);
    _csvContentIdxToCsvFileContentImportHeader.insert(4,
                                                      label_importer_fichier_csv_entete_4);
    _csvContentIdxToCsvFileContentImportHeader.insert(5,
                                                      label_importer_fichier_csv_entete_5);
    _csvContentIdxToCsvFileContentImportHeader.insert(6,
                                                      label_importer_fichier_csv_entete_6);
    _csvContentIdxToCsvFileContentImportHeader.insert(7,
                                                      label_importer_fichier_csv_entete_7);
    _csvContentIdxToCsvFileContentImportHeader.insert(8,
                                                      label_importer_fichier_csv_entete_8);
    _csvContentIdxToCsvFileContentImportHeader.insert(9,
                                                      label_importer_fichier_csv_entete_9);
    _csvContentIdxToCsvFileContentImportHeader.insert(10,
                                                      label_importer_fichier_csv_entete_10);
    _csvContentIdxToCsvFileContentImportHeader.insert(11,
                                                      label_importer_fichier_csv_entete_11);
    _csvContentIdxToCsvFileContentImportHeader.insert(12,
                                                      label_importer_fichier_csv_entete_12);
    _csvContentIdxToCsvFileContentImportHeader.insert(13,
                                                      label_importer_fichier_csv_entete_13);
    _csvContentIdxToCsvFileContentImportHeader.insert(14,
                                                      label_importer_fichier_csv_entete_14);
    _csvContentIdxToCsvFileContentImportHeader.insert(15,
                                                      label_importer_fichier_csv_entete_15);

    for (int k = 0; k < _csvContentIdxToCsvFileContentImportHeader.size(); ++k)
    {
        _csvContentIdxToCsvFileContentImportHeader.value(k)->
        setVisible(false);
    }

    _csvContentIdxToSQLTableImportHeader.insert(0,
                                                comboBox_importer_tableau_entete_0);
    _csvContentIdxToSQLTableImportHeader.insert(1,
                                                comboBox_importer_tableau_entete_1);
    _csvContentIdxToSQLTableImportHeader.insert(2,
                                                comboBox_importer_tableau_entete_2);
    _csvContentIdxToSQLTableImportHeader.insert(3,
                                                comboBox_importer_tableau_entete_3);
    _csvContentIdxToSQLTableImportHeader.insert(4,
                                                comboBox_importer_tableau_entete_4);
    _csvContentIdxToSQLTableImportHeader.insert(5,
                                                comboBox_importer_tableau_entete_5);
    _csvContentIdxToSQLTableImportHeader.insert(6,
                                                comboBox_importer_tableau_entete_6);
    _csvContentIdxToSQLTableImportHeader.insert(7,
                                                comboBox_importer_tableau_entete_7);
    _csvContentIdxToSQLTableImportHeader.insert(8,
                                                comboBox_importer_tableau_entete_8);
    _csvContentIdxToSQLTableImportHeader.insert(9,
                                                comboBox_importer_tableau_entete_9);
    _csvContentIdxToSQLTableImportHeader.insert(10,
                                                comboBox_importer_tableau_entete_10);
    _csvContentIdxToSQLTableImportHeader.insert(11,
                                                comboBox_importer_tableau_entete_11);
    _csvContentIdxToSQLTableImportHeader.insert(12,
                                                comboBox_importer_tableau_entete_12);
    _csvContentIdxToSQLTableImportHeader.insert(13,
                                                comboBox_importer_tableau_entete_13);
    _csvContentIdxToSQLTableImportHeader.insert(14,
                                                comboBox_importer_tableau_entete_14);
    _csvContentIdxToSQLTableImportHeader.insert(15,
                                                comboBox_importer_tableau_entete_15);

    for (int k = 0; k < _csvContentIdxToSQLTableImportHeader.size(); ++k)
    {
        _csvContentIdxToSQLTableImportHeader.value(k)->setVisible(false);
    }


    QSqlQuery databaseTableNameQuery;

    YerothERPWindows *yerothERPWindows = YerothUtils::getAllWindows();

    if (0 != yerothERPWindows)
    {
        comboBox_tableaux_mariadb_sql->addItem(YerothDatabase::MARCHANDISES);
        comboBox_tableaux_mariadb_sql->addItem(YerothDatabase::STOCKS);
    }
}


void YerothAdminWindow::creer(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminCreateWindow
        ->rendreVisible(_curStocksTableModel,
                        selectedSujetAction);
    rendreInvisible();
}

void YerothAdminWindow::lister(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);
    rendreInvisible();
}


void YerothAdminWindow::modifier(enum AdminSujetAction selectedSujetAction)
{
    _allWindows->_adminListerWindow->rendreVisible(selectedSujetAction);
    rendreInvisible();
}


void YerothAdminWindow::DISABLE_YR_DB_RUNTIME_VERIF_BUTTONS()
{
	tabWidget_maintenance_verification_DATABASE->setTabVisible(0, false);

	groupBox_yr_db_runtime_verif->setVisible(false);

	groupBox_yr_db_runtime_verif->setEnabled(false);

	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(false);

	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(false);

	label_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_START_DB_RUNTIME_VERIFICATOR->setVisible(false);

    pushButton_STOP_DB_RUNTIME_VERIFICATOR->setVisible(false);
}


void YerothAdminWindow::ENABLE_YR_DB_RUNTIME_VERIF_BUTTONS()
{
	tabWidget_maintenance_verification_DATABASE->setTabVisible(0, true);

	groupBox_yr_db_runtime_verif->setVisible(true);

	groupBox_yr_db_runtime_verif->setEnabled(true);

	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(true);

	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(true);

    pushButton_REFRESH_STATUS_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS()));

    pushButton_START_DB_RUNTIME_VERIFICATOR
		->enable(this,
                 SLOT(START_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    pushButton_STOP_DB_RUNTIME_VERIFICATOR
		->enable(this,
				 SLOT(STOP_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()));

    REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS();
}


void YerothAdminWindow::stop_TESTING_MAINTENANCE()
{
    int indexEffacer = comboBox_operations_maintenance->findText(EFFACER);

    if (indexEffacer > -1)
    {
        comboBox_operations_maintenance->removeItem(indexEffacer);
    }

    int indexSupprimer = comboBox_operations_maintenance->findText(SUPPRIMER);

    if (indexSupprimer > -1)
    {
        comboBox_operations_maintenance->removeItem(indexSupprimer);
    }

    groupBox_maintenance_commandes->setVisible(false);

    groupBox_maintenance_commandes->setEnabled(false);

    DISABLE_YR_DB_RUNTIME_VERIF_BUTTONS();
}


void YerothAdminWindow::start_TESTING_MAINTENANCE()
{
    int indexEffacer = comboBox_operations_maintenance->findText(EFFACER);

    if (indexEffacer < 0)
    {
        comboBox_operations_maintenance->addItem(EFFACER);
    }

    int indexSupprimer = comboBox_operations_maintenance->findText(SUPPRIMER);

    if (indexSupprimer < 0)
    {
        comboBox_operations_maintenance->addItem(SUPPRIMER);
    }

    lineEdit_administration_maintenance_commandes_importer_un_tableau->setText
    (QString("mysql -u '%1' -p < '%2'"));

    lineEdit_administration_maintenance_commandes_effacer_un_tableau->setText
    (QString("mysql -u '%1' -p --execute=truncate table %2"));

    lineEdit_administration_maintenance_commandes_supprimer_un_tableau->setText
    (QString("mysql -u '%1' -p --execute=drop table %2"));

    groupBox_maintenance_commandes->setVisible(true);

    groupBox_maintenance_commandes->setEnabled(true);

    ENABLE_YR_DB_RUNTIME_VERIF_BUTTONS();
}


void YerothAdminWindow::enableAllOperationsTabPushButtons()
{
    pushButton_creer->enable(this, SLOT(action_creer()));
    pushButton_lister->enable(this, SLOT(action_lister()));
    pushButton_modifier->enable(this, SLOT(action_modifier()));
    pushButton_supprimer->enable(this, SLOT(action_lister()));
}

void YerothAdminWindow::disableAllOperationsTabPushButtons()
{
    pushButton_creer->disable(this);
    pushButton_lister->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}

void YerothAdminWindow::
	setCurrentAdminWindowTitle(enum YerothAdminWindowTabPositionsTypes tabPosition)
{
    QStringList adminWindowTitleParts =
                    _yerothAdminWindowTitleStart.split('~');

    //adminWindowTitleParts.at(0) contient le string 'Yeroth-pgi-3.0'.

    if (2 <= adminWindowTitleParts.size())
    {
        QString newAdminWindowTitle(adminWindowTitleParts.at(0).trimmed());

        newAdminWindowTitle.append(" ~ ")
        				   .append(tabWidget_administration->tabText(tabPosition))
						   .append(" ~ ")
						   .append(adminWindowTitleParts.at(1).trimmed());


        setWindowTitle(newAdminWindowTitle);
    }
}

void YerothAdminWindow::handleTabChanged(int currentTab)
{
    switch (currentTab)
    {
    case CONNECTER_LOCALISATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(CONNECTER_LOCALISATION);
        break;

    case DONNEES_ENTREPRISE:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(DONNEES_ENTREPRISE);
        break;

    case IMPORTER_CSV_TABLEAU:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(IMPORTER_CSV_TABLEAU);
        break;

    case MAINTENANCE:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(MAINTENANCE);
        break;

    case OPERATIONS:
        enableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(OPERATIONS);
        break;

    case PARAMETRES_APPLICATION:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(PARAMETRES_APPLICATION);
        break;

    case SYSTEME_DALERTES:
        disableAllOperationsTabPushButtons();
        setCurrentAdminWindowTitle(SYSTEME_DALERTES);
        break;

    default:
        disableAllOperationsTabPushButtons();
        setWindowTitle(_yerothAdminWindowTitleStart);
        break;
    }
}

void YerothAdminWindow::action_creer()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        creer(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        creer(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        creer(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        creer(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        creer(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        creer(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        creer(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        creer(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        creer(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_lister()
{
//    _curAdminSujetAction =
//    		_sujetActionsToConst->value(comboBox_sujets_actions->currentText());

    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        lister(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        lister(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        lister(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        lister(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        lister(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        lister(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        lister(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        lister(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        lister(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_modifier()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());

    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        modifier(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        modifier(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        modifier(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_supprimer()
{
//    _curAdminSujetAction =
//                    _sujetActionsToConst->value(comboBox_sujets_actions->currentText());
    switch (_curAdminSujetAction)
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        modifier(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        modifier(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        modifier(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        modifier(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        modifier(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        modifier(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_ALERTE:
        modifier(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_REMISE:
        modifier(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
        modifier(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_exporter()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_exporter(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_exporter(SUJET_ACTION_TABLEAU);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_importer()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::action_effacer()
{
    _curAdminSujetsMaintainAction =
    		_sujetActionsToConst->value(comboBox_sujets_maintenance->currentText());

    switch (_curAdminSujetsMaintainAction)
    {
    case SUJET_ACTION_DB:
        maintenance_importer(SUJET_ACTION_DB);
        break;

    case SUJET_ACTION_TABLEAU:
        maintenance_importer(SUJET_ACTION_TABLEAU);
        break;

    case SUJET_ACTION_DONNEES:
        maintenance_effacer(SUJET_ACTION_DONNEES);
        break;

    default:
        break;
    }
}

void YerothAdminWindow::gerer_choix_action()
{
    //qDebug() << "YerothAdminWindow::gerer_choix_action()";
	_curAdminAction = _actionsToConst->value(comboBox_actions->currentText());

	_curAdminSujetAction = (enum AdminSujetAction) comboBox_sujets_actions->currentIndex();

    switch (_curAdminAction)
    {
    case ACTION_CREER:
        action_creer();
        break;

    case ACTION_LISTER:
        action_lister();
        break;

    case ACTION_MODIFIER:
        action_modifier();
        break;

    case ACTION_SUPPRIMER:
        action_supprimer();
        break;

    case ACTION_EXPORTER:
        action_exporter();
        break;

    case ACTION_IMPORTER:
        action_importer();
        break;

    default:
        break;
    }
}


void YerothAdminWindow::choose_fichier_csv_a_importer()
{
    QString csvFilePath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au fichier au format (.csv)"),
										 QString::null,
										 QObject::tr(".csv (*.csv)"));
    if (!csvFilePath.isEmpty())
    {
        lineEdit_fichier_csv_a_importer->setText(csvFilePath);

        YerothUtils::import_csv_file_content(csvFilePath,
                                             _curCsvFileToImportContentWordList);

        generate_table_header_mapping_entries_for_csv_import();
    }
}


void YerothAdminWindow::choose_path_mariadb_client_dir()
{
    QString mariaDBClientBASE_DIR_FULL_PATH =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au répertoire de base de 'MariaDB-client'"),
										 QString::null,
										 QString::null);

    if (!mariaDBClientBASE_DIR_FULL_PATH.isEmpty())
    {
        lineEdit_mariadb_client_base_dir_full_path
			->setText(mariaDBClientBASE_DIR_FULL_PATH);
    }
}


void YerothAdminWindow::choose_path_pdfReader()
{
    QString pdfReaderFilePath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Choisir le chemin qui mène au lecteur de fichiers PDF"),
										 QString::null,
										 QString::null);

    if (!pdfReaderFilePath.isEmpty())
    {
        lineEdit_pdfReader->setText(pdfReaderFilePath);
    }
}


void YerothAdminWindow::choose_path_thermalPrinterDeviceFile()
{
    QString thermalPrinterDeviceFileFullPath =
    		QFileDialog::getOpenFileName(this,
    									 QObject::tr("Chemin jusqu'au fichier système représentant l'imprimante thermique"),
										 QString::null,
										 QString::null);

    if (!thermalPrinterDeviceFileFullPath.isEmpty())
    {
        lineEdit_fichier_systeme_imprimante_thermique
			->setText(thermalPrinterDeviceFileFullPath);
    }
}


void YerothAdminWindow::choose_path_latex_system_root_folder()
{
    QString latexSystemRootFolderPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::trUtf8("Choisir le chemin jusqu'au répertoire du système de documentation Latex"),
											  QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!latexSystemRootFolderPath.isEmpty())
    {
        lineEdit_repertoire_systeme_latex->setText(latexSystemRootFolderPath);
    }
}


void YerothAdminWindow::choose_repertoire_des_sauvegardes()
{
    QString backupFileDirPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::tr("Choisir le chemin qui mène au répertoire des sauvegardes"),
                                              QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!backupFileDirPath.isEmpty())
    {
        lineEdit_repertoire_des_sauvegardes->setText(backupFileDirPath);
    }
}


void YerothAdminWindow::choose_repertoire_fichiers_temporaires()
{
    QString temporaryFileDirPath =
    		QFileDialog::getExistingDirectory(this,
    										  QObject::tr("Choisir le chemin qui mène au répertoire des fichiers temporaires"),
											  QString::null,
											  QFileDialog::ShowDirsOnly);

    if (!temporaryFileDirPath.isEmpty())
    {
        lineEdit_repertoire_fichiers_temporaires
			->setText(temporaryFileDirPath);
    }
}


void YerothAdminWindow::read_configuration()
{
	checkBox_activer_RAW_DB_TABLE_column
		->setChecked(YerothERPConfig::SHOW_DATABASE_RAW);

    checkBox_activer_registre_de_caisse
		->setChecked(YerothERPConfig::ouvrirRegistreDeCaisse);

    lineEdit_devise->setText(YerothERPConfig::currency);

    lineEdit_mariadb_client_base_dir_full_path
		->setText(YerothERPConfig::pathToMARIA_DB_BASE_DIR);

    lineEdit_pdfReader->setText(YerothERPConfig::pathToPdfReader);

    lineEdit_fichier_systeme_imprimante_thermique
		->setText(YerothERPConfig::pathToThermalPrinterDeviceFile);

    lineEdit_repertoire_systeme_latex
		->setText(YerothERPConfig::pathToLatexSystemRootFolder);


    lineEdit_sauvegarde_de_yeroth_erp_3_secondes
		->setText(QString::number(YerothERPConfig::backup_restore_yeroth_erp_3_time_interval));


    lineEdit_alert_period_time_interval
		->setText(QString::number(YerothERPConfig::alert_period_time_interval));


    lineEdit_alert_quantity_time_interval
		->setText(QString::number(YerothERPConfig::alert_quantity_time_interval));


    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE
		->setText(QString::number(YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING));


    lineEdit_longueur_maximale_string
		->setText(QString::number(YerothERPConfig::max_string_display_length));


    lineEdit_taille_de_pagination_par_defaut
		->setText(QString::number(YerothERPConfig::pagination_number_of_table_rows));


    lineEdit_MAINTENANCE_VERIFICATION_log_size_max
		->setText(QString::number(YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT));


    lineEdit_repertoire_fichiers_temporaires->setText(YerothERPConfig::temporaryFilesDir);

    lineEdit_tva_value->setText(YerothUtils::getTvaString());


    lineEdit_annee_depart_rapports_chiffre_affaire
		->setText(YerothERPConfig::annee_depart_rapports_chiffre_affaire_value);


    lineEdit_repertoire_des_sauvegardes
		->setText(YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY);


    lineEdit_annee_de_depart_pour_la_pagination
		->setText(YerothERPConfig::annee_depart_pour_la_pagination);


    lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE
		->setText(YerothERPConfig::IP_ADDRESS_NETWORK_printer);


    comboBox_strategie_vente_sortie->clear();

    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

    comboBox_strategie_vente_sortie->addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

    if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::salesStrategy,
                                            YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerothUtils::STRATEGIE_ALL_COMBOBOX_INDEX);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::salesStrategy,
              YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerothUtils::STRATEGIE_FEFO_COMBOBOX_INDEX);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::salesStrategy,
              YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerothUtils::STRATEGIE_FIFO_COMBOBOX_INDEX);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::salesStrategy,
              YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
    {
        comboBox_strategie_vente_sortie
			->setCurrentIndex(YerothUtils::STRATEGIE_LIFO_COMBOBOX_INDEX);
    }

    comboBox_impression_sur
		->addItems_AS_INITIALIZATION(YerothERPConfig::printer,
									 YerothERPConfig::printer,
									 comboBox_impression_sur_CHOICE_list);
}


/*void YerothAdminWindow::handle__tabWidget_maintenance_verification_DATABASE(const QString &CURRENT_INDEX)
{
	uint current_IDX = CURRENT_INDEX.toUInt();

	switch (current_IDX)
	{
	case 0:
		break;

	case 1:
		break;

	default:
		break;
	}
}*/


void YerothAdminWindow::handleThermalPrinterConfiguration(const QString &
                                                          addedPrinterValue)
{
    comboBox_format_de_facture->
    addItems_AS_INITIALIZATION(YerothERPConfig::receiptFormat,
                               YerothERPConfig::receiptFormat,
                               comboBox_format_de_facture_CHOICE_list);

    comboBox_format_de_facture->setYerothEnabled(true);

    if (YerothUtils::isEqualCaseInsensitive
            (YerothUtils::IMPRIMANTE_EPSON_TM_T20ii, addedPrinterValue))
    {
        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (true);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (true);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU, addedPrinterValue))
    {
        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->checkField();

        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(true);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(true);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);
    }
    else
    {
        label_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->setVisible(false);

        label_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);

        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setVisible
        (false);
    }
}


void YerothAdminWindow::read_entreprise_info_database_table(bool
                                                            initalizationValues
                                                            /* = true */)
{
    YerothERPWindows *allWindows = YerothUtils::getAllWindows();

    if (0 != allWindows)
    {
        YerothSqlTableModel &entreprise_info_TableModel =
                        allWindows->getSqlTableModel_entreprise_info();

        int row = 0;

        if (true == initalizationValues)
        {
            QString msgReinit(QObject::tr
                              ("Réinitialiser les données de l'entreprise "
                               "avec les paramètres par défaut ?"));

            if (QMessageBox::Ok == YerothQMessageBox::question(this,
                                                               QObject::tr
                                                               ("réinitialiser"),
                                                               msgReinit,
                                                               QMessageBox::
                                                               Cancel,
                                                               QMessageBox::
                                                               Ok))
            {
                row = 0;
            }
            else
            {
                return;
            }
        }
        else
        {
            row = 1;
        }


        if (false == initalizationValues)
        {
            if (!YerothInfoEntreprise::
                    load_COMPANY_INFO_LOGO_FROM_DB_TABLE_ENTREPRISE_INFO
                    (label_logo_de_lentreprise))
            {
                label_logo_de_lentreprise->setAutoFillBackground(false);
            }
        }


        QSqlRecord entrepriseInfoRecord =
                        entreprise_info_TableModel.record(row);

        lineEdit_entreprise_denomination_de_lentreprise->setText(QString
                                                                 (GET_SQL_RECORD_DATA
                                                                  (entrepriseInfoRecord,
                                                                   1)));

        lineEdit_entreprise_siege_social->setText(QString
                                                  (GET_SQL_RECORD_DATA
                                                   (entrepriseInfoRecord, 2)));

        lineEdit_entreprise_localisation->setYerothEnabled(false);

        lineEdit_entreprise_localisation->
        setText(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

        lineEdit_entreprise_boite_postale->setText(QString
                                                   (GET_SQL_RECORD_DATA
                                                    (entrepriseInfoRecord,
                                                     4)));

        lineEdit_entreprise_adresse->setText(QString
                                             (GET_SQL_RECORD_DATA
                                              (entrepriseInfoRecord, 5)));

        lineEdit_entreprise_ville->setText(QString
                                           (GET_SQL_RECORD_DATA
                                            (entrepriseInfoRecord, 6)));

        lineEdit_entreprise_pays->setText(QString
                                          (GET_SQL_RECORD_DATA
                                           (entrepriseInfoRecord, 7)));

        lineEdit_entreprise_email->setText(QString
                                           (GET_SQL_RECORD_DATA
                                            (entrepriseInfoRecord, 8)));

        lineEdit_entreprise_telephone->setText(QString
                                               (GET_SQL_RECORD_DATA
                                                (entrepriseInfoRecord, 9)));

        lineEdit_entreprise_numero_de_contribuable->setText(QString
                                                            (GET_SQL_RECORD_DATA
                                                             (entrepriseInfoRecord,
                                                              10)));

        lineEdit_entreprise_secteurs_dactivites->setText(QString
                                                         (GET_SQL_RECORD_DATA
                                                          (entrepriseInfoRecord,
                                                           11)));

        lineEdit_entreprise_agence_du_compte_bancaire->setText(QString
                                                               (GET_SQL_RECORD_DATA
                                                                (entrepriseInfoRecord,
                                                                 12)));

        lineEdit_entreprise_reference_du_compte_bancaire->setText(QString
                                                                  (GET_SQL_RECORD_DATA
                                                                   (entrepriseInfoRecord,
                                                                    13)));

        lineEdit_entreprise_reference_registre_du_commerce->setText(QString
                                                                    (GET_SQL_RECORD_DATA
                                                                     (entrepriseInfoRecord,
                                                                      14)));

        lineEdit_entreprise_denomination_de_lentreprise_SUR_LA_CARTE_DE_FIDELITE_CLIENT->
        setText(GET_SQL_RECORD_DATA
                (entrepriseInfoRecord,
                 YerothDatabaseTableColumn::
                 CARTE_DE_FIDELITE_CLIENT_nom_entreprise));

        lineEdit_entreprise_service_de_gestion_de_la_clientele_SIEGE->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege));

        lineEdit_entreprise_service_de_gestion_de_la_clientele->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerothDatabaseTableColumn::
          CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele));

        lineEdit_entreprise_service_du_programme_de_fidelite_clients->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerothDatabaseTableColumn::
          CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client));

        lineEdit_entreprise_email_du_service_de_gestion_de_la_clientele->setText
        (GET_SQL_RECORD_DATA
         (entrepriseInfoRecord,
          YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email));

        lineEdit_entreprise_telephone_du_service_de_gestion_de_la_clientele->
        setText(GET_SQL_RECORD_DATA
                (entrepriseInfoRecord,
                 YerothDatabaseTableColumn::
                 CARTE_DE_FIDELITE_CLIENT_telephone));
    }
}


void YerothAdminWindow::read_YEROTH_ERP_3_0_SYSTEM_DAEMON_init_configuration()
{
    YerothSqlTableModel &initConfigurationsTableModel =
                    _allWindows->getSqlTableModel_init_configurations();

    //***
    QSqlRecord initConfigurationRecord =
                    initConfigurationsTableModel.record
                    (YerothERPConfig::
                     CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3);

    QString CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE
    (GET_SQL_RECORD_DATA(initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.record
                    (YerothERPConfig::CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL);

    QString
    BACKUP_RESTORE_DATABASE_YEROTH_ERP_3_IntervalValue(GET_SQL_RECORD_DATA
                                                       (initConfigurationRecord,
                                                        "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);

    QString
    alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA
                                 (initConfigurationRecord,
                                  "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);

    QString
    alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA
                                   (initConfigurationRecord,
                                    "valeur_configuration"));


    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerothERPConfig::CONFIG_SALES_STRATEGY);

    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));


    lineEdit_repertoire_des_sauvegardes->setText
    (CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE);

    lineEdit_sauvegarde_de_yeroth_erp_3_secondes->setText
    (BACKUP_RESTORE_DATABASE_YEROTH_ERP_3_IntervalValue);

    lineEdit_alert_period_time_interval->setText(alertPeriodTimeIntervalValue);

    lineEdit_alert_quantity_time_interval->setText
    (alertQuantityTimeIntervalValue);
}

void YerothAdminWindow::read_app_parameters_init_configuration()
{
    YerothSqlTableModel &initConfigurationsTableModel =
                    _allWindows->getSqlTableModel_init_configurations();

    //***
    QSqlRecord initConfigurationRecord =
                    initConfigurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
    QString
    tvaValue(GET_SQL_RECORD_DATA
             (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerothERPConfig::CONFIG_SALES_STRATEGY);
    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
    QString
    currencyValue(GET_SQL_RECORD_DATA
                  (initConfigurationRecord, "valeur_configuration"));


    //***
    initConfigurationRecord =
                    initConfigurationsTableModel.
                    record(YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
    QString
    pageBeginYearValue(GET_SQL_RECORD_DATA
                       (initConfigurationRecord, "valeur_configuration"));

    QString
    userLanguageReceiptFormatValue
    (YerothUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage
     (YerothERPConfig::receiptFormat));

    /*
     * SHOW DATABASE table column RAW INITIAL CONFIGURATION
     */
    if (YerothERPConfig::SHOW_DATABASE_RAW)
    {
    	checkBox_activer_RAW_DB_TABLE_column->setChecked(true);
    }
    else
    {
    	checkBox_activer_RAW_DB_TABLE_column->setChecked(false);
    }

    /*
     * OPEN_CASH_DRAWER INITIALIZATION CONFIGURATION
     */
    if (YerothERPConfig::ouvrirRegistreDeCaisse)
    {
        checkBox_activer_registre_de_caisse->setChecked(true);
    }
    else
    {
        checkBox_activer_registre_de_caisse->setChecked(false);
    }

    /*
     * PRINTER INITIALIZATION CONFIGURATION
     */
    comboBox_impression_sur->
    addItems_AS_INITIALIZATION(YerothERPConfig::printer,
                               YerothUtils::IMPRIMANTE_PDF,
                               comboBox_impression_sur_CHOICE_list);

    comboBox_format_de_facture->addItems_AS_INITIALIZATION
    (userLanguageReceiptFormatValue, userLanguageReceiptFormatValue,
     comboBox_format_de_facture_CHOICE_list);

    lineEdit_devise->setText(currencyValue);

    lineEdit_mariadb_client_base_dir_full_path->
    setText(YerothERPConfig::pathToMARIA_DB_BASE_DIR);

    lineEdit_pdfReader->setText(YerothERPConfig::pathToPdfReader);

    lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->setText
    (QString::number
     (YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING));

    lineEdit_longueur_maximale_string->
    setText(QString::number(YerothERPConfig::max_string_display_length));

    lineEdit_taille_de_pagination_par_defaut->
		setText(QString::number(YerothERPConfig::pagination_number_of_table_rows));

    lineEdit_MAINTENANCE_VERIFICATION_log_size_max->
		setText(QString::number(YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT));

    lineEdit_fichier_systeme_imprimante_thermique->
    setText(YerothERPConfig::pathToThermalPrinterDeviceFile);

    lineEdit_repertoire_systeme_latex->
    setText(YerothERPConfig::pathToLatexSystemRootFolder);

    lineEdit_repertoire_fichiers_temporaires->
    setText(YerothERPConfig::temporaryFilesDir);

    lineEdit_tva_value->setText(tvaValue);

    if (!pageBeginYearValue.isEmpty())
    {
        lineEdit_annee_de_depart_pour_la_pagination->setText
        (pageBeginYearValue);
    }
    else
    {
        lineEdit_annee_de_depart_pour_la_pagination->setText
        (GET_CURRENT_DATE_YEAR);
    }

    lineEdit_annee_depart_rapports_chiffre_affaire->
    setText(YerothERPConfig::annee_depart_rapports_chiffre_affaire_value);

    {
        comboBox_strategie_vente_sortie->clear();
        //qDebug() << "++ test";
        comboBox_strategie_vente_sortie->
        addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);
        comboBox_strategie_vente_sortie->
        addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);
        comboBox_strategie_vente_sortie->
        addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);
        comboBox_strategie_vente_sortie->
        addItem(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);

        int salesStrategyId =
                        comboBox_strategie_vente_sortie->findText(salesStrategyValue);

        comboBox_strategie_vente_sortie->setCurrentIndex(salesStrategyId);
    }
}

void YerothAdminWindow::reinitialiser_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration()
{
    _logger->log("reinitialiser_configuration");

    QString msgReinit(QObject::tr
                      ("Réinitialiser la configuration du 'SYSTEM DAEMON' "
                       "avec les paramètres par défaut ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("réinitialiser"),
                                        msgReinit, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        read_YEROTH_ERP_3_0_SYSTEM_DAEMON_init_configuration();
    }
    else
    {
    }
}

void YerothAdminWindow::reinitialiser_app_parameters_configuration()
{
    _logger->log("reinitialiser_configuration");

    QString
    msgReinit(QObject::tr
              ("Réinitialiser la configuration (générale) "
               "avec les paramètres par défaut ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("réinitialiser"),
                                        msgReinit, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        read_app_parameters_init_configuration();
    }
    else
    {
    }
}


void YerothAdminWindow::enregistrer_entreprise_info_database_table()
{
    _logger->log("enregistrer_entreprise_info_database_table");

    QString
    msgEnregistrer(QObject::tr
                   ("Enregistrer les données de l'entreprise (modifiées) ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        YerothSqlTableModel &entreprise_info_TableModel =
                        _allWindows->getSqlTableModel_entreprise_info();

        QSqlRecord aNewEntrepriseInfoRecord =
                        entreprise_info_TableModel.record(1);

        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                 lineEdit_entreprise_denomination_de_lentreprise->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL,
                 lineEdit_entreprise_siege_social->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::LOCALISATION,
                 YerothERPConfig::THIS_SITE_LOCALISATION_NAME);
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::BOITE_POSTALE,
                 lineEdit_entreprise_boite_postale->text());
        aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::ADRESSE,
                                          lineEdit_entreprise_adresse->
                                          text());
        aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::VILLE,
                                          lineEdit_entreprise_ville->text());
        aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::PAYS,
                                          lineEdit_entreprise_pays->text());
        aNewEntrepriseInfoRecord.setValue(YerothDatabaseTableColumn::EMAIL,
                                          lineEdit_entreprise_email->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1,
                 lineEdit_entreprise_telephone->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                 lineEdit_entreprise_numero_de_contribuable->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::SECTEURS_DACTIVITES,
                 lineEdit_entreprise_secteurs_dactivites->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE,
                 lineEdit_entreprise_agence_du_compte_bancaire->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                 lineEdit_entreprise_reference_du_compte_bancaire->text());
        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 lineEdit_entreprise_reference_registre_du_commerce->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise,
         lineEdit_entreprise_denomination_de_lentreprise_SUR_LA_CARTE_DE_FIDELITE_CLIENT->
         text());

        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege,
                 lineEdit_entreprise_service_de_gestion_de_la_clientele_SIEGE->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerothDatabaseTableColumn::
         CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele,
         lineEdit_entreprise_service_de_gestion_de_la_clientele->text());

        aNewEntrepriseInfoRecord.setValue
        (YerothDatabaseTableColumn::
         CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client,
         lineEdit_entreprise_service_du_programme_de_fidelite_clients->
         text());

        aNewEntrepriseInfoRecord.
        setValue(YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email,
                 lineEdit_entreprise_email_du_service_de_gestion_de_la_clientele->
                 text());

        aNewEntrepriseInfoRecord.setValue
        (YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone,
         lineEdit_entreprise_telephone_du_service_de_gestion_de_la_clientele->
         text());

        if (label_logo_de_lentreprise->pixmap())
        {
            QByteArray bytes;
            YerothUtils::savePixmapToByteArray(bytes,
                                               *label_logo_de_lentreprise->
                                               pixmap(), "JPG");
            aNewEntrepriseInfoRecord.
            setValue(YerothDatabaseTableColumn::LOGO_ENTREPRISE, bytes);
        }

        bool recordUpdated =
                        entreprise_info_TableModel.updateRecord(1,
                        										aNewEntrepriseInfoRecord,
																"src/admin/yeroth-erp-admin-window.cpp",
																3000);

        QString retMesg;

        if (true == recordUpdated)
        {
            retMesg =
                            QObject::tr("Les données commerciales de l'entreprise "
                                            "ont été enregistrées avec succès !");

            YerothQMessageBox::information(this, QObject::tr("succès"),
                                           retMesg);
        }
        else
        {
            retMesg =
                            QObject::tr
                            ("Échec de l'enregistrement des données commerciales de l'entreprise !");

            if (QMessageBox::Ok ==
                    YerothQMessageBox::warning(this, QObject::tr("échec"),
                                               retMesg))
            {
            }
            else
            {
            }
        }
    }
    else
    {
    }

    _allWindows->getInfoEntreprise().toString();
}


void YerothAdminWindow::enregistrer_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration()
{
    _logger->log("enregistrer_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration");

    QString
    msgEnregistrer(QObject::tr
                   ("Enregistrer la configuration (SYSTEM DAEMON) modifiée ?"));
    if (QMessageBox::Cancel ==
            YerothQMessageBox::question(this, QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        return;
    }
    else
    {
        YerothSqlTableModel &configurationsTableModel =
                        _allWindows->getSqlTableModel_configurations();

        bool success = false;

        QSqlRecord configurationsRecord;

        if (lineEdit_sauvegarde_de_yeroth_erp_3_secondes->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.record
                            (YerothERPConfig::
                             CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_sauvegarde_de_yeroth_erp_3_secondes->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord
                      	  (YerothERPConfig::CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL,
                      	   configurationsRecord,
						   "src/admin/yeroth-erp-admin-window.cpp",
						   3075);

            if (success)
            {
                YerothERPConfig::backup_restore_yeroth_erp_3_time_interval =
                                lineEdit_sauvegarde_de_yeroth_erp_3_secondes->text().
                                toUInt();
            }
        }

        if (lineEdit_repertoire_des_sauvegardes->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.record
                            (YerothERPConfig::
                             CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_repertoire_des_sauvegardes->
                                          text());

            bool success = configurationsTableModel.updateRecord
                           	   (YerothERPConfig::CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3,
                           	    configurationsRecord,
								"src/admin/yeroth-erp-admin-window.cpp",
								3101);

            if (success)
            {
                YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY =
                                lineEdit_repertoire_des_sauvegardes->text();
            }
        }

        if (lineEdit_alert_period_time_interval->checkField())
        {
            configurationsRecord.clear();

            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_alert_period_time_interval->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL,
                                   	   	   	   	   	   	    configurationsRecord,
															"src/admin/yeroth-erp-admin-window.cpp",
															3126);

            if (success)
            {
                YerothERPConfig::alert_period_time_interval =
                                lineEdit_alert_period_time_interval->text().toUInt();
            }
        }

        if (lineEdit_alert_quantity_time_interval->checkField())
        {
            configurationsRecord.clear();

            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);

            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_alert_quantity_time_interval->
                                          text());

            success = success ||
                      configurationsTableModel.updateRecord
                      	  (YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL,
                      	   configurationsRecord,
						   "src/admin/yeroth-erp-admin-window.cpp",
						   3151);

            if (success)
            {
                YerothERPConfig::alert_quantity_time_interval =
                                lineEdit_alert_quantity_time_interval->text().toUInt();
            }
        }

//        QDEBUG_STRING_OUTPUT_2("success", BOOL_TO_STRING(success));

        if (success)
        {
            stop_alert_daemon_process();

            start_alert_daemon_process();
        }
    }
}


void YerothAdminWindow::enregistrer_system_local_app_parameters_configuration()
{
    _logger->log("enregistrer_system_local_app_parameters_configuration");

    if (checkBox_activer_RAW_DB_TABLE_column->isChecked())
    {
        YerothERPConfig::SHOW_DATABASE_RAW = true;
    }
    else
    {
        YerothERPConfig::SHOW_DATABASE_RAW = false;
    }

    if (checkBox_activer_registre_de_caisse->isChecked())
    {
        YerothERPConfig::ouvrirRegistreDeCaisse = true;
    }
    else
    {
        YerothERPConfig::ouvrirRegistreDeCaisse = false;
    }

    if (lineEdit_fichier_systeme_imprimante_thermique->checkField())
    {
        YerothERPConfig::pathToThermalPrinterDeviceFile =
                        lineEdit_fichier_systeme_imprimante_thermique->text();
    }

    if (lineEdit_repertoire_systeme_latex->checkField())
    {
        YerothERPConfig::pathToLatexSystemRootFolder =
                        lineEdit_repertoire_systeme_latex->text();
    }

    if (lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->checkField
            ())
    {
        YerothERPConfig::LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING =
                        lineEdit_nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE->text().
                        toInt();
    }

    if (lineEdit_longueur_maximale_string->checkField())
    {
        YerothERPConfig::max_string_display_length =
                        lineEdit_longueur_maximale_string->text().toInt();
    }

    if (lineEdit_taille_de_pagination_par_defaut->checkField())
    {
        YerothERPConfig::pagination_number_of_table_rows =
                        lineEdit_taille_de_pagination_par_defaut->text().toInt();
    }

    if (lineEdit_MAINTENANCE_VERIFICATION_log_size_max->checkField())
    {
    	YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
    			lineEdit_MAINTENANCE_VERIFICATION_log_size_max->text().toInt();

    	YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
    			YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT;
    }

    if (lineEdit_mariadb_client_base_dir_full_path->checkField())
    {
        YerothERPConfig::pathToMARIA_DB_BASE_DIR =
                        lineEdit_mariadb_client_base_dir_full_path->text();
    }

    if (lineEdit_pdfReader->checkField())
    {
        YerothERPConfig::pathToPdfReader = lineEdit_pdfReader->text();
    }

    if (comboBox_format_de_facture->checkField())
    {
        YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage
        (comboBox_format_de_facture->currentText());
    }

    if (lineEdit_repertoire_fichiers_temporaires->checkField())
    {
        YerothERPConfig::temporaryFilesDir =
                        lineEdit_repertoire_fichiers_temporaires->text();
    }

    if (lineEdit_annee_depart_rapports_chiffre_affaire->checkField())
    {
        YerothERPConfig::annee_depart_rapports_chiffre_affaire_value =
                        lineEdit_annee_depart_rapports_chiffre_affaire->text();
    }

    if (comboBox_impression_sur->checkField())
    {
        YerothERPConfig::printer = comboBox_impression_sur->currentText();

        if (YerothUtils::isEqualCaseInsensitive(YerothERPConfig::printer,
                                                YerothUtils::
                                                IMPRIMANTE_EPSON_TM_T20ii_RESEAU))
        {
            YerothERPConfig::IP_ADDRESS_NETWORK_printer =
                            lineEdit_ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE->text();
        }
    }

    YerothERPConfig::saveYerothConfig();
}


void YerothAdminWindow::enregistrer_app_parameters_configuration()
{
    _logger->log("enregistrer_app_parameters_configuration");

    QString msgEnregistrer(QObject::tr
                           ("Enregistrer la configuration (générale) modifiée ?"));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("enregistrer"),
                                        msgEnregistrer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        enregistrer_system_local_app_parameters_configuration();

        YerothSqlTableModel &configurationsTableModel =
                        _allWindows->getSqlTableModel_configurations();

        QSqlRecord configurationsRecord;

        if (lineEdit_devise->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_CURRENCY);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_devise->text());

            bool success = configurationsTableModel
            					.updateRecord(YerothERPConfig::CONFIG_CURRENCY,
            							      configurationsRecord,
											  "src/admin/yeroth-erp-admin-window.cpp",
											  3313);

            if (success)
            {
                YerothERPConfig::currency = lineEdit_devise->text();
            }
        }

        if (lineEdit_annee_de_depart_pour_la_pagination->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_annee_de_depart_pour_la_pagination->
                                          text());

            bool success = configurationsTableModel
            					.updateRecord(YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR,
                                         	  configurationsRecord,
											  "src/admin/yeroth-erp-admin-window.cpp",
											  3334);

            if (success)
            {
                YerothERPConfig::annee_depart_pour_la_pagination =
                                lineEdit_annee_de_depart_pour_la_pagination->text();
            }
        }

        if (lineEdit_tva_value->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_TVA_VALUE);
            configurationsRecord.setValue("valeur_configuration",
                                          lineEdit_tva_value->text());

            bool success = configurationsTableModel
            				.updateRecord(YerothERPConfig::CONFIG_TVA_VALUE,
                                          configurationsRecord,
										  "src/admin/yeroth-erp-admin-window.cpp",
										  3355);
            if (success)
            {
                YerothERPConfig::tva_value =
                                (lineEdit_tva_value->text().toDouble() / 100.0);
            }
        }

        if (comboBox_strategie_vente_sortie->checkField())
        {
            configurationsRecord =
                            configurationsTableModel.
                            record(YerothERPConfig::CONFIG_SALES_STRATEGY);
            configurationsRecord.setValue("valeur_configuration",
                                          comboBox_strategie_vente_sortie->
                                          currentText());

            bool success = configurationsTableModel
            					.updateRecord(YerothERPConfig::CONFIG_SALES_STRATEGY,
                                         	  configurationsRecord,
											  "src/admin/yeroth-erp-admin-window.cpp",
											  3376);

            if (success)
            {
                YerothERPConfig::salesStrategy =
                                comboBox_strategie_vente_sortie->currentText();
            }
        }

#ifdef YEROTH_SERVER
        YEROTH_RUNTIME_VERIFIER *dbusServer = _allWindows->dbusServer();
        if (dbusServer)
        {
            //dbusServer->emit_signal_refresh_app_parameters();
            qDebug() << "YerothAdminWindow application "
            			"parameters have been changed on the server";
        }
#endif

    }
    else
    {
    }
}


void YerothAdminWindow::REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS()
{
    _logger
        ->log("yeroth-erp-3-0 | YerothAdminWindow::check_Yr_Db_Runtime_Verif_SERVICE_Running");


    comboBox_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR
        ->populateComboBoxRawString(YerothDatabase::YR_DB_RUNTIME_VERIF,
                                    YerothDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME);


    comboBox_QT_DBUS_ERROR_DB_RUNTIME_VERIFICATOR->setCurrentIndex(1);


    bool started =
    		YerothProcessInfo::check_Yr_Db_Runtime_Verif_Daemon_Running(_logger);

    if (started)
    {
        //qDebug() << "++ OK 3: " << SET_COLOUR_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS_BUTTONS ;
    	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(true);
    	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(false);
    }
    else
    {
        //qDebug() << "++ OK pas demarre: ";
    	label_ON_DB_RUNTIME_VERIFICATOR->setVisible(false);
    	label_OFF_DB_RUNTIME_VERIFICATOR->setVisible(true);
    }
}


void YerothAdminWindow::START_YR_DB_RUNTIME_VERIF_GUI()
{
    YerothERPProcess::START_yr_db_runtime_verif_GUI();

	REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS();
}


void YerothAdminWindow::START_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()
{
    YerothERPProcess::START_yr_db_runtime_verif();

	REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS();
}


void YerothAdminWindow::STOP_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS()
{
    _logger->log("STOP_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS");

    YerothERPProcess::STOP_yr_db_runtime_verif();

	REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS();
}


void YerothAdminWindow::set_colour_status_of_alert_daemon_process_buttons()
{
    _logger->log
    ("yeroth-erp-3-0 | YerothAdminWindow::check_alert_daemon_process");

    bool started =
                    YerothProcessInfo::checkYerothERPAlertDaemonRunning(_logger);

    if (started)
    {
        //qDebug() << "++ OK 3: " << checkAlertDeamonProcessOutput ;
        label_ON->setVisible(true);
        label_OFF->setVisible(false);
    }
    else
    {
        //qDebug() << "++ OK pas demarre: ";
        label_ON->setVisible(false);
        label_OFF->setVisible(true);
    }
}


void YerothAdminWindow::start_alert_daemon_process()
{
    _logger->log("start_alert_daemon_process");

    YerothERPProcess::startYerothERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}

void YerothAdminWindow::stop_alert_daemon_process()
{
    _logger->log("stop_alert_daemon_process");

    YerothERPProcess::stopYerothERPAlertProcess();

    set_colour_status_of_alert_daemon_process_buttons();
}


void YerothAdminWindow::set_localisation_adresse_ip_text()
{
    YerothSqlTableModel &localisationSqlTableModel =
                    _allWindows->getSqlTableModel_localisations();

    localisationSqlTableModel.yerothSetFilter_WITH_where_clause
    (GENERATE_SQL_IS_STMT
     (YerothDatabaseTableColumn::NOM_LOCALISATION,
      comboBox_localisation->currentText()));

    int localisationSqlTableModelRowCount =
    		localisationSqlTableModel.easySelect("src/admin/yeroth-erp-admin-window.cpp", 3486);

    if (localisationSqlTableModelRowCount > 0)
    {
        QSqlRecord localisationRecord = localisationSqlTableModel.record(0);

        _localisation_adresse_ip =
                        GET_SQL_RECORD_DATA(localisationRecord, "adresse_ip");

        lineEdit_localisation_adresse_ip->setText(_localisation_adresse_ip);
    }

    //qDebug() << "++localisation_adresse_ip: " << _localisation_adresse_ip;
    localisationSqlTableModel.resetFilter();
}


void YerothAdminWindow::connecter_localisation_db()
{
    if (YerothUtils::slot_connecter_localisation(*this,
                                                 _allWindows,
                                                 comboBox_localisation->
                                                 currentText()))
    {
        label_ONLINE->setVisible(true);
        label_OFFLINE->setVisible(false);
    }
    else
    {
        comboBox_localisation->resetYerothComboBox();

        lineEdit_localisation_adresse_ip->clear();

        label_ONLINE->setVisible(false);
        label_OFFLINE->setVisible(true);
    }

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();
}

void YerothAdminWindow::deconnecter_localisation_db()
{
    YerothUtils::slot_deconnecter_localisation(_allWindows);

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();

    comboBox_localisation->resetYerothComboBox();

    lineEdit_localisation_adresse_ip->clear();

    label_ONLINE->setVisible(false);
    label_OFFLINE->setVisible(true);
}
