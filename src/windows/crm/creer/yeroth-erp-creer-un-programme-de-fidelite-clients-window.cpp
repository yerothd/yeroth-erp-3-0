/*
 * yeroth-erp-creer-un-programme-de-fidelite-clients-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerUnProgrammeDeFideliteClientsWindow::YerothCreerUnProgrammeDeFideliteClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerUnProgrammeDeFideliteClientsWindow"))
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("créer un programme de fidélité clients");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);


//    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
//              << ;


    setup_select_configure_dbcolumn(YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS);

    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->setMaximum(100.0);

    populateComboBoxes();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);


    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_clients->disable(this);
    pushButton_enregistrer->disable(this);


    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this,
            SLOT(annuler_la_creation_dun_programme_de_fidelite_de_clients
                 ()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnProgrammeDeFideliteClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));



#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::updatePopulateComboBoxes()
{
    QStringList new_items;

    new_items.append(YerothUtils::EMPTY_STRING);

    new_items.append(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);


    comboBox_creer_un_programme_de_fidelite_clients_localisation->populateComboBoxRawString
    (YerothDatabase::LOCALISATIONS,
     YerothDatabaseTableColumn::NOM_LOCALISATION);


    comboBox_creer_un_programme_de_fidelite_clients_localisation->addItem_AFTER_POPULATE
    (new_items);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);

    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_clients->disable(this);
    pushButton_enregistrer->disable(this);

}


void YerothCreerUnProgrammeDeFideliteClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, true);


    pushButton_annuler->enable(this,
                               SLOT
                               (annuler_la_creation_dun_programme_de_fidelite_de_clients
                                ()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_enregistrer->enable(this,
                                   SLOT
                                   (creerEnregistrerUnProgrammeDeFideliteClients
                                    ()));
}


bool
YerothCreerUnProgrammeDeFideliteClientsWindow::
royaltyClientProgramAlreadyExists()
{
    bool
    result = false;

    YerothSqlTableModel &programmes_de_fidelite_clients_TableModel =
                    _allWindows->getSqlTableModel_programmes_de_fidelite_clients();

    // ** check if customer account with same name exist
    result =
                    YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                programmes_de_fidelite_clients_TableModel,
                                                                *lineEdit_creer_un_programme_de_fidelite_clients_designation,
                                                                QObject::tr
                                                                ("programme de fidélité clients"),
                                                                YerothDatabaseTableColumn::
                                                                DESIGNATION);

    return result;
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::check_fields()
{
    bool result = false;

    bool programme_de_fidelite_clients_designation =
                    lineEdit_creer_un_programme_de_fidelite_clients_designation->checkField
                    ();

    result = programme_de_fidelite_clients_designation;

    return result;
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::clear_all_fields()
{
    lineEdit_creer_un_programme_de_fidelite_clients_reference->clear();

    lineEdit_creer_un_programme_de_fidelite_clients_designation->clearField();

    textEdit_creer_un_programme_de_fidelite_clients_description->clear();

    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->
    setValue(0.0);

    comboBox_creer_un_programme_de_fidelite_clients_localisation->
    resetYerothComboBox();
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    updatePopulateComboBoxes();

    check_fields();

    lineEdit_creer_un_programme_de_fidelite_clients_designation->setFocus();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("créer un programme de fidélité clients"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerothCreerUnProgrammeDeFideliteClientsWindow::annuler_la_creation_dun_programme_de_fidelite_de_clients()
{
    clear_all_fields();

    programmes_de_fidelite_clients();
}


bool YerothCreerUnProgrammeDeFideliteClientsWindow::creerEnregistrerUnProgrammeDeFideliteClients()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this, "",
                                           QObject::tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }

    if (royaltyClientProgramAlreadyExists())
    {
        return false;
    }

    QString
    retMsg(QObject::tr("Le programme de fidélité de clients '"));

    YerothSqlTableModel &clientRoyaltyProgramTableModel =
                    _allWindows->getSqlTableModel_programmes_de_fidelite_clients();


    QSqlRecord
    record = clientRoyaltyProgramTableModel.record();


    QString
    new_royalty_program_designation =
                    lineEdit_creer_un_programme_de_fidelite_clients_designation->text();

    int
    new_client_LOYALTY_PROGRAM_db_ID =
                    YerothERPWindows::getNextIdSqlTableModel_programme_de_fidelite_clients();


    record.setValue(YerothDatabaseTableColumn::ID,
                    new_client_LOYALTY_PROGRAM_db_ID);

    record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS,
    				lineEdit_creer_un_programme_de_fidelite_clients_reference->text());

    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    new_royalty_program_designation);

    if (!comboBox_creer_un_programme_de_fidelite_clients_localisation->isEmpty
            ())
    {
        record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                        comboBox_creer_un_programme_de_fidelite_clients_localisation->currentText());
    }

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
    				textEdit_creer_un_programme_de_fidelite_clients_description->toPlainText());

    record.setValue(YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS,
                    doubleSpinBox_creer_un_programme_de_fidelite_clients_pourcentage_du_rabais->value());


    retMsg.append(lineEdit_creer_un_programme_de_fidelite_clients_designation->text());

    bool success = clientRoyaltyProgramTableModel
    					.insertNewRecord(record,
    									 this,
										 "src/windows/crm/creer/yeroth-erp-creer-un-programme-de-fidelite-clients-window.cpp",
										 358,
										 new_royalty_program_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("créer 1 programme de fidélité clients"),
                                   retMsg);
        return false;
    }

    clientRoyaltyProgramTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerothQMessageBox::information(this,
                                   QObject::tr
                                   ("créer 1 programme de fidélité clients"),
                                   retMsg);

    clear_all_fields();

    programmes_de_fidelite_clients();

    return true;
}
