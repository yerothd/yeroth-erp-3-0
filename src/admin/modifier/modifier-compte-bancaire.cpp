

void YerothAdminModifierWindow::setupEditCompteBancaire()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *comptesBancairesTableModel =
                    lw->getCurSearchSqlTableModel();

    if (0 == comptesBancairesTableModel)
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (false ==
             YerothUtils::isEqualCaseInsensitive(comptesBancairesTableModel->
                                                 sqlTableName(),
                                                 YerothDatabase::
                                                 COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =
                        &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record =
                    comptesBancairesTableModel->record(lw->
                                                       lastSelectedItemForModification());

    lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    lineEdit_modifier_compte_bancaire_institut_bancaire->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::INSTITUT_BANCAIRE));

    textEdit_modifier_compte_bancaire_description_du_compte->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE));
}


void YerothAdminModifierWindow::modifier_compte_bancaire()
{
    if (modifier_compte_bancaire_check_fields())
    {
        YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_compte_bancaire_main();
        YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerothAdminModifierWindow::modifier_compte_bancaire_main()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *comptesBancairesTableModel = lw->getCurSearchSqlTableModel();

    if (0 == comptesBancairesTableModel)
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }
    else if (0 != comptesBancairesTableModel &&
             !YerothUtils::isEqualCaseInsensitive(comptesBancairesTableModel->sqlTableName(),
                     YerothDatabase::COMPTES_BANCAIRES))
    {
        comptesBancairesTableModel =  &_allWindows->getSqlTableModel_comptes_bancaires();
    }

    QSqlRecord record = comptesBancairesTableModel->record(lw->lastSelectedItemForModification());

    QString old_INTITULE_DU_CompteBancaire(
    			GET_SQL_RECORD_DATA(record,
    								YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

    QString new_INTITULE_DU_CompteBancaire(lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->text());


    QSqlQuery CHECK_intitule_du_compte_bancaire_QUERY;

    QString CHECK_WHETHER_intitule_du_compte_bancaire_ALREADY_EXISTS_query =
    		QString("select %1 from %2 where %3='%4';")
				.arg(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
					 YerothDatabase::COMPTES_BANCAIRES,
					 YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
					 new_INTITULE_DU_CompteBancaire);

    int rowCount =
    		YerothUtils::execQuery(CHECK_intitule_du_compte_bancaire_QUERY,
    							   CHECK_WHETHER_intitule_du_compte_bancaire_ALREADY_EXISTS_query);

    if (rowCount > 0)
    {
    	QString retMsg_ALREADY_EXISTS(
    			QObject::tr("Un compte bancaire '%1' existe déjà !")
					.arg(new_INTITULE_DU_CompteBancaire));

        YerothQMessageBox::warning(this,
                                   QObject::tr("Yeroth-pgi-3.0 ~ admin-modifier-ligne budgétaire"),
								    retMsg_ALREADY_EXISTS);

        comptesBancairesTableModel->resetFilter();

        return ;
    }


    record.setValue(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
    		lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->text());

    record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
    				new_INTITULE_DU_CompteBancaire);

    record.setValue(YerothDatabaseTableColumn::INSTITUT_BANCAIRE,
    		lineEdit_modifier_compte_bancaire_institut_bancaire->text());

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE,
    				textEdit_modifier_categorie_description->toPlainText());

    bool success_COMPTE_bancaire_Table = comptesBancairesTableModel
    								->updateRecord(lw->lastSelectedItemForModification(), record);

    if (success_COMPTE_bancaire_Table)
    {
        //Handling of table "lignes_budgetaires"
      QString lignes_budgetaires_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                                           .arg(YerothDatabase::LIGNES_BUDGETAIRES,
                                        		YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
												new_INTITULE_DU_CompteBancaire,
												YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
												old_INTITULE_DU_CompteBancaire));

        YerothUtils::execQuery(lignes_budgetaires_Query, 0);
    }

    if (success_COMPTE_bancaire_Table)
    {
        ////Handling of table "PAIEMENTS"
        QString PAIEMENTS_Query(QString("UPDATE %1 SET %2='%3' WHERE %4='%5'")
                                     .arg(YerothDatabase::PAIEMENTS,
                                          YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
										  new_INTITULE_DU_CompteBancaire,
  										  YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
  										  old_INTITULE_DU_CompteBancaire));

        YerothUtils::execQuery(PAIEMENTS_Query, 0);
    }


    QString retMsg(QObject::tr("LES DONNÉES DU COMPTE BANCAIRE '%1'")
                       .arg(lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->text()));

    if (success_COMPTE_bancaire_Table)
    {
        retMsg.append(QObject::tr(" ont été modifiées avec succès !"));

        YerothQMessageBox::information(this,
                                 QObject::tr("Yeroth-pgi-3.0 ~ admin-modifier-catégorie"),
                                 retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);
         rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " << comptesBancairesTableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerothQMessageBox::warning(this,
                             QObject::tr("Yeroth-pgi-3.0 ~ admin-modifier-catégorie"),
                             retMsg);
    }
}


bool YerothAdminModifierWindow::modifier_compte_bancaire_check_fields()
{
    bool reference_du_compte_bancaire =
                    lineEdit_modifier_compte_bancaire_reference_du_compte_bancaire->checkField
                    ();
    bool intitule_du_compte_bancaire =
                    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->checkField
                    ();
    bool institut_bancaire =
                    lineEdit_modifier_compte_bancaire_institut_bancaire->checkField();

    return reference_du_compte_bancaire &&
           intitule_du_compte_bancaire  &&
		   institut_bancaire;
}

void YerothAdminModifierWindow::clear_compte_bancaire_all_fields()
{
    lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();
    lineEdit_modifier_compte_bancaire_intitule_du_compte_bancaire->clearField();
    lineEdit_modifier_compte_bancaire_institut_bancaire->clearField();
    textEdit_modifier_compte_bancaire_description_du_compte->clear();
}
