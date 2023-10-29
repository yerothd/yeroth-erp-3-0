/*
 * yeroth-erp-groupes-dun-client-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-groupes-dun-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtWidgets/QTableWidgetItem>

#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerothGroupesDunClientWindow::YerothGroupesDunClientWindow()
:YerothWindowsCommons("yeroth-erp-groupes-dun-client"),
 _Last_SelectedRow_Row_INDEX(0),
 _logger(new YerothLogger("YerothGroupesDunClientWindow")),
 _curClientsTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("groupes d'1 client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_groupes_dun_client->font());


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau;


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}")
    						     .arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0);

    _curClientsTableModel = &_allWindows->getSqlTableModel_clients();

    setupLineEdits();

    populateComboBoxes();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne_WITH_A_YEROTH_PROGRESS_BAR()));

    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
			SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));

    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
			SLOT(export_csv_file()));


    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionListerDesGroupesDeClients, SIGNAL(triggered()), this, SLOT(groupes_de_clients()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAfficher_ce_groupe_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));



    connect(tableWidget_groupes_dun_client,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));

    connect(tableWidget_groupes_dun_client,
            SIGNAL(pressed(const QModelIndex &)),
            this,
            SLOT(setLast_SelectedRow_Row_ID(const QModelIndex &)));



    connect(tableWidget_groupes_dun_client,
            SIGNAL(addedYerothTableWidget()),
            this,
            SLOT(enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()));


    connect(tableWidget_groupes_dun_client,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_au_detail(const QModelIndex &)));

    connect(comboBox_groupes_dun_client_recherche,
            SIGNAL(activated(const QString &)),
            this,
			SLOT(ajouter_appartenance(const QString &)));

    setupShortcuts();

    comboBox_groupes_dun_client_recherche->setYerothEditable(true);

    comboBox_groupes_dun_client_recherche->setFocus();
}


void YerothGroupesDunClientWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_menuGroupesDunClientWindow->palette());
    menu.addAction(action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne);
    menu.addAction(actionAfficher_ce_groupe_au_detail);
    menu.exec(event->globalPos());
}


void YerothGroupesDunClientWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothGroupesDunClientWindow::executer_ajouter_appartenance(const QString *
                                                                 un_groupe_de_clients)
{
    if (0 == un_groupe_de_clients)
    {
        return;
    }

    QString
    SELECT_GROUP_FROM_DB_TABLE(QString
                               ("select %1, %2, %3, %4 from %5 where %6='%7'").
                               arg(YerothDatabaseTableColumn::ID,
                                   YerothDatabaseTableColumn::
                                   REFERENCE_GROUPE,
                                   YerothDatabaseTableColumn::
                                   MEMBRES_DU_GROUPE_db_ID,
                                   YerothDatabaseTableColumn::
                                   MAXIMUM_DE_MEMBRES,
                                   YerothDatabase::GROUPES_DE_CLIENTS,
                                   YerothDatabaseTableColumn::DESIGNATION,
                                   *un_groupe_de_clients));

    QSqlQuery aQSqlQuery;

    int query_size =
                    YerothUtils::execQuery(aQSqlQuery, SELECT_GROUP_FROM_DB_TABLE);

    if (query_size <= 0)
    {
        return;
    }

    QSqlRecord aClientGroupRecordInfo;

    if (!aQSqlQuery.next())
    {
        return;
    }

    aClientGroupRecordInfo = aQSqlQuery.record();

    int maximum_de_membres = GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                                 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES).
                             toInt();

    QString membres_du_groupe_db_ID =
                    GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                        YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);

    if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le groupe de clients '%1' a déjà atteint son nombre maximum de membres (%2) !").
                                       arg(*un_groupe_de_clients,
                                           QString::
                                           number(maximum_de_membres)));

        return;
    }


    if (YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
            (_curClientDBID, membres_du_groupe_db_ID))
    {

        QString
        INSERT_UPDATE_CLIENT_WITHIN_GROUP(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerothDatabase::
                                              GROUPES_DE_CLIENTS,
                                              YerothDatabaseTableColumn::
                                              MEMBRES_DU_GROUPE_db_ID,
                                              membres_du_groupe_db_ID,
                                              YerothDatabaseTableColumn::
                                              DESIGNATION,
                                              *un_groupe_de_clients));

        bool insert_update_success =
                        YerothUtils::execQuery(INSERT_UPDATE_CLIENT_WITHIN_GROUP);

//              QDEBUG_STRING_OUTPUT_2("insert_update_success - 1", BOOL_TO_STRING(insert_update_success));

        if (!insert_update_success)
        {
            YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                           QObject::tr
                                           ("Le client '%1' n'a pas pu être ajouté "
                                            "dans le groupe de clients '%2' !").
                                           arg(_curClient_NOM_ENTREPRISE,
                                               *un_groupe_de_clients));

            return;
        }
    }

    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString groupes_du_client = GET_SQL_RECORD_DATA(record,
                                                    YerothDatabaseTableColumn::GROUPES_DU_CLIENT);

    bool update_DB_GROUPES_DU_CLIENT_TABLE =
                    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (*un_groupe_de_clients,
                     groupes_du_client);

    QString groupes_du_client_id = GET_SQL_RECORD_DATA(record,
                                                       YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID);

    QString un_groupe_de_clients_db_ID =
                    GET_SQL_RECORD_DATA(aClientGroupRecordInfo,
                                        YerothDatabaseTableColumn::ID);


    bool update_groupes_du_client_id =
                    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (un_groupe_de_clients_db_ID,
                     groupes_du_client_id);

    update_DB_GROUPES_DU_CLIENT_TABLE =
                    update_DB_GROUPES_DU_CLIENT_TABLE || update_groupes_du_client_id;


    if (!update_DB_GROUPES_DU_CLIENT_TABLE)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le client '%1' est déjà membre du groupe de clients '%2' !").
                                       arg(_curClient_NOM_ENTREPRISE,
                                           *un_groupe_de_clients));
        return;
    }


    QString
    UPDATE_CLIENT_GROUP_INFORMATION(QString
                                    ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").
                                    arg(YerothDatabase::CLIENTS,
                                        YerothDatabaseTableColumn::
                                        GROUPES_DU_CLIENT,
                                        groupes_du_client,
                                        YerothDatabaseTableColumn::
                                        GROUPES_DU_CLIENT_ID,
                                        groupes_du_client_id,
                                        YerothDatabaseTableColumn::ID,
                                        _curClientDBID));

    bool update_client_group_success =
                    YerothUtils::execQuery(UPDATE_CLIENT_GROUP_INFORMATION);

    if (!update_client_group_success)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("Le client '%1' n'a pas pu être ajouté "
                                        "dans le groupe de clients '%2' !").arg
                                       (_curClient_NOM_ENTREPRISE,
                                        *un_groupe_de_clients));

        YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    tableWidget_groupes_dun_client->insert_group(GET_SQL_RECORD_DATA
                                                 (aClientGroupRecordInfo,
                                                  YerothDatabaseTableColumn::
                                                  ID), *un_groupe_de_clients,
                                                 GET_SQL_RECORD_DATA
                                                 (aClientGroupRecordInfo,
                                                  YerothDatabaseTableColumn::
                                                  REFERENCE_GROUPE),
                                                 QString::
                                                 number(maximum_de_membres));


    lineEdit_groupes_dun_client_nombre_de_groupes->setText(QString::number
                                                           (tableWidget_groupes_dun_client->rowCount
                                                            ()));


    YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                   QObject::tr
                                   ("Le client '%1' a été ajouté dans le groupe de clients '%2' !").
                                   arg(_curClient_NOM_ENTREPRISE,
                                       *un_groupe_de_clients));
}


bool
YerothGroupesDunClientWindow::
EXECUTER_retirer_ce_client_du_groupe_selectionne()
{
    QString
    SELECT_GROUP_DU_CLIENT_INFO(QString
                                ("select %1, %2 from %3 where %4='%5'").arg
                                (YerothDatabaseTableColumn::
                                 GROUPES_DU_CLIENT,
                                 YerothDatabaseTableColumn::
                                 GROUPES_DU_CLIENT_ID,
                                 YerothDatabase::CLIENTS,
                                 YerothDatabaseTableColumn::ID,
                                 _curClientDBID));

    QSqlQuery
    aQSqlQuery;

    int
    query_size =
                    YerothUtils::execQuery(aQSqlQuery, SELECT_GROUP_DU_CLIENT_INFO);

    if (query_size <= 0)
    {
        return false;
    }

    QSqlRecord
    aClientRecordGroupInfo;

    if (!aQSqlQuery.next())
    {
        return false;
    }

    aClientRecordGroupInfo = aQSqlQuery.record();

    QString
    groupes_du_client_ID = GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                               YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID);

    QString
    groupes_du_client = GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                            YerothDatabaseTableColumn::GROUPES_DU_CLIENT);

    int
    currentRow = tableWidget_groupes_dun_client->currentRow();

//      QDEBUG_STRING_OUTPUT_1(QString("groupes_du_client_ID: %1, currentRow: %2")
//                                                              .arg(groupes_du_client_ID,
//                                                                       QString::number(currentRow)));

    QString
    clientGroup_db_ID =
                    tableWidget_groupes_dun_client->get_DB_ELEMENT_db_ID(currentRow);

//      QDEBUG_STRING_OUTPUT_1(QString("clientGroup_db_ID: %1, groupes_du_client_ID: %2")
//                                                              .arg(clientGroup_db_ID,
//                                                                       groupes_du_client_ID));

    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
    (groupes_du_client_ID, clientGroup_db_ID);

//      QDEBUG_STRING_OUTPUT_1(QString("clientGroup_db_ID: %1, groupes_du_client_ID: %2")
//                                                              .arg(clientGroup_db_ID,
//                                                                       groupes_du_client_ID));

    QTableWidgetItem *item = tableWidget_groupes_dun_client->item(currentRow, 0);

    if (0 == item)
    {
        return false;
    }

    QString clientGroup_designation = item->text().trimmed();

    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_du_client,
    															   clientGroup_designation);

    bool result = true;

    {
        QString
        UPDATE_GROUPES_DU_CLIENT_INFO(QString
                                      ("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").
                                      arg(YerothDatabase::CLIENTS,
                                          YerothDatabaseTableColumn::
                                          GROUPES_DU_CLIENT,
                                          groupes_du_client,
                                          YerothDatabaseTableColumn::
                                          GROUPES_DU_CLIENT_ID,
                                          groupes_du_client_ID,
                                          YerothDatabaseTableColumn::ID,
                                          _curClientDBID));

        result = result && YerothUtils::execQuery(UPDATE_GROUPES_DU_CLIENT_INFO);
    }

    {
        aQSqlQuery.clear();

        QSqlRecord a_client_group_record_info;

        SELECT_GROUP_DU_CLIENT_INFO.clear();

        SELECT_GROUP_DU_CLIENT_INFO =
                        QString("select %1 from %2 where %3='%4'").arg
                        (YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerothDatabase::GROUPES_DE_CLIENTS,
                         YerothDatabaseTableColumn::DESIGNATION, clientGroup_designation);

//              QDEBUG_STRING_OUTPUT_2("SELECT_GROUP_DU_CLIENT_INFO",
//                                                         SELECT_GROUP_DU_CLIENT_INFO);

        result = result
                 && YerothUtils::execQuery(aQSqlQuery, SELECT_GROUP_DU_CLIENT_INFO);

        if (!aQSqlQuery.next())
        {
            return false;
        }

        a_client_group_record_info = aQSqlQuery.record();

        QString
        membres_du_groupe_db_ID =
                        GET_SQL_RECORD_DATA(a_client_group_record_info,
                                            YerothDatabaseTableColumn::
                                            MEMBRES_DU_GROUPE_db_ID);


//              QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID *",
//                                                         membres_du_groupe_db_ID);


        YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING
        (membres_du_groupe_db_ID, _curClientDBID);


//              QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID ***",
//                                                         membres_du_groupe_db_ID);


        QString
        UPDATE_GROUPES_DE_CLIENT_DB_TABLE(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerothDatabase::
                                              GROUPES_DE_CLIENTS,
                                              YerothDatabaseTableColumn::
                                              MEMBRES_DU_GROUPE_db_ID,
                                              membres_du_groupe_db_ID,
                                              YerothDatabaseTableColumn::
                                              DESIGNATION,
                                              clientGroup_designation));

        result = result
                 && YerothUtils::execQuery(UPDATE_GROUPES_DE_CLIENT_DB_TABLE);
    }


    if (result)
    {
        tableWidget_groupes_dun_client->removeArticle(currentRow);

        int tableWidget_groupes_dun_client_RowCount =
        		tableWidget_groupes_dun_client->rowCount();

        lineEdit_groupes_dun_client_nombre_de_groupes
    		->setText(QString::number(tableWidget_groupes_dun_client_RowCount));

        if (tableWidget_groupes_dun_client_RowCount > 0)
        {
            enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
        }
        else
        {
            disable_yeroth_widgets();
        }
    }

    return result;
}


void YerothGroupesDunClientWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curClientsTableModel->rowCount() > 0)
    {
        _allWindows->
        _detailsGroupeDeClientsWindow->rendreVisible(_curClientsTableModel,
                                                     _curStocksTableModel,
                                                     tableWidget_groupes_dun_client->
                                                     get_DB_ELEMENT_db_ID
                                                     (modelIndex.row()));

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à afficher les détails."));
    }
}


void YerothGroupesDunClientWindow::afficher_tous_les_groupes_du_client()
{
    tableWidget_groupes_dun_client->yerothClearTableWidgetContent();

    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curClient_NOM_ENTREPRISE =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    _curClientDBID =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);


    groupBox_groupes_dun_client->setTitle(QObject::tr
                                          ("Groupes d'appartenance du client (%1)").
                                          arg(_curClient_NOM_ENTREPRISE));


    QString
    groupes_du_client(GET_SQL_RECORD_DATA
                      (record,
                       YerothDatabaseTableColumn::GROUPES_DU_CLIENT));


    QStringList
    allClientGroups(groupes_du_client.split(YerothUtils::STAR_CHAR));

    QString current_group_db_ID;

    QString maximum_de_membres;

    QString current_client_group_reference;

    int curQuerySize = -1;

    QSqlQuery aSqlGroupTableModelQUERY;

    QString clientGroupTableModelQUERY_STR;

    for (int k = 0; k < allClientGroups.size(); ++k)
    {
        clientGroupTableModelQUERY_STR =
                        QString("select %1, %2, %3 from %4 where %5='%6'").arg
                        (YerothDatabaseTableColumn::ID,
                         YerothDatabaseTableColumn::REFERENCE_GROUPE,
                         YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                         YerothDatabase::GROUPES_DE_CLIENTS,
                         YerothDatabaseTableColumn::DESIGNATION,
                         allClientGroups.at(k).trimmed());

        aSqlGroupTableModelQUERY.clear();

        curQuerySize =
                        YerothUtils::execQuery(aSqlGroupTableModelQUERY,
                                               clientGroupTableModelQUERY_STR, _logger);

        if (curQuerySize > 0)
        {
            if (aSqlGroupTableModelQUERY.next())
            {
                current_group_db_ID =
                                QString::number(aSqlGroupTableModelQUERY.value(0).toInt());

                current_client_group_reference =
                                aSqlGroupTableModelQUERY.value(1).toString();

                maximum_de_membres =
                                aSqlGroupTableModelQUERY.value(2).toString();

                tableWidget_groupes_dun_client->insert_group
                (current_group_db_ID, allClientGroups.at(k).trimmed(),
                 current_client_group_reference, maximum_de_membres);
            }

//              qDebug() << tableWidget_groupes_dun_client->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_groupes_dun_client->rowCount() > 0)
    {
        tableWidget_groupes_dun_client->selectRow(0);
    }
}


void YerothGroupesDunClientWindow::enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
        (action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    actionAfficher_ce_groupe_au_detail->setVisible(true);

    pushButton_retirer->setVisible(true);
}


void YerothGroupesDunClientWindow::disable_yeroth_widgets()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
        (action_IMPRIMER_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    actionAfficher_ce_groupe_au_detail->setVisible(false);

    pushButton_retirer->setVisible(false);
}


void YerothGroupesDunClientWindow::get_PRINT_OUT_TexTableString(QString       &texTable_IN_OUT,
                                                                int           row_MAX_TO_GO_export /* = -1 */)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
                           "\\centering\n"
						   "\\resizebox{\\textwidth}{!}{\n"
						   "\\begin{tabular}"
						   "{|c|l|l|r|} \\hline");

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texTable_IN_OUT.append("& & &				        \\\\ \n"
                               "\\textbf{ID}			    & 	 \n"
							   "\\textbf{GROUP NAME}		& 	 \n"
							   "\\textbf{GROUP REFERENCE} 	& 	 \n"
							   "\\textbf{MAX MEMBERS}		\\\\ \n"
							   "& &	&				        \\\\ \\hline \\hline \n");
	}
	else //FRANCAIS
	{
		texTable_IN_OUT.append("& & &						        \\\\ \n"
                               "\\textbf{ID}			            & 	 \n"
							   "\\textbf{DÉSIGNATION DU GROUPE}		& 	 \n"
							   "\\textbf{RÉFÉRENCE DU GROUPE} 		& 	 \n"
							   "\\textbf{NOMBRE DE MEMBRES MAXIMUM}	\\\\ \n"
							   "& &	&						        \\\\ \\hline \\hline \n");
	}


	int rowCount = tableWidget_groupes_dun_client->rowCount();

	int columnCount = tableWidget_groupes_dun_client->columnCount();


    int MAX_TABLE_MODDEL_ROW_COUNT__to_export = rowCount;

	if (row_MAX_TO_GO_export > -1)
	{
        MAX_TABLE_MODDEL_ROW_COUNT__to_export = row_MAX_TO_GO_export;
	}



	bool color_this_row_grey = true;


	int LINE_COUNT_PER_PDF_PAGE = 41;


	QString cell_text;


    int TABLE_COUNT = qFloor(rowCount / LINE_COUNT_PER_PDF_PAGE);

    int current_table_count = 0;

	int current_pdf_page_line_count = 0;

	uint Visual_ID_counter = 0;

	//Tex table body
	for (int i = 0;
         i < MAX_TABLE_MODDEL_ROW_COUNT__to_export && current_pdf_page_line_count <= LINE_COUNT_PER_PDF_PAGE;
         ++i)
	{
        color_this_row_grey = (0 == i%2);

        if (color_this_row_grey)
        {
            texTable_IN_OUT.append(QString("\\rowcolor{yerothColorGray}"));
        }
        else
        {
            texTable_IN_OUT.append(QString("\\rowcolor{white}"));
        }


        //We add a cell for row numbering wioth an ID number.
        {
            ++Visual_ID_counter;

            QTableWidgetItem *an_item_ID = new QTableWidgetItem(QString::number(Visual_ID_counter));

            if (0 != an_item_ID)
            {
                YerothUtils::handleTexTableItemText(columnCount,
                                                    texTable_IN_OUT,
                                                    0,
                                                    an_item_ID->text());
            }
        }


        for (int j = 0; j < columnCount; ++j)
        {
            QTableWidgetItem *an_item = 0;

            an_item = tableWidget_groupes_dun_client->item(i, j);

            if (0 != an_item)
            {
                if (j != 4)
                {
                    cell_text = an_item->text();
                }
                else
                {
                    cell_text = GET_NUM_STRING(an_item->text().toDouble());

                }
            }

            YerothUtils::handleTexTableItemText(columnCount,
                                                texTable_IN_OUT,
                                                j,
                                                cell_text);
        }

        if (i < rowCount - 1)
        {
            texTable_IN_OUT.append("\\hline\n");
        }


        ++current_pdf_page_line_count;


        if (LINE_COUNT_PER_PDF_PAGE - 1 == current_pdf_page_line_count)
        {
            current_pdf_page_line_count = 0;

            if (current_table_count < TABLE_COUNT)
            {
                texTable_IN_OUT.append("\\hline\n"
                                       "\\end{tabular}}\n"
                                       "\\end{table*}\n");

                texTable_IN_OUT.append("\\newpage\n");

                texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
                                       "\\centering\n"
                                       "\\resizebox{\\textwidth}{!}{\n"
                                       "\\begin{tabular}"
                                       "{|c|l|l|r|} \\hline");

                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    texTable_IN_OUT.append("& & &				        \\\\ \n"
                                           "\\textbf{ID}			    & 	 \n"
                                           "\\textbf{GROUP NAME}		& 	 \n"
                                           "\\textbf{GROUP REFERENCE} 	& 	 \n"
                                           "\\textbf{MAX MEMBERS}		\\\\ \n"
                                           "& &	&				        \\\\ \\hline \\hline \n");
                }
                else //FRANCAIS
                {
                    texTable_IN_OUT.append("& & &						        \\\\ \n"
                                           "\\textbf{ID}			            & 	 \n"
                                           "\\textbf{DÉSIGNATION DU GROUPE}		& 	 \n"
                                           "\\textbf{RÉFÉRENCE DU GROUPE} 		& 	 \n"
                                           "\\textbf{NOMBRE DE MEMBRES MAXIMUM}	\\\\ \n"
                                           "& &	&						        \\\\ \\hline \\hline \n");
                }
            }

            ++current_table_count;

            if (current_table_count > 0)
            {
                LINE_COUNT_PER_PDF_PAGE = 57;
            }
        }

    } //for-i


    texTable_IN_OUT.append("\\hline\n"
                           "\\end{tabular}}\n"
                           "\\end{table*}\n");

}


void YerothGroupesDunClientWindow::populateComboBoxes()
{
	comboBox_groupes_dun_client_recherche
    	->populateComboBoxRawString(YerothDatabase::GROUPES_DE_CLIENTS,
                              	    YerothDatabaseTableColumn::DESIGNATION);

	comboBox_groupes_dun_client_recherche->setYerothEnabled(true);
}


void YerothGroupesDunClientWindow::rendreInvisible()
{
    tableWidget_groupes_dun_client->yerothClearTableWidgetContent();

    YerothWindowsCommons::rendreInvisible();
}


void YerothGroupesDunClientWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
                                                 YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    tableWidget_groupes_dun_client->resizeColumnsToContents();

    afficher_tous_les_groupes_du_client();

    int tableRowCount = tableWidget_groupes_dun_client->rowCount();

    if (tableRowCount > 0)
    {
        enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else
    {
        disable_yeroth_widgets();
    }

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("groupes d'1 client"),
    								 _curClientsTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);

    lineEdit_groupes_dun_client_nombre_de_groupes
		->setText(GET_NUM_STRING(tableRowCount));

    tableWidget_groupes_dun_client->resizeColumnsToContents();
}


bool YerothGroupesDunClientWindow::export_csv_file()
{
    bool success = false;

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success =
            YerothUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_groupes_dun_client,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "Client group export listing");
    }
    else
    {
        success =
            YerothUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_groupes_dun_client,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "FICHE D'EXPORTATION DES groupes de fidélité d'1 clients");
    }

    return success;
}


bool YerothGroupesDunClientWindow::
        imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne()
{
	if (tableWidget_groupes_dun_client->rowCount() <= 0)
	{
		YerothQMessageBox::information(this,
				QObject::tr("Impression d'1 LISTE de groupes d'1 client"),
				QObject::tr("PAS DE DONNÉES À Imprimer maintenant !"));

		return false;
	}


    int a_row_FOR_pdf_printing_max = getLast_SelectedRow_Row_ID() + 1;


	QString partial_GROUPES_DUN_client__tex_table;


	QString PARTIAL__GROUPES_DUN_CLIENT__TexDocument;


	get_PRINT_OUT_TexTableString(partial_GROUPES_DUN_client__tex_table,
                                 a_row_FOR_pdf_printing_max);





	YerothUtils::get_GROUPES_DUN_client_TexDocumentString(PARTIAL__GROUPES_DUN_CLIENT__TexDocument,
														  partial_GROUPES_DUN_client__tex_table);


    emit SIGNAL_INCREMENT_PROGRESS_BAR(18);



    QString NOM_Client(_curClient_NOM_ENTREPRISE);



	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
        PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .replace("YEROTHSUBJECT",
                     QString("client '%1' belonging GROUPS (Excerpt)")
                        .arg(NOM_Client));
	}
	else
	{
        PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .replace("YEROTHSUBJECT",
                     QString("Groupes d'appartenance du CLIENT '%1' (Partiel)")
                        .arg(NOM_Client));
	}


	//qDebug() << PARTIAL__GROUPES_DUN_CLIENT__TexDocument;


	emit SIGNAL_INCREMENT_PROGRESS_BAR(40);


    PARTIAL__GROUPES_DUN_CLIENT__TexDocument
            .append(partial_GROUPES_DUN_client__tex_table)
            .append("\n")
            .append("\\end{document}");


	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);


	YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();


	QString factureDate(infoEntreprise.getVille_LATEX());


	YerothUtils::getCurrentSimplifiedDate(factureDate);


    PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHPAPERSPEC",
											get_PRINTING_PARAMETER_printing_position());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHDATE", factureDate);

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerothUtils::getAllWindows()->getUser()->titre(),
					YerothUtils::getAllWindows()->getUser()->nom_completTex()));


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument
        .replace("YEROTHSUCCURSALE",
                 YerothUtils::LATEX_IN_OUT_handleForeignAccents
                    (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));


	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);



	YerothSqlTableModel &CLIENT_TableModel = _allWindows->getSqlTableModel_clients();

	CLIENT_TableModel.yerothSetFilter_WITH_where_clause(
			QString("%1 = '%2'")
			.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
					NOM_Client));

	PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCLIENT",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));


	emit SIGNAL_INCREMENT_PROGRESS_BAR(60);


	if (CLIENT_TableModel.easySelect("src/windows/crm/yeroth-erp-groupes-dun-client-window.cpp", 114) > 0)
	{
		QSqlRecord record = CLIENT_TableModel.record(0);

		QString Client_Ville(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));

		QString Client_POBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

		QString Client_Email(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

		QString Client_Tel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (Client_Tel.isEmpty())
		{
			Client_Tel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ Client_Email: " << Client_Email;

		if (!Client_Ville.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYEROTHCITY",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Ville));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHCITY", "");
		}

		if (!Client_POBox.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPOBOX", Client_POBox);
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
		}

		if (!Client_Email.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHMAIL",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Email));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHMAIL", "");
		}

		if (!Client_Tel.isEmpty())
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYEROTHPHONE",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Tel));
		}
		else
		{
			PARTIAL__GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPHONE", "");
		}

		CLIENT_TableModel.resetFilter();
	}


	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


    QString prefixFileName;

    prefixFileName
        .append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-CLIENT-group-hr-PARTIAL-"));
	//qDebug() << "++\n" << PARTIAL__GROUPES_DUN_CLIENT__TexDocument;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(PARTIAL__GROUPES_DUN_CLIENT__TexDocument.toUtf8());
	}

	tmpLatexFile.close();

	QString pdfReceiptFileName = YerothERPProcess::compileLatex(prefixFileName);

	if (!pdfReceiptFileName.isEmpty())
	{
		YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
	}

	emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

	return true;
}


bool YerothGroupesDunClientWindow::imprimer_pdf_document()
{
//	QDEBUG_STRING_OUTPUT_1("YerothGroupesDunClientWindow::imprimer_pdf_document");

	if (tableWidget_groupes_dun_client->rowCount() <= 0)
	{
		YerothQMessageBox::information(this,
				QObject::tr("impression"),
				QObject::tr("Il n'y a pas de données à imprimer !"));

		return false;
	}

	QString GROUPES_DUN_client__tex_table;

	//int pageNumber = qCeil(tableWidget_groupes_dun_client->rowCount() / 20);


	YerothPOSUser *yerothUser = _allWindows->getUser();

	YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

	QString GROUPES_DUN_CLIENT__TexDocument;

	QString factureDate(infoEntreprise.getVille_LATEX());

	YerothUtils::getCurrentSimplifiedDate(factureDate);

	get_PRINT_OUT_TexTableString(GROUPES_DUN_client__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(18);

	YerothUtils::get_GROUPES_DUN_client_TexDocumentString(GROUPES_DUN_CLIENT__TexDocument,
														  GROUPES_DUN_client__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

	QString NOM_Client(_curClient_NOM_ENTREPRISE);

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		GROUPES_DUN_CLIENT__TexDocument
		.replace("YEROTHSUBJECT",
				QString("client '%1' belonging GROUPS")
				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client)));
	}
	else
	{
		GROUPES_DUN_CLIENT__TexDocument
		.replace("YEROTHSUBJECT",
				QString("Groupes d'appartenance du CLIENT '%1'")
				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client)));
	}

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHPAPERSPEC",
											get_PRINTING_PARAMETER_printing_position());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHDATE", factureDate);

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerothUtils::getAllWindows()->getUser()->titre(),
					YerothUtils::getAllWindows()->getUser()->nom_completTex()));

	GROUPES_DUN_CLIENT__TexDocument
	.replace("YEROTHSUCCURSALE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents
			(YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


	YerothSqlTableModel &CLIENT_TableModel = _allWindows->getSqlTableModel_clients();

	CLIENT_TableModel.yerothSetFilter_WITH_where_clause(
			QString("%1 = '%2'")
			.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
					NOM_Client));

	GROUPES_DUN_CLIENT__TexDocument.replace("YEROTHCLIENT",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(NOM_Client));

	if (CLIENT_TableModel.easySelect("src/windows/crm/yeroth-erp-groupes-dun-client-window.cpp", 894) > 0)
	{
		QSqlRecord record = CLIENT_TableModel.record(0);

		QString Client_Ville(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));

		QString Client_POBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

		QString Client_Email(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

		QString Client_Tel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (Client_Tel.isEmpty())
		{
			Client_Tel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ Client_Email: " << Client_Email;

		if (!Client_Ville.isEmpty())
		{
			GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYEROTHCITY",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Ville));
		}
		else
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHCITY", "");
		}

		if (!Client_POBox.isEmpty())
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPOBOX", Client_POBox);
		}
		else
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
		}

		if (!Client_Email.isEmpty())
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHMAIL",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Email));
		}
		else
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHMAIL", "");
		}

		if (!Client_Tel.isEmpty())
		{
			GROUPES_DUN_CLIENT__TexDocument
			.replace("CLIENTYEROTHPHONE",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(Client_Tel));
		}
		else
		{
			GROUPES_DUN_CLIENT__TexDocument.replace("CLIENTYEROTHPHONE", "");
		}

		CLIENT_TableModel.resetFilter();
	}


	QString prefixFileName;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-CLIENT-group-hr"));
	}
	else
	{
		prefixFileName.append(
				YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-GROUPES-DUN-client-HR"));
	}

	//qDebug() << "++\n" << GROUPES_DUN_CLIENT__TexDocument;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(GROUPES_DUN_CLIENT__TexDocument.toUtf8());
	}

	tmpLatexFile.close();

	QString pdfReceiptFileName = YerothERPProcess::compileLatex(prefixFileName);

	if (!pdfReceiptFileName.isEmpty())
	{
		YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
	}

	emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

	return true;
}


void YerothGroupesDunClientWindow::incrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_groupes_dun_client &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		++_TABLE_FONT_SIZE;

		_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

		//QDEBUG_STRING_OUTPUT_2_N("incrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
		//						  _TABLE_FONT_SIZE);

		tableWidget_groupes_dun_client->hide();

		tableWidget_groupes_dun_client->resizeColumnsToContents();

		tableWidget_groupes_dun_client->show();

        YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
	}
}


void YerothGroupesDunClientWindow::decrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_groupes_dun_client &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		if (_TABLE_FONT_SIZE > 5)
		{
			--_TABLE_FONT_SIZE;

			_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

			//QDEBUG_STRING_OUTPUT_2_N("decrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
			//						  _TABLE_FONT_SIZE);

			tableWidget_groupes_dun_client->hide();

			tableWidget_groupes_dun_client->resizeColumnsToContents();

			tableWidget_groupes_dun_client->show();

            YerothERPWindows *allWindows = YerothUtils::getAllWindows();

            YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
		}
	}
}


void YerothGroupesDunClientWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_groupes_dun_client_recherche->setVisible(true);

    pushButton_groupes_de_clients->enable(this, SLOT(groupes_de_clients()));
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(clients()));
    pushButton_retirer->enable(this,
                               SLOT(retirer_ce_client_du_groupe_selectionne
                                    ()));
}


void YerothGroupesDunClientWindow::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_groupes_dun_client_recherche->setVisible(false);

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->enable(this, SLOT(clients()));
    pushButton_RETOUR->enable(this, SLOT(clients()));
    pushButton_retirer->disable(this);
}


void YerothGroupesDunClientWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDeClients,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_au_detail,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    comboBox_groupes_dun_client_recherche->setVisible(false);

    pushButton_groupes_de_clients->disable(this);
    pushButton_menu_clients->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);
}
