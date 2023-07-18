/*
 * yeroth-erp-charges-financieres-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-charges-financieres-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/QThread>

#include <QtCore/QProcess>

#include <QtCore/qmath.h>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QCompleter>

#include <unistd.h>


/**
 * yeroth-erp-windows.hpp cannot be included in
 * the header file because it will caused a circular
 * dependency that will lead to an unsuccessful
 * compilation.
 */


unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothChargesFinancieresWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerothChargesFinancieresWindow::YerothChargesFinancieresWindow()
:YerothWindowsCommons("yeroth-erp-achats-de-services"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::CHARGES_FINANCIERES),
 _logger(new YerothLogger("YerothAchatsDEServicesWindow")),
 _pushButton_charges_financieres_filtrer_font(0),
 _cur_CHARGES_FINANCIERES_SqlTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("fiche des charges financières"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

	YerothDatabaseTableColumn::_tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::CATEGORIE,
				QObject::tr("LIGNE BUDGÉTAIRE"));

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_charges_financieres);


    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
        << pushButton_afficher
        << pushButton_supprimer;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< action_CREER_A_PARTIR_DE_CECI
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionSupprimerUneCHARGE_FINANCIERE
        << actionAfficher_charge_financiere_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION
    (tableView_charges_financieres) QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerothDatabase::CHARGES_FINANCIERES);


    _lineEditsToANDContentForSearch
		.insert(&lineEdit_charges_financieres_terme_recherche,
				YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString
    	.insert(YerothDatabaseTableColumn::REFERENCE);

    _yeroth_WINDOW_references_dbColumnString
    	.insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT);


    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::CHARGES_FINANCIERES);


    reinitialiser_colones_db_visibles();


    _cur_CHARGES_FINANCIERES_SqlTableModel =
    		&_allWindows->getSqlTableModel_charges_financieres();


    populateComboBoxes();


    setupLineEdits();


    setupLineEditsQCompleters((QObject *) this);


    setupDateTimeEdits();


    _pushButton_charges_financieres_filtrer_font =
                    new QFont(pushButton_charges_financieres_filtrer->font());


    tableView_charges_financieres->
    setSqlTableName(&YerothDatabase::CHARGES_FINANCIERES);


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_charge_financiere_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUneCHARGE_FINANCIERE,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_charges_financieres_filtrer->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_entrer->disable(this);
    pushButton_paiements->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));


    connect(action_CREER_A_PARTIR_DE_CECI,
    		SIGNAL(triggered()),
			this,
			SLOT(SLOT_CREER_A_PARTIR_DE_CECI_charge_financiere()));


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
            SLOT(selectionner_champs_db_visibles_CHARGES_FINANCIERES()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_filtres_checkBox(int)));


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionSupprimerUneCHARGE_FINANCIERE, SIGNAL(triggered()), this,
            SLOT(supprimer_une_charge_financiere()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this,
            SLOT(export_csv_file()));
    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this,
            SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionReinitialiserRecherche, SIGNAL(triggered()), this,
            SLOT(reinitialiser_recherche()));
    connect(actionReinitialiserElementsDeFiltrage, SIGNAL(triggered()), this,
            SLOT(reinitialiser_elements_filtrage()));
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

    connect(actionAfficher_charge_financiere_au_detail, SIGNAL(triggered()),
            this, SLOT(afficher_au_detail()));

    connect(tableView_charges_financieres,
            SIGNAL(doubleClicked(const QModelIndex &)), this,
            SLOT(afficher_au_detail(const QModelIndex &)));

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING
    (YerothDatabaseTableColumn::STOCKS_ID);

    setupShortcuts();
}

YerothChargesFinancieresWindow::~YerothChargesFinancieresWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerothChargesFinancieresWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element_charges_financieres->setVisible(IS_VISIBLE);
    comboBox_element_charges_financieres_condition->setVisible(IS_VISIBLE);
    lineEdit_element_charges_financieres_resultat->setVisible(IS_VISIBLE);
    pushButton_charges_financieres_reinitialiser_filtre->setVisible
    (IS_VISIBLE);
    pushButton_charges_financieres_filtrer->setVisible(IS_VISIBLE);
}


bool YerothChargesFinancieresWindow::filtrer_achats()
{
    QString
    achatsTableColumnValue
    (comboBox_element_charges_financieres_condition->currentText());

    if (achatsTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerothQMessageBox::information(this, QObject::tr("filtrer"), msg);

        return false;
    }

    reinitialiser_recherche();

    QString
    achatsTableColumnProperty
    (comboBox_element_charges_financieres->currentText());

    QString
    mathOperator(comboBox_element_charges_financieres_condition->currentText
                 ());

    QString
    REAL_DB_ID_NAME_achatsTableColumnProperty
    (YerothDatabaseTableColumn::_tableColumnToUserViewString.key
     (achatsTableColumnProperty));

    QString filterString;

    filterString.append(QString("%1 %2 %3").arg
                        (REAL_DB_ID_NAME_achatsTableColumnProperty,
                         mathOperator, achatsTableColumnValue));

    _cur_CHARGES_FINANCIERES_SqlTableModel->yerothSetFilter_WITH_where_clause
    (filterString);

    int resultRows =
    		_cur_CHARGES_FINANCIERES_SqlTableModel
				->easySelect("src/windows/yeroth-erp-charges-financieres-window.cpp", 296);

    if (resultRows > 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau
        (*_cur_CHARGES_FINANCIERES_SqlTableModel);

        YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "achats-de-services - filtrer");

        return true;
    }
    else
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this,
                                                  "achats-de-services - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerothChargesFinancieresWindow::populateComboBoxes()
{
    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR);
    aQStringList.removeAll(YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT);
    aQStringList.removeAll(YerothDatabaseTableColumn::ENREGISTREUR_STOCK);
    aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION);
    aQStringList.removeAll(YerothDatabaseTableColumn::LOCALISATION_STOCK);

//      qDebug() << "++ test: " << aQStringList;

    QString aDBColumnElementString;

    for (int k = 0; k < aQStringList.size(); ++k)
    {
        aDBColumnElementString = aQStringList.at(k);

        if (!YerothUtils::isEqualCaseInsensitive
                (YerothDatabaseTableColumn::REFERENCE, aDBColumnElementString))
        {
            comboBox_element_string_db
				->addItem(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
							(aDBColumnElementString));
        }
    }


    comboBox_element_string_db
		->insertItem(0,
					 YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
					 	 (YerothDatabaseTableColumn::REFERENCE));

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::MARGE_BENEFICIAIRE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::MONTANT_TVA));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_VENTE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_UNITAIRE));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::QUANTITE_TOTALE));


    comboBox_element_charges_financieres->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_element_charges_financieres_condition->addItems(aQStringList);
}


void YerothChargesFinancieresWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_charges_financieres_nombre_de_lignes_par_page;

    lineEdit_charges_financieres_terme_recherche->enableForSearch
    (QObject::tr("terme à rechercher (désignation de l'article)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_charges_financieres_nombre_dachats->setYerothEnabled(false);

    lineEdit_charges_financieres_total_achats->setYerothEnabled(false);

    comboBox_element_charges_financieres_condition->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_charges_financieres_nombre_de_lignes_par_page,
     tableView_charges_financieres);
}


void YerothChargesFinancieresWindow::setupDateTimeEdits()
{
    dateEdit_charges_financieres_debut->
    setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_charges_financieres_fin->setStartDate(GET_CURRENT_DATE);

    _charges_financieresDateFilter.clear();

    _charges_financieresDateFilter.append(QString
                                          (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                          arg
                                          (YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_charges_financieres_debut->date
                                            ()),
                                           YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_charges_financieres_fin->
                                            date())));

    connect(dateEdit_charges_financieres_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerothLineEdits()));

    connect(dateEdit_charges_financieres_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerothLineEdits()));
}


void YerothChargesFinancieresWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);

    actionReinitialiserRecherche->
    setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothChargesFinancieresWindow::resetFilter(YerothSqlTableModel *
                                                 achatsTableModel)
{
    _cur_CHARGES_FINANCIERES_SqlTableModel = achatsTableModel;

    if (0 != _cur_CHARGES_FINANCIERES_SqlTableModel)
    {
        _cur_CHARGES_FINANCIERES_SqlTableModel->resetFilter();
    }

    lineEdit_charges_financieres_terme_recherche->myClear();

    dateEdit_charges_financieres_debut->reset();
    dateEdit_charges_financieres_fin->reset();
}


void YerothChargesFinancieresWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();

    if (0 != _cur_CHARGES_FINANCIERES_SqlTableModel)
    {
        lister_les_elements_du_tableau
        (*_cur_CHARGES_FINANCIERES_SqlTableModel);
    }
}


void YerothChargesFinancieresWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_element_charges_financieres_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_charges_financieres_terme_recherche->text());

    if (!searchTerm.isEmpty())
    {
        QStringList searchTermList = searchTerm.split(QRegExp("\\s+"));

        QString partSearchTerm;

        int lastIdx = searchTermList.size() - 1;

        for (int k = 0; k < searchTermList.size(); ++k)
        {
            partSearchTerm = searchTermList.at(k);
            //qDebug() << "++ searchTermList: " << partSearchTerm;

            _searchFilter.append(QString("%1").arg(GENERATE_SQL_LIKE_STMT
                                                   (YerothDatabaseTableColumn::DESIGNATION,
                                                    partSearchTerm)));

            if (k != lastIdx)
            {
                _searchFilter.append(" AND ");
            }
        }
    }


    YerothWindowsCommons::setYerothLineEditQCompleterSearchFilter
    (_searchFilter);


    YerothLineEdit *aYerothLineEdit = 0;

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

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

    YEROTH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER
    QString finalSearchFilter(_charges_financieresDateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_charges_financieresDateFilter,
                              _searchFilter));

        finalSearchFilter = searchFilterWithDate;
    }

    _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(finalSearchFilter);

    if (_yerothSqlTableModel->select())
    {
        lister_les_elements_du_tableau(*_yerothSqlTableModel);
    }
    else
    {
        qDebug() <<
                 QString
                 ("++ YerothAchatsDEServicesWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothChargesFinancieresWindow::initialiser_FILTRES()
{
    comboBox_element_charges_financieres->setVisible(false);
    comboBox_element_charges_financieres_condition->setVisible(false);
    lineEdit_element_charges_financieres_resultat->setVisible(false);
    pushButton_charges_financieres_reinitialiser_filtre->setVisible(false);
    pushButton_charges_financieres_filtrer->setVisible(false);
}


void YerothChargesFinancieresWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerothDatabaseTableColumn::DESIGNATION
            << YerothDatabaseTableColumn::CATEGORIE
            << YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
            << YerothDatabaseTableColumn::QUANTITE_TOTALE
            << YerothDatabaseTableColumn::PRIX_DACHAT
            << YerothDatabaseTableColumn::PRIX_VENTE
            << YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
            << YerothDatabaseTableColumn::DATE_DE_COMMANDE;
}


void YerothChargesFinancieresWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_charges_financieres->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_charges_financieresWindow->palette());
        menu.addAction(actionAfficher_charge_financiere_au_detail);
        menu.addAction(actionSupprimerUneCHARGE_FINANCIERE);
        menu.addAction(action_CREER_A_PARTIR_DE_CECI);
        menu.exec(event->globalPos());
    }
}


void YerothChargesFinancieresWindow::rendreVisible(YerothSqlTableModel *
                                                   stocksTableModel)
{
	retranslateUi(this);

    YEROTH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerothSqlTableModel(_cur_CHARGES_FINANCIERES_SqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_charges_financieres_terme_recherche->setFocus();

    setVisible(true);

    afficher_achats();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("fiche des charges financières"),
    					  	  	  	 _cur_CHARGES_FINANCIERES_SqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerothChargesFinancieresWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_charge_financiere_au_detail, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUneCHARGE_FINANCIERE,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _cur_CHARGES_FINANCIERES_SqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_charges_financieres_filtrer->enable(this,
                                                   SLOT(filtrer_achats()));
    pushButton_charges_financieres_reinitialiser_filtre->enable(this,
                                                                SLOT
                                                                (reinitialiser_elements_filtrage
                                                                 ()));

    pushButton_supprimer->enable(this,
                                 SLOT(supprimer_une_charge_financiere()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_entrer->enable(this, SLOT(entrer_une_charge_financiere()));
    pushButton_paiements->enable(this, SLOT(paiements()));
}


void YerothChargesFinancieresWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_CREER_A_PARTIR_DE_CECI, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionAfficher_charge_financiere_au_detail, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerUneCHARGE_FINANCIERE,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);

    pushButton_charges_financieres_filtrer->disable(this);
    pushButton_charges_financieres_reinitialiser_filtre->disable(this);

    pushButton_supprimer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_entrer->disable(this);
    pushButton_paiements->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothChargesFinancieresWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1
            && _cur_CHARGES_FINANCIERES_SqlTableModel->rowCount() > 0)
    {
        _allWindows->_charges_financieres_detailsWindow->rendreVisible
        (_curStocksTableModel, _cur_CHARGES_FINANCIERES_SqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 CHARGE FINANCIÈRE à afficher les détails."));

        return;
    }
}


void YerothChargesFinancieresWindow::afficher_au_detail(const QModelIndex &
                                                        modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    if (_cur_CHARGES_FINANCIERES_SqlTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_charges_financieres_detailsWindow->rendreVisible
        (_curStocksTableModel, _cur_CHARGES_FINANCIERES_SqlTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 achat au fournisseur à afficher les détails."));

        return;
    }
}


void YerothChargesFinancieresWindow::refineYerothLineEdits()
{
    _charges_financieresDateFilter.clear();

    _charges_financieresDateFilter.append(QString
                                          (" ( %1 >= '%2' AND %3 <= '%4' ) ").
                                          arg
                                          (YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_charges_financieres_debut->date
                                            ()),
                                           YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                                           DATE_TO_DB_FORMAT_STRING
                                           (dateEdit_charges_financieres_fin->
                                            date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void YerothChargesFinancieresWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_element_charges_financieres_resultat->clear();

    YEROTH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothChargesFinancieresWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    comboBox_element_charges_financieres_condition->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_charges_financieres());

    resetLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void
YerothChargesFinancieresWindow::lister_les_elements_du_tableau
(YerothSqlTableModel &achatSqlTableModel)
{
    int curAchatsTableModelRowCount =
    		achatSqlTableModel.easySelect("src/windows/yeroth-erp-charges-financieres-window.cpp", 865);

    double
    prix_dachat = 0.0;

    double
    montant_total = 0.0;

    QSqlRecord
    aRecord;

    for (int k = 0; k < curAchatsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = achatSqlTableModel.record(k);

        prix_dachat =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        montant_total += prix_dachat;
    }

    lineEdit_charges_financieres_total_achats->setText(GET_CURRENCY_STRING_NUM
                                                       (montant_total));


    tableView_charges_financieres->queryYerothTableViewCurrentPageContentRow
    (achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_charges_financieres);

    int
    rowCount = tableView_charges_financieres->rowCount();

    lineEdit_charges_financieres_nombre_dachats->setText(GET_NUM_STRING
                                                         (rowCount));
}


void YerothChargesFinancieresWindow::afficher_stock_selectioner(const QString &
                                                                stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    QString
    filter(GENERATE_SQL_IS_STMT
           (YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _cur_CHARGES_FINANCIERES_SqlTableModel->yerothSetFilter_WITH_where_clause
    (filter);

    if (_cur_CHARGES_FINANCIERES_SqlTableModel->easySelect("src/windows/yeroth-erp-charges-financieres-window.cpp", 922) > 0)
    {
        lister_les_elements_du_tableau
        (*_cur_CHARGES_FINANCIERES_SqlTableModel);
    }
}


void YerothChargesFinancieresWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_charges_financieres_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_charges_financieres_filtrer_font->setUnderline(false);
    }

    pushButton_charges_financieres_filtrer->setFont
    (*_pushButton_charges_financieres_filtrer_font);
}


bool YerothChargesFinancieresWindow::supprimer_une_charge_financiere()
{
    if (_cur_CHARGES_FINANCIERES_SqlTableModel->rowCount() <= 0)
    {
        YerothQMessageBox::information(this, QObject::tr("supprimer"),
                                       QObject::tr
                                       ("Sélectionnez 1 charge financière à supprimer."));

        return false;
    }

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    QSqlRecord _charge_financiere_record;

    _allWindows->
    	_charges_financieresWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(_charge_financiere_record);

    QString msgSupprimer =
                    QObject::tr("Poursuivre avec la suppression "
                                    "de la charge financière '%1' ?").arg
                    (GET_SQL_RECORD_DATA(_charge_financiere_record,
                                         YerothDatabaseTableColumn::DESIGNATION));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression d'une charge financière"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        QString prix_dachat = GET_SQL_RECORD_DATA(_charge_financiere_record,
                                                  YerothDatabaseTableColumn::
                                                  PRIX_DACHAT);

        QString reference = GET_SQL_RECORD_DATA(_charge_financiere_record,
                                                YerothDatabaseTableColumn::
                                                REFERENCE);

        QString fournisseur = GET_SQL_RECORD_DATA(_charge_financiere_record,
                                                  YerothDatabaseTableColumn::
                                                  NOM_ENTREPRISE_FOURNISSEUR);

        bool resRemoved =
                        _allWindows->
                        _charges_financieresWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        if (resRemoved)
        {
            QString GET_COMPTE_FOURNISSEUR =
                            QString("select %1 from %2 where %3='%4'").arg
                            (YerothDatabaseTableColumn::COMPTE_FOURNISSEUR,
                             YerothDatabase::FOURNISSEURS,
                             YerothDatabaseTableColumn::NOM_ENTREPRISE,
                             fournisseur);

            double a_compte_fournisseur;

            QSqlQuery qsql_query;

            int qsql_query_size =
                            YerothUtils::execQuery(qsql_query, GET_COMPTE_FOURNISSEUR);

            if (qsql_query_size > 0 && qsql_query.next())
            {
                a_compte_fournisseur =
                		qsql_query.value(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR).toDouble();

                a_compte_fournisseur += (-1 * prix_dachat.toDouble());

                QString UPDATE_COMPTE_FOURNISSEUR =
                                QString("UPDATE %1 SET %2='%3' where %4='%5'").arg
                                (YerothDatabase::FOURNISSEURS,
                                 YerothDatabaseTableColumn::COMPTE_FOURNISSEUR,
                                 QString::number(a_compte_fournisseur),
                                 YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                 fournisseur);

                bool success_UPDATE_COMPTE_FOURNISSEUR =
                                YerothUtils::execQuery(UPDATE_COMPTE_FOURNISSEUR);

                if (success_UPDATE_COMPTE_FOURNISSEUR)
                {
                    QString DELETE_CHARGE_FINANCIERE_DES_paiements =
                                    QString
                                    ("delete from %1 WHERE %2='%3' AND %4='%5' AND %6='%7'").
                                    arg(YerothDatabase::PAIEMENTS,
                                        YerothDatabaseTableColumn::MONTANT_PAYE,
                                        prix_dachat,
                                        YerothDatabaseTableColumn::REFERENCE,
                                        reference,
                                        YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                        fournisseur);

                    //              QDEBUG_STRING_OUTPUT_2("DELETE_CHARGE_FINANCIERE_DES_paiements",
                    //                              DELETE_CHARGE_FINANCIERE_DES_paiements);

                    resRemoved = YerothUtils::execQuery(DELETE_CHARGE_FINANCIERE_DES_paiements);

                    QString intitule_de_la_ligne_budgetaire;

                    if (resRemoved)
                    {
                        intitule_de_la_ligne_budgetaire =
                        		GET_SQL_RECORD_DATA(_charge_financiere_record,
                                                    YerothDatabaseTableColumn::CATEGORIE);

                        QSqlRecord ligne_budgetaire_RECORD;

                        YerothSqlTableModel &lignes_budgetaires_TableModel =
                        		_allWindows->getSqlTableModel_lignes_budgetaires();

                        lignes_budgetaires_TableModel
							.yerothSetFilter_WITH_where_clause(
									QString("%1 = '%2'")
										.arg(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
											 intitule_de_la_ligne_budgetaire));

                        int lignesBudgetairesRowCount =
                        		lignes_budgetaires_TableModel
									.easySelect("src/windows/yeroth-erp-charges-financieres-window.cpp", 1067);

                        if (lignesBudgetairesRowCount > 0)
                        {
                        	ligne_budgetaire_RECORD = lignes_budgetaires_TableModel.record(0);

                            QString montant_RESTANT_de_la_ligne_budgetaire =
                            		GET_SQL_RECORD_DATA(ligne_budgetaire_RECORD,
                                                        YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire);

                        	double NOUVEAU_montant_RESTANT_de_la_ligne_budgetaire =
                        			montant_RESTANT_de_la_ligne_budgetaire.toDouble() +
									(-1 * prix_dachat.toDouble());

                        	QString put_BACK_MONEY_on_BUDGET_LINE =
                        				QString("UPDATE %1 SET %2='%3' where %4='%5'")
    										.arg(YerothDatabase::LIGNES_BUDGETAIRES,
    											 YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
												 QString::number(NOUVEAU_montant_RESTANT_de_la_ligne_budgetaire),
    											 YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
												 intitule_de_la_ligne_budgetaire);

                        	lignes_budgetaires_TableModel.resetFilter();

                        	YerothUtils::execQuery(put_BACK_MONEY_on_BUDGET_LINE);
                        }
                        else
                        {
                        	lignes_budgetaires_TableModel.resetFilter();

                            QString msgBudgetLineNOT_FOUND =
                            		QObject::tr ("La LIGNE BUDGÉTAIRE '%1' est "
                            						 "inexistante dans la base de donnée!")
                                            .arg(intitule_de_la_ligne_budgetaire);

                            YerothQMessageBox::warning(this,
                            						   QObject::tr("suppression d'une charge financière - LIGNE BUDGÉTAIRE"),
													   msgBudgetLineNOT_FOUND);
                        }
                    }

                    if (resRemoved)
                    {
                        msgSupprimer =
                                        QObject::tr
                                        ("La charge financière '%1' a été supprimée !").
                                        arg(GET_SQL_RECORD_DATA
                                            (_charge_financiere_record,
                                             YerothDatabaseTableColumn::DESIGNATION));

                        YerothQMessageBox::information(this,
                                                       QObject::tr("suppression d'une charge financière"),
                                                       msgSupprimer,
                                                       QMessageBox::Cancel,
                                                       QMessageBox::Ok);

                        setupLineEditsQCompleters((QObject *) this);

                        lister_les_elements_du_tableau();

                        YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

                        return true;
                    }
                    else
                    {
                        msgSupprimer =
                                        QObject::tr
                                        ("La charge financière '%1' ne pouvait pas "
                                         "être supprimée du tableau des paiements !").arg
                                        (GET_SQL_RECORD_DATA
                                         (_charge_financiere_record,
                                          YerothDatabaseTableColumn::DESIGNATION));

                        YerothQMessageBox::information(this,
                                                       QObject::tr
                                                       ("suppression d'une charge financière"),
                                                       msgSupprimer,
                                                       QMessageBox::Cancel,
                                                       QMessageBox::Ok);

                        YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

                        return false;
                    }
                }
            }
        }
    }
    else
    {
    }

    msgSupprimer =
                    QObject::tr
                    ("La charge financière '%1' ne pouvait pas être supprimée !").arg
                    (GET_SQL_RECORD_DATA
                     (_charge_financiere_record, YerothDatabaseTableColumn::DESIGNATION));

    YerothQMessageBox::information(this,
                                   QObject::tr
                                   ("suppression d'une charge financière"),
                                   msgSupprimer, QMessageBox::Cancel,
                                   QMessageBox::Ok);

    YEROTH_ERP_3_0_ROLLBACK_DATABASE_TRANSACTION;

    return false;
}


bool YerothChargesFinancieresWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_charges_financieres,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-FINANCIAL-EXPENSES-listing",
                                               "financial expenses file");
    }
    else
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_charges_financieres,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-charges-financieres",
                                               "fiche des charges financières");
    }

    return success;
}


QString YerothChargesFinancieresWindow::get_latex_template_print_pdf_content()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_CHARGES_FINANCIERES_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_CHARGES_FINANCIERES_tex;
    }

    return _latex_template_print_pdf_content;
}
