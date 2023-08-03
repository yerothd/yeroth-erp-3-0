/*
 * yeroth-erp-groupes-dun-EMPLOYE-table-widget.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#include "src/widgets/widgets-table/yeroth-erp-groupes-dun-EMPLOYE-table-widget.hpp"


#include "src/windows/supplier/yeroth-erp-GROUPES-dun-employe-hr-window.hpp"


int YerothGroupesDunEmployeTableWidget::insert_group(const QString &group_db_ID,
                                                     const QString &groupDesignation,
                                                     const QString &groupReference,
                                                     const QString &groupNumber)
{
//      QDEBUG_STRING_OUTPUT_2("YerothGroupesDunClientWindow::insert_group", groupDesignation);

    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setRowCount(_curRow + 1);

    _groupDesignationItem = new YerothQTableWidgetItem(groupDesignation);
    _groupReferenceItem = new YerothQTableWidgetItem(groupReference);
    _groupNumberItem = new YerothQTableWidgetItem(groupNumber);

    _mapListIdxToElement_db_ID.insert(_curRow, group_db_ID);

    unsigned idx = 0;

    //Each call to setItem triggers a call to YerothPointDeVenteWindow::handleQteChange
    setItem(_curRow, idx++, _groupDesignationItem);
    setItem(_curRow, idx++, _groupReferenceItem);
    setItem(_curRow, idx++, _groupNumberItem);

    resizeColumnsToContents();

    int lastCurRow = _curRow;

    if (_curRow >= 0)
    {
        selectRow(_curRow);
    }

    ++_curRow;

    set_MY_QStandardItemFlags();

    emit addedYerothTableWidget();

    return lastCurRow;
}


void YerothGroupesDunEmployeTableWidget::
        selectionChanged(const QItemSelection &selected,
                         const QItemSelection &deselected)
{
    QModelIndexList selectedIndexes = QAbstractItemView::selectedIndexes();

    QString db_ID_in_out;

    if (0 != a_current_window   &&
        selectedIndexes.size() > 0)
    {
        int row = selectedIndexes.at(0).row();

        int column = selectedIndexes.at(0).column();

        a_current_window
            ->handle_DATE_DEBUT_et_DATE_FIN_dappartenance(row);
    }
}



