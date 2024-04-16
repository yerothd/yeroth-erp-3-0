/*
 * yeroth-erp-paiements-table-view.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_
#define SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view_WITH_TABWIDGET_AS_DIRECT_PARENT_movable_section.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERPPaiementsTableView:public
    YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline
    YerothERPPaiementsTableView
    ():YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPPaiementsTableView(QWidget *
                                       parent):YerothERP_TABLE_VIEW_WITH_TABWIDGET_AS_DIRECT_PARENT_MOVABLE_SECTION
        (parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPPaiementsTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &
                                                tableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YEROTH_PAIEMENTS_TABLE_VIEW_HPP_ */
