/*
 * yeroth-erp-ADMIN-UPPER-TABLE-VIEW-CLASS.hpp
 *
 * Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_
#define SRC_YEROTH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class yerothERPAdminUpperTableViewClass : public YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline yerothERPAdminUpperTableViewClass()
    :YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
        _stdItemModel->_curTableView = this;
    }

    inline yerothERPAdminUpperTableViewClass(QWidget *parent)
    :YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~yerothERPAdminUpperTableViewClass()
    {
    }

    virtual void SAVE_YEROTH_TABLEVIEW_header_COLUMN_order(YerothWindowsCommons &a_window);

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &aSqlTableModel);
};


#endif /* SRC_YEROTH_ADMIN_UPPER_TABLE_VIEW_CLASS_HPP_ */
