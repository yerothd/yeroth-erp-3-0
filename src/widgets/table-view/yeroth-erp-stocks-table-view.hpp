/*
 * yeroth-erp-stocks-table-view.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_
#define SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPStocksTableView:public YerothTableViewWITHpagination
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothERPStocksTableView():YerothTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerothERPStocksTableView(QWidget *
                                    parent):YerothTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPStocksTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
                                                    	   const QString &aStockListingStrategy,
    													   const QString &aCurYerothTableViewPageFilter = "",
    													   bool DEPRECATED_stocks_lister_view = false);

    virtual inline void lister_les_elements_du_tableau(YerothSqlTableModel &tableModel,
                                                	   const QString &aStockListingStrategy,
													   const QString &aCurYerothTableViewPageFilter = "")
    {
    	lister_les_elements_du_tableau(tableModel,
    								   aStockListingStrategy,
									   aCurYerothTableViewPageFilter,
									   false);
    }


protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YEROTH_STOCKS_TABLE_VIEW_HPP_ */
