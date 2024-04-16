/*
 * yeroth-erp-marchandises-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-marchandises-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-style.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"


#include <QtSql/QSqlRelationalTableModel>

#include <QtSql/QSqlQuery>

#include <QtCore/qmath.h>

#include <QtWidgets/QCompleter>


YerothMarchandisesWindow::YerothMarchandisesWindow()
:YerothWindowsCommons("yeroth-erp-marchandises"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::MARCHANDISES),
 _logger(new YerothLogger("YerothMarchandisesWindow")),
 _NON_TERMINEES_MARCHANDISE_RE_ENTRANT(false),
 _qteTotaleDarticlesEnStock(0.0),
 _currentlyFiltered(false),
 _lastSelectedRow__ID(0),
 _pushButton_filtrer_font(0),
 _curMarchandisesTableModel(&_allWindows->getSqlTableModel_marchandises())
{
    _windowName = QString("%1 - %2")
                			.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                				 QObject::tr("les marchandises / SERVICES"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_marchandises);


//    _list_yeroth_pushbutton_to_enable_on_positive_tableview_ROW_COUNT
//              << ;


    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau
		<< actionExporter_au_format_csv
        << action_parametrer_les_impressions
        << actionAfficherPDF
        << actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES
        << actionModifierMarchandise;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION
    (tableView_marchandises) QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerothDatabaseTableColumn::STOCK_MINIMUM
            << YerothDatabaseTableColumn::STOCK_MAXIMUM
            << YerothDatabaseTableColumn::STOCK_DALERTE
            << YerothDatabaseTableColumn::IS_SERVICE;


    setup_select_configure_dbcolumn(YerothDatabase::MARCHANDISES);


    _lineEditsToANDContentForSearch
		.insert(&lineEdit_marchandises_terme_recherche, YerothUtils::EMPTY_STRING);

    _yeroth_WINDOW_references_dbColumnString
		.insert(YerothDatabaseTableColumn::REFERENCE);

    YEROTH_TABLE_VIEW_AND_SEARCH_CONTENT_CONFIGURATION(YerothDatabase::MARCHANDISES);

    reinitialiser_colones_db_visibles();

    setupLineEdits();

    localSetupLineEditsQCompleters();

    populateMarchandisesComboBoxes();

    _pushButton_filtrer_font = new QFont(pushButton_filtrer->font());

    tableView_marchandises->setSqlTableName(&YerothDatabase::MARCHANDISES);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);


    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS


    pushButton_add_condition_SEARCH->disable(this);

    pushButton_reinitialiser_filtre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_reinitialiser->enable(this, SLOT(reinitialiser_recherche()));


    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));


    connect(actionReinitialiserChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(slot_reinitialiser_colones_db_visibles()));

    connect(actionChampsDBVisible,
    		SIGNAL(triggered()),
			this,
            SLOT(selectionner_champs_db_visibles()));


    connect(action_BASCULER_SERVICES_MARCHANDISES,
            SIGNAL(triggered()),
            this,
			SLOT(handle_ACTION_CHOICE_SERVICES_MARCHANDISES()));


    connect(checkBox_filtres,
            SIGNAL(stateChanged(int)),
            this,
			SLOT(handle_filtres_checkBox(int)));


    //Menu actions
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_Principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer_un_stock()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionExporter_au_format_csv, SIGNAL(triggered()), this, SLOT(export_csv_file()));

    connect(actionAfficher_les_marchandises_non_terminees,
            SIGNAL(triggered()),
			this,
            SLOT(slot_filtrer_non_empty_product_stock__WITH_PROGRESS_BAR()));

    connect(actionAfficher_les_marchandises_terminees,
    		SIGNAL(triggered()),
            this,
            SLOT(slot_filter_empty_product_stock__WITH_PROGRESS_BAR()));

    connect(action_parametrer_les_impressions, SIGNAL(triggered()), this, SLOT(setup_print()));
    connect(actionAfficherPDF, SIGNAL(triggered()), this, SLOT(print_PDF_PREVIOUSLY_SETUP()));
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

    connect(checkBox_services, SIGNAL(stateChanged(int)), this,
            SLOT(handle_services_checkBox(int)));

    connect(actionModifierMarchandise, SIGNAL(triggered()), this,
            SLOT(modifier_marchandise()));

    connect(actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES,
            SIGNAL(triggered()), this,
            SLOT(supprimer_cette_marchandise()));

    setupShortcuts();
}


YerothMarchandisesWindow::~YerothMarchandisesWindow()
{
    MACRO_TO_DELETE_PAGINATION_INTEGER_VALIDATOR
    delete _pushButton_filtrer_font;
    delete _logger;
}


void YerothMarchandisesWindow::reinitialiser_colones_db_visibles()
{
    _visibleDBColumnNameStrList.clear();

    _visibleDBColumnNameStrList
            << YerothDatabaseTableColumn::DESIGNATION
            << YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT
            << YerothDatabaseTableColumn::CATEGORIE
            << YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT
            << YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT
            << YerothDatabaseTableColumn::REFERENCE;
}


void YerothMarchandisesWindow::contextMenuEvent(QContextMenuEvent *event)
{
    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        if (aUser->isManager() ||
                aUser->isGestionaireDesStocks() || aUser->isMagasinier())
        {
            if (tableView_marchandises->rowCount() > 0)
            {
                QMenu menu(this);
                menu.setPalette(toolBar_marchandisesWindow->palette());
                menu.addAction(actionModifierMarchandise);
                menu.addAction
                (actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES);
                menu.exec(event->globalPos());
            }
        }
    }
}


void YerothMarchandisesWindow::hideEvent(QHideEvent *hideEvent)
{
    YerothWindowsCommons::
    CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
}


void YerothMarchandisesWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide
    (*actionAfficher_les_marchandises_terminees);
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
    setupShortcutActionPARAMETRER_IMPRESSION_PDF
    (*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);

    actionAfficher_les_marchandises_non_terminees->setShortcut
    (YerothUtils::AFFICHER_LES_MARCHANDISES_NON_TERMINES_QKEYSEQUENCE);
    actionAfficher_les_marchandises_terminees->setShortcut
    (YerothUtils::AFFICHER_LES_MARCHANDISES_TERMINES_QKEYSEQUENCE);
    actionReinitialiserRecherche->
    setShortcut(YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE);
}


void YerothMarchandisesWindow::slot_reinitialiser_colones_db_visibles()
{
    reinitialiser_colones_db_visibles();
    resetTableViewHorizontalHeader_DEFAULT_ORDERING();
    afficherMarchandises();
}


void YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters()
{
    lineEdit_marchandises_element_de_stock_resultat->clear();


    if (_current_filtering_non_empty_stock_SQL_QUERY.isEmpty())
    {
        setCurrentlyFiltered(false);

        clearSearchFilter();
    }


    QString searchTerm(lineEdit_marchandises_terme_recherche->text());

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
                                                   (YerothDatabaseTableColumn::
                                                    DESCRIPTION_PRODUIT,
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

    QMapIterator < YerothLineEdit **,
                 QString > it(_lineEditsToANDContentForSearch);

    QString correspondingDBFieldKeyValue;

    QString aTableColumnFieldContentForANDSearch;

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
    if (_current_filtering_non_empty_stock_SQL_QUERY.isEmpty())
    {
        _yerothSqlTableModel->yerothSetFilter_WITH_where_clause(_searchFilter);

        if (_yerothSqlTableModel->select())
        {
            lister_les_elements_du_tableau(*_yerothSqlTableModel);
        }
        else
        {
            qDebug() <<
                     QString
                     ("++ YerothMarchandisesWindow::textChangedSearchLineEditsQCompleters(): %1").
                     arg(_yerothSqlTableModel->lastError().text());
        }
    }
    else
    {
        lister_les_elements_du_tableau(*_yerothSqlTableModel);
    }

    handle_some_actions_tools_enabled();
}


void YerothMarchandisesWindow::initialiser_FILTRES()
{
    comboBox_inventaire_des_stocks_element_de_stock->setVisible(false);
    comboBox_inventaire_des_stocks_element_de_stock_condition->setVisible
    (false);
    lineEdit_marchandises_element_de_stock_resultat->setVisible(false);
    pushButton_reinitialiser_filtre->setVisible(false);
    pushButton_filtrer->setVisible(false);
}


void YerothMarchandisesWindow::handle_filtres_checkBox(int state)
{
    bool IS_VISIBLE = checkBox_filtres->isChecked();

    comboBox_inventaire_des_stocks_element_de_stock->setVisible(IS_VISIBLE);
    comboBox_inventaire_des_stocks_element_de_stock_condition->setVisible
    (IS_VISIBLE);
    lineEdit_marchandises_element_de_stock_resultat->setVisible(IS_VISIBLE);
    pushButton_reinitialiser_filtre->setVisible(IS_VISIBLE);
    pushButton_filtrer->setVisible(IS_VISIBLE);
}


void YerothMarchandisesWindow::entrer_un_stock()
{
    if (getLastListerSelectedRow__ID_AS_INTEGER() > -1 &&
            _curMarchandisesTableModel->rowCount() > 0)
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel,
                                                  _curMarchandisesTableModel);
        rendreInvisible();
    }
    else
    {
        _allWindows->_entrerWindow->rendreVisible(_curStocksTableModel);
        rendreInvisible();
    }
}


void YerothMarchandisesWindow::handle_services_checkBox(int state)
{
    if (checkBox_services->isChecked())
    {
        label_nombre_darticles->setVisible(false);

        lineEdit_nombre_darticles->setVisible(false);

        label_nombre_de_marchandises->setText(QObject::tr("# services"));

        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("LES SERVICES"),
        								 _curMarchandisesTableModel);

        action_BASCULER_SERVICES_MARCHANDISES
			->setText(QObject::tr("AFFICHER DES MARCHANDISES"));

        actionAfficher_les_marchandises_non_terminees->setVisible(false);

        actionAfficher_les_marchandises_terminees->setVisible(false);

        _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause
        (YerothUtils::generateSqlIs
         (YerothDatabaseTableColumn::IS_SERVICE,
          YerothUtils::MYSQL_TRUE_LITERAL));
    }
    else
    {
        label_nombre_darticles->setVisible(true);

        lineEdit_nombre_darticles->setVisible(true);

        label_nombre_de_marchandises->setText(QObject::tr("# marchandises"));

        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("les marchandises"),
        								 _curMarchandisesTableModel);

        action_BASCULER_SERVICES_MARCHANDISES->setText(QObject::tr
                                                       ("AFFICHER DES SERVICES"));

        actionAfficher_les_marchandises_non_terminees->setVisible(true);

        actionAfficher_les_marchandises_terminees->setVisible(true);

        _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause
        (YerothUtils::generateSqlIs
         (YerothDatabaseTableColumn::IS_SERVICE,
          YerothUtils::MYSQL_FALSE_LITERAL));
    }

    _curMarchandisesTableModel->easySelect("src/windows/stocks/yeroth-erp-marchandises-windows.cpp", 505);

    lineEdit_nom_element_string_db->clear();

    if (-1 != state)
    {
        reinitialiser_elements_filtrage();
    }


    localSetupLineEditsQCompleters();

    afficherMarchandises();
}


bool
YerothMarchandisesWindow::
slot_filtrer_non_empty_product_stock__AVANT_MARCHANDISES_TERMINEES()
{
    _curMarchandisesTableModel->resetFilter("src/windows/stocks/yeroth-erp-marchandises-window.cpp");

    emit
    SIGNAL_INCREMENT_PROGRESS_BAR(5);

    QString
    cur_select_stmt = _curMarchandisesTableModel->yerothSelectStatement();

    QString
    SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS =
                    QString(" (SELECT DISTINCT stocks.reference, "
                            "stocks.designation FROM stocks WHERE %1 > 0) T "
                            "INNER JOIN marchandises ON marchandises.%2 = T.%3").arg
                    (YerothDatabaseTableColumn::QUANTITE_TOTALE,
                     YerothDatabaseTableColumn::DESIGNATION,
                     YerothDatabaseTableColumn::DESIGNATION);

//      QDEBUG_STRING_OUTPUT_2("SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS",
//                                                 SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

    int
    from_index = cur_select_stmt.indexOf("from", 0, Qt::CaseInsensitive);

    cur_select_stmt.remove(from_index, cur_select_stmt.length());

    QStringList
    cur_select_stmt_string_list_splitted = cur_select_stmt.split(',');

    emit
    SIGNAL_INCREMENT_PROGRESS_BAR(12);

    cur_select_stmt.clear();

    QString
    a_cur_element_split;

    uint
    cur_select_stmt_string_list_splitted__SIZE =
                    cur_select_stmt_string_list_splitted.size();

    for (uint k = 0; k < cur_select_stmt_string_list_splitted__SIZE; ++k)
    {
        a_cur_element_split =
                        cur_select_stmt_string_list_splitted.at(k).trimmed();

        if (0 == k)
        {
            QStringList
            select_splitted =
                            a_cur_element_split.split(YerothUtils::EMPTY_SPACE_REGEXP);

            cur_select_stmt.append(QString("SELECT %1.%2, ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    select_splitted.at(1)));
        }
        else if (k < cur_select_stmt_string_list_splitted__SIZE - 1)
        {
            cur_select_stmt.append(QString("%1.%2, ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    a_cur_element_split));
        }
        else if (k < cur_select_stmt_string_list_splitted__SIZE)
        {
            cur_select_stmt.append(QString("%1.%2 FROM ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    a_cur_element_split));
        }
    }

    emit
    SIGNAL_INCREMENT_PROGRESS_BAR(92);

    cur_select_stmt.append(SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

    _current_filtering_non_empty_stock_SQL_QUERY = cur_select_stmt;


    int
    resultRows =
                    _curMarchandisesTableModel->yerothSetQueryRowCount(cur_select_stmt);


//      QDEBUG_STRING_OUTPUT_2("_curMarchandisesTableModel", _curMarchandisesTableModel->query().lastQuery());


    if (resultRows >= 0)
    {
        textChangedSearchLineEditsQCompleters();

        MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS;

        setCurrentlyFiltered(true);

        emit
        SIGNAL_INCREMENT_PROGRESS_BAR(100);

        QThread::sleep(0.7);

        return true;
    }
    else
    {
        emit
        SIGNAL_INCREMENT_PROGRESS_BAR(100);

        QThread::sleep(0.7);
    }

    return false;
}


bool YerothMarchandisesWindow::slot_filtrer_non_empty_product_stock()
{
    _curMarchandisesTableModel->resetFilter("src/windows/stocks/yeroth-erp-marchandises-window.cpp");

    emit SIGNAL_INCREMENT_PROGRESS_BAR(5);

    QString cur_select_stmt =
                    _curMarchandisesTableModel->yerothSelectStatement();

    QString SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS =
                    QString(" (SELECT DISTINCT stocks.reference, "
                            "stocks.designation FROM stocks WHERE %1 > 0) T "
                            "INNER JOIN marchandises ON marchandises.%2 = T.%3").arg
                    (YerothDatabaseTableColumn::QUANTITE_TOTALE,
                     YerothDatabaseTableColumn::DESIGNATION,
                     YerothDatabaseTableColumn::DESIGNATION);

//      QDEBUG_STRING_OUTPUT_2("SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS",
//                                                 SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

    int from_index = cur_select_stmt.indexOf("from", 0, Qt::CaseInsensitive);

    cur_select_stmt.remove(from_index, cur_select_stmt.length());

    QStringList cur_select_stmt_string_list_splitted =
                    cur_select_stmt.split(',');

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    cur_select_stmt.clear();

    QString a_cur_element_split;

    uint cur_select_stmt_string_list_splitted__SIZE =
                    cur_select_stmt_string_list_splitted.size();

    for (uint k = 0; k < cur_select_stmt_string_list_splitted__SIZE; ++k)
    {
        a_cur_element_split =
                        cur_select_stmt_string_list_splitted.at(k).trimmed();

        if (0 == k)
        {
            QStringList select_splitted =
                            a_cur_element_split.split(YerothUtils::EMPTY_SPACE_REGEXP);

            cur_select_stmt.append(QString("SELECT %1.%2, ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    select_splitted.at(1)));
        }
        else if (k < cur_select_stmt_string_list_splitted__SIZE - 1)
        {
            cur_select_stmt.append(QString("%1.%2, ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    a_cur_element_split));
        }
        else if (k < cur_select_stmt_string_list_splitted__SIZE)
        {
            cur_select_stmt.append(QString("%1.%2 FROM ").arg
                                   (YerothDatabase::MARCHANDISES,
                                    a_cur_element_split));
        }
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(92);

    cur_select_stmt.append(SELECT__FILTERING__NON_EMPTY_PRODUCT_STOCKS);

    _current_filtering_non_empty_stock_SQL_QUERY = cur_select_stmt;


    int resultRows =
                    _curMarchandisesTableModel->yerothSetQueryRowCount(cur_select_stmt);


//      QDEBUG_STRING_OUTPUT_2("_curMarchandisesTableModel", _curMarchandisesTableModel->query().lastQuery());


    if (resultRows >= 0)
    {
        textChangedSearchLineEditsQCompleters();

        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("les marchandises NON TERMINÉES"),
        								 _curMarchandisesTableModel);

        MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS;

        setCurrentlyFiltered(true);

        emit SIGNAL_INCREMENT_PROGRESS_BAR(100);

        QThread::sleep(0.7);

        if (!_NON_TERMINEES_MARCHANDISE_RE_ENTRANT)
        {
            YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                        "non terminées");
        }

        _NON_TERMINEES_MARCHANDISE_RE_ENTRANT = true;

        return true;
    }
    else
    {
        YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("les marchandises"),
        								 _curMarchandisesTableModel);

        emit SIGNAL_INCREMENT_PROGRESS_BAR(100);

        QThread::sleep(0.7);

        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "non terminées");
    }

    return false;
}


bool YerothMarchandisesWindow::slot_filter_empty_product_stock()
{
    slot_filtrer_non_empty_product_stock__AVANT_MARCHANDISES_TERMINEES();

    QThread::sleep(1);

    //This is clear from the way our project is organized
    QStandardItemModel *stdItemModel =
                    (QStandardItemModel *) tableView_marchandises->model();

    if (0 == stdItemModel)
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this,
                                                  "marchandises - filtrer");

        return false;
    }

    emit SIGNAL_INCREMENT_PROGRESS_BAR(12);

    _curMarchandisesTableModel->resetFilter("src/windows/stocks/yeroth-erp-marchandises-window.cpp");

    afficherMarchandises();

    QString filter_string;

    QStringList filter_string_list;

    QStringList splittedData;

    QString data;

    QString nom_departement_str;
    QString categorie_str;
    QString designation_str;


//      QDEBUG_STRING_OUTPUT_2("stdItemModel->rowCount()", QString::number(stdItemModel->rowCount()));

    QStandardItem *anItem = 0;

    for (int k = 0; k < stdItemModel->rowCount(); ++k)
    {
        anItem = stdItemModel->item(k, 0);

        if (0 != anItem)
        {
            if (YerothUtils::YEROTH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING
                    == anItem->foreground().color())
            {
                data = anItem->accessibleText();

//                              QDEBUG_STRING_OUTPUT_2("data", data);

                splittedData = data.split("|");

                if (splittedData.size() > 1)
                {
                    nom_departement_str = splittedData.at(0);
                    categorie_str = splittedData.at(1);
                    designation_str = splittedData.at(2);

                    filter_string_list.append(QString
                                              ("(nom_departement_produit = '%1' AND categorie = '%2' AND designation = '%3')").
                                              arg(nom_departement_str,
                                                  categorie_str,
                                                  designation_str));

                    if (k + 1 < stdItemModel->rowCount())
                    {
                        filter_string_list.append(" OR ");
                    }
                }
            }
        }
    }


    if (filter_string_list.isEmpty())
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");

        return false;
    }

    if (YerothUtils::isEqualCaseInsensitive
            (filter_string_list.last(), " OR "))
    {
        filter_string_list.removeLast();
    }

    filter_string = filter_string_list.join(" ");

    emit SIGNAL_INCREMENT_PROGRESS_BAR(93);

    if (filter_string.isEmpty())
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");

        return false;
    }

    _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause
    (filter_string);

    int resultRows =
    		_curMarchandisesTableModel->easySelect("src/windows/stocks/yeroth-erp-marchandises-windows.cpp", 868);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        emit SIGNAL_INCREMENT_PROGRESS_BAR(78);

        lister_les_elements_du_tableau(*_curMarchandisesTableModel);

        emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

        YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "terminées");

        return true;
    }
    else
    {
        emit SIGNAL_INCREMENT_PROGRESS_BAR(98);

        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this, "terminées");
    }

    return false;
}


bool YerothMarchandisesWindow::slot_filtrer()
{
    QString
    stockTableColumnValue
    (lineEdit_marchandises_element_de_stock_resultat->text());

    if (stockTableColumnValue.isEmpty())
    {
        QString msg(QObject::tr
                    ("Veuillez saisir une valeur numérique pour la recherche !"));

        YerothQMessageBox::information(this, QObject::tr("filtrer"), msg);
        return false;
    }

    reinitialiser_recherche();

    QString
    stockTableColumnProperty
    (comboBox_inventaire_des_stocks_element_de_stock->currentText());

    QString
    mathOperator
    (comboBox_inventaire_des_stocks_element_de_stock_condition->currentText
     ());

    QString filterString;

    QString
    REAL_DB_ID_NAME_stockTableColumnProperty
    (YerothDatabaseTableColumn::_tableColumnToUserViewString.key
     (stockTableColumnProperty));

    filterString.append(QString("%2 %3 '%4'").arg
                        (REAL_DB_ID_NAME_stockTableColumnProperty,
                         mathOperator, stockTableColumnValue));

//      qDebug() << QString("filterString: %1")
//                                      .arg(filterString);

    _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause
    (filterString);

//      QDEBUG_STRING_OUTPUT_2("_curMarchandisesTableModel->yerothSelectStatement()",
//                      _curMarchandisesTableModel->yerothSelectStatement());

    int resultRows =
    		_curMarchandisesTableModel->easySelect("src/windows/stocks/yeroth-erp-marchandises-windows.cpp", 943);

    if (resultRows >= 0)
    {
        setCurrentlyFiltered(true);

        lister_les_elements_du_tableau(*_curMarchandisesTableModel);

        YEROTH_QMESSAGE_BOX_QUELQUE_RESULTAT_FILTRE(this, resultRows,
                                                    "marchandises - filtrer");

        return true;
    }
    else
    {
        YEROTH_QMESSAGE_BOX_AUCUN_RESULTAT_FILTRE(this,
                                                  "marchandises - filtrer");
    }

    return false;
}


void YerothMarchandisesWindow::localSetupLineEditsQCompleters()
{
    QString aConditionStr;

    if (checkBox_services->isChecked())
    {
        aConditionStr =
                        YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
                                                   "1");
    }
    else
    {
        aConditionStr =
                        YerothUtils::generateSqlIs(YerothDatabaseTableColumn::IS_SERVICE,
                                                   "0");
    }

    setupLineEditsQCompleters((QObject *) this, aConditionStr);
}


void YerothMarchandisesWindow::set_filtrer_font()
{
    if (isCurrentlyFiltered())
    {
        _pushButton_filtrer_font->setUnderline(true);
    }
    else
    {
        _pushButton_filtrer_font->setUnderline(false);
    }

    pushButton_filtrer->setFont(*_pushButton_filtrer_font);
}


void YerothMarchandisesWindow::populateMarchandisesComboBoxes()
{
    QStringList aQStringList;

    aQStringList.append(_varchar_dbtable_column_name_list.values());

    aQStringList.removeAll(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT);

    comboBox_element_string_db->addItems(aQStringList,
                                         YerothDatabaseTableColumn::
                                         _tableColumnToUserViewString);

    comboBox_element_string_db->setCurrentIndex(0);


    aQStringList.clear();

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT));

    aQStringList.append(YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING
                        (YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT));

    comboBox_inventaire_des_stocks_element_de_stock->addItems(aQStringList);


    aQStringList.clear();

    aQStringList.append(">=");

    aQStringList.append("<=");

    aQStringList.append(">");

    aQStringList.append("<");

    aQStringList.append("=");

    comboBox_inventaire_des_stocks_element_de_stock_condition->addItems
    (aQStringList);
}


void YerothMarchandisesWindow::setupLineEdits()
{
    _QLINEEDIT_nombre_de_lignes_par_page =
                    lineEdit_marchandises_nombre_de_lignes_par_page;

    lineEdit_marchandises_terme_recherche->enableForSearch(QObject::tr
                                                           ("terme à rechercher (description de l'article (ou service))"));

    lineEdit_nom_element_string_db->enableForSearch(QObject::tr
                                                    ("valeur à rechercher"));

    lineEdit_nombre_de_marchandises->setYerothEnabled(false);

    lineEdit_nombre_darticles->setYerothEnabled(false);

    MACRO_TO_BIND_PAGING_WITH_QLINEEDIT
    (lineEdit_marchandises_nombre_de_lignes_par_page, tableView_marchandises);

    lineEdit_marchandises_element_de_stock_resultat->setValidator
    (&YerothUtils::POSITIVE_DoubleValidator);
}


void YerothMarchandisesWindow::rendreInvisible()
{
    tableView_marchandises->stopEditingModeSelection();

    YerothWindowsCommons::rendreInvisible();
}


void YerothMarchandisesWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    YEROTH_INITIALISE_WINDOW_SEARCH_FILTERS_WIDGETS;

    YerothPOSUser *aCurrentUser = _allWindows->getUser();

    if (0 != aCurrentUser && !aCurrentUser->isManager())
    {
        checkBox_services->setChecked(false);

        checkBox_services->setVisible(false);
    }
    else
    {
        checkBox_services->setVisible(true);
    }

    setupLineEdits();

    setYerothSqlTableModel(_curMarchandisesTableModel);

    _curStocksTableModel = stocksTableModel;

    lineEdit_marchandises_terme_recherche->setFocus();

    setVisible(true);


    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("les marchandises / SERVICES"),
    								 _curMarchandisesTableModel);


    APPLY_USER_LOCAL_SETTINGS_PARAMETERS();

    if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
    {
        reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

        QString currentFilter(_curMarchandisesTableModel->filter());

        if (!currentFilter.isEmpty())
        {
            currentFilter.append(QString(" AND "));
        }

        if (checkBox_services->isChecked())
        {
            currentFilter.append(QString("%1").arg(YerothUtils::generateSqlIs
                                                   (YerothDatabaseTableColumn::IS_SERVICE,
                                                    YerothUtils::MYSQL_TRUE_LITERAL)));
        }
        else
        {
            currentFilter.append(QString("%1").arg(YerothUtils::generateSqlIs
                                                   (YerothDatabaseTableColumn::IS_SERVICE,
                                                    YerothUtils::MYSQL_FALSE_LITERAL)));
        }

        _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(currentFilter);

        localSetupLineEditsQCompleters();

        afficherMarchandises();
    }
    else
    {
        setCurrentlyFiltered(true);

        slot_filtrer_non_empty_product_stock();
    }
}


void YerothMarchandisesWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMarchandisesTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer_un_stock()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerothMarchandisesWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMarchandisesTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
}


void YerothMarchandisesWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMarchandisesTableModel)
    pushButton_add_condition_SEARCH->enable(this,
                                            SLOT
                                            (AJOUTER_CONDITION_DE_RECHERCHE
                                             ()));

    pushButton_reinitialiser_filtre->enable(this,
                                            SLOT
                                            (reinitialiser_elements_filtrage
                                             ()));
    pushButton_filtrer->enable(this, SLOT(slot_filtrer()));
    pushButton_entrer->enable(this, SLOT(entrer_un_stock()));
    pushButton_rapports->disable(this);
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
}


void YerothMarchandisesWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionExporter_au_format_csv,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_parametrer_les_impressions,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAfficherPDF, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifierMarchandise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionSupprimer_cette_TOUTES_LES_marchandiseS_SELECTIONEES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_Principal, false);

    MACRO_TO_DISABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS
    pushButton_reinitialiser_filtre->disable(this);
    pushButton_filtrer->disable(this);
    pushButton_entrer->disable(this);
    pushButton_rapports->disable(this);
    pushButton_stocks->disable(this);
    pushButton_sortir->disable(this);
    pushButton_menu_principal->disable(this);
}


void YerothMarchandisesWindow::afficher_stock_selectioner(const QString &
                                                          stockName)
{
    QString
    filter(GENERATE_SQL_IS_STMT
           (YerothDatabaseTableColumn::DESIGNATION, stockName));

    //qDebug() << QString("filter: %1").arg(filter);

    _curMarchandisesTableModel->yerothSetFilter_WITH_where_clause(filter);

    if (_curMarchandisesTableModel->easySelect("src/windows/stocks/yeroth-erp-marchandises-windows.cpp", 1346) > 0)
    {
        afficherMarchandises();
    }
}


void YerothMarchandisesWindow::modifier_marchandise()
{
    if (0 != _curMarchandisesTableModel)
    {
        tableView_marchandises->startEditingModeSelection();
    }
}


void
YerothMarchandisesWindow::supprimer_PLUSIEURS_Marchandises
(YerothSqlTableModel &aMarchandisesTableModel)
{
    QString
    msgSupprimer =
                    QObject::tr("SUPPRIMER TOUTES LES MARCHANDISES "
                                    "SÉLECTIONÉES (sans confirmation de suppression) ?");


    if (QMessageBox::Cancel ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression de plusieurs"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("suppression de plusieurs"),
                                       QObject::tr
                                       ("ANNULATION DES SUPPRESSIONS DES MARCHANDISES SÉLECTIONÉES !"));

        return;
    }
    else
    {
    }

    QMapIterator < QString,
                 QString > j(tableView_marchandises->lastSelected_Rows__IDs());

    while (j.hasNext())
    {
        j.next();

        supprimer_cette_marchandise_DE_PLUSIEURS(j.value());
    }

    localSetupLineEditsQCompleters();

    afficherMarchandises();

    tableView_marchandises->clearSelection();
}


void YerothMarchandisesWindow::supprimer_cette_marchandise_DE_PLUSIEURS(QString
                                                                        aMarchandiseID)
{
    QSqlRecord record;

    /*
     * 'static' declaration helps us to increase runtime speed
     * of application in case of several thousands of merchandise !
     */
    static QSqlQuery query;

    query.clear();

    QString
    QUERY_MARCHANDISE_DATA(QString("select * from %1 where %2='%3'").arg
                           (YerothDatabase::MARCHANDISES,
                            YerothDatabaseTableColumn::ID, aMarchandiseID));

    int querySize = YerothUtils::execQuery(query, QUERY_MARCHANDISE_DATA);

    if (querySize > 0 && query.next())
    {
        record = query.record();
    }


    static QString QUERY_MARCHANDISE_DATA_in_stock;

    QUERY_MARCHANDISE_DATA_in_stock.clear();

    QUERY_MARCHANDISE_DATA_in_stock =
                    QString("select * from %1 where %2='%3'").arg(YerothDatabase::STOCKS,
                                                                  YerothDatabaseTableColumn::
                                                                  DESIGNATION,
                                                                  GET_SQL_RECORD_DATA
                                                                  (record,
                                                                   YerothDatabaseTableColumn::
                                                                   DESIGNATION));


    static QSqlQuery query_stock_table_for_merchandise_removal;

    query_stock_table_for_merchandise_removal.clear();

    static int query_stock_size = 0;

    query_stock_size = 0;

    query_stock_size =
                    YerothUtils::execQuery(query_stock_table_for_merchandise_removal,
                                           QUERY_MARCHANDISE_DATA_in_stock);

    if (query_stock_size > 0)
    {
        QString msg_pas_de_suppression(QObject::tr
                                       ("AU MINIMUM ENCORE 1 STOCK EXISTANT dans la tableau des stocks !\n"
                                        "DONC, PAS DE SUPPRESSION DE LA\n"
                                        "MARCHANDISE '%1' POSSIBLE !").arg
                                       (GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::
                                         DESIGNATION)));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("suppression - STOCK ENCORE EXISTANT"),
                                       msg_pas_de_suppression);

        return;
    }

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        /*
         * WE DO NOT SUPPRESS SERVICES in merchandise THIS WAY.
         */
        return;
    }

    QString DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING
    (QString("DELETE FROM %1 WHERE %2 = '%3'").arg
     (YerothDatabase::MARCHANDISES, YerothDatabaseTableColumn::ID,
      aMarchandiseID));

    YerothUtils::execQuery
    (DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

    //              QDEBUG_STRING_OUTPUT_2(QString("resRemoved: %1, stocksID: %2")
    //                                                                      .arg(BOOL_TO_STRING(resRemoved),
    //                                                                               aStockID),
    //                                                                                       DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);
}


void YerothMarchandisesWindow::supprimer_cette_marchandise(QString aMarchandiseID
                                                           /* = YerothUtils::EMPTY_STRING */
                                                           , bool _reEntrant
                                                           /* = false */)
{
    if (!_reEntrant
            && tableView_marchandises->lastSelected_Rows__IDs_INT_SIZE() > 1)
    {
        supprimer_PLUSIEURS_Marchandises(*_curMarchandisesTableModel);

        afficherMarchandises();

        return;
    }

    QSqlRecord record;

    /*
     * 'static' declaration helps us to increase runtime speed
     * of application in case of several thousands of merchandise !
     */
    static QSqlQuery query;

    if (!aMarchandiseID.isEmpty())
    {
        query.clear();

        QString
        QUERY_MARCHANDISE_DATA(QString("select * from %1 where %2='%3'").arg
                               (YerothDatabase::MARCHANDISES,
                                YerothDatabaseTableColumn::ID,
                                aMarchandiseID));

        int m_query_size =
                        YerothUtils::execQuery(query, QUERY_MARCHANDISE_DATA);

        if (m_query_size > 0 && query.next())
        {
            record = query.record();
        }
    }
    else
    {
        _allWindows->
        _marchandisesWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
        (record);

        aMarchandiseID =
                        GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);
    }

    query.clear();

    QString
    QUERY_MARCHANDISE_DATA(QString
                           ("select %1 from %2 where %3='%4' AND %5='%6' AND %7='%8'").
                           arg(YerothDatabaseTableColumn::ID,
                               YerothDatabase::MARCHANDISES,
                               YerothDatabaseTableColumn::DESIGNATION,
                               GET_SQL_RECORD_DATA(record,
                                                   YerothDatabaseTableColumn::
                                                   DESIGNATION),
                               YerothDatabaseTableColumn::CATEGORIE,
                               GET_SQL_RECORD_DATA(record,
                                                   YerothDatabaseTableColumn::
                                                   CATEGORIE),
                               YerothDatabaseTableColumn::
                               NOM_DEPARTEMENT_PRODUIT,
                               GET_SQL_RECORD_DATA(record,
                                                   YerothDatabaseTableColumn::
                                                   NOM_DEPARTEMENT_PRODUIT)));


    int MARCHANDISE_query_size =
                    YerothUtils::execQuery(query, QUERY_MARCHANDISE_DATA);


    static QString QUERY_MARCHANDISE_DATA_in_stock;

    QUERY_MARCHANDISE_DATA_in_stock.clear();

    QUERY_MARCHANDISE_DATA_in_stock =
                    QString("select * from %1 where %2='%3'").arg(YerothDatabase::STOCKS,
                                                                  YerothDatabaseTableColumn::
                                                                  DESIGNATION,
                                                                  GET_SQL_RECORD_DATA
                                                                  (record,
                                                                   YerothDatabaseTableColumn::
                                                                   DESIGNATION));


    static QSqlQuery query_stock_table_for_merchandise_removal;

    query_stock_table_for_merchandise_removal.clear();

    static int query_stock_size = 0;

    query_stock_size = 0;

    query_stock_size =
                    YerothUtils::execQuery(query_stock_table_for_merchandise_removal,
                                           QUERY_MARCHANDISE_DATA_in_stock);

//      QDEBUG_STRING_OUTPUT_2_N("MARCHANDISE_query_size", MARCHANDISE_query_size);

    if (1 == MARCHANDISE_query_size)
    {
//              QDEBUG_STRING_OUTPUT_2_N("query_stock_size", query_stock_size);

        if (query_stock_size > 0)
        {
            QString msg_pas_de_suppression(QObject::tr
                                           ("AU MINIMUM ENCORE 1 STOCK EXISTANT dans la tableau des stocks !\n"
                                            "DONC, PAS DE SUPPRESSION DE LA\n"
                                            "MARCHANDISE '%1' POSSIBLE !").arg
                                           (GET_SQL_RECORD_DATA
                                            (record,
                                             YerothDatabaseTableColumn::
                                             DESIGNATION)));

            if (!_reEntrant)
            {
                YerothQMessageBox::information(this,
                                               QObject::tr
                                               ("suppression - STOCK ENCORE EXISTANT"),
                                               msg_pas_de_suppression);
            }

            return;
        }
    }

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        /*
         * WE DO NOT SUPPRESS SERVICES in merchandise THIS WAY.
         */
        return;
    }

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de la marchandise '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression"),
                                        msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved = false;

        if (!aMarchandiseID.isEmpty())
        {
            QString DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING
            (QString("DELETE FROM %1 WHERE %2 = '%3'").arg
             (YerothDatabase::MARCHANDISES, YerothDatabaseTableColumn::ID,
              aMarchandiseID));

            resRemoved =
                            YerothUtils::execQuery
                            (DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);

//              QDEBUG_STRING_OUTPUT_2(QString("resRemoved: %1, stocksID: %2")
//                                                                      .arg(BOOL_TO_STRING(resRemoved),
//                                                                               aStockID),
//                                                                                       DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW_QUERY_STRING);
        }
        else
        {
            resRemoved =
                            _allWindows->
                            _marchandisesWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                            ();
        }

        //qDebug() << "YerothInventaireDesStocksWindow::supprimer_ce_stock() " << resRemoved;
        if (resRemoved && _curMarchandisesTableModel->select())
        {
            msgSupprimer =
                            QObject::tr("La marchandise '%1' a été supprimée.").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::DESIGNATION));

            if (!_reEntrant)
            {
                tableView_marchandises->clearSelection();
            }

            YerothQMessageBox::information(this,
                                           QObject::tr("succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("L'article \"%1\" ne pouvait pas être supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::DESIGNATION));

            YerothQMessageBox::warning(this,
                                       QObject::tr("échec"),
                                       msgSupprimer);
        }

        localSetupLineEditsQCompleters();

        afficherMarchandises();
    }
    else
    {
    }
}


void
YerothMarchandisesWindow::
reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES()
{
    MACRO_TO_ENABLE_PAGE_FIRST_NEXT_PREVIOUS_LAST_PUSH_BUTTONS(this,
                                                               _curMarchandisesTableModel)
    _NON_TERMINEES_MARCHANDISE_RE_ENTRANT = false;

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("les marchandises"),
    								 _curMarchandisesTableModel);

    _current_filtering_non_empty_stock_SQL_QUERY.clear();
}


void YerothMarchandisesWindow::reinitialiser_elements_filtrage()
{
    reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

    lineEdit_marchandises_element_de_stock_resultat->clear();

    YEROTH_RESET_FILTER_WITH_SEARCH_WITH_MORE_THAN_ONE_SEARCH_FILTER
    setCurrentlyFiltered(false);

    reinitialiser_recherche();
}


void YerothMarchandisesWindow::reinitialiser_recherche()
{
    reinitialiser__FILTRE__MARCHANDISES__NON__TERMINEES();

    lineEdit_nom_element_string_db->clear();

    setCurrentlyFiltered(false);

    resetLineEditsQCompleters((QObject *) this);

    handle_services_checkBox(-1);
}


void YerothMarchandisesWindow::lister_les_elements_du_tableau(YerothSqlTableModel
                                                              &
                                                              aYerothSqlTableModel)
{
    if (!checkBox_services->isChecked())
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" AND (%2 = '0')").arg
                                 (YerothDatabaseTableColumn::IS_SERVICE));
        }
        else
        {
            _searchFilter.append(QString(" (%2 = '0')").arg
                                 (YerothDatabaseTableColumn::IS_SERVICE));
        }
    }
    else
    {
        if (!_searchFilter.isEmpty())
        {
            _searchFilter.append(QString(" AND (%2 = '1')").arg
                                 (YerothDatabaseTableColumn::IS_SERVICE));
        }
        else
        {
            _searchFilter.append(QString(" (%2 = '1')").arg
                                 (YerothDatabaseTableColumn::IS_SERVICE));
        }
    }


    aYerothSqlTableModel.setFilter(_searchFilter);

    tableView_marchandises->queryYerothTableViewCurrentPageContentRow
    (aYerothSqlTableModel);

    tableView_show_or_hide_columns(*tableView_marchandises);

    int rowCount =
                    tableView_marchandises->getYerothSqlTableModelTotalRowCount();

    lineEdit_nombre_de_marchandises->setText(GET_NUM_STRING(rowCount));
}


bool YerothMarchandisesWindow::export_csv_file()
{
    bool success = false;

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_marchandises,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-merchandise-listing",
                                               "merchandise listing file");
    }
    else
    {
        success = YerothUtils::export_csv_file(*this,
                                               *tableView_marchandises,
                                               tableColumnsToIgnore,
                                               "yeroth-erp-fiche-marchandises",
                                               "fiche des marchandises");
    }

    return success;
}


QString YerothMarchandisesWindow::get_latex_template_print_pdf_content()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_marchandises_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_marchandises_tex;
    }

    if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
    {
        _latex_template_print_pdf_content.replace
        ("YEROTHMARCHANDISESNONTERMINEES", YerothUtils::EMPTY_STRING);
    }
    else
    {
        _latex_template_print_pdf_content.replace
        ("YEROTHMARCHANDISESNONTERMINEES",
         QObject::tr("NON TERMINÉES"));
    }

    return _latex_template_print_pdf_content;
}


bool YerothMarchandisesWindow::imprimer_pdf_document()
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_marchandises_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_marchandises_tex;
    }

    if (!IS__CURRENTLY__CHECKING__NON__EMPTY__STOCKS())
    {
        _latex_template_print_pdf_content.replace
        ("YEROTHMARCHANDISESNONTERMINEES", YerothUtils::EMPTY_STRING);
    }
    else
    {
        _latex_template_print_pdf_content.replace
        ("YEROTHMARCHANDISESNONTERMINEES",
         QObject::tr("NON TERMINÉES"));
    }

    return YerothWindowsCommons::imprimer_pdf_document();
}
