/*
   * yeroth-erp-ventes-window.cpp
   *
   *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
   */
#include "yeroth-erp-ventes-window.hpp"


#include "src/process/yeroth-erp-process.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/windows/stocks/yeroth-erp-stock-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <QtWidgets/QCalendarWidget>

#include <QtGui/QContextMenuEvent>

#include <QtCore/QThread>

#include <QtCore/QDir>

#include <QtCore/qmath.h>

#include <QtCore/QFile>

#include <QtCore/QProcessEnvironment>


#include <unistd.h>


YerothVentesWindow::YerothVentesWindow()
:YerothWindowsCommons("yeroth-erp-journal-ventes"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::STOCKS_VENDU),
 _retourVenteTabWidget(0),
 _logger(new YerothLogger("YerothVentesWindow")),
 _pushButton_ventes_filtrer_font(0),
 _ventesDateFilter(YerothUtils::EMPTY_STRING),
 _curStocksVenduTableModel(&_allWindows->getSqlTableModel_stocks_vendu())
{
    setYerothSqlTableModel(_curStocksVenduTableModel);

    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("ventes"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_ventes);


    _ACTIONS_TO_DISABLE_WHEN_NOT_PRINT_ACTION
            << action_parametrer_les_impressions
            << actionExporter_au_format_csv
            << actionAfficher_lhistorique_de_ce_stock
			<< actionAfficherPDF;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionAfficher_lhistorique_de_ce_stock
        << actionAnnulerCetteVente
		<< actionAfficherVenteDetail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_ventes);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}").arg
                    (COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _retourVenteTabWidgetTitle = tabWidget_ventes->tabText(RetourDuneVente);

    _retourVenteTabWidget = tabWidget_ventes->widget(RetourDuneVente);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF
            << YerothDatabaseTableColumn::HISTORIQUE_STOCK;


    setup_select_configure_dbcolumn(YerothDatabase::STOCKS_VENDU);


    _lineEditsToANDContentForSearch.insert(&lineEdit_ventes_terme_recherche,
                                           YerothUtils::EMPTY_STRING);

    _lineEditsToANDContentForSearch.insert(&lineEdit_ventes_nom_caissier,
                                           YerothDatabaseTableColumn::
                                           NOM_CAISSIER);

    _yeroth_WINDOW_references_dbColumnString
        .insert(YerothDatabaseTableColumn::REFERENCE);

    _comboBoxesToANDContentForSearch.insert(&comboBox_ventes_type_de_vente,
                                            YerothDatabaseTableColumn::TYPE_DE_VENTE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION
    (YerothDatabase::STOCKS_VENDU);

    reinitialiser_colones_db_visibles();

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    tableView_ventes->setSqlTableName(&YerothDatabase::STOCKS_VENDU);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    _pushButton_ventes_filtrer_font =
                    new QFont(pushButton_ventes_filtrer->font());

    pushButton_ventes_filtrer->disable(this);
    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));


    connect(actionReinitialiserChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible, SIGNAL(triggered()), this,
            SLOT(selectionner_champs_db_visibles()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    connect(actionAfficher_lhistorique_de_ce_stock, SIGNAL(triggered()),
            this, SLOT(private_slot_afficher_historique_du_stock()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionAnnulerCetteVente, SIGNAL(triggered()), this,
            SLOT(annuler_cette_vente()));
    connect(actionAfficherVenteDetail, SIGNAL(triggered()), this,
            SLOT(afficher_vente_detail()));
    connect(action_regenerer_recu_client_vente, SIGNAL(triggered()), this,
            SLOT(slot_regenerer_recu_client_vente()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    connect(tabWidget_ventes, SIGNAL(currentChanged(int)), this,
            SLOT(handleCurrentTabChanged(int)));

    connect(tableView_ventes, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_vente_detail()));

    connect(tableView_ventes, SIGNAL(activated(const QModelIndex &)), this,
            SLOT(afficher_vente_detail()));

    connect(tableView_ventes->getStandardItemModel(),
            SIGNAL(rowsRemoved(const QModelIndex &, int, int)),
            this, SLOT(modifier_visibilite_actions_sur_cette_vente()));

    connect(tableView_ventes->getStandardItemModel(),
            SIGNAL(rowsInserted(const QModelIndex &, int, int)),
            this, SLOT(modifier_visibilite_actions_sur_cette_vente()));

    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING
    (YerothDatabaseTableColumn::TYPE_DE_VENTE);

    setupShortcuts();

    comboBox_retour_vente_nom_client->setYerothEditable(true);
}


YerothVentesWindow::~YerothVentesWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerothVentesWindow::initialiser_FILTRES()
{
    comboBox_ventes_element_de_vente->setVisible(false);
    comboBox_ventes_element_de_vente_condition->setVisible(false);
    lineEdit_ventes_element_de_vente_resultat->setVisible(false);
    pushButton_ventes_reinitialiser_filtre->setVisible(false);
    pushButton_ventes_filtrer->setVisible(false);
}


void YerothVentesWindow::private_slot_afficher_historique_du_stock()
{
    //qDebug() << QString("lastSelectedRow__ID: %1")
    //                              .arg(QString::number(lastSelectedRow__ID));

    if (0 != _curStocksTableModel && _curStocksTableModel->rowCount() > 0)
    {
        QSqlRecord record;

        _allWindows->
        _ventesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        QString
        historiqueStockSelectionne(GET_SQL_RECORD_DATA
                                   (record,
                                    YerothDatabaseTableColumn::
                                    HISTORIQUE_STOCK));


        QString
        stockReference(GET_SQL_RECORD_DATA
                       (record, YerothDatabaseTableColumn::REFERENCE));

        QString
        stockDesignation(GET_SQL_RECORD_DATA
                         (record, YerothDatabaseTableColumn::DESIGNATION));

        bool show_asset = YerothUtils::IS_QSQL_RECORD_AN_ASSET(record);

//          qDebug() << QString("++ historiqueStockSelectionne: %1")
//                                      .arg(historiqueStockSelectionne);

        YerothHistoriqueStock::listHistoriqueDuStock
        (historiqueStockSelectionne, stockReference, stockDesignation,
         show_asset);
    }
    else
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("historique du stock"),
                                       QObject::tr
                                       ("Il n'y a pas de stocks listés !"));
    }
}


void YerothVentesWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_ventes_element_de_vente->setVisible(IS_VISIBLE);
    comboBox_ventes_element_de_vente_condition->setVisible(IS_VISIBLE);
    lineEdit_ventes_element_de_vente_resultat->setVisible(IS_VISIBLE);
    pushButton_ventes_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_ventes_filtrer->setVisible(IS_VISIBLE);
}


void YerothVentesWindow::slot_regenerer_recu_client_vente()
{
    int ventesTableViewRowCount = tableView_ventes->rowCount();

    if (ventesTableViewRowCount <= 0)
    {
        return;
    }

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord curStocksVenduRecord;

    _allWindows->_ventesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (curStocksVenduRecord);

    QString reference_recu_vendu = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                       YerothDatabaseTableColumn::
                                                       REFERENCE_RECU_VENDU);


    QString SELECT_ALL_SOLD_ITEMS_FOR_THIS_SALE =
                    QString("select * from %1 WHERE %2='%3'").
                    arg(YerothDatabase::STOCKS_VENDU,
                        YerothDatabaseTableColumn::REFERENCE_RECU_VENDU,
                        reference_recu_vendu);

    QSqlQuery a_ref_recu_vendu_qsql_query;

    int aquery_size = YerothUtils::execQuery(a_ref_recu_vendu_qsql_query,
                                             SELECT_ALL_SOLD_ITEMS_FOR_THIS_SALE);

    if (aquery_size <= 0)
    {
        return;
    }

    uint columns_size = 8;

    QTableWidget tableau_nouveau_recu;

    tableau_nouveau_recu.setRowCount(aquery_size);

    tableau_nouveau_recu.setColumnCount(columns_size);

    tableau_nouveau_recu.setHorizontalHeaderItem(0,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Référence")));
    tableau_nouveau_recu.setHorizontalHeaderItem(1,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Désignation")));
    tableau_nouveau_recu.setHorizontalHeaderItem(2,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Catégorie")));
    tableau_nouveau_recu.setHorizontalHeaderItem(3,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr("Qté")));
    tableau_nouveau_recu.setHorizontalHeaderItem(4,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Qté en stock")));
    tableau_nouveau_recu.setHorizontalHeaderItem(5,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Prix unitaire")));
    tableau_nouveau_recu.setHorizontalHeaderItem(6,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr("TVA")));
    tableau_nouveau_recu.setHorizontalHeaderItem(7,
                                                 new
                                                 QTableWidgetItem
                                                 (QObject::tr
                                                  ("Total TTC")));


    double quantite_totale_vendue = 0.0;

    double montant_tva = 0.0;

    double montant_total_vente = 0.0;


    QString date_vente;

    QString heure_de_vente;

    QString NOM_CAISSIER;

    QString nomClient;

    int type_de_vente = YerothUtils::VENTE_COMPTANT;

    uint row = 0;

    while (a_ref_recu_vendu_qsql_query.next())
    {
        if (0 == row)
        {
            date_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerothDatabaseTableColumn::DATE_VENTE);

            heure_de_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerothDatabaseTableColumn::HEURE_VENTE);

            NOM_CAISSIER =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerothDatabaseTableColumn::NOM_CAISSIER);

            nomClient =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerothDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT);

            type_de_vente =
                            GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                                YerothDatabaseTableColumn::TYPE_DE_VENTE).
                            toInt();
        }


        montant_tva +=
                        GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                            YerothDatabaseTableColumn::MONTANT_TVA).
                        toDouble();

        montant_total_vente +=
                        GET_SQL_RECORD_DATA(a_ref_recu_vendu_qsql_query,
                                            YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        quantite_totale_vendue +=
                        qFabs(GET_SQL_RECORD_DATA
                              (a_ref_recu_vendu_qsql_query,
                               YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble());


        tableau_nouveau_recu.setItem(row, 0,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       REFERENCE)));

        tableau_nouveau_recu.setItem(row, 1,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       DESIGNATION)));

        tableau_nouveau_recu.setItem(row, 2,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       CATEGORIE)));

        tableau_nouveau_recu.setItem(row, 3,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       QUANTITE_VENDUE)));

        tableau_nouveau_recu.setItem(row, 4,
                                     new
                                     QTableWidgetItem
                                     (YerothUtils::EMPTY_STRING));

        tableau_nouveau_recu.setItem(row, 5,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       PRIX_UNITAIRE)));

        tableau_nouveau_recu.setItem(row, 6,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       MONTANT_TVA)));

        tableau_nouveau_recu.setItem(row, 7,
                                     new
                                     QTableWidgetItem(GET_SQL_RECORD_DATA
                                                      (a_ref_recu_vendu_qsql_query,
                                                       YerothDatabaseTableColumn::
                                                       MONTANT_TOTAL_VENTE)));

        ++row;
    }

    QString factureTexTable;

    YerothUtils::getFactureTexTableString(factureTexTable,
                                          tableau_nouveau_recu,
                                          quantite_totale_vendue,
                                          montant_tva, montant_total_vente);


    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString factureTexDocument;

    QString factureDate(infoEntreprise.getVille_LATEX());

    YerothUtils::getCurrentLocaleDate(factureDate);

    YerothUtils::get_RECU_TexDocumentString(factureTexDocument,
                                            factureTexTable);

    QString typeDeVenteStr(YerothUtils::_typedeventeToUserViewString.
                           value(type_de_vente));

    factureTexDocument.replace("YEROTHTYPEPAIEMENT",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));

    factureTexDocument.replace("YEROTHNUMEROSORTIETRANSFERT",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents
                               (reference_recu_vendu));

    if (YerothUtils::isEqualCaseInsensitive
            (YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
             YerothERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YEROTHPAPERSPEC", "a4paper");
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
              YerothERPConfig::receiptFormat))
    {
        factureTexDocument.replace("YEROTHPAPERSPEC", "a5paper");
    }

    factureTexDocument.replace("YEROTHPAIEMENT",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents
                               (typeDeVenteStr));
    factureTexDocument.replace("YEROTHENTREPRISE",
                               infoEntreprise.getNomCommercial_LATEX());
    factureTexDocument.replace("YEROTHACTIVITESENTREPRISE",
                               infoEntreprise.getSecteursActivitesTex());
    factureTexDocument.replace("YEROTHBOITEPOSTALE",
                               infoEntreprise.getBoitePostal());
    factureTexDocument.replace("YEROTHVILLE",
                               infoEntreprise.getVille_LATEX());
    factureTexDocument.replace("YEROTHLOCALISATION_SORTIE",
                               infoEntreprise.getLocalisation_LATEX());
    factureTexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    factureTexDocument.replace("YEROTHEMAIL",
                               infoEntreprise.getEmail_LATEX());
    factureTexDocument.replace("YEROTHTELEPHONE",
                               infoEntreprise.getTelephone());
    factureTexDocument.replace("YEROTHDATE", factureDate);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    factureTexDocument.replace("YEROTHHEUREVENTE",
                               QString("%1, %2").arg(heure_de_vente,
                                                     YerothUtils::
                                                     frenchLocale.
                                                     toString
                                                     (GET_DATE_FROM_STRING
                                                      (date_vente))));
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    factureTexDocument.replace("YEROTHHEUREVENTE",
                               QString("%1, %2").arg(heure_de_vente,
                                                     YerothUtils::
                                                     englishLocale.
                                                     toString
                                                     (GET_DATE_FROM_STRING
                                                      (date_vente))));
#endif

    factureTexDocument.replace("YEROTHVENDEUR", NOM_CAISSIER);
    factureTexDocument.replace("YEROTHCOMPTEBANCAIRENR",
                               infoEntreprise.getNumeroCompteBancaire());
    factureTexDocument.replace("YEROTHCONTRIBUABLENR",
                               infoEntreprise.getNumeroDeContribuable());
    factureTexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE",
                               infoEntreprise.getAgenceCompteBancaireTex());

    factureTexDocument.replace("YEROTHSOMME",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents
                               (GET_CURRENCY_STRING_NUM
                                (montant_total_vente)));


    QString
    YEROTHMONTANTLITERAL(QString("'%1' %2").arg
                         (GET_SPELLED_NUMBER(montant_total_vente),
                          YerothERPConfig::currency));

    //    QDEBUG_STRING_OUTPUT_2("YEROTHMONTANTLITERAL", YEROTHMONTANTLITERAL);

    factureTexDocument.replace("YEROTHMONTANTLITERAL",
                               YerothUtils::LATEX_IN_OUT_handleForeignAccents
                               (YEROTHMONTANTLITERAL.toUpper()));

    if (YerothUtils::isEqualCaseInsensitive
            (nomClient, YerothUtils::STRING_FRENCH_DIVERS))
    {
        factureTexDocument.replace("YEROTHCLIENT",
                                   YerothUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));
        factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHCITY",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHPOBOX",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHMAIL",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHPHONE",
                                   YerothUtils::EMPTY_STRING);
    }
    else if (nomClient.isEmpty())
    {
        nomClient.append("Client \"DIVERS\"");
        factureTexDocument.replace("YEROTHCLIENT",
                                   YerothUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));
        factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHCITY",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHPOBOX",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHMAIL",
                                   YerothUtils::EMPTY_STRING);
        factureTexDocument.replace("CLIENTYEROTHPHONE",
                                   YerothUtils::EMPTY_STRING);
    }
    else
    {
        YerothSqlTableModel &clientTableModel =
                        _allWindows->getSqlTableModel_clients();

        clientTableModel.yerothSetFilter_WITH_where_clause(QString
                                                           ("nom_entreprise = '%1'").
                                                           arg(nomClient));
        factureTexDocument.replace("YEROTHCLIENT",
                                   YerothUtils::
                                   LATEX_IN_OUT_handleForeignAccents
                                   (nomClient));

        if (clientTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 703) > 0)
        {
            QSqlRecord record = clientTableModel.record(0);
            QString
            clientRepresentant(GET_SQL_RECORD_DATA
                               (record,
                                YerothDatabaseTableColumn::
                                NOM_REPRESENTANT));
            QString
            clientVille(GET_SQL_RECORD_DATA
                        (record, YerothDatabaseTableColumn::VILLE));
            QString
            clientPOBox(GET_SQL_RECORD_DATA
                        (record, YerothDatabaseTableColumn::BOITE_POSTALE));
            QString
            clientEmail(GET_SQL_RECORD_DATA
                        (record, YerothDatabaseTableColumn::EMAIL));
            QString
            clientTel(GET_SQL_RECORD_DATA
                      (record,
                       YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

            if (clientTel.isEmpty())
            {
                clientTel.append(GET_SQL_RECORD_DATA
                                 (record,
                                  YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));
            }

            //qDebug() << "++ clientEmail: " << clientEmail;
            if (!clientRepresentant.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientRepresentant));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                           YerothUtils::EMPTY_STRING);
            }

            if (!clientVille.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHCITY",
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientVille));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHCITY",
                                           YerothUtils::EMPTY_STRING);
            }

            if (!clientPOBox.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHPOBOX",
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientPOBox));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHPOBOX", "N/a");
            }

            if (!clientEmail.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHMAIL",
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientEmail));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHMAIL",
                                           YerothUtils::EMPTY_STRING);
            }

            if (!clientTel.isEmpty())
            {
                factureTexDocument.replace("CLIENTYEROTHPHONE",
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (clientTel));
            }
            else
            {
                factureTexDocument.replace("CLIENTYEROTHPHONE",
                                           YerothUtils::EMPTY_STRING);
            }
        }
        else
        {
            factureTexDocument.replace("YEROTHCLIENT",
                                       YerothUtils::
                                       LATEX_IN_OUT_handleForeignAccents
                                       (nomClient));
            factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
                                       YerothUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYEROTHCITY",
                                       YerothUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYEROTHPOBOX",
                                       YerothUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYEROTHMAIL",
                                       YerothUtils::EMPTY_STRING);
            factureTexDocument.replace("CLIENTYEROTHPHONE",
                                       YerothUtils::EMPTY_STRING);
        }

        clientTableModel.resetFilter();
    }

    QString prefixFileName;

#ifdef YEROTH_FRANCAIS_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yeroth-erp-vente-facture-grand-NOUVEAU"));

#endif

#ifdef YEROTH_ENGLISH_LANGUAGE

    prefixFileName.append(YerothUtils::getUniquePrefixFileInTemporaryFilesDir
                          ("yeroth-erp-sale-receipt-big-NEW"));

#endif //qDebug() << "++\n" << factureTexDocument;


    QFile tmpLatexFile(prefixFileName + "tex");

    if (tmpLatexFile.open(QFile::WriteOnly))
    {
        tmpLatexFile.write(factureTexDocument.toUtf8());
    }

    tmpLatexFile.close();


    QString
    pdfReceiptFileName(YerothERPProcess::compileLatex(prefixFileName));

    if (!pdfReceiptFileName.isEmpty())
    {
        YerothERPProcess::startPdfViewerProcess(pdfReceiptFileName);
    }

    return;
}


void YerothVentesWindow::afficher_ventes()
{
    tabWidget_ventes->setCurrentIndex(TableauDesVentes);
    textChangedSearchLineEditsQCompleters();
}


void YerothVentesWindow::ENABLE_AfficherAnnulerCetteVente(bool enable)
{
    if (!enable)
    {
        actionAnnulerCetteVente->setVisible(false);
        return;
    }
    else
    {
        YerothPOSUser *aUser = _allWindows->getUser();

        if (0 == aUser)
        {
            actionAnnulerCetteVente->setVisible(false);
            return;
        }

        if (aUser->isManager())
        {
            actionAnnulerCetteVente->setVisible(true);
        }
        else
        {
            actionAnnulerCetteVente->setVisible(false);
        }
    }
}


void YerothVentesWindow::modifier_visibilite_actions_sur_cette_vente()
{
    if (tableView_ventes->rowCount() > 0)
    {
        actionAfficherVenteDetail->setVisible(true);
        ENABLE_AfficherAnnulerCetteVente(true);
    }
    else
    {
        actionAfficherVenteDetail->setVisible(false);
        ENABLE_AfficherAnnulerCetteVente(false);
    }
}


bool YerothVentesWindow::annuler_cette_vente()
{
    int ventesTableViewRowCount = tableView_ventes->rowCount();

    if (ventesTableViewRowCount <= 0)
    {
        return false;
    }

    tabWidget_ventes->setCurrentIndex(RetourDuneVente);

    QString msg;

    if (lineEdit_retour_vente_quantite_a_retourner->text().isEmpty())
    {
        msg =
                        QObject::tr
                        ("Veuillez saisir la quantité d'articles à retourner !");

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("saisir la quantité d'articles à retourner"),
                                       msg);

        lineEdit_retour_vente_quantite_a_retourner->checkField();

        return false;
    }

    double quantite_a_retourner =
                    lineEdit_retour_vente_quantite_a_retourner->text().toDouble();

//      qDebug() << QString("++ qte a retourner: %1")
//                                      .arg(QString::number(quantite_a_retourner));

    if (quantite_a_retourner <= 0.0)
    {
        msg =
                        QObject::tr
                        ("La quantité d'articles à retourner doit être supérieure zéro !");

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("quantité d'articles à retourner"),
                                       msg);

        return false;
    }


    if (YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING,
                                            comboBox_retour_vente_nom_client->
                                            currentText()))
    {
        comboBox_retour_vente_nom_client->checkField();

        return false;
    }

    if (YerothUtils::isEqualCaseInsensitive(YerothUtils::STRING_FRENCH_DIVERS,
                                            comboBox_retour_vente_nom_client->
                                            currentText()))
    {
        msg = QObject::tr("VEUILLEZ SAISIR 1 NOM DU CLIENT 'valide' !");

        YerothQMessageBox::information(this,
                                       QObject::tr("NOM DU CLIENT"), msg);

        return false;
    }


    if (YerothUtils::isEqualCaseInsensitive(YerothUtils::EMPTY_STRING,
                                            lineEdit_retour_vente_numero_de_telephone_client->
                                            text()))
    {
        msg =
                        QObject::tr
                        ("veuillez saisir 1 numéro de téléphone du client !");

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("numéro de téléphone du client"),
                                       msg);

        lineEdit_retour_vente_numero_de_telephone_client->checkField();

        return false;
    }

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord curStocksVenduRecord;

    _allWindows->_ventesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (curStocksVenduRecord);

    double curStocksVenduQuantiteVendue = 0.0;

    curStocksVenduQuantiteVendue =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::QUANTITE_VENDUE).
                    toDouble();

    if (quantite_a_retourner > curStocksVenduQuantiteVendue)
    {
        msg =
                        QObject::tr("La quantité d'articles à retourner doit être "
                                        "inférieure ou égale à la quantité d'articles qui a été vendu !");

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("quantité d'articles à retourner"),
                                       msg);
        return false;
    }

    QString curVenteReference = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerothDatabaseTableColumn::REFERENCE);

    QString
    curVenteReferenceRecuVendu(GET_SQL_RECORD_DATA
                               (curStocksVenduRecord,
                                YerothDatabaseTableColumn::
                                REFERENCE_RECU_VENDU));

    QString
    curStocksVenduDesignation(GET_SQL_RECORD_DATA
                              (curStocksVenduRecord,
                               YerothDatabaseTableColumn::DESIGNATION));

    msg = QObject::tr("Poursuivre avec l'annulation de "
                          "la vente avec la désignation '%1', et "
                          "avec la référence 'reçu de vente %2' ?").arg
          (curStocksVenduDesignation, curVenteReferenceRecuVendu);

    if (QMessageBox::Cancel ==
            YerothQMessageBox::question(this,
                                        QObject::tr
                                        ("poursuivre l'annulation de la vente"),
                                        msg, QMessageBox::Cancel, QMessageBox::Ok))
    {
        return false;
    }

    int typeDeVente = YerothUtils::VENTE_COMPTANT;

    QString curNomDuClient;

    int clients_id = -1;

    double curMontantTotalVente = 0.0;

    double curMontantARembourserAuClient = 0.0;

    bool rembourserAuCompteClient = false;

    bool successReinsertStock = false;

    double curStockNouvelleQuantiteTotal = 0.0;
    double curStockQuantiteTotal = 0.0;

    QSqlRecord curStockRecord;
    QString curStockTableFilter;

    QString curHistoriqueStock;
    QString curHistoriqueStockRetour;

    QString curStocksVenduID;
    QString curStocksVendu_stocksID;
    QString curStocksVenduNomDepartementProduit;
    QString curStocksVenduCategorie;

    curStocksVenduID =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::ID);

    curStocksVendu_stocksID =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::STOCKS_ID);

    curStocksVenduNomDepartementProduit =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    curStocksVenduDesignation =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::DESIGNATION);

    curStocksVenduCategorie =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::CATEGORIE);

    //Je verifie deja si le stock est encore existant
    //dans la base de donnees
    YerothSqlTableModel &curStockTableModel =
                    _allWindows->getSqlTableModel_stocks();

    curStockTableFilter =
                    QString("%1 = '%2'").arg(YerothDatabaseTableColumn::ID,
                                             curStocksVendu_stocksID);

    curStockTableModel.yerothSetFilter_WITH_where_clause(curStockTableFilter);

    int curStocksTableRowCount =
    		curStockTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 1112);

    curNomDuClient =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

    clients_id =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::CLIENTS_ID).toInt();

    typeDeVente =
                    GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                        YerothDatabaseTableColumn::TYPE_DE_VENTE).toInt();

    bool cur_historique_stock_actif = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                          YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                                      toInt();


    if (1 == curStocksTableRowCount)
    {
        curStockRecord = curStockTableModel.record(0);

        curStockQuantiteTotal =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerothDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        curStockNouvelleQuantiteTotal =
                        curStockQuantiteTotal + quantite_a_retourner;

        curMontantTotalVente =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                                curStockNouvelleQuantiteTotal);

        cur_historique_stock_actif =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerothDatabaseTableColumn::
                                            HISTORIQUE_STOCK_ACTIF).toInt();

        curHistoriqueStock =
                        GET_SQL_RECORD_DATA(curStockRecord,
                                            YerothDatabaseTableColumn::HISTORIQUE_STOCK);

        if (cur_historique_stock_actif)
        {
            curHistoriqueStockRetour =
                            YerothHistoriqueStock::creer_mouvement_stock
                            (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
                             curStockRecord.value(YerothDatabaseTableColumn::ID).toInt(),
                             GET_CURRENT_DATE, curStockQuantiteTotal, quantite_a_retourner,
                             curStockNouvelleQuantiteTotal);
        }

        curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE).
        append(curHistoriqueStockRetour);

        curStockRecord.
        setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 cur_historique_stock_actif);

        curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                                curHistoriqueStock);

        successReinsertStock =
                        curStockTableModel.updateRecord(0,
                        								curStockRecord,
														"src/windows/yeroth-erp-ventes-window.cpp",
														1181);
    }
    else
    {
        curStockRecord = curStockTableModel.record();

        curStockRecord.setValue(YerothDatabaseTableColumn::ID,
                                curStocksVendu_stocksID);

        curStockRecord.
        setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                 cur_historique_stock_actif);

        curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerothDatabaseTableColumn::
                                                    HISTORIQUE_STOCK));

        curStockRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerothDatabaseTableColumn::
                                                    REFERENCE));

        curStockRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                                curStocksVenduDesignation);

        curStockRecord.
        setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                 curStocksVenduNomDepartementProduit);

        curStockRecord.setValue(YerothDatabaseTableColumn::CATEGORIE,
                                curStocksVenduCategorie);

        curStockRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);

        curStockRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                                curStocksVenduRecord.value
                                (YerothDatabaseTableColumn::
                                 DATE_PEREMPTION).toDate());

        curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT,
                                quantite_a_retourner);

        curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                                quantite_a_retourner);

        curMontantTotalVente =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        //                  qDebug() << QString("++ a rembourser au client: %1")
        //                                              .arg(QString::number(curMontantARembourserAuClient));

        double montant_total_tva = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                       YerothDatabaseTableColumn::MONTANT_TVA).
                                   toDouble();

        double montant_total_tva_en_gros =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS).
                        toDouble();

        double montant_tva_unitaire =
                        montant_total_tva / curStocksVenduQuantiteVendue;

        double montant_tva_unitaire_en_gros =
                        montant_total_tva_en_gros / curStocksVenduQuantiteVendue;

        double prix_unitaire = GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                   YerothDatabaseTableColumn::PRIX_UNITAIRE).
                               toDouble();

        double prix_unitaire_en_gros =
                        GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                            YerothDatabaseTableColumn::
                                            PRIX_UNITAIRE_EN_GROS).toDouble();

        double prix_vente = prix_unitaire + montant_tva_unitaire;

        double prix_vente_en_gros =
                        prix_unitaire_en_gros + montant_tva_unitaire_en_gros;

        curStockRecord.
        setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                 prix_unitaire_en_gros);

        curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                                prix_unitaire);

        curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                prix_vente_en_gros);

        curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE,
                                prix_vente);

        curStockRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                                montant_tva_unitaire_en_gros);

        curStockRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                                montant_tva_unitaire);

        curStockRecord.setValue(YerothDatabaseTableColumn::IS_SERVICE,
                                YerothUtils::MYSQL_FALSE_LITERAL);

        curStockRecord.
        setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                 GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                     YerothDatabaseTableColumn::
                                     NOM_ENTREPRISE_FOURNISSEUR));

        curStockRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                                GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                    YerothDatabaseTableColumn::
                                                    LOCALISATION));

        curStockRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE,
                                GET_CURRENT_DATE);


        if (cur_historique_stock_actif)
        {
            curHistoriqueStockRetour =
                            YerothHistoriqueStock::creer_mouvement_stock
                            (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
                             curStocksVendu_stocksID.toInt(), GET_CURRENT_DATE, 0.0,
                             quantite_a_retourner, quantite_a_retourner);
        }

        curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
        				  .append(curHistoriqueStockRetour);

        curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                                curHistoriqueStock);

        successReinsertStock = curStockTableModel
        						.insertNewRecord(curStockRecord,
        										 this,
												 "src/windows/yeroth-erp-ventes-window.cpp",
												 1312,
												 curStocksVenduDesignation);
    }

    curStockTableModel.resetFilter();

    if (successReinsertStock)
    {
        double curStocksVenduNouvelleQteVendue =
                        curStocksVenduQuantiteVendue - quantite_a_retourner;

        if (0 != curStocksVenduNouvelleQteVendue)
        {
            curMontantARembourserAuClient =
                            quantite_a_retourner * (curMontantTotalVente /
                                                    curStocksVenduQuantiteVendue);

            double curStocksVenduNouveauMontantTotalVente =
                            curMontantTotalVente - curMontantARembourserAuClient;

            double curStocksVendu_MARGE_BENEFICIAIRE =
                            GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                YerothDatabaseTableColumn::
                                                MARGE_BENEFICIAIRE).toDouble();

            if (curStocksVendu_MARGE_BENEFICIAIRE > 0)
            {
                double marge_beneficiaire_unitaire =
                                curStocksVendu_MARGE_BENEFICIAIRE /
                                curStocksVenduQuantiteVendue;

                double curStocksVendu_nouvelle_MARGE_BENEFICIAIRE =
                                marge_beneficiaire_unitaire * curStocksVenduNouvelleQteVendue;

                curStocksVenduRecord.
                setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
                         curStocksVendu_nouvelle_MARGE_BENEFICIAIRE);
            }

            double curStocksVenduMontantTVA =
                            GET_SQL_RECORD_DATA(curStocksVenduRecord,
                                                YerothDatabaseTableColumn::MONTANT_TVA).
                            toDouble();

            double curStocksVenduNouveauMontantTVA =
                            curStocksVenduNouvelleQteVendue * (curStocksVenduMontantTVA /
                                                               curStocksVenduQuantiteVendue);

//                      qDebug() << QString("++ curStocksVenduNouveauMontantTotalVente: %1, "
//                                                              "curStocksVenduMontantTVA: %2, "
//                                                              "curStocksVenduNouveauMontantTVA: %3, "
//                                                              "curMontantARembourserAuClient: %4")
//                                                      .arg(QString::number(curStocksVenduNouveauMontantTotalVente),
//                                                               QString::number(curStocksVenduMontantTVA),
//                                                               QString::number(curStocksVenduNouveauMontantTVA),
//                                                               QString::number(curMontantARembourserAuClient));

            curStocksVenduRecord.
            setValue(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                     curStocksVenduNouveauMontantTotalVente);

            curStocksVenduRecord.
            setValue(YerothDatabaseTableColumn::QUANTITE_VENDUE,
                     curStocksVenduNouvelleQteVendue);

            curStocksVenduRecord.
            setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                     curStocksVenduNouveauMontantTVA);

            _allWindows->
            _ventesWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
            (curStocksVenduRecord);
        }
        else
        {
            curMontantARembourserAuClient = curMontantTotalVente;

            QString
            removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'").arg
                           (YerothDatabase::STOCKS_VENDU,
                            YerothDatabaseTableColumn::ID,
                            curStocksVenduID));

            bool successRemoveRowQuery =
                            YerothUtils::execQuery(removeRowQuery);
        }

        QString STR_CLIENTS_ID = QString::number(clients_id);

        rembourserAuCompteClient =
                        handleCompteClient(STR_CLIENTS_ID,
                                           typeDeVente, curMontantARembourserAuClient);

        clients_id = STR_CLIENTS_ID.toInt();
    }

    if (successReinsertStock)
    {
//              QDEBUG_STRING_OUTPUT_2_N("new created clients_id", clients_id);

        curNomDuClient = comboBox_retour_vente_nom_client->currentText();

        if (rembourserAuCompteClient && -1 != clients_id)
        {
            msg =
                            QObject::tr
                            ("La vente (avec référence 'reçu de vente %1') a été "
                             "annulée avec succès !\n\n"
                             "(Montant remis (crédité) au compte du client '%2': '%3' !)").
                            arg(curVenteReferenceRecuVendu, curNomDuClient,
                                GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
        }
        else
        {
            msg =
                            QObject::tr
                            ("La vente (avec référence 'reçu de vente %1') a été "
                             "annulée avec succès !\n\n"
                             "(Montant à rembourser au client (comptant): '%2' !)").arg
                            (curVenteReferenceRecuVendu,
                             GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
        }

        YerothQMessageBox::information(this, QObject::tr("succès"), msg);

        //J'ajoute 1 entree dans le tableau de paiements afin d'indiquer
        //la somme remboursee au au compte client

        bool successPaiementsInsert = false;

        YerothSqlTableModel &paiementsSqlTableModel =
                        _allWindows->getSqlTableModel_paiements();

        QSqlRecord paiementsRecord = paiementsSqlTableModel.record();

        paiementsRecord.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT,
                                 GET_CURRENT_DATE);
        paiementsRecord.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT,
                                 CURRENT_TIME);
        paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                 curNomDuClient);

        QString utilisateurCourantNomComplet;

        YerothPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            utilisateurCourantNomComplet = aUser->nom_complet();

            paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR,
                                     utilisateurCourantNomComplet);
        }
        else
        {
            paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR,
                                     QObject::tr("inconnu(e)"));
        }

        //This amount is debited from company account to client account;
        //that is why we negate its value into the payments table.
        paiementsRecord.setValue(YerothDatabaseTableColumn::MONTANT_PAYE,
                                 -1 * curMontantARembourserAuClient);

        paiementsRecord.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
                                 YerothUtils::
                                 DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT);

//              QDEBUG_STRING_OUTPUT_2("curNomDuClient", curNomDuClient);
//
//              QDEBUG_STRING_OUTPUT_2_N("clients_id", clients_id);

        if (-1 != clients_id)
        {
            paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                     curNomDuClient);
        }
        else
        {
            paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                     YerothUtils::STRING_FRENCH_DIVERS);
        }

        paiementsRecord.setValue(YerothDatabaseTableColumn::NOTES,
                                 QObject::tr
                                 ("RETOUR D'ACHAT de L'ARTICLE '%1' (quantité retournée: '%2') AYANT "
                                  "EU POUR RÉFÉRENCE DE REÇU DE VENTE: '%3' !"
                                  "\n\n" "NOM DU CLIENT: \"%4\" \n"
                                  "NUMÉRO DE TÉLÉPHONE DU client: \"%5\".").
                                 arg(curStocksVenduDesignation,
                                     QString::number(quantite_a_retourner),
                                     curVenteReferenceRecuVendu,
                                     curNomDuClient,
                                     lineEdit_retour_vente_numero_de_telephone_client->
                                     text()));

        int IDforReceipt =
                        YerothERPWindows::getNextIdSqlTableModel_paiements();

        QString
        referenceRecuPaiementClient
        (YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT
         (QString::number(IDforReceipt)));

        paiementsRecord.
        setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
                 referenceRecuPaiementClient);


        paiementsRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                                 curStocksVenduDesignation);

        paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                                 curVenteReference);


        YerothSqlTableModel &clientsTableModel =
                        _allWindows->getSqlTableModel_clients();

        QString clientsTableFilter =
                        QString("%1 = '%2'").arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                                 curNomDuClient);

        clientsTableModel.yerothSetFilter_WITH_where_clause(clientsTableFilter);

        double nouveau_compte_client = 0.0;

        int rows = clientsTableModel
        			.easySelect("src/windows/yeroth-erp-ventes-window.cpp",
        						1543);

        if (rows > 0)
        {
            QSqlRecord clientsRecord = clientsTableModel.record(0);

            //The client new account value has been updated previously in
            // method 'bool handleCompteClient(QString client_id,
            //                                                                 double curMontantARembourserAuClient)
            nouveau_compte_client =
                            GET_SQL_RECORD_DATA(clientsRecord,
                                                YerothDatabaseTableColumn::COMPTE_CLIENT).
                            toDouble();
        }

        clientsTableModel.resetFilter();

//              QDEBUG_STRING_OUTPUT_2_N("nouveau_compte_client", nouveau_compte_client);

        paiementsRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                                 YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

        paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT,
                                 nouveau_compte_client);

        paiementsRecord.setValue(YerothDatabaseTableColumn::ID, IDforReceipt);

        successPaiementsInsert =
                        paiementsSqlTableModel
							.insertNewRecord(paiementsRecord,
											 this,
											 "src/windows/yeroth-erp-ventes-window.cpp",
											 1572,
											 curNomDuClient);

        if (!successPaiementsInsert)
        {
            YerothQMessageBox::warning(this,
                                       QObject::tr("paiements - échec"),
                                       QObject::tr
                                       ("1 entrée dans le tableau des paiements n'a pas pu "
                                        "être faites pour la vente annulée (du client %1), "
                                        "avec référence 'reçu de vente %2' !").
                                       arg(curNomDuClient,
                                           curVenteReferenceRecuVendu));
        }

        tabWidget_ventes->setCurrentIndex(TableauDesVentes);

        lineEdit_retour_vente_quantite_a_retourner->clear();

        reinitialiser_recherche();
    }
    else
    {
        msg = QObject::tr("Échec de l'annulation de la vente "
                              "(avec référence '%1') !").arg
              (curVenteReferenceRecuVendu);

        YerothQMessageBox::information(this, QObject::tr("échec"), msg);
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    return successReinsertStock;
}


//bool annuler_cette_vente_2()
//{
//      QString msg;
//
//      int ventesTableViewRowCount = tableView_ventes->rowCount();
//
//      QString curVenteReferenceRecuVendu(lineEdit_ventes_reference_recu_vendu->text());
//
//      if (curVenteReferenceRecuVendu.isEmpty() || ventesTableViewRowCount <= 0)
//      {
//              msg = QObject::tr("Veuillez saisir la référence du 'reçu de vente' à annuler !");
//
//              YerothQMessageBox::information(this,
//                                                                         QObject::tr("annuler une vente"),
//                                                                         msg);
//
//              return false;
//      }
//
//    msg = QObject::tr("Poursuivre avec l'annulation de "
//                                                        "la vente avec la référence 'reçu de vente "
//                                                        "%1' ?"))
//              .arg(curVenteReferenceRecuVendu);
//
//    if (QMessageBox::Cancel ==
//            YerothQMessageBox::question(this, QObject::tr("poursuivre l'annulation de la vente"),
//                                                                               msg,
//                                                                                       QMessageBox::Cancel,
//                                                                                       QMessageBox::Ok))
//    {
//      return false;
//    }
//
//      YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
//
//      QString typeDeVente(QObject::tr("achat-comptant"));
//
//      QString curNomDuClient;
//
//      int clients_id = -1;
//
//      double curMontantARembourserAuClient = 0.0;
//
//      bool rembourserAuCompteClient = false;
//
//      bool successReinsertStock = false;
//
//      double curStocksVenduQuantiteVendue = 0.0;
//      double curStockNouvelleQuantiteTotal = 0.0;
//      double curStockQuantiteTotal = 0.0;
//
//      QSqlRecord curStockRecord;
//      QString curStockTableFilter;
//
//    QString curHistoriqueStock;
//    QString curHistoriqueStockRetour;
//
//      QString curStocksVenduID;
//      QString curStocksVendu_stocksID;
//      QString curStocksVenduDesignation;
//      QString curStocksVenduCategorie;
//
//      QSqlRecord curStocksVenduRecord;
//
//      for (int k = 0; k < ventesTableViewRowCount; ++k)
//      {
//              curStocksVenduRecord.clear();
//
//              curStocksVenduRecord = _curStocksVenduTableModel->record(k);
//
//              curStocksVenduID =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::ID);
//
//              curStocksVendu_stocksID =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::STOCKS_ID);
//
//              curStocksVenduDesignation =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DESIGNATION);
//
//              curStocksVenduCategorie =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CATEGORIE);
//
//              //Je verifie deja si le stock est encore existant
//              //dans la base de donnees
//              YerothSqlTableModel &curStockTableModel = _allWindows->getSqlTableModel_stocks();
//
//              curStockTableFilter = QString("%1 = '%2'")
//                                                              .arg(YerothDatabaseTableColumn::ID,
//                                                                       curStocksVendu_stocksID);
//
//              curStockTableModel.yerothSetFilter(curStockTableFilter);
//
//              int curStocksTableRowCount = curStockTableModel.easySelect();
//
//              if (0 == k)
//              {
//                      curNomDuClient =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);
//
//                      clients_id =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::CLIENTS_ID).toInt();
//
//                      typeDeVente =
//                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::TYPE_DE_VENTE);
//              }
//
//              curStocksVenduQuantiteVendue =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::QUANTITE_VENDUE).toDouble();
//
//              if (1 == curStocksTableRowCount)
//              {
//                      curStockRecord = curStockTableModel.record(0);
//
//                      curStockQuantiteTotal =
//                                      GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
//
//                      curStockNouvelleQuantiteTotal = curStockQuantiteTotal + curStocksVenduQuantiteVendue;
//
//                      curMontantARembourserAuClient +=
//                                                      GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
//                      curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE, curStockNouvelleQuantiteTotal);
//
//                      curHistoriqueStock =
//                              GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);
//
//              curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
//                                      (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
//                                       curStockRecord.value(YerothDatabaseTableColumn::ID).toInt(),
//                                               GET_CURRENT_DATE,
//                                               curStockQuantiteTotal,
//                                               curStocksVenduQuantiteVendue,
//                                               curStockNouvelleQuantiteTotal);
//
//              curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
//                                         .append(curHistoriqueStockRetour);
//
//              curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//                      successReinsertStock = curStockTableModel.updateRecord(0, curStockRecord);
//              }
//              else
//              {
//                      curStockRecord = curStockTableModel.record();
//
//                      curStockRecord.setValue(YerothDatabaseTableColumn::ID, curStocksVendu_stocksID);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::REFERENCE));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::DESIGNATION, curStocksVenduDesignation);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::CATEGORIE, curStocksVenduCategorie);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE, curStocksVenduQuantiteVendue);
//
//                  curMontantARembourserAuClient +=
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble();
//
////                qDebug() << QString("++ a rembourser au client: %1")
////                                            .arg(QString::number(curMontantARembourserAuClient));
//
//                  double quantite_totale = curStocksVenduQuantiteVendue;
//
//                  double montant_total_tva =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
//
//                  double montant_tva_unitaire = montant_total_tva / quantite_totale;
//
//                  double prix_unitaire =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();
//
//                  double prix_vente = prix_unitaire + montant_tva_unitaire;
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE, prix_unitaire);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA, montant_tva_unitaire);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::IS_SERVICE,
//                              YerothUtils::MYSQL_FALSE_LITERAL);
//
//                  QString curStocksVenduDatePeremption =
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::DATE_PEREMPTION);
//
//                  QDate formatee(GET_DATE_FROM_STRING(curStocksVenduDatePeremption));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION, DATE_TO_DB_FORMAT_STRING(formatee));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
//                              GET_SQL_RECORD_DATA(curStocksVenduRecord, YerothDatabaseTableColumn::LOCALISATION));
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::LOTS_ENTRANT, 1);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::QUANTITE_PAR_LOT, quantite_totale);
//
//                  curStockRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE, GET_CURRENT_DATE);
//
//                      curHistoriqueStock =
//                              GET_SQL_RECORD_DATA(curStockRecord, YerothDatabaseTableColumn::HISTORIQUE_STOCK);
//
//              curHistoriqueStockRetour = YerothHistoriqueStock::creer_mouvement_stock
//                                      (MOUVEMENT_DE_STOCK_RETOUR_VENTE,
//                                       curStocksVendu_stocksID.toInt(),
//                                               GET_CURRENT_DATE,
//                                               0.0,
//                                               curStocksVenduQuantiteVendue,
//                                               curStocksVenduQuantiteVendue);
//
//              curHistoriqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE)
//                                         .append(curHistoriqueStockRetour);
//
//              curStockRecord.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK, curHistoriqueStock);
//
//                  successReinsertStock = curStockTableModel.insertNewRecord(curStockRecord);
//              }
//
//              curStockTableModel.resetFilter();
//
//              if (successReinsertStock)
//              {
//                      QString removeRowQuery(QString("DELETE FROM %1 WHERE %2 = '%3'")
//                                      .arg(YerothDatabase::STOCKS_VENDU,
//                                                      YerothDatabaseTableColumn::ID,
//                                                      curStocksVenduID));
//
//                      bool successRemoveRowQuery = YerothUtils::execQuery(removeRowQuery);
//
//                      if (successRemoveRowQuery &&
//                              -1 != clients_id          &&
//                              YerothUtils::isEqualCaseInsensitive(QObject::tr("achat-compte-client"), typeDeVente))
//                      {
//                              rembourserAuCompteClient = handleCompteClient(QString::number(clients_id),
//                                                                                                                        curMontantARembourserAuClient);
//                      }
//
//                      successReinsertStock = successReinsertStock && successRemoveRowQuery;
//              }
//      } //for
//
//      YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
//
//      if (successReinsertStock)
//      {
//              if (rembourserAuCompteClient)
//              {
//                      msg = QObject::tr("La vente (avec référence 'reçu de vente %1') a été "
//                                                                                "annulée avec succès !\n\n"
//                                      "(Montant crédité au compte du client '%2': '%3' !)"))
//                                              .arg(curVenteReferenceRecuVendu,
//                                                       curNomDuClient,
//                                                       GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//              }
//              else
//              {
//                      msg = QObject::tr("La vente (avec référence 'reçu de vente %1') a été "
//                                                                                "annulée avec succès !\n\n"
//                                                                                "(Montant à rembourser au client (comptant): '%2' !)"))
//                                              .arg(curVenteReferenceRecuVendu,
//                                                       GET_CURRENCY_STRING_NUM(curMontantARembourserAuClient));
//              }
//
//              reinitialiser_recherche();
//
//              YerothQMessageBox::information(this,
//                                                                         QObject::tr("succès"),
//                                                                         msg);
//      }
//      else
//      {
//              msg = QObject::tr("Échec de l'annulation de la vente "
//                                                                        "(avec référence '%1') !"))
//                              .arg(curVenteReferenceRecuVendu);
//
//              YerothQMessageBox::information(this,
//                                                                         QObject::tr("échec"),
//                                                                         msg);
//      }
//
//      return successReinsertStock;
//}


bool YerothVentesWindow::filtrer_ventes()
{
    QString
    ventesTableColumnValue(lineEdit_ventes_element_de_vente_resultat->text
                           ());

    if (ventesTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerothQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;

    }

    QString
    ventesTableColumnProperty(comboBox_ventes_element_de_vente->currentText
                              ());

    QString
    mathOperator(comboBox_ventes_element_de_vente_condition->currentText());

    QString
    REAL_DB_ID_NAME_ventesTableColumnProperty
    (YerothDatabaseTableColumn::_tableColumnToUserViewString.key
     (ventesTableColumnProperty));

    QString filterString(QString("%1 AND (%2 %3 %4)").arg(_ventesDateFilter,
                                                          REAL_DB_ID_NAME_ventesTableColumnProperty,
                                                          mathOperator,
                                                          ventesTableColumnValue));

    //qDebug() << "++ " << _searchFilter;

    if (!_searchFilter.isEmpty())
    {
        _searchFilter.append(QString(" AND %1").arg(filterString));
    }
    else
    {
        _searchFilter.append(filterString);
    }

    //qDebug() << QString("++ %1").arg(_searchFilter);

    setCurrentlyFiltered(true);

    _curStocksVenduTableModel->yerothSetFilter_WITH_where_clause
    (_searchFilter);

    int resultRows =
    		_curStocksVenduTableModel
				->easySelect("src/windows/yeroth-erp-ventes-window.cpp", 1949);

    lister_les_elements_du_tableau(*_curStocksVenduTableModel);

    if (resultRows > 0)
    {
        YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "ventes - filtrer");

        return true;
    }
    else
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "ventes - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerothVentesWindow::setCurrentUser_NOM_CAISSIER(const QString &
                                                     aUserNomComplet)
{
    lineEdit_ventes_nom_caissier->setYerothEnabled(false);
    lineEdit_ventes_nom_caissier->setText(aUserNomComplet);
}


void YerothVentesWindow::disableNomCaissier()
{
    lineEdit_ventes_nom_caissier->setYerothEnabled(false);
    lineEdit_ventes_nom_caissier->myClear();
}


void YerothVentesWindow::enableNomCaissier_ONLY_MANAGER()
{
    lineEdit_ventes_nom_caissier->setYerothEnabled(true);
    lineEdit_ventes_nom_caissier->myClear();
}


bool YerothVentesWindow::handleCompteClient(QString &client_id_IN_OUT,
                                            int type_de_vente,
                                            double curMontantARembourserAuClient)
{
    bool result = false;

    YerothSqlTableModel &clientsSqlTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString
    clientsFilter(QString("%1 = '%2'").arg
                  (YerothDatabaseTableColumn::ID, client_id_IN_OUT));

    clientsSqlTableModel.yerothSetFilter_WITH_where_clause(clientsFilter);

    QString a_new_client = YerothUtils::STRING_FRENCH_DIVERS;

    if (!comboBox_retour_vente_nom_client->isEmpty())
    {
        a_new_client = comboBox_retour_vente_nom_client->currentText();
    }

    bool new_client_record_created = false;

    int clientsRowCount =
    		clientsSqlTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 2021);

    if (clientsRowCount <= 0)
    {
        new_client_record_created =
                        YerothUtils::creerNouveauClient(a_new_client, this);

        assert(true == new_client_record_created);
    }

    if (new_client_record_created)
    {
        clientsSqlTableModel.resetFilter();

        QString
        aNewClientsFilter(QString("%1 = '%2'").arg
                          (YerothDatabaseTableColumn::NOM_ENTREPRISE,
                           a_new_client));

        clientsSqlTableModel.yerothSetFilter_WITH_where_clause
        (aNewClientsFilter);

        int newClientsRowCount =
        		clientsSqlTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 2044);

        assert(newClientsRowCount > 0);
    }


    QSqlRecord clientsRecord = clientsSqlTableModel.record(0);

    QString a_client_to_be_reimbursed = GET_SQL_RECORD_DATA(clientsRecord,
                                                            YerothDatabaseTableColumn::NOM_ENTREPRISE);

    client_id_IN_OUT.clear();

    client_id_IN_OUT.append(GET_SQL_RECORD_DATA
                            (clientsRecord, YerothDatabaseTableColumn::ID));

    bool client_account_to_be_updated =
                    (type_de_vente == YerothUtils::VENTE_COMPTE_CLIENT);

    if (client_account_to_be_updated)
    {
        double curCompteClient =
                        clientsRecord.value(YerothDatabaseTableColumn::COMPTE_CLIENT).
                        toDouble();

        double nouveauCompteClient =
                        curCompteClient + curMontantARembourserAuClient;

        clientsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT,
                               nouveauCompteClient);

        result = clientsSqlTableModel.updateRecord(0,
        										   clientsRecord,
												   "src/windows/yeroth-erp-ventes-window.cpp",
												   2086);
    }

    clientsSqlTableModel.resetFilter();

    return result;
}


void YerothVentesWindow::populateComboBoxes()
{
    _logger->log("populateComboBoxes");


    YerothUtils::RELOAD_VIEW_STRINGS();


    comboBox_ventes_type_de_vente->clear();

    comboBox_ventes_type_de_vente
        ->setupPopulateNORawString(YerothDatabase::TYPE_DE_VENTE,
                                   YerothDatabaseTableColumn::TYPE_DE_VENTE,
                                   &YerothUtils::_typedeventeToUserViewString);


    comboBox_ventes_type_de_vente->populateComboBox();


    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerothDatabaseTableColumn::REFERENCE);
    aQStringList.removeAll(YerothDatabaseTableColumn::DATE_PEREMPTION);
    aQStringList.removeAll(YerothDatabaseTableColumn::HISTORIQUE_STOCK);
    aQStringList.removeAll(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER);
    aQStringList.removeAll(YerothDatabaseTableColumn::NOM_CAISSIER);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerothDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::MARGE_BENEFICIAIRE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::QUANTITE_VENDUE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_VENTE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_UNITAIRE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::MONTANT_TVA));


    comboBox_ventes_element_de_vente->clear();

    comboBox_ventes_element_de_vente->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_ventes_element_de_vente_condition->clear();

    comboBox_ventes_element_de_vente_condition->addItems(aQStringList);
}


void YerothVentesWindow::setupLineEdits()
{
    _logger->log("setupLineEdits");

    _QLINEEDIT_nombre_de_lignes_par_page = lineEdit_nombre_de_lignes_par_page;

    lineEdit_retour_vente_numero_de_telephone_client->setValidator
    (&YerothUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);

    lineEdit_ventes_terme_recherche->enableForSearch(QObject::tr
                                                     ("terme à rechercher (référence, fournisseur)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_ventes_nom_caissier->enableForSearch(QObject::tr
                                                  ("nom du caissier (caissière)"));

    lineEdit_ventes_quantite_vendue->setYerothEnabled(false);
    lineEdit_ventes_BENEFICE->setYerothEnabled(false);
    lineEdit_ventes_CAISSE->setYerothEnabled(false);
    lineEdit_ventes_recette_totale->setYerothEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT(lineEdit_nombre_de_lignes_par_page,
                                        tableView_ventes);


    lineEdit_details_type_de_vente->setYerothEnabled(false);
    lineEdit_details_nom_client->setYerothEnabled(false);
    lineEdit_details_reference_produit->setYerothEnabled(false);
    lineEdit_details_designation->setYerothEnabled(false);
    lineEdit_details_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_details_categorie->setYerothEnabled(false);
    lineEdit_details_remise_prix->setYerothEnabled(false);
    lineEdit_details_remise_pourcentage->setYerothEnabled(false);
    lineEdit_details_tva->setYerothEnabled(false);
    lineEdit_details_montant_total->setYerothEnabled(false);
    lineEdit_details_nom_caissier->setYerothEnabled(false);
    lineEdit_details_localisation->setYerothEnabled(false);
    lineEdit_details_quantite_vendue->setYerothEnabled(false);
    lineEdit_details_heure_vente->setYerothEnabled(false);
    lineEdit_details_reference_recu_de_vente->setYerothEnabled(false);
    lineEdit_details_prix_unitaire->setYerothEnabled(false);

    lineEdit_retour_vente_quantite_a_retourner->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_retour_vente_type_de_vente->setYerothEnabled(false);
    lineEdit_retour_vente_reference_produit->setYerothEnabled(false);
    lineEdit_retour_vente_designation->setYerothEnabled(false);
    lineEdit_retour_vente_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_retour_vente_categorie->setYerothEnabled(false);
    lineEdit_retour_vente_remise_prix->setYerothEnabled(false);
    lineEdit_retour_vente_remise_pourcentage->setYerothEnabled(false);
    lineEdit_retour_vente_tva->setYerothEnabled(false);
    lineEdit_retour_vente_montant_total->setYerothEnabled(false);
    lineEdit_retour_vente_nom_caissier->setYerothEnabled(false);
    lineEdit_retour_vente_localisation->setYerothEnabled(false);
    lineEdit_retour_vente_quantite_vendue->setYerothEnabled(false);
    lineEdit_retour_vente_numero_de_telephone_client->setYerothEnabled(true);
    lineEdit_retour_vente_quantite_a_retourner->setYerothEnabled(true);
    lineEdit_retour_vente_heure_vente->setYerothEnabled(false);
    lineEdit_retour_vente_reference_recu_de_vente->setYerothEnabled(false);
    lineEdit_retour_vente_prix_unitaire->setYerothEnabled(false);

    lineEdit_ventes_terme_recherche->setFocus();
}


void YerothVentesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionAnnulerCetteVente->setShortcut(Qt::Key_F2);
}


void YerothVentesWindow::set_filtrer_font()
{
    //_logger->log("set_filtrer_font");

    if (isCurrentlyFiltered())
    {
        _pushButton_ventes_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_ventes_filtrer_font->setUnderline(false);
    }

    pushButton_ventes_filtrer->setFont(*_pushButton_ventes_filtrer_font);
}


void YerothVentesWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    lister_les_elements_du_tableau();
}


void YerothVentesWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_ventes_element_de_vente_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_ventes_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
//              qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("(%1 OR %2)").arg
                                 (GENERATE_SQL_LIKE_STMT
                                  (YerothDatabaseTableColumn::REFERENCE,
                                   partSearchTerm),
                                  GENERATE_SQL_LIKE_STMT
                                  (YerothDatabaseTableColumn::
                                   NOM_ENTREPRISE_FOURNISSEUR,
                                   partSearchTerm)));

            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    YerothWindowsCommons::setYerothLineEditQCompleterSearchFilter
    (_searchFilter);


    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

    {
        YerothLineEdit *aYerothLineEdit = 0;

        QMapIterator < YerothLineEdit **,
                     QString > it(_lineEditsToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerothLineEdit = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerothLineEdit)
            {
                aTableColumnFieldContentForANDSearch = aYerothLineEdit->text();

                if (!correspondingDBFieldKeyValue.isEmpty() &&
                        !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    _searchFilter.append(GENERATE_SQL_IS_STMT
                                         (correspondingDBFieldKeyValue,
                                          aTableColumnFieldContentForANDSearch));
                }
            }
        }
    }

    {
        QString curSearchDBStr;

        YerothComboBox *aYerothComboBox = 0;

        QMapIterator < YerothComboBox **,
                     QString > it(_comboBoxesToANDContentForSearch);

        while (it.hasNext())
        {
            it.next();

            aYerothComboBox = *it.key();

            correspondingDBFieldKeyValue = it.value();

            if (0 != aYerothComboBox)
            {
                aTableColumnFieldContentForANDSearch =
                                aYerothComboBox->currentText();

                if (!correspondingDBFieldKeyValue.isEmpty() &&
                        !aTableColumnFieldContentForANDSearch.isEmpty())
                {
                    if (!_searchFilter.isEmpty())
                    {
                        _searchFilter.append(" AND ");
                    }

                    if (aYerothComboBox == comboBox_ventes_type_de_vente)
                    {
                        int typedevente =
                                        YerothUtils::getComboBoxDatabaseQueryValue
                                        (aTableColumnFieldContentForANDSearch,
                                         YerothUtils::_typedeventeToUserViewString);

                        curSearchDBStr = QString::number(typedevente);
                    }

                    _searchFilter.append(GENERATE_SQL_IS_STMT
                                         (correspondingDBFieldKeyValue,
                                          curSearchDBStr));
                }
            }
        }
    }

    YEROTH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    QString finalSearchFilter(_ventesDateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_ventesDateFilter, _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
        lister_les_elements_du_tableau();
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerothVentesWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothVentesWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT
            << YerothDatabaseTableColumn::LOCALISATION
            << YerothDatabaseTableColumn::DATE_VENTE
            << YerothDatabaseTableColumn::DESIGNATION
            << YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT
            << YerothDatabaseTableColumn::QUANTITE_VENDUE
            << YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE;
}


void YerothVentesWindow::resizeEvent(QResizeEvent *event)
{
    lineEdit_ventes_terme_recherche->setFixedWidth(event->size().width() - 4);

    tabWidget_ventes->setFixedWidth(event->size().width() - 4);

    tableView_ventes->setFixedWidth(tabWidget_ventes->width() - 4);
}


void YerothVentesWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_ventes->rowCount() > 0)
    {
        QMenu menu(this);
        menu.addAction(actionAfficherVenteDetail);
        menu.addAction(action_regenerer_recu_client_vente);
        menu.setPalette(YerothUtils::YEROTH_WHITE_PALETTE);
        menu.exec(event->globalPos());
    }
}


void YerothVentesWindow::clear_all_fields()
{
    comboBox_retour_vente_nom_client->clearField();

    lineEdit_details_nom_client->clearField();
    lineEdit_details_reference_produit->clearField();
    lineEdit_details_designation->clearField();
    lineEdit_details_nom_entreprise_fournisseur->clearField();
    lineEdit_details_categorie->clearField();
    lineEdit_details_remise_prix->clearField();
    lineEdit_details_remise_pourcentage->clearField();
    lineEdit_details_tva->clearField();
    lineEdit_details_montant_total->clearField();
    lineEdit_details_nom_caissier->clearField();
    lineEdit_details_localisation->clearField();
    lineEdit_details_quantite_vendue->clearField();
    lineEdit_details_reference_recu_de_vente->clearField();
    lineEdit_details_heure_vente->clearField();
    lineEdit_details_prix_unitaire->clearField();


    lineEdit_retour_vente_numero_de_telephone_client->clear();

    lineEdit_retour_vente_quantite_a_retourner->clear();

    lineEdit_retour_vente_reference_produit->clearField();
    lineEdit_retour_vente_designation->clearField();
    lineEdit_retour_vente_nom_entreprise_fournisseur->clearField();
    lineEdit_retour_vente_categorie->clearField();
    lineEdit_retour_vente_remise_prix->clearField();
    lineEdit_retour_vente_remise_pourcentage->clearField();
    lineEdit_retour_vente_tva->clearField();
    lineEdit_retour_vente_montant_total->clearField();
    lineEdit_retour_vente_nom_caissier->clearField();
    lineEdit_retour_vente_localisation->clearField();
    lineEdit_retour_vente_quantite_vendue->clearField();
    lineEdit_retour_vente_reference_recu_de_vente->clearField();
    lineEdit_retour_vente_heure_vente->clearField();
    lineEdit_retour_vente_prix_unitaire->clearField();

    lineEdit_ventes_BENEFICE->clearField();
    lineEdit_ventes_CAISSE->clearField();
    lineEdit_ventes_recette_totale->clearField();
    lineEdit_ventes_quantite_vendue->clearField();
}


void YerothVentesWindow::setupDateTimeEdits()
{
    dateEdit_details_date_vente->setYerothEnabled(false);

    dateEdit_details_date_peremption->setYerothEnabled(false);

    dateEdit_retour_vente_date_vente->setYerothEnabled(false);

    dateEdit_retour_vente_date_peremption->setYerothEnabled(false);

    dateEdit_ventes_debut->setStartDate(GET_CURRENT_DATE);

    dateEdit_ventes_fin->setStartDate(GET_CURRENT_DATE);

    _ventesDateFilter.clear();

    _ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ")
    							.arg(YerothDatabaseTableColumn::DATE_VENTE,
    								 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->date()),
									 YerothDatabaseTableColumn::DATE_VENTE,
									 DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->date())));

    connect(dateEdit_ventes_debut,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerothLineEdits()));

    connect(dateEdit_ventes_fin,
            SIGNAL(dateChanged(const QDate &)),
            this,
			SLOT(refineYerothLineEdits()));
}


void YerothVentesWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothVentesWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_ventes);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}

void YerothVentesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    if (tabWidget_ventes->count() < 3)
    {
        tabWidget_ventes->insertTab(RetourDuneVente,
                                    _retourVenteTabWidget,
                                    _retourVenteTabWidgetTitle);
    }


    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksVenduTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->enable(this, SLOT(retourVentes()));

    pushButton_annuler_vente->enable(this, SLOT(annuler_cette_vente()));
}


void YerothVentesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curStocksVenduTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_ventes_filtrer->enable(this, SLOT(filtrer_ventes()));

    pushButton_ventes_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));

    pushButton_retour_details->enable(this, SLOT(retourVentes()));

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}


void YerothVentesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}


void YerothVentesWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


void YerothVentesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_lhistorique_de_ce_stock, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionReinitialiserRecherche,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherVenteDetail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_regenerer_recu_client_vente,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnulerCetteVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    tabWidget_ventes->removeTab(RetourDuneVente);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    (tableView_ventes);

    pushButton_add_condition_SEARCH->disable(this);

    pushButton_ventes_filtrer->disable(this);

    pushButton_ventes_reinitialiser_filtre->disable(this);

    pushButton_reinitialiser->disable(this);

    pushButton_retour_details->disable(this);

    pushButton_retour_annuler->disable(this);

    pushButton_annuler_vente->disable(this);
}


bool YerothVentesWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_ventes,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-cashier-journal-csv-format",
                                               "journal of sales");
    }
    else
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_ventes,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-journal-ventes-format-csv",
                                               "journal des ventes");
    }

    return success;
}


QString YerothVentesWindow::get_latex_template_print_pdf_content()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        _latex_template_print_pdf_content = YerothUtils::EN_template_journal_des_ventes_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_journal_des_ventes_tex;
    }

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YEROTHVENTESDEBUT",
				DATE_TO_STRING(dateEdit_ventes_debut->date()));


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING
		.insert("YEROTHVENTESFIN",
				DATE_TO_STRING(dateEdit_ventes_fin->date()));


    return _latex_template_print_pdf_content;
}


void YerothVentesWindow::resetFilter(YerothSqlTableModel *stocksVenduTableModel)
{
    _curStocksVenduTableModel = stocksVenduTableModel;

    if (0 != _curStocksVenduTableModel)
    {
        _curStocksVenduTableModel->resetFilter();
    }

    lineEdit_ventes_terme_recherche->myClear();


    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enableNomCaissier_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_CAISSIER(aUser->nom_complet());
        }
    }
    else
    {
        disableNomCaissier();
    }

    dateEdit_ventes_debut->reset();
    dateEdit_ventes_fin->reset();
}


void YerothVentesWindow::handleTabEnabled()
{
    if (tableView_ventes->rowCount() > 0)
    {
        tabWidget_ventes->setTabEnabled(RetourDuneVente, true);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, true);
    }
    else
    {
        tabWidget_ventes->setTabEnabled(RetourDuneVente, false);
        tabWidget_ventes->setTabEnabled(AfficherVenteAuDetail, false);
    }
}


void YerothVentesWindow::handleCurrentTabChanged(int index)
{
    //_logger->log("handleCurrentChanged(int)",
    //                  QString("handleCurrentChanged]. index: %1").arg(index));

    switch (index)
    {
    case TableauDesVentes:
        lister_les_elements_du_tableau();
        enable_on_print_actions_SET_VISIBILITY();
        break;

    case AfficherVenteAuDetail:
        if (afficher_vente_detail())
        {
            disable_on_print_actions_SET_VISIBILITY();
        }
        break;

    case RetourDuneVente:
        lineEdit_retour_vente_quantite_a_retourner->setFocus();

        if (afficher_retour_vente())
        {
            disable_on_print_actions_SET_VISIBILITY();
        }
        break;

    default:
        break;
    }

    modifier_visibilite_actions_sur_cette_vente();
}


void YerothVentesWindow::lister_les_elements_du_tableau(YerothSqlTableModel &
                                                        stocksVenduTableModel)
{
    int curStocksVenduTableModelRowCount =
    		_curStocksVenduTableModel->easySelect("src/windows/yeroth-erp-ventes-window.cpp", 2914);

    double quantite_vendue = 0;
    double quantite_vendue_total = 0.0;


    QString reference_recu_vendu;

    QMap < QString, double >reference_recu_vendu_TO_remise_FIDELITE_CLIENTS;

    double remise_FIDELITE_CLIENTS = 0.0;
    double remise_totale_FIDELITE_CLIENTS_fcfa = 0.0;


    double remise_prix = 0.0;
    double remise_total_fcfa = 0.0;

    double montant_total = 0.0;
    double montant_total_vente = 0.0;

    double benefice = 0.0;
    double benefice_total = 0.0;

    double montant_tva = 0.0;
    double total_montant_tva = 0.0;


    QSqlRecord aRecord;

    for (int k = 0; k < curStocksVenduTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = _curStocksVenduTableModel->record(k);

        reference_recu_vendu =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::REFERENCE_RECU_VENDU);

        quantite_vendue =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::QUANTITE_VENDUE).
                        toDouble();

        quantite_vendue_total += quantite_vendue;

        montant_total_vente =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).
                        toDouble();

        montant_total += montant_total_vente;


        if (!reference_recu_vendu_TO_remise_FIDELITE_CLIENTS.contains
                (reference_recu_vendu))
        {
            remise_FIDELITE_CLIENTS =
                            GET_SQL_RECORD_DATA(aRecord,
                                                YerothDatabaseTableColumn::
                                                MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS).
                            toDouble();

//              QDEBUG_STRING_OUTPUT_2_N("remise_FIDELITE_CLIENTS", remise_FIDELITE_CLIENTS);

            reference_recu_vendu_TO_remise_FIDELITE_CLIENTS.insert
            (reference_recu_vendu, remise_FIDELITE_CLIENTS);
        }


        remise_prix =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::REMISE_PRIX).
                        toDouble();

        remise_total_fcfa += (quantite_vendue * remise_prix);

        benefice =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::MARGE_BENEFICIAIRE).
                        toDouble();

        benefice_total += benefice;
    }

    if (_allWindows->getUser()->isManager())
    {
        lineEdit_ventes_BENEFICE->setText(GET_CURRENCY_STRING_NUM
                                          (benefice_total));
    }
    else
    {
        lineEdit_ventes_BENEFICE->clear();
    }

    QMapIterator < QString,
                 double >it(reference_recu_vendu_TO_remise_FIDELITE_CLIENTS);

    while (it.hasNext())
    {
        it.next();
        remise_totale_FIDELITE_CLIENTS_fcfa += it.value();
    }

    double total_caisse = montant_total - remise_totale_FIDELITE_CLIENTS_fcfa;

    lineEdit_ventes_CAISSE->setText(GET_CURRENCY_STRING_NUM(total_caisse));
    lineEdit_ventes_quantite_vendue->setText(GET_DOUBLE_STRING
                                             (quantite_vendue_total));
    lineEdit_ventes_recette_totale->setText(GET_CURRENCY_STRING_NUM
                                            (montant_total));

    tableView_ventes->queryYerothTableViewCurrentPageContentRow
    (stocksVenduTableModel);

    tableView_show_or_hide_columns(*tableView_ventes);

    handleTabEnabled();
}


void YerothVentesWindow::rendreInvisible()
{
    lineEdit_retour_vente_quantite_a_retourner->clear();
    lineEdit_retour_vente_numero_de_telephone_client->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothVentesWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
    retranslateUi(this);

    YEROTH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    _curStocksTableModel = stocksTableModel;

    _curStocksVenduTableModel = &_allWindows->getSqlTableModel_stocks_vendu();

    setYerothSqlTableModel(_curStocksVenduTableModel);

    setupLineEditsQCompleters((QObject *) this);

    populateComboBoxes();

    tabWidget_ventes->setCurrentIndex(TableauDesVentes);

    label_details_tva->setText(QObject::tr("TVA (%1)").
                               arg(YerothERPConfig::currency));

    label_details_remise_prix->setText(QObject::tr("remise (%1)").
                                       arg(YerothERPConfig::currency));

    label_retour_vente_remise_prix->setText(QObject::tr("remise (%1)").
                                            arg(YerothERPConfig::currency));


    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager())
        {
            enableNomCaissier_ONLY_MANAGER();
        }
        else
        {
            setCurrentUser_NOM_CAISSIER(aUser->nom_complet());
        }
    }
    else
    {
        disableNomCaissier();
    }

    modifier_visibilite_actions_sur_cette_vente();

    setVisible(true);

    afficher_ventes();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("ventes"),
    								 _curStocksVenduTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    lineEdit_ventes_terme_recherche->setFocus();
}


bool YerothVentesWindow::afficher_retour_vente()
{
    if (tableView_ventes->rowCount() <= 0)
    {
        return false;
    }

    lineEdit_retour_vente_quantite_a_retourner->checkField();

    lineEdit_retour_vente_numero_de_telephone_client->checkField();


    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record;

    _allWindows->
    _ventesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    bool isService = GET_SQL_RECORD_DATA(record,
                                         YerothDatabaseTableColumn::IS_SERVICE).
                     toInt();

    if (isService)
    {
        QString msg(QObject::tr
                    ("Cette fonctionnalité n'est pas disponible pour les 'services' !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("fonctionnalité pas disponible"),
                                       msg);

        tabWidget_ventes->setCurrentIndex(TableauDesVentes);

        return false;
    }

    lineEdit_retour_vente_reference_produit->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerothDatabaseTableColumn::REFERENCE));
    lineEdit_retour_vente_designation->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_retour_vente_nom_entreprise_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_retour_vente_categorie->setText(GET_SQL_RECORD_DATA
                                             (record,
                                              YerothDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record,
                                                 YerothDatabaseTableColumn::QUANTITE_VENDUE).
                             toDouble();

    lineEdit_retour_vente_quantite_vendue->setText(GET_NUM_STRING
                                                   (quantite_vendue));
    lineEdit_retour_vente_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                                 (GET_SQL_RECORD_DATA
                                                  (record,
                                                   "prix_unitaire").toDouble
                                                  ()));
    lineEdit_retour_vente_remise_prix->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record, YerothDatabaseTableColumn::REMISE_PRIX).toDouble()));

    QString remise_pourcentage(QString("%1 %").arg(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerothDatabaseTableColumn::
                                                    REMISE_POURCENTAGE)));

    lineEdit_retour_vente_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_retour_vente_tva->setText(GET_CURRENCY_STRING_NUM
                                       (GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::MONTANT_TVA).
                                        toDouble()));
    lineEdit_retour_vente_montant_total->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record,
              YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_retour_vente_date_peremption->setDate(record.value
                                                   (YerothDatabaseTableColumn::
                                                    DATE_PEREMPTION).
                                                   toDate());
    dateEdit_retour_vente_date_vente->
    setDate(GET_DATE_FROM_STRING
            (GET_SQL_RECORD_DATA
             (record, YerothDatabaseTableColumn::DATE_VENTE)));

    lineEdit_retour_vente_heure_vente->setText(GET_SQL_RECORD_DATA
                                               (record,
                                                YerothDatabaseTableColumn::HEURE_VENTE));

    lineEdit_retour_vente_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA
                                                           (record,
                                                            YerothDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_retour_vente_nom_caissier->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_retour_vente_localisation->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::LOCALISATION));


    comboBox_retour_vente_nom_client->populateComboBoxRawString
    (YerothDatabase::CLIENTS, YerothDatabaseTableColumn::NOM_ENTREPRISE);


    YerothSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'").arg
                           (YerothDatabaseTableColumn::ID,
                            GET_SQL_RECORD_DATA(record,
                                                YerothDatabaseTableColumn::
                                                CLIENTS_ID)));

    clientsTableModel.yerothSetFilter_WITH_where_clause(clientsIdFilter);

    int clientsTableModelRowCount =
    		clientsTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 3228);

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);

        comboBox_retour_vente_nom_client->setCurrentIndex
        (comboBox_retour_vente_nom_client->findText
         (GET_SQL_RECORD_DATA
          (clientsRecord, YerothDatabaseTableColumn::NOM_ENTREPRISE)));
    }
    else
    {
        comboBox_retour_vente_nom_client->resetYerothComboBox();
    }

    clientsTableModel.resetFilter();

    QString _nom_entreprise_client = GET_SQL_RECORD_DATA(record,
                                                         YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT);

    if (-1 ==
            comboBox_retour_vente_nom_client->findText(_nom_entreprise_client))
    {
        comboBox_retour_vente_nom_client->addItem(_nom_entreprise_client);

        comboBox_retour_vente_nom_client->setCurrentIndex
        (comboBox_retour_vente_nom_client->findText(_nom_entreprise_client));
    }


    int typeDeVente = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::TYPE_DE_VENTE).
                      toInt();

    lineEdit_retour_vente_type_de_vente->
    setText(YerothUtils::_typedeventeToUserViewString.value(typeDeVente));

    tabWidget_ventes->setCurrentIndex(RetourDuneVente);

    return true;
}


bool YerothVentesWindow::afficher_vente_detail()
{
    if (tableView_ventes->rowCount() <= 0)
    {
        return false;
    }

    //_logger->log("afficher_vente_detail]", QString("row: %1").arg(lastSelectedVentesRow));
    QSqlRecord record;

    _allWindows->
    _ventesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    lineEdit_details_reference_produit->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::REFERENCE));
    lineEdit_details_designation->setText(GET_SQL_RECORD_DATA
                                          (record,
                                           YerothDatabaseTableColumn::DESIGNATION));
    lineEdit_details_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    lineEdit_details_categorie->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::CATEGORIE));

    double quantite_vendue = GET_SQL_RECORD_DATA(record,
                                                 YerothDatabaseTableColumn::QUANTITE_VENDUE).
                             toDouble();

    lineEdit_details_quantite_vendue->setText(GET_NUM_STRING
                                              (quantite_vendue));
    lineEdit_details_prix_unitaire->setText(GET_CURRENCY_STRING_NUM
                                            (GET_SQL_RECORD_DATA
                                             (record,
                                              "prix_unitaire").toDouble()));
    lineEdit_details_remise_prix->setText(GET_CURRENCY_STRING_NUM
                                          (GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::REMISE_PRIX).
                                           toDouble()));

    QString remise_pourcentage(QString("%1 %").arg(GET_SQL_RECORD_DATA
                                                   (record,
                                                    YerothDatabaseTableColumn::
                                                    REMISE_POURCENTAGE)));

    lineEdit_details_remise_pourcentage->setText(remise_pourcentage);
    lineEdit_details_tva->setText(GET_CURRENCY_STRING_NUM
                                  (GET_SQL_RECORD_DATA
                                   (record,
                                    YerothDatabaseTableColumn::
                                    MONTANT_TVA).toDouble()));
    lineEdit_details_montant_total->
    setText(GET_CURRENCY_STRING_NUM
            (GET_SQL_RECORD_DATA
             (record,
              YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE).toDouble()));

    dateEdit_details_date_peremption->setDate(record.value
                                              (YerothDatabaseTableColumn::
                                               DATE_PEREMPTION).toDate());
    dateEdit_details_date_vente->
    setDate(GET_DATE_FROM_STRING
            (GET_SQL_RECORD_DATA
             (record, YerothDatabaseTableColumn::DATE_VENTE)));

    lineEdit_details_heure_vente->setText(GET_SQL_RECORD_DATA
                                          (record,
                                           YerothDatabaseTableColumn::HEURE_VENTE));

    lineEdit_details_reference_recu_de_vente->setText(GET_SQL_RECORD_DATA
                                                      (record,
                                                       YerothDatabaseTableColumn::REFERENCE_RECU_VENDU));

    lineEdit_details_nom_caissier->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::NOM_CAISSIER));
    lineEdit_details_localisation->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::LOCALISATION));

    YerothSqlTableModel &clientsTableModel =
                    _allWindows->getSqlTableModel_clients();

    QString clientsIdFilter;

    clientsIdFilter.append(QString("%1 = '%2'").arg
                           (YerothDatabaseTableColumn::ID,
                            GET_SQL_RECORD_DATA(record,
                                                YerothDatabaseTableColumn::
                                                CLIENTS_ID)));

    clientsTableModel.yerothSetFilter_WITH_where_clause(clientsIdFilter);

    int clientsTableModelRowCount =
    		clientsTableModel.easySelect("src/windows/yeroth-erp-ventes-window.cpp", 3368);

    if (clientsTableModelRowCount > 0)
    {
        QSqlRecord clientsRecord = clientsTableModel.record(0);
        lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA
                                             (clientsRecord,
                                              YerothDatabaseTableColumn::NOM_ENTREPRISE));
    }
    else
    {
        lineEdit_details_nom_client->setText(YerothUtils::EMPTY_STRING);
    }

    clientsTableModel.resetFilter();

    lineEdit_details_nom_client->setText(GET_SQL_RECORD_DATA
                                         (record,
                                          YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));

    int typeDeVente = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::TYPE_DE_VENTE).
                      toInt();

    lineEdit_details_type_de_vente->
    setText(YerothUtils::_typedeventeToUserViewString.value(typeDeVente));

    tabWidget_ventes->setCurrentIndex(AfficherVenteAuDetail);

    return true;
}


void YerothVentesWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_ventes_element_de_vente_resultat->clear();

    YEROTH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothVentesWindow::reinitialiser_recherche()
{
    //  _logger->log("reinitialiser_recherche");
    lineEdit_ventes_element_de_vente_resultat->clear();

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_stocks_vendu());

    resetLineEditsQCompleters((QObject *) this);

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser && aUser->isVendeur())
    {
        lineEdit_ventes_nom_caissier->setYerothEnabled(false);
        lineEdit_ventes_nom_caissier->setText(aUser->nom_complet());
    }
    else
    {
        lineEdit_ventes_nom_caissier->clear();
        lineEdit_ventes_nom_caissier->setYerothEnabled(true);
    }

    afficher_ventes();

    lineEdit_ventes_terme_recherche->setFocus();
}


void YerothVentesWindow::refineYerothLineEdits()
{
    _ventesDateFilter.clear();

    _ventesDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerothDatabaseTableColumn::DATE_VENTE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_debut->
                                                       date()),
                              YerothDatabaseTableColumn::DATE_VENTE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_ventes_fin->
                                                       date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_ventes();
}
