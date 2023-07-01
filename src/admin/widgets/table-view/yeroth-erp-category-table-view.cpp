/*
 * yeroth-erp-category-table-view.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "yeroth-erp-category-table-view.hpp"


#include "src/yeroth-erp-windows.hpp"


#include "src/utils/yeroth-erp-utils.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"


#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>



void YerothERPCategoryTableView::
		lister_les_elements_du_tableau(YerothSqlTableModel &categoriesSqlTableModel)
{
    _stdItemModel->_curSqlTableModel = &categoriesSqlTableModel;

    emit signal_lister(categoriesSqlTableModel);

    bool s = true;

    if (_needExecSelectStatement_FOR_TABLE_VIEW_PAGING_LISTING)
    {
        s = categoriesSqlTableModel.select();
    }

    int rows = categoriesSqlTableModel.rowCount();
    int columns = categoriesSqlTableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    YerothUtils::createTableModelHeaders(categoriesSqlTableModel,
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

    QString curTableModelRawHdr;

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    for (int i = 0; i < rows; ++i)
    {
        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = categoriesSqlTableModel.record(i).fieldName(k);

            if (!_tableModelRawHeaders_IN_OUT.contains(curTableModelRawHdr))
            {
                continue;
            }

            qv.setValue(categoriesSqlTableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:
                anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:
                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr, YerothDatabaseTableColumn::ID))
                {
                    anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem = new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem = new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::ULongLong:
                anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::LongLong:
                anItem = new YerothQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Char:
                anItem = new YerothQStandardItem(QString(qv.toChar()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::String:
                tmpQvString.clear();
                tmpQvString.append(qv.toString());

                anItem = new YerothQStandardItem(tmpQvString, false);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem = new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                anItem = new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem = new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
                //qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }

    static YerothERPWindows *curAllWindows = YerothUtils::getAllWindows();

    YEROTH_SET_LAST_TABLE_VIEW_SELECTED_ROW_ID(curAllWindows,
                                               curAllWindows->_adminListerWindow)

    resizeColumnsToContents();

    curAllWindows->_adminListerWindow->handle_some_actions_tools_enabled();
}



