/*
 * yeroth-erp-admin-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_
#define SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_

#include "../../ui_yeroth-erp-admin-window.h"


#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "admin-actions-subjects.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerothERPDatabaseTableColumnInfo;
class YerothSqlTableModel;
class YerothLogger;
class YerothDatabase;


class YerothAdminWindow : public YerothPOSAdminWindowsCommons,
    					  public Ui_YerothAdminWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

    enum YerothAdminWindowTabPositionsTypes
    {
        CONNECTER_LOCALISATION = 0,
        DONNEES_ENTREPRISE = 1,
        IMPORTER_CSV_TABLEAU = 2,
        MAINTENANCE = 3,
        OPERATIONS = 4,
        PARAMETRES_APPLICATION = 5,
        SYSTEME_DALERTES = 6
    };


    YerothAdminWindow();

    virtual ~YerothAdminWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_adminWindow;
    }

    inline void setImporterParlant(bool aValue)
    {
        _importer_parlant = aValue;
    }

    inline bool getImporterParlant()
    {
        return _importer_parlant;
    }

    void setupValidators();

    void PRINT_SIGNALS_SOURCE_TARGET_info();

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel = 0);

public slots:

	void DISABLE_YR_DB_RUNTIME_VERIF_BUTTONS();

	void ENABLE_YR_DB_RUNTIME_VERIF_BUTTONS();

    void stop_TESTING_MAINTENANCE();

    void start_TESTING_MAINTENANCE();

    void enableAllOperationsTabPushButtons();

    void disableAllOperationsTabPushButtons();

    void setCurrentAdminWindowTitle(enum YerothAdminWindowTabPositionsTypes
                                    tabPosition);

    void handleTabChanged(int);

    void action_creer();

    void action_lister();

    void action_modifier();

    void action_supprimer();

    void action_exporter();

    void action_importer();

    void action_effacer();

    void gerer_choix_action();

    void choose_fichier_csv_a_importer();

    void choose_path_mariadb_client_dir();

    void choose_path_pdfReader();

    void choose_path_thermalPrinterDeviceFile();

    void choose_path_latex_system_root_folder();

    void choose_repertoire_des_sauvegardes();

    void choose_repertoire_fichiers_temporaires();

    void read_configuration();

    //void handle__tabWidget_maintenance_verification_DATABASE(const QString &CURRENT_INDEX);

    void handleThermalPrinterConfiguration(const QString &addedPrinterValue);

    void read_entreprise_info_database_table(bool initalizationValues = true);

    void read_YEROTH_ERP_3_0_SYSTEM_DAEMON_init_configuration();

    void read_app_parameters_init_configuration();

    void reinitialiser_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration();

    void reinitialiser_app_parameters_configuration();

    void enregistrer_entreprise_info_database_table();

    void enregistrer_YEROTH_ERP_3_0_SYSTEM_DAEMON_configuration();

    void enregistrer_system_local_app_parameters_configuration();

    void enregistrer_app_parameters_configuration();

    inline void set_DBUS_CONNECTION_ERROR_TO_CLIENT(bool v)
    {
    	_DBUS_CONNECTION_ERROR_TO_CLIENT = v;
    }

    void REFRESH_STATUS_OF_YR_DB_RUNTIME_VERIF_PROCESS();

    void START_YR_DB_RUNTIME_VERIF_GUI();

    void START_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS();

    void STOP_YR_DB_RUNTIME_VERIF_DAEMON_PROCESS();

    void set_colour_status_of_alert_daemon_process_buttons();

    void start_alert_daemon_process();

    void stop_alert_daemon_process();

    void set_localisation_adresse_ip_text();

    void connecter_localisation_db();

    void deconnecter_localisation_db();

    virtual void deconnecter_utilisateur();

private slots:

	inline bool GET__DBUS_CONNECTION_ERROR_TO_CLIENT()
	{
		return _DBUS_CONNECTION_ERROR_TO_CLIENT;
	}

	void reinitialiser_la_derniere_selection_du_logo_de_lentreprise();

    void supprimer_logo_de_lentreprise();

    inline void selectionner_logo_de_lentreprise()
    {
        YerothUtils::selectionner_image(this, *label_logo_de_lentreprise);
    }

    void YEROTH_ERP_3_populate_all_tables();

    void EXECUTER_COMMANDE_MAINTENANCE();

    void reinitialiser_AFFICHAGE_COMMANDE_MAINTENANCE();

    void handleCheckboxActiver_SHOW_DATABASE_RAW(int state);

    void handleCheckboxActiverRegistreCaisse(int state);

    void handle_changer_commande_MAINTENANCE_OPERATION_SUJET_for_database(const
                                                                          QString
                                                                          &
                                                                          commande_MAINTENANCE,
                                                                          const
                                                                          QString
                                                                          &
                                                                          sqlTableName);

    void handle_changer_commande_MAINTENANCE_OPERATION(const QString &
                                                       commande_MAINTENANCE);

    void handle_changer_commande_MAINTENANCE_SUJET(const QString &
                                                   sqlTableName);

    inline void handle_changer_tableau_dimportation(const QString &
                                                    sqlTableName)
    {
        generate_table_header_mapping_entries_for_csv_import();
    }

    void handle_importer_parlant_checkBox(int aState);

    void choix_registre_de_caisse(const QString &);

    void reset_import_current_selected_csv_file();

    void import_current_selected_csv_file();

    bool generate_table_header_mapping_entries_for_csv_import();

private:

    void populateComboBoxes();

    void cacher_autres_commandes_YEROTH_LINE_EDIT(YerothLineEdit *
                                                  aYerothCommandLineEdit);

    void changer_commande_YEROTH_LINE_EDIT(YerothLineEdit &
                                           aYerothCommandLineEdit,
                                           const QString &sqlTableName);

    void clear_csv_import_panel_content_mapping();

    void initialize_admin_importer_csv_tableau();

    void creer(enum AdminSujetAction selectedSujetAction);

    void lister(enum AdminSujetAction selectedSujetAction);

    void modifier(enum AdminSujetAction selectedSujetAction);

    inline void maintenance_exporter(enum AdminSujetAction selectedSujetAction)
    {
    }

    inline void maintenance_importer(enum AdminSujetAction selectedSujetAction)
    {
    }

    inline void maintenance_effacer(enum AdminSujetAction selectedSujetAction)
    {
    }

    static const int MAX_IMPORT_CSV_HEADER_SIZE;

    static const QString DATABASE_YEROTH_ERP_3_0_KEYWORD;

    static const QString CREER;
    static const QString LISTER;
    static const QString MODIFIER;
    static const QString SUPPRIMER;
    static const QString EXPORTER;
    static const QString IMPORTER;
    static const QString EFFACER;

    static const QString COMPTE_UTILISATEUR;
    static const QString LOCALISATION;
    static const QString DEPARTEMENTS_DE_PRODUITS;
    static const QString CATEGORIE;
    static const QString LIGNE_BUDGETAIRE;
    static const QString FOURNISSEUR;
    static const QString ALERTE;
    static const QString COMPTE_BANCAIRE;
    static const QString REMISE;
    static const QString BON_DE_COMMANDE;
    static const QString DB;
    static const QString TABLEAU;


    static QStringList comboBox_impression_sur_CHOICE_list;

    static QStringList comboBox_format_de_facture_CHOICE_list;

    QString _currentWindowTitle;
    QString _yerothAdminWindowTitleStart;


    bool _DBUS_CONNECTION_ERROR_TO_CLIENT;

    bool _importer_parlant;


public:

    enum AdminAction _curAdminAction;

    enum AdminSujetAction _curAdminSujetAction;

    enum AdminAction _curAdminMaintainAction;

    enum AdminSujetAction _curAdminSujetsMaintainAction;


private:

    YerothLogger *_logger;

    QStringList _curCsvFileToImportContentWordList;

    QList <YerothERPDatabaseTableColumnInfo *> _allDatabaseTableColumnInfo;

    QMap<QString, bool> _dbTableColumnToIsNotNULL;

    QMap<QString, QString> _dbTableColumnToType;

    QMap<int, YerothERPDatabaseTableColumnInfo *> _csvContentIdxToDatabaseTableColumnInfo;

    QMap<int, YerothComboBox *> _csvContentIdxToSQLTableImportHeader;

    QMap<int, QLabel * > _csvContentIdxToCsvFileContentImportHeader;

    QMap<QString, AdminAction> *_actionsToConst;

    QMap<QString, AdminSujetAction> *_sujetActionsToConst;

    QString _current_selected_import_table;

    QString _localisation_adresse_ip;
};


#endif /* SRC_ADMIN_YEROTH_ADMIN_WINDOW_HPP_ */
