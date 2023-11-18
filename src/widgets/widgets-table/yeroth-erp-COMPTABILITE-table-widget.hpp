/*
 * yeroth-erp-COMPTABILITE-table-widget.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_COMPTABILITE_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_COMPTABILITE_TABLE_WIDGET_HPP_

#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"

class YerothComptabiliteTableWidget : public YerothTableWidget
{
    Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothComptabiliteTableWidget(QWidget *parent = 0)
	:YerothTableWidget(parent),
	 _yr_type_doperation_financiere_item(0),
	 _yr_intitule_ligne_budgetaire_item(0)
	{
	}

	virtual inline ~YerothComptabiliteTableWidget()
	{
	}

	int add_an_association(QString yr_type_doperation_financiere_DB_ID,
                           QString yr_type_doperation_financiere,
                           QString yr_intitule_ligne_budgetaire);


	static const unsigned int TYPE_DOPERATION_FINANCIERE_COLUMN;

	static const unsigned int INTITULE_LIGNE_BUDGETAIRE_COLUMN;


private:

	YerothQTableWidgetItem *_yr_type_doperation_financiere_item;

	YerothQTableWidgetItem *_yr_intitule_ligne_budgetaire_item;
};

#endif /* SRC_WIDGETS_YEROTH_COMPTABILITE_TABLE_WIDGET_HPP_ */
