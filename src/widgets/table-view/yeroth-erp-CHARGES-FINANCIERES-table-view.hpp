/*
 * yeroth-erp-CHARGES-FINANCIERES-table-view.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_
#define SRC_YEROTH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>

class YerothSqlTableModel;

class YerothPOSQStandardItemModel;

class YerothERP_CHARGES_FINANCIERES_TableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS


    inline YerothERP_CHARGES_FINANCIERES_TableView()
    :YerothTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }


    inline YerothERP_CHARGES_FINANCIERES_TableView(QWidget *parent)
    :YerothTableViewWITHpagination(parent)
    {
        _stdItemModel->_curTableView = this;
    }


    virtual inline ~YerothERP_CHARGES_FINANCIERES_TableView()
    {
    }


    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &achatAUXFournisseursSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* SRC_YEROTH_CHARGES_FINANCIERES_TABLE_VIEW_HPP_ */
