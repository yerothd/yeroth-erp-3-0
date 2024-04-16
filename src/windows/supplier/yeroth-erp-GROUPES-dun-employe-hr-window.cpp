/*
 * yeroth-erp-GROUPES-dun-employe-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-GROUPES-dun-employe-hr-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "yeroth-erp-calcul-salaire-EMPLOYE-window.hpp"

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
 _curEMPLOYEEgroups_TableModel(0),
 _somme_SALAIRE_CUMULE_A_DATE(0.0)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("groupes d'1 employé"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    tableWidget_Groupes_Dun_Employe->setSortingEnabled(true);


    _CURRENT_TABLE_VIEW_font = const_cast<QFont *>(&tableWidget_Groupes_Dun_Employe->font());

    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
                            	 .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                            		  COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _curEMPLOYEEgroups_TableModel = &_allWindows->getSqlTableModel_fournisseurs();


    dateEdit_groupe_dun_employe_date_begin->setStartDate(GET_CURRENT_DATE);

    dateEdit_groupe_dun_employe_date_end->setStartDate(GET_CURRENT_DATE);


    dateEdit_groupe_dun_employe_date_begin->setDate(GET_CURRENT_DATE);

    dateEdit_groupe_dun_employe_date_end->setDate(GET_CURRENT_DATE);


    dateEdit_groupe_dun_employe_date_begin->setYerothEnabled(false);

    dateEdit_groupe_dun_employe_date_end->setYerothEnabled(false);


    setupLineEdits();


    populateComboBoxes();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

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


    pushButton_VALIDER->disable(this);
    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);


    connect(action_parametrer_les_impressions,
            SIGNAL(triggered()),
            this,
            SLOT(setup_print()));

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

    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
			SLOT(export_csv_file()));


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


    connect(checkBox_activer_DATES_DAPPARTENANCE,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(handle_activer_DATES_DAPPARTENANCE(int)));

    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(addedYerothTableWidget()),
            this,
            SLOT(enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()));

    connect(tableWidget_Groupes_Dun_Employe,
            SIGNAL(clicked(const QModelIndex &)),
            this,
			SLOT(handle_DATE_DEBUT_et_DATE_FIN_dappartenance(const QModelIndex &)));

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


bool YerothGROUPES_DUN_EMPLOYE_Window::export_csv_file()
{
    bool success = false;

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success =
            YerothUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_Groupes_Dun_Employe,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "Employee group export listing");
    }
    else
    {
        success =
            YerothUtils::SAVE_AS_csv_file(*this,
                                          *tableWidget_Groupes_Dun_Employe,
                                          QString("%1-listing-csv-format")
                                          .arg(_output_print_pdf_latexFileNamePrefix),
                                          "FICHE D'EXPORTATION DES groupes d'1 employé");
    }

    return success;
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

	if (supplierTableModel.easySelect("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 284) > 0)
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


void YerothGROUPES_DUN_EMPLOYE_Window::
        handle_DATE_DEBUT_et_DATE_FIN_dappartenance(int row)
{
    QTableWidgetItem *item_designation =
        tableWidget_Groupes_Dun_Employe->item(row,
                                              0);

    QTableWidgetItem *item_GROUPE_DE_PAIE_HR =
        tableWidget_Groupes_Dun_Employe->item(row,
                                              1);

    if (0 != item_designation &&
        0 != item_GROUPE_DE_PAIE_HR)
    {
        QString designation_GROUPE_DEMPLOYES_HR = item_designation->text();


        //QDEBUG_STRING_OUTPUT_2("_curEMPLOYEE_NOM_ENTREPRISE", _curEMPLOYEE_NOM_ENTREPRISE);

        //QDEBUG_STRING_OUTPUT_2("designation_GROUPE_DEMPLOYES_HR", designation_GROUPE_DEMPLOYES_HR);

        //QDEBUG_STRING_OUTPUT_2("_curEMPLOYEE_groupe_de_paie_hr", _curEMPLOYEE_groupe_de_paie_hr);

        _curEMPLOYEE_groupe_de_paie_hr = item_GROUPE_DE_PAIE_HR->text();


        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM
                        (_map_GROUPE_SELECTIONNE__to__salaire
                            .value(_curEMPLOYEE_groupe_de_paie_hr)));


        if (!_curEMPLOYEE_NOM_ENTREPRISE.isEmpty() &&
            !_curEMPLOYEE_groupe_de_paie_hr.isEmpty())
        {
            YerothSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_TableModel
                = _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


            QString CONDITION = QString("(%1) AND (%2)")
                                    .arg(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                                              _curEMPLOYEE_NOM_ENTREPRISE),
                                         GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                                              _curEMPLOYEE_groupe_de_paie_hr));


            periodes_dappartenance_groupes_de_paie_hr_TableModel
                .yerothSetFilter_WITH_where_clause(CONDITION);


            int query_size = periodes_dappartenance_groupes_de_paie_hr_TableModel
                                .easySelect("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 491);


            //QDEBUG_STRING_OUTPUT_2_N("query_size", query_size);


            if (0 < query_size)
            {
                QSqlRecord a_record =
                    periodes_dappartenance_groupes_de_paie_hr_TableModel.record(0);

                QString dateEdit_groupe_dun_employe_date_begin_STRING =
                    GET_SQL_RECORD_DATA(a_record,
                                        YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE);

                QString dateEdit_groupe_dun_employe_date_end_STRING =
                    GET_SQL_RECORD_DATA(a_record,
                                        YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE);


                dateEdit_groupe_dun_employe_date_begin
                    ->setDate(GET_DATE_FROM_STRING(dateEdit_groupe_dun_employe_date_begin_STRING));

                dateEdit_groupe_dun_employe_date_end
                    ->setDate(GET_DATE_FROM_STRING(dateEdit_groupe_dun_employe_date_end_STRING));
            }
            else
            {
                dateEdit_groupe_dun_employe_date_begin->reset();
                dateEdit_groupe_dun_employe_date_end->reset();
            }

            periodes_dappartenance_groupes_de_paie_hr_TableModel
                .resetFilter("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 524);
        }
    }
    else
    {
        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));
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
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


bool YerothGROUPES_DUN_EMPLOYE_Window::CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE()
{
    bool successBeginEndDataSaved = false;


    QDate begin_DATE = dateEdit_groupe_dun_employe_date_begin->date();

    QDate end_DATE = dateEdit_groupe_dun_employe_date_end->date();


    YerothSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.resetFilter();


    QSqlRecord periodes_dappartenance_Record =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record();


    periodes_dappartenance_Record
        .setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                  _curEMPLOYEE_NOM_ENTREPRISE);

    periodes_dappartenance_Record
        .setValue(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                  _curEMPLOYEE_groupe_de_paie_hr);

    periodes_dappartenance_Record
        .setValue(YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
                  begin_DATE);

    periodes_dappartenance_Record
        .setValue(YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
                  end_DATE);


    int IDforPERIODESdappartenance =
        YerothERPWindows::getNextIdSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    periodes_dappartenance_Record.setValue(YerothDatabaseTableColumn::ID,
                                           IDforPERIODESdappartenance);


    successBeginEndDataSaved =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .insertNewRecord(periodes_dappartenance_Record,
                             0,
                             "src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp",
                             602);



    return successBeginEndDataSaved;
}


void YerothGROUPES_DUN_EMPLOYE_Window::handle_activer_DATES_DAPPARTENANCE(int state)
{
    if (checkBox_activer_DATES_DAPPARTENANCE->isChecked())
    {
        dateEdit_groupe_dun_employe_date_begin->setYerothEnabled(true);

        dateEdit_groupe_dun_employe_date_end->setYerothEnabled(true);
    }
    else
    {
        dateEdit_groupe_dun_employe_date_begin->setYerothEnabled(false);

        dateEdit_groupe_dun_employe_date_end->setYerothEnabled(false);
    }
}


void YerothGROUPES_DUN_EMPLOYE_Window::handle_VALIDER_button()
{
    if (!dateEdit_groupe_dun_employe_date_begin->isEnabled() ||
        !dateEdit_groupe_dun_employe_date_end->isEnabled())
    {
        return ;
    }


    QDate begin_DATE = dateEdit_groupe_dun_employe_date_begin->date();

    QDate end_DATE = dateEdit_groupe_dun_employe_date_end->date();


    if (end_DATE < begin_DATE)
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr("date de fin AVANT date de début !"),
                                   QObject::tr("La date de fin [\"%1\"] d'appartenance au "
                                               "groupe de paie (HR) '%2' DEVRAIT ÊTRE avant "
                                               "la date de début [\"%3\"] !")
                                       .arg(DATE_TO_STRING(end_DATE),
                                            _curEMPLOYEE_groupe_de_paie_hr,
                                            DATE_TO_STRING(begin_DATE)));
        return ;
    }

    if (begin_DATE < GET_CURRENT_DATE)
    {

        YerothQMessageBox::warning(this,
                                   QObject::tr("date de début PRÉ-DATÉE !"),
                                   QObject::tr("La date de début [\"%1\"] d'appartenance au "
                                               "groupe de paie (HR) '%2' DEVRAIT ÊTRE daté "
                                               "au minimum à partir de ce jour [\"%3\"] !")
                                       .arg(DATE_TO_STRING(begin_DATE),
                                            _curEMPLOYEE_groupe_de_paie_hr,
                                            DATE_TO_STRING(GET_CURRENT_DATE)));

        YerothQMessageBox::information(this,
                                       QObject::tr("NOUVELLES DATES D'APPARTENACE"),
                                       QObject::tr("Pour faire de nouvelles dates d'appartenance au "
                                                   "groupe de paie (HR) '%1' pour cet employé [\"%2\"], "
                                                   "VOUS DEVEZ LE RETIRER DE CE GROUPE, et ensuite l'y remettre !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));
        return ;
    }


    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;


    YerothSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        _allWindows->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    QString condition_exist_already =
        QString("(%1) AND (%2)")
            .arg(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                      _curEMPLOYEE_NOM_ENTREPRISE),
                 GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                      _curEMPLOYEE_groupe_de_paie_hr));


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .yerothSetFilter_WITH_where_clause(condition_exist_already);


    int query_size = periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                        .easySelect("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 701);


    //QDEBUG_STRING_OUTPUT_2_N("query_size", query_size);
    bool successBeginEndDataSaved = false;


    if (0 < query_size)
    {
        QSqlRecord a_record =
            periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record(0);


        QSqlRecord a_periodes_dappartenance_UpdateRecord(a_record);


        a_periodes_dappartenance_UpdateRecord
            .setValue(YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
                      begin_DATE);


        a_periodes_dappartenance_UpdateRecord
            .setValue(YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
                      end_DATE);


        successBeginEndDataSaved =
            periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                .updateRecord(0,
                              a_periodes_dappartenance_UpdateRecord,
                              "src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp",
                              728,
                              _curEMPLOYEE_NOM_ENTREPRISE);
    }
    else
    {
        successBeginEndDataSaved =
            CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE();
    }


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .resetFilter("src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.cpp", 743);


    if (successBeginEndDataSaved)
    {
        YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        YerothQMessageBox::information(this,
                                       QObject::tr("ajouter périodes d'appartenance"),
                                       QObject::tr("LES DATES D'APPARTENANCE AU GROUPE DE PAIE (HR) '%1' "
                                                   "POUR L'EMPLOYÉ [%2] ont été enregistrées avec succès !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));

    }
    else
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("ajouter périodes d'appartenance"),
                                       QObject::tr("LES DATES D'APPARTENANCE AU GROUPE DE PAIE (HR) '%1' "
                                                   "POUR L'EMPLOYÉ [%2] N'ONT PAS PU ÊTRE ENREGISTRÉES !")
                                         .arg(_curEMPLOYEE_groupe_de_paie_hr,
                                              _curEMPLOYEE_NOM_ENTREPRISE));

        YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;
    }

    checkBox_activer_DATES_DAPPARTENANCE->setCheckState(Qt::Unchecked);

    afficher_tous_les_groupes_DUN_EMPLOYE_hr();
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
                 YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
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

    int maximum_de_membres =
        GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                            YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES).toInt();

    QString membres_du_groupe_db_ID =
                    GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                        YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

    QStringList membres_du_groupe_db_ID_LIST;

    YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID_LIST,
                                                membres_du_groupe_db_ID);

    if (membres_du_groupe_db_ID_LIST.size() >= maximum_de_membres)
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("ajouter"),
                                       QObject::tr("LE GROUPE D'EMPLOYÉS '%1' a déjà atteint son nombre maximum de membres (%2) !")
                                         .arg(*un_groupe_DEMPLOYES_hr,
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

    int last_row_INSERTED =
        tableWidget_Groupes_Dun_Employe
            ->insert_group(GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo, YerothDatabaseTableColumn::ID),
                           *un_groupe_DEMPLOYES_hr,
                           GET_SQL_RECORD_DATA(AN_EMPLOYEE_GroupRecordInfo,
                                               YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR),
                            QString::number(maximum_de_membres));

    handle_VALIDER_button();


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


    QString EMPLOYEE_Group_db_ID = tableWidget_Groupes_Dun_Employe->get_DB_ELEMENT_db_ID(currentRow);


    YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(groupes_dun_EMPLOYE_ID,
    															   EMPLOYEE_Group_db_ID);


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
        QString UPDATE_GROUPES_DUN_EMPLOYE_INFO =
        			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'")
						.arg(YerothDatabase::FOURNISSEURS,
                             YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
                             groupes_dun_EMPLOYE,
                             YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                             groupes_dun_EMPLOYE_ID,
                             YerothDatabaseTableColumn::ID,
                             _curEmployeeDBID);

        result = result && YerothUtils::execQuery(UPDATE_GROUPES_DUN_EMPLOYE_INFO);
    }

    {
        QString UPDATE_PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR =
        			QString("DELETE FROM %1 WHERE %2='%3' AND %4='%5'")
						.arg(YerothDatabase::PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR,
                             YerothDatabaseTableColumn::NOM_ENTREPRISE,
                             _curEMPLOYEE_NOM_ENTREPRISE,
                             YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                             _curEMPLOYEE_groupe_de_paie_hr);

        result = result && YerothUtils::execQuery(UPDATE_PERIODES_DAPPARTENANCE_GROUPES_DE_PAIE_HR);
    }

    {
        aQSqlQuery.clear();

        QSqlRecord an_employee_group_record_info;

        SELECT_GROUPES_DUN_EMPLOYE_INFO.clear();

        SELECT_GROUPES_DUN_EMPLOYE_INFO =
                        QString("select %1 from %2 where %3='%4'").arg
                        (YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
                         YerothDatabase::GROUPES_DEMPLOYES_hr,
                         YerothDatabaseTableColumn::DESIGNATION,
						 employeeGroup_designation);

        result = result && YerothUtils::execQuery(aQSqlQuery, SELECT_GROUPES_DUN_EMPLOYE_INFO);

        if (!aQSqlQuery.next())
        {
            return false;
        }

        an_employee_group_record_info = aQSqlQuery.record();

        QString membres_du_groupe_db_ID =
        			GET_SQL_RECORD_DATA(an_employee_group_record_info,
                                        YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID);

        YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(membres_du_groupe_db_ID,
        															   _curEmployeeDBID);

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
    else // <= 0
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));


        lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));


        disable_yeroth_widgets();
    }


    lineEdit_Groupes_Dun_Employe_nombre_de_groupes
		->setText(QString::number(tableWidget_Groupes_Dun_Employe->rowCount()));


    afficher_tous_les_groupes_DUN_EMPLOYE_hr();


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
    _somme_SALAIRE_CUMULE_A_DATE = 0.0;

    _map_GROUPE_SELECTIONNE__to__salaire.clear();

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


    double CALCULATED_SALARY_for_EMPLOYEE = 0.0;

    YerothERPCalculSalaireEMPLOYE CHECK_VALIDITY_DATES_for_belonging_of_EMPLOYEE;


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


                CALCULATED_SALARY_for_EMPLOYEE =
                    CHECK_VALIDITY_DATES_for_belonging_of_EMPLOYEE
                        .calculate_PAY_GROUP_MONEY_BENEFITS(_curEMPLOYEE_NOM_ENTREPRISE,
                                                            current_EMPLOYEE_PAY_GROUP);


                _map_GROUPE_SELECTIONNE__to__salaire
                    .insert(current_EMPLOYEE_PAY_GROUP,
                            CALCULATED_SALARY_for_EMPLOYEE);


                _somme_SALAIRE_CUMULE_A_DATE += CALCULATED_SALARY_for_EMPLOYEE;


                tableWidget_Groupes_Dun_Employe
					->insert_group(current_group_db_ID,
                                   allEmployeeGroups.at(k).trimmed(),
								   current_EMPLOYEE_PAY_GROUP,
								   maximum_de_membres,
								   (0.0 == CALCULATED_SALARY_for_EMPLOYEE));
            }

//              qDebug() << tableWidget_Groupes_Dun_Employe->get_mapListIdxToElement_db_ID();
        }
    }

    if (tableWidget_Groupes_Dun_Employe->rowCount() > 0)
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(_somme_SALAIRE_CUMULE_A_DATE));

        tableWidget_Groupes_Dun_Employe->selectRow(0);
    }
    else
    {
        lineEdit_SALAIRE_CUMULE
            ->setText(GET_CURRENCY_STRING_NUM(0.0));
    }
}


void YerothGROUPES_DUN_EMPLOYE_Window::enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    actionAfficher_ce_groupe_DEMPLOYES_au_detail->setVisible(true);

    pushButton_retirer->setVisible(true);
}


void YerothGROUPES_DUN_EMPLOYE_Window::disable_yeroth_widgets()
{
	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

	YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);

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


void YerothGROUPES_DUN_EMPLOYE_Window::setupLineEdits()
{
    lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE
        ->setText(GET_CURRENCY_STRING_NUM(0.0));

    lineEdit_SALAIRE_CUMULE
        ->setText(GET_CURRENCY_STRING_NUM(0.0));


    lineEdit_SALAIRE_CUMULE->setYerothEnabled(false);

    lineEdit_SALAIRE_DU_GROUPE_SELECTIONNE->setYerothEnabled(false);

    lineEdit_Groupes_Dun_Employe_nombre_de_groupes->setYerothEnabled(false);
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
    _map_GROUPE_SELECTIONNE__to__salaire.clear();

    _somme_SALAIRE_CUMULE_A_DATE = 0.0;

    dateEdit_groupe_dun_employe_date_begin->reset();

    dateEdit_groupe_dun_employe_date_end->reset();


    dateEdit_groupe_dun_employe_date_begin->setYerothEnabled(false);

    dateEdit_groupe_dun_employe_date_end->setYerothEnabled(false);

    tableWidget_Groupes_Dun_Employe->yerothClearTableWidgetContent();


    YerothWindowsCommons::rendreInvisible();
}


void YerothGROUPES_DUN_EMPLOYE_Window::rendreVisible(YerothSqlTableModel *EMPLOYEE_TableModel,
                                                 	 YerothSqlTableModel *stocksTableModel)
{
    static bool FIRST_time_execution = true;

    if (FIRST_time_execution)
    {
        tableWidget_Groupes_Dun_Employe->a_current_window = this;

        FIRST_time_execution = false;
    }

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

    pushButton_VALIDER->enable(this, SLOT(handle_VALIDER_button()));
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

    pushButton_VALIDER->disable(this);
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

    pushButton_VALIDER->disable(this);
    pushButton_groupes_demployes->disable(this);
    pushButton_menu_employes_fournisseurs->disable(this);
    pushButton_RETOUR->disable(this);
    pushButton_retirer->disable(this);
}
