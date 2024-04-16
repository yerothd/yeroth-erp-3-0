/*
 * yeroth-erp-select-db-qcheckbox.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_
#define SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_


#include "src/widgets/yeroth-erp-qcheckbox.hpp"


class YerothSelectDBQCheckBox : public YerothERPQCheckBox
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothSelectDBQCheckBox(QWidget *parent,
							QStringList *visibleDBFieldColumnStrList);


    virtual inline ~YerothSelectDBQCheckBox()
    {
    }

    virtual inline void set_visibleDBFieldColumnStrList(QStringList *A_visibleDBFieldColumnStrList)
    {
    	_visibleDBFieldColumnStrList = A_visibleDBFieldColumnStrList;
    }

public slots:

    void handle_visible_db_field_checkBox(bool clicked);


private:

    QStringList *_visibleDBFieldColumnStrList;

};

#endif /* SRC_WIDGETS_YEROTH_SELECT_DB_QCHECKBOX_HPP_ */
