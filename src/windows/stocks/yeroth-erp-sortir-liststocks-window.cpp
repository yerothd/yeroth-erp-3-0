/*
 * yeroth-erp-sortir-liststocks-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */
#include"yeroth-erp-sortir-liststocks-window.hpp"

#include"src/yeroth-erp-windows.hpp"

#include"src/utils/yeroth-erp-sqltable-model.hpp"

#include"src/utils/yeroth-erp-config.hpp"

#include"src/users/yeroth-erp-users.hpp"


#include<QtWidgets/QDesktopWidget>


YerothSortirListStocksWindow::YerothSortirListStocksWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothSortirListStocksWindow"))
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("stocks à sortir ou à transférer");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);


    setYerothTableView_FROM_WINDOWS_COMMONS(tableView_list_stocks);


    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
            << YerothDatabaseTableColumn::REFERENCE
            << YerothDatabaseTableColumn::DESCRIPTION_PRODUIT
            << YerothDatabaseTableColumn::PRIX_UNITAIRE
            << YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS
            << YerothDatabaseTableColumn::MONTANT_TVA
            << YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS
            << YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS
            << YerothDatabaseTableColumn::STOCK_DALERTE
            << YerothDatabaseTableColumn::LOCALISATION_STOCK
            << YerothDatabaseTableColumn::IMAGE_PRODUIT
            << YerothDatabaseTableColumn::DATE_ENTREE
            << YerothDatabaseTableColumn::DATE_PEREMPTION
            << YerothDatabaseTableColumn::LOCALISATION
            << YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR
            << YerothDatabaseTableColumn::LOTS_ENTRANT
            << YerothDatabaseTableColumn::QUANTITE_PAR_LOT
            << YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF
            << YerothDatabaseTableColumn::HISTORIQUE_STOCK
            << YerothDatabaseTableColumn::PRIX_DACHAT
            << YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT
            << YerothDatabaseTableColumn::ENREGISTREUR_STOCK
            << YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT
            << YerothDatabaseTableColumn::IS_SERVICE;

    setup_select_configure_dbcolumn(YerothDatabase::STOCKS);

    enableResizing();
}


void YerothSortirListStocksWindow::listStocks(YerothSqlTableModel &aSqlTableModel)
{
	retranslateUi(this);

	tableView_list_stocks->_currentViewWindow = this;

    tableView_list_stocks->lister_les_elements_du_tableau(aSqlTableModel,
                                                          YerothERPConfig::salesStrategy,
														  YerothUtils::EMPTY_STRING,
														  false);

    if (YerothUtils::isEqualCaseInsensitive
            (YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
             YerothERPConfig::salesStrategy)
            || YerothUtils::isEqualCaseInsensitive(YerothERPConfig::
                                                   STRATEGIE_VENTE_SORTIE_FIFO,
                                                   YerothERPConfig::salesStrategy)
            || YerothUtils::isEqualCaseInsensitive(YerothERPConfig::
                                                   STRATEGIE_VENTE_SORTIE_LIFO,
                                                   YerothERPConfig::salesStrategy))
    {
        tableView_list_stocks->setSortingEnabled(false);
    }
    else				//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
        tableView_list_stocks->setSortingEnabled(true);
    }

    yeroth_hide_columns();

    tableView_list_stocks->selectRow(0);

    char *aWindowTitleWithStrategy = const_cast<char *>
         			(YerothUtils::getWindowTitleWithStrategy(this).toStdString().c_str());

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName(aWindowTitleWithStrategy),
      								 _curStocksTableModel);

    show();
}
