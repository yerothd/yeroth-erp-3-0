/*
 * yeroth-erp-table-view.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */


#include "yeroth-erp-table-view.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"


#ifdef YEROTH_SERVER

	#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"

#endif



#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QMap>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>


YerothERPWindows *
YerothTableView::YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER;


YerothTableView::YerothTableView():QTableView()
{
    _writeEnabled = false;
    _tableName = 0;

    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);

    _q_header_view = horizontalHeader();

    if (0 != _q_header_view)
    {
        _q_header_view->setSectionsMovable(true);

        connect(_q_header_view,
                SIGNAL(sectionMoved(int, int, int)),
                this,
                SLOT(handle_yeroth_header_view_position_changed
                     (int, int, int)));
    }
}


YerothTableView::YerothTableView(QWidget *parent):QTableView(parent)
{
    _writeEnabled = false;
    _tableName = 0;

    _stdItemModel = new YerothPOSQStandardItemModel;

    setModel(_stdItemModel);
    setupSelectionOptions();
    setBackgroundRole(QPalette::Window);

    _q_header_view = horizontalHeader();

    if (0 != _q_header_view)
    {
        _q_header_view->setSectionsMovable(true);

        connect(_q_header_view,
                SIGNAL(sectionMoved(int, int, int)),
                this,
                SLOT(handle_yeroth_header_view_position_changed
                     (int, int, int)));
    }
}


const
QStringList &YerothTableView::get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME()
{
    return ((YerothWindowsCommons *) parent()->
            parent())->get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME();
}


void YerothTableView::setupSelectionOptions()
{
    setFrameShadow(QFrame::Plain);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}


void YerothTableView::handle_yeroth_header_view_position_changed(int logicalIndex,
                                                                 int
                                                                 oldVisualIndex,
                                                                 int
                                                                 newVisualIndex)
{
//      QDEBUG_QSTRINGLIST_OUTPUT("_tableModelRawHeaders_IN_OUT",
//                                                         _tableModelRawHeaders_IN_OUT);

//      QDEBUG_STRING_OUTPUT_1(QString("handle_yeroth_header_view_position_changed, "
//                                                                 "logicalIndex: %1,"
//                                                                 "oldVisualIndex: %2, "
//                                                                 "newVisualIndex: %3")
//                                                              .arg(QString::number(logicalIndex),
//                                                                       QString::number(oldVisualIndex),
//                                                                       QString::number(newVisualIndex)));

    YerothWindowsCommons *a_yeroth_window = 0;

    if (0 != parent())
    {
        a_yeroth_window = (YerothWindowsCommons *) parent()->parent();

        if (0 != _q_header_view &&
                0 != a_yeroth_window &&
                !a_yeroth_window->
                GET_CURRENTLY_APPLYING_USER_FILE_SETTING_TABLE_COLUMN_ORDER())
        {
            QStringList
            yerothTableView_model_raw_visible_headers
            (_tableModelRawHeaders_IN_OUT);

            const QStringList *a_visible_DB_columnname_string_List =
                            a_yeroth_window->get_visible_DB_column_name_str_list();

            QString pageTableColumnOrder_STRING;

            if (0 != a_visible_DB_columnname_string_List)
            {
                for (uint i = 0;
                        i < yerothTableView_model_raw_visible_headers.size(); ++i)
                {
                    const QString &headerColumnData =
                                    yerothTableView_model_raw_visible_headers.at(i);

                    if (!headerColumnData.isEmpty())
                    {
                        if (a_visible_DB_columnname_string_List->contains
                                (headerColumnData))
                        {
                            pageTableColumnOrder_STRING.append(QString
                                                               ("%1:%2;").arg
                                                               (headerColumnData,
                                                                QString::number
                                                                (_q_header_view->
                                                                 visualIndex
                                                                 (i))));
                        }
                    }
                }
            }

            a_yeroth_window->set_PARAMETER_TABLE_COLUMN_ORDER
            (pageTableColumnOrder_STRING);

            YerothERPWindows *allWindows = YerothUtils::getAllWindows();

            if (0 != allWindows)
            {
                YerothPOSUser *aUser = allWindows->getUser();

                if (0 != aUser)
                {
                    aUser->save_user_personal_PRINTING_PARAMETER_settings();
                }
            }
        }
    }
}


void YerothTableView::YEROTH_resizeEvent(QResizeEvent *event)
{
    if (0 != event)
    {
//              QDEBUG_STRING_OUTPUT_1("YerothTableView::resizeEvent(QResizeEvent *event)");

        QSize cur_size = size();

        cur_size.scale(event->size(), Qt::KeepAspectRatio);

        resize(cur_size);
    }
}


void YerothTableView::selectionChanged(const QItemSelection &selected,
                                       const QItemSelection &deselected)
{
    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (selectedIndexes.size() > 0)
    {
        _lastSelected_Row__ID = selectedIndexes.at(0).row();
    }

    /*
     * Cet appel de la fonction 'clearFocus' est necessaire pour
     * que les nouveaux elements du tableau soit visible immediatement.
     */
    clearFocus();
}


void YerothTableView::lister_les_elements_du_tableau(YerothSqlTableModel &
                                                     tableModel)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    bool IS_USER_TABLE = false;

    if (YerothUtils::isEqualCaseInsensitive(YerothDatabase::USERS,
                                            _stdItemModel->_curSqlTableModel->sqlTableName()))
    {
        IS_USER_TABLE = true;
    }
    else
    {
        IS_USER_TABLE = false;
    }


    emit signal_lister(tableModel);

    bool s = tableModel.select();

#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(257)));
        }
#endif

    int rows = tableModel.rowCount();

    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);

    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;


    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	    *_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    QStandardItem *anItem = 0;
    QVariant qv;

    if (s)
    {
        QSqlRecord record;

        for (int i = 0; i < rows; ++i)
        {
            record = tableModel.record(i);

            for (int k = 0; k < columns; ++k)
            {
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

                    if (IS_USER_TABLE)
                    {
                        QString tmpQvUSERrole =
                                        YerothUtils::_roleToUserViewString.value(qv.
                                                                                 toInt());
                        anItem = new YerothQStandardItem(tmpQvUSERrole);
                    }
                    else
                    {
                        anItem =
                                        new
                        YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                    }

                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Double:
                    anItem =
                                    new
                    YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::ULongLong:
                    anItem =
                                    new
                    YerothQStandardItem(GET_NUM_STRING(qv.toULongLong()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::LongLong:
                    anItem =
                                    new
                    YerothQStandardItem(GET_NUM_STRING(qv.toLongLong()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Char:
                    anItem = new YerothQStandardItem(QString(qv.toChar()));
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::String:
                    anItem = new YerothQStandardItem(qv.toString());
                    _stdItemModel->setItem(i, k, anItem);
                    break;

                case QVariant::Bool:
                    anItem =
                                    new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE
                                                            : BOOLEAN_STRING_FALSE);
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
                    //qDebug() << "YerothTableView::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                if (anItem)
                {
                    anItem->setForeground(Qt::white);
                }
            }
        }

        resizeColumnsToContents();
    }
}


void YerothTableView::construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr(YerothSqlTableModel &tableModel)
{
    _MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW.clear();

    int rows = tableModel.rowCount();

    QString curTableModelRawHdr;

    QVariant qv;

    QSqlRecord record;

    int db_ID_row_column_number = 0;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curTableModelRawHdr = tableModel.record(i).fieldName(db_ID_row_column_number);

        qv.setValue(tableModel.record(i).value(db_ID_row_column_number));

        switch (qv.type())
        {
        case QVariant::Int:

            if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                    YerothDatabaseTableColumn::ID))
            {
//                      QDEBUG_STRING_OUTPUT_2("YerothTableView::construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr db_ID <--> db_ROW", QString("%1 <--> %2")
//                                      .arg(QString::number(qv.toInt()),
//                                                      QString::number(i)));

                _MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW.insert(qv.toInt(), i);
            }

            break;

        default:
            break;
        }

    }
}


void YerothTableView::lister_les_transactions_dun_fournisseur(QSqlQuery &
                                                              sqlFournisseurTransactionsUnionQuery)
{
    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString operationTypeHdr(QObject::tr("Type d'opération"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString customerAccountValueAfterHdr(QObject::tr
                                         ("Compte fournisseur de services (après)"));
    QString referenceHdr(QObject::tr("Référence service (achat)"));
    QString receiptReferenceHdr(QObject::tr("Référence reçu"));

    _tableModelHeaders.clear();

    _tableModelHeaders.append(dateHdr);
    _tableModelHeaders.append(timeHdr);
    _tableModelHeaders.append(operationTypeHdr);
    _tableModelHeaders.append(transactionAmountHdr);
    _tableModelHeaders.append(customerAccountValueAfterHdr);
    _tableModelHeaders.append(referenceHdr);
    _tableModelHeaders.append(receiptReferenceHdr);


    int querySize = sqlFournisseurTransactionsUnionQuery.size();

    _stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders.size());

    _stdItemModel->setRowCount(querySize);

    //Nous mettons les noms des colones
    for (int k = 0; k < _tableModelHeaders.size(); ++k)
    {
        _stdItemModel->setHeaderData(k, Qt::Horizontal,
                                     _tableModelHeaders.at(k));
    }

    YerothQStandardItem *aYerothQStandardItem = 0;

    if (querySize <= 0)
    {
        return;
    }

    int keyValue;
    QString stringKeyValue;

    for (int i = 0; i < querySize; ++i)
    {
        if (sqlFournisseurTransactionsUnionQuery.next())
        {
            QVariant qv;

            for (int j = 0; j < _stdItemModel->columnCount(); ++j)
            {
                /*
                 * J'ignore le nom de l'entreprise pour la presentation
                 * des donnees de transactions de l'entreprise cliente.
                 * Ce nom d'entreprise apparait deja dans le fichier
                 * PDF.
                 *
                 * J'utilise (j+1), et non (j) comme index !
                 */

                qv = sqlFournisseurTransactionsUnionQuery.value(j + 1);

                //                                      qDebug() << QString("++ j: %1, qv: %2")
                //                                                                      .arg(QString::number(j), qv.toString());

                switch (qv.type())
                {
                case QVariant::Double:
                    /*
                     * Le montant de la transaction est un 'double'.
                     * Dans ce cas de figure, utiliser le chaine de
                     * caractere "N/A" au cas ou il est 'NULL' !
                     */
                    if (qv.isNull())
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem("N/A", Qt::AlignRight);
                    }
                    else
                    {
                        aYerothQStandardItem =
                                        new
                        YerothQStandardItem(GET_DOUBLE_STRING
                                            (qv.toDouble()),
                                            Qt::AlignRight);
                    }

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::String:

                    if (5 == j || 6 == j)
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(qv.toString());
                    }
                    else
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(qv.toString(), false);
                    }

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::Int:
                    keyValue = qv.toInt();

                    if (2 == j)
                    {
                        if (0 < keyValue)
                        {
                            if (YerothUtils::_typedepaiementToUserViewString.
                                    contains(keyValue))
                            {
                                stringKeyValue =
                                                YerothUtils::_typedepaiementToUserViewString.
                                                value(keyValue);
                            }
                            else if (YerothUtils::_typedeventeToUserViewString.
                                     contains(keyValue))
                            {
                                stringKeyValue =
                                                YerothUtils::_typedeventeToUserViewString.
                                                value(keyValue);
                            }
                        }

                        aYerothQStandardItem =
                                        new YerothQStandardItem(stringKeyValue, false);
                        _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    }
                    else
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(keyValue, Qt::AlignRight);
                        _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    }

                    break;

                case QVariant::Date:
                    aYerothQStandardItem =
                                    new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));
                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::Time:
                    aYerothQStandardItem =
                                    new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                default:
                    //qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                _stdItemModel->setItem(i, j, aYerothQStandardItem);
            }
        }
    }
}


void YerothTableView::lister_les_transactions_dun_client(uint
                                                         ROW_COUNT_PER_VIEWING_PAGE,
                                                         QSqlQuery &
                                                         sqlClientTransactionsUnionQuery,
                                                         QMap < uint,
                                                         QString >
                                                         &CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT)
{
    CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT.clear();

    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString operationTypeHdr(QObject::tr("Type d'opération"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString
    customerAccountValueAfterHdr(QObject::tr("Compte client (après)"));
    QString designationHdr(QObject::tr("Désignation"));
    QString referenceHdr(QObject::tr("Référence (article ou service)"));
    QString receiptReferenceHdr(QObject::tr("Référence reçu"));

    _tableModelHeaders.clear();

    _tableModelHeaders.append(dateHdr);
    _tableModelHeaders.append(timeHdr);
    _tableModelHeaders.append(operationTypeHdr);
    _tableModelHeaders.append(transactionAmountHdr);
    _tableModelHeaders.append(customerAccountValueAfterHdr);
    _tableModelHeaders.append(designationHdr);
    _tableModelHeaders.append(referenceHdr);
    _tableModelHeaders.append(receiptReferenceHdr);


    int querySize = sqlClientTransactionsUnionQuery.size();


    _stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders.size());

    _stdItemModel->setRowCount(querySize);

    //Nous mettons les noms des colones
    for (int k = 0; k < _tableModelHeaders.size(); ++k)
    {
        _stdItemModel->setHeaderData(k, Qt::Horizontal,
                                     _tableModelHeaders.at(k));
    }

    YerothQStandardItem *aYerothQStandardItem = 0;

    if (querySize <= 0)
    {
        return;
    }

    int keyValue;
    QString stringKeyValue;

    for (int i = 0; i < querySize; ++i)
    {
        if (sqlClientTransactionsUnionQuery.next())
        {
            QVariant qv;

            for (int j = 0; j < _stdItemModel->columnCount(); ++j)
            {
                /*
                 * J'ignore le nom de l'entreprise pour la presentation
                 * des donnees de transactions de l'entreprise cliente.
                 * Ce nom d'entreprise apparait deja dans le fichier
                 * PDF.
                 *
                 * J'utilise (j+1), et non (j) comme index !
                 */

                qv = sqlClientTransactionsUnionQuery.value(j + 1);

                //                                      qDebug() << QString("++ j: %1, qv: %2")
                //                                                                      .arg(QString::number(j), qv.toString());

                switch (qv.type())
                {
                case QVariant::Double:
                    /*
                     * Le montant de la transaction est un 'double'.
                     * Dans ce cas de figure, utiliser le chaine de
                     * caractere "N/A" au cas ou il est 'NULL' !
                     */
                    if (qv.isNull())
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem("N/A", Qt::AlignRight);
                    }
                    else
                    {
                        double a_double_value = qv.toDouble();

                        aYerothQStandardItem =
                                        new
                        YerothQStandardItem(GET_DOUBLE_STRING
                                            (a_double_value),
                                            Qt::AlignRight);

                        if (aYerothQStandardItem)
                        {
                            aYerothQStandardItem->setForeground(Qt::white);

                            if (a_double_value > 0)
                            {
                                aYerothQStandardItem->setForeground
                                (YerothUtils::YEROTH_YEROTHGREEN_COLOR);
                            }
                            else if (a_double_value < 0)
                            {
                                aYerothQStandardItem->setForeground
                                (YerothUtils::YEROTH_RED_COLOR);
                            }
                        }
                    }

                    aYerothQStandardItem->
                    setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::String:

                    if (4 == j || 5 == j || 6 == j)
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(qv.toString());
                    }
                    else
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(qv.toString(), false);
                    }

                    aYerothQStandardItem->
                    setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::Int:
                    keyValue = qv.toInt();

                    if (2 == j)
                    {
                        if (0 < keyValue)
                        {
                            if (YerothUtils::_typedepaiementToUserViewString.
                                    contains(keyValue))
                            {
                                stringKeyValue =
                                                YerothUtils::_typedepaiementToUserViewString.
                                                value(keyValue);
                            }
                            else if (YerothUtils::_typedeventeToUserViewString.
                                     contains(keyValue))
                            {
                                stringKeyValue =
                                                YerothUtils::_typedeventeToUserViewString.
                                                value(keyValue);
                            }
                        }

                        aYerothQStandardItem =
                                        new YerothQStandardItem(stringKeyValue);

                        _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    }
                    else
                    {
                        aYerothQStandardItem =
                                        new YerothQStandardItem(keyValue);

                        _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    }

                    aYerothQStandardItem->
                    setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

                    break;

                case QVariant::Date:
                    aYerothQStandardItem =
                                    new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                case QVariant::Time:
                    aYerothQStandardItem =
                                    new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));

                    _stdItemModel->setItem(i, j, aYerothQStandardItem);
                    break;

                default:
                    //qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
                    break;
                }

                _stdItemModel->setItem(i, j, aYerothQStandardItem);
            }
        }
    }


    int client_financial_transaction_VIEWING_PAGE_COUNT =
                    qCeil(querySize / ((double) ROW_COUNT_PER_VIEWING_PAGE));

//      QDEBUG_STRING_OUTPUT_2_N("client_financial_transaction_VIEWING_PAGE_COUNT",
//                                                       client_financial_transaction_VIEWING_PAGE_COUNT);
//
//      QDEBUG_STRING_OUTPUT_2_N("sqlClientTransactionsUnionQuery.size(), querySize",
//                                                      querySize);


    if (0 == client_financial_transaction_VIEWING_PAGE_COUNT)
    {
        client_financial_transaction_VIEWING_PAGE_COUNT = 1;
    }


    QString client_financial_transaction_ROW;

    QStringList client_financial_transactions;

    for (int row = 0; row < _stdItemModel->rowCount(); ++row)
    {
        client_financial_transaction_ROW.clear();

        for (int column = 0; column < _stdItemModel->columnCount(); ++column)
        {
            client_financial_transaction_ROW.append(QString("%1%2").arg
                                                    (_stdItemModel->
                                                     item(row,
                                                          column)->text(),
                                                     YerothHistoriqueStock::
                                                     SEPARATION_EXTERNE));
        }

//              QDEBUG_STRING_OUTPUT_2("client_financial_transaction_ROW",
//                                                         client_financial_transaction_ROW);

        client_financial_transactions.append(QString("%1%2").arg
                                             (client_financial_transaction_ROW,
                                              "%"));
    }


    uint k = 1;

    uint s = 0;

    QString a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION;

    for (int j = 0; j < client_financial_transaction_VIEWING_PAGE_COUNT; ++j)
    {
        a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION.clear();

        for (uint i = 0;
                s < client_financial_transactions.size()
                && i < ROW_COUNT_PER_VIEWING_PAGE; ++i)
        {
            a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION.append
            (client_financial_transactions.at(s));

//                      QDEBUG_STRING_OUTPUT_2("a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION",
//                                                                      a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION);

            ++s;
        }

        CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT.insert(k,
                                                            a_paging_viewing_CLIENT_FINANCIAL_TRANSACTION);

        ++k;
    }

//      qDebug() << "CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT"
//                       << CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT;
}


void YerothTableView::lister_les_transactions_dun_client(uint
                                                         ROW_COUNT_PER_VIEWING_PAGE,
                                                         const QString &
                                                         client_financial_transactions_ROWS)
{
    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString operationTypeHdr(QObject::tr("Type d'opération"));
    QString transactionAmountHdr(QObject::tr("Montant total"));
    QString
    customerAccountValueAfterHdr(QObject::tr("Compte client (après)"));
    QString designationHdr(QObject::tr("Désignation"));
    QString referenceHdr(QObject::tr("Réf. (art. ou serv.)"));
    QString receiptReferenceHdr(QObject::tr("Réf. reçu"));

    _tableModelHeaders.clear();

    _tableModelHeaders.append(dateHdr);
    _tableModelHeaders.append(timeHdr);
    _tableModelHeaders.append(operationTypeHdr);
    _tableModelHeaders.append(transactionAmountHdr);
    _tableModelHeaders.append(customerAccountValueAfterHdr);
    _tableModelHeaders.append(designationHdr);
    _tableModelHeaders.append(referenceHdr);
    _tableModelHeaders.append(receiptReferenceHdr);


    int client_financial_transaction_VIEWING_PAGE_COUNT =
                    qCeil(client_financial_transactions_ROWS.size() /
                          ((double) ROW_COUNT_PER_VIEWING_PAGE));


    QStringList
    a_client_financial_transaction_VIEWING_ROWS
    (client_financial_transactions_ROWS.split("%", Qt::SkipEmptyParts));


    int rowSize = a_client_financial_transaction_VIEWING_ROWS.size();


    _stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders.size());

    _stdItemModel->setRowCount(rowSize);

    //Nous mettons les noms des colones
    for (int k = 0; k < _tableModelHeaders.size(); ++k)
    {
        _stdItemModel->setHeaderData(k, Qt::Horizontal,
                                     _tableModelHeaders.at(k));
    }


    QStringList une_transaction_financiere_dun_client;

    YerothQStandardItem *anItem = 0;

//    QDEBUG_STRING_OUTPUT_2_N("a_client_financial_transaction_VIEWING_ROWS.size()",
//                                       a_client_financial_transaction_VIEWING_ROWS.size());

    for (int i = 0; i < a_client_financial_transaction_VIEWING_ROWS.size();
            ++i)
    {
        une_transaction_financiere_dun_client.clear();

//        QDEBUG_STRING_OUTPUT_2("une_transaction_financiere_dun_client",
//                                                 une_transaction_financiere_dun_client);


        une_transaction_financiere_dun_client.append
        (a_client_financial_transaction_VIEWING_ROWS.at(i).
         split(YerothHistoriqueStock::SEPARATION_EXTERNE));

        une_transaction_financiere_dun_client.removeLast();

        for (int j = 0; j < une_transaction_financiere_dun_client.size(); ++j)
        {
            //qDebug() << "++ 1: " << anEntry.at(j);

            anItem =
                            new
            YerothQStandardItem(une_transaction_financiere_dun_client.at
                                (j));

            _stdItemModel->setItem(i, j, anItem);
        }
    }

    resizeColumnsToContents();
}


void YerothTableView::lister_lhistorique_du_stock(const QString &
                                                  aMouvementStockList)
{
    //qDebug() << QString("YerothTableView::lister_lhistorique_du_stock(const QStringList &)");

    QString dateHdr(QObject::tr("Date"));
    QString timeHdr(QObject::tr("Heure"));
    QString idHdr(QObject::tr("ID de l'opération"));
    QString operationHdr(QObject::tr("Type d'opération"));
    QString qteInitialeHdr(QObject::tr("Qté initiale en stock"));
    QString qteRetireeHdr(QObject::tr("Qté en mouvement"));
    QString qteRestanteHdr(QObject::tr("Qté restante en stock"));

    _tableModelHeaders.clear();

    _tableModelHeaders.append(dateHdr);
    _tableModelHeaders.append(timeHdr);
    _tableModelHeaders.append(operationHdr);
    _tableModelHeaders.append(idHdr);
    _tableModelHeaders.append(qteInitialeHdr);
    _tableModelHeaders.append(qteRetireeHdr);
    _tableModelHeaders.append(qteRestanteHdr);

    QStringList
    a_stock_HISTORY_list(aMouvementStockList.split
                         (YerothHistoriqueStock::SEPARATION_EXTERNE,
                          Qt::SkipEmptyParts));

    int rowSize = a_stock_HISTORY_list.size();

    _stdItemModel->yerothPOSClear();

    _stdItemModel->setColumnCount(_tableModelHeaders.size());

    _stdItemModel->setRowCount(rowSize);

    //Nous mettons les noms des colones
    for (int k = 0; k < _tableModelHeaders.size(); ++k)
    {
        _stdItemModel->setHeaderData(k, Qt::Horizontal,
                                     _tableModelHeaders.at(k));
    }

    QString strOut;

    QStringList anEntry;

    QString curTypeMouvementStock;

    QString unMouvementDeStock;

    YerothQStandardItem *anItem = 0;


    for (int i = 0; i < a_stock_HISTORY_list.size(); ++i)
    {
        unMouvementDeStock.clear();
        unMouvementDeStock.append(a_stock_HISTORY_list.at(i));

        anEntry.clear();

        anEntry.append(unMouvementDeStock.split
                       (YerothHistoriqueStock::SEPARATION_INTERNE,
                        Qt::SkipEmptyParts));

        for (int j = 0; j < _stdItemModel->columnCount(); ++j)
        {
            //qDebug() << "++ 1: " << anEntry.at(j);

            if (2 == j)
            {
                curTypeMouvementStock =
                                YerothHistoriqueStock::get_type_mouvement_stock_string
                                (anEntry.at(j));

                anItem = new YerothQStandardItem(curTypeMouvementStock);
            }
            else if (5 == j)
            {
                if (QObject::tr("RÉ-APPROVISIONNEMENT") ==
                        curTypeMouvementStock
                        || QObject::tr("RETOUR VENTE") == curTypeMouvementStock
                        || QObject::tr("ENTRÉE") == curTypeMouvementStock)
                {
                    //5 is corresponds to 'RETOUR'.
                    strOut = "(+) " + anEntry.at(j);
                }
                else
                {
                    strOut = "(-) " + anEntry.at(j);
                }

                anItem = new YerothQStandardItem(strOut, false);
            }
            else
            {
                strOut = anEntry.at(j);
                anItem = new YerothQStandardItem(strOut, false);
            }

            _stdItemModel->setItem(i, j, anItem);
        }
    }

    resizeColumnsToContents();
}


void YerothTableView::lister(YerothSqlTableModel &tableModel,
                             bool truncateString /* = true */)
{
    _stdItemModel->_curSqlTableModel = &tableModel;

    emit signal_lister(tableModel);

    bool s = tableModel.select();

#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(1120)));
        }
#endif

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);
    if (!s)
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
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                                                         REFERENCE))
                {
                    if (true == truncateString
                            && tmpQvString.length() >
                            YerothERPConfig::max_string_display_length)
                    {
                        tmpQvString.
                        truncate(YerothERPConfig::max_string_display_length);
                        tmpQvString.append(".");
                    }
                }

                anItem = new YerothQStandardItem(tmpQvString);
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
                //qDebug() << "YerothTableView:::lister(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }

    resizeColumnsToContents();
}


void YerothTableView::lister_ALL(YerothSqlTableModel &tableModel,
                                 QMap < QString,
                                 QString > &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString anALLQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_ALL);

    bool s = tableModel.yerothSetQuery(anALLQueryStr);

#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(1298)));
        }
#endif

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	    *_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QString tmpQvString;
    QStandardItem *anItem = 0;
    QVariant qv;

    QString curDesignation;

    QSqlRecord record;

    QString curStockID;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DESIGNATION))
                {
                    curDesignation = qv.toString();

                    if (!stockNameToStockID_in_out.contains(curDesignation))
                    {
                        stockNameToStockID_in_out.insert(curDesignation,
                                                         curStockID);
                    }
                }

                tmpQvString.clear();
                tmpQvString.append(qv.toString());

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::
                                                         REFERENCE))
                {
                    if (tmpQvString.length() >
                            YerothERPConfig::max_string_display_length)
                    {
                        tmpQvString.
                        truncate(YerothERPConfig::max_string_display_length);
                        tmpQvString.append(".");
                    }
                }
                anItem = new YerothQStandardItem(tmpQvString);
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
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
                //qDebug() << "YerothTableView::lister_ALL(): undecoded QVariant -> " << qv.type();
                break;
            }

        }			//for columns
    }				//for rows

    resizeColumnsToContents();

    //    qDebug() << "++ ALL, designationToTableRows_in_out: " << designationToTableRows_in_out;
}


void YerothTableView::lister_FIFO(YerothSqlTableModel &tableModel,
                                  QMap < QString,
                                  QString > &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aFIFOQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_FIFO);

    bool s = tableModel.yerothSetQuery(aFIFOQueryStr);


#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(1501)));
        }
#endif


    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QMap < QString, QDate > stockNameToDateEntree;

    QString prevStockName;
    QString curStockName;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QString curStockID;

    bool itemHasExpired = false;
    bool testDateEntreeOK = false;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

        itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DESIGNATION))
                {
                    prevStockName = curStockName;
                    curStockName = qv.toString();
                }

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::
                                                         REFERENCE))
                {
                    anItem = new YerothQStandardItem(qv.toString(), false);
                }
                else
                {
                    anItem = new YerothQStandardItem(qv.toString());
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

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_ENTREE))
                {
                    prevDate = curDate;
                    curDate = qv.toDate();

                    if (curDate < prevDate)
                    {
                        testDateEntreeOK = true;
                        stockNameToDateEntree.insert(curStockName, curDate);
                        stockNameToStockID_in_out.insert(curStockName,
                                                         curStockID);
                        //qDebug() << QString("++ FIFO. 1) curDesignation: %1, curDate: %2")
                        //                      .arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
                    }
                    else
                    {
                        if (!stockNameToDateEntree.contains(curStockName))
                        {
                            testDateEntreeOK = true;
                            stockNameToDateEntree.insert(curStockName,
                                                         curDate);
                            stockNameToStockID_in_out.insert(curStockName,
                                                             curStockID);
                            //qDebug() << QString("++ FIFO. 2) curDesignation: %1, curDate: %2")
                            //                      .arg(curDesignation, DATE_TO_STRING(designationToDateEntree[curDesignation]));
                        }
                    }
                    //qDebug() << "++ 1. test, curDesignation: " << curDesignation
                    //       << ", t date: " << designationToDateEntree[curDesignation]
                    // << ", i row: " << i;
                }

                anItem =
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
                //qDebug() << "YerothTableView::lister_FIFO(): undecoded QVariant -> " << qv.type();
                break;
            }

            if (anItem)
            {
                anItem->setForeground(Qt::white);

                anItem->setTextAlignment(Qt::AlignCenter);

                if (testDateEntreeOK &&
                        YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                            YerothDatabaseTableColumn::
                                                            DATE_ENTREE))
                {
                    anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                    testDateEntreeOK = false;
                }

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        QUANTITE_TOTALE)
                        && (quantite_totale.toDouble() <=
                            stock_dalerte.toDouble()))
                {
                    anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                }

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_PEREMPTION))
                {
                    if (itemHasExpired)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                    }
                }
            }
        }
    }

    resizeColumnsToContents();

//    qDebug() << "++ FIFO, designationToTableRows_in_out: " << stockNameToStockID_in_out;
}


QDate YerothTableView::getEarliestDate(QMultiMap < QString, QDate > allElements,
                                       QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    if (allElements.count(aDesignation) <= 0)
    {
        return defaultDate;
    }

    QDate earliestDate = defaultDate;

    //Let's only keep the stock with the earliest
    //(Date of Preemption First) preemption date
    QMap < QString, QDate >::const_iterator itAllReadyUsed =
                    allElements.find(aDesignation);
    QDate curDate;
    while (itAllReadyUsed != allElements.end())
    {
        curDate = itAllReadyUsed.value();

        if (curDate < earliestDate)
        {
            earliestDate = curDate;
        }

        ++itAllReadyUsed;
    }

    return earliestDate;
}

QDate YerothTableView::getLatestDate(QMultiMap < QString, QDate > allElements,
                                     QString aDesignation)
{
    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    if (allElements.count(aDesignation) <= 0)
    {
        return defaultDate;
    }

    QDate latestDate = defaultDate;

//    qDebug() << QString("++ YerothTableView::getLatestDate. aDesignation: %1 - allElements: ")
//                              .arg(aDesignation)
//               << allElements;

    //Let's only keep the stock with the latest date
    //of stock entry
    QMap < QString, QDate >::const_iterator itAllReadyUsed =
                    allElements.find(aDesignation);
    QDate curDate;

    while (itAllReadyUsed != allElements.end())
    {
        curDate = itAllReadyUsed.value();

        if (curDate > latestDate)
        {
            latestDate = curDate;
        }

        ++itAllReadyUsed;
    }

//    QDEBUG_STRING_OUTPUT_2("latestDate", DATE_TO_STRING(latestDate));

    return latestDate;
}

void YerothTableView::lister_LIFO(YerothSqlTableModel &tableModel,
                                  QMap < QString,
                                  QString > &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aLIFOStringQuery =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_LIFO);

    bool s = tableModel.yerothSetQuery(aLIFOStringQuery);


#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(1861)));
        }
#endif


    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *aPreviousLIFOGreenItem = 0;
    QStandardItem *anItem = 0;
    QVariant qv;

    QMap < QString, QDate > designationToDateEntree;

    QString prevStockName;
    QString curStockName;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    QMultiMap < QString, QDate > allElements;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DESIGNATION))
                {
                    prevStockName = curStockName;
                    curStockName = qv.toString();
                }

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::
                                                         REFERENCE))
                {
                    anItem = new YerothQStandardItem(qv.toString(), false);
                }
                else
                {
                    anItem = new YerothQStandardItem(qv.toString());
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

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_ENTREE))
                {
                    curDate = qv.toDate();

//                              QDEBUG_STRING_OUTPUT_2("prevDate", DATE_TO_STRING(prevDate));
//                              QDEBUG_STRING_OUTPUT_2("curDate", DATE_TO_STRING(curDate));

                    if (curDate >= prevDate)
                    {
                        prevDate = curDate;

                        designationToDateEntree.insert(curStockName, curDate);
                        stockNameToStockID_in_out.insert(curStockName,
                                                         curStockID);
                        allElements.insert(curStockName, curDate);

//                                      qDebug() << QString("++ LIFO. 1) curDesignation: %1, prevDate: %2, curDate: %3")
//                                                      .arg(curStockName,
//                                                               DATE_TO_STRING(prevDate),
//                                                               DATE_TO_STRING(designationToDateEntree[curStockName]));
                    }
                    else
                    {
                        if (!designationToDateEntree.contains(curStockName))
                        {
                            designationToDateEntree.insert(curStockName,
                                                           prevDate);
                            stockNameToStockID_in_out.insert(curStockName,
                                                             curStockID);
                            allElements.insert(curStockName, prevDate);

//                                              qDebug() << QString("++ LIFO. 2) curDesignation: %1, curDate: %2")
//                                                                      .arg(curStockName, DATE_TO_STRING(designationToDateEntree[curStockName]));
                        }
                    }
                }

                anItem =
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
                //qDebug() << "YerothTableView::lister_LIFO(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }

    resizeColumnsToContents();

//    qDebug() << "++ LIFO, stockNameToStockID_in_out: " << stockNameToStockID_in_out;
}


void YerothTableView::lister_FEFO(YerothSqlTableModel &tableModel,
                                  QMap < QString,
                                  QString > &stockNameToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aDEFDEOQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_FEFO);

    bool s = tableModel.yerothSetQuery(aDEFDEOQueryStr);


#ifdef YEROTH_SERVER

    	YerothERPWindows *allWindows = YerothUtils::getAllWindows();

        YEROTH_RUNTIME_VERIFIER *dbusServer = allWindows->dbusServer();
        if (dbusServer)
        {
        	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED
							(QString("%1;%2;%3")
								.arg(tableModel.sqlTableName(),
									 "src/widgets/table-view/yeroth-erp-table-view.cpp",
									 QString::number(2116)));
        }
#endif


    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    //QStandardItem *prevItem = 0;
    QVariant qv;

    QMap < QString, QDate > stockNameToDatePreemption;
    QMultiMap < QString, QDate > allElements;

    QString prevDesignation;
    QString curDesignation;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    bool testDatePreemptionOK = false;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

        itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (0 != anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DESIGNATION))
                {
                    prevDesignation = curDesignation;
                    curDesignation = qv.toString();
                }

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::
                                                         REFERENCE))
                {
                    anItem = new YerothQStandardItem(qv.toString(), false);
                }
                else
                {
                    anItem = new YerothQStandardItem(qv.toString());
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

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_PEREMPTION))
                {
                    prevDate = curDate;
                    curDate = qv.toDate();

                    if (curDate < prevDate)
                    {
                        testDatePreemptionOK = true;
                        allElements.insert(curDesignation, curDate);
                        //qDebug() << "++ 1-OK. curDesignation: "
                        //<< curDesignation << " date: " << curDate;
                    }
                    else
                    {
                        if (!stockNameToDatePreemption.contains
                                (curDesignation))
                        {
                            testDatePreemptionOK = true;
                            allElements.insert(curDesignation, curDate);
                            //qDebug() << "++ 2-OK. curDesignation: "
                            //<< curDesignation << " date: " << curDate;
                        }
                    }
                }

                anItem =
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//                      qDebug() << "YerothTableView::lister_FEFO(): undecoded QVariant -> " << qv.type();
                break;
            }

            if (0 != anItem)
            {
                anItem->setForeground(Qt::white);
                //prevItem = anItem;

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        QUANTITE_TOTALE)
                        && quantite_totale.toDouble() <= stock_dalerte.toDouble())
                {
                    anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                }
                else if (YerothUtils::isEqualCaseInsensitive
                         (curTableModelRawHdr,
                          YerothDatabaseTableColumn::DATE_PEREMPTION))
                {
                    if (itemHasExpired)
                    {
                        anItem->setForeground(YerothUtils::YEROTH_RED_COLOR);
                    }
                    else if (testDatePreemptionOK)
                    {
                        QDate earliestDate =
                                        getEarliestDate(allElements, curDesignation);

                        //qDebug() << "\t++ earliestDate: " << earliestDate;
                        QStandardItem *item;
                        QStandardItem *itemDesignation;
                        QString aDesignation;
                        QDate aDate;

                        for (int h = 0; h < i; ++h)
                        {
                            itemDesignation = _stdItemModel->item(h,
                                                                  YerothUtils::
                                                                  get_index_of_table_raw_column
                                                                  (tableModelRawHeaders,
                                                                   YerothDatabaseTableColumn::
                                                                   DESIGNATION));

                            if (itemDesignation)
                            {
                                aDesignation = itemDesignation->text();
                                aDate =
                                                stockNameToDatePreemption.value
                                                (aDesignation);
                                //qDebug() << "++ itemDesignation: " << aDesignation;

                                if (YerothUtils::isEqualCaseInsensitive
                                        (aDesignation, curDesignation))
                                {
                                    item = _stdItemModel->item(h,
                                                               YerothUtils::
                                                               get_index_of_table_raw_column
                                                               (tableModelRawHeaders,
                                                                YerothDatabaseTableColumn::
                                                                DATE_PEREMPTION));

                                    bool isPreempted = aDate < GET_CURRENT_DATE;
                                    if (!isPreempted)
                                    {
                                        item->setForeground(Qt::white);
                                        //qDebug() << QString("\twhite for item %1 at (%2, %3).")
                                        //                                      .arg(item->text(),
                                        //                                      QString::number(h),
                                        //                                      QString::number(DATE_PREEMPTION_COLUMN));
                                    }
                                    else
                                    {
                                        //item is preempted. so leave its colour as it is (red).
                                    }
                                }
                            }
                        }

                        stockNameToDatePreemption.insert(curDesignation,
                                                         earliestDate);
                        stockNameToStockID_in_out.insert(curDesignation,
                                                         curStockID);

                        //qDebug() << "++ ici: " << designationToDatePreemption;

                        anItem->setForeground(YerothUtils::YEROTH_GREEN_COLOR);
                        //qDebug() << QString("\tgreen for %1 with date %2.")
                        //                                      .arg(curDesignation,
                        //                                      DATE_TO_STRING(earliestDate));

                        testDatePreemptionOK = false;
                    }
                }
            }			// first if
        }			//switch-case
    }

    resizeColumnsToContents();
    //    qDebug() << "++ FEFO, designationToTableRows_in_out: " << designationToTableRows_in_out;
}


void YerothTableView::lister_codebar_ALL(YerothSqlTableModel &tableModel,
                                         QMap < QString,
                                         QString >
                                         &stockReferenceToStockID_in_out)
{
    stockReferenceToStockID_in_out.clear();

    emit signal_lister(tableModel);

    QString aReferenceALLQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_ALL);

//    qDebug() << QString("++ aReferenceALLQueryStr: %1")
//                              .arg(aReferenceALLQueryStr);

    bool s = tableModel.yerothSetQuery(aReferenceALLQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString curStockReference;

    QSqlRecord record;

    QVariant date_premption;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

        itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (0 != anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        REFERENCE)
                        && !itemHasExpired)
                {
                    curStockReference = qv.toString();

                    if (!stockReferenceToStockID_in_out.contains
                            (curStockReference))
                    {
                        stockReferenceToStockID_in_out.insert
                        (curStockReference, curStockID);
                        anItem = new YerothQStandardItem(curStockReference);
                        _stdItemModel->setItem(i, k, anItem);
                    }
                }

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
//                      qDebug() << "YerothTableView::lister_codebar_ALL(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }
    //    qDebug() << "++ ALL, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_FIFO(YerothSqlTableModel &tableModel,
                                          QMap < QString,
                                          QString >
                                          &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceFIFOQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_FIFO);

    bool s = tableModel.yerothSetQuery(aReferenceFIFOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant date_premption;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

        itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (0 != anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        REFERENCE)
                        && !itemHasExpired)
                {
                    prevStockReference = curStockReference;
                    curStockReference = qv.toString();
                }
                anItem = new YerothQStandardItem(qv.toString());
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem =
                                new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE :
                                                        BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_ENTREE)
                        && !itemHasExpired)
                {
                    prevDate = curDate;
                    curDate = qv.toDate();

                    if (curDate < prevDate)
                    {
                        stockReferenceToStockID_in_out.insert
                        (curStockReference, curStockID);
                    }
                    else
                    {
                        if (!stockReferenceToStockID_in_out.contains
                                (curStockReference))
                        {
                            stockReferenceToStockID_in_out.insert
                            (curStockReference, curStockID);
                        }
                    }
                }

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
//                      qDebug() << "YerothTableView::lister_codebar_FIFO(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }

    //    qDebug() << "++ FIFO, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_LIFO(YerothSqlTableModel &tableModel,
                                          QMap < QString,
                                          QString >
                                          &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceLIFOStringQuery =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_LIFO);

    bool s = tableModel.yerothSetQuery(aReferenceLIFOStringQuery);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() - 7, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant date_premption;
    QVariant quantite_totale;
    QVariant stock_dalerte;

    QString curStockID;

    bool itemHasExpired = false;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);
        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);

        itemHasExpired = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        REFERENCE)
                        && !itemHasExpired)
                {
                    prevStockReference = curStockReference;
                    curStockReference = qv.toString();
                }

                anItem = new YerothQStandardItem(qv.toString());
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem =
                                new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE :
                                                        BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:

                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_ENTREE)
                        && !itemHasExpired)
                {
                    prevDate = curDate;
                    curDate = qv.toDate();

                    if (curDate >= prevDate)
                    {
                        stockReferenceToStockID_in_out.insert
                        (curStockReference, curStockID);
                    }
                    else
                    {
                        if (!stockReferenceToStockID_in_out.contains
                                (curStockReference))
                        {
                            stockReferenceToStockID_in_out.insert
                            (curStockReference, curStockID);
                        }
                    }
                }

                anItem =
                                new YerothQStandardItem(DATE_TO_STRING(qv.toDate()));;
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Time:
                anItem =
                                new YerothQStandardItem(TIME_TO_STRING(qv.toTime()));
                _stdItemModel->setItem(i, k, anItem);
                break;

            default:
                anItem = new YerothQStandardItem(YerothUtils::EMPTY_STRING);
//                      qDebug() << "YerothTableView::lister_codebar_LIFO(): undecoded QVariant -> " << qv.type();
                break;
            }
        }
    }
    //    qDebug() << "++ LIFO, codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}


void YerothTableView::lister_codebar_FEFO(YerothSqlTableModel &tableModel,
                                          QMap < QString,
                                          QString >
                                          &stockReferenceToStockID_in_out)
{
    emit signal_lister(tableModel);

    QString aReferenceDEFDEOQueryStr =
                    YerothUtils::getStrategySqlQueryStr(tableModel.yerothSelectStatement(),
                                                        YerothERPConfig::
                                                        STRATEGIE_VENTE_SORTIE_FEFO);

    bool s = tableModel.yerothSetQuery(aReferenceDEFDEOQueryStr);

    int rows = tableModel.rowCount();
    int columns = tableModel.columnCount();

    _stdItemModel->setRowCount(rows);
    _stdItemModel->setColumnCount(columns);

    QStringList tableModelRawHeaders;

    YerothUtils::createTableModelHeaders_DEPRECATED(tableModel,
                                         	 	 	*_stdItemModel,
													_tableModelHeaders,
													tableModelRawHeaders);

    if (!s)
    {
        return;
    }

    QString curTableModelRawHdr;

    QStandardItem *anItem = 0;
    QVariant qv;

    QString prevStockReference;
    QString curStockReference;

    QDate defaultDate;
    defaultDate.setDate(QDate::currentDate().year() + 1000, 12, 31);

    QDate curDate = defaultDate;
    QDate prevDate = defaultDate;

    QSqlRecord record;

    QVariant quantite_totale;
    QVariant stock_dalerte;
    QVariant date_premption;

    QString curStockID;

    bool itemIsPreempted = false;

    for (int i = 0; i < rows; ++i)
    {
        record = tableModel.record(i);

        curStockID = record.value(YerothDatabaseTableColumn::ID).toString();

        quantite_totale =
                        record.value(YerothDatabaseTableColumn::QUANTITE_TOTALE);
        stock_dalerte = record.value(YerothDatabaseTableColumn::STOCK_DALERTE);
        date_premption =
                        record.value(YerothDatabaseTableColumn::DATE_PEREMPTION);

        itemIsPreempted = (date_premption.toDate() < GET_CURRENT_DATE);

        for (int k = 0; k < columns; ++k)
        {
            curTableModelRawHdr = tableModelRawHeaders.at(k);

            qv.setValue(tableModel.record(i).value(k));

            anItem = _stdItemModel->item(i, k);

            if (anItem)
            {
                delete anItem;
            }

            switch (qv.type())
            {
            case QVariant::UInt:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toUInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toUInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Int:

                if (!YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                         YerothDatabaseTableColumn::ID))
                {
                    anItem =
                                    new YerothQStandardItem(GET_NUM_STRING(qv.toInt()));
                }
                else
                {
                    anItem =
                                    new YerothQStandardItem(QString::number(qv.toInt()));
                }

                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Double:
                anItem =
                                new YerothQStandardItem(GET_DOUBLE_STRING(qv.toDouble()));
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
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        REFERENCE)
                        && !itemIsPreempted)
                {
                    prevStockReference = curStockReference;
                    curStockReference = qv.toString();
                }

                anItem = new YerothQStandardItem(qv.toString());
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Bool:
                anItem =
                                new YerothQStandardItem(qv.toBool()? BOOLEAN_STRING_TRUE :
                                                        BOOLEAN_STRING_FALSE);
                _stdItemModel->setItem(i, k, anItem);
                break;

            case QVariant::Date:
                if (YerothUtils::isEqualCaseInsensitive(curTableModelRawHdr,
                                                        YerothDatabaseTableColumn::
                                                        DATE_PEREMPTION)
                        && !itemIsPreempted)
                {
                    prevDate = curDate;
                    curDate = qv.toDate();

                    if (curDate < prevDate)
                    {
                        stockReferenceToStockID_in_out.insert
                        (curStockReference, curStockID);
                        //qDebug() << "++ 1-OK. curCodebar: "
                        //       << curCodebar << " date: " << curDate;
                    }
                    else
                    {
                        if (!stockReferenceToStockID_in_out.contains
                                (curStockReference))
                        {
                            stockReferenceToStockID_in_out.insert
                            (curStockReference, curStockID);
                        }
                    }
                }

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
//                      qDebug() << "YerothTableView::lister_codebar_FEFO(): undecoded QVariant -> " << qv.type();
                break;
            }
        }			//switch-case
    }
//    qDebug() << "++ codebarToTableRows_in_out: " << codebarToTableRows_in_out;
}
