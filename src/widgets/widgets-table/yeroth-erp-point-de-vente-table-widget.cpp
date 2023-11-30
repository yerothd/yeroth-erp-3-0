/*
 * yeroth-erp-point-de-vente-table-widget.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-point-de-vente-table-widget.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


const unsigned int YerothPointDeVenteTableWidget::REFERENCE_COLUMN(0);

const unsigned int YerothPointDeVenteTableWidget::DESIGNATION_COLUMN(1);

const unsigned int YerothPointDeVenteTableWidget::CATEGORIE_COLUMN(2);

const unsigned int YerothPointDeVenteTableWidget::TOTAL_TVA_COLUMN(6);

const unsigned int YerothPointDeVenteTableWidget::SERVICE_COST_COLUMN(7);

const unsigned int YerothPointDeVenteTableWidget::TOTAL_COLUMN(8);

const unsigned int YerothPointDeVenteTableWidget::QTE_COLUMN(3);

const unsigned int YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK(4);


int YerothPointDeVenteTableWidget::addArticleAfter(YerothArticleVenteInfo &
                                                   articleVenteInfo)
{
    _logger->log("addArticleAfter(YerothArticleVenteInfo &)");

    setRowCount(_curRow + 1);

    double prix_unitaire = articleVenteInfo.prix_unitaire();

    if (articleVenteInfo.remise_prix() > 0)
    {
        prix_unitaire =
                        articleVenteInfo.prix_unitaire() - articleVenteInfo.remise_prix();
    }

    _stockReferenceItem =
                    new YerothQTableWidgetItem(articleVenteInfo._stockReference);
    _stockNameItem = new YerothQTableWidgetItem(articleVenteInfo._stockName);
    _stockCategorieItem =
                    new YerothQTableWidgetItem(articleVenteInfo._stockCategorie);
    _prixUnitaireItem =
                    new YerothQTableWidgetItem(QString::number(prix_unitaire, 'f', 2));
    _montantTvaItem =
                    new YerothQTableWidgetItem(articleVenteInfo.montantTva());

    _SERVICECOSTItem =
                    new YerothQTableWidgetItem(articleVenteInfo.Get_ajout_pour_COMMERCIAL_text());

    _prixVenteItem =
                    new
    YerothQTableWidgetItem(GET_CURRENCY_STRING_NUM(articleVenteInfo.prix_vente()));

    _qteItem = new YerothQTableWidgetItem(articleVenteInfo.quantiteAVendre());
    _qteTotalStockItem =
                    new YerothQTableWidgetItem(articleVenteInfo.quantiteEnStock());


    _mapListIdxToElement_db_ID.insert(_curRow, articleVenteInfo._stockID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _stockReferenceItem);
    setItem(_curRow, idx++, _stockNameItem);
    setItem(_curRow, idx++, _stockCategorieItem);
    setItem(_curRow, idx++, _qteItem);
    setItem(_curRow, idx++, _qteTotalStockItem);
    setItem(_curRow, idx++, _prixUnitaireItem);
    setItem(_curRow, idx++, _montantTvaItem);
    setItem(_curRow, idx++, _SERVICECOSTItem);
    setItem(_curRow, idx, _prixVenteItem);

    _stockReferenceItem->setFlags(_myQStandardItemFlags);
    _stockNameItem->setFlags(_myQStandardItemFlags);
    _stockCategorieItem->setFlags(_myQStandardItemFlags);
    _qteItem->setFlags(_myQStandardItemFlags | Qt::ItemIsEditable);
    _qteTotalStockItem->setFlags(_myQStandardItemFlags);
    _prixUnitaireItem->setFlags(_myQStandardItemFlags);
    _montantTvaItem->setFlags(_myQStandardItemFlags);
    _SERVICECOSTItem->setFlags(_myQStandardItemFlags);
    _prixVenteItem->setFlags(_myQStandardItemFlags);

    setColours_QTE_QStandardItem();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerothTableWidget();

    return lastCurRow;
}


int YerothPointDeVenteTableWidget::addArticle(QString stockID,
                                              QString stockReference,
                                              QString stockName,
                                              QString stockCategorie,
                                              QString prix_unitaire,
											  QString montant_tva,
											  QString service_cost,
                                              QString prix_vente,
                                              QString quantite_a_vendre,
                                              QString quantite_total_stock)
{
    _logger->log("addArticle");

    setRowCount(_curRow + 1);

    _stockReferenceItem = new YerothQTableWidgetItem(stockReference);
    _stockNameItem = new YerothQTableWidgetItem(stockName);
    _stockCategorieItem = new YerothQTableWidgetItem(stockCategorie);
    _prixUnitaireItem = new YerothQTableWidgetItem(prix_unitaire);
    _montantTvaItem = new YerothQTableWidgetItem(montant_tva);
    _SERVICECOSTItem = new YerothQTableWidgetItem(service_cost);
    _prixVenteItem = new YerothQTableWidgetItem(prix_vente);
    _qteItem = new YerothQTableWidgetItem(quantite_a_vendre);
    _qteTotalStockItem = new YerothQTableWidgetItem(quantite_total_stock);


    _mapListIdxToElement_db_ID.insert(_curRow, stockID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _stockReferenceItem);
    setItem(_curRow, idx++, _stockNameItem);
    setItem(_curRow, idx++, _stockCategorieItem);
    setItem(_curRow, idx++, _qteItem);
    setItem(_curRow, idx++, _qteTotalStockItem);
    setItem(_curRow, idx++, _prixUnitaireItem);
    setItem(_curRow, idx++, _montantTvaItem);
    setItem(_curRow, idx++, _SERVICECOSTItem);
    setItem(_curRow, idx, _prixVenteItem);

    _stockReferenceItem->setFlags(_myQStandardItemFlags);
    _stockNameItem->setFlags(_myQStandardItemFlags);
    _stockCategorieItem->setFlags(_myQStandardItemFlags);
    _qteItem->setFlags(_myQStandardItemFlags | Qt::ItemIsEditable);
    _qteTotalStockItem->setFlags(_myQStandardItemFlags);
    _prixUnitaireItem->setFlags(_myQStandardItemFlags);
    _montantTvaItem->setFlags(_myQStandardItemFlags);
    _SERVICECOSTItem->setFlags(_myQStandardItemFlags);
    _prixVenteItem->setFlags(_myQStandardItemFlags);

    setColours_QTE_QStandardItem();

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    ++_curRow;

    emit addedYerothTableWidget();

    return lastCurRow;
}
