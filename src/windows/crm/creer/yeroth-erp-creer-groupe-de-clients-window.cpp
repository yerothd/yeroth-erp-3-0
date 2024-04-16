/*
 * yeroth-erp-creer-groupe-de-clients-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-creer-groupe-de-clients-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerGroupeDeClientsWindow::YerothCreerGroupeDeClientsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerGroupeDeClientsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("créer un groupe de clients"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID;


    setup_select_configure_dbcolumn(YerothDatabase::GROUPES_DE_CLIENTS);

    setupLineEdits();

    setupLineEditsQCompleters();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this,
            SLOT(clients()));
    connect(actionListerLesGroupesDeClients, SIGNAL(triggered()), this,
            SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionSupprimerUnMembreDuGroupeDeClients, SIGNAL(triggered()),
            this, SLOT(supprimerUnMembreDunGroupeDeClients()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnGroupeDeClients()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));


    connect(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe,
            SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));

    connect(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));


#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();

    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->setYerothEditable(true);

    lineEdit_creer_groupe_clients_designation->setFocus();
}

void YerothCreerGroupeDeClientsWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothCreerGroupeDeClientsWindow::setupLineEditsQCompleters()
{
    lineEdit_creer_groupe_clients_recherche_client_initiaux
		->setupMyStaticQCompleter(YerothDatabase::CLIENTS,
								  YerothDatabaseTableColumn::NOM_ENTREPRISE);

    connect((QObject *) lineEdit_creer_groupe_clients_recherche_client_initiaux->getMyQCompleter(),
			SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_un_membre_au_groupe_de_clients(const QString &)));
}


void YerothCreerGroupeDeClientsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothCreerGroupeDeClientsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_supprimer_membre_du_groupe_de_clients->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerGroupeDeClientsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_de_clients
                                    ()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDeClients
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDeClients
                                        ()));
}


void YerothCreerGroupeDeClientsWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDeClients,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDeClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_de_clients
                                    ()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_supprimer_membre_du_groupe_de_clients->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDeClients
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDeClients
                                        ()));
}


void YerothCreerGroupeDeClientsWindow::update_client_group_member_data(const
                                                                       QString &
                                                                       new_client_group_designation,
                                                                       const
                                                                       QString &
                                                                       new_client_group_db_ID,
                                                                       const
                                                                       QString &
                                                                       client_db_ID)
{
    QString
    strQueryClientGroupMemberDATA(QString
                                  ("select %1, %2 from %3 where %4='%5'").arg
                                  (YerothDatabaseTableColumn::
                                   GROUPES_DU_CLIENT,
                                   YerothDatabaseTableColumn::
                                   GROUPES_DU_CLIENT_ID,
                                   YerothDatabase::CLIENTS,
                                   YerothDatabaseTableColumn::ID,
                                   client_db_ID));

    QSqlQuery query;

    int querySize =
                    YerothUtils::execQuery(query, strQueryClientGroupMemberDATA, _logger);

    if (querySize <= 0)
    {
        return;
    }

    QString groupes_du_clients;
    QString groupes_du_clients_ID;

    if (query.next())
    {
        groupes_du_clients = query.value(0).toString();
        groupes_du_clients_ID = query.value(1).toString();
    }

    query.clear();

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    (new_client_group_designation, groupes_du_clients);

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    (new_client_group_db_ID, groupes_du_clients_ID);

    QString
    strQuery_UPDATE_ClientGroupMemberDATA(QString
                                          ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").
                                          arg(YerothDatabase::CLIENTS,
                                              YerothDatabaseTableColumn::
                                              GROUPES_DU_CLIENT,
                                              groupes_du_clients,
                                              YerothDatabaseTableColumn::
                                              GROUPES_DU_CLIENT_ID,
                                              groupes_du_clients_ID,
                                              YerothDatabaseTableColumn::ID,
                                              client_db_ID));

//      QDEBUG_STRING_OUTPUT_2("strQuery_UPDATE_ClientGroupMemberDATA", strQuery_UPDATE_ClientGroupMemberDATA);

    YerothUtils::execQuery(query, strQuery_UPDATE_ClientGroupMemberDATA,
                           _logger);
}


bool YerothCreerGroupeDeClientsWindow::clientGroupAlreadyExists()
{
    bool result = false;

    YerothSqlTableModel &groupes_de_clients_TableModel =
                    _allWindows->getSqlTableModel_groupes_de_clients();

    // ** check if customer account with same name exist
    result =
                    YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                groupes_de_clients_TableModel,
                                                                *lineEdit_creer_groupe_clients_recherche_client_initiaux,
                                                                QObject::tr
                                                                ("Un groupe de client"),
                                                                YerothDatabaseTableColumn::
                                                                NOM_ENTREPRISE);

    return result;
}


bool YerothCreerGroupeDeClientsWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_clients_creer =
                    lineEdit_creer_groupe_clients_designation->checkField();

    bool nombre_maximum_de_membres =
                    lineEdit_creer_groupe_clients_maximum_de_membres->checkField();

    bool programme_de_fidelite_clients =
                    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->checkField();


    result = nom_du_groupe_de_clients_creer &&
             nombre_maximum_de_membres 		&&
			 programme_de_fidelite_clients;

    return result;
}


void YerothCreerGroupeDeClientsWindow::clear_all_fields()
{
    lineEdit_creer_groupe_clients_recherche_client_initiaux->clear();

    lineEdit_creer_groupe_clients_reference_groupe->clear();
    lineEdit_creer_groupe_clients_designation->clearField();
    lineEdit_creer_groupe_clients_maximum_de_membres->clearField();
    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->clearField();

    textEdit_creer_groupe_clients_description_groupe->clear();

    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->
    yerothClearTableWidgetContent();
}


void YerothCreerGroupeDeClientsWindow::rendreInvisible()
{
    clear_all_fields();
    YerothWindowsCommons::rendreInvisible();
}


void YerothCreerGroupeDeClientsWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    setupLineEditsQCompleters();

    check_fields();

    lineEdit_creer_groupe_clients_designation->setFocus();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("créer un groupe de clients"),
      								 _curStocksTableModel);

    setVisible(true);
}


bool YerothCreerGroupeDeClientsWindow::
	IS_client_present_dans_un_groupe_de_clients(const QString &aClientGroupMemberName)
{
    QTableWidgetItem *anItem = 0;

    for (unsigned int k = 0;
         k < tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount();
         ++k)
    {
        anItem = tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->item(k, 1);

        if (0 != anItem)
        {
            if (YerothUtils::isEqualCaseInsensitive(aClientGroupMemberName,
                                                    anItem->text()))
            {
                return true;
            }
        }
    }

    return false;
}


void
YerothCreerGroupeDeClientsWindow::ajouter_un_membre_au_groupe_de_clients
(const QString &aClientGroupMemberName)
{
    if (IS_client_present_dans_un_groupe_de_clients(aClientGroupMemberName))
    {
        return;
    }

    int
    MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt();

    if (tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount()
            == MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES ('%1') INITIAUX ATTEINT !").
                                       arg(QString::
                                           number
                                           (MAXIMUM_DE_MEMBRES_DU_GROUPE)));

        return;
    }

    YerothSqlTableModel &aClientSqlTableModel =
                    _allWindows->getSqlTableModel_clients();

    aClientSqlTableModel.setFilter(QString("%1='%2'").arg
                                   (YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                    aClientGroupMemberName));

    QSqlRecord
    aClientRecord = aClientSqlTableModel.record(0);

    QString
    aClient_db_ID =
                    GET_SQL_RECORD_DATA(aClientRecord, YerothDatabaseTableColumn::ID);

    QString
    aClientReference = GET_SQL_RECORD_DATA(aClientRecord,
                                           YerothDatabaseTableColumn::REFERENCE_CLIENT);


    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->
    addAClientGroupMember(aClient_db_ID, aClientReference,
                          aClientGroupMemberName);

    aClientSqlTableModel.resetFilter();

    lineEdit_creer_groupe_clients_recherche_client_initiaux->myClear();
}


void YerothCreerGroupeDeClientsWindow::annuler_la_creation_dun_groupe_de_clients()
{
    clear_all_fields();

    groupes_de_clients();
}


void YerothCreerGroupeDeClientsWindow::supprimerUnMembreDunGroupeDeClients()
{
    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->removeArticle
    (_last_selected_table_widget_row);
}


bool YerothCreerGroupeDeClientsWindow::creerEnregistrerUnGroupeDeClients()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this,
                                           QObject::tr
                                           ("groupe de FIDÉLITÉ clients"),
                                           QObject::tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    int NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS =
                    tableWidget_creer_groupe_clients_membres_initiaux_du_groupe->rowCount();

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt();

    if (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS >
            MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        uint difference =
                        (uint) qFabs(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS
                                     - MAXIMUM_DE_MEMBRES_DU_GROUPE);

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr
                                       ("NOMBRE MAXIMUM DE MEMBRES: %1\n\n"
                                        "NOMBRE DE MEMBRES INITIAUX DU GROUPE DE FIDÉLITÉ CLIENT: %2\n\n"
                                        "Veuillez corriger le nombre maximum de membres à '%3' ou "
                                        "diminuer son nombre de membres initiaux de ('%4 MEMBRES') !").
                                       arg(QString::
                                           number
                                           (MAXIMUM_DE_MEMBRES_DU_GROUPE),
                                           QString::
                                           number
                                           (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS),
                                           QString::
                                           number
                                           (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DE_FIDELITE_CLIENTS),
                                           QString::number(difference)));

        return false;
    }


    if (clientGroupAlreadyExists())
    {
        return false;
    }


    QString retMsg(QObject::tr("Le groupe de clients '"));

    YerothSqlTableModel &clientGroupTableModel =
                    _allWindows->getSqlTableModel_groupes_de_clients();


    QSqlRecord record = clientGroupTableModel.record();


    QString new_client_group_designation =
                    lineEdit_creer_groupe_clients_designation->text();

    int new_client_group_db_ID =
                    YerothERPWindows::getNextIdSqlTableModel_groupes_de_clients();


    record.setValue(YerothDatabaseTableColumn::ID, new_client_group_db_ID);

    record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerothDatabaseTableColumn::REFERENCE_GROUPE,
                    lineEdit_creer_groupe_clients_reference_groupe->text());

    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    new_client_group_designation);

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_GROUPE,
                    textEdit_creer_groupe_clients_description_groupe->toPlainText());

    record.setValue(YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                    lineEdit_creer_groupe_clients_maximum_de_membres->text().toInt());

    record.setValue(YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
                    comboBox_creer_groupe_clients_programme_de_fidelite_de_clients->currentText());


    QMapIterator<int, QString> it_MapListIdxToElement_db_ID
								(tableWidget_creer_groupe_clients_membres_initiaux_du_groupe
										->get_mapListIdxToElement_db_ID());


    QString a_cur_client_db_ID;

    QString membres_du_groupe_db_ID;

    while (it_MapListIdxToElement_db_ID.hasNext())
    {
        it_MapListIdxToElement_db_ID.next();

        a_cur_client_db_ID = it_MapListIdxToElement_db_ID.value();

        update_client_group_member_data(new_client_group_designation,
                                        QString::number(new_client_group_db_ID),
                                        a_cur_client_db_ID);

        YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(a_cur_client_db_ID,
        															membres_du_groupe_db_ID);
    }

    record.setValue(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                    membres_du_groupe_db_ID);


    retMsg.append(lineEdit_creer_groupe_clients_designation->text());

    bool success = clientGroupTableModel.insertNewRecord(record,
    													 this,
														 "src/windows/crm/creer/yeroth-erp-creer-groupe-de-clients-window.cpp",
														 647,
														 new_client_group_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("créer 1 groupe de clients"),
								   retMsg);
        return false;
    }

    clientGroupTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerothQMessageBox::information(this,
                                   QObject::tr("créer 1 groupe de clients"),
								   retMsg);

    clear_all_fields();

    groupes_de_clients();

    return true;
}
