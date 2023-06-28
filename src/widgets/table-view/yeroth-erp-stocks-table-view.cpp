/*
 * yeroth-erp-stocks-table-view.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-stocks-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


static void COLOR_ASSET_ROW(uint a_row_index,
                            double quantite_totale,
                            double stock_dalerte,
                            QDate date_premption,
                            QStringList &table_model_raw_headers,
                            YerothPOSQStandardItemModel &stdItemModel)
{
    QString a_cur_table_raw_header;

    QStandardItem *anItem = 0;

    bool already_colored = false;

    for (uint column = 0; column < stdItemModel.columnCount(); ++column)
    {
        a_cur_table_raw_header = table_model_raw_headers.at(column);

        anItem = stdItemModel.item(a_row_index, column);

        if (0 != anItem)
        {
            already_colored = false;

//                      QDEBUG_STRING_OUTPUT_2("a_cur_table_raw_header", a_cur_table_raw_header);

            if (YerothUtils::isEqualCaseInsensitive(a_cur_table_raw_header,
                                                    YerothDatabaseTableColumn::
                                                    QUANTITE_TOTALE)
                    && quantite_totale <= stock_dalerte)
            {
                already_colored = true;
                anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
            }
            else if (YerothUtils::isEqualCaseInsensitive
                     (a_cur_table_raw_header,
                      YerothDatabaseTableColumn::DATE_PEREMPTION)
                     && date_premption <= GET_CURRENT_DATE)
            {
                already_colored = true;
                anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
            }

            if (!already_colored)
            {
                anItem->setForeground(YerothUtils::YEROTH_WHITE_COLOR);
                already_colored = false;
            }
        }
    }
}


void YerothERPStocksTableView::lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
                                                              const QString &aStockListingStrategy,
															  const QString &aCurYerothTableViewPageFilter /* = "" */,
															  bool DEPRECATED_stocks_lister_view /* = false */)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    QString curStockListingStrategy(aStockListingStrategy);

    if (curStockListingStrategy.isEmpty())
    {
        curStockListingStrategy.append(YerothERPConfig::salesStrategy);
    }

    static QString curStrategyQueryString;

    bool s = true;

    if (YerothUtils::isEqualCaseInsensitive
            (YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, curStockListingStrategy))
    {
        curStrategyQueryString.clear();
        curStrategyQueryString.append(YerothUtils::getStrategySqlQueryStr
                                      (tableModel.yerothSelectStatement(),
                                       YerothERPConfig::
                                       STRATEGIE_VENTE_SORTIE_FIFO));

        if (!aCurYerothTableViewPageFilter.isEmpty())
        {
            curStrategyQueryString.append(QString(" %1").
                                          arg(aCurYerothTableViewPageFilter));
        }

        s = tableModel.yerothSetQuery(curStrategyQueryString);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
              curStockListingStrategy))
    {
        curStrategyQueryString.clear();
        curStrategyQueryString.append(YerothUtils::getStrategySqlQueryStr
                                      (tableModel.yerothSelectStatement(),
                                       YerothERPConfig::
                                       STRATEGIE_VENTE_SORTIE_LIFO));

        if (!aCurYerothTableViewPageFilter.isEmpty())
        {
            curStrategyQueryString.append(QString(" %1").
                                          arg(aCurYerothTableViewPageFilter));
        }

        s = tableModel.yerothSetQuery(curStrategyQueryString);
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
              curStockListingStrategy))
    {
        curStrategyQueryString.clear();
        curStrategyQueryString.append(YerothUtils::getStrategySqlQueryStr
                                      (tableModel.yerothSelectStatement(),
                                       YerothERPConfig::
                                       STRATEGIE_VENTE_SORTIE_FEFO));

        if (!aCurYerothTableViewPageFilter.isEmpty())
        {
            curStrategyQueryString.append(QString(" %1").
                                          arg(aCurYerothTableViewPageFilter));
        }

        s = tableModel.yerothSetQuery(curStrategyQueryString);
    }
    else				// YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL
    {
        curStrategyQueryString.clear();
        curStrategyQueryString.append(YerothUtils::getStrategySqlQueryStr
                                      (tableModel.yerothSelectStatement(),
                                       YerothERPConfig::
                                       STRATEGIE_VENTE_SORTIE_ALL));

        if (!aCurYerothTableViewPageFilter.isEmpty())
        {
            curStrategyQueryString.append(QString(" %1").
                                          arg(aCurYerothTableViewPageFilter));
        }

        s = tableModel.yerothSetQuery(curStrategyQueryString);
    }

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);


    if (! DEPRECATED_stocks_lister_view)
    {
    	YerothUtils::createTableModelHeaders(tableModel,
    	                                     *_stdItemModel,
    	                                      _tableModelHeaders,
    	                                      _tableModelRawHeaders_IN_OUT,
    										  *_currentViewWindow);
    }
    else
    {
    	YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
    	                                         	 	*_stdItemModel,
														_tableModelHeaders,
														_tableModelRawHeaders_IN_OUT);
    }



    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());

    if (!s || (0 == _tableModelHeaders.size())
            || (0 == _tableModelRawHeaders_IN_OUT.size()))
    {
        return;
    }

    QString curTableModelRawHdr;

    QString tmpQvString;

    bool PREVIOUS_ROW_IS_ASSET = false;

    QStandardItem *anItem = 0;

    QVariant qv;

    QSqlRecord record;

    QVariant prix_vente;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    for (uint i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        prix_vente = record.value(YerothDatabaseTableColumn::PRIX_VENTE);

        if (prix_vente.toDouble() <= 0)
        {
            PREVIOUS_ROW_IS_ASSET = true;
        }

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

        for (uint k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModel.record(i).fieldName(k);

            if (!_tableModelRawHeaders_IN_OUT.contains(curTableModelRawHdr))
            {
                continue;
            }

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            anItem = new YerothQStandardItem;

            switch (qv.type())
            {
            case QVariant::UInt:

                anItem =
                                new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    if (YerothUtils::isEqualCaseInsensitive
                            (curTableModelRawHdr,
                             YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF))
                    {
                        tmpQvString =
                                        YerothUtils::_alerte_OUI_NON_ToUserViewString.
                                        value(qv.toInt()).toUpper();

                        anItem = new YerothQStandardItem(tmpQvString);
                    }
                    else
                    {
                        anItem =
                                        new
                        YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                    }
                }
                else
                {
                    YEROTH_SAVE_ID_TO_ROW_NUMBER_FOR_YEROTH_TABLE_VIEW
                    (tmpQvString, qv.toInt(), i) anItem =
                                    new YerothQStandardItem(tmpQvString);

//                              QDEBUG_STRINGS_OUTPUT_1(QString("row: %1, db_ID: %2")
//                                                                                      .arg(QString::number(i), tmpQvString));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                //quantite_totale (c'est la quantite restante en stock)

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        QUANTITE_TOTALE))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new
                    YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::ULongLong:
                anItem =
                                new YerothQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::LongLong:
                anItem =
                                new YerothQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Char:
                anItem = new YerothQStandardItem(QString(qv.toChar()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::String:
                tmpQvString.clear();
                tmpQvString.append(qv.toString());

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::
                                                         REFERENCE)
                        && !YerothUtils::
                        isEqualCaseInsensitive(curTableModelRawHdr,
                                               YerothDatabaseTableColumn::
                                               REFERENCE_RECU_DACHAT))
                {
                    anItem = new YerothQStandardItem(tmpQvString, false);
                }
                else
                {
                    anItem = new YerothQStandardItem(tmpQvString);
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem =
                                new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE :
                                                        BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                anItem =
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;
            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
                _stdItemModel->setItem(i, k, anItem);
                //qDebug() << "YerothERPStocksTableView::lister(): undecoded QVariant -> " << qv.type();
                break;
            }

            if (anItem)
            {
                anItem->setForeground(Qt::white);

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        QUANTITE_TOTALE)
                        && quantite_totale.toDouble() <= stock_dalerte.toDouble())
                {
                    anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                }
                else if (YerothUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr,
                          YerothDatabaseTableColumn::DATE_PEREMPTION)
                         && date_premption.toDate() <= GET_CURRENT_DATE)
                {
                    anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                }
            }
        }			//for columns

        if (PREVIOUS_ROW_IS_ASSET)
        {
            COLOR_ASSET_ROW(i,
                            quantite_totale.toDouble(),
                            stock_dalerte.toDouble(),
                            date_premption.toDate(),
                            _tableModelRawHeaders_IN_OUT, *_stdItemModel);

            PREVIOUS_ROW_IS_ASSET = false;
        }

    }

    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->_stocksWindow)
    resizeColumnsToContents();

    curAllWindows->_stocksWindow->handle_some_actions_tools_enabled();
}


void YerothERPStocksTableView::selectionChanged(const QItemSelection &selected,
                                                const QItemSelection &deselected)
{
    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    _MAP_lastSelected_Row__TO__DB_ID.clear();

    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
        uint last_Row_ID = selectedIndexes.size() - 1;

        for (uint j = 0; j < selectedIndexes.size(); ++j)
        {
            curAllWindows->_stocksWindow->getQModelIndex_dbID_from_MODEL_INDEX
            (selectedIndexes.at(j), db_ID_in_out);

            _MAP_lastSelected_Row__TO__DB_ID.
            insert(QString::number(selectedIndexes.at(j).row()),
                   db_ID_in_out);

            if (last_Row_ID == j)
            {
                _lastSelected_Row__ID = selectedIndexes.at(j).row();
            }
        }
    }

//    qDebug() << _MAP_lastSelected_Row__TO__DB_ID;

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}
