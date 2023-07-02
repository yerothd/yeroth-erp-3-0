/*
 * yeroth-erp-financial-expense-table-view.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_CHARGES_FINANCIERES_VIEW_HPP_
#define SRC_YEROTH_ADMIN_CHARGES_FINANCIERES_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/admin/widgets/table-view/yeroth-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPFinancialExpenseTableView : public yerothERPAdminUpperTableViewClass
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothERPFinancialExpenseTableView()
    :yerothERPAdminUpperTableViewClass()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPFinancialExpenseTableView(QWidget *parent)
    :yerothERPAdminUpperTableViewClass(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    inline virtual ~YerothERPFinancialExpenseTableView()
    {
    }
};


#endif /* SRC_YEROTH_ADMIN_CHARGES_FINANCIERES_VIEW_HPP_ */
