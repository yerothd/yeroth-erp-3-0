/*
 * yeroth-erp-admin-detail-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-admin-detail-window.hpp"

#include "src/yeroth-erp-windows.hpp"


#include "src/process/yeroth-erp-process.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

#include <QtSql/QSqlRecord>


YerothAdminDetailWindow::YerothAdminDetailWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ détail")),
 _logger(new YerothLogger("YerothAdminDetailWindow"))
{
    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
						.arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
							 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setupLineEdits();

    dateEdit_detail_alerte_date_debut->setYerothEnabled(false);

    dateEdit_detail_alerte_date_fin->setYerothEnabled(false);

    dateEdit_date_de_reception->setYerothEnabled(false);

    dateEdit_date_de_commande->setYerothEnabled(false);

    dateEdit_detail_utilisateur_date_naissance->setEnabled(false);

    dateEdit_detail_localisation_date_ouverture->setEnabled(false);

    textEdit_detail_departements_de_produits_description->setYerothEnabled(false);
    textEdit_detail_categorie_description->setYerothEnabled(false);
    textEdit_une_CHARGE_FINANCIERE->setYerothEnabled(false);
    textEdit_detail_dune_LIGNE_budgetaire->setYerothEnabled(false);
    textEdit_detail_compte_bancaire_description_du_compte->setYerothEnabled(false);
    textEdit_detail_localisation_description_lieu->setYerothEnabled(false);
    textEdit_detail_alerte_message->setYerothEnabled(false);

    radioButton_detail_alerte_date_periode_temps->setEnabled(false);
    radioButton_detail_alerte_quantite->setEnabled(false);


    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);


    pushButton_detail_charges_financieres_CREER->disable(this);

    pushButton_creer->enable(this, SLOT(creer()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_modifier->enable(this, SLOT(modifier()));
    pushButton_supprimer->enable(this, SLOT(supprimer()));

    connect(actionCreer, SIGNAL(triggered()), this, SLOT(creer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(modifier()));
    connect(actionSupprimer, SIGNAL(triggered()), this, SLOT(supprimer()));

    pushButton_detail_utilisateur_retour->enable(this,
                                                 SLOT
                                                 (retourListerCompteUtilisateur
                                                  ()));
    pushButton_detail_localisation_retour->enable(this,
                                                  SLOT
                                                  (retourListerLocalisation
                                                   ()));
    pushButton_detail_departements_de_produits_retour->enable(this,
                                                              SLOT
                                                              (retourListerDepartementsDeProduits
                                                               ()));
    pushButton_detail_categorie_retour->enable(this,
                                               SLOT(retourListerCategorie()));

    pushButton_detail_LIGNE_budgetaire_retour->enable(this,
                                               	      SLOT(retourLister_LIGNE_BUDGETAIRE()));

    pushButton_detail_compte_bancaire_retour->enable(this,
                                                     SLOT
                                                     (retourListerCompteBancaire
                                                      ()));
    pushButton_detail_remise_retour->enable(this,
                                            SLOT(retourListerRemise()));

    pushButton_detail_charges_financieres_retour->enable(this,
    													 SLOT(retourListerChargeFinanciere()));

    pushButton_detail_alerte_retour->enable(this,
                                            SLOT(retourListerAlerte()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));

    connect(tabWidget_detail,
    		SIGNAL(currentChanged(int)),
			this,
            SLOT(handleCurrentChanged(int)));

    connect(actionAfficherPDF,
            SIGNAL(triggered()),
            this,
            SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));

    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
}


void YerothAdminDetailWindow::setupLineEdits()
{
	//FINANCIAL EXPENSE WIDGET SETUP
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


    lineEdit_detail_utilisateur_prenom->setYerothEnabled(false);
    lineEdit_detail_utilisateur_nom->setYerothEnabled(false);
    lineEdit_detail_utilisateur_lieu_naissance->setYerothEnabled(false);
    lineEdit_detail_utilisateur_email->setYerothEnabled(false);
    lineEdit_detail_utilisateur_ville->setYerothEnabled(false);
    lineEdit_detail_utilisateur_province_etat->setYerothEnabled(false);
    lineEdit_detail_utilisateur_pays->setYerothEnabled(false);
    lineEdit_detail_utilisateur_boite_postale->setYerothEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_1->setYerothEnabled(false);
    lineEdit_detail_utilisateur_numero_telephone_2->setYerothEnabled(false);
    lineEdit_detail_utilisateur_id->setYerothEnabled(false);
    lineEdit_detail_utilisateur_mot_passe->setYerothEnabled(false);


    lineEdit_detail_departements_de_produits_nom->setYerothEnabled(false);

    lineEdit_detail_categorie_nom_departement_produit->setYerothEnabled(false);

    lineEdit_detail_categorie_nom->setYerothEnabled(false);

    lineEdit_detail_INTITULE_de_la_LIGNE_BUDGETAIRE->setYerothEnabled(false);

    lineEdit_detail_montant_de_la_LIGNE_BUDGETAIRE->setYerothEnabled(false);

    lineEdit_detail_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->setYerothEnabled(false);

    lineEdit_detail_INTITULE_DU_COMPTE_BANCAIRE->setYerothEnabled(false);


    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire
		->setYerothEnabled(false);

    lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire
		->setYerothEnabled(false);

    lineEdit_detail_compte_bancaire_institut_bancaire
		->setYerothEnabled(false);


    lineEdit_detail_localisation_adresse_ip->setYerothEnabled(false);
    lineEdit_detail_localisation_nom->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_unique->setYerothEnabled(false);
    lineEdit_detail_localisation_quartier->setYerothEnabled(false);
    lineEdit_detail_localisation_ville->setYerothEnabled(false);
    lineEdit_detail_localisation_province_etat->setYerothEnabled(false);
    lineEdit_detail_localisation_pays->setYerothEnabled(false);
    lineEdit_detail_localisation_boite_postale->setYerothEnabled(false);
    lineEdit_detail_localisation_email->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_telephone_1->setYerothEnabled(false);
    lineEdit_detail_localisation_numero_telephone_2->setYerothEnabled(false);

    lineEdit_detail_alerte_designation_article->setYerothEnabled(false);
    lineEdit_detail_alerte_quantite->setYerothEnabled(false);
}


void YerothAdminDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
}


void YerothAdminDetailWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, false);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);

    pushButton_detail_charges_financieres_CREER->disable(this);
}


void YerothAdminDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

    pushButton_detail_charges_financieres_CREER
		->enable(this, SLOT(SLOT_CREER_A_PARTIR_DE_CECI()));
}


void YerothAdminDetailWindow::creer()
{
    _allWindows->_adminCreateWindow
        ->rendreVisible(_curStocksTableModel,
                        tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerothAdminDetailWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerothAdminDetailWindow::modifier()
{
    _logger->log("modifier");
    switch (tabWidget_detail->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
        break;

    case SUJET_ACTION_LOCALISATION:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
        break;

    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
        break;

    case SUJET_ACTION_CATEGORIE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
        break;

    case SUJET_ACTION_ligne_budgetaire:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
        break;

    case SUJET_ACTION_COMPTE_BANCAIRE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
        break;

    case SUJET_ACTION_REMISE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_REMISE);
        break;

    case SUJET_ACTION_ALERTE:
        _allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_ALERTE);
        break;

    case SUJET_ACTION_CHARGE_FINANCIERE:
    	_allWindows->_adminModifierWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
        break;

    default:
        break;
    }

    rendreInvisible();
}


void YerothAdminDetailWindow::supprimer()
{
    _logger->log("supprimer");
    _allWindows->_adminListerWindow->supprimer();
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_detail->currentIndex());
    rendreInvisible();
}


void YerothAdminDetailWindow::rendreInvisible()
{
    _logger->log("rendreInvisible");
    lineEdit_detail_alerte_id_destinataire->clear();
    lineEdit_detail_alerte_nom_destinataire->clear();
    lineEdit_detail_alerte_designation_article->clear();
    clear_set_edit_comboBoxes();
    setVisible(false);
}


bool YerothAdminDetailWindow::imprimer_pdf_document()
{
    _logger->log("imprimer_pdf_document");


    QString latexFileNamePrefix("yeroth-erp-ADMINISTRATION-BON-DE-COMMANDE");


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        latexFileNamePrefix.clear();
        latexFileNamePrefix.append("yeroth-erp-ADMINISTRATION-ORDER-SHEET");
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


void YerothAdminDetailWindow::handleCurrentChanged(int index)
{
	_allWindows->_adminWindow->_curAdminSujetAction =
			(enum AdminSujetAction) tabWidget_detail->currentIndex();

    switch (tabWidget_detail->currentIndex())
    {
    case SUJET_ACTION_CHARGE_FINANCIERE:

    	disconnect(actionAfficherPDF, 0, 0, 0);

        connect(actionAfficherPDF,
                SIGNAL(triggered()),
                this,
                SLOT(imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR()));

        break;

    default:
        YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
        disconnect(actionAfficherPDF, 0, 0, 0);

        break;
    }
}


void YerothAdminDetailWindow::enableOtherTabs(enum AdminSujetAction curAction,
                                              bool enabled)
{
    tabWidget_detail->setTabEnabled(curAction, !enabled);
    for (int k = 0; k <= 7; ++k)
    {
        if (k != curAction)
        {
            tabWidget_detail->setTabEnabled(k, enabled);
        }
    }
}


void YerothAdminDetailWindow::rendreVisibleCompteUtilisateur(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ utilisateurs"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_UTILISATEUR);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *usersSqlTableModel = lw->getCurSearchSqlTableModel();

    if (!usersSqlTableModel)
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }
    else if (usersSqlTableModel &&
             !YerothUtils::isEqualCaseInsensitive(usersSqlTableModel->
                                                  sqlTableName(),
                                                  YerothDatabase::USERS))
    {
        usersSqlTableModel = &_allWindows->getSqlTableModel_users();
    }

    //YerothSqlTableModel &usersSqlTableRecord = _allWindows->getSqlTableModel_users();

    QSqlRecord record = usersSqlTableModel->record(sqlTableRow);

    lineEdit_detail_utilisateur_prenom
		->setText(GET_SQL_RECORD_DATA(record,
				  YerothDatabaseTableColumn::PRENOM));

    lineEdit_detail_utilisateur_nom
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::NOM));

    comboBox_detail_utilisateur_titre
		->addItem(YerothUtils::_titreToUserViewString
					.value(GET_SQL_RECORD_DATA(record,
											   YerothDatabaseTableColumn::TITRE).toInt()));

    lineEdit_detail_utilisateur_lieu_naissance
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::LIEU_NAISSANCE));

    dateEdit_detail_utilisateur_date_naissance
		->setDate(GET_DATE_FROM_STRING
					(GET_SQL_RECORD_DATA(record,
										 YerothDatabaseTableColumn::DATE_NAISSANCE)));

    lineEdit_detail_utilisateur_email->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_utilisateur_pays->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_utilisateur_ville->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_utilisateur_province_etat->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerothDatabaseTableColumn::PROVINCE_ETAT));
    lineEdit_detail_utilisateur_boite_postale->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerothDatabaseTableColumn::BOITE_POSTALE));
    lineEdit_detail_utilisateur_numero_telephone_1->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_utilisateur_numero_telephone_2->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    comboBox_detail_utilisateur_role->
    addItem(YerothUtils::_roleToUserViewString.
            value(GET_SQL_RECORD_DATA
                  (record, YerothDatabaseTableColumn::ROLE).toInt()));

    lineEdit_detail_utilisateur_id->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerothDatabaseTableColumn::NOM_UTILISATEUR));

    lineEdit_detail_utilisateur_mot_passe->setText("****");

    lineEdit_detail_utilisateur_localisation->setYerothEnabled(false);

    lineEdit_detail_utilisateur_localisation->
    setText(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

    enableOtherTabs(SUJET_ACTION_COMPTE_UTILISATEUR, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleLocalisation(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ localisations"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_LOCALISATION);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *localisationSqlTableModel =
                    lw->getCurSearchSqlTableModel();

    if (!localisationSqlTableModel)
    {
        localisationSqlTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }
    else if (localisationSqlTableModel
             &&
             !YerothUtils::
             isEqualCaseInsensitive(localisationSqlTableModel->sqlTableName(),
                                    YerothDatabase::LOCALISATIONS))
    {
        localisationSqlTableModel =
                        &_allWindows->getSqlTableModel_localisations();
    }

    //YerothSqlTableModel &localisationSqlTableModel = _allWindows->getSqlTableModel_localisations();
    QSqlRecord record = localisationSqlTableModel->record(sqlTableRow);

    lineEdit_detail_localisation_adresse_ip->setText(GET_SQL_RECORD_DATA
                                                     (record, "adresse_ip"));
    lineEdit_detail_localisation_nom->setText(GET_SQL_RECORD_DATA
                                              (record, "nom_localisation"));
    lineEdit_detail_localisation_numero_unique->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         "numero_unique"));
    lineEdit_detail_localisation_quartier->setText(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerothDatabaseTableColumn::QUARTIER));
    lineEdit_detail_localisation_ville->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::VILLE));
    lineEdit_detail_localisation_province_etat->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         "province_etat"));
    lineEdit_detail_localisation_pays->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::PAYS));
    lineEdit_detail_localisation_boite_postale->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::BOITE_POSTALE));

    dateEdit_detail_localisation_date_ouverture->setDate(QDate::fromString
                                                         (GET_SQL_RECORD_DATA
                                                          (record,
                                                           "date_ouverture"),
                                                          YerothUtils::
                                                          DATE_FORMAT));

    lineEdit_detail_localisation_email->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::EMAIL));
    lineEdit_detail_localisation_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));
    lineEdit_detail_localisation_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
    lineEdit_detail_localisation_base_donnees->setText(GET_SQL_RECORD_DATA
                                                       (record, "dbms"));
    textEdit_detail_localisation_description_lieu->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            "description_lieu"));

    enableOtherTabs(SUJET_ACTION_LOCALISATION, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleDepartementsDeProduits(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ départements"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *departementsDeProduitsTableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == departementsDeProduitsTableModel)
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }
    else if (0 != departementsDeProduitsTableModel
             &&
             !YerothUtils::isEqualCaseInsensitive
             (departementsDeProduitsTableModel->sqlTableName(),
              YerothDatabase::DEPARTEMENTS_PRODUITS))
    {
        departementsDeProduitsTableModel =
                        &_allWindows->getSqlTableModel_departements_produits();
    }

    QSqlRecord record = departementsDeProduitsTableModel->record(sqlTableRow);

    lineEdit_detail_departements_de_produits_nom->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerothDatabaseTableColumn::
                                                           NOM_DEPARTEMENT_PRODUIT));

    textEdit_detail_departements_de_produits_description->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT));

    enableOtherTabs(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleCategorie(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ catégories"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_CATEGORIE);


    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;


    YerothSqlTableModel *categoriesTableModel = lw->getCurSearchSqlTableModel();


    if (!categoriesTableModel)
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }
    else if (categoriesTableModel &&
    		 !YerothUtils::isEqualCaseInsensitive(categoriesTableModel->sqlTableName(),
                                                  YerothDatabase::CATEGORIES))
    {
        categoriesTableModel = &_allWindows->getSqlTableModel_categories();
    }


    QSqlRecord record = categoriesTableModel->record(sqlTableRow);

    lineEdit_detail_categorie_nom_departement_produit
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    lineEdit_detail_categorie_nom
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::NOM_CATEGORIE));

    textEdit_detail_categorie_description
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE));

    enableOtherTabs(SUJET_ACTION_CATEGORIE, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleLigneBudgetaire(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ lignes budgétaires"));

    setWindowTitle(_windowName);

	tabWidget_detail->setCurrentIndex(SUJET_ACTION_ligne_budgetaire);

	YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

	YerothSqlTableModel *ligneBudgetaireTableModel =
			lw->getCurSearchSqlTableModel();

	if (!ligneBudgetaireTableModel)
	{
		ligneBudgetaireTableModel =
				&_allWindows->getSqlTableModel_lignes_budgetaires();
	}
	else if (ligneBudgetaireTableModel				&&
			!YerothUtils::isEqualCaseInsensitive(ligneBudgetaireTableModel->sqlTableName(),
												 YerothDatabase::LIGNES_BUDGETAIRES))
	{
		ligneBudgetaireTableModel =
				&_allWindows->getSqlTableModel_lignes_budgetaires();
	}

	QSqlRecord record = ligneBudgetaireTableModel->record(sqlTableRow);

	lineEdit_detail_INTITULE_de_la_LIGNE_BUDGETAIRE->setText
	(GET_SQL_RECORD_DATA
			(record, YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire));

	lineEdit_detail_montant_de_la_LIGNE_BUDGETAIRE
		->setText(GET_DOUBLE_STRING(
				GET_SQL_RECORD_DATA(record,
									YerothDatabaseTableColumn::montant_de_la_ligne_budgetaire).toDouble()));

	lineEdit_detail_montant_RESTANT_de_la_LIGNE_BUDGETAIRE
		->setText(GET_DOUBLE_STRING(
				GET_SQL_RECORD_DATA(record,
									YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire).toDouble()));

	lineEdit_detail_INTITULE_DU_COMPTE_BANCAIRE->setText
	(GET_SQL_RECORD_DATA
			(record, YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

	textEdit_detail_dune_LIGNE_budgetaire->setText
	(GET_SQL_RECORD_DATA
			(record, YerothDatabaseTableColumn::DESCRIPTION_ligne_budgetaire));

	enableOtherTabs(SUJET_ACTION_ligne_budgetaire, false);

	setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleCompteBancaire(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ comptes bancaires"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_COMPTE_BANCAIRE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *comptesBancairesTableModel =
                    lw->getCurSearchSqlTableModel();

    if (!comptesBancairesTableModel)
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (comptesBancairesTableModel
             &&
             !YerothUtils::isEqualCaseInsensitive
             (comptesBancairesTableModel->sqlTableName(),
              YerothDatabase::COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(sqlTableRow);

    lineEdit_detail_compte_bancaire_reference_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_intitule_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_detail_compte_bancaire_institut_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_detail_compte_bancaire_description_du_compte->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));

    enableOtherTabs(SUJET_ACTION_COMPTE_BANCAIRE, false);

    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleRemise(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ remises"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_REMISE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *remiseTableModel = lw->getCurSearchSqlTableModel();

    if (!remiseTableModel)
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remiseTableModel &&
             !YerothUtils::isEqualCaseInsensitive(remiseTableModel->
                                                  sqlTableName(),
                                                  YerothDatabase::REMISES))
    {
        remiseTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record = remiseTableModel->record(sqlTableRow);

    lineEdit_detail_remise_nom->setYerothEnabled(false);
    lineEdit_detail_remise_designation_article->setYerothEnabled(false);
    lineEdit_detail_remise_montant->setYerothEnabled(false);

    spinBox_detail_remise_pourcentage->setEnabled(false);

    dateEdit_detail_remise_date_debut->setYerothEnabled(false);
    dateEdit_detail_remise_date_fin->setYerothEnabled(false);

    textEdit_detail_remise_message->setYerothEnabled(false);

    lineEdit_detail_remise_nom->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::DESIGNATION_REMISE));
    lineEdit_detail_remise_designation_article->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::DESIGNATION));

    spinBox_detail_remise_pourcentage->setValue(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::REMISE_POURCENTAGE).toDouble
                                                ());

    QString
    date_debut(GET_SQL_RECORD_DATA
               (record, YerothDatabaseTableColumn::DATE_DEBUT));
    dateEdit_detail_remise_date_debut->setDate(GET_DATE_FROM_STRING
                                               (date_debut));

    QString
    date_fin(GET_SQL_RECORD_DATA
             (record, YerothDatabaseTableColumn::DATE_FIN));
    dateEdit_detail_remise_date_fin->setDate(GET_DATE_FROM_STRING(date_fin));

    textEdit_detail_remise_message->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerothDatabaseTableColumn::REMISE_NOTES));

    enableOtherTabs(SUJET_ACTION_REMISE, false);
    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisible_CHARGE_FINANCIERE(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ CHARGE FINANCIÈRE"));

    setWindowTitle(_windowName);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_CHARGE_FINANCIERE);


    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *CHARGES_FINANCIERES_TableModel = lw->getCurSearchSqlTableModel();

    if (0 == CHARGES_FINANCIERES_TableModel)
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }
    else if ((0 != CHARGES_FINANCIERES_TableModel) &&
             !YerothUtils::isEqualCaseInsensitive(CHARGES_FINANCIERES_TableModel->sqlTableName(),
                                                  YerothDatabase::CHARGES_FINANCIERES))
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }


    QSqlRecord record = CHARGES_FINANCIERES_TableModel->record(sqlTableRow);

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
        if (currentUser->isManager())
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



    enableOtherTabs(SUJET_ACTION_CHARGE_FINANCIERE, false);


    setVisible(true);
}


void YerothAdminDetailWindow::rendreVisibleAlerte(int sqlTableRow)
{
	retranslateUi(this);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("administration ~ détail ~ alertes"));

    setWindowTitle(_windowName);

    tabWidget_detail->setCurrentIndex(SUJET_ACTION_ALERTE);

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *alertesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == alertesTableModel)
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }
    else if ((0 != alertesTableModel) &&
             !YerothUtils::isEqualCaseInsensitive(alertesTableModel->
                                                  sqlTableName(),
                                                  YerothDatabase::ALERTES))
    {
        alertesTableModel = &_allWindows->getSqlTableModel_alertes();
    }

    QSqlRecord record = alertesTableModel->record(sqlTableRow);

    lineEdit_detail_alerte_designation_alerte->setYerothEnabled(false);
    lineEdit_detail_alerte_id_destinataire->setYerothEnabled(false);
    lineEdit_detail_alerte_nom_destinataire->setYerothEnabled(false);
    lineEdit_detail_alerte_designation_article->setYerothEnabled(false);

    lineEdit_detail_alerte_designation_alerte->setText(GET_SQL_RECORD_DATA
                                                       (record,
                                                        YerothDatabaseTableColumn::DESIGNATION_ALERTE));

    lineEdit_detail_alerte_id_destinataire->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerothDatabaseTableColumn::DESTINATAIRE));

    lineEdit_detail_alerte_nom_destinataire->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE));

    lineEdit_detail_alerte_designation_article->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::DESIGNATION));

    comboBox_detail_alerte_condition->addItem(GET_SQL_RECORD_DATA
                                              (record,
                                               YerothDatabaseTableColumn::CONDITION_ALERTE));

    double quantite = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::
                                          QUANTITE).toDouble();

    if (quantite > -1)
    {
        radioButton_detail_alerte_quantite->setChecked(true);

        lineEdit_detail_alerte_quantite->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerothDatabaseTableColumn::QUANTITE));
    }
    else
    {
        radioButton_detail_alerte_date_periode_temps->setChecked(true);

        QString
        date_debut(GET_SQL_RECORD_DATA
                   (record, YerothDatabaseTableColumn::DATE_DEBUT));
        dateEdit_detail_alerte_date_debut->setDate(GET_DATE_FROM_STRING
                                                   (date_debut));

        QString
        date_fin(GET_SQL_RECORD_DATA
                 (record, YerothDatabaseTableColumn::DATE_FIN));
        dateEdit_detail_alerte_date_fin->setDate(GET_DATE_FROM_STRING
                                                 (date_fin));
    }

    textEdit_detail_alerte_message->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerothDatabaseTableColumn::MESSAGE_ALERTE));

    enableOtherTabs(SUJET_ACTION_ALERTE, false);

    setVisible(true);
}


void YerothAdminDetailWindow::retourListerCompteUtilisateur()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_COMPTE_UTILISATEUR);
    rendreInvisible();
}


void YerothAdminDetailWindow::retourListerLocalisation()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_LOCALISATION);
    rendreInvisible();
}


void YerothAdminDetailWindow::retourListerDepartementsDeProduits()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS);
    rendreInvisible();
}


void YerothAdminDetailWindow::retourListerCategorie()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CATEGORIE);
    rendreInvisible();
}


void YerothAdminDetailWindow::retourLister_LIGNE_BUDGETAIRE()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
    rendreInvisible();
}


void YerothAdminDetailWindow::retourListerCompteBancaire()
{
    _allWindows->_adminListerWindow->
    rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
    rendreInvisible();
}

void YerothAdminDetailWindow::retourListerRemise()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_REMISE);
    rendreInvisible();
}

void YerothAdminDetailWindow::retourListerChargeFinanciere()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);
    rendreInvisible();
}

void YerothAdminDetailWindow::retourListerAlerte()
{
    _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ALERTE);
    rendreInvisible();
}

void YerothAdminDetailWindow::clear_set_edit_utilisateur_comboBoxes()
{
    comboBox_detail_utilisateur_role->clear();
    comboBox_detail_utilisateur_titre->clear();
}

void YerothAdminDetailWindow::clear_set_edit_alerte_comboBoxes()
{
    comboBox_detail_alerte_condition->clear();
}

void YerothAdminDetailWindow::clear_set_edit_comboBoxes()
{
    clear_set_edit_utilisateur_comboBoxes();
    clear_set_edit_alerte_comboBoxes();
}
