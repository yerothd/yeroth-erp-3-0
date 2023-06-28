
void YerothAdminCreateWindow::populate_LIGNE_BUDGETAIRE_ComboBoxes()
{
    _logger->log("populate_LIGNE_BUDGETAIRE_ComboBoxes");

	comboBox_INTITULE_DU_COMPTE_BANCAIRE
		->populateComboBoxRawString(YerothDatabase::COMPTES_BANCAIRES,
									YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);
}


bool YerothAdminCreateWindow::creer_ligne_budgetaire()
{
    if (creer_ligne_budgetaire_CHECK_fields())
    {
        QString retMsg(QObject::tr("La LIGNE BUDGÉTAIRE '"));

        YerothSqlTableModel &LIGNES_BUDGETAIRES_TableModel =
                        _allWindows->getSqlTableModel_lignes_budgetaires();

        LIGNES_BUDGETAIRES_TableModel
			.yerothSetFilter_WITH_where_clause(QString("LOWER(intitule_de_la_ligne_budgetaire) = LOWER('%1')")
												.arg(lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->text()));

        //qDebug() << "++ filter: " << LIGNES_BUDGETAIRES_TableModel.filter();

        int LIGNES_BUDGETAIRES_TableModelRowCount = LIGNES_BUDGETAIRES_TableModel.easySelect();

        //qDebug() << "++ result LIGNES_BUDGETAIRES_TableModelRowCount : " << LIGNES_BUDGETAIRES_TableModelRowCount ;

        if (LIGNES_BUDGETAIRES_TableModelRowCount > 0)
        {
            QSqlRecord record = LIGNES_BUDGETAIRES_TableModel.record(0);

            QString duplicate_LIGNE_BUDGETAIRE(
            			GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::LIGNE_BUDGETAIRE));

            //qDebug() << "++ duplicate LIGNE_BUDGETAIRE: " << duplicate_LIGNE_BUDGETAIRE;

            retMsg.append(QObject::tr("%1' existe déjà ! ").
                          arg(duplicate_LIGNE_BUDGETAIRE));

            YerothQMessageBox::warning(this,
                                       QObject::tr("catégorie"), retMsg);

            LIGNES_BUDGETAIRES_TableModel.resetFilter();

            return false;
        }

        LIGNES_BUDGETAIRES_TableModel.resetFilter();

        QSqlRecord record = LIGNES_BUDGETAIRES_TableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID,
                        YerothERPWindows::getNextIdSqlTableModel_LIGNES_BUDGETAIRES());

        record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
        		comboBox_INTITULE_DU_COMPTE_BANCAIRE->currentText());

        record.setValue(YerothDatabaseTableColumn::montant_de_la_ligne_budgetaire,
        		lineEdit_creer_montant_de_la_LIGNE_BUDGETAIRE->text().toDouble());

        record.setValue(YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
        		lineEdit_creer_montant_de_la_LIGNE_BUDGETAIRE->text().toDouble());

        record.setValue(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
        		lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->text());

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_ligne_budgetaire,
        		textEdit_creer_une_LIGNE_budgetaire->toPlainText());


        bool success =
        		LIGNES_BUDGETAIRES_TableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-LIGNE_BUDGETAIRE.cpp",
									 74,
									 lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->text());

        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer ! ").
                          arg(lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->text()));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-LIGNE BUDGÉTAIRE"),
                                       retMsg);
            return false;
        }

        retMsg.append(QObject::tr("%1' a été créer avec succès ! ").
                      arg(lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->text()));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-LIGNE BUDGÉTAIRE"),
                                       retMsg);

        clear_LIGNE_BUDGETAIRE_all_fields();
        rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);

        return true;
    }
    else
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-LIGNE BUDGÉTAIRE"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}


bool YerothAdminCreateWindow::creer_ligne_budgetaire_CHECK_fields()
{
	bool intitule_de_la_ligne_budgetaire = lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->checkField();

    bool MONTANT_de_la_ligne_budgetaire = lineEdit_creer_montant_de_la_LIGNE_BUDGETAIRE->checkField();

    bool INTITULE_DU_COMPTE_BANCAIRE = comboBox_INTITULE_DU_COMPTE_BANCAIRE->checkField();

    return intitule_de_la_ligne_budgetaire &&
    	   MONTANT_de_la_ligne_budgetaire  &&
		   INTITULE_DU_COMPTE_BANCAIRE;
}


void YerothAdminCreateWindow::clear_LIGNE_BUDGETAIRE_all_fields()
{
	lineEdit_creer_INTITULE_de_la_LIGNE_BUDGETAIRE->clearField();
	lineEdit_creer_montant_de_la_LIGNE_BUDGETAIRE->clearField();
	lineEdit_creer_montant_RESTANT_de_la_LIGNE_BUDGETAIRE->clearField();
	comboBox_INTITULE_DU_COMPTE_BANCAIRE->clearField();
	textEdit_creer_une_LIGNE_budgetaire->clear();
}
