/*
 * yeroth-erp-alertes-table-view.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_
#define SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPAlertesTableView : public YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

    inline YerothERPAlertesTableView()
    :YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPAlertesTableView(QWidget *parent)
    :YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPAlertesTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &alertesSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YEROTH_ALERTES_TABLE_VIEW_HPP_ */
