/*
 * yeroth-erp-GROUPES-dun-employe-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-GROUPES-dun-employe-hr-window.hpp"

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


YerothGROUPES_DUN_EMPLOYE_Window::YerothGROUPES_DUN_EMPLOYE_Window()
:YerothWindowsCommons("yeroth-erp-GROUPES-dun-employe-hr"),
 _logger(new YerothLogger("YerothGROUPES_DUN_EMPLOYE_Window")),
 _curEMPLOYEEgroups_TableModel(0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("groupes d'1 employé"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_Groupes_Dun_Employe->font());

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
                            	 .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                            		  COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _curEMPLOYEEgroups_TableModel = &_allWindows->getSqlTableModel_fournisseurs();

    setupLineEdits();

    populateComboBoxes();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));

    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
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


    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
			SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));

    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionListerDesGroupesDemployes, SIGNAL(triggered()), this, SLOT(GROUPES_demployes_hr()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAfficher_ce_groupe_DEMPLOYES_au_detail, SIGNAL(triggered()), this, SLOT(afficher_au_detail()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));


    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(addedYerothTableWidget()),
            this,
            SLOT(enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()));

    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(doubleClicked(const QModelIndex &)),
            this,
			SLOT(afficher_au_detail(const QModelIndex &)));

    connect(comboBox_Groupes_Dun_Employe_recherche,
            SIGNAL(activated(const QString &)),
            this,
			SLOT(ajouter_appartenance(const QString &)));

    setupShortcuts();

    comboBox_Groupes_Dun_Employe_recherche->setYerothEditable(true);

    comboBox_Groupes_Dun_Employe_recherche->setFocus();
}


bool YerothGROUPES_DUN_EMPLOYE_Window::imprimer_pdf_document()
{
//	QDEBUG_STRING_OUTPUT_1("YerothGROUPES_DUN_EMPLOYE_Window::imprimer_pdf_document");

	if (tableWidget_Groupes_Dun_Employe->rowCount() <= 0)
	{
		YerothQMessageBox::information(this,
				QObject::tr("impression"),
				QObject::tr("Il n'y a pas de données à imprimer !"));

		return false;
	}

	QString GROUPES_DUN_EMPLOYE__tex_table;

	//int pageNumber = qCeil(tableWidget_Groupes_Dun_Employe->rowCount() / 20);


	YerothPOSUser *yerothUser = _allWindows->getUser();

	YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

	QString GROUPES_DUN_EMPLOYE__TexDocument;

	QString factureDate(infoEntreprise.getVille_LATEX());

	YerothUtils::getCurrentSimplifiedDate(factureDate);

	get_PRINT_OUT_TexTableString(GROUPES_DUN_EMPLOYE__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(25);

	YerothUtils::get_GROUPES_DUN_EMPLOYE_TexDocumentString(GROUPES_DUN_EMPLOYE__TexDocument,
														   GROUPES_DUN_EMPLOYE__tex_table);

	emit SIGNAL_INCREMENT_PROGRESS_BAR(50);

	QString NomEmploye(_curEMPLOYEE_NOM_ENTREPRISE);

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		GROUPES_DUN_EMPLOYE__TexDocument
		.replace("YEROTHSUBJECT",
				QString("EMPLOYEE '%1' belonging GROUPS")
				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye)));
	}
	else
	{
		GROUPES_DUN_EMPLOYE__TexDocument
		.replace("YEROTHSUBJECT",
				QString("Groupes d'appartenance de l'EMPLOYÉ '%1'")
				.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye)));
	}

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHPAPERSPEC",
											 get_PRINTING_PARAMETER_printing_position());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHENTREPRISE", infoEntreprise.getNomCommercial_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHACTIVITESENTREPRISE", infoEntreprise.getSecteursActivitesTex());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHBOITEPOSTALE", infoEntreprise.getBoitePostal());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHDATE", factureDate);

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHHEUREVENTE", CURRENT_TIME);

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHCOMPTEBANCAIRENR", infoEntreprise.getNumeroCompteBancaire());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHCONTRIBUABLENR", infoEntreprise.getNumeroDeContribuable());

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE", infoEntreprise.getAgenceCompteBancaireTex());


	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHNOMUTILISATEUR",
			QString("%1 %2")
			.arg(YerothUtils::getAllWindows()->getUser()->titre(),
					YerothUtils::getAllWindows()->getUser()->nom_completTex()));

	GROUPES_DUN_EMPLOYE__TexDocument
	.replace("YEROTHSUCCURSALE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents
			(YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);


	emit SIGNAL_INCREMENT_PROGRESS_BAR(70);


	YerothSqlTableModel &supplierTableModel = _allWindows->getSqlTableModel_fournisseurs();

	supplierTableModel.yerothSetFilter_WITH_where_clause(
			QString("%1 = '%2' && %3 = '1'")
			.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
					NomEmploye,
					YerothDatabaseTableColumn::EMPLOYE));

	GROUPES_DUN_EMPLOYE__TexDocument.replace("YEROTHEMPLOYE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(NomEmploye));

	if (supplierTableModel.easySelect("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 251) > 0)
	{
		QSqlRecord record = supplierTableModel.record(0);

		QString EMPLOYE_Ville(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::VILLE));

		QString EMPLOYE_POBox(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::BOITE_POSTALE));

		QString EMPLOYE_Email(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

		QString EMPLOYE_Tel(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

		if (EMPLOYE_Tel.isEmpty())
		{
			EMPLOYE_Tel.append(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
		}

		//qDebug() << "++ EMPLOYE_Email: " << EMPLOYE_Email;

		if (!EMPLOYE_Ville.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument
			.replace("EMPLOYEYEROTHCITY",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Ville));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHCITY", "");
		}

		if (!EMPLOYE_POBox.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHPOBOX", EMPLOYE_POBox);
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHPOBOX", "N/a");
		}

		if (!EMPLOYE_Email.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHMAIL",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Email));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHMAIL", "");
		}

		if (!EMPLOYE_Tel.isEmpty())
		{
			GROUPES_DUN_EMPLOYE__TexDocument
			.replace("EMPLOYEYEROTHPHONE",
					YerothUtils::LATEX_IN_OUT_handleForeignAccents(EMPLOYE_Tel));
		}
		else
		{
			GROUPES_DUN_EMPLOYE__TexDocument.replace("EMPLOYEYEROTHPHONE", "");
		}

		supplierTableModel.resetFilter();
	}


	QString prefixFileName;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-employee-group-hr"));
	}
	else
	{
		prefixFileName.append(
				YerothUtils::getUniquePrefixFileInTemporaryFilesDir("yeroth-erp-GROUPES-DUN-EMPLOYE-HR"));
	}

	//qDebug() << "++\n" << GROUPES_DUN_EMPLOYE__TexDocument;

	QFile tmpLatexFile(prefixFileName + "tex");

	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(GROUPES_DUN_EMPLOYE__TexDocument.toUtf8());
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


void YerothGROUPES_DUN_EMPLOYE_Window::incrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_Groupes_Dun_Employe &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		++_TABLE_FONT_SIZE;

		_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

		//QDEBUG_STRING_OUTPUT_2_N("incrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
		//						  _TABLE_FONT_SIZE);

		tableWidget_Groupes_Dun_Employe->hide();

		tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();

		tableWidget_Groupes_Dun_Employe->show();

        YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
	}
}


void YerothGROUPES_DUN_EMPLOYE_Window::decrementFontSize__OF_TABLE()
{
	if (0 != tableWidget_Groupes_Dun_Employe &&
		0 != _CURRENT_TABLE_VIEW_font)
	{
		_TABLE_FONT_SIZE = _CURRENT_TABLE_VIEW_font->pointSize();

		if (_TABLE_FONT_SIZE > 5)
		{
			--_TABLE_FONT_SIZE;

			_CURRENT_TABLE_VIEW_font->setPointSize(_TABLE_FONT_SIZE);

			//QDEBUG_STRING_OUTPUT_2_N("decrementFontSize__OF_TABLE, _TABLE_FONT_SIZE",
			//						  _TABLE_FONT_SIZE);

			tableWidget_Groupes_Dun_Employe->hide();

			tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();

			tableWidget_Groupes_Dun_Employe->show();

            YerothERPWindows *allWindows = YerothUtils::getAllWindows();

            YR_SAVE_USER_SETTINGS_QTABLEWIDGET(this);
		}
	}
}


void YerothGROUPES_DUN_EMPLOYE_Window::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_menuGROUPES_DE_PAIE_DUN_EMPLOYE_Window->palette());
    menu.addAction(actionAfficher_ce_groupe_DEMPLOYES_au_detail);
    menu.exec(event->globalPos());
}


void YerothGROUPES_DUN_EMPLOYE_Window::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    //setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothGROUPES_DUN_EMPLOYE_Window::
	executer_ajouter_appartenance(const QString *un_groupe_DEMPLOYES_hr)
{
    if (0 == un_groupe_DEMPLOYES_hr)
    {
        return;
    }

    QString SELECT_GROUP_FROM_DB_TABLE
		(QString("select %1, %2, %3, %4 from %5 where %6='%7'")
			.arg(YerothDatabaseTableColumn::ID,
                 YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
                 YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
                 YerothDatabase::GROUPES_DEMPLOYES_hr,
                 YerothDatabaseTableColumn::DESIGNATION,
                 *un_groupe_DEMPLOYES_hr));

    QSqlQuery aQSqlQuery;

    int query_size = YerothUtils::execQuery(aQSqlQuery, SELECT_GROUP_FROM_DB_TABLE);

    if (query_size <= 0)
    {
        return;
    }

    QSqlRecord AN_EMPLOYEE_GroupRecordInfo;

    if (!aQSqlQuery.next())
    {
        return;
    }

    AN_EMPLOYEE_GroupRecordInfo = aQSqlQuery.record();

    int maximum_de_membres = GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                                 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES).
                             toInt();

    QString membres_du_groupe_db_ID =
                    GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                        YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);

    if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr
                                       ("LE GROUPE D'EMPLOYÉS '%1' a déjà atteint son nombre maximum de membres (%2) !").
                                       arg(*un_groupe_DEMPLOYES_hr,
                                           QString::number(maximum_de_membres)));

        return;
    }


    if (YerothUtils::
    		APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(_curEmployeeDBID, membres_du_groupe_db_ID))
    {

        QString INSERT_UPDATE_employee_WITHIN_GROUP
			(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
				.arg(YerothDatabase::GROUPES_DEMPLOYES_hr,
					 YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                     membres_du_groupe_db_ID,
                     YerothDatabaseTableColumn::DESIGNATION,
                     *un_groupe_DEMPLOYES_hr));

        bool insert_update_success =
        		YerothUtils::execQuery(INSERT_UPDATE_employee_WITHIN_GROUP);

//              QDEBUG_STRING_OUTPUT_2("insert_update_success - 1", BOOL_TO_STRING(insert_update_success));

        if (!insert_update_success)
        {
            YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                           QObject::tr
                                           ("L'EMPLOYÉ '%1' n'a pas pu être ajouté "
                                            "dans le groupe des employés '%2' !").
                                           arg(_curEMPLOYEE_NOM_ENTREPRISE,
                                               *un_groupe_DEMPLOYES_hr));

            return;
        }
    }

    QSqlRecord record;

    _allWindows->_fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString groupes_dun_employe =
    		GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE);

    bool update_DB_GROUPES_DUN_EMPLOYE_TABLE =
                    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (*un_groupe_DEMPLOYES_hr,
                     groupes_dun_employe);

    QString groupes_dun_employe_id =
    		GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID);

    QString un_groupe_DEMPLOYES_db_ID =
                    GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                        YerothDatabaseTableColumn::ID);


    bool update_groupes_dun_employe_id =
                    YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING
                    (un_groupe_DEMPLOYES_db_ID,
                     groupes_dun_employe_id);

    update_DB_GROUPES_DUN_EMPLOYE_TABLE =
                    update_DB_GROUPES_DUN_EMPLOYE_TABLE || update_groupes_dun_employe_id;


    if (!update_DB_GROUPES_DUN_EMPLOYE_TABLE)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr("L'EMPLOYÉ '%1' EST DÉJÀ MEMBRE DU GROUPE D'EMPLOYÉS '%2' !")
									   	   .arg(_curEMPLOYEE_NOM_ENTREPRISE,
									   			*un_groupe_DEMPLOYES_hr));
        return;
    }


    QString UPDATE_employee_GROUP_INFORMATION
		(QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
				.arg(YerothDatabase::FOURNISSEURS,
                     YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                     groupes_dun_employe,
                     YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                     groupes_dun_employe_id,
                     YerothDatabaseTableColumn::ID,
                     _curEmployeeDBID));


    bool UPDATE_EMPLOYEE_GROUP_SUCCESS =
                    YerothUtils::execQuery(UPDATE_employee_GROUP_INFORMATION);

    if (!UPDATE_EMPLOYEE_GROUP_SUCCESS)
    {
        YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                       QObject::tr("L'employé '%1' n'a pas pu être ajouté "
                                    		   	   	   "dans le GROUPE D'EMPLOYÉS '%2' !")
												.arg(_curEMPLOYEE_NOM_ENTREPRISE,
													 *un_groupe_DEMPLOYES_hr));

        YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

        return;
    }

    tableWidget_Groupes_Dun_Employe
		->insert_group(GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                       YerothDatabaseTableColumn::ID),
					   *un_groupe_DEMPLOYES_hr,
					   GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                           YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr),
                       QString::number(maximum_de_membres));


    lineEdit_Groupes_Dun_Employe_nombre_de_groupes
		->setText(QString::number(tableWidget_Groupes_Dun_Employe->rowCount()));


    YerothQMessageBox::information(this, QObject::tr("ajouter"),
                                   QObject::tr("L'EMPLOYÉ '%1' a été ajouté dans le groupe des employés '%2' !")
								   	   .arg(_curEMPLOYEE_NOM_ENTREPRISE,
								   			*un_groupe_DEMPLOYES_hr));
}


bool YerothGROUPES_DUN_EMPLOYE_Window::EXECUTER_retirer_cet_employe_du_groupe_selectionne()
{
    QString SELECT_GROUPES_DUN_EMPLOYE_INFO(
    			QString("select %1, %2 from %3 where %4='%5'")
					.arg(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                         YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         YerothDatabase::FOURNISSEURS,
                         YerothDatabaseTableColumn::ID,
                         _curEmployeeDBID));

    QSqlQuery aQSqlQuery;

    int query_size = YerothUtils::execQuery(aQSqlQuery, SELECT_GROUPES_DUN_EMPLOYE_INFO);

    if (query_size <= 0)
    {
        return false;
    }

    QSqlRecord aClientRecordGroupInfo;

    if (!aQSqlQuery.next())
    {
        return false;
    }

    aClientRecordGroupInfo = aQSqlQuery.record();

    QString groupes_dun_EMPLOYE_ID =
    		GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID);

    QString groupes_dun_EMPLOYE =
    		GET_SQL_RECORD_DATA(aClientRecordGroupInfo,
                                YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE);

    int currentRow = tableWidget_Groupes_Dun_Employe->currentRow();

//      QDEBUG_STRING_OUTPUT_1(QString("groupes_du_client_ID: %1, currentRow: %2")
//                                                              .arg(groupes_du_client_ID,
//                                                                       QString::number(currentRow)));

    QString EMPLOYEE_Group_db_ID = tableWidget_Groupes_Dun_Employe->get_DB_ELEMENT_db_ID(currentRow);

//      QDEBUG_STRING_OUTPUT_1(QString("EMPLOYEE_Group_db_ID: %1, groupes_dun_EMPLOYE_ID: %2")
//                                                              .arg(EMPLOYEE_Group_db_ID,
//                                                                   groupes_dun_EMPLOYE_ID));

    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_EMPLOYE_ID,
    															   EMPLOYEE_Group_db_ID);

//      QDEBUG_STRING_OUTPUT_1(QString("EMPLOYEE_Group_db_ID: %1, groupes_dun_EMPLOYE_ID: %2")
//                                                              .arg(EMPLOYEE_Group_db_ID,
//                                                                       groupes_dun_EMPLOYE_ID));

    QTableWidgetItem *item = tableWidget_Groupes_Dun_Employe->item(currentRow, 0);

    if (0 == item)
    {
        return false;
    }

    QString employeeGroup_designation = item->text().trimmed();

    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_EMPLOYE,
    														       employeeGroup_designation);

    bool result = true;

    {
        QString UPDATE_GROUPES_DU_CLIENT_INFO(
        			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
						.arg(YerothDatabase::FOURNISSEURS,
                             YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                             groupes_dun_EMPLOYE,
                             YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                             groupes_dun_EMPLOYE_ID,
                             YerothDatabaseTableColumn::ID,
                             _curEmployeeDBID));

        result = result && YerothUtils::execQuery(UPDATE_GROUPES_DU_CLIENT_INFO);
    }

    {
        aQSqlQuery.clear();

        QSqlRecord a_client_group_record_info;

        SELECT_GROUPES_DUN_EMPLOYE_INFO.clear();

        SELECT_GROUPES_DUN_EMPLOYE_INFO =
                        QString("select %1 from %2 where %3='%4'").arg
                        (YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerothDatabase::GROUPES_DEMPLOYES_hr,
                         YerothDatabaseTableColumn::DESIGNATION,
						 employeeGroup_designation);

//              QDEBUG_STRING_OUTPUT_2("SELECT_GROUPES_DUN_EMPLOYE_INFO",
//                                     SELECT_GROUPES_DUN_EMPLOYE_INFO);

        result = result && YerothUtils::execQuery(aQSqlQuery, SELECT_GROUPES_DUN_EMPLOYE_INFO);

        if (!aQSqlQuery.next())
        {
            return false;
        }

        a_client_group_record_info = aQSqlQuery.record();

        QString membres_du_groupe_db_ID =
        			GET_SQL_RECORD_DATA(a_client_group_record_info,
                                        YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

//      QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID *",
//                             membres_du_groupe_db_ID);


        YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID,
        															   _curEmployeeDBID);


//              QDEBUG_STRING_OUTPUT_2("membres_du_groupe_db_ID ***",
//                                                         membres_du_groupe_db_ID);


        QString UPDATE_GROUPES_DUN_EMPLOYE_DB_TABLE
					(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
						.arg(YerothDatabase::GROUPES_DEMPLOYES_hr,
                             YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                             membres_du_groupe_db_ID,
                             YerothDatabaseTableColumn::DESIGNATION,
                             employeeGroup_designation));

        result = result && YerothUtils::execQuery(UPDATE_GROUPES_DUN_EMPLOYE_DB_TABLE);
    }

    if (result)
    {
        tableWidget_Groupes_Dun_Employe->removeArticle(currentRow);
    }

    int tableRowCount = tableWidget_Groupes_Dun_Employe->rowCount();

    if (tableRowCount > 0)
    {
        enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else
    {
        disable_yeroth_widgets();
    }

    lineEdit_Groupes_Dun_Employe_nombre_de_groupes
		->setText(QString::number(tableWidget_Groupes_Dun_Employe->rowCount()));

    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();

    return result;
}


void YerothGROUPES_DUN_EMPLOYE_Window::afficher_au_detail(const QModelIndex &modelIndex)
{
    if (_curEMPLOYEEgroups_TableModel->rowCount() > 0)
    {
        _allWindows->_DETAILS_DUN_GROUPE_DEMPLOYES_hr
						->rendreVisible(_curEMPLOYEEgroups_TableModel,
                                        _curStocksTableModel,
                                        tableWidget_Groupes_Dun_Employe->get_DB_ELEMENT_db_ID(modelIndex.row()));

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 compte client à afficher les détails."));
    }
}


void YerothGROUPES_DUN_EMPLOYE_Window::afficher_tous_les_groupes_DUN_EMPLOYE_hr()
{
    tableWidget_Groupes_Dun_Employe->yerothClearTableWidgetContent();

    QSqlRecord record;

    _allWindows->_fournisseursWindow
		->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curEMPLOYEE_NOM_ENTREPRISE =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::NOM_ENTREPRISE);

    _curEmployeeDBID =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);


    groupBox_Groupes_Dun_Employe->setTitle(QObject::tr("Groupes d'appartenance DE L'EMPLOYÉ (%1)")
    										.arg(_curEMPLOYEE_NOM_ENTREPRISE));


    QString groupes_de_lemploye
		(GET_SQL_RECORD_DATA(record,
    						 YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE));


    QStringList allEmployeeGroups(groupes_de_lemploye.split(YerothUtils::STAR_CHAR));

    QString current_group_db_ID;

    QString maximum_de_membres;

    QString current_EMPLOYEE_PAY_GROUP;

    int curQuerySize = -1;

    QSqlQuery aSqlGroupTableModelQUERY;

    QString employeeGroupTableModelQUERY_STR;

    for (int k = 0; k < allEmployeeGroups.size(); ++k)
    {
        employeeGroupTableModelQUERY_STR =
                        QString("select %1, %2, %3 from %4 where %5='%6'")
							.arg(YerothDatabaseTableColumn::ID,
								 YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
								 YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
								 YerothDatabase::GROUPES_DEMPLOYES_hr,
								 YerothDatabaseTableColumn::DESIGNATION,
								 allEmployeeGroups.at(k).trimmed());

        aSqlGroupTableModelQUERY.clear();

        curQuerySize = YerothUtils::execQuery(aSqlGroupTableModelQUERY,
                                              employeeGroupTableModelQUERY_STR,
                                              _logger);

        if (curQuerySize > 0)
        {
            if (aSqlGroupTableModelQUERY.next())
            {
                current_group_db_ID =
                                QString::number(aSqlGroupTableModelQUERY.value(0).toInt());

                current_EMPLOYEE_PAY_GROUP =
                                aSqlGroupTableModelQUERY.value(1).toString();

                maximum_de_membres = aSqlGroupTableModelQUERY.value(2).toString();

                tableWidget_Groupes_Dun_Employe
					->insert_group(current_group_db_ID,
                                   allEmployeeGroups.at(k).trimmed(),
								   current_EMPLOYEE_PAY_GROUP,
								   maximum_de_membres);
            }

//              qDebug() << tableWidget_Groupes_Dun_Employe->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_Groupes_Dun_Employe->rowCount() > 0)
    {
        tableWidget_Groupes_Dun_Employe->selectRow(0);
    }
}


void YerothGROUPES_DUN_EMPLOYE_Window::enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    actionAfficher_ce_groupe_DEMPLOYES_au_detail->setVisible(true);

    pushButton_retirer->setVisible(true);
}


void YerothGROUPES_DUN_EMPLOYE_Window::disable_yeroth_widgets()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    actionAfficher_ce_groupe_DEMPLOYES_au_detail->setVisible(false);

    pushButton_retirer->setVisible(false);
}


void YerothGROUPES_DUN_EMPLOYE_Window::get_PRINT_OUT_TexTableString(QString &texTable_IN_OUT)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
						   //"\\resizebox{\\textwidth}{!}{\n"
						   "\\centering\n"
						   "\\begin{tabular}"
						   "{|l|l|r|} \\hline");

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texTable_IN_OUT.append("& & 			\\\\ \n"
							   "GROUP NAME		& 	 \n"
							   "PAY GROUP  		& 	 \n"
							   "MAX MEMBERS		\\\\ \n"
							   "& &				\\\\ \\hline \\hline \n");
	}
	else //FRANCAIS
	{
		texTable_IN_OUT.append("& & 						\\\\ \n"
							   "DÉSIGNATION DU GROUPE		& 	 \n"
							   "GROUP DE PAIE 				& 	 \n"
							   "NOMBRE DE MEMBRES MAXIMUM	\\\\ \n"
							   "& &							\\\\ \\hline \\hline \n");
	}


	int rowCount = tableWidget_Groupes_Dun_Employe->rowCount();

	int columnCount = tableWidget_Groupes_Dun_Employe->columnCount();

	bool color_this_row_grey = true;

	QString cell_text;

	//Tex table body
	for (int i = 0; i < rowCount; ++i)
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

		for (int j = 0; j < columnCount; ++j)
		{
			if (j != 2)
			{
				cell_text = tableWidget_Groupes_Dun_Employe->item(i, j)->text();
			}
			else
			{
				cell_text = GET_NUM_STRING(tableWidget_Groupes_Dun_Employe->item(i, j)->text().toDouble());

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
	}

	texTable_IN_OUT.append("\\hline\n"
						   //"\\end{tabular}}\n"
						   "\\end{tabular}\n"
						   "\\end{table*}\n");
}


void YerothGROUPES_DUN_EMPLOYE_Window::populateComboBoxes()
{
	comboBox_Groupes_Dun_Employe_recherche
    	->populateComboBoxRawString(YerothDatabase::GROUPES_DEMPLOYES_hr,
                              	    YerothDatabaseTableColumn::DESIGNATION);

	comboBox_Groupes_Dun_Employe_recherche->setYerothEnabled(true);
}


void YerothGROUPES_DUN_EMPLOYE_Window::rendreInvisible()
{
    tableWidget_Groupes_Dun_Employe->yerothClearTableWidgetContent();

    YerothWindowsCommons::rendreInvisible();
}


void YerothGROUPES_DUN_EMPLOYE_Window::rendreVisible(YerothSqlTableModel *EMPLOYEE_TableModel,
                                                 	 YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curEMPLOYEEgroups_TableModel = EMPLOYEE_TableModel;

    populateComboBoxes();

    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();

    afficher_tous_les_groupes_DUN_EMPLOYE_hr();

    int tableRowCount = tableWidget_Groupes_Dun_Employe->rowCount();

    if (tableRowCount > 0)
    {
        enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();
    }
    else
    {
        disable_yeroth_widgets();
    }

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("groupes d'1 employé"),
    								 _curEMPLOYEEgroups_TableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS_QTABLEWIDGET(*this);

    lineEdit_Groupes_Dun_Employe_nombre_de_groupes->setText(GET_NUM_STRING(tableRowCount));

    tableWidget_Groupes_Dun_Employe->resizeColumnsToContents();
}


void YerothGROUPES_DUN_EMPLOYE_Window::definirManager()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(true);

    pushButton_groupes_demployes->enable(this, SLOT(GROUPES_demployes_hr()));
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(fournisseurs()));
    pushButton_retirer->enable(this, SLOT(retirer_CET_EMPLOYE_du_groupe_selectionne()));
}


void YerothGROUPES_DUN_EMPLOYE_Window::definirVendeur()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(false);

    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_RETOUR->enable(this, SLOT(fournisseurs()));
    pushButton_retirer->disable(this);
}


void YerothGROUPES_DUN_EMPLOYE_Window::definirPasDeRole()
{
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionListerDesGroupesDemployes,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficher_ce_groupe_DEMPLOYES_au_detail,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    comboBox_Groupes_Dun_Employe_recherche->setVisible(false);

    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);
}
