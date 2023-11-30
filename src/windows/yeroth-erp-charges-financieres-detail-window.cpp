/*
 * yeroth-erp-charges-financieres-detail-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-charges-financieres-detail-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include <QtSql/QSqlRecord>


YerothChargesFinancieresDetailsWindow::YerothChargesFinancieresDetailsWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothChargesFinancieresDetailsWindow"))
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("détails d'1 CHARGE FINANCIÈRE"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_DARK_GRAY_60_60_60,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_menu->disable(this);
    pushButton_retour->disable(this);

    /** Menu actions */

    connect(action_CREER_A_PARTIR_DE_CECI,
    		SIGNAL(triggered()),
			this,
			SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));


#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#endif

    setupShortcuts();
}

void YerothChargesFinancieresDetailsWindow::setupLineEdits()
{
    dateEdit_date_de_reception->setYerothEnabled(false);
    dateEdit_date_de_commande->setYerothEnabled(false);

    lineEdit_departement->setYerothEnabled(false);
    lineEdit_reference_produit->setYerothEnabled(false);
    lineEdit_designation->setYerothEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_LIGNE_BUDGETAIRE->setYerothEnabled(false);
    lineEdit_quantite->setYerothEnabled(false);
    lineEdit_prix_dachat->setYerothEnabled(false);
    lineEdit_prix_unitaire->setYerothEnabled(false);

    lineEdit_STATUT_DE_LACHAT->setYerothEnabled(false);
	lineEdit_MONTANT_TVA->setYerothEnabled(false);
	lineEdit_ref_RECU_DACHAT->setYerothEnabled(false);
	lineEdit_LOCALISATION->setYerothEnabled(false);
	lineEdit_ID_commandeur->setYerothEnabled(false);

    textEdit_une_CHARGE_FINANCIERE->setYerothEnabled(false);
}


void YerothChargesFinancieresDetailsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_detail_charges_financieres_CREER->disable(this);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_lister->disable(this);
    pushButton_retour->disable(this);
}


void YerothChargesFinancieresDetailsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_detail_charges_financieres_CREER
		->enable(this, SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_lister->enable(this, SLOT(stocks()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_retour->enable(this, SLOT(charges_financieres()));
}


void YerothChargesFinancieresDetailsWindow::rendreInvisible()
{
	dateEdit_date_de_commande->reset();
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_LIGNE_BUDGETAIRE->clear();
    lineEdit_quantite->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothChargesFinancieresDetailsWindow::rendreVisible(YerothSqlTableModel *stocksTableModel,
                                                          YerothSqlTableModel *achatStocksTableModel)
{
    retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _cur_CHARGES_FINANCIERESTableModel = achatStocksTableModel;

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("détails d'1 CHARGE FINANCIÈRE"),
    								 _cur_CHARGES_FINANCIERESTableModel);

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_db_ID();
    showItem();
}


void YerothChargesFinancieresDetailsWindow::showItem()
{
    _cur_CHARGES_FINANCIERESTableModel
		->yerothSetFilter_WITH_where_clause
			(QString("%1 = '%2'")
				.arg(YerothDatabaseTableColumn::ID,
					 YerothERPWindows::get_last_lister_selected_row_db_ID()));


    QSqlRecord record = _cur_CHARGES_FINANCIERESTableModel->record(0);

    dateEdit_date_de_reception
	->setDate(record.value(YerothDatabaseTableColumn::DATE_DE_RECEPTION).toDate());

    dateEdit_date_de_commande
	->setDate(record.value(YerothDatabaseTableColumn::DATE_DE_COMMANDE).toDate());


    lineEdit_departement
	->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    lineEdit_reference_produit
	->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::REFERENCE));

    lineEdit_designation
	->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_LIGNE_BUDGETAIRE
	->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::CATEGORIE));

    lineEdit_nom_entreprise_fournisseur
	->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));


    double prix_unitaire = GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();


    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));


    double prix_dachat = 0.0;


    YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
    	if (currentUser->isManager() ||
    		currentUser->isGestionaireDesStocks())
    	{
    		prix_dachat =
    				GET_SQL_RECORD_DATA(record,
    						YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();
    	}
    }


    lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM(prix_dachat));


    double quantite_restante =
    		GET_SQL_RECORD_DATA(record,
    				YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();


    lineEdit_quantite->setText(GET_DOUBLE_STRING_P(quantite_restante, 0));


    lineEdit_STATUT_DE_LACHAT->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR));

    lineEdit_MONTANT_TVA->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::MONTANT_TVA));

    lineEdit_ref_RECU_DACHAT->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));

    lineEdit_LOCALISATION->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::LOCALISATION));

    lineEdit_ID_commandeur->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT));


    textEdit_une_CHARGE_FINANCIERE->setText(GET_SQL_RECORD_DATA(record,
    		YerothDatabaseTableColumn::DESCRIPTION_charge_financiere));


    _cur_CHARGES_FINANCIERESTableModel->resetFilter();
}


bool YerothChargesFinancieresDetailsWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");


    QString latexFileNamePrefix("yeroth-erp-BON-DE-COMMANDE");


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        latexFileNamePrefix.clear();
        latexFileNamePrefix.append("yeroth-erp-ORDER-SHEET");
    }


    QString texDocument;

    //TODO
    YerothUtils::getLatex_FINANCIALexpense_Data(texDocument);

    QString data;

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("NOM DE L'employé commandeur: ")));
    data.append(QString("%1\\\\\n").arg
                (_allWindows->getUser()->nom_completTex()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("DATE DE COMMANDE: ")));
    data.append(QString("%1\\\\\n").arg
                (dateEdit_date_de_commande->dateTime().toString("dd.MM.yyyy")));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("DATE DE réception: ")));

    //QDEBUG_STRING_OUTPUT_2_N("dateEdit_date_de_reception >= dateEdit_date_de_commande",
    //                         dateEdit_date_de_reception >= dateEdit_date_de_commande);

    data.append(QString("%1\\\\\n")
                 .arg(dateEdit_date_de_commande->dateTime()
                        .toString("dd.MM.yyyy")));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Département : ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_departement->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Référence: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_reference_produit->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Désignation: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_designation->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("FOURNISSEUR: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_nom_entreprise_fournisseur->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("LIGNE BUDGÉTAIRE: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_LIGNE_BUDGETAIRE->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Quantité: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_quantite->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("PRIX D'ACHAT: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_prix_dachat->text_LATEX()));

    data.
    append(YerothUtils::get_latex_bold_text(QObject::tr("PRIX UNITAIRE: ")));
    data.append(QString("%1\\\\\n").
                arg(lineEdit_prix_unitaire->text_LATEX()));

    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YEROTHDETAILSBONDECOMMANDE", data);


    data.clear();

    data.append(QString("%1")
                 .arg(YerothUtils::get_latex_bold_text
                    (QObject::tr("Description de la charge financière: "))));

    data.append(QString("%1\\\\").arg
                (textEdit_une_CHARGE_FINANCIERE->toPlainTextForLatex()));

    texDocument.replace("YEROTHDESCRIPTIONBONDECOMMANDE", data);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(78);

    YerothInfoEntreprise &infoEntreprise =
                    YerothUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHENTREPRISE",
                        infoEntreprise.getNomCommercial_LATEX());
    texDocument.replace("YEROTHACTIVITESENTREPRISE",
                        infoEntreprise.getSecteursActivitesTex());
    texDocument.replace("YEROTHBOITEPOSTALE",
                        infoEntreprise.getBoitePostal());
    texDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument.replace("YEROTHDATE", fileDate);
    texDocument.replace("YEROTHFOURNISSEUR",
                        lineEdit_nom_entreprise_fournisseur->text_LATEX());
    texDocument.replace("YEROTHDESIGNATIONBONDECOMMANDE",
                        lineEdit_designation->text_LATEX());
    texDocument.replace("YEROTHNOMUTILISATEUR",
                        _allWindows->getUser()->nom_completTex());
    texDocument.replace("YEROTHSUCCURSALE",
                        YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));
    texDocument.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument.replace("YEROTHCOMPTEBANCAIRENR",
                        infoEntreprise.getNumeroCompteBancaire());
    texDocument.replace("YEROTHCONTRIBUABLENR",
                        infoEntreprise.getNumeroDeContribuable());
    texDocument.replace("YEROTHAGENCECOMPTEBANCAIRE",
                        infoEntreprise.getAgenceCompteBancaireTex());


    QString yerothPrefixFileName;

    yerothPrefixFileName
        .append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir(latexFileNamePrefix));

    //qDebug() << "++\n" << texDocument;

    QFile tmpLatexFile(QString("%1tex").arg(yerothPrefixFileName));

    YerothUtils::writeStringToQFilewithUTF8Encoding(tmpLatexFile, texDocument);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);

    QString
    pdfSupplierDataFileName(YerothERPProcess::compileLatex
                            (yerothPrefixFileName));

    if (pdfSupplierDataFileName.isEmpty())
    {
        return false;
    }

    YerothERPProcess::startPdfViewerProcess(pdfSupplierDataFileName);

    emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

    return true;
}


void YerothChargesFinancieresDetailsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_CHARGE_FINANCIERE_detailWindow->palette());
    menu.addAction(action_CREER_A_PARTIR_DE_CECI);
    menu.exec(event->globalPos());
}


void YerothChargesFinancieresDetailsWindow::hideEvent(QHideEvent *hideEvent)
{
    YerothWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
}


void YerothChargesFinancieresDetailsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}
