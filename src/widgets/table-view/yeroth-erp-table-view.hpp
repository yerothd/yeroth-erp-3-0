/*
 * yeroth-erp-table-view.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_TABLE_VIEW_HPP_
#define SRC_YEROTH_TABLE_VIEW_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QStringList>

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"


#include <QtWidgets/QTableView>

#include <QtSql/QSqlTableModel>


class YerothERPWindows;

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothWindowsCommons;


class YerothTableView : public QTableView
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothTableView();

    YerothTableView(QWidget *parent);

    virtual inline ~YerothTableView()
    {
        delete _stdItemModel;
    }

    inline void setWriteEnabled(bool w)
    {
        _writeEnabled = w;
    }

    inline bool isWriteEnabled()
    {
        return _writeEnabled;
    }

    void YEROTH_resizeEvent(QResizeEvent *event);

    const QStringList &get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME();

    void setupSelectionOptions();

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                tableModel);

    virtual inline void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                       tableModel,
                                                       const QString &
                                                       aStockListingStrategy,
                                                       const QString &
                                                       aCurYerothTableViewPageFilter
                                                       = "")
    {
    }

    virtual void construire_le_MAPPING_ORIGINAL_db_ID_VERS_db_row_Nr(YerothSqlTableModel &
                                                                     tableModel);

    void lister_les_transactions_dun_fournisseur(QSqlQuery &
                                                 sqlFournisseurTransactionsUnionQuery);

    void lister_les_transactions_dun_client(uint ROW_COUNT_PER_VIEWING_PAGE,
                                            QSqlQuery &
                                            sqlClientTransactionsUnionQuery,
                                            QMap<uint,
                                            QString>
                                            &CURRENT_PAGING_NR__TO__TABLE_VIEW_LIST_INOUT);

    void lister_les_transactions_dun_client(uint ROW_COUNT_PER_VIEWING_PAGE,
                                            const QString &
                                            client_financial_transactions_ROWS);

    void lister_lhistorique_du_stock(const QString &aMouvementStockList);

    void lister(YerothSqlTableModel &sqlTableModel,
                bool truncateString = true);

    void lister_ALL(YerothSqlTableModel &tableModel,
                    QMap<QString, QString> &stockNameToStockID_in_out);

    void lister_FIFO(YerothSqlTableModel &tableModel,
                     QMap<QString, QString> &stockNameToStockID_in_out);

    QDate getEarliestDate(QMultiMap < QString, QDate> allElements,
                          QString aDesignation);

    QDate getLatestDate(QMultiMap < QString, QDate> allElements,
                        QString aDesignation);

    void lister_LIFO(YerothSqlTableModel &tableModel,
                     QMap<QString, QString> &stockNameToStockID_in_out);

    void lister_FEFO(YerothSqlTableModel &tableModel,
                     QMap<QString, QString> &stockNameToStockID_in_out);

    void lister_codebar_ALL(YerothSqlTableModel &tableModel,
                            QMap<QString,
                            QString> &stockReferenceToStockID_in_out);

    void lister_codebar_FIFO(YerothSqlTableModel &tableModel,
                             QMap<QString,
                             QString> &stockReferenceToStockID_in_out);

    void lister_codebar_LIFO(YerothSqlTableModel &tableModel,
                             QMap<QString,
                             QString> &stockReferenceToStockID_in_out);

    void lister_codebar_FEFO(YerothSqlTableModel &tableModel,
                             QMap<QString,
                             QString> &stockReferenceToStockID_in_out);

    inline int rowCount()
    {
        return _stdItemModel->rowCount();
    }

    inline QString getSqlTableName()
    {
        return (0 != _tableName) ? QString(*_tableName) : "";
    }

    inline void setSqlTableName(const QString *tableName)
    {
        _tableName = tableName;
    }

    inline int lastSelected_Rows__IDs_INT_SIZE()
    {
        return _MAP_lastSelected_Row__TO__DB_ID.size();
    }

    inline const QMap<QString, QString> &lastSelected_Rows__IDs()
    {
        return _MAP_lastSelected_Row__TO__DB_ID;
    }

    inline const QString &lastSelectedRow__ID()
    {
        return _lastSelected_Row__ID;
    }

    inline void setLastSelectedRow__ID(const QString &aLastSelectedRow__ID)
    {
        _lastSelected_Row__ID = aLastSelectedRow__ID;
    }

    inline const QStringList &getTableModelRawHeaders()
    {
        return _tableModelRawHeaders_IN_OUT;
    }

    inline YerothPOSQStandardItemModel *getStandardItemModel()
    {
        return _stdItemModel;
    }

    inline void setStandardItemModel(YerothPOSQStandardItemModel *
                                     stdItemModel)
    {
        _stdItemModel = stdItemModel;
    }

    static YerothERPWindows *YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER;


    QMap<int, int> _MAP_ORIGINAL_NON_FILTERED_DB_ID__TO__ORIGINAL_DB_ROW;

    /**
     * This map must be filled during execution of method
     * 'lister_les_elements_du_tableau'.
     */
    QMap<QString, int> _map_dbID_TO_yeroth_table_view_ROW_NUMBER;

    QMap<QString, QString> _MAP_lastSelected_Row__TO__DB_ID;

signals:

    void signal_lister(YerothSqlTableModel &sqlTableModel);

protected slots:

	virtual void handle_yeroth_header_view_position_changed(int logicalIndex,
                                                            int oldVisualIndex,
                                                            int newVisualIndex);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);

    bool _writeEnabled;

    QString _lastSelected_Row__ID;

    const QString *_tableName;

    QHeaderView *_q_header_view;

    YerothPOSQStandardItemModel *_stdItemModel;

    QStringList _tableModelRawHeaders_IN_OUT;

    QStringList _tableModelHeaders;
};


#endif /* SRC_YEROTH_TABLE_VIEW_HPP_ */
