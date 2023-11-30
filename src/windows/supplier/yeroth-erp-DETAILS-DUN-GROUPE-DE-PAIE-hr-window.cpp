/*
 * yeroth-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-DETAILS-DUN-GROUPE-DE-PAIE-hr-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerothDetailsDunGroupeDePaieWindow::YerothDetailsDunGroupeDePaieWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothDetailsDunGroupeDePaieWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("DÉTAILS D'UN GROUPE DE PAIE"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
    		QString("QMessageBox {background-color: rgb(%1);}"
                    "QMessageBox QLabel {color: rgb(%2);}")
				.arg(COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);


    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais->setMaximum(100.0);

    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais->setYerothEnabled(false);


    setupLineEdits();


    textEdit_details_DUN_GROUPE_DE_PAIE_description->setYerothEnabled(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);


    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionGroupesDemployes, SIGNAL(triggered()), this, SLOT(GROUPES_DE_PAIE_hr()));
    connect(actionMenuFOURNISSEURS, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionRETOUR, SIGNAL(triggered()), this, SLOT(GROUPES_DE_PAIE_hr()));
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


void YerothDetailsDunGroupeDePaieWindow::setupLineEdits()
{
    lineEdit_details_DUN_GROUPE_DE_PAIE_reference->setYerothEnabled(false);
    lineEdit_details_DUN_GROUPE_DE_PAIE_designation->setYerothEnabled(false);
    lineEdit_details_dun_GroupeDePaie_MONTANT_A_PAYER->setYerothEnabled(false);
}


void YerothDetailsDunGroupeDePaieWindow::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, false);

    pushButton_groupes_de_paie->disable(this);
    pushButton_menu_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
}


void YerothDetailsDunGroupeDePaieWindow::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionRETOUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGroupesDemployes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenuFOURNISSEURS, true);

    pushButton_groupes_de_paie->enable(this, SLOT(GROUPES_DE_PAIE_hr()));
    pushButton_menu_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(GROUPES_DE_PAIE_hr()));
}


void YerothDetailsDunGroupeDePaieWindow::rendreInvisible()
{
    comboBox_DETAILS_UN_GROUPE_DE_PAIE_localisation->clear();
    YerothWindowsCommons::rendreInvisible();
}


void YerothDetailsDunGroupeDePaieWindow::
		rendreVisible(YerothSqlTableModel *curClient_LOYALTY_PROGRAM_TableModel,
				      YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curPAY_GROUP_TableModel =
                    curClient_LOYALTY_PROGRAM_TableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_db_ID();

    setVisible(true);

    showPayGroup_PROGRAM_DETAIL();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("DÉTAILS D'UN GROUPE DE PAIE"),
    								 _curPAY_GROUP_TableModel);
}


void YerothDetailsDunGroupeDePaieWindow::showPayGroup_PROGRAM_DETAIL()
{
    QSqlRecord record;

    QString designation_GROUPE_DE_PAIE;

    QString montant_a_payer;

    _allWindows->_GROUPES_DE_PAIE_Window
					->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    lineEdit_details_DUN_GROUPE_DE_PAIE_reference
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr));

    designation_GROUPE_DE_PAIE =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DESIGNATION);

    montant_a_payer =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL);


    lineEdit_details_DUN_GROUPE_DE_PAIE_designation
		->setText(designation_GROUPE_DE_PAIE);

    lineEdit_details_dun_GroupeDePaie_MONTANT_A_PAYER
		->setText(GET_CURRENCY_STRING_NUM(montant_a_payer.toDouble()));


    comboBox_DETAILS_UN_GROUPE_DE_PAIE_localisation
		->addItem(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LOCALISATION));


    doubleSpinBox_details_DUN_GROUPE_DE_PAIE_pourcentage_du_rabais
		->setValue(GET_SQL_RECORD_DATA(record,
									   YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS).toDouble());


    textEdit_details_DUN_GROUPE_DE_PAIE_description
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS));

    tableWidget_details_dun_groupe_de_paie_groupes_demployes_participants
		->yerothClearTableWidgetContent();

    QString SELECT_ALL_employee_GROUP_PARTICIPATING
				(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
					.arg(YerothDatabaseTableColumn::ID,
                         YerothDatabaseTableColumn::DESIGNATION,
                         YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
                         YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerothDatabase::GROUPES_DEMPLOYES_hr,
                         YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                         designation_GROUPE_DE_PAIE));


    QSqlQuery aQSqlQuery;

    int query_size = YerothUtils::execQuery(aQSqlQuery,
                                        	SELECT_ALL_employee_GROUP_PARTICIPATING);

    if (query_size <= 0)
    {
        return;
    }

    QString EMPLOYEE_group_db_ID;
    QString designation;
    QString reference_GROUPE_DEMPLOYES;
    QStringList EMPLOYEE_group_member_db_ID;

    while (aQSqlQuery.next())
    {
        EMPLOYEE_group_db_ID = aQSqlQuery.value(YerothDatabaseTableColumn::ID).toString();

        designation = aQSqlQuery.value(YerothDatabaseTableColumn::DESIGNATION).toString();

        reference_GROUPE_DEMPLOYES =
        		aQSqlQuery.value(YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr).toString();

        EMPLOYEE_group_member_db_ID.clear();

        YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING
						(EMPLOYEE_group_member_db_ID,
                         aQSqlQuery.value(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID).toString());

        tableWidget_details_dun_groupe_de_paie_groupes_demployes_participants
			->insert_group(EMPLOYEE_group_db_ID,
						   designation,
						   reference_GROUPE_DEMPLOYES,
						   QString::number(EMPLOYEE_group_member_db_ID.size()));
    }
}


void YerothDetailsDunGroupeDePaieWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
