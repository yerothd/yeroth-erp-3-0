/*
 * yeroth-erp-COMPTABILITE-table-widget.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-COMPTABILITE-table-widget.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


const unsigned int YerothComptabiliteTableWidget::TYPE_DOPERATION_FINANCIERE_COLUMN(0);

const unsigned int YerothComptabiliteTableWidget::INTITULE_LIGNE_BUDGETAIRE_COLUMN(1);



int YerothComptabiliteTableWidget::
        add_an_association(QString yr_type_doperation_financiere_DB_ID,
                           QString yr_type_doperation_financiere,
                           QString yr_intitule_ligne_budgetaire)
{
    _logger->log("YerothComptabiliteTableWidget::add_an_association");


    setRowCount(_curRow + 1);

    _yr_type_doperation_financiere_item =
        new YerothQTableWidgetItem(yr_type_doperation_financiere);

    _yr_intitule_ligne_budgetaire_item =
        new YerothQTableWidgetItem(yr_intitule_ligne_budgetaire);


    _mapListIdxToElement_db_ID
        .insert(_curRow,
                yr_type_doperation_financiere_DB_ID);


    unsigned idx = 0;

    setItem(_curRow, idx++, _yr_type_doperation_financiere_item);
    setItem(_curRow, idx, _yr_intitule_ligne_budgetaire_item);

    _yr_type_doperation_financiere_item->setFlags(_myQStandardItemFlags);
    _yr_intitule_ligne_budgetaire_item->setFlags(_myQStandardItemFlags);

    resize_columns_AND_rows_to_contents();

    int lastCurRow = _curRow;

    ++_curRow;

    return lastCurRow;
}


