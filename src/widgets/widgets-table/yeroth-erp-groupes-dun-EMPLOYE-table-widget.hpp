/*
 * yeroth-erp-groupes-dun-EMPLOYE-table-widget.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_


#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"


class YerothGROUPES_DUN_EMPLOYE_Window;


class YerothGroupesDunEmployeTableWidget : public YerothTableWidget
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	inline YerothGroupesDunEmployeTableWidget(QWidget *parent = 0)
    :YerothTableWidget(parent),
     _groupDesignationItem(0),
     _groupReferenceItem(0),
	 _groupNumberItem(0)
    {
    }

    inline virtual ~YerothGroupesDunEmployeTableWidget()
    {
    }

    int insert_group(const QString &group_db_ID,
                     const QString &groupDesignation,
                     const QString &groupReference,
                     const QString &groupNumber,
                     bool          COLOR_RED = false);


protected:

    virtual void selectionChanged(const QItemSelection &selected,
                                  const QItemSelection &deselected);



public:

    YerothGROUPES_DUN_EMPLOYE_Window *a_current_window;


private:

    YerothQTableWidgetItem *_groupDesignationItem;

    YerothQTableWidgetItem *_groupReferenceItem;

    YerothQTableWidgetItem *_groupNumberItem;
};


#endif /* SRC_WIDGETS_YEROTH_GROUPE_DUN_EMPLOYE_TABLE_WIDGET_HPP_ */
