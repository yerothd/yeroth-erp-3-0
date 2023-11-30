/*
 * yeroth-erp-achats-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-achats-window.hpp"


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


unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_STOCKS_ID(1);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_REFERENCE(3);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_DESIGNATION(2);

unsigned int YerothAchatsWindow::PDF_LISTING_COLUMN_CATEGORIE(4);


YerothAchatsWindow::YerothAchatsWindow()
:YerothWindowsCommons("yeroth-erp-achats"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::ACHATS),
 _logger(new YerothLogger("YerothAchatsWindow")),
 _pushButton_achats_filtrer_font(0),
 _show_ASSET(false),
 _curAchatSqlTableModel(0)
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("fiche des achats des IMMOBILISATIONS et des stocks"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_achats);


    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
            << pushButton_afficher;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
		<< action_parametrer_les_impressions
		<< actionAfficherPDF << actionAfficher_achat_au_detail;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_achats)


    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_170,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    setup_select_configure_dbcolumn(YerothDatabase::ACHATS);


    _lineEditsToANDContentForSearch.insert(&lineEdit_achats_terme_recherche,
                                           YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString
        .insert(YerothDatabaseTableColumn::REFERENCE);

    _yeroth_WINDOW_references_dbColumnString
        .insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::ACHATS);

    reinitialiser_colones_db_visibles();

    _curAchatSqlTableModel = &_allWindows->getSqlTableModel_achats();

    if (!_show_ASSET)
    {
        radioButton_lister_des_stocks->setChecked(true);
    }
    else
    {
        radioButton_lister_des_IMMOBILISATIONS->setChecked(true);
    }

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setupDateTimeEdits();

    _pushButton_achats_filtrer_font =
                    new QFont(pushButton_achats_filtrer->font());

    tableView_achats->setSqlTableName(&YerothDatabase::ACHATS);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);
    pushButton_achats_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));


    //Menu actions

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


    connect(action_BASCULER_STOCK_IMMOBILISATION,
            SIGNAL(triggered()),
            this, SLOT(handle_ACTION_CHOICE_IMMOBILISATION_STOCK()));


    connect(radioButton_lister_des_stocks,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_RADIO_BUTTONS_immobilisations_STOCKS(bool)));

    connect(radioButton_lister_des_IMMOBILISATIONS,
            SIGNAL(toggled(bool)),
            this, SLOT(handle_RADIO_BUTTONS_immobilisations_STOCKS(bool)));

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
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionVentes, SIGNAL(triggered()), this, SLOT(ventes()));
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

    connect(actionAfficher_achat_au_detail, SIGNAL(triggered()),
            this, SLOT(afficher_au_detail()));

    connect(tableView_achats, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(afficher_au_detail(const QModelIndex &)));


    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING
    (YerothDatabaseTableColumn::STOCKS_ID);

    setupShortcuts();
}

YerothAchatsWindow::~YerothAchatsWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR delete _logger;
}


void YerothAchatsWindow::handle_ACTION_CHOICE_IMMOBILISATION_STOCK()
{
    if (radioButton_lister_des_stocks->isChecked())
    {
        radioButton_lister_des_IMMOBILISATIONS->setChecked(true);
    }
    else if (radioButton_lister_des_IMMOBILISATIONS->isChecked())
    {
        radioButton_lister_des_stocks->setChecked(true);
    }
}


void YerothAchatsWindow::handle_RADIO_BUTTONS_immobilisations_STOCKS(bool
                                                                     is_toggle)
{
    if (radioButton_lister_des_stocks->isChecked())
    {
        action_BASCULER_STOCK_IMMOBILISATION->setText(QObject::tr
                                                      ("AFFICHER ACHATS IMMOBILISATIONS"));

        _show_ASSET = false;
    }
    else if (radioButton_lister_des_IMMOBILISATIONS->isChecked())
    {
        action_BASCULER_STOCK_IMMOBILISATION->setText(QObject::tr
                                                      ("AFFICHER ACHATS stocks"));

        _show_ASSET = true;
    }

    textChangedSearchLineEditsQCompleters();
}


void YerothAchatsWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_element_achats->setVisible(IS_VISIBLE);
    comboBox_element_achats_condition->setVisible(IS_VISIBLE);
    lineEdit_element_achats_resultat->setVisible(IS_VISIBLE);
    pushButton_achats_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_achats_filtrer->setVisible(IS_VISIBLE);
}


bool YerothAchatsWindow::filtrer_achats()
{
    QString achatsTableColumnValue(lineEdit_element_achats_resultat->text());

    if (achatsTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerothQMessageBox::information(this, QObject::tr("filtrer"), msg);

        return false;
    }

    reinitialiser_recherche();

    QString achatsTableColumnProperty(comboBox_element_achats->currentText());

    QString mathOperator(comboBox_element_achats_condition->currentText());

    QString
    REAL_DB_ID_NAME_achatsTableColumnProperty
    (YerothDatabaseTableColumn::_tableColumnToUserViewString.key
     (achatsTableColumnProperty));

    QString filterString;

    filterString.append(QString("%1 %2 %3").arg
                        (REAL_DB_ID_NAME_achatsTableColumnProperty,
                         mathOperator, achatsTableColumnValue));

    _curAchatSqlTableModel->yerothSetFilter_WITH_where_clause(filterString);

    int resultRows =
    		_curAchatSqlTableModel->easySelect("src/windows/purchase/yeroth-erp-achats-window.cpp", 329);

    if (resultRows > 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau(*_curAchatSqlTableModel);

        YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "achats - filtrer");

        return true;
    }
    else
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "achats - filtrer");

        return false;
    }

    set_filtrer_font();

    return false;
}


void YerothAchatsWindow::initialiser_FILTRES()
{
    comboBox_element_achats->setVisible(false);
    comboBox_element_achats_condition->setVisible(false);
    lineEdit_element_achats_resultat->setVisible(false);
    pushButton_achats_reinitialiser_filtre->setVisible(false);
    pushButton_achats_filtrer->setVisible(false);
}


void YerothAchatsWindow::populateComboBoxes()
{
    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerothDatabaseTableColumn::ENREGISTREUR_STOCK);


    comboBox_element_string_db->clear();

    comboBox_element_string_db->addItems(aQStringList,
                                         YerothDatabaseTableColumn::
                                         _tableColumnToUserViewString);

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


    comboBox_element_achats->clear();

    comboBox_element_achats->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");


    comboBox_element_achats_condition->clear();

    comboBox_element_achats_condition->addItems(aQStringList);
}


void YerothAchatsWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_achats_nombre_de_lignes_par_page;

    lineEdit_achats_terme_recherche->enableForSearch(QObject::tr
                                                     ("terme à rechercher (désignation de l'article)"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_achats_total_achats->setYerothEnabled(false);

    lineEdit_achats_nombre_dachats->setYerothEnabled(false);

    lineEdit_element_achats_resultat->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_achats_nombre_de_lignes_par_page, tableView_achats);
}


void YerothAchatsWindow::setupDateTimeEdits()
{
    dateEdit_achats_debut->
    setStartDate(YerothERPConfig::GET_YEROTH_PAGING_DEFAULT_START_DATE());

    dateEdit_achats_fin->setStartDate(GET_CURRENT_DATE);

    _achatsDateFilter.clear();

    _achatsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerothDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_debut->
                                                       date()),
                              YerothDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_fin->
                                                       date())));

    connect(dateEdit_achats_debut,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerothLineEdits()));

    connect(dateEdit_achats_fin,
            SIGNAL(dateChanged(const QDate &)),
            this, SLOT(refineYerothLineEdits()));
}


void YerothAchatsWindow::setupShortcuts()
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


void YerothAchatsWindow::resetFilter(YerothSqlTableModel *achatsTableModel)
{
    _curAchatSqlTableModel = achatsTableModel;

    if (0 != _curAchatSqlTableModel)
    {
        _curAchatSqlTableModel->resetFilter();
    }

    lineEdit_achats_terme_recherche->myClear();

    dateEdit_achats_debut->reset();
    dateEdit_achats_fin->reset();
}


void YerothAchatsWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();

    if (0 != _curAchatSqlTableModel)
    {
        lister_les_elements_du_tableau(*_curAchatSqlTableModel);
    }
}


void YerothAchatsWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    clearSearchFilter();

    QString searchTerm(lineEdit_achats_terme_recherche->text());

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

    YEROTH_SEARCH_DB_TABLE_WITH_MORE_THAN_ONE_SEARCH_FILTER;

	if (_show_ASSET)
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 <= 0").arg
                                 (YerothDatabaseTableColumn::PRIX_VENTE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 <= 0").arg
                                 (YerothDatabaseTableColumn::PRIX_VENTE));
        }
    }
    else
    {
        if (_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" %1 > 0").arg
                                 (YerothDatabaseTableColumn::PRIX_VENTE));
        }
        else
        {
            _searchFilter.append(QString(" AND %1 > 0").arg
                                 (YerothDatabaseTableColumn::PRIX_VENTE));
        }
    }




    QString finalSearchFilter(_achatsDateFilter);

    if (!_searchFilter.isEmpty())
    {
        QString
        searchFilterWithDate(QString("%1 AND (%2)").arg
                             (_achatsDateFilter, _searchFilter));

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
                 ("++ YerothAchatsWindow::textChangedSearchLineEditsQCompleters(): %1").
                 arg(_yerothSqlTableModel->lastError().text());
    }

    handle_some_actions_tools_enabled();
}


void YerothAchatsWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerothDatabaseTableColumn::DESIGNATION
            << YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
            << YerothDatabaseTableColumn::CATEGORIE
            << YerothDatabaseTableColumn::PRIX_DACHAT
            << YerothDatabaseTableColumn::PRIX_VENTE
            << YerothDatabaseTableColumn::MARGE_BENEFICIAIRE
            << YerothDatabaseTableColumn::DATE_ENTREE
            << YerothDatabaseTableColumn::QUANTITE_TOTALE;

}


void YerothAchatsWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (tableView_achats->rowCount() > 0)
    {
        QMenu menu(this);
        menu.setPalette(toolBar_achatsWindow->palette());
        menu.addAction(actionAfficher_achat_au_detail);
        menu.exec(event->globalPos());
    }
}


void YerothAchatsWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YEROTH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    populateComboBoxes();

    setupLineEdits();

    setupLineEditsQCompleters((QObject *) this);

    setYerothSqlTableModel(_curAchatSqlTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_achats_terme_recherche->setFocus();

    setVisible(true);

    afficher_achats();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("fiche des achats des IMMOBILISATIONS et des stocks"),
    								 _curAchatSqlTableModel);

    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();
}


void YerothAchatsWindow::definirManager()
{
    _logger->log("definirManager");

    tableView_achats->setVisible(true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerothAchatsWindow::definirVendeur()
{
    _logger->log("definirVendeur - definirPasDeRole()");

    _logger->log("definirManager");

    tableView_achats->setVisible(true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->disable(this);
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
}


void YerothAchatsWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    tableView_achats->setVisible(true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curAchatSqlTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_achats_filtrer->enable(this, SLOT(filtrer_achats()));
    pushButton_achats_reinitialiser_filtre->enable(this,
                                                   SLOT
                                                   (reinitialiser_elements_filtrage
                                                    ()));

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher->enable(this, SLOT(afficher_au_detail()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_ventes->disable(this);
}


void YerothAchatsWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    tableView_achats->setVisible(false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVentes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_add_condition_SEARCH->disable(this);

    pushButton_achats_filtrer->disable(this);
    pushButton_achats_reinitialiser_filtre->disable(this);

    pushButton_entrer->disable(this);
    pushButton_afficher->disable(this);
    pushButton_stocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_menu_principal->disable(this);
}

void YerothAchatsWindow::afficher_au_detail()
{
    _logger->log("afficher_au_detail");

    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1
            && _curAchatSqlTableModel->rowCount() > 0)
    {
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
                                                        _curAchatSqlTableModel);
        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 achat à afficher les détails."));

        return;
    }
}


void YerothAchatsWindow::afficher_au_detail(const QModelIndex &modelIndex)
{
    _logger->log("afficher_au_detail(const QModelIndex &)");

    if (_curAchatSqlTableModel->rowCount() > 0)
    {
        //qDebug() << "++ test" << modelIndex.row();
        _allWindows->_achatsDetailWindow->rendreVisible(_curStocksTableModel,
                                                        _curAchatSqlTableModel);

        rendreInvisible();
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("détails"),
                                       QObject::tr
                                       ("Sélectionnez 1 achat à afficher les détails."));

        return;
    }
}


void YerothAchatsWindow::refineYerothLineEdits()
{
    _achatsDateFilter.clear();

    _achatsDateFilter.append(QString(" ( %1 >= '%2' AND %3 <= '%4' ) ").arg
                             (YerothDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_debut->
                                                       date()),
                              YerothDatabaseTableColumn::DATE_ENTREE,
                              DATE_TO_DB_FORMAT_STRING(dateEdit_achats_fin->
                                                       date())));

    setupLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void YerothAchatsWindow::reinitialiser_elements_filtrage()
{
    _logger->log("reinitialiser_elements_filtrage");

    lineEdit_element_achats_resultat->clear();

    YEROTH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothAchatsWindow::reinitialiser_recherche()
{
    _logger->log("reinitialiser_recherche");

    lineEdit_nom_element_string_db->clear();

    lineEdit_element_achats_resultat->clear();

    setCurrentlyFiltered(false);

    resetFilter(&_allWindows->getSqlTableModel_achats());

    resetLineEditsQCompleters((QObject *) this);

    afficher_achats();
}


void YerothAchatsWindow::lister_les_elements_du_tableau(YerothSqlTableModel &
                                                        achatSqlTableModel)
{
    int curAchatsTableModelRowCount =
    		achatSqlTableModel.easySelect("src/windows/purchase/yeroth-erp-achats-window.cpp", 972);

    double quantite_totale = 0.0;
    double prix_dachat = 0.0;
    double montant_total_achat = 0.0;

    double montant_total = 0.0;

    QSqlRecord aRecord;

    for (int k = 0; k < curAchatsTableModelRowCount; ++k)
    {
        aRecord.clear();

        aRecord = achatSqlTableModel.record(k);

        quantite_totale =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

        prix_dachat =
                        GET_SQL_RECORD_DATA(aRecord,
                                            YerothDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        montant_total_achat = prix_dachat * quantite_totale;

        montant_total += montant_total_achat;
    }

    lineEdit_achats_total_achats->setText(GET_CURRENCY_STRING_NUM
                                          (montant_total));


    tableView_achats->queryYerothTableViewCurrentPageContentRow
    (achatSqlTableModel);

    tableView_show_or_hide_columns(*tableView_achats);

    int rowCount = tableView_achats->rowCount();

    lineEdit_achats_nombre_dachats->setText(GET_NUM_STRING(rowCount));



    if (_show_ASSET)
    {
        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("fiche des achats des IMMOBILISATIONS"),
        								 _curAchatSqlTableModel);
    }
    else
    {
        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("fiche des achats de stocks"),
        								 _curAchatSqlTableModel);
    }
}


void YerothAchatsWindow::afficher_stock_selectioner(const QString &stockName)
{
    _logger->log("afficher_stock_selectioner(const QString &)");

    //qDebug() << QString("afficher_stock_selectioner(%1)").arg(stockName);

    QString
    filter(GENERATE_SQL_IS_STMT
           (YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curAchatSqlTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curAchatSqlTableModel->easySelect("src/windows/purchase/yeroth-erp-achats-window.cpp", 1050) > 0)
    {
        lister_les_elements_du_tableau(*_curAchatSqlTableModel);
    }
}


void YerothAchatsWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_achats_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_achats_filtrer_font->setUnderline(false);
    }

    pushButton_achats_filtrer->setFont(*_pushButton_achats_filtrer_font);
}


bool YerothAchatsWindow::export_csv_file()
{
    _logger->log("export_csv_file");

    bool success = false;

    QList<int> tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_achats,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-stock-purchase-listing",
                                               "stock purchase listing file");
    }
    else
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_achats,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-fiche-achats",
                                               "fiche des achats");
    }

    return success;
}


QString YerothAchatsWindow::get_latex_template_print_pdf_content()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_lister_achats_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_lister_achats_tex;
    }

    return _latex_template_print_pdf_content;
}
