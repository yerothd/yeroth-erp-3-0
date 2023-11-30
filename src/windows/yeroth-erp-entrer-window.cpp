/*
 * yeroth-erp-entrer-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-entrer-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-payment-processing-information.hpp"

#include "src/utils/yeroth-erp-service-stock-marchandise-data.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


#include <QtCore/QtMath>

#include <QtSql/QSqlQuery>


YerothEntrerWindow::YerothEntrerWindow():YerothWindowsCommons(),
    _logger(new YerothLogger("YerothEntrerWindow")),
    _currentServiceInfo(0),
    _stocks_id(0),
    _montantTva(0.0),
    _montantTva_en_gros(0.0),
    _historique_stock_actif(true),
	_tvaCheckBoxPreviousState(false)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("entrer 1 IMMOBILISATION, OU 1 stock, ou 1 service, Ou 1 Charge financière"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GRAY_78_78_78,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    checkBox_achat->setChecked(true);

    label_montant_total_vente_service->setVisible(false);

    lineEdit_service_montant_total_vente->setVisible(false);

    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();


    checkBox_HISTORIQUE->setChecked(_historique_stock_actif);

    checkBox_pourcentage_prix_dachat_prix_de_vente->setChecked(false);

    checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setChecked(false);

    checkBox_date_peremption->setChecked(true);


    comboBox_reference_produit->setYerothEditable(true);

    comboBox_nom_departement_produit->setYerothEditable(true);

    comboBox_nom_entreprise_fournisseur->setYerothEditable(true);

    comboBox_categorie_produit->setYerothEditable(true);


    populateEntrerUnStock_OU_ServiceComboBoxes();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this,
            SLOT(supprimer_image_stock()));
    connect(actionEnregisterStock, SIGNAL(triggered()), this,
            SLOT(enregistrer_produit()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(lineEdit_quantite_par_lot, SIGNAL(textChanged(const QString &)),
            this, SLOT(display_quantite_totale(const QString &)));


    connect(comboBox_categorie_produit,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_categorie_produit_changement(const QString &)));


    connect(comboBox_nom_departement_produit,
            SIGNAL(currentTextChanged(const QString &)),
            this,
            SLOT(handle_departement_de_produits_changement
                 (const QString &)));


    connect(doubleSpinBox_lots_entrant, SIGNAL(valueChanged(double)),
            this, SLOT(display_quantite_totale_by_spinbox(double)));


    connect(radioButton_inserer_une_IMMOBILISATION,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_checkBox_INSERT_ASSET(bool)));

    connect(radioButton_inserer_un_stock,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_checkBox_INSERT_STOCK(bool)));

    connect(radioButton_INSERER_UNE_CHARGE_FINANCIERE,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_checkBox_INSERER_UNE_CHARGE_FINANCIERE(bool)));

    connect(radioButton_service_vente_de_service_au_client,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_checkBox_service_vente(bool)));


    connect(checkBox_HISTORIQUE,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_HISTORIQUE_CHECKBOX(int)));


    connect(checkBox_pourcentage_prix_dachat_prix_de_vente,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_POURCENTAGE_PRIX_DACHAT_PRIX_DE_VENTE(int)));


    connect(checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros,
            SIGNAL(stateChanged(int)),
            this,
            SLOT(handle_POURCENTAGE_PRIX_DACHAT_PRIX_DE_VENTE_EN_GROS(int)));


    connect(checkBox_date_peremption,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_EXPIRATION_DATE_checkBox(int)));

    connect(checkBox_achat, SIGNAL(stateChanged(int)), this,
            SLOT(handle_achat_checkBox(int)));

    connect(checkBox_tva, SIGNAL(stateChanged(int)), this,
            SLOT(handleTVACheckBox(int)));

    connect(lineEdit_prix_dachat,
            SIGNAL(textChanged(const QString &)),
            this,
            SLOT(calculate_and_display_ALL_benefit_buying_price_percentage
                 ()));

    connect(lineEdit_prix_vente,
            SIGNAL(textChanged(const QString &)),
            this, SLOT(display_service_montant_total_vente()));

    connect(lineEdit_prix_vente,
            SIGNAL(textEdited(const QString &)),
            this, SLOT(edited_prix_vente(const QString &)));

    connect(lineEdit_prix_vente, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente()));

    connect(lineEdit_prix_vente_en_gros,
            SIGNAL(textEdited(const QString &)),
            this, SLOT(edited_prix_vente_en_gros(const QString &)));

    connect(lineEdit_prix_vente_en_gros, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente_en_gros()));


    connect(lineEdit_pourcentage_prix_dachat_prix_de_vente,
            SIGNAL(textEdited(const QString &)),
            this,
            SLOT(edited_pourcentage_prix_dachat_prix_de_vente
                 (const QString &)));


    connect(lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros,
            SIGNAL(textEdited(const QString &)),
            this,
            SLOT(edited_pourcentage_prix_dachat_prix_de_vente_EN_GROS
                 (const QString &)));


    connect(lineEdit_designation,
            SIGNAL(editingFinished()),
            this, SLOT(product_search_with_designation()));

    connect(comboBox_reference_produit,
            SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(product_search_with_reference()));

    radioButton_inserer_un_stock->setChecked(true);

    setupShortcuts();
}

void YerothEntrerWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->show();
    rendreInvisible();
}

void YerothEntrerWindow::setupLineEdits()
{
    label_POURCENTAGE_1->setVisible(false);
    label_POURCENTAGE_2->setVisible(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
    lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setYerothEnabled(false);
    lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setYerothEnabled
    (false);

    lineEdit_service_montant_total_vente->setYerothEnabled(false);
    lineEdit_quantite_totale->setYerothEnabled(false);
    lineEdit_tva->setYerothEnabled(false);

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());

    lineEdit_quantite_totale->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_quantite_par_lot->setValidator(&YerothUtils::UintValidator);
    lineEdit_stock_dalerte->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_prix_dachat->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_prix_vente->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_prix_vente_en_gros->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    calculate_and_display_benefit_buying_price_percentage();
}


void YerothEntrerWindow::setupLineEditsQCompleters__FOR_STOCK_INVENTORY()
{
    QString departement_de_produits_choisi =
                    comboBox_nom_departement_produit->currentText();

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        checkBox_tva->setVisible(true);
        lineEdit_tva->setVisible(true);
    }

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
            radioButton_service_vente_de_service_au_client->isChecked())
    {
        label_prix_vente->setVisible(true);
        lineEdit_prix_vente->setVisible(true);
        lineEdit_designation->clearQCompleter();
    }
    else
    {
        QString
        aConditionStr(YerothUtils::generateSqlIs
                      (YerothDatabaseTableColumn::IS_SERVICE,
                       YerothUtils::MYSQL_FALSE_LITERAL));

        lineEdit_designation->
        setupMyStaticQCompleter(YerothDatabase::MARCHANDISES,
                                YerothDatabaseTableColumn::DESIGNATION,
                                false, aConditionStr);
    }

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        label_fournisseur->setText(QObject::tr("client"));

        comboBox_nom_entreprise_fournisseur->populateComboBoxRawString
        (YerothDatabase::CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE);
    }
    else
    {
        label_fournisseur->setText(QObject::tr("fournisseur"));

        comboBox_nom_entreprise_fournisseur->populateComboBoxRawString
        (YerothDatabase::FOURNISSEURS,
         YerothDatabaseTableColumn::NOM_ENTREPRISE);
    }

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        label_prix_vente->setText(QObject::tr("prix d'achat"));

        label_montant_total_vente_service->
        setText(QObject::tr("montant total achat"));
    }
    else
    {
        label_prix_vente->setText(QObject::tr("prix de vente (TTC)"));

        label_montant_total_vente_service->
        setText(QObject::tr("montant total vente"));
    }
}


void YerothEntrerWindow::populateCOMBOBOX__REFERENCE_PRODUIT()
{
    if (radioButton_inserer_un_stock->isChecked())
    {
        QString
        aConditionStr(YerothUtils::generateSqlIs
                      (YerothDatabaseTableColumn::IS_SERVICE,
                       YerothUtils::MYSQL_FALSE_LITERAL));
        comboBox_reference_produit->populateComboBoxRawString(YerothDatabase::
                                                              MARCHANDISES,
                                                              YerothDatabaseTableColumn::
                                                              REFERENCE,
                                                              aConditionStr);
    }
    else if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        QString
        aConditionStr(YerothUtils::generateSqlIs
                      (YerothDatabaseTableColumn::IS_SERVICE,
                       YerothUtils::MYSQL_TRUE_LITERAL));

        comboBox_reference_produit->populateComboBoxRawString(YerothDatabase::
                                                              MARCHANDISES,
                                                              YerothDatabaseTableColumn::
                                                              REFERENCE,
                                                              aConditionStr);

        comboBox_reference_produit->populateComboBoxRawString_APPEND
        (YerothDatabase::SERVICES_COMPLETES,
         YerothDatabaseTableColumn::REFERENCE, aConditionStr);
    }
    else if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        comboBox_reference_produit->populateComboBoxRawString(YerothDatabase::
                                                              CHARGES_FINANCIERES,
                                                              YerothDatabaseTableColumn::
                                                              REFERENCE);
    }
}


void YerothEntrerWindow::populateCOMBOBOX__CATEGORIE()
{
    QString departement_de_produits_choisi =
                    comboBox_nom_departement_produit->currentText();

    QString
    aConditionStr(YerothUtils::generateSqlIs
                  (YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                   departement_de_produits_choisi));

    if (radioButton_inserer_une_IMMOBILISATION->isChecked() 	||
    	radioButton_inserer_un_stock->isChecked()				||
		radioButton_service_vente_de_service_au_client->isChecked())
    {
    	comboBox_categorie_produit->clearField();

    	comboBox_categorie_produit
			->populateComboBoxRawString(YerothDatabase::CATEGORIES,
										YerothDatabaseTableColumn::NOM_CATEGORIE,
										aConditionStr);
    }

    comboBox_categorie_produit->checkField();
}


void YerothEntrerWindow::populateEntrerUnStock_OU_ServiceComboBoxes(enum
                                                                    entrer_VISIBLE_CHOICE
                                                                    choice_insert
                                                                    /* = set_choice_insert_STANDARD_INVENTORY_STOCK */
                                                                   )
{
    if (set_choice_sell_TO_CLIENT == choice_insert)
    {
        comboBox_nom_entreprise_fournisseur->populateComboBoxRawString
        (YerothDatabase::CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE);
    }
    else if (set_choice_insert_FINANCIAL_EXPENSE == choice_insert)
    {
    	comboBox_categorie_produit
			->populateComboBoxRawString(YerothDatabase::LIGNES_BUDGETAIRES,
										YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);
    }
    else
    {
        comboBox_nom_entreprise_fournisseur->populateComboBoxRawString
        (YerothDatabase::FOURNISSEURS,
         YerothDatabaseTableColumn::NOM_ENTREPRISE);
    }

    comboBox_nom_departement_produit->populateComboBoxRawString
    (YerothDatabase::DEPARTEMENTS_PRODUITS,
     YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    comboBox_nom_departement_produit->setFocus();
}


void YerothEntrerWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothEntrerWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);;
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_afficher_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
    pushButton_INSERER->disable(this);
}


void YerothEntrerWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothEntrerWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);

    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_INSERER->enable(this, SLOT(handle_enregistrer()));
}


void YerothEntrerWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);

    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->disable(this);
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->disable(this);
    pushButton_INSERER->enable(this, SLOT(handle_enregistrer()));
}


void YerothEntrerWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregisterStock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);

    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_annuler->enable(this, SLOT(menu()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_INSERER->enable(this, SLOT(handle_enregistrer()));
}


void YerothEntrerWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothEntrerWindow::handle_enregistrer()
{
    if (radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
        executer_enregistrer_une_IMMOBILISATION();
    }
    else if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
    	enregistrer_une_charge_financiere();
    }
    else
    {
        enregistrer_produit();
    }
}


bool YerothEntrerWindow::product_search_with_designation()
{
    YerothSqlTableModel &productListSqlTableModel =
                    _allWindows->getSqlTableModel_marchandises();

    QString productName(lineEdit_designation->text());

    int productNameRowCount =
                    productListSqlTableModel.
                    Is_SearchQSqlTable(YerothDatabaseTableColumn::DESIGNATION, productName);

    if (!productName.isEmpty() && 1 == productNameRowCount)
    {
        QString
        strQuery(QString("select * from %1 where %2='%3' AND %4='%5'").arg
                 (productListSqlTableModel.sqlTableName(),
                  YerothDatabaseTableColumn::IS_SERVICE,
                  YerothUtils::MYSQL_FALSE_LITERAL,
                  YerothDatabaseTableColumn::DESIGNATION, productName));

        QSqlQuery query;

        query.prepare(strQuery);

        bool success = query.exec();

        if (success && query.last())
        {
            comboBox_reference_produit->setCurrentIndex
            (comboBox_reference_produit->findText
             (query.value(YerothDatabaseTableColumn::REFERENCE).
              toString()));

            comboBox_nom_departement_produit->setCurrentIndex
            (comboBox_nom_departement_produit->findText
             (query.value
              (YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT).
              toString()));

            comboBox_categorie_produit->setCurrentIndex
            (comboBox_categorie_produit->findText
             (query.value(YerothDatabaseTableColumn::CATEGORIE).
              toString()));

            textEdit_description->setText(query.value
                                          (YerothDatabaseTableColumn::
                                           DESCRIPTION_PRODUIT).toString());

            return true;
        }
    }

    return false;
}


bool YerothEntrerWindow::product_search_with_reference()
{
    YerothSqlTableModel &productListSqlTableModel =
                    _allWindows->getSqlTableModel_marchandises();

    QString aCodebar(comboBox_reference_produit->currentText());

    int codebarRowCount =
                    productListSqlTableModel.
                    Is_SearchQSqlTable(YerothDatabaseTableColumn::REFERENCE, aCodebar);

    if (!aCodebar.isEmpty() && 1 == codebarRowCount)
    {
        QString
        strQuery(QString("select * from %1 where %2='%3' AND %4='%5'").arg
                 (productListSqlTableModel.sqlTableName(),
                  YerothDatabaseTableColumn::IS_SERVICE,
                  YerothUtils::MYSQL_FALSE_LITERAL,
                  YerothDatabaseTableColumn::REFERENCE, aCodebar));

        QSqlQuery query;
        query.prepare(strQuery);

        bool success = query.exec();

        if (success && query.last())
        {
            lineEdit_designation->setText(query.
                                          value(YerothDatabaseTableColumn::
                                                DESIGNATION).toString());

            comboBox_nom_departement_produit->setCurrentIndex
            (comboBox_nom_departement_produit->findText
             (query.value
              (YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT).
              toString()));

            comboBox_categorie_produit->setCurrentIndex
            (comboBox_categorie_produit->findText
             (query.value(YerothDatabaseTableColumn::CATEGORIE).
              toString()));

            comboBox_reference_produit->setCurrentIndex
            (comboBox_reference_produit->findText(aCodebar));

            textEdit_description->setText(query.value
                                          (YerothDatabaseTableColumn::
                                           DESCRIPTION_PRODUIT).toString());

            return true;
        }
    }

    return false;
}


void YerothEntrerWindow::display_quantite_totale(const QString &quantite_par_lot)
{
    double qte_lot = quantite_par_lot.toDouble();
    double qte_totale = doubleSpinBox_lots_entrant->valueMultiplyBy(qte_lot);

    lineEdit_quantite_totale->setText(QString::number(qte_totale, 'f', 2));

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
            radioButton_service_vente_de_service_au_client->isChecked())
    {
        double montant_total_vente_ou_achat =
                        qte_totale * lineEdit_prix_vente->text().toDouble();
        lineEdit_service_montant_total_vente->setText(GET_CURRENCY_STRING_NUM
                                                      (montant_total_vente_ou_achat));
    }
}


void YerothEntrerWindow::display_quantite_totale_by_spinbox(double lots)
{
    double qte_lot = lineEdit_quantite_par_lot->text().toDouble();
    double qte_totale = lots * qte_lot;
    lineEdit_quantite_totale->setText(QString::number(qte_totale, 'f', 2));

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
            radioButton_service_vente_de_service_au_client->isChecked())
    {
        display_service_montant_total_vente();
    }
}


void YerothEntrerWindow::display_prix_vente()
{
    if (_lastEditedPrixVente != lineEdit_prix_vente->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        _montantTva = prix_vente * YerothERPConfig::tva_value;
        prix_vente = prix_vente + _montantTva;
        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
}


void YerothEntrerWindow::display_prix_vente_en_gros()
{
    if (_lastEditedPrixVente_en_gros != lineEdit_prix_vente_en_gros->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente_en_gros =
                        lineEdit_prix_vente_en_gros->text().toDouble();
        _montantTva_en_gros = prix_vente_en_gros * YerothERPConfig::tva_value;
        prix_vente_en_gros = prix_vente_en_gros + _montantTva_en_gros;
        lineEdit_prix_vente_en_gros->
        setText(QString::number(prix_vente_en_gros, 'f', 2));
    }
}


void
YerothEntrerWindow::
calculate_and_display_ALL_benefit_buying_price_percentage()
{
    calculate_and_display_benefit_buying_price_percentage();
    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void YerothEntrerWindow::calculate_and_display_benefit_buying_price_percentage()
{
    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && checkBox_achat->isChecked())
    {
        double prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double prix_vente = lineEdit_prix_vente->text().toDouble();

        QString STRING_pourcentage_benefice_prix_dachat = "0.00";

        if (!checkBox_pourcentage_prix_dachat_prix_de_vente->isChecked())
        {
            if (YerothUtils::isProfitable
                    (prix_vente, prix_dachat, _montantTva))
            {
                double pourcentage_benefice_prix_dachat =
                                POURCENTAGE_YEROTH_GET_VALUE
                                (YerothUtils::getMargeBeneficiaire(prix_vente, prix_dachat,
                                                                   _montantTva),
                                 prix_dachat);

                STRING_pourcentage_benefice_prix_dachat =
                                QString("%1").arg(QString::number
                                                  (pourcentage_benefice_prix_dachat, 'f',
                                                   2));
            }
            else
            {
                STRING_pourcentage_benefice_prix_dachat = "0.00";
            }
        }
        else
        {
            STRING_pourcentage_benefice_prix_dachat =
                            lineEdit_pourcentage_prix_dachat_prix_de_vente->text();

            prix_vente =
                            prix_dachat +
                            (prix_dachat *
                             STRING_pourcentage_benefice_prix_dachat.toDouble() / 100.0);

            prix_vente = prix_vente + _montantTva;

            lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
        }

        lineEdit_pourcentage_prix_dachat_prix_de_vente->setText
        (STRING_pourcentage_benefice_prix_dachat);
    }
}


void
YerothEntrerWindow::
calculate_and_display_benefit_buying_price_percentage_EN_GROS()
{
    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && checkBox_achat->isChecked())
    {
        double
        prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double
        prix_vente_en_gros = get_prix_vente_en_gros();

        QString
        STRING_pourcentage_benefice_prix_dachat_gros = "0.00";

        if (!checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->isChecked
                ())
        {
            if (YerothUtils::isProfitable(prix_vente_en_gros, prix_dachat,
                                          _montantTva_en_gros))
            {
                double
                pourcentage_benefice_prix_dachat_gros =
                                POURCENTAGE_YEROTH_GET_VALUE
                                (YerothUtils::getMargeBeneficiaire(prix_vente_en_gros,
                                                                   prix_dachat,
                                                                   _montantTva_en_gros),
                                 prix_dachat);

                STRING_pourcentage_benefice_prix_dachat_gros =
                                QString("%1").arg(QString::number
                                                  (pourcentage_benefice_prix_dachat_gros,
                                                   'f', 2));
            }
            else
            {
                STRING_pourcentage_benefice_prix_dachat_gros = "0.00";
            }
        }
        else
        {
            STRING_pourcentage_benefice_prix_dachat_gros =
                            lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->text();

            prix_vente_en_gros =
                            prix_dachat +
                            (prix_dachat *
                             STRING_pourcentage_benefice_prix_dachat_gros.toDouble() /
                             100.0);

            prix_vente_en_gros = prix_vente_en_gros + _montantTva;

            lineEdit_prix_vente_en_gros->
            setText(QString::number(prix_vente_en_gros, 'f', 2));
        }

        lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setText
        (STRING_pourcentage_benefice_prix_dachat_gros);
    }
}


void YerothEntrerWindow::edited_prix_vente(const QString &newPrixVente)
{
    _lastEditedPrixVente = newPrixVente;

    calculate_and_display_benefit_buying_price_percentage();
}


void YerothEntrerWindow::edited_prix_vente_en_gros(const QString &newPrixVente)
{
    _lastEditedPrixVente_en_gros = newPrixVente;

    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void YerothEntrerWindow::edited_pourcentage_prix_dachat_prix_de_vente(const
                                                                      QString &
                                                                      new_pourcentage_prix_dachat_prix_de_vente)
{
    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && checkBox_achat->isChecked())
    {
        double prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double DOUBLE_new_pourcentage_prix_dachat_prix_de_vente =
                        new_pourcentage_prix_dachat_prix_de_vente.toDouble();

        double prix_vente =
                        prix_dachat * (1 +
                                       (DOUBLE_new_pourcentage_prix_dachat_prix_de_vente /
                                        100.0));

        if (checkBox_tva->isChecked())
        {
            _montantTva = prix_vente * YerothERPConfig::tva_value;
        }
        else
        {
            _montantTva = 0;
        }

        prix_vente = prix_vente + _montantTva;

        //QDEBUG_STRING_OUTPUT_2_N("_montantTva", _montantTva);
        //QDEBUG_STRING_OUTPUT_2_N("DOUBLE_new_pourcentage_prix_dachat_prix_de_vente", DOUBLE_new_pourcentage_prix_dachat_prix_de_vente);
        //QDEBUG_STRING_OUTPUT_2_N("prix_dachat", prix_dachat);
        //QDEBUG_STRING_OUTPUT_2_N("prix_vente", prix_vente);

        if (YerothUtils::isProfitable(prix_vente, prix_dachat, _montantTva))
        {
            lineEdit_prix_vente->setText(QString("%1").arg
                                         (QString::
                                          number(prix_vente, 'f', 2)));
        }
        else
        {
            lineEdit_prix_vente->setText("0");
        }
    }
}


void
YerothEntrerWindow::edited_pourcentage_prix_dachat_prix_de_vente_EN_GROS
(const QString &new_pourcentage_prix_dachat_prix_de_vente_EN_GROS)
{
    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && checkBox_achat->isChecked())
    {
        double
        prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double
        DOUBLE_new_pourcentage_prix_dachat_prix_de_vente_EN_GROS =
                        new_pourcentage_prix_dachat_prix_de_vente_EN_GROS.toDouble();

        double
        prix_vente_en_gros =
                        prix_dachat * (1 +
                                       (DOUBLE_new_pourcentage_prix_dachat_prix_de_vente_EN_GROS
                                        / 100.0));

        if (checkBox_tva->isChecked())
        {
            _montantTva_en_gros =
                            prix_vente_en_gros * YerothERPConfig::tva_value;
        }
        else
        {
            _montantTva_en_gros = 0;
        }

        prix_vente_en_gros = prix_vente_en_gros + _montantTva_en_gros;

        if (YerothUtils::isProfitable
                (prix_vente_en_gros, prix_dachat, _montantTva_en_gros))
        {
            lineEdit_prix_vente_en_gros->setText(QString("%1").arg
                                                 (QString::
                                                  number(prix_vente_en_gros,
                                                         'f', 2)));
        }
        else
        {
            lineEdit_prix_vente_en_gros->setText("0");
        }
    }
}


void YerothEntrerWindow::display_service_montant_total_vente()
{
    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
            radioButton_service_vente_de_service_au_client->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        double qte_totale = lineEdit_quantite_totale->text().toDouble();
        double montant_total_vente = qte_totale * prix_vente;
        lineEdit_service_montant_total_vente->setText(GET_CURRENCY_STRING_NUM
                                                      (montant_total_vente));
    }
    else
    {
        calculate_and_display_benefit_buying_price_percentage();
    }
}


void YerothEntrerWindow::setStockSpecificWidgetVisible(bool visible)
{
    if (visible)
    {
        checkBox_HISTORIQUE->setVisible(true);

        doubleSpinBox_lots_entrant->setMinimum(1.0);

        doubleSpinBox_lots_entrant->setFixedWidth(52);

        doubleSpinBox_lots_entrant->setDecimals(0);

        lineEdit_quantite_par_lot->clear();

        lineEdit_prix_vente->setFixedWidth(88);

        lineEdit_quantite_totale->setFixedWidth(104);
    }
    else
    {
        checkBox_HISTORIQUE->setVisible(false);

        doubleSpinBox_lots_entrant->setMinimum(0.0);

        doubleSpinBox_lots_entrant->setFixedWidth(205);

        doubleSpinBox_lots_entrant->setDecimals(2);

        lineEdit_quantite_par_lot->setText("1");

        lineEdit_prix_vente->setFixedWidth(205);

        lineEdit_quantite_totale->setFixedWidth(205);

        if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
                radioButton_service_vente_de_service_au_client->isChecked())
        {
            doubleSpinBox_lots_entrant->setValue(0.0);
        }
        else
        {
            doubleSpinBox_lots_entrant->setValue(1.0);
        }
    }


    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        checkBox_tva->setVisible(false);
        lineEdit_tva->setVisible(false);
    }
    else
    {
        checkBox_tva->setVisible(true);
        lineEdit_tva->setVisible(true);
    }


    label_description->setVisible(visible);

    textEdit_description->setVisible(visible);

    label_quantite->setVisible(visible);

    label_quantite_totale->setVisible(visible);

    lineEdit_quantite_totale->setVisible(visible);

    lineEdit_quantite_par_lot->setVisible(visible);

    pushButton_selectionner_image->setVisible(visible);

    pushButton_supprimer_limage_du_stock->setVisible(visible);

    groupBox_image_du_stock_service->setVisible(visible);

    label_reference_recu_dachat->setVisible(visible);
    lineEdit_reference_recu_dachat->setVisible(visible);

    label_stock_dalerte->setVisible(visible);
    lineEdit_stock_dalerte->setVisible(visible);

    label_localisation_du_stock->setVisible(visible);
    lineEdit_localisation_produit->setVisible(visible);

    checkBox_date_peremption->setChecked(visible);

    dateEdit_date_peremption->setVisible(visible);
    checkBox_date_peremption->setVisible(visible);


    /*
     * '*_montant_total_vente_service' et
     * '*_prix_dachat' sont justaposes dans
     * l'interface utilisateur.
     *
     * Donc ils ne peuvent jamais etre visible
     * au meme moment.
     */
    label_montant_total_vente_service->setVisible(!visible);
    lineEdit_service_montant_total_vente->setVisible(!visible);

    label_prix_vente_en_gros->setVisible(visible);
    lineEdit_prix_vente_en_gros->setVisible(visible);

    label_prix_dachat->setVisible(visible);
    lineEdit_prix_dachat->setVisible(visible);

    label_POURCENTAGE_1->setVisible(visible);
    label_POURCENTAGE_2->setVisible(visible);
    lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(visible);
    lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
    (visible);

    checkBox_achat->setVisible(visible);
}


void YerothEntrerWindow::handle_checkBox_INSERT_ASSET(bool checked)
{
    radioButton_inserer_une_IMMOBILISATION->setChecked(checked);

    if (radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
    	comboBox_categorie_produit->clear();

        radioButton_inserer_une_IMMOBILISATION->
        setPalette(YerothUtils::YEROTH_BLUE_PALETTE);
        radioButton_inserer_une_IMMOBILISATION->
        setStyleSheet(QString
                      ("color: rgb(0, 0, 255);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_un_stock->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_un_stock->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_service_vente_de_service_au_client->setPalette
        (YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_service_vente_de_service_au_client->setStyleSheet(QString
                                                                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }
    else
    {
        radioButton_inserer_une_IMMOBILISATION->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_une_IMMOBILISATION->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }

    radioButton_inserer_une_IMMOBILISATION->update();
    radioButton_inserer_un_stock->update();
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->update();
    radioButton_service_vente_de_service_au_client->update();


    if (radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
//      QDEBUG_STRING_OUTPUT_1("handle_checkBox_INSERT_ASSET 1");

        label_localisation_du_stock->setText(QObject::tr
                                             ("localisation immobilisation"));

        label_prix_vente->setVisible(false);
        label_prix_vente_en_gros->setVisible(false);

        lineEdit_prix_vente->setVisible(false);
        lineEdit_prix_vente_en_gros->setVisible(false);

        checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
        checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (false);
        checkBox_tva->setVisible(false);
        lineEdit_tva->setVisible(false);
    }
    else
    {
//      QDEBUG_STRING_OUTPUT_1("handle_checkBox_INSERT_ASSET 2");

        label_localisation_du_stock->
        setText(QObject::tr("localisation STOCK"));

        label_prix_vente->setVisible(true);
        label_prix_vente_en_gros->setVisible(true);

        lineEdit_prix_vente->setVisible(true);
        lineEdit_prix_vente_en_gros->setVisible(true);

        checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(true);
        checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (true);
        checkBox_tva->setVisible(true);
        lineEdit_tva->setVisible(true);
    }

    set_achat_checkBox_elements_visibility();
}


void YerothEntrerWindow::handle_checkBox_INSERT_STOCK(bool checked)
{
    radioButton_inserer_un_stock->setChecked(checked);

    if (radioButton_inserer_un_stock->isChecked())
    {
    	comboBox_categorie_produit->clear();

        radioButton_inserer_un_stock->
        setPalette(YerothUtils::YEROTH_BLUE_PALETTE);
        radioButton_inserer_un_stock->
        setStyleSheet(QString
                      ("color: rgb(0, 0, 255);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_une_IMMOBILISATION->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_une_IMMOBILISATION->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_service_vente_de_service_au_client->setPalette
        (YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_service_vente_de_service_au_client->setStyleSheet(QString
                                                                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }
    else
    {
        radioButton_inserer_un_stock->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_un_stock->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }

    radioButton_inserer_une_IMMOBILISATION->update();
    radioButton_inserer_un_stock->update();
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->update();
    radioButton_service_vente_de_service_au_client->update();


    if (radioButton_inserer_un_stock->isChecked())
    {
        label_localisation_du_stock->
        setText(QObject::tr("localisation STOCK"));

        label_prix_vente->setVisible(true);
        label_prix_vente_en_gros->setVisible(true);

        lineEdit_prix_vente->setVisible(true);
        lineEdit_prix_vente_en_gros->setVisible(true);

        checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(true);
        checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (true);
        checkBox_tva->setVisible(true);
        lineEdit_tva->setVisible(true);
    }
    else
    {
        label_localisation_du_stock->setText(QObject::tr
                                             ("localisation immobilisation"));

        label_prix_vente->setVisible(false);
        label_prix_vente_en_gros->setVisible(false);

        lineEdit_prix_vente->setVisible(false);
        lineEdit_prix_vente_en_gros->setVisible(false);

        checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
        checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (false);
        checkBox_tva->setVisible(false);
        lineEdit_tva->setVisible(false);
    }

    set_achat_checkBox_elements_visibility();
}


void YerothEntrerWindow::handle_checkBox_INSERER_UNE_CHARGE_FINANCIERE(bool
                                                                       checked)
{
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->setChecked(checked);

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
    	comboBox_categorie_produit
			->populateComboBoxRawString(YerothDatabase::LIGNES_BUDGETAIRES,
										YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);

    	comboBox_categorie_produit->setYerothEditable(false);

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setPalette(YerothUtils::YEROTH_BLUE_PALETTE);
        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setStyleSheet(QString
                      ("color: rgb(0, 0, 255);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_une_IMMOBILISATION->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_une_IMMOBILISATION->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_un_stock->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_un_stock->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_service_vente_de_service_au_client->setPalette
        (YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_service_vente_de_service_au_client->setStyleSheet(QString
                                                                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }
    else
    {
    	comboBox_categorie_produit->setYerothEditable(true);

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }

    radioButton_inserer_une_IMMOBILISATION->update();
    radioButton_inserer_un_stock->update();
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->update();
    radioButton_service_vente_de_service_au_client->update();


    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        setStockSpecificWidgetVisible(false);

        check_fields_service_vente();
    }
    else
    {
        setStockSpecificWidgetVisible(true);

        check_fields(true);
    }

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
    	label_categorie->setText(QObject::tr("LIGNE BUDGÉTAIRE"));
    }
    else
    {
    	label_categorie->setText(QObject::tr("catégorie"));
    }

    setupLineEditsQCompleters__FOR_STOCK_INVENTORY();
}


void YerothEntrerWindow::handle_checkBox_service_vente(bool checked)
{
    radioButton_service_vente_de_service_au_client->setChecked(checked);

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
    	comboBox_categorie_produit->clear();

        radioButton_service_vente_de_service_au_client->setPalette
        (YerothUtils::YEROTH_BLUE_PALETTE);
        radioButton_service_vente_de_service_au_client->setStyleSheet(QString
                                                                      ("color: rgb(0, 0, 255);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_une_IMMOBILISATION->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_une_IMMOBILISATION->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_inserer_un_stock->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_inserer_un_stock->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setPalette(YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_INSERER_UNE_CHARGE_FINANCIERE->
        setStyleSheet(QString
                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }
    else
    {
        radioButton_service_vente_de_service_au_client->setPalette
        (YerothUtils::YEROTH_LIGHT_GREEN_PALETTE);
        radioButton_service_vente_de_service_au_client->setStyleSheet(QString
                                                                      ("color: rgb(0, 255, 100);  border: 0px solid rgb(60, 60, 60);"));
    }

    radioButton_inserer_une_IMMOBILISATION->update();
    radioButton_inserer_un_stock->update();
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->update();
    radioButton_service_vente_de_service_au_client->update();


    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        setStockSpecificWidgetVisible(false);

        check_fields_service_vente();
    }
    else
    {
        setStockSpecificWidgetVisible(true);

        check_fields(true);
    }

    setupLineEditsQCompleters__FOR_STOCK_INVENTORY();
}


void YerothEntrerWindow::handleTVACheckBox(int state)
{
    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_en_gros = get_prix_vente_en_gros();

    if (checkBox_tva->isChecked())
    {
        _montantTva = prix_vente * YerothERPConfig::tva_value;

        _montantTva_en_gros = prix_vente_en_gros * YerothERPConfig::tva_value;

        prix_vente = prix_vente + _montantTva;

        prix_vente_en_gros = prix_vente_en_gros + _montantTva_en_gros;
    }
    else
    {
        _montantTva = 0;

        _montantTva_en_gros = 0;

        prix_vente = prix_vente / (1 + YerothERPConfig::tva_value);

        prix_vente_en_gros =
                        prix_vente_en_gros / (1 + YerothERPConfig::tva_value);
    }

    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    lineEdit_prix_vente_en_gros->
    setText(QString::number(prix_vente_en_gros, 'f', 2));

    calculate_and_display_benefit_buying_price_percentage();

    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


bool YerothEntrerWindow::check_fields_mandatory_buying()
{
    bool prix_dachat = true;

    set_achat_checkBox_elements_visibility();

    if (checkBox_achat->isChecked())
    {
        prix_dachat = lineEdit_prix_dachat->checkField();
    }

    return prix_dachat;
}


void YerothEntrerWindow::set_achat_checkBox_elements_visibility()
{
    if (checkBox_achat->isChecked())
    {
        label_prix_dachat->setVisible(true);

        lineEdit_prix_vente->setFixedWidth(88);
        lineEdit_prix_vente_en_gros->setFixedWidth(88);

        lineEdit_prix_dachat->setVisible(true);

        if (radioButton_inserer_un_stock->isChecked())
        {
            checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(true);
            checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
            (true);
            label_POURCENTAGE_1->setVisible(true);
            label_POURCENTAGE_2->setVisible(true);
            lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(true);
            lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
            (true);
        }
        else
        {
            checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
            checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
            (false);
            label_POURCENTAGE_1->setVisible(false);
            label_POURCENTAGE_2->setVisible(false);
            lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
            lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
            (false);
        }
    }
    else
    {
        label_prix_dachat->setVisible(false);

        lineEdit_prix_vente->setFixedWidth(205);
        lineEdit_prix_vente_en_gros->setFixedWidth(205);

        lineEdit_prix_dachat->setText(YerothUtils::EMPTY_STRING);

        lineEdit_prix_dachat->setVisible(false);

        checkBox_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
        checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (false);

        label_POURCENTAGE_1->setVisible(false);
        label_POURCENTAGE_2->setVisible(false);

        lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(false);
        lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setVisible
        (false);
    }
}


void YerothEntrerWindow::showItem(YerothSqlTableModel *
                                  stocks_OR_marchandises_TableModel /* = 0 */,
                                  enum entrer_VISIBLE_CHOICE choice_insert
                                  /* = set_choice_insert_ASSET */)
{
    populateEntrerUnStock_OU_ServiceComboBoxes();

    QString yerothSqlTableModelFilter =
    QString("%1 = '%2'").arg(YerothDatabaseTableColumn::ID,
                             YerothERPWindows::get_last_lister_selected_row_db_ID());


    if (0 == stocks_OR_marchandises_TableModel)
    {
        stocks_OR_marchandises_TableModel = _curStocksTableModel;
    }


    QSqlRecord record;

    stocks_OR_marchandises_TableModel->yerothSetFilter_WITH_where_clause
    (yerothSqlTableModelFilter);

    record = stocks_OR_marchandises_TableModel->record(0);


    lineEdit_designation->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerothDatabaseTableColumn::DESIGNATION));
    textEdit_description->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));

    double prix_vente = GET_SQL_RECORD_DATA(record,
                                            YerothDatabaseTableColumn::PRIX_VENTE).
    toDouble();
    double montant_tva = GET_SQL_RECORD_DATA(record,
                                             YerothDatabaseTableColumn::MONTANT_TVA).
    toDouble();

    double prix_vente_en_gros = GET_SQL_RECORD_DATA(record,
                                                    YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS).
    toDouble();
    double montant_tva_en_gros = GET_SQL_RECORD_DATA(record,
                                                     YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS).
    toDouble();


    if (!YerothUtils::IS_QSQL_RECORD_AN_ASSET(record))
    {
        lineEdit_designation->setReadOnly(true);
    }
    else
    {
        lineEdit_designation->setReadOnly(false);
    }


    if (prix_vente <= 0)
    {
        if (set_choice_insert_STANDARD_INVENTORY_STOCK == choice_insert)
        {
            radioButton_inserer_un_stock->setYerothEnabled(true);
            radioButton_inserer_un_stock->setChecked(true);

            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);

            handle_checkBox_INSERT_STOCK(true);
        }
        else if (set_choice_insert_ASSET == choice_insert)
        {
            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(true);
            radioButton_inserer_une_IMMOBILISATION->setChecked(true);

            radioButton_inserer_un_stock->setYerothEnabled(false);

            handle_checkBox_INSERT_ASSET(true);
        }
    }
    else
    {
        radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);

        radioButton_inserer_un_stock->setYerothEnabled(true);
        radioButton_inserer_un_stock->setChecked(true);

        handle_checkBox_INSERT_STOCK(false);
    }


    _historique_stock_actif =
                    GET_SQL_RECORD_DATA(record,
                                        YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                    toInt();


    checkBox_HISTORIQUE->setChecked(_historique_stock_actif);


    double prix_dachat = GET_SQL_RECORD_DATA(record,
                                             YerothDatabaseTableColumn::PRIX_DACHAT).
                         toDouble();

    if (prix_dachat > 0)
    {
        lineEdit_prix_dachat->setText(QString::number(prix_dachat));
    }
    else
    {
        lineEdit_prix_dachat->setText(YerothUtils::EMPTY_STRING);
    }


    if (0 < montant_tva || 0 < montant_tva_en_gros)
    {
        checkBox_tva->setChecked(true);
        handleTVACheckBox(true);
    }
    else
    {
        checkBox_tva->setChecked(false);
        handleTVACheckBox(false);
    }

    if (prix_vente > 0)
    {
        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
    else
    {
        lineEdit_prix_vente->setText(YerothUtils::EMPTY_STRING);
    }

    if (prix_vente_en_gros > 0)
    {
        lineEdit_prix_vente_en_gros->
        setText(QString::number(prix_vente_en_gros, 'f', 2));
    }
    else
    {
        lineEdit_prix_vente_en_gros->setText(YerothUtils::EMPTY_STRING);
    }

    comboBox_nom_departement_produit->find_AND_SET_CURRENT_INDEX_NOT_EDITABLE
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    comboBox_categorie_produit->find_AND_SET_CURRENT_INDEX_NOT_EDITABLE
    (GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::CATEGORIE));

    QString reference_produit =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::REFERENCE);

    if (!YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING,
                                             reference_produit))
    {
        comboBox_reference_produit->find_AND_SET_CURRENT_INDEX_NOT_EDITABLE
        (reference_produit);
    }
    else
    {
        comboBox_reference_produit->setYerothEnabled(false);
    }

    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::LOCALISATION_STOCK));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }

    if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::STOCKS,
                                            stocks_OR_marchandises_TableModel->
                                            sqlTableName()))
    {
        QString recordID = YerothERPWindows::get_last_lister_selected_row_db_ID();

        int achatQuerySize =
                        YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(recordID);

        if (achatQuerySize > 0)
        {
            checkBox_achat->setChecked(true);
        }
        else
        {
            checkBox_achat->setChecked(false);
        }
    }

    stocks_OR_marchandises_TableModel->resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

    lineEdit_quantite_par_lot->setFocus();
}


bool YerothEntrerWindow::check_fields_service_achat()
{
    bool fournisseur = comboBox_nom_entreprise_fournisseur->checkField();

    bool nom_departement_produit =
                    comboBox_nom_departement_produit->checkField();

    bool categorie_produit = comboBox_categorie_produit->checkField();

    bool reference = comboBox_reference_produit->checkField();

    bool quantite = lineEdit_quantite_par_lot->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    bool result = fournisseur &&
                  nom_departement_produit &&
                  categorie_produit && reference && prix_vente && quantite;

    return result;
}


bool YerothEntrerWindow::check_fields_service_vente()
{
    bool designation = lineEdit_designation->checkField();

    bool client_fournisseur =
                    comboBox_nom_entreprise_fournisseur->checkField();

    bool nom_departement_produit =
                    comboBox_nom_departement_produit->checkField();

    bool categorie_produit = comboBox_categorie_produit->checkField();

    bool reference = comboBox_reference_produit->checkField();

    bool quantite = lineEdit_quantite_par_lot->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    bool result = designation &&
                  client_fournisseur &&
                  nom_departement_produit &&
                  categorie_produit && reference && prix_vente && quantite;

    return result;
}


bool YerothEntrerWindow::check_fields(bool withClearAllServiceMandatoryFields
                                      /*  = false */)
{
    if (withClearAllServiceMandatoryFields)
    {
        comboBox_nom_entreprise_fournisseur->resetYerothComboBox();

        comboBox_reference_produit->clearField();
    }

    bool prix_dachat = check_fields_mandatory_buying();

    bool designation = lineEdit_designation->checkField();

    bool quantite_par_lot = lineEdit_quantite_par_lot->checkField();

    if (quantite_par_lot)
    {
        if (lineEdit_quantite_par_lot->text().toDouble() <= 0)
        {
            YerothQMessageBox::information(this,
                                           QObject::tr("enregistrer article"),
                                           QObject::tr
                                           ("La quantité par lot doit être supérieure à zéro !"));

            lineEdit_quantite_par_lot->clear();

            lineEdit_quantite_par_lot->checkField();

            return false;
        }
    }

    bool nom_departement_produit =
                    comboBox_nom_departement_produit->checkField();

    bool categorie_produit = comboBox_categorie_produit->checkField();


    bool result = false;


    if (radioButton_inserer_un_stock->isChecked())
    {
        bool prix_vente = lineEdit_prix_vente->checkField();

        result = prix_dachat &&
                 designation &&
                 nom_departement_produit &&
                 categorie_produit && prix_vente && quantite_par_lot;
    }
    else
    {
        result = prix_dachat &&
                 designation &&
                 nom_departement_produit && categorie_produit && quantite_par_lot;
    }


    return result;
}


void YerothEntrerWindow::clear_all_fields()
{
    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
            radioButton_service_vente_de_service_au_client->isChecked())
    {
        doubleSpinBox_lots_entrant->setValue(0.0);
    }
    else
    {
        doubleSpinBox_lots_entrant->setValue(1.0);
    }

    comboBox_reference_produit->clearField();
    lineEdit_designation->clearField();
    comboBox_nom_departement_produit->clearField();
    comboBox_categorie_produit->clearField();
    comboBox_nom_entreprise_fournisseur->clearField();

    lineEdit_pourcentage_prix_dachat_prix_de_vente->clear();
    lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->clear();

    lineEdit_quantite_par_lot->clearField();
    lineEdit_quantite_totale->clear();
    lineEdit_stock_dalerte->clearField();
    lineEdit_reference_recu_dachat->clearField();
    lineEdit_prix_dachat->clearField();
    lineEdit_prix_vente->clearField();
    lineEdit_prix_vente_en_gros->clearField();
    lineEdit_localisation_produit->clearField();

    textEdit_description->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    checkBox_tva->setChecked(false);

    _lastEditedPrixVente.clear();
    _lastEditedPrixVente_en_gros.clear();

    _montantTva = 0.0;
    _montantTva_en_gros = 0.0;

    _tvaCheckBoxPreviousState = false;
}

void YerothEntrerWindow::rendreInvisible()
{
    clear_all_fields();
    dateEdit_date_peremption->reset();
    setVisible(false);
}

void YerothEntrerWindow::rendreVisible(YerothSqlTableModel 	*stocksTableModel,
                                       bool 				aShowItem /* = false */,
                                       YerothSqlTableModel 	*marchandisesTableModel /* = 0 */,
                                       enum 				entrer_VISIBLE_CHOICE choice_insert /* = set_choice_insert_ASSET */)
{
    //QDEBUG_STRING_OUTPUT_2_N("choice_insert", choice_insert);
	retranslateUi(this);

    groupBox_choix->setVisible(false);

    radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);
    radioButton_inserer_un_stock->setYerothEnabled(false);
    radioButton_service_vente_de_service_au_client->setYerothEnabled(false);
    radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(false);

    radioButton_inserer_une_IMMOBILISATION->setChecked(true);

    YerothPOSUser *aCurrentUser = _allWindows->getUser();

    if (0 != aCurrentUser)
    {
        if (aCurrentUser->isManager())
        {
            groupBox_choix->setVisible(true);

            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(true);
            radioButton_inserer_un_stock->setYerothEnabled(true);
            radioButton_service_vente_de_service_au_client->setYerothEnabled(true);
            radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(true);

            if (set_choice_sell_TO_CLIENT == choice_insert)
            {
                radioButton_service_vente_de_service_au_client->setChecked(true);
            }
            else if (set_choice_insert_FINANCIAL_EXPENSE == choice_insert)
            {
                radioButton_INSERER_UNE_CHARGE_FINANCIERE->setChecked(true);
            }
            else if (set_choice_insert_STANDARD_INVENTORY_STOCK == choice_insert)
            {
                radioButton_inserer_un_stock->setChecked(true);
            }

        }
        else if (aCurrentUser->isVendeur())
        {
            groupBox_choix->setVisible(true);

            radioButton_service_vente_de_service_au_client->setYerothEnabled(true);

            radioButton_service_vente_de_service_au_client->setChecked(true);
        }
        else if (aCurrentUser->isGestionaireDesStocks())
        {
            groupBox_choix->setVisible(true);

            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(true);

            radioButton_inserer_un_stock->setYerothEnabled(true);

            if (set_choice_insert_ASSET == choice_insert)
            {
                radioButton_inserer_une_IMMOBILISATION->setChecked(true);
            }
            else			// set_choice_insert_STANDARD_INVENTORY_STOCK
            {
                radioButton_inserer_un_stock->setChecked(true);
            }
        }
    }

    _curStocksTableModel = stocksTableModel;

    bool stockCheckInVisible = true;


    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked() ||
        radioButton_service_vente_de_service_au_client->isChecked())
    {
        stockCheckInVisible = false;
    }

    if (aShowItem)
    {
        stockCheckInVisible = true;

        radioButton_INSERER_UNE_CHARGE_FINANCIERE->setChecked(false);
        radioButton_service_vente_de_service_au_client->setChecked(false);

        radioButton_service_vente_de_service_au_client->setYerothEnabled(false);

        if (set_choice_sell_TO_CLIENT == choice_insert)
        {
        	radioButton_service_vente_de_service_au_client->setChecked(true);
        	radioButton_service_vente_de_service_au_client->setYerothEnabled(true);

        	radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);
        	radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(false);
        	radioButton_inserer_un_stock->setYerothEnabled(false);
        }
        if (set_choice_insert_FINANCIAL_EXPENSE == choice_insert)
        {
        	radioButton_INSERER_UNE_CHARGE_FINANCIERE->setChecked(true);
        	radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(true);

        	radioButton_service_vente_de_service_au_client->setYerothEnabled(false);
        	radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);
        	radioButton_inserer_un_stock->setYerothEnabled(false);
        }
        if (set_choice_insert_STANDARD_INVENTORY_STOCK == choice_insert)
        {
            radioButton_inserer_un_stock->setChecked(true);
            radioButton_inserer_un_stock->setYerothEnabled(true);

            radioButton_service_vente_de_service_au_client->setYerothEnabled(false);
            radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(false);
            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(false);
        }
        else if (set_choice_insert_ASSET == choice_insert)
        {
            radioButton_inserer_une_IMMOBILISATION->setChecked(true);
            radioButton_inserer_une_IMMOBILISATION->setYerothEnabled(true);

            radioButton_service_vente_de_service_au_client->setYerothEnabled(false);
            radioButton_INSERER_UNE_CHARGE_FINANCIERE->setYerothEnabled(false);
            radioButton_inserer_un_stock->setYerothEnabled(false);
        }
    }


    if (stockCheckInVisible &&
            set_choice_insert_STANDARD_INVENTORY_STOCK == choice_insert)
    {
        setStockSpecificWidgetVisible(stockCheckInVisible);
    }

    setupLineEditsQCompleters();

    populateEntrerUnStock_OU_ServiceComboBoxes(choice_insert);

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());

    if (radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        checkBox_achat->setChecked(false);
        lineEdit_prix_vente->setFixedWidth(205);
        lineEdit_quantite_totale->setFixedWidth(205);

        check_fields_service_achat();
    }
    else if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        checkBox_achat->setChecked(false);
        lineEdit_prix_vente->setFixedWidth(205);
        lineEdit_quantite_totale->setFixedWidth(205);

        check_fields_service_vente();
    }
    else
    {
        if (aShowItem)
        {
            clear_all_fields();

            check_fields(true);

            showItem(marchandisesTableModel, choice_insert);
        }
    }

    if (!aShowItem)
    {
        comboBox_reference_produit->setYerothEnabled(true);

        comboBox_nom_departement_produit->setYerothEnabled(true);

        comboBox_categorie_produit->setYerothEnabled(true);

        lineEdit_designation->setReadOnly(false);

        check_fields(true);
    }

    setVisible(true);
}


void YerothEntrerWindow::handle_HISTORIQUE_CHECKBOX(int aState)
{
    if (!YerothMainWindow::LANGUE_ANGLAISE)
    {
        if (checkBox_HISTORIQUE->isChecked())
        {
            checkBox_HISTORIQUE
                ->setFont(QFont(STRING_APPLICATION_WIDE_FONT_TYPE_YEROTH_ERP_3_0,
                          INT_APPLICATION_WIDE_FONT_SIZE_YEROTH_ERP_3_0));
        }
        else
        {
            checkBox_HISTORIQUE
                ->setFont(QFont(STRING_APPLICATION_WIDE_FONT_TYPE_YEROTH_ERP_3_0, 9));
        }
    }


    if (checkBox_HISTORIQUE->isChecked())
    {
        checkBox_HISTORIQUE->setText(QObject::tr("historique actif"));

        _historique_stock_actif = true;
    }
    else
    {
        checkBox_HISTORIQUE->setText(QObject::tr("historique désac"));

        _historique_stock_actif = false;
    }
}


void YerothEntrerWindow::handle_POURCENTAGE_PRIX_DACHAT_PRIX_DE_VENTE(int aState)
{
    if (checkBox_pourcentage_prix_dachat_prix_de_vente->isChecked())
    {
        lineEdit_prix_vente->setYerothEnabled(false);
        lineEdit_pourcentage_prix_dachat_prix_de_vente->setYerothEnabled(true);
    }
    else
    {
        lineEdit_prix_vente->setYerothEnabled(true);
        lineEdit_pourcentage_prix_dachat_prix_de_vente->setYerothEnabled(false);
    }
}


void YerothEntrerWindow::handle_POURCENTAGE_PRIX_DACHAT_PRIX_DE_VENTE_EN_GROS(int
                                                                              aState)
{
    if (checkBox_pourcentage_prix_dachat_prix_de_vente_en_gros->isChecked())
    {
        lineEdit_prix_vente_en_gros->setYerothEnabled(false);
        lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setYerothEnabled
        (true);
    }
    else
    {
        lineEdit_prix_vente_en_gros->setYerothEnabled(true);
        lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setYerothEnabled
        (false);
    }
}


void YerothEntrerWindow::handle_EXPIRATION_DATE_checkBox(int aState)
{
    if (checkBox_date_peremption->isChecked())
    {
        dateEdit_date_peremption->setVisible(true);
    }
    else
    {
        dateEdit_date_peremption->setVisible(false);
    }
}


void YerothEntrerWindow::handle_achat_checkBox(int aState)
{
    set_achat_checkBox_elements_visibility();

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00");
    lineEdit_pourcentage_prix_dachat_prix_de_vente_en_gros->setText("0.00");

    check_fields_mandatory_buying();
}


bool YerothEntrerWindow::handle_stocks_vendu_table(int stockID,
                                                   ServiceClientInfo &
                                                   aServiceInfo,
                                                   double montant_total_vente,
                                                   double nouveau_compte_client,
                                                   double
                                                   compteClient_PROGRAMME_DE_FIDELITE_CLIENTS)
{
    QString clientName = comboBox_nom_entreprise_fournisseur->currentText();

    int stocksVenduID =
                    YerothERPWindows::getNextIdSqlTableModel_stocks_vendu();

    QString
    referenceRecuVenduCompteClient(YerothUtils::GET_REFERENCE_RECU_VENDU
                                   (QString::number(stocksVenduID)));

    YerothSqlTableModel &stocksTableModel =
                    _allWindows->getSqlTableModel_stocks();

    QString stocksTableFilter =
                    QString("%1 = '%2'").arg(YerothDatabaseTableColumn::ID,
                                             QString::number(stockID));

    stocksTableModel.yerothSetFilter_WITH_where_clause(stocksTableFilter);


    int rows = stocksTableModel.easySelect("src/windows/yeroth-erp-entrer-windows.cpp", 2278);

    if (rows > 0)
    {
        QSqlRecord stockRecord = stocksTableModel.record(0);

        QString
        quantiteQueryStr(QString("SELECT %1 FROM %2 WHERE id = '%3'").arg
                         (YerothDatabaseTableColumn::QUANTITE_TOTALE,
                          YerothDatabase::STOCKS,
                          QString::number(stockID)));
        QSqlQuery quantiteQuery;

        double quantite_actuelle = 0.0;

        int querySize =
                        YerothUtils::execQuery(quantiteQuery, quantiteQueryStr, _logger);

        if (querySize > 0 && quantiteQuery.next())
        {
            quantite_actuelle = quantiteQuery.value(0).toDouble();
        }


        QString historiqueStock = GET_SQL_RECORD_DATA(stockRecord,
                                                      YerothDatabaseTableColumn::HISTORIQUE_STOCK);

        YerothSqlTableModel &stocksVenduTableModel =
                        _allWindows->getSqlTableModel_stocks_vendu();

        QSqlRecord record = stocksVenduTableModel.record();

        int typeDeVente = YerothUtils::VENTE_COMPTE_CLIENT;

        record.setValue(YerothDatabaseTableColumn::TYPE_DE_VENTE, typeDeVente);

        record.setValue(YerothDatabaseTableColumn::IS_VENTE_EN_GROS, false);

        record.setValue(YerothDatabaseTableColumn::ID, stocksVenduID);

        record.setValue(YerothDatabaseTableColumn::REFERENCE,
                        aServiceInfo.reference);

        record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                        aServiceInfo.designation);

        record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                        aServiceInfo.nom_departement_produit);

        record.setValue(YerothDatabaseTableColumn::CATEGORIE,
                        aServiceInfo.nom_categorie);

        record.setValue(YerothDatabaseTableColumn::IS_SERVICE,
                        YerothUtils::MYSQL_TRUE_LITERAL);

        record.setValue(YerothDatabaseTableColumn::REMISE_PRIX, 0.0);

        record.setValue(YerothDatabaseTableColumn::REMISE_POURCENTAGE, 0.0);

        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                        GET_CURRENT_DATE);

        record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_VENDU,
                        referenceRecuVenduCompteClient);

        double quantite_total_vendue = GET_SQL_RECORD_DATA(stockRecord,
                                                           YerothDatabaseTableColumn::QUANTITE_TOTALE).
                                       toDouble();

        record.setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE,
                        quantite_total_vendue);

        record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                        GET_SQL_RECORD_DATA(stockRecord,
                                            YerothDatabaseTableColumn::
                                            PRIX_UNITAIRE).toDouble());

        record.setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                        montant_total_vente);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                        GET_SQL_RECORD_DATA(stockRecord,
                                            YerothDatabaseTableColumn::
                                            MONTANT_TVA).toDouble());

        record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                        GET_SQL_RECORD_DATA(stockRecord,
                                            YerothDatabaseTableColumn::
                                            LOCALISATION_STOCK));

        record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                        GET_SQL_RECORD_DATA(stockRecord,
                                            YerothDatabaseTableColumn::
                                            LOCALISATION));

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                        GET_SQL_RECORD_DATA(stockRecord,
                                            YerothDatabaseTableColumn::
                                            NOM_ENTREPRISE_CLIENT));

        YerothPOSUser *user = _allWindows->getUser();

        record.setValue(YerothDatabaseTableColumn::NOM_CAISSIER,
                        user->nom_complet());
        record.setValue(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
                        user->nom_utilisateur());
        record.setValue(YerothDatabaseTableColumn::DATE_VENTE,
                        GET_CURRENT_DATE);
        record.setValue(YerothDatabaseTableColumn::HEURE_VENTE, CURRENT_TIME);

        record.setValue(YerothDatabaseTableColumn::STOCKS_ID, stockID);

        record.setValue(YerothDatabaseTableColumn::MONTANT_RECU,
                        montant_total_vente);

        double montant_a_rembourser = montant_total_vente;

        if (nouveau_compte_client >= 0.0)
        {
            montant_a_rembourser = 0.0;
        }
        else if (qFabs(nouveau_compte_client) >= montant_total_vente)
        {
            montant_a_rembourser = montant_total_vente;
        }
        else
        {
            montant_a_rembourser = qFabs(nouveau_compte_client);
        }

        record.setValue(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
                        montant_a_rembourser);

        double nouvelle_quantite = quantite_actuelle - quantite_total_vendue;

        QString historiqueStockVendu;

        if (_historique_stock_actif)
        {
            historiqueStockVendu =
            		YerothHistoriqueStock::creer_mouvement_stock(MOUVEMENT_DE_STOCK_VENTE,
                                    							 stocksVenduID,
																 GET_CURRENT_DATE,
																 quantite_actuelle,
																 quantite_total_vendue,
																 nouvelle_quantite);
        }

        historiqueStock.append(QString("%1%2")
        						.arg(YerothHistoriqueStock::SEPARATION_EXTERNE,
        							 historiqueStockVendu));

        record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
        				compteClient_PROGRAMME_DE_FIDELITE_CLIENTS);

        record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT,
                        aServiceInfo.nouveau_compte_client);

        record.setValue(YerothDatabaseTableColumn::CLIENTS_ID,
                        aServiceInfo.ID_client);

        record.setValue(YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
                        aServiceInfo.quartier_entreprise_client);

        record.setValue(YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
                        aServiceInfo.ville_entreprise_client);

        record.setValue(YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT,
                 	 	aServiceInfo.region_etat_entreprise_client);

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                        aServiceInfo.nom_entreprise_client);

        record.setValue(YerothDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                        0.0);

        bool success1 = stocksVenduTableModel.insertNewRecord(record,
        													  this,
															  "src/windows/yeroth-erp-entrer-window.cpp",
															  2452,
															  aServiceInfo.designation);

        if (success1)
        {
            QString
            removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                           (YerothDatabase::STOCKS,
                            YerothDatabaseTableColumn::ID,
                            QString::number(stockID)));

            YerothUtils::execQuery(removeRowQuery);

            if (0 == montant_a_rembourser)
            {
                //copy row from 'stocksVendu' to 'services_completes'
                QString copyRowQuery(QString("DROP TABLE IF EXISTS TempData;"
                                             "CREATE TEMPORARY TABLE TempData LIKE %1;"
                                             "INSERT INTO TempData SELECT * FROM %2 WHERE id = '%3';"
                                             "ALTER TABLE TempData CHANGE COLUMN `id` `id` INT(11) NULL, DROP PRIMARY KEY;"
                                             "UPDATE TempData SET id = %4;"
                                             "INSERT INTO %5 SELECT * FROM TempData;"
                                             "DROP TABLE IF EXISTS TempData;").
                                     arg(YerothDatabase::STOCKS_VENDU,
                                         YerothDatabase::STOCKS_VENDU,
                                         QString::number(stocksVenduID),
                                         QString::number(YerothERPWindows::
                                                         getNextIdSqlTableModel_services_completes
                                                         ()),
                                         YerothDatabase::SERVICES_COMPLETES));

                if (YerothUtils::execQuery(copyRowQuery))
                {
                    //handle stocksVendu table
                    QString
                    removeStocksVenduRowQuery(QString
                                              ("DELETE FROM %1 WHERE %2 = '%3'").
                                              arg(YerothDatabase::
                                                  STOCKS_VENDU,
                                                  YerothDatabaseTableColumn::
                                                  REFERENCE,
                                                  aServiceInfo.reference));

                    if (YerothUtils::execQuery(removeStocksVenduRowQuery))
                    {
                        //handle marchandise table
                        QString
                        removeMarchandisesRowQuery(QString
                                                   ("DELETE FROM %1 WHERE %2 = '%3'").
                                                   arg(YerothDatabase::
                                                       MARCHANDISES,
                                                       YerothDatabaseTableColumn::
                                                       REFERENCE,
                                                       aServiceInfo.
                                                       reference));

                        YerothUtils::execQuery(removeMarchandisesRowQuery);
                    }
                }
            }

            return true;
        }
    }

    return false;
}


bool YerothEntrerWindow::handle_clients_table(int stockID,
                                              double montant_total_vente)
{
    ServiceClientInfo aServiceClientInfo;

    QString clientName = comboBox_nom_entreprise_fournisseur->currentText();

    YerothSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientsTableFilter =
                    QString("%1 = '%2'").arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                             clientName);

    clientsTableModel.yerothSetFilter_WITH_where_clause(clientsTableFilter);

    int rows = clientsTableModel.easySelect("src/windows/yeroth-erp-entrer-windows.cpp", 2536);

    if (rows > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);

        double compteClient_PROGRAMME_DE_FIDELITE_CLIENTS =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerothDatabaseTableColumn::
                                            COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS).
                        toDouble();

        double compteClient = GET_SQL_RECORD_DATA(clientsRecord,
                                                  YerothDatabaseTableColumn::COMPTE_CLIENT).
                              toDouble();

        double nouveau_compte_client = compteClient - montant_total_vente;

        double detteMaximale = GET_SQL_RECORD_DATA(clientsRecord,
                                                   YerothDatabaseTableColumn::
                                                   DETTE_MAXIMALE_COMPTE_CLIENT).toDouble
                               ();

        if (nouveau_compte_client >= 0.0)
        {
            detteMaximale = 0.0;
        }
        else if (qFabs(nouveau_compte_client) >= montant_total_vente)
        {
            detteMaximale = detteMaximale + montant_total_vente;
        }
        else
        {
            detteMaximale = qFabs(nouveau_compte_client);
        }

        clientsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT,
                               nouveau_compte_client);
        clientsRecord.
        setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
                 detteMaximale);

        bool success_clients =
                        clientsTableModel.updateRecord(0,
                        							   clientsRecord,
													   "src/windows/yeroth-erp-entrer-window.cpp",
													   2581);

        aServiceClientInfo.ID_client =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerothDatabaseTableColumn::ID).toInt();

        aServiceClientInfo.reference =
                        comboBox_reference_produit->currentText();
        aServiceClientInfo.designation = lineEdit_designation->text();
        aServiceClientInfo.nom_departement_produit =
                        comboBox_nom_departement_produit->currentText();
        aServiceClientInfo.nom_categorie =
                        comboBox_categorie_produit->currentText();

        aServiceClientInfo.quartier_entreprise_client =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerothDatabaseTableColumn::QUARTIER);

        aServiceClientInfo.ville_entreprise_client =
                        GET_SQL_RECORD_DATA(clientsRecord, YerothDatabaseTableColumn::VILLE);

        aServiceClientInfo.region_etat_entreprise_client =
                        GET_SQL_RECORD_DATA(clientsRecord,
                                            YerothDatabaseTableColumn::PROVINCE_ETAT);

        aServiceClientInfo.nom_entreprise_client = clientName;

        aServiceClientInfo.nouveau_compte_client = nouveau_compte_client;

        clientsTableModel.resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

        //handle 'stocks_vendu' table
        bool success_stocksVendu = false;

        if (success_clients)
        {
            success_stocksVendu = handle_stocks_vendu_table(stockID,
                                                            aServiceClientInfo,
                                                            montant_total_vente,
                                                            nouveau_compte_client,
                                                            compteClient_PROGRAMME_DE_FIDELITE_CLIENTS);
        }

        return success_stocksVendu && true;
    }

    return false;
}


bool YerothEntrerWindow::IS__SERVICE_STOCK_DESIGNATION_REFERENCE__AVAILABLE(enum
                                                                            service_stock_already_exist_type
                                                                            *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT)
{
    *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT = SERVICE_STOCK_UNDEFINED;

    if (!comboBox_reference_produit->isEmpty())
    {
        QString curExistingReferenceDesignation_PRODUCT_in_out;

        *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT =
                        YerothUtils::IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE
                        (comboBox_reference_produit->currentText(),
                         comboBox_categorie_produit->currentText(),
                         lineEdit_designation->text(),
                         curExistingReferenceDesignation_PRODUCT_in_out);

//      QDEBUG_STRING_OUTPUT_2_N("SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT",
//                                                   *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT);

        if (SERVICE_REFERENCE_EXISTS ==
                *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT)
        {
            if (!curExistingReferenceDesignation_PRODUCT_in_out.isEmpty() &&
                    !YerothUtils::isEqualCaseInsensitive(lineEdit_designation->
                                                         text(),
                                                         curExistingReferenceDesignation_PRODUCT_in_out))
            {
                QString infoMesg =
                                QObject::tr("Cette référence ('%1') "
                                                "est déjà utilisée par la marchandise (SERVICE) '%2' "
                                                "dans la catégorie ('%3') !").arg
                                (comboBox_reference_produit->currentText(),
                                 curExistingReferenceDesignation_PRODUCT_in_out,
                                 comboBox_categorie_produit->currentText());

                YerothQMessageBox::warning(this, "enregistrer", infoMesg);

                return false;
            }
        }
        else if (SERVICE_STOCK_DESIGNATION_EXIST ==
                 *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT)
        {
            if (!curExistingReferenceDesignation_PRODUCT_in_out.isEmpty() &&
                    !YerothUtils::isEqualCaseInsensitive(lineEdit_designation->
                                                         text(),
                                                         curExistingReferenceDesignation_PRODUCT_in_out))
            {
                QString infoMesg =
                                QObject::tr
                                ("La marchandise (SERVICE) '%1' utilise déjà la désignation ('%2') "
                                 "dans la catégorie ('%3') !").arg
                                (comboBox_reference_produit->currentText(),
                                 curExistingReferenceDesignation_PRODUCT_in_out,
                                 comboBox_categorie_produit->currentText());

                YerothQMessageBox::warning(this, "enregistrer", infoMesg);

                return false;
            }
        }
        else if (SERVICE_STOCK_CATEGORY_EXIST ==
                 *SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST_IN_OUT)
        {
            if (!curExistingReferenceDesignation_PRODUCT_in_out.isEmpty() &&
                    !YerothUtils::isEqualCaseInsensitive
                    (comboBox_categorie_produit->currentText(),
                     curExistingReferenceDesignation_PRODUCT_in_out))
            {
                QString infoMesg =
                                QObject::tr
                                ("La marchandise (SERVICE) '%1' est déjà dans la catégorie ('%2') !").
                                arg(comboBox_reference_produit->currentText(),
                                    curExistingReferenceDesignation_PRODUCT_in_out);

                YerothQMessageBox::warning(this, "enregistrer", infoMesg);

                return false;
            }
        }
    }

    return true;
}


bool YerothEntrerWindow::executer_enregistrer_une_IMMOBILISATION()
{
    if (!radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
        return false;
    }

    bool result_check_field = check_fields();

    if (!result_check_field)
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this, "IMMOBILISATION",
                                           tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    YerothSqlTableModel &cur_stocks_table_model =
                    _allWindows->getSqlTableModel_stocks();

    bool designation_EXIST = YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                                         cur_stocks_table_model,
                                                                         *lineEdit_designation,
                                                                         QObject::
                                                                         tr
                                                                         ("Une immobilisation"),
                                                                         YerothDatabaseTableColumn::
                                                                         DESIGNATION);

    if (designation_EXIST)
    {
        return false;
    }

    if (checkBox_date_peremption->isChecked())
    {
        if (dateEdit_date_peremption->date() <= GET_CURRENT_DATE)
        {
            QString warnMsg(QObject::tr
                            ("La date de péremption n'est pas postdatée !\n\n"
                             "Continuer avec l'enregistrement des données de l'immobilisation ?"));

            if (QMessageBox::Ok ==
                    YerothQMessageBox::question(this,
                                                QObject::tr
                                                ("enregistrer 1 immobilisation"),
                                                warnMsg, QMessageBox::Cancel,
                                                QMessageBox::Ok))
            {
            }
            else
            {
                return false;
            }
        }
    }


    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;



    QString proposed_nom_departement_produit =
                    comboBox_nom_departement_produit->currentText();

    if (!proposed_nom_departement_produit.isEmpty())
    {
        if (!YerothUtils::creerNouveauDepartementDeProduits
                (proposed_nom_departement_produit, this))
        {
            return false;
        }
    }


    QString proposed_nom_categorie = comboBox_categorie_produit->currentText();

    if (!proposed_nom_categorie.isEmpty())
    {
        if (!YerothUtils::creerNouvelleCategorie(proposed_nom_categorie,
                                                 proposed_nom_departement_produit,
                                                 this))
        {
            return false;
        }
    }


    QString proposed_Fournisseur_Client_Name =
                    comboBox_nom_entreprise_fournisseur->currentText();

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
            if (!YerothUtils::creerNouveauClient
                    (proposed_Fournisseur_Client_Name, this))
            {
                return false;
            }
        }
    }
    else
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
            if (!YerothUtils::creerNouveauFournisseur
                    (proposed_Fournisseur_Client_Name, this))
            {
                return false;
            }
        }
    }

    YerothSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    QSqlRecord achatRecord;

    QSqlRecord record = _curStocksTableModel->record();

    int stock_id_to_save = YerothERPWindows::getNextIdSqlTableModel_stocks();

    if (radioButton_inserer_une_IMMOBILISATION->isChecked() && hasBuying())
    {
        achatRecord = achatSqlTableModel.record();

        int achat_id_to_save =
                        YerothERPWindows::getNextIdSqlTableModel_achats();

        achatRecord.setValue(YerothDatabaseTableColumn::ID, achat_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID,
                             stock_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                             comboBox_reference_produit->currentText());
        achatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                             lineEdit_designation->text());
        achatRecord.
        setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 comboBox_nom_departement_produit->currentText());
        achatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE,
                             comboBox_categorie_produit->currentText());
    }

    record.setValue(YerothDatabaseTableColumn::IS_SERVICE, 0);
    record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);
    record.setValue(YerothDatabaseTableColumn::REFERENCE,
                    comboBox_reference_produit->currentText());
    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    lineEdit_designation->text());
    record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                    comboBox_nom_departement_produit->currentText());
    record.setValue(YerothDatabaseTableColumn::CATEGORIE,
                    comboBox_categorie_produit->currentText());
    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
                    textEdit_description->toPlainText());
    record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT,
                    doubleSpinBox_lots_entrant->value());
    record.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT,
                    lineEdit_quantite_par_lot->text().toDouble());

    double quantite_totale = lineEdit_quantite_totale->text().toDouble();

    double stock_dalerte = lineEdit_stock_dalerte->text().toDouble();

    QString reference_recu_dachat = lineEdit_reference_recu_dachat->text();

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (prix_dachat < 0)
    {
        prix_dachat = 0;
    }

    double montant_total_service_vente = quantite_totale * prix_vente;

    QString utilisateurCourrantNomComplet;

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourrantNomComplet.append(aUser->nom_complet());
    }

    if (hasBuying())
    {
        achatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                             utilisateurCourrantNomComplet);
        achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                             quantite_totale);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                             reference_recu_dachat);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT,
                             prix_dachat);

        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                             0.0);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, 0.0);

        //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
        achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, 0.0);
        achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                             0.0);
    }


    record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                    reference_recu_dachat);

    record.setValue(YerothDatabaseTableColumn::STOCK_DALERTE, stock_dalerte);

    record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);

    record.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                    utilisateurCourrantNomComplet);

    record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                    quantite_totale);


    double prix_unitaire_ht = 0.0;

    double prix_unitaire_en_gros_ht = 0.0;

    double marge_beneficiaire = 0.0;

    double marge_beneficiaire_en_gros = 0.0;

    record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, 0.0);

    record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS, 0.0);

    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, 0.0);

    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS, 0.0);


    if (hasBuying())
    {
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                             prix_unitaire_ht);

        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                             prix_unitaire_en_gros_ht);

        achatRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 	 	 	 proposed_Fournisseur_Client_Name);

        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                             YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                             lineEdit_localisation_produit->text());

        achatRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE,
                             GET_CURRENT_DATE);

        if (checkBox_date_peremption->isChecked())
        {
            achatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                                 dateEdit_date_peremption->date());
        }
    }

    record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                    lineEdit_localisation_produit->text());

    if (checkBox_date_peremption->isChecked())
    {
        record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                        dateEdit_date_peremption->date());
    }

    record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                    proposed_Fournisseur_Client_Name);

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                    prix_unitaire_ht);

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                    prix_unitaire_en_gros_ht);

    record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                    YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

    record.setValue(YerothDatabaseTableColumn::DATE_ENTREE,
    				GET_CURRENT_DATE);


    QString historiqueStockInitial;

    if (_historique_stock_actif)
    {
        historiqueStockInitial =
                        YerothHistoriqueStock::creer_mouvement_stock
                        (MOUVEMENT_DE_STOCK_ENTREE, stock_id_to_save, GET_CURRENT_DATE, 0.0,
                         quantite_totale, quantite_totale);
    }

    //qDebug() << QString("++ test: %1")
    //                              .arg(historiqueStockInitial);

    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                    _historique_stock_actif);

    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                    historiqueStockInitial);

    if (label_image_produit->pixmap())
    {
        QByteArray bytes;
        YerothUtils::savePixmapToByteArray(bytes,
                                           *label_image_produit->pixmap(),
                                           "JPG");
        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT,
                        QVariant::fromValue(bytes));
    }

    bool achatSuccess = false;

    if (hasBuying())
    {
        achatSuccess = achatSqlTableModel
        					.insertNewRecord(achatRecord,
        									 this,
											 "src/windows/yeroth-erp-entrer-window.cpp",
											 3058,
											 lineEdit_designation->text());
    }

    bool successInsertStock =
    		_curStocksTableModel
				->insertNewRecord(record,
								  this,
								  "src/windows/yeroth-erp-entrer-window.cpp",
								  3066,
								  lineEdit_designation->text());


    QString achatRetMsg;

    achatRetMsg.append(QObject::tr("L'achat de l'immobilisation '%1'")
    					.arg(lineEdit_designation->text()));


    if (hasBuying())
    {
        if (achatSuccess)
        {
            achatRetMsg.append(QObject::tr
                               (" a été enregistré dans la base de données !"));

            YerothQMessageBox::information(this,
                                           QObject::tr("enregistrement - succès"),
                                           achatRetMsg);
        }
        else
        {
            achatRetMsg.append(QObject::tr(" n'a pas pu être enregistré dans la base de données !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr("enregistrement - échec"),
                                       achatRetMsg);
        }
    }

    QString retMsg;

    retMsg.append(QObject::tr("L'immobilisation '%1'")
                  	  .arg(lineEdit_designation->text()));


    if (successInsertStock)
    {
        retMsg.append(QObject::tr
                      (" a été enregistré dans la base de données !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("enregistrement - succès"), retMsg);
    }
    else
    {
        retMsg.append(QObject::tr
                      (" n'a pas pu être enregistré dans la base de données !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("enregistrement - échec"),
                                   retMsg);
    }


    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    rendreInvisible();

    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel, true);

    return true;
}



bool YerothEntrerWindow::executer_enregistrer_une_charge_financiere()
{
    if (!radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked())
    {
        return false;
    }

    bool result_check_field = check_fields_service_achat();

    if (!result_check_field)
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this, QObject::tr("achat"),
                                           tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }


    enum service_stock_already_exist_type SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST;

    if (!IS__SERVICE_STOCK_DESIGNATION_REFERENCE__AVAILABLE(&SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST))
    {
        return false;
    }



    QString proposed_nom_departement_produit =
    			comboBox_nom_departement_produit->currentText();

    if (!proposed_nom_departement_produit.isEmpty())
    {
        if (!YerothUtils::creerNouveauDepartementDeProduits(proposed_nom_departement_produit, this))
        {
            return false;
        }
    }


    YerothERPServiceData a_service_achat_au_fournisseur_data;

    a_service_achat_au_fournisseur_data._nom_departement_produit =
                    comboBox_nom_departement_produit->currentText();
    a_service_achat_au_fournisseur_data._categorie =
                    comboBox_categorie_produit->currentText();
    a_service_achat_au_fournisseur_data._description =
                    textEdit_description->toPlainText();
    a_service_achat_au_fournisseur_data._designation =
                    lineEdit_designation->text();
    a_service_achat_au_fournisseur_data._reference =
                    comboBox_reference_produit->currentText();

    QString proposed_Categorie_Name =
                    comboBox_categorie_produit->currentText();

    if (!proposed_Categorie_Name.isEmpty())
    {
        if (!YerothUtils::creerNouvelleCategorie
                (a_service_achat_au_fournisseur_data._categorie,
                 a_service_achat_au_fournisseur_data._nom_departement_produit))
        {
            return false;
        }
    }

    QString proposed_Fournisseur_Client_Name =
                    comboBox_nom_entreprise_fournisseur->currentText();

    if (!proposed_Fournisseur_Client_Name.isEmpty())
    {
        if (!YerothUtils::creerNouveauFournisseur
                (proposed_Fournisseur_Client_Name, this))
        {
            return false;
        }
    }


    double quantite_totale = doubleSpinBox_lots_entrant->value();

    double prix_unitaire = -1 * lineEdit_prix_vente->text().toDouble();

    double prix_dachat_du_service = quantite_totale * prix_unitaire;

    double prix_dachat_du_service_POSITIF = -1.0 * prix_dachat_du_service;

    QString utilisateurCourantNomComplet;

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourantNomComplet.append(aUser->nom_complet());
    }

    bool successAchatDeServiceInsert = false;


	YerothSqlTableModel &LIGNES_BUDGETAIRES_SqlTableModel =
			_allWindows->getSqlTableModel_lignes_budgetaires();

	QSqlRecord LIGNES_BUDGETAIRES_query_RECORD;

	double montant_de_la_ligne_budgetaire_RESTANTE = 0.0;

    {
    	/*
    	 * VÉRIFIONS SI CETTE LIGNE BUGÉTAIRE DISPOSE D'ASSEZ
    	 * D'ARGENT pour cet achat !
    	 */

        QString yeroth_LIGNES_BUDGETAIRES_SqlTableModelFilter =
        		QString("%1 = '%2'")
					.arg(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
						 proposed_Categorie_Name);

    	LIGNES_BUDGETAIRES_SqlTableModel
			.yerothSetFilter_WITH_where_clause
				(yeroth_LIGNES_BUDGETAIRES_SqlTableModelFilter);

    	QString retMsg_ECHEC_inserer_charge_financiere;

    	int rows =
    			LIGNES_BUDGETAIRES_SqlTableModel
					.easySelect("src/windows/yeroth-erp-entrer-windows.cpp", 3247);

    	if (rows <= 0)
    	{
    		retMsg_ECHEC_inserer_charge_financiere =
    				QObject::tr("LA LIGNE BUDGÉTAIRE '%1' est inexistante !")
						.arg(proposed_Categorie_Name);

            YerothQMessageBox::warning(this,
                                       QObject::tr("enregistrement ACHAT SUR LIGNE BUDGÉTAIRE (1) - échec"),
									   retMsg_ECHEC_inserer_charge_financiere);

            LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

            return false;
    	}


    	LIGNES_BUDGETAIRES_query_RECORD = LIGNES_BUDGETAIRES_SqlTableModel.record(0);

    	montant_de_la_ligne_budgetaire_RESTANTE =
    			GET_SQL_RECORD_DATA(LIGNES_BUDGETAIRES_query_RECORD,
    								YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire).toDouble();

    	if (montant_de_la_ligne_budgetaire_RESTANTE < prix_dachat_du_service_POSITIF)
    	{
    		retMsg_ECHEC_inserer_charge_financiere =
    				QObject::tr("LA LIGNE BUDGÉTAIRE '%1' DÉTIENT UN \n"
    								"MONTANT INSUFFISANT: ('%2') (requis pour \n"
    								"la charge financière !)!")
						.arg(proposed_Categorie_Name,
							 QString::number(montant_de_la_ligne_budgetaire_RESTANTE));

            YerothQMessageBox::warning(this,
                                       QObject::tr("enregistrement ACHAT SUR LIGNE BUDGÉTAIRE (2) - échec"),
									   retMsg_ECHEC_inserer_charge_financiere);

            LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

            return false;
    	}
    }

    {
        YerothSqlTableModel &charges_financieresSqlTableModel =
                        _allWindows->getSqlTableModel_charges_financieres();

        QSqlRecord achatRecord = charges_financieresSqlTableModel.record();

        int achat_de_service_id_to_save =
                        YerothERPWindows::getNextIdSqlTableModel_charges_financieres();

        achatRecord.setValue(YerothDatabaseTableColumn::ID,
                             achat_de_service_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                             comboBox_reference_produit->currentText());
        achatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                             lineEdit_designation->text());
        achatRecord.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                             a_service_achat_au_fournisseur_data.
                             _nom_departement_produit);
        achatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE,
                             a_service_achat_au_fournisseur_data._categorie);

        achatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                             utilisateurCourantNomComplet);
        achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                             quantite_totale);
        achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                             _montantTva);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                             prix_unitaire);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT,
                             prix_dachat_du_service);

        achatRecord.
        setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 proposed_Fournisseur_Client_Name);
        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                             YerothERPConfig::THIS_SITE_LOCALISATION_NAME);
        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                             lineEdit_localisation_produit->text());
        achatRecord.setValue(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                             GET_CURRENT_DATE);

        if (!YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
        												 charges_financieresSqlTableModel,
														 *lineEdit_designation,
														 QObject::tr("Désignation"),
														 YerothDatabaseTableColumn::DESIGNATION)  &&
        	!YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
        		            						     charges_financieresSqlTableModel,
        												 *(comboBox_reference_produit->lineEdit()),
        												 QObject::tr("Référence"),
        												 YerothDatabaseTableColumn::REFERENCE)	)
        {
            successAchatDeServiceInsert =
            		charges_financieresSqlTableModel
    					.insertNewRecord(achatRecord,
    									 this,
    									 "src/windows/yeroth-erp-entrer-window.cpp",
    									 3346,
    									 lineEdit_designation->text());
        }
    }

    bool successPaiementsInsert = false;

    {
        YerothERPPaymentProcessingInformation payment_processing_info;

        payment_processing_info._nom_entreprise =
                        proposed_Fournisseur_Client_Name;

        payment_processing_info._type_de_paiement =
                        YerothUtils::DECAISSEMENT_COMPTANT;

        payment_processing_info._montant_paye = prix_dachat_du_service;

        payment_processing_info._designation =
        		a_service_achat_au_fournisseur_data._designation;

        payment_processing_info._paiement_intitule_compte_bancaire =
        		GET_SQL_RECORD_DATA(LIGNES_BUDGETAIRES_query_RECORD,
        							YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

        payment_processing_info._reference =
        		a_service_achat_au_fournisseur_data._reference;

        bool is_supplier_payment = true;

        successPaiementsInsert =
                        payment_processing_info.save_payment_info_record(is_supplier_payment);
    }


    if (!successAchatDeServiceInsert || !successPaiementsInsert)
    {
    	LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

        return false;
    }


    /*
     * UPDATING TABLE lignes_budgetaires WITH A NEW REMAINING
     * AMOUNT for the current modified budget line !
     */
    double NOUVEAU_MONTANT_montant_de_la_ligne_budgetaire_RESTANTE =
    		montant_de_la_ligne_budgetaire_RESTANTE - prix_dachat_du_service_POSITIF;

    LIGNES_BUDGETAIRES_query_RECORD
		.setValue(YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
				  NOUVEAU_MONTANT_montant_de_la_ligne_budgetaire_RESTANTE);

    LIGNES_BUDGETAIRES_SqlTableModel.updateRecord(0,
    											  LIGNES_BUDGETAIRES_query_RECORD,
												  "src/windows/yeroth-erp-entrer-window.cpp",
												  3406);


    QString achatRetMsg(QObject::tr
                        ("La charge financière '%1' au fournisseur '%2'").arg
                        (lineEdit_designation->text(),
                         proposed_Fournisseur_Client_Name));

    if (successAchatDeServiceInsert)
    {
        achatRetMsg.append(QObject::tr
                           (" a été enregistrée dans la base de données !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("ajout d'une charge financière - succès"),
                                       achatRetMsg);
    }
    else
    {
        achatRetMsg.append(QObject::tr
                           (" n'a pas pu être enregistrée dans la base de données !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("ajout d'1 charge financière - échec"),
                                   achatRetMsg);
    }


    QString paiementsRetMsg(QObject::tr
                            ("La charge financière '%1' au fournisseur '%2'").
                            arg(lineEdit_designation->text(),
                                proposed_Fournisseur_Client_Name));

    if (successPaiementsInsert)
    {
        paiementsRetMsg.append(QObject::tr
                               (" a été répertoriée dans le tableau des paiements !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("ajout d'une charge financière - répertorié - succès"),
                                       paiementsRetMsg);
    }
    else
    {
        paiementsRetMsg.append(QObject::tr
                               (" n'a pas pu être répertoriée dans le tableau des paiements !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("ajout d'une charge financière - répertorié - échec"),
                                   paiementsRetMsg);
    }

    LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/windows/yeroth-erp-entrer-window.cpp");

    rendreInvisible();

    _allWindows->_charges_financieresWindow->rendreVisible(_curStocksTableModel);

    return true;
}


void YerothEntrerWindow::enregistrer_produit()
{
    bool result_check_field = false;

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        result_check_field = check_fields_service_vente();
    }
    else
    {
        result_check_field = check_fields();
    }

    if (!result_check_field)
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this, "stock (service)",
                                           tr
                                           ("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return;
    }


    enum service_stock_already_exist_type SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST;

    if (!IS__SERVICE_STOCK_DESIGNATION_REFERENCE__AVAILABLE
            (&SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST))
    {
        return;
    }


    double prix_vente_en_gros = 0.0;

    if (radioButton_inserer_un_stock->isChecked())
    {
        if (!YerothUtils::isProfitable
                (lineEdit_prix_vente->text().toDouble(),
                 lineEdit_prix_dachat->text().toDouble(), _montantTva))
        {
            QString warnMsg(QObject::tr
                            ("Le prix de vente doit être supérieur ou égal au prix d'achat !"));

            YerothQMessageBox::warning(this, QObject::tr("pas profitable"),
                                       warnMsg);

            return;
        }

        prix_vente_en_gros = get_prix_vente_en_gros();

        if (!YerothUtils::isProfitable(prix_vente_en_gros,
                                       lineEdit_prix_dachat->text().
                                       toDouble(), _montantTva_en_gros))
        {
            QString warnMsg(QObject::tr
                            ("Le prix de vente (en gros) doit être supérieur ou égal au prix d'achat !"));

            YerothQMessageBox::warning(this, QObject::tr("pas profitable"),
                                       warnMsg);

            return;
        }
    }

    bool CURRENTLY_checking_expiration_date =
                    (radioButton_inserer_une_IMMOBILISATION->isChecked() ||
                     radioButton_inserer_un_stock->isChecked());

    if (checkBox_date_peremption->isChecked())
    {
        if (CURRENTLY_checking_expiration_date &&
                dateEdit_date_peremption->date() <= GET_CURRENT_DATE)
        {
            QString warnMsg(QObject::tr
                            ("La date de péremption n'est pas postdatée !\n\n"
                             "Continuer avec l'enregistrement des données du stock (service) ?"));

            if (QMessageBox::Ok ==
                    YerothQMessageBox::question(this,
                                                QObject::tr
                                                ("enregistrer un stock"), warnMsg,
                                                QMessageBox::Cancel,
                                                QMessageBox::Ok))
            {
            }
            else
            {
                return;
            }
        }
    }


    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    YerothERPStockMarchandiseData a_stock_data;

    a_stock_data._isService =
                    radioButton_service_vente_de_service_au_client->isChecked() ||
                    radioButton_INSERER_UNE_CHARGE_FINANCIERE->isChecked();

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        a_stock_data._nom_entreprise_client =
                        comboBox_nom_entreprise_fournisseur->currentText();
    }

    a_stock_data._nom_departement_produit =
                    comboBox_nom_departement_produit->currentText();
    a_stock_data._categorie = comboBox_categorie_produit->currentText();
    a_stock_data._description = textEdit_description->toPlainText();
    a_stock_data._designation = lineEdit_designation->text();

    if (checkBox_achat->isChecked())
    {
        a_stock_data._prix_dachat_precedent = lineEdit_prix_dachat->text();
    }

    a_stock_data._prix_vente_precedent = lineEdit_prix_vente->text();

    if (!lineEdit_prix_vente_en_gros->isEmpty())
    {
        a_stock_data._prix_vente_en_gros_precedent =
                        lineEdit_prix_vente_en_gros->text();
    }
    else
    {
        a_stock_data._prix_vente_en_gros_precedent =
                        lineEdit_prix_vente->text();
    }

    a_stock_data._reference = comboBox_reference_produit->currentText();


    QString proposed_nom_departement_produit =
                    comboBox_nom_departement_produit->currentText();

    if (!proposed_nom_departement_produit.isEmpty())
    {
        if (!YerothUtils::creerNouveauDepartementDeProduits
                (proposed_nom_departement_produit, this))
        {
            return;
        }
    }


    if (!a_stock_data._categorie.isEmpty())
    {
        if (!YerothUtils::creerNouvelleCategorie(a_stock_data._categorie,
                                                 proposed_nom_departement_produit,
                                                 this))
        {
            return;
        }
    }


    if (SERVICE_STOCK_UNDEFINED == SERVICE_REFERENCE_STOCK_DESIGNATION_EXIST)
    {
        if (!radioButton_inserer_une_IMMOBILISATION->isChecked())
        {
            if (!YerothUtils::insertStockItemInProductList(a_stock_data))
            {
                return;
            }
        }
    }

    QString proposed_Fournisseur_Client_Name =
                    comboBox_nom_entreprise_fournisseur->currentText();

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
            if (!YerothUtils::creerNouveauClient
                    (proposed_Fournisseur_Client_Name, this))
            {
                return;
            }
        }
    }
    else
    {
        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
            if (!YerothUtils::creerNouveauFournisseur
                    (proposed_Fournisseur_Client_Name, this))
            {
                return;
            }
        }
    }

    YerothSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    QSqlRecord achatRecord;

    QSqlRecord record = _curStocksTableModel->record();

    int stock_id_to_save = YerothERPWindows::getNextIdSqlTableModel_stocks();

    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && hasBuying())
    {
        achatRecord = achatSqlTableModel.record();

        int achat_id_to_save =
                        YerothERPWindows::getNextIdSqlTableModel_achats();

        achatRecord.setValue(YerothDatabaseTableColumn::ID, achat_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID,
                             stock_id_to_save);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                             comboBox_reference_produit->currentText());
        achatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                             lineEdit_designation->text());
        achatRecord.
        setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 a_stock_data._nom_departement_produit);
        achatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE,
                             a_stock_data._categorie);
    }

    record.setValue(YerothDatabaseTableColumn::IS_SERVICE,
                    a_stock_data._isService);
    record.setValue(YerothDatabaseTableColumn::ID, stock_id_to_save);
    record.setValue(YerothDatabaseTableColumn::REFERENCE,
                    comboBox_reference_produit->currentText());
    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    lineEdit_designation->text());
    record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                    a_stock_data._nom_departement_produit);
    record.setValue(YerothDatabaseTableColumn::CATEGORIE,
                    a_stock_data._categorie);
    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
                    textEdit_description->toPlainText());
    record.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT,
                    doubleSpinBox_lots_entrant->value());
    record.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT,
                    lineEdit_quantite_par_lot->text().toDouble());

    double quantite_totale = lineEdit_quantite_totale->text().toDouble();

    double stock_dalerte = lineEdit_stock_dalerte->text().toDouble();

    QString reference_recu_dachat = lineEdit_reference_recu_dachat->text();

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (prix_dachat < 0)
    {
        prix_dachat = 0;
    }

    double montant_total_service_vente = quantite_totale * prix_vente;

    QString utilisateurCourrantNomComplet;

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourrantNomComplet.append(aUser->nom_complet());
    }

    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && hasBuying())
    {
        achatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                             utilisateurCourrantNomComplet);
        achatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                             quantite_totale);
        achatRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                             reference_recu_dachat);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT,
                             prix_dachat);

        if (!radioButton_inserer_une_IMMOBILISATION->isChecked())
        {
            achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                 prix_vente_en_gros);
            achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE,
                                 prix_vente);

            //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
            achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                                 _montantTva);
            achatRecord.
            setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                     _montantTva_en_gros);
        }
        else
        {
            achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                 0.0);
            achatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, 0.0);

            //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);
            achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, 0.0);
            achatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS, 0.0);
        }
    }

    if (!radioButton_service_vente_de_service_au_client->isChecked())
    {
        record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                        reference_recu_dachat);
        record.setValue(YerothDatabaseTableColumn::STOCK_DALERTE,
                        stock_dalerte);

        YerothUtils::UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList(a_stock_data,
                                                                 this);

        record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);
    }

    record.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                    utilisateurCourrantNomComplet);
    record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                    quantite_totale);


    double prix_unitaire_ht = 0.0;

    double prix_unitaire_en_gros_ht = 0.0;

    double marge_beneficiaire = 0.0;

    double marge_beneficiaire_en_gros = 0.0;

    if (radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, 0.0);

        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS, 0.0);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, 0.0);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS, 0.0);
    }
    else
    {
        YerothUtils::UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList(a_stock_data,
                                                                  this);

        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);

        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                        prix_vente_en_gros);
        //qDebug() << "++_tva: " << QString::number(_tva, 'f', 2);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

        record.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                        _montantTva_en_gros);

        prix_unitaire_ht = prix_vente - _montantTva;

        prix_unitaire_en_gros_ht = prix_vente_en_gros - _montantTva_en_gros;

        marge_beneficiaire =
                        YerothUtils::getMargeBeneficiaire(prix_vente, prix_dachat,
                                                          _montantTva);

        marge_beneficiaire_en_gros =
                        YerothUtils::getMargeBeneficiaire(prix_vente_en_gros, prix_dachat,
                                                          _montantTva_en_gros);
    }


    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && hasBuying())
    {
        if (!radioButton_inserer_une_IMMOBILISATION->isChecked())
        {
            achatRecord.setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
                                 marge_beneficiaire);
            achatRecord.
            setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE_EN_GROS,
                     marge_beneficiaire_en_gros);
        }

        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                             prix_unitaire_ht);
        achatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                             prix_unitaire_en_gros_ht);

        achatRecord.
        setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 proposed_Fournisseur_Client_Name);

        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                             YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

        achatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                             lineEdit_localisation_produit->text());

        achatRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE,
                             GET_CURRENT_DATE);

        if (checkBox_date_peremption->isChecked())
        {
            achatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                                 dateEdit_date_peremption->date());
        }
    }

    if (radioButton_inserer_un_stock->isChecked())
    {
        record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                        lineEdit_localisation_produit->text());

        if (checkBox_date_peremption->isChecked())
        {
            record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                            dateEdit_date_peremption->date());
        }

        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                        proposed_Fournisseur_Client_Name);
    }
    else
    {
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
                        proposed_Fournisseur_Client_Name);
    }

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                    prix_unitaire_ht);
    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                    prix_unitaire_en_gros_ht);
    record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                    YerothERPConfig::THIS_SITE_LOCALISATION_NAME);
    record.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);


    QString historiqueStockInitial;

    if (_historique_stock_actif)
    {
        historiqueStockInitial =
                        YerothHistoriqueStock::creer_mouvement_stock
                        (MOUVEMENT_DE_STOCK_ENTREE, stock_id_to_save, GET_CURRENT_DATE, 0.0,
                         quantite_totale, quantite_totale);
    }

    //qDebug() << QString("++ test: %1")
    //                              .arg(historiqueStockInitial);

    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                    _historique_stock_actif);

    record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                    historiqueStockInitial);

    if (label_image_produit->pixmap())
    {
        QByteArray bytes;
        YerothUtils::savePixmapToByteArray(bytes,
                                           *label_image_produit->pixmap(),
                                           "JPG");
        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT,
                        QVariant::fromValue(bytes));
    }

    bool achatSuccess = false;

    if (hasBuying())
    {
        if (radioButton_inserer_une_IMMOBILISATION->isChecked() ||
                radioButton_inserer_un_stock->isChecked())
        {
            achatSuccess = achatSqlTableModel
            				.insertNewRecord(achatRecord,
            								 this,
											 "src/windows/yeroth-erp-entrer-window.cpp",
											 3969,
											 lineEdit_designation->text());
        }
    }


    bool successInsertStock =
    		_curStocksTableModel->insertNewRecord(record,
    											  this,
												  "src/windows/yeroth-erp-entrer-window.cpp",
												  3979,
												  lineEdit_designation->text());


    QString achatRetMsg;

    if (!radioButton_inserer_une_IMMOBILISATION->isChecked())
    {
        achatRetMsg.append(QObject::tr("L'achat du stock '%1'").
                           arg(lineEdit_designation->text()));
    }
    else
    {
        achatRetMsg.append(QObject::tr("L'achat de l'immobilisation '%1'").arg
                           (lineEdit_designation->text()));
    }


    if (!radioButton_service_vente_de_service_au_client->isChecked()
            && hasBuying())
    {
        if (achatSuccess)
        {
            achatRetMsg.append(QObject::tr
                               (" a été enregistré dans la base de données !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("enregistrement - succès"),
                                           achatRetMsg);
        }
        else
        {
            achatRetMsg.append(QObject::tr
                               (" n'a pas pu être enregistré dans la base de données !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("enregistrement - échec"),
                                       achatRetMsg);
        }
    }

    QString retMsg;

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        retMsg.append(QObject::tr("Le service '%1'").
                      arg(comboBox_reference_produit->currentText()));
    }
    else
    {
        retMsg.append(QObject::tr("Le stock '%1'").
                      arg(lineEdit_designation->text()));
    }

    if (successInsertStock
            && radioButton_service_vente_de_service_au_client->isChecked())
    {
        //handle 'clients' table
        handle_clients_table(stock_id_to_save, montant_total_service_vente);
    }

    if (successInsertStock)
    {
        retMsg.append(QObject::tr
                      (" a été enregistré dans la base de données !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("enregistrement - succès"), retMsg);
    }
    else
    {
        retMsg.append(QObject::tr
                      (" n'a pas pu être enregistré dans la base de données !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("enregistrement - échec"),
                                   retMsg);
    }


    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    rendreInvisible();

    if (radioButton_service_vente_de_service_au_client->isChecked())
    {
        _allWindows->_ventesWindow->rendreVisible(_curStocksTableModel);
    }
    else
    {
        if (radioButton_inserer_une_IMMOBILISATION->isChecked())
        {
            _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel,
                                                      true);
        }
        else
        {
            _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);
        }
    }
}
