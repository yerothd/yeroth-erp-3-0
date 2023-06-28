/*
 * yeroth-erp-admin-verif-logging-table-widget.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-admin-verif-logging-table-widget.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"


const unsigned int YerothADMINVerifLoggingTableWidget::TIME_STAMP_COLUMN(0);

const unsigned int YerothADMINVerifLoggingTableWidget::SIGNAL_COLUMN(1);

const unsigned int YerothADMINVerifLoggingTableWidget::SOURCE_COLUMN(2);

const unsigned int YerothADMINVerifLoggingTableWidget::TARGET_COLUMN(3);

const unsigned int YerothADMINVerifLoggingTableWidget::changed_or_MODIFIED_database_QTY_COLUMN(4);



int YerothADMINVerifLoggingTableWidget::addArticle(QString TIMESTAMPtem,
												   QString SIGNALItem,
		   	   	   	   	   	   	   	   	   	  	   QString SOURCEItem,
												   QString TARGETItem,
												   QString changed_OR_modified_database_qty_Item)
{
    _logger->log("addArticle");

    setRowCount(_curRow + 1);

    _TIMESTAMPtem = new YerothQTableWidgetItem(TIMESTAMPtem);
    _SIGNALItem = new YerothQTableWidgetItem(SIGNALItem);
    _SOURCEItem = new YerothQTableWidgetItem(SOURCEItem);
    _TARGETItem = new YerothQTableWidgetItem(TARGETItem);

    _changed_OR_modified_database_qty_Item =
    		new YerothQTableWidgetItem(changed_OR_modified_database_qty_Item);

    _mapListIdxToElement_db_ID.insert(_curRow, SIGNALItem);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _TIMESTAMPtem);
    setItem(_curRow, idx++, _SIGNALItem);
    setItem(_curRow, idx++, _SOURCEItem);
    setItem(_curRow, idx++, _TARGETItem);
    setItem(_curRow, idx++, _changed_OR_modified_database_qty_Item);

    _TIMESTAMPtem->setFlags(_myQStandardItemFlags);
    _SIGNALItem->setFlags(_myQStandardItemFlags);
    _SOURCEItem->setFlags(_myQStandardItemFlags);
    _TARGETItem->setFlags(_myQStandardItemFlags);
    _changed_OR_modified_database_qty_Item->setFlags(_myQStandardItemFlags);

    selectRow(_curRow);

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow < YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
    {
    	++_curRow;
    }
    else
    {
    	_curRow = 0;
    }

    emit addedYerothTableWidget();

    return lastCurRow;
}
