/*
 * yeroth-erp-comptes-doperations-comptabilite-table-view.hpp.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-comptes-doperations-comptabilite-table-view.hpp"

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


void YerothERPComptes_doperations_comptabilite_TableView::
		lister_les_elements_du_tableau(YerothSqlTableModel &tableModel)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
        s = tableModel.select();
    }

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(tableModel,
                                         *_stdItemModel,
                                         _tableModelHeaders,
                                         _tableModelRawHeaders_IN_OUT,
										 _currentViewWindow);


    _stdItemModel->setColumnCount(_tableModelRawHeaders_IN_OUT.size());


    if (!s || (0 == _tableModelHeaders.size()) ||
    	(0 == _tableModelRawHeaders_IN_OUT.size()))
    {
        return;
    }

    QString
    curTableModelRawHdr;

    QString
    tmpQvString;

    QStandardItem *
    anItem = 0;

    QVariant
    qv;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
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
                delete
                anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem =
                                new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        TYPE_DE_PAIEMENT))
                {
                    tmpQvString =
                                    YerothUtils::_typedepaiementToUserViewString.value(qv.
                                                                                       toInt
                                                                                       ());
                    anItem = new YerothQStandardItem(tmpQvString, false);
                }
                else if (!YerothUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    YEROTH_SAVE_ID_TO_ROW_NUMBER_FOR_YEROTH_TABLE_VIEW
                    (tmpQvString, qv.toInt(), i) anItem =
                                    new YerothQStandardItem(tmpQvString);
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        MONTANT_PAYE)
                        || YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                               YerothDatabaseTableColumn::
                                                               COMPTE_CLIENT)
                        || YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                               YerothDatabaseTableColumn::
                                                               COMPTE_FOURNISSEUR))
                {
                    anItem->
                    setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
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
                                                         REFERENCE_DU_COMPTE_BANCAIRE))
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
                                new YerothQStandardItem(BOOL_TO_STRING(qv.toBool()));
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
                //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                break;
            }

            if (anItem)
            {
                anItem->setForeground(Qt::white);
            }
        }
    }

    static YerothERPWindows *
    curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->
                                               _comptabiliteWindow)
    resizeColumnsToContents();

    curAllWindows->_comptabiliteWindow->handle_some_actions_tools_enabled();
}


void YerothERPComptes_doperations_comptabilite_TableView::selectionChanged(const
                                                                           QItemSelection
                                                                           &
                                                                           selected,
                                                                           const
                                                                           QItemSelection
                                                                           &
                                                                           deselected)
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
            curAllWindows->
            _comptabiliteWindow->getQModelIndex_dbID_from_MODEL_INDEX
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

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}
