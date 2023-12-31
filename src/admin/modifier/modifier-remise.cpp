
void YerothAdminModifierWindow::setupEditRemise()
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("administration ~ modifier ~ remises"));

    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *remisesTableModel = lw->getCurSearchSqlTableModel();

    if (!remisesTableModel)
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remisesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(remisesTableModel->
                                                  sqlTableName(),
                                                  YerothDatabase::REMISES))
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record =
                    remisesTableModel->record(lw->lastSelectedItemForModification());

}

void YerothAdminModifierWindow::modifier_remise()
{
    //_logger->log("modifier_remise");
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;
    YerothSqlTableModel *remisesTableModel = lw->getCurSearchSqlTableModel();

    if (!remisesTableModel)
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }
    else if (remisesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(remisesTableModel->
                                                  sqlTableName(),
                                                  YerothDatabase::REMISES))
    {
        remisesTableModel = &_allWindows->getSqlTableModel_remises();
    }

    QSqlRecord record =
                    remisesTableModel->record(lw->lastSelectedItemForModification());

}



bool YerothAdminModifierWindow::modifier_remise_check_fields(QString
                                                             previousDiscountText)
{
    return false;
}

void YerothAdminModifierWindow::clear_remise_all_fields()
{
}
