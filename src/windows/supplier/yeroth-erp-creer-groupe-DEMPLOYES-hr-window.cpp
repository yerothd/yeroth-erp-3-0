/*
 * yeroth-erp-creer-groupe-DEMPLOYES-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-creer-groupe-DEMPLOYES-hr-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


YerothCreerGroupeDemployesWindow::YerothCreerGroupeDemployesWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerGroupeDemployesWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("créer un groupe D'EMPLOYÉS"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


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
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUnMembreDuGroupeDEMPLOYES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->disable(this);
    pushButton_enregistrer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionListerLesGroupesDemployes, SIGNAL(triggered()), this,
            SLOT(GROUPES_demployes_hr()));
    connect(actionEMPLOYES, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionSupprimerUnMembreDuGroupeDEMPLOYES, SIGNAL(triggered()),
            this, SLOT(supprimerUnMembreDunGroupeDemployes_HR()));
    connect(actionEnregistrer, SIGNAL(triggered()), this,
            SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));


    connect(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe,
            SIGNAL(itemClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));

    connect(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe,
            SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
            this, SLOT(handle_select_table_widget_row(QTableWidgetItem *)));


#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();

    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->setYerothEditable(true);

    lineEdit_creer_groupe_DEMPLOYES_designation->setFocus();
}

void YerothCreerGroupeDemployesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothCreerGroupeDemployesWindow::setupLineEditsQCompleters()
{
    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux
		->setupMyStaticQCompleter(YerothDatabase::FOURNISSEURS,
								  YerothDatabaseTableColumn::NOM_ENTREPRISE,
								  QString("%1 = '1'")
								  	  .arg(YerothDatabaseTableColumn::EMPLOYE));

    connect((QObject *) lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->getMyQCompleter(),
			SIGNAL(activated(const QString &)),
			this,
            SLOT(ajouter_un_membre_au_groupe_DEMPLOYES(const QString &)));
}


void YerothCreerGroupeDemployesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothCreerGroupeDemployesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);


    pushButton_annuler->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->disable(this);
    pushButton_enregistrer->disable(this);
}


void YerothCreerGroupeDemployesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_demployes
                                    ()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDemployes_HR
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr
                                        ()));
}


void YerothCreerGroupeDemployesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerLesGroupesDemployes,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEMPLOYES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimerUnMembreDuGroupeDEMPLOYES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#elif YEROTH_SERVER

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_annuler->enable(this,
                               SLOT(annuler_la_creation_dun_groupe_demployes
                                    ()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_supprimer_membre_du_groupe_de_DEMPLOYES->enable(this,
                                                             SLOT
                                                             (supprimerUnMembreDunGroupeDemployes_HR
                                                              ()));
    pushButton_enregistrer->enable(this,
                                   SLOT(creerEnregistrerUnGroupeDEMPLOYES_hr
                                        ()));
}


void YerothCreerGroupeDemployesWindow::
		update_EMPLOYEE_GROUP_MEMBER_DATA(const QString &new_EMPLOYEE_GROUP_designation,
                                          const QString &new_EMPLOYEE_GROUP_db_ID,
                                          const QString &employee_db_ID)
{
//	QDEBUG_STRING_OUTPUT_2("new_EMPLOYEE_GROUP_db_ID", new_EMPLOYEE_GROUP_db_ID);

//	QDEBUG_STRING_OUTPUT_2("employee_db_ID", employee_db_ID);

    QString strQueryEmployeeGroupMemberDATA =
    		QString("select %1, %2 from %3 where %4='%5' and %6 = '1'")
				.arg(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                     YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                     YerothDatabase::FOURNISSEURS,
                     YerothDatabaseTableColumn::ID,
                     employee_db_ID,
					 YerothDatabaseTableColumn::EMPLOYE);


//    QDEBUG_STRING_OUTPUT_2("strQueryEmployeeGroupMemberDATA",
//    					   strQueryEmployeeGroupMemberDATA);


    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query,
    									   strQueryEmployeeGroupMemberDATA,
										   _logger);

//    QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

    if (querySize <= 0)
    {
        return;
    }

    QString GROUPES_DUN_EMPLOYE;

    QString GROUPES_DUN_EMPLOYE_ID;

    if (query.next())
    {
        GROUPES_DUN_EMPLOYE = query.value(0).toString();
        GROUPES_DUN_EMPLOYE_ID = query.value(1).toString();
    }

    query.clear();

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
		(new_EMPLOYEE_GROUP_designation, GROUPES_DUN_EMPLOYE);

    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
    	(new_EMPLOYEE_GROUP_db_ID, GROUPES_DUN_EMPLOYE_ID);

    QString strQuery_UPDATE_EmployeeGroupMemberDATA =
    			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6 = '%7'")
					.arg(YerothDatabase::FOURNISSEURS,
                         YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                         GROUPES_DUN_EMPLOYE,
                         YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         GROUPES_DUN_EMPLOYE_ID,
                         YerothDatabaseTableColumn::ID,
                         employee_db_ID);

//    QDEBUG_STRING_OUTPUT_2("strQuery_UPDATE_EmployeeGroupMemberDATA",
//    					   strQuery_UPDATE_EmployeeGroupMemberDATA);

    YerothUtils::execQuery(query,
    					   strQuery_UPDATE_EmployeeGroupMemberDATA,
                           _logger);
}


bool YerothCreerGroupeDemployesWindow::employeeGroupAlreadyExists()
{
    bool result = false;

    YerothSqlTableModel &groupes_DEMPLOYES_TableModel =
                    _allWindows->getSqlTableModel_groupes_demployes_hr();

    // ** check if customer account with same name exist
    result = YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                         groupes_DEMPLOYES_TableModel,
                                                         *lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux,
                                                         QObject::tr("1 GROUPE D'EMPLOYÉS"),
                                                         YerothDatabaseTableColumn::NOM_ENTREPRISE);

    return result;
}


bool YerothCreerGroupeDemployesWindow::check_fields()
{
    bool result;

    bool nom_du_groupe_de_EMPLOYES_CREER =
                    lineEdit_creer_groupe_DEMPLOYES_designation->checkField();

    bool nombre_maximum_de_membres =
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->checkField();

    bool GROUPE_DE_PAIE_HR =
    		comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->checkField();


    result = nom_du_groupe_de_EMPLOYES_CREER 			&&
    		 nombre_maximum_de_membres					&&
             nombre_maximum_de_membres && GROUPE_DE_PAIE_HR;

    return result;
}


void YerothCreerGroupeDemployesWindow::clear_all_fields()
{
    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->clear();

    lineEdit_creer_groupe_DEMPLOYES_reference_groupe->clear();
    lineEdit_creer_groupe_DEMPLOYES_designation->clearField();
    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->clearField();
    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->clearField();

    textEdit_creer_groupe_DEMPLOYES_description_groupe->clear();

    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
		->yerothClearTableWidgetContent();
}


void YerothCreerGroupeDemployesWindow::rendreInvisible()
{
    clear_all_fields();
    YerothWindowsCommons::rendreInvisible();
}


void YerothCreerGroupeDemployesWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    populateComboBoxes();

    setupLineEditsQCompleters();

    check_fields();

    lineEdit_creer_groupe_DEMPLOYES_designation->setFocus();

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("créer un groupe D'EMPLOYÉS"),
    								 _curStocksTableModel);
}


bool YerothCreerGroupeDemployesWindow::
		IS_client_present_dans_un_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName)
{
    QTableWidgetItem *anItem = 0;

    for (unsigned int k = 0;
         k < tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount();
         ++k)
    {
        anItem = tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->item(k, 1);

        if (0 != anItem)
        {
            if (YerothUtils::isEqualCaseInsensitive(anEmployeeGroupMemberName,
                                                    anItem->text()))
            {
                return true;
            }
        }
    }

    return false;
}


void YerothCreerGroupeDemployesWindow::
		ajouter_un_membre_au_groupe_DEMPLOYES(const QString &anEmployeeGroupMemberName)
{
    if (IS_client_present_dans_un_groupe_DEMPLOYES(anEmployeeGroupMemberName))
    {
        return;
    }

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
    		lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt();

    if (tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount()
            == MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("NOMBRE MAXIMUM DE MEMBRES"),
                                       QObject::tr("NOMBRE MAXIMUM DE MEMBRES ('%1') INITIAUX ATTEINT !")
                                       	   .arg(QString::number(MAXIMUM_DE_MEMBRES_DU_GROUPE)));

        return;
    }

    YerothSqlTableModel &anEMPLOYEESqlTableModel = _allWindows->getSqlTableModel_fournisseurs();

    anEMPLOYEESqlTableModel.setFilter(QString("%1='%2'")
    									.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
    										 anEmployeeGroupMemberName));


    QSqlRecord anEMPLOYEERecord = anEMPLOYEESqlTableModel.record(0);

    QString anEMPLOYEE_db_ID = GET_SQL_RECORD_DATA(anEMPLOYEERecord, YerothDatabaseTableColumn::ID);

    QString anEMPLOYEEReference = GET_SQL_RECORD_DATA(anEMPLOYEERecord,
                                           	   	   YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR);


    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
		->addAClientGroupMember(anEMPLOYEE_db_ID,
								anEMPLOYEEReference,
                          	  	anEmployeeGroupMemberName);

    anEMPLOYEESqlTableModel.resetFilter();

    lineEdit_creer_groupe_DEMPLOYES_recherche_demploye_initiaux->myClear();
}


void YerothCreerGroupeDemployesWindow::annuler_la_creation_dun_groupe_demployes()
{
    clear_all_fields();

    GROUPES_demployes_hr();
}


bool YerothCreerGroupeDemployesWindow::creerEnregistrerUnGroupeDEMPLOYES_hr()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this,
                                           QObject::tr("groupe d'EMPLOYÉS (hr)"),
                                           QObject::tr("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    int NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES =
                    tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe->rowCount();

    int MAXIMUM_DE_MEMBRES_DU_GROUPE =
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt();

    if (NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES > MAXIMUM_DE_MEMBRES_DU_GROUPE)
    {
        uint difference = (uint) qFabs(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES
                                       - MAXIMUM_DE_MEMBRES_DU_GROUPE);

        YerothQMessageBox::information(this,
                                       QObject::tr("nombre maximum de membres"),
                                       QObject::tr("nombre maximum de membres: %1\n\n"
                                    		   	   "nombre de membres initiaux du groupe d'employés: %2\n\n"
                                    		   	   "VEUILLEZ CORRIGER LE NOMBRE MAXIMUM DE MEMBRES À '%3' ou "
                                    		   	   "DIMINUER SON NOMBRE DE MEMBRES INITIAUX DE ('%4 MEMBRES') !")
											.arg(QString::number(MAXIMUM_DE_MEMBRES_DU_GROUPE),
												 QString::number(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES),
												 QString::number(NOMBRE_DE_MEMBRES_INITIAUX_DU_GROUPE_DEMPLOYES),
												 QString::number(difference)));

        return false;
    }


    if (employeeGroupAlreadyExists())
    {
        return false;
    }


    QString retMsg(QObject::tr("Le groupe D'EMPLOYÉS '"));

    YerothSqlTableModel &EMPLOYEE_GroupTableModel =
                    _allWindows->getSqlTableModel_groupes_demployes_hr();


    QSqlRecord record = EMPLOYEE_GroupTableModel.record();


    QString new_employee_group_designation =
                    lineEdit_creer_groupe_DEMPLOYES_designation->text();

    int new_EMPLOYEE_group_db_ID =
                    YerothERPWindows::getNextIdSqlTableModel_groupes_demployes_hr();


    record.setValue(YerothDatabaseTableColumn::ID, new_EMPLOYEE_group_db_ID);

    record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
                    lineEdit_creer_groupe_DEMPLOYES_reference_groupe->text());

    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    new_employee_group_designation);

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR,
                    textEdit_creer_groupe_DEMPLOYES_description_groupe->toPlainText());

    record.setValue(YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                    lineEdit_creer_groupe_DEMPLOYES_maximum_de_membres->text().toInt());

    record.setValue(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                    comboBox_creer_groupe_DEMPLOYES_groupe_de_paie_HR->currentText());


    QMapIterator<int, QString> it_MapListIdxToElement_db_ID
								(tableWidget_creer_groupe_demployes_HR_membres_initiaux_du_groupe
										->get_mapListIdxToElement_db_ID());


    QString a_cur_EMPLOYEE_db_ID;

    QString membres_du_groupe_db_ID;

    while (it_MapListIdxToElement_db_ID.hasNext())
    {
        it_MapListIdxToElement_db_ID.next();

        a_cur_EMPLOYEE_db_ID = it_MapListIdxToElement_db_ID.value();

        update_EMPLOYEE_GROUP_MEMBER_DATA(new_employee_group_designation,
                                          QString::number(new_EMPLOYEE_group_db_ID),
										  a_cur_EMPLOYEE_db_ID);

        YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
			(a_cur_EMPLOYEE_db_ID,
			 membres_du_groupe_db_ID);
    }

    record.setValue(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                    membres_du_groupe_db_ID);


    retMsg.append(lineEdit_creer_groupe_DEMPLOYES_designation->text());

    bool success = EMPLOYEE_GroupTableModel
    					.insertNewRecord(record,
    									 this,
										 "src/windows/supplier/yeroth-erp-creer-groupe-DEMPLOYES-hr-window.cpp",
										 616,
										 new_employee_group_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("créer 1 groupe D'EMPLOYÉS"), retMsg);
        return false;
    }

    EMPLOYEE_GroupTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerothQMessageBox::information(this,
                                   QObject::tr("créer 1 groupe D'EMPLOYÉS"), retMsg);

    clear_all_fields();

    GROUPES_demployes_hr();

    return true;
}
