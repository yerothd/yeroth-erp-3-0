/*
 * yeroth-erp-admin-verif-logging-table-widget.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_
#define SRC_WIDGETS_YEROTH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_

#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"

class YerothADMINVerifLoggingTableWidget: public YerothTableWidget
{
	Q_OBJECT

public:

	YEROTH_CLASS_OPERATORS

	inline YerothADMINVerifLoggingTableWidget(QWidget *parent = 0)
	:YerothTableWidget(parent),
	 _TIMESTAMPtem(0),
	 _SIGNALItem(0),
	 _SOURCEItem(0),
	 _TARGETItem(0),
	 _changed_OR_modified_database_qty_Item(0)
	{
		setRowCount(12);
	}

	virtual inline ~YerothADMINVerifLoggingTableWidget()
	{
	}

	int addArticle(QString TIMESTAMPtem,
				   QString SIGNALItem,
				   QString SOURCEItem,
				   QString TARGETItem,
				   QString changed_OR_modified_database_qty_Item);

	static const unsigned int TIME_STAMP_COLUMN;

	static const unsigned int SIGNAL_COLUMN;

	static const unsigned int SOURCE_COLUMN;

	static const unsigned int TARGET_COLUMN;

	static const unsigned int changed_or_MODIFIED_database_QTY_COLUMN;

private:

	YerothQTableWidgetItem *_TIMESTAMPtem;

	YerothQTableWidgetItem *_SIGNALItem;

	YerothQTableWidgetItem *_SOURCEItem;

	YerothQTableWidgetItem *_TARGETItem;

	YerothQTableWidgetItem *_changed_OR_modified_database_qty_Item;
};

#endif //SRC_WIDGETS_YEROTH_ADMIN_VERIF_LOGGING_TABLE_WIDGET_HPP_
