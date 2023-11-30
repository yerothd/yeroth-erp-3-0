/*
 * yeroth-erp-clients-table-view.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPClientsTableView:public YerothTableViewWITHpagination
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothERPClientsTableView():YerothTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerothERPClientsTableView(QWidget *
                                     parent):YerothTableViewWITHpagination
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPClientsTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                achatSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YEROTH_CLIENTS_TABLE_VIEW_HPP_ */
