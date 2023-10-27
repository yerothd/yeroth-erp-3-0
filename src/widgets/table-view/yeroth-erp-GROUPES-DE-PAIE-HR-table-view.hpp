/*
 * yeroth-erp-GROUPES-DE-PAIE-HR-table-view.hpp
 *
 * Author: XAVIER NOUMBISSI NOUNDOU, PH.D., P. ENG.
 */

#ifndef _SRC_YEROTH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_
#define _SRC_YEROTH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/widgets/yeroth-erp-qstandard-item-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include <QtSql/QSqlTableModel>


class YerothSqlTableModel;

class YerothPOSQStandardItemModel;


class YerothERPGroupesDePaieHRTableView : public YerothTableViewWITHpagination
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothERPGroupesDePaieHRTableView()
    :YerothTableViewWITHpagination()
    {
        _stdItemModel->_curTableView = this;
    }

    inline YerothERPGroupesDePaieHRTableView(QWidget *parent)
    :YerothTableViewWITHpagination(parent)
    {
        _stdItemModel->_curTableView = this;
    }

    virtual inline ~YerothERPGroupesDePaieHRTableView()
    {
    }

    virtual void lister_les_elements_du_tableau(YerothSqlTableModel &clientGroupSqlTableModel);

protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);
};


#endif /* _SRC_YEROTH_GROUPES_DE_PAIE_HR_TABLE_VIEW_HPP_ */
