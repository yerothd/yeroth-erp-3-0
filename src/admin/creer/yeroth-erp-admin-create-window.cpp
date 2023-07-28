/*
 * yeroth-erp-admin-create-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-admin-create-window.hpp"

#include "src/yeroth-erp-windows.hpp"


#include "src/utils/yeroth-erp-payment-processing-information.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QDesktopWidget>

#include <QtSql/QSqlError>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlQuery>


YerothAdminCreateWindow::YerothAdminCreateWindow()
:YerothPOSAdminWindowsCommons(QObject::tr("administration ~ créer")),
 _logger(new YerothLogger("YerothAdminCreateWindow"))
{
    setupUi(this);

    mySetupUi(this);


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}")
						.arg(COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67,
							 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setupLineEdits();

    setupDateTimeEdits();

    comboBox_creer_nom_departement->setYerothEditable(true);
    comboBox_creer_fournisseur->setYerothEditable(true);

    comboBox_creer_alerte_designation->setYerothEditable(true);


    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);

    pushButton_menu->enable(this, SLOT(menu()));

    pushButton_creer_charge_financiere_ANNULER->enable(this, SLOT(annuler()));
    pushButton_creer_utilisateur_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_localisation_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_departements_de_produits_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_categorie_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_ligne_budgetaire_ANNULER->enable(this, SLOT(annuler()));
    pushButton_creer_compte_bancaire_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_alerte_annuler->enable(this, SLOT(annuler()));
    pushButton_creer_remise_annuler->enable(this, SLOT(annuler()));



    pushButton_creer_charge_financiere_VALIDER->enable(this, SLOT(creer_charge_financiere()));

    pushButton_creer_utilisateur_valider->enable(this, SLOT(creer_utilisateur()));


    pushButton_creer_localisation_valider->enable(this, SLOT(creer_localisation()));

    pushButton_creer_departements_de_produits_valider
		->enable(this, SLOT(creer_departements_de_produits()));

    pushButton_creer_categorie_valider->enable(this, SLOT(creer_categorie()));

    pushButton_creer_LIGNE_budgetaire_VALIDER
		->enable(this, SLOT(creer_ligne_budgetaire()));

    pushButton_creer_compte_bancaire_valider
		->enable(this, SLOT(creer_compte_bancaire()));

    pushButton_creer_alerte_valider->enable(this, SLOT(creer_alerte()));

    pushButton_creer_remise_valider->enable(this, SLOT(creer_remise()));


    pushButton_lister->enable(this, SLOT(lister()));
    pushButton_modifier->enable(this, SLOT(lister()));


    connect(lineEdit_creer_CHARGE_FINANCIERE_tva_value,
    		SIGNAL(textChanged(const QString &)),
            this,
			SLOT(calculate_tva_value(const QString &)));


    connect(lineEdit_creer_prix_dachat_alunite,
    		SIGNAL(textChanged(const QString &)),
            this,
			SLOT(display_montant_prix_dachat_total(const QString &)));


    connect(doubleSpinBox_quantite,
    		SIGNAL(valueChanged(double)),
            this,
			SLOT(display_montant_prix_dachat_totalby_spinbox(double)));


    connect(actionStocks, SIGNAL(triggered()), this, SLOT(lister()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionModifier, SIGNAL(triggered()), this, SLOT(lister()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionRetournerMenuPrincipal, SIGNAL(triggered()), this,
            SLOT(retour_menu_principal()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));


    connect(lineEdit_creer_montant_de_la_LIGNE_BUDGETAIRE,
    		SIGNAL(textChanged(const QString &)),
			this,
			SLOT(HANDLE_lineEdit_creer_montant_RESTANT_de_la_LIGNE_BUDGETAIRE(const QString &)));


    connect(comboBox_creer_alerte_designation,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(showProduitInfo(const QString &)));

    connect(comboBox_creer_alerte_destinataire,
            SIGNAL(currentTextChanged(const QString &)), this,
            SLOT(showDestinataireNomComplet(const QString &)));


    connect(radioButton_creer_alerte_quantite, SIGNAL(clicked(bool)), this,
            SLOT(radioButtons_quantite()));

    connect(radioButton_creer_alerte_periode_temps, SIGNAL(clicked(bool)),
            this, SLOT(radioButtons_periode_temps()));

    connect(tabWidget_creer,
    		SIGNAL(currentChanged(int)),
			this,
            SLOT(handleCurrentChanged()));

    connect(tabWidget_creer, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentChanged()));
}

void YerothAdminCreateWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 false);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);
}

void YerothAdminCreateWindow::definirAdministrateur()
{
    _logger->log("definirAdministrateur");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 false);
}


void YerothAdminCreateWindow::definirManager()
{
    _logger->log("definirManager");
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                                 true);
    YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(actionRetournerMenuPrincipal,
                                                 true);
}


void YerothAdminCreateWindow::show_item_CHARGE_FINANCIERE(bool from_charge_financiere_window /* = false */)
{
    YerothSqlTableModel *CHARGES_FINANCIERES_TableModel = 0;

    int sqlTableRow = 0;


    if (from_charge_financiere_window)
    {
        YerothChargesFinancieresWindow *chfw = _allWindows->_charges_financieresWindow;

        CHARGES_FINANCIERES_TableModel = chfw->getCurStocksTableModel();

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


        sqlTableRow = chfw->getLastListerSelectedRow__ID_AS_INTEGER();
    }
    else
    {
        YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

        CHARGES_FINANCIERES_TableModel = lw->getCurSearchSqlTableModel();

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


        sqlTableRow = lw->lastSelectedItemForModification();
    }


	QSqlRecord record = CHARGES_FINANCIERES_TableModel->record(sqlTableRow);


	lineEdit_creer_reference
		->setText(GET_SQL_RECORD_DATA(record,
				  YerothDatabaseTableColumn::REFERENCE));

	lineEdit_creer_designation
		->setText(GET_SQL_RECORD_DATA(record,
				  YerothDatabaseTableColumn::DESIGNATION));

	comboBox_creer_nom_departement
		->find_AND_SET_CURRENT_INDEX
			(GET_SQL_RECORD_DATA(record,
			 YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

	comboBox_creer_LIGNE_BUDGETAIRE
		->find_AND_SET_CURRENT_INDEX
			(GET_SQL_RECORD_DATA(record,
			 YerothDatabaseTableColumn::CATEGORIE));

	comboBox_creer_fournisseur
		->find_AND_SET_CURRENT_INDEX
			(GET_SQL_RECORD_DATA(record,
			 YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

	doubleSpinBox_quantite
		->setValue(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::QUANTITE_TOTALE)
                    .toDouble());


    double prix_dachat_a_lunite =
        qAbs(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_UNITAIRE)
                .toDouble());


	lineEdit_creer_prix_dachat_alunite->setText(QString::number(prix_dachat_a_lunite));


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


	lineEdit_creer_prix_dachat_total->setText(GET_CURRENCY_STRING_NUM(prix_dachat));


	textEdit_creer_une_CHARGE_FINANCIERE->setText(GET_SQL_RECORD_DATA(record,
			YerothDatabaseTableColumn::DESCRIPTION_charge_financiere));

}


void YerothAdminCreateWindow::setupLineEdits()
{
	lineEdit_creer_prix_dachat_total->setYerothEnabled(false);


	lineEdit_creer_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->setYerothEnabled(false);

    lineEdit_creer_remise_montant->setYerothEnabled(false);

    lineEdit_creer_localisation_adresse_ip
		->setValidator(&YerothUtils::STRING_FOR_YEROTH_ERP_3_0_IP_ADDRESS_VALIDATOR);


    lineEdit_creer_CHARGE_FINANCIERE_tva_value->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_creer_prix_dachat_alunite->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_creer_remise_montant->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_creer_alerte_quantite->setValidator(&YerothUtils::UintValidator);

    lineEdit_creer_utilisateur_mot_passe_1->setEchoMode(QLineEdit::Password);

    lineEdit_creer_utilisateur_verification->setEchoMode(QLineEdit::Password);


    connect(lineEdit_creer_remise_designation_article,
            SIGNAL(textChanged(const QString &)),
			this,
            SLOT(showProduitInfo(const QString &)));
}


void YerothAdminCreateWindow::setupDateTimeEdits()
{
    dateEdit_creer_utilisateur_date_naissance->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_localisation_date_ouverture->setStartDate(GET_CURRENT_DATE);

    dateEdit_creer_alerte_date_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_alerte_date_fin->setStartDate(GET_CURRENT_DATE);

    dateEdit_creer_remise_date_debut->setStartDate(GET_CURRENT_DATE);
    dateEdit_creer_remise_date_fin->setStartDate(GET_CURRENT_DATE);
}


void YerothAdminCreateWindow::rendreVisible(YerothSqlTableModel *stocksTableModel,
                                            unsigned selectedSujetAction,
											bool 	 a_show_item_CHARGE_FINANCIERE /* = false */,
											bool     from_charge_financiere_window /* = false */ )
{
    retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    tabWidget_creer->setCurrentIndex(selectedSujetAction);

    clear_utilisateur_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_CHARGE_FINANCIERE_all_fields();

    clear_LIGNE_BUDGETAIRE_all_fields();

    clear_compte_bancaire_all_fields();

    clear_localisation_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();


    comboBox_nom_departement_produit
        ->populateComboBoxRawString(YerothDatabase::DEPARTEMENTS_PRODUITS,
                                    YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    comboBox_creer_alerte_designation
        ->populateComboBoxRawString(YerothDatabase::STOCKS,
                                    YerothDatabaseTableColumn::DESIGNATION);


    lineEdit_creer_remise_designation_article
        ->setupMyStaticQCompleter(YerothDatabase::STOCKS,
                                  YerothDatabaseTableColumn::DESIGNATION);


    lineEdit_creer_utilisateur_localisation->setYerothEnabled(false);


    lineEdit_creer_utilisateur_localisation
        ->setText(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);


    populateUtilisateurComboBoxes();

    populate_CHARGE_FINANCIERE_ComboBoxes();

    populate_LIGNE_BUDGETAIRE_ComboBoxes();

    populateRemiseComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();




    creer_departements_de_produits_check_fields();

    creer_categorie_check_fields();

    creer_charge_financiere_CHECK_fields();

    creer_ligne_budgetaire_CHECK_fields();

    creer_compte_bancaire_check_fields();

    creer_localisation_check_fields();


    creer_utilisateur_check_fields_entry();

    creer_alerte_check_fields_entry();

    creer_remise_check_fields_entry();


    lineEdit_creer_categorie_nom->setFocus();

    comboBox_creer_nom_departement->setFocus();

	lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->setFocus();

	lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->setFocus();

	lineEdit_creer_utilisateur_prenom->setFocus();

	lineEdit_creer_localisation_adresse_ip->setFocus();

	lineEdit_creer_remise_nom->setFocus();

    lineEdit_creer_departements_de_produits_nom->setFocus();


    if (a_show_item_CHARGE_FINANCIERE)
    {
    	show_item_CHARGE_FINANCIERE(from_charge_financiere_window);
    }


    setVisible(true);
}


void YerothAdminCreateWindow::rendreInvisible()
{
    dateEdit_creer_utilisateur_date_naissance->reset();
    dateEdit_creer_localisation_date_ouverture->reset();
    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();

    YerothPOSAdminWindowsCommons::rendreInvisible();
}

void YerothAdminCreateWindow::showDestinataireNomComplet(const QString &
                                                         destinataireId)
{
    _logger->log("showDestinataireNomComplet(const QString &)",
                 QString("destinataireId: %1").arg(destinataireId));

    QString
    strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg
             (YerothDatabaseTableColumn::NOM_COMPLET, YerothDatabase::USERS,
              YerothDatabaseTableColumn::NOM_UTILISATEUR, destinataireId));

    QSqlQuery query(strQuery);

    QSqlRecord rec = query.record();

    if (query.last())
    {
        QString destinataireId_nom_complet(query.value(0).toString());

        lineEdit_creer_alerte_nom_destinataire->setText
        (destinataireId_nom_complet);
    }
}


void YerothAdminCreateWindow::showProduitInfo(const QString &nomProduit)
{
    _logger->log("showProduitInfo(const QString &)",
                 QString("designation: %1").arg(nomProduit));

    double stock_dalerte = 0.0;

    double quantite_en_stock = 0.0;

    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));

    QString
    strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg
             (YerothDatabaseTableColumn::QUANTITE_TOTALE,
              YerothDatabase::STOCKS, YerothDatabaseTableColumn::DESIGNATION,
              sqlNomProduit));

    QSqlQuery query(strQuery);
    QSqlRecord rec = query.record();

    while (query.next())
    {
        quantite_en_stock += query.value(0).toDouble();
    }

    query.clear();

    strQuery =
                    QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg
                    (YerothDatabaseTableColumn::STOCK_DALERTE, YerothDatabase::STOCKS,
                     YerothDatabaseTableColumn::DESIGNATION, sqlNomProduit);

    if (query.exec(strQuery))
    {
        rec = query.record();

        if (query.next())
        {
            stock_dalerte = query.value(0).toDouble();
        }
    }

    switch (tabWidget_creer->currentIndex())
    {
    case SUJET_ACTION_ALERTE:
        lineEdit_creer_alerte_quantite_en_stock->setText(QString::number
                                                         (quantite_en_stock,
                                                          'f', 0));
        lineEdit_creer_alerte_stock_dalerte->
        setText(QString::number(stock_dalerte, 'f', 0));
        break;

    case SUJET_ACTION_REMISE:
        lineEdit_creer_remise_quantite_en_stock->setText(QString::number
                                                         (quantite_en_stock,
                                                          'f', 0));
        break;

    default:
        break;
    }

    showDatePeremption(nomProduit);
}

void YerothAdminCreateWindow::showDatePeremption(const QString &nomProduit)
{
    _logger->log("showDatePeremption",
                 QString("designation: %1").arg(nomProduit));

    QString sqlNomProduit(YerothUtils::prepareSqlStr(nomProduit));

    QString
    strQuery(QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg
             (YerothDatabaseTableColumn::DATE_PEREMPTION,
              YerothDatabase::STOCKS, YerothDatabaseTableColumn::DESIGNATION,
              sqlNomProduit));

    QSqlQuery query(strQuery);

    QSqlRecord rec = query.record();

    QDate date_peremption;
    QDate date_peremption_tmp;

    unsigned c = 0;

    while (query.next())
    {
        date_peremption_tmp = query.value(0).toDate();
        if (0 == c)
        {
            date_peremption = date_peremption_tmp;
            ++c;
        }
        else
        {
            if (date_peremption_tmp < date_peremption)
            {
                date_peremption = date_peremption_tmp;
            }
        }
    }

    switch (tabWidget_creer->currentIndex())
    {
    case SUJET_ACTION_ALERTE:
        lineEdit_creer_alerte_date_peremption->setText(date_peremption.
                                                       toString(YerothUtils::
                                                                DATE_FORMAT));
        break;

    case SUJET_ACTION_REMISE:
        lineEdit_creer_remise_date_peremption->setText(date_peremption.
                                                       toString(YerothUtils::
                                                                DATE_FORMAT));
        break;

    default:
        break;
    }
}

void YerothAdminCreateWindow::lister()
{
    _allWindows->_adminListerWindow->rendreVisible(tabWidget_creer->
                                                   currentIndex());
    rendreInvisible();
}

void YerothAdminCreateWindow::annuler()
{
    clear_utilisateur_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_compte_bancaire_all_fields();

    clear_localisation_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    _allWindows->_adminWindow
        ->rendreVisible(_curStocksTableModel);

    rendreInvisible();
}


void YerothAdminCreateWindow::radioButtons_quantite()
{
    dateEdit_creer_alerte_date_fin->setYerothEnabled(false);
    dateEdit_creer_alerte_date_debut->setYerothEnabled(false);

    comboBox_creer_alerte_condition->setEnabled(true);
    lineEdit_creer_alerte_quantite->setYerothEnabled(true);

    if (radioButton_creer_alerte_periode_temps->isChecked())
    {
        radioButton_creer_alerte_periode_temps->setChecked(false);
    }
}


void YerothAdminCreateWindow::radioButtons_periode_temps()
{
    dateEdit_creer_alerte_date_fin->setYerothEnabled(true);
    dateEdit_creer_alerte_date_debut->setYerothEnabled(true);

    comboBox_creer_alerte_condition->setEnabled(false);
    lineEdit_creer_alerte_quantite->setYerothEnabled(false);

    if (radioButton_creer_alerte_quantite->isChecked())
    {
        radioButton_creer_alerte_quantite->setChecked(false);
    }
}


void YerothAdminCreateWindow::handleCurrentChanged()
{
    clear_utilisateur_all_fields();

    clear_localisation_all_fields();

    clear_departements_de_produits_all_fields();

    clear_categorie_all_fields();

    clear_compte_bancaire_all_fields();

    clear_alerte_all_fields();

    clear_remise_all_fields();

    populateUtilisateurComboBoxes();

    populateAlerteComboBoxes();

    populateLocalisationComboBoxes();

    creer_utilisateur_check_fields_entry();

    creer_departements_de_produits_check_fields();

    creer_categorie_check_fields();

    creer_compte_bancaire_check_fields();

    creer_localisation_check_fields();

    creer_alerte_check_fields_entry();

    creer_remise_check_fields_entry();



	_allWindows->_adminWindow->_curAdminSujetAction =
			(enum AdminSujetAction) tabWidget_creer->currentIndex();


    switch (tabWidget_creer->currentIndex())
    {
    case SUJET_ACTION_COMPTE_UTILISATEUR:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ utilisateurs"));
		break;


    case SUJET_ACTION_LOCALISATION:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ LOCALISATIONS"));
        break;


    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ départements"));
		break;


    case SUJET_ACTION_CATEGORIE:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ catégories"));
        break;


    case SUJET_ACTION_ligne_budgetaire:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ LIGNES BUDÉGTAIRES"));
        break;


    case SUJET_ACTION_COMPTE_BANCAIRE:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ comptes bancaires"));
        break;


    case SUJET_ACTION_REMISE:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ remises"));
        break;


    case SUJET_ACTION_ALERTE:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ alertes"));
        break;


    case SUJET_ACTION_CHARGE_FINANCIERE:

        _windowName = QString("%1 - %2")
                    			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                    				 QObject::tr("administration ~ créer ~ charges financières"));
        break;

    default:
        break;
    }

    setWindowTitle(_windowName);
}


void YerothAdminCreateWindow::calculate_tva_value(const QString &VALEUR_TVA)
{
	_VALEUR_TVA = VALEUR_TVA.toDouble() / 100.0 ;

	display_montant_prix_dachat_total(lineEdit_creer_prix_dachat_alunite->text());

	display_montant_prix_dachat_totalby_spinbox(doubleSpinBox_quantite->value());
}


void YerothAdminCreateWindow::
		display_montant_prix_dachat_total(const QString &prix_dachat_alunite)
{
    double qte_totale = doubleSpinBox_quantite->value();

    double montant_PRIX_DACHAT_TOTAL =
    		qte_totale * lineEdit_creer_prix_dachat_alunite->text().toDouble();


    _MONTANT_TVA_CHARGE_FINANCIERE = _VALEUR_TVA * montant_PRIX_DACHAT_TOTAL;


    _montant_PRIX_DACHAT_TOTAL = _MONTANT_TVA_CHARGE_FINANCIERE +
    							 montant_PRIX_DACHAT_TOTAL;


    lineEdit_creer_prix_dachat_total
		->setText(GET_CURRENCY_STRING_NUM(_montant_PRIX_DACHAT_TOTAL));
}


void YerothAdminCreateWindow::
		display_montant_prix_dachat_totalby_spinbox(double quantite)
{
    double qte_totale = doubleSpinBox_quantite->value();

    double montant_PRIX_DACHAT_TOTAL =
    		qte_totale * lineEdit_creer_prix_dachat_alunite->text().toDouble();


    _MONTANT_TVA_CHARGE_FINANCIERE = _VALEUR_TVA * montant_PRIX_DACHAT_TOTAL;


    _montant_PRIX_DACHAT_TOTAL = _MONTANT_TVA_CHARGE_FINANCIERE +
    							 montant_PRIX_DACHAT_TOTAL;


    lineEdit_creer_prix_dachat_total
		->setText(GET_CURRENCY_STRING_NUM(_montant_PRIX_DACHAT_TOTAL));
}


void YerothAdminCreateWindow::hideEvent(QHideEvent *hideEvent)
{
    dateEdit_creer_utilisateur_date_naissance->reset();
    dateEdit_creer_localisation_date_ouverture->reset();
    dateEdit_creer_alerte_date_debut->reset();
    dateEdit_creer_alerte_date_fin->reset();
    dateEdit_creer_remise_date_debut->reset();
    dateEdit_creer_remise_date_fin->reset();
}




#include "creer-utilisateur.cpp"

#include "creer-localisation.cpp"

#include "creer-departements-de-produits.cpp"

#include "creer-categorie.cpp"

#include "creer-CHARGE_FINANCIERE.cpp"

#include "creer-LIGNE_BUDGETAIRE.cpp"

#include "creer-alerte.cpp"

#include "creer-remise.cpp"

#include "creer-compte-bancaire.cpp"
