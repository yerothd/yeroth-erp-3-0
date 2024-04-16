/*
 * yeroth-erp-historique-du-stock-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-historique-du-stock-window.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/qmath.h>


//const uint YerothHistoriqueDuStockWindow::ROW_SIZE_PER_VIEWING_PAGE = 9;


YerothHistoriqueDuStockWindow::YerothHistoriqueDuStockWindow()
:YerothWindowsCommons("yeroth-erp-historique-dun-stock"),
 YerothAbstractClassYerothSearchWindow(YerothDatabase::STOCKS),
 _show_ASSET(false),
 _CURRENT_PAGING_NR(1),
 _logger(new YerothLogger("YerothHistoriqueDuStockWindow"))
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("historique du stock");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    enableResizing();

    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_historique_du_stock, false);

    _list_actions_to_enable_on_positive_tableview_ROW_COUNT
		<< actionAUGMENTER_LA_POLICE_DU_TABLEAU
		<< actiondiminuer_la_police_du_tableau;


    MACRO_TO_DEFINE_CURRENT_VIEW_WINDOW_FOR_TABLE_PAGINATION(tableView_historique_du_stock)


    QMESSAGE_BOX_STYLE_SHEET = QString("QMessageBox {background-color: rgb(%1);}"
                            		   "QMessageBox QLabel {color: rgb(%2);}")
									.arg(COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
										 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setupLineEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAUGMENTER_LA_POLICE_DU_TABLEAU, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actiondiminuer_la_police_du_tableau, true);

    connect(actionAUGMENTER_LA_POLICE_DU_TABLEAU,
    		SIGNAL(triggered()),
    		this,
            SLOT(incrementFontSize__OF_TABLE()));

    connect(actiondiminuer_la_police_du_tableau,
    		SIGNAL(triggered()),
    		this,
            SLOT(decrementFontSize__OF_TABLE()));

    connect(lineEdit_nombre_de_lignes_par_page,
            SIGNAL(textChanged(const QString &)),
            this,
			SLOT(SLOT_line_per_pagte_MODIFIED(const QString &)));

    connect(actionExporter_au_format_csv,
    		SIGNAL(triggered()),
			this,
            SLOT(export_csv_file()));

    connect(action_parametrer_les_impressions,
    		SIGNAL(triggered()),
			this,
            SLOT(setup_print()));

    connect(actionAfficherPDF,
    		SIGNAL(triggered()),
			this,
            SLOT(print_PDF_PREVIOUSLY_SETUP()));


    pushButton_page_premiere->enable(this, SLOT(appliquer_bouton_PREMIERE_PAGE()));
    pushButton_page_precedente->enable(this, SLOT(appliquer_bouton_PRECEDENTE_PAGE()));
    pushButton_page_suivante->enable(this, SLOT(appliquer_bouton_SUIVANTE_PAGE()));
    pushButton_page_derniere->enable(this, SLOT(appliquer_bouton_DERNIERE_PAGE()));


    setupShortcuts();


    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(0);
    setLeftAligned_FOR_YEROTH_PDF_LATEX_PRINTING(1);

    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(2);
    setCenterAligned_FOR_YEROTH_PDF_LATEX_PRINTING(3);
}


YerothHistoriqueDuStockWindow::~YerothHistoriqueDuStockWindow()
{
    clear_CURRENT_STOCK_HISTORY();

    delete _logger;
}


void YerothHistoriqueDuStockWindow::clear_CURRENT_STOCK_HISTORY()
{
    _CURRENT_PAGING_NR = 1;

    _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.clear();
}


void YerothHistoriqueDuStockWindow::listHistoriqueDuStock(const QStringList &aMouvementStockList,
                                                          const QString 	stockReference,
                                                          const QString 	stockDesignation,
                                                          bool 				show_ASSET /* = false */)
{
	retranslateUi(this);

    _show_ASSET = show_ASSET;

    QString curTitle(windowTitle());

    _a_previous_mouvement_stock_list = aMouvementStockList;

    _currentStockID = YerothWindowsCommons::get_last_lister_selected_row_db_ID();

    if (!YerothUtils::isEqualCaseInsensitive(_previousStockID, _currentStockID))
    {
        clear_CURRENT_STOCK_HISTORY();
    }

    _previousStockID = _currentStockID;

    _currentStockReference = stockReference;

    _currentStockDesignation = stockDesignation;

    //qDebug() << QString("curTitle: %1")
    //                              .arg(curTitle);

    static QString preambleTitle;

    if (!_show_ASSET)
    {
        preambleTitle = QString("%1 - %2 -").arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                                                 QObject::tr("historique du stock"));
    }
    else
    {
        preambleTitle = QString("%1 - %2 -").arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                                                 QObject::tr("historique de L'IMMOBILISATION"));
    }


    static bool first_time_inside = true;

    static int preambleTitleLength = preambleTitle.length();

    if (!first_time_inside)
    {
        int len = curTitle.length() - preambleTitleLength;

        preambleTitle.remove(preambleTitleLength, len);

        //qDebug() << QString("preambleTitle: %1")
        //                              .arg(preambleTitle);
    }
    else
    {
        first_time_inside = false;
    }

    if (!_show_ASSET)
    {
        preambleTitle.append(QObject::tr(" ID (%1), stock \"%2\"")
        						.arg(YerothWindowsCommons::get_last_lister_selected_row_db_ID(),
        							 stockDesignation));
    }
    else
    {
        preambleTitle.append(QObject::tr(" ID (%1), IMMOBILISATION \"%2\"")
                             	 .arg(YerothWindowsCommons::get_last_lister_selected_row_db_ID(),
                             		  stockDesignation));
    }

    YEROTH_set_windowName_TRANSLATED
		(YerothMainWindow::get_TRANSLATED_WindowName(const_cast<char *>(preambleTitle.toStdString().c_str())),
      							   _curStocksTableModel);

    int stock_history_VIEWING_PAGE_COUNT =
                    qFloor(aMouvementStockList.size() /
                           ((double) GET_ROW_NUMBER_PER_VIEWING_PAGE()));

    //QDEBUG_STRING_OUTPUT_2_N("stock_history_VIEWING_PAGE_COUNT", stock_history_VIEWING_PAGE_COUNT);

    //QDEBUG_STRING_OUTPUT_2_N("aMouvementStockList.size()", aMouvementStockList.size());

    if (0 == stock_history_VIEWING_PAGE_COUNT)
    {
        stock_history_VIEWING_PAGE_COUNT = 1;
    }


    uint k = 1;

    uint s = 0;

    QString a_paging_viewing_STOCK_HISTORY;

    for (int j = 0; j < stock_history_VIEWING_PAGE_COUNT; ++j)
    {
        a_paging_viewing_STOCK_HISTORY.clear();

        for (uint i = 0;
                i < GET_ROW_NUMBER_PER_VIEWING_PAGE()
                && s < aMouvementStockList.size(); ++i)
        {
            a_paging_viewing_STOCK_HISTORY.append(aMouvementStockList.at(s));
            a_paging_viewing_STOCK_HISTORY.append(YerothHistoriqueStock::SEPARATION_EXTERNE);

            ++s;
        }

        //QDEBUG_STRING_OUTPUT_2_N("s", s);
        //QDEBUG_STRING_OUTPUT_2("a_paging_viewing_STOCK_HISTORY", a_paging_viewing_STOCK_HISTORY);

        _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.insert(k, a_paging_viewing_STOCK_HISTORY);

        ++k;
    }


    int tableViewSize = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();

    _last_page_nr = (0 == tableViewSize ? 1 : stock_history_VIEWING_PAGE_COUNT);


    //QDEBUG_STRING_OUTPUT_2_N("tableViewSize", tableViewSize);

    //QDEBUG_STRING_OUTPUT_2_N("_last_page_nr", _last_page_nr);

    if (_CURRENT_PAGING_NR >= _last_page_nr)
    {
        _CURRENT_PAGING_NR = 1;
    }

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));

    label_numero_page_derniere->setText(QString::number(_last_page_nr));


    tableView_historique_du_stock->lister_lhistorique_du_stock(_CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_historique_du_stock->resizeColumnsToContents();


    if (tableView_historique_du_stock->rowCount() > 0)
    {
        tableView_historique_du_stock->selectRow(0);
    }

    show();
}


bool YerothHistoriqueDuStockWindow::export_csv_file()
{
    bool success = false;

    QList < int >tableColumnsToIgnore;

    fill_table_columns_to_ignore(tableColumnsToIgnore);

    QString aCurrentStockDesignation =
    			YerothUtils::YEROTH_REPLACE_ALL_EMPTYSPACES_WITH_UNDERSCORE_CHARACTER(_currentStockDesignation);

#ifdef YEROTH_FRANCAIS_LANGUAGE
    success = YerothUtils::export_csv_file(*this,
                                           *tableView_historique_du_stock,
                                           tableColumnsToIgnore,
                                           QString("yeroth-erp-historique-stock-%1-format-csv")
                                           	   .arg(aCurrentStockDesignation), "historique d'un stock");
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
    success = YerothUtils::export_csv_file(*this,
                                           *tableView_historique_du_stock,
                                           tableColumnsToIgnore,
                                           QString("yeroth-erp-%1-stock-history-csv-format")
										   	   .arg(aCurrentStockDesignation), "stock history");
#endif

    return success;
}


bool YerothHistoriqueDuStockWindow::imprimer_pdf_document()
{

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	_latex_template_print_pdf_content = YerothUtils::EN_template_historique_dun_stock_tex;
    }
    else
    {
    	_latex_template_print_pdf_content = YerothUtils::FR_template_historique_dun_stock_tex;
    }


    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.clear();


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        if (_show_ASSET)
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHHISTORYSTOCKASSET", "OF ASSET");
        }
        else
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHHISTORYSTOCKASSET", "Of Stock");
        }
    }
    else
    {
        if (_show_ASSET)
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHHISTORYSTOCKASSET", "DE L'IMMOBILISATION");
        }
        else
        {
            _documentSpecificElements_FOR_PDF_LATEX_PRINTING
				.insert("YEROTHHISTORYSTOCKASSET", "Du Stock");
        }
    }


    //QDEBUG_STRING_OUTPUT_2("_currentStockID", _currentStockID);

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.insert("YEROTHSTOCKID",
                                                            _currentStockID);

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.insert("YEROTHSTOCKHISTORYREFERENCE",
    														YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockReference));

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.insert("YEROTHSTOCKHISTORYDESIGNATION",
    														YerothUtils::LATEX_IN_OUT_handleForeignAccents(_currentStockDesignation));

    _documentSpecificElements_FOR_PDF_LATEX_PRINTING.insert("YEROTHSTOCKHISTORYSTOCKID",
    														YerothWindowsCommons::getLastListerSelectedRow__ID());

    return YerothWindowsCommons::imprimer_pdf_document();
}


void YerothHistoriqueDuStockWindow::textChangedSearchLineEditsQCompleters()
{
}


void YerothHistoriqueDuStockWindow::SLOT_line_per_pagte_MODIFIED(const QString &
                                                                 a_line_per_page_NR)
{
    listHistoriqueDuStock(_a_previous_mouvement_stock_list,
                          _currentStockReference,
                          _currentStockDesignation, _show_ASSET);
}


void YerothHistoriqueDuStockWindow::appliquer_bouton_PREMIERE_PAGE()
{
    _CURRENT_PAGING_NR = 1;

    tableView_historique_du_stock->lister_lhistorique_du_stock(_CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_historique_du_stock->resizeColumnsToContents();

    if (tableView_historique_du_stock->rowCount() > 0)
    {
        tableView_historique_du_stock->selectRow(0);
    }

    label_numero_page_derniere->setText(QString::number(_last_page_nr));

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerothHistoriqueDuStockWindow::appliquer_bouton_PRECEDENTE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR - 1;

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_historique_du_stock->lister_lhistorique_du_stock(_CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_historique_du_stock->resizeColumnsToContents();


    if (tableView_historique_du_stock->rowCount() > 0)
    {
        tableView_historique_du_stock->selectRow(0);
    }

    label_numero_page_derniere->setText(QString::number(_last_page_nr));

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerothHistoriqueDuStockWindow::appliquer_bouton_SUIVANTE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR + 1;

    if (_CURRENT_PAGING_NR > _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size())
    {
        _CURRENT_PAGING_NR = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();
    }

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_historique_du_stock->lister_lhistorique_du_stock(_CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_historique_du_stock->resizeColumnsToContents();


    if (tableView_historique_du_stock->rowCount() > 0)
    {
        tableView_historique_du_stock->selectRow(0);
    }

    label_numero_page_derniere->setText(QString::number(_last_page_nr));

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerothHistoriqueDuStockWindow::appliquer_bouton_DERNIERE_PAGE()
{
    _CURRENT_PAGING_NR = _CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.size();

    if (_CURRENT_PAGING_NR <= 0)
    {
        _CURRENT_PAGING_NR = 1;
    }

    tableView_historique_du_stock->lister_lhistorique_du_stock(_CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST.value(_CURRENT_PAGING_NR));

    tableView_historique_du_stock->resizeColumnsToContents();


    if (tableView_historique_du_stock->rowCount() > 0)
    {
        tableView_historique_du_stock->selectRow(0);
    }

    label_numero_page_derniere->setText(QString::number(_last_page_nr));

    label_numero_page_courante->setText(QString::number(_CURRENT_PAGING_NR));
}


void YerothHistoriqueDuStockWindow::setupShortcuts()
{
    setupShortcutActionPARAMETRER_IMPRESSION_PDF(*action_parametrer_les_impressions);
    setupShortcutActionAfficherPDF(*actionAfficherPDF);
    setupShortcutActionExporterAuFormatCsv(*actionExporter_au_format_csv);
}


void YerothHistoriqueDuStockWindow::setupLineEdits()
{
    lineEdit_nombre_de_lignes_par_page->setValidator(&YerothUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);

    lineEdit_nombre_de_lignes_par_page->setText(QString::number(9));
}
