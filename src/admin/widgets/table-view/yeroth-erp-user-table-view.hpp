/*
 * yeroth-erp-user-table-view.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_USER_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_USER_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/admin/widgets/table-view/yeroth-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include <QtSql/QSqlTableModel>


class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPUserTableView : public yerothERPAdminUpperTableViewClass
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothERPUserTableView()
    :yerothERPAdminUpperTableViewClass()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPUserTableView(QWidget *parent)
    :yerothERPAdminUpperTableViewClass(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPUserTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &aSqlTableModel);
};


#endif /* SRC_YEROTH_ADMIN_USER_TABLE_VIEW_HPP_ */
