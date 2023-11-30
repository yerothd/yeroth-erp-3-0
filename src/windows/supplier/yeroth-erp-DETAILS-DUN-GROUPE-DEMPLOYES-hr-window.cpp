/*
 * yeroth-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-DETAILS-DUN-GROUPE-DEMPLOYES-hr-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothDetailsDunGroupeDemployesWindow::YerothDetailsDunGroupeDemployesWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothDetailsDunGroupeDemployesWindow"))
{
    _windowName = QString("%1 - %2")
    				 .arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                          QObject::tr("DÉTAILS D'UN GROUPE D'EMPLOYÉS [hr]"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();

    textEdit_details_dun_GROUPE_DEMPLOYES_description_groupe->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionGroupesDePaie, SIGNAL(triggered()), this, SLOT(GROUPES_DE_PAIE_hr()));
    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(GROUPES_demployes_hr()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));

#endif

    setupShortcuts();
}


void YerothDetailsDunGroupeDemployesWindow::afficher_groupes_dun_EMPLOYE()
{
    if (0 == _curEmployeeTableModel)
    {
        GROUPES_demployes_hr();
    }

    rendreInvisible();

    _allWindows->_groupes_DUN_EMPLOYE_window->rendreVisible(_curEmployeeTableModel,
                                                        	_curStocksTableModel);
}


void YerothDetailsDunGroupeDemployesWindow::setupLineEdits()
{
    lineEdit_details_dun_GROUPE_DEMPLOYES_reference_groupe->setYerothEnabled(false);
    lineEdit_details_dun_GROUPE_DEMPLOYES_designation->setYerothEnabled(false);
    lineEdit_details_dun_GROUPE_DEMPLOYES_maximum_de_membres->setYerothEnabled(false);
    lineEdit_details_dun_GROUPE_DEMPLOYES_groupe_de_paie->setYerothEnabled(false);
}


void YerothDetailsDunGroupeDemployesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDePaie, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerothDetailsDunGroupeDemployesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDePaie, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_groupes_de_paie->enable(this, SLOT(GROUPES_DE_PAIE_hr()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(GROUPES_demployes_hr()));
}


void YerothDetailsDunGroupeDemployesWindow::
		rendreVisible(YerothSqlTableModel *anEmployeeTableModel,
                      YerothSqlTableModel *anEmployeeGroupTableModel,
                      YerothSqlTableModel *stocksTableModel,
                      const QString &employeeGroup_db_ID /* = YerothUtils::EMPTY_STRING */)
{
	retranslateUi(this);

    disconnect(actionRETOUR, 0, 0, 0);

    pushButton_RETOUR->disable(this);

    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(GROUPES_demployes_hr()));

    pushButton_RETOUR->enable(this, SLOT(GROUPES_demployes_hr()));


    _curEmployeeTableModel = 0;

    _curStocksTableModel = stocksTableModel;

    _curEmployeeGroupTableModel = anEmployeeGroupTableModel;

    setVisible(true);

    showEmployeeGroup_DETAIL(employeeGroup_db_ID);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("DÉTAILS D'UN GROUPE D'EMPLOYÉS [hr]"),
    								 _curEmployeeGroupTableModel);
}


void YerothDetailsDunGroupeDemployesWindow::
		rendreVisible(YerothSqlTableModel *clientTableModel,
                      YerothSqlTableModel *stocksTableModel,
                      const QString &EMPLOYEE_Group_db_ID /* = YerothUtils::EMPTY_STRING */)
{
	retranslateUi(this);

    disconnect(actionRETOUR, 0, 0, 0);

    pushButton_RETOUR->disable(this);

    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(afficher_groupes_dun_EMPLOYE()));

    pushButton_RETOUR->enable(this, SLOT(afficher_groupes_dun_EMPLOYE()));


    _curEmployeeTableModel = clientTableModel;

    _curStocksTableModel = stocksTableModel;

    _curEmployeeGroupTableModel = &_allWindows->getSqlTableModel_groupes_demployes_hr();

    setVisible(true);

    showEmployeeGroup_DETAIL(EMPLOYEE_Group_db_ID);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("DÉTAILS D'UN GROUPE D'EMPLOYÉS [hr]"),
    								 _curEmployeeGroupTableModel);
}


void YerothDetailsDunGroupeDemployesWindow::
		showEmployeeGroup_DETAIL(const QString & employeeGroup_db_ID /* = YerothUtils::EMPTY_STRING */)
{
    tableWidget_details_dun_GROUPE_DEMPLOYES_membres_initiaux_du_groupe
		->yerothClearTableWidgetContent();

    QSqlRecord record;

    if (employeeGroup_db_ID.isEmpty())
    {
        _allWindows->_groupe_demployes_window
						->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);
    }
    else
    {
        _curEmployeeGroupTableModel
			->setFilter(QString("%1='%2'")
						 .arg(YerothDatabaseTableColumn::ID,
                              employeeGroup_db_ID));

        record = _curEmployeeGroupTableModel->record(0);

        _curEmployeeGroupTableModel->resetFilter();
    }


    QString membres_du_groupe_db_ID = GET_SQL_RECORD_DATA(record,
                                                          YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);


    int curQuerySize = -1;

    QSqlQuery aSql_EMPLOYEE_TableModelQUERY;

    QString employeeTableModelQUERY_STR;


    QString AN_EMPLOYEE_Member_nom_entreprise;

    QString AN_EMPLOYEE_Member_reference;

    QString AN_EMPLOYEE_Member_db_ID;

    for (unsigned int i = 0; i < membres_du_groupe_db_ID_LIST.size(); ++i)
    {
        AN_EMPLOYEE_Member_db_ID = membres_du_groupe_db_ID_LIST.at(i).trimmed();

        if (AN_EMPLOYEE_Member_db_ID.isEmpty())
        {
            continue;
        }

        employeeTableModelQUERY_STR =
                        QString("select %1, %2 from %3 where %4='%5'")
							.arg(YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR,
								 YerothDatabaseTableColumn::NOM_ENTREPRISE,
								 YerothDatabase::FOURNISSEURS,
								 YerothDatabaseTableColumn::ID,
								 AN_EMPLOYEE_Member_db_ID);

        aSql_EMPLOYEE_TableModelQUERY.clear();

        curQuerySize = YerothUtils::execQuery(aSql_EMPLOYEE_TableModelQUERY,
                                              employeeTableModelQUERY_STR,
											  _logger);

        if (curQuerySize > 0)
        {
            if (aSql_EMPLOYEE_TableModelQUERY.next())
            {
                AN_EMPLOYEE_Member_reference =
                                aSql_EMPLOYEE_TableModelQUERY.value(0).toString();

                AN_EMPLOYEE_Member_nom_entreprise =
                                aSql_EMPLOYEE_TableModelQUERY.value(1).toString();

                tableWidget_details_dun_GROUPE_DEMPLOYES_membres_initiaux_du_groupe
					->addAClientGroupMember(AN_EMPLOYEE_Member_db_ID,
                                      	  	AN_EMPLOYEE_Member_reference,
											AN_EMPLOYEE_Member_nom_entreprise);
            }
        }
    }

//      QDEBUG_STRING_OUTPUT_2("designation",
//                      GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_details_dun_GROUPE_DEMPLOYES_reference_groupe
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr));

    lineEdit_details_dun_GROUPE_DEMPLOYES_designation
		->setText(GET_SQL_RECORD_DATA(record,
				  YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_details_dun_GROUPE_DEMPLOYES_maximum_de_membres
		->setText(GET_SQL_RECORD_DATA(record,
				  	  	  	  	      YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES));

    lineEdit_details_dun_GROUPE_DEMPLOYES_groupe_de_paie
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR));

    textEdit_details_dun_GROUPE_DEMPLOYES_description_groupe
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR));
}


void YerothDetailsDunGroupeDemployesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
