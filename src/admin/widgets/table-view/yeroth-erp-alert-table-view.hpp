/*
 * yeroth-erp-alert-table-view.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_
#define SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/admin/widgets/table-view/yeroth-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAlertTableView : public yerothERPAdminUpperTableViewClass
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothERPAlertTableView()
    :yerothERPAdminUpperTableViewClass()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPAlertTableView(QWidget *parent)
    :yerothERPAdminUpperTableViewClass(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPAlertTableView()
    {
    }
};


#endif /* SRC_YEROTH_ADMIN_ALERT_TABLE_VIEW_HPP_ */
