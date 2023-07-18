/*
 * yeroth-erp-fournisseur-details-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-fournisseur-details-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerothFournisseurDetailsWindow::YerothFournisseurDetailsWindow():YerothWindowsCommons(),
    _logger(new
            YerothLogger("YerothFournisseurDetailsWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("détails d'un fournisseur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    textEdit_fournisseur_details_description_du_fournisseur->setYerothEnabled
    (false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));
    connect(actionFournisseurs, SIGNAL(triggered()), this,
            SLOT(fournisseurs()));
    connect(actionModifierFournisseur, SIGNAL(triggered()), this,
            SLOT(modifierFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this,
            SLOT(supprimerFournisseur()));
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


void YerothFournisseurDetailsWindow::modifierFournisseur()
{
    rendreInvisible();

    bool show_TabDESCRIPTION_SUPPLIER =
                    (tabWidget_details_fournisseur->currentIndex() == 1);

    _allWindows->_modifierFournisseurWindow
		->rendreVisible(_curFournisseurTableModel,
						_curStocksTableModel,
						show_TabDESCRIPTION_SUPPLIER,
						_SHOW_EMPLOYEE);
}


void YerothFournisseurDetailsWindow::supprimerFournisseur()
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du compte fournisseur '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression"), msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _fournisseursWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;
        fournisseurs();
        if (resRemoved)
        {
            msgSupprimer =
                            QObject::tr
                            ("Le compte fournisseur '%1' a été supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le compte fournisseur '%1' ne pouvait pas être supprimé !").
                            arg(GET_SQL_RECORD_DATA
                                (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothFournisseurDetailsWindow::setupLineEdits()
{
    lineEdit_fournisseur_details_reference_registre_du_commerce->
    setYerothEnabled(false);
    lineEdit_fournisseur_details_reference_fournisseur->
    setYerothEnabled(false);
    lineEdit_fournisseur_details_nom_entreprise->setYerothEnabled(false);
    lineEdit_fournisseur_details_nom_representant->setYerothEnabled(false);
    lineEdit_fournisseur_details_quartier->setYerothEnabled(false);
    lineEdit_fournisseur_details_ville->setYerothEnabled(false);
    lineEdit_fournisseur_details_province_etat->setYerothEnabled(false);
    lineEdit_fournisseur_details_pays->setYerothEnabled(false);
    lineEdit_fournisseur_details_boite_postale->setYerothEnabled(false);
    lineEdit_fournisseur_details_siege_social->setYerothEnabled(false);
    lineEdit_fournisseur_details_email->setYerothEnabled(false);
    lineEdit_fournisseur_details_numero_telephone_1->setYerothEnabled(false);
    lineEdit_fournisseur_details_numero_telephone_2->setYerothEnabled(false);
    lineEdit_fournisseur_details_numero_contribuable->setYerothEnabled(false);
    lineEdit_fournisseur_details_fournisseur->setYerothEnabled(false);
    lineEdit_employe_qualite->setYerothEnabled(false);
    lineEdit_employe_SUPERIEUR_hierarchique->setYerothEnabled(false);
	lineEdit_employe_DEPARTEMENT->setYerothEnabled(false);
	lineEdit_EMPLOYE_SALAIRE_annuel->setYerothEnabled(false);
	lineEdit_EMPLOYE_SALAIRE_MENSUEL->setYerothEnabled(false);
	lineEdit_EMPLOYE_position_fonction->setYerothEnabled(false);
}


void YerothFournisseurDetailsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}


void YerothFournisseurDetailsWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_modifier->enable(this, SLOT(modifierFournisseur()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));


#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif
}


bool YerothFournisseurDetailsWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");

    QString yerothSupplierAccountImage("yeroth");

    QString
    yerothCustomerAccountImageTmpFile(QString("%1JPG").arg
                                      (YerothUtils::
                                       getUniquePrefixFileInTemporaryFilesDir
                                       (yerothSupplierAccountImage)));

    const QPixmap *label_image_produit_pixmap = label_image_produit->pixmap();

    if (0 != label_image_produit_pixmap)
    {
        YerothUtils::savePixmapToFile(yerothCustomerAccountImageTmpFile,
                                      *label_image_produit_pixmap, "JPG");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    QString latexFileNamePrefix("yeroth-erp-liste-fournisseur");

#ifdef YEROTH_ENGLISH_LANGUAGE
    latexFileNamePrefix.clear();
    latexFileNamePrefix.append("yeroth-erp-supplier-file");
#endif

    QString texDocument;

    YerothUtils::getLatexSupplierData(texDocument);

    QString data;

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Référence fournisseur: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_reference_fournisseur->text_LATEX
                 ()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Nom de l'entreprise: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_nom_entreprise->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Nom du Représentant: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_nom_representant->
                 text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Quartier: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_quartier->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Ville: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_ville->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Province / État: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_province_etat->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text(QObject::tr("Pays: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_pays->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Boîte postale: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_boite_postale->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Siège social: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_siege_social->text_LATEX()));

    data.
    append(YerothUtils::get_latex_bold_text(QObject::tr("Émail: ")));
    data.append(QString("%1\\\\\n").
                arg(lineEdit_fournisseur_details_email->text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 1: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_telephone_1->
                 text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Numéro de téléphone 2: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_telephone_2->
                 text_LATEX()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("Numéro de contribuable: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_numero_contribuable->text_LATEX
                 ()));

    data.append(YerothUtils::get_latex_bold_text
                (QObject::tr("RCCM N\\textsuperscript{o}: ")));
    data.append(QString("%1\\\\\n").arg
                (lineEdit_fournisseur_details_reference_registre_du_commerce->
                 text_LATEX()));

    data.
    append(YerothUtils::get_latex_bold_text(QObject::tr("Fournisseur: ")));
    data.append(QString("%1\\\\\n").
                arg(lineEdit_fournisseur_details_fournisseur->text_LATEX()));

    data.append("\n\n\\vspace{0.3cm}\n\n");



    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
    	    data.append(
                QString("%1")
                    .arg(YerothUtils::get_latex_bold_text
                            ("Employee Description:")));
    	}
    	else
    	{
    	    data.append(
                QString("%1")
                    .arg(YerothUtils::get_latex_bold_text
                            ("Supplier Description:")));
    	}
    }
    else
    {
    	if (_SHOW_EMPLOYEE)
    	{
    	    data.append(
                QString("%1")
                    .arg(YerothUtils::get_latex_bold_text
                            ("Description de l'employé:")));
    	}
    	else
    	{
    	    data.append(
                QString("%1")
                    .arg(YerothUtils::get_latex_bold_text
                            ("Description du fournisseur:")));
    	}
    }



    data.append("\n\n\\vspace{0.3cm}\n\n");

    texDocument.replace("YEROTHDETAILSFOURNISSEUR", data);

    data.clear();
    data.append(QString("%1\\\\").arg
                (textEdit_fournisseur_details_description_du_fournisseur->
                 toPlainTextForLatex()));

    texDocument.replace("YEROTHDESCRIPTIONFOURNISSEUR", data);

    if (0 != label_image_produit_pixmap)
    {
        texDocument.replace("YEROTHCHEMINCOMPLETIMAGEFOURNISSEUR",
                            yerothCustomerAccountImageTmpFile);
    }
    else
    {
        texDocument.replace("YEROTHCHEMINCOMPLETIMAGEFOURNISSEUR", "");
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(24);

    YerothInfoEntreprise &infoEntreprise =
                    YerothUtils::getAllWindows()->getInfoEntreprise();

    QString
    fileDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents
             (infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(fileDate);

    texDocument.replace("YEROTHPAPERSPEC", "a4paper");

    texDocument.replace("YEROTHFOURNISSEUR",
                        QString("%1").arg
                        (lineEdit_fournisseur_details_nom_entreprise->
                         text_LATEX()));
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


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	if (_SHOW_EMPLOYEE)
    	{
    	    texDocument.replace("YEROTHOBJECT", "Employee");
    	}
    	else
    	{
            texDocument.replace("YEROTHOBJECT", "Supplier");
    	}
    }
    else
    {
        if (_SHOW_EMPLOYEE)
    	{
    	    texDocument.replace("YEROTHOBJECT", "de l'EMPLOYÉ");
    	}
    	else
    	{
            texDocument.replace("YEROTHOBJECT", "de fournisseur");
    	}
    }


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


void YerothFournisseurDetailsWindow::rendreInvisible()
{
    lineEdit_fournisseur_details_reference_registre_du_commerce->clear();
    lineEdit_fournisseur_details_reference_fournisseur->clear();
    lineEdit_fournisseur_details_nom_entreprise->clear();
    lineEdit_fournisseur_details_nom_representant->clear();
    lineEdit_fournisseur_details_quartier->clear();
    lineEdit_fournisseur_details_ville->clear();
    lineEdit_fournisseur_details_province_etat->clear();
    lineEdit_fournisseur_details_pays->clear();
    lineEdit_fournisseur_details_boite_postale->clear();
    lineEdit_fournisseur_details_siege_social->clear();
    lineEdit_fournisseur_details_email->clear();
    lineEdit_fournisseur_details_numero_telephone_1->clear();
    lineEdit_fournisseur_details_numero_telephone_2->clear();
    lineEdit_fournisseur_details_numero_contribuable->clear();
    lineEdit_fournisseur_details_fournisseur->clear();

    lineEdit_employe_qualite->clear();
	lineEdit_employe_SUPERIEUR_hierarchique->clear();
	lineEdit_employe_DEPARTEMENT->clear();
	lineEdit_EMPLOYE_SALAIRE_annuel->clear();
	lineEdit_EMPLOYE_SALAIRE_MENSUEL->clear();
	lineEdit_EMPLOYE_position_fonction->clear();

    textEdit_fournisseur_details_description_du_fournisseur->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    YerothWindowsCommons::rendreInvisible();
}


void YerothFournisseurDetailsWindow::rendreVisible(YerothSqlTableModel *fournisseurTableModel,
                                                   YerothSqlTableModel *stocksTableModel,
												   bool 			   employe /* = false */)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curFournisseurTableModel = fournisseurTableModel;

    _SHOW_EMPLOYEE = employe;
    //qDebug() << "++ last selected row: " << _allWindows->getLastSelectedListerRow();

    if (true == _SHOW_EMPLOYEE)
    {
    	label_employe_SUPERIEUR_hierarchique->setVisible(true);
    	label_employe_DEPARTEMENT->setVisible(true);
		label_EMPLOYE_SALAIRE_annuel->setVisible(true);
		label_EMPLOYE_SALAIRE_MENSUEL->setVisible(true);
		label_EMPLOYE_position_fonction->setVisible(true);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(true);
		lineEdit_employe_DEPARTEMENT->setVisible(true);
		lineEdit_EMPLOYE_SALAIRE_annuel->setVisible(true);
		lineEdit_EMPLOYE_SALAIRE_MENSUEL->setVisible(true);
		lineEdit_EMPLOYE_position_fonction->setVisible(true);
    }
    else
    {
    	label_employe_SUPERIEUR_hierarchique->setVisible(false);
    	label_employe_DEPARTEMENT->setVisible(false);
		label_EMPLOYE_SALAIRE_annuel->setVisible(false);
		label_EMPLOYE_SALAIRE_MENSUEL->setVisible(false);
		label_EMPLOYE_position_fonction->setVisible(false);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(false);
		lineEdit_employe_DEPARTEMENT->setVisible(false);
		lineEdit_EMPLOYE_SALAIRE_annuel->setVisible(false);
		lineEdit_EMPLOYE_SALAIRE_MENSUEL->setVisible(false);
		lineEdit_EMPLOYE_position_fonction->setVisible(false);
    }

    setVisible(true);

    showFournisseurDetail(_SHOW_EMPLOYEE);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("détails d'un fournisseur"),
    								 _curFournisseurTableModel);
}


void YerothFournisseurDetailsWindow::showFournisseurDetail(bool employe /* = false */)
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    lineEdit_fournisseur_details_reference_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR));

    lineEdit_fournisseur_details_reference_registre_du_commerce->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

    lineEdit_fournisseur_details_nom_entreprise->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerothDatabaseTableColumn::NOM_ENTREPRISE));

    lineEdit_fournisseur_details_nom_representant->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            YerothDatabaseTableColumn::NOM_REPRESENTANT));

    lineEdit_fournisseur_details_quartier->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerothDatabaseTableColumn::QUARTIER));

    lineEdit_fournisseur_details_ville->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::VILLE));

    lineEdit_fournisseur_details_province_etat->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::PROVINCE_ETAT));

    lineEdit_fournisseur_details_pays->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::PAYS));

    lineEdit_fournisseur_details_boite_postale->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::BOITE_POSTALE));

    lineEdit_fournisseur_details_siege_social->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerothDatabaseTableColumn::SIEGE_SOCIAL));

    lineEdit_fournisseur_details_email->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::EMAIL));

    if (true == employe)
    {
    	lineEdit_employe_qualite
			->setText(QObject::tr("Employé"));

    	lineEdit_employe_SUPERIEUR_hierarchique
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MANAGER));

    	lineEdit_employe_DEPARTEMENT
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DEPARTEMENT));

    	lineEdit_EMPLOYE_SALAIRE_annuel
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SALAIRE_ANNUEL));

    	lineEdit_EMPLOYE_SALAIRE_MENSUEL
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::SALAIRE_MENSUEL));

    	lineEdit_EMPLOYE_position_fonction
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POSITION_FONCTION));
    }
    else
    {
    	lineEdit_employe_qualite
			->setText(QObject::tr("FOURNISSEUR"));

    	lineEdit_employe_SUPERIEUR_hierarchique->clear();
    	lineEdit_employe_DEPARTEMENT->clear();
    	lineEdit_EMPLOYE_SALAIRE_annuel->clear();
    	lineEdit_EMPLOYE_SALAIRE_MENSUEL->clear();
    	lineEdit_EMPLOYE_position_fonction->clear();
    }

    lineEdit_fournisseur_details_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

    lineEdit_fournisseur_details_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    lineEdit_fournisseur_details_numero_contribuable->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));


    double fournisseur = GET_SQL_RECORD_DATA(record,
                                             YerothDatabaseTableColumn::COMPTE_FOURNISSEUR).
                         toDouble();

    lineEdit_fournisseur_details_fournisseur->setText(GET_CURRENCY_STRING_NUM
                                                      (fournisseur));

    textEdit_fournisseur_details_description_du_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}


void YerothFournisseurDetailsWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
}
