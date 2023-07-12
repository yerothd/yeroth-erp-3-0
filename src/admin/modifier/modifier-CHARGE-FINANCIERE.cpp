

void YerothAdminModifierWindow::setupEdit_CHARGE_FINANCIERE()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *CHARGES_FINANCIERES_TableModel = lw->getCurSearchSqlTableModel();

    if (0 == CHARGES_FINANCIERES_TableModel)
    {
    	CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }
    else if (false == YerothUtils::isEqualCaseInsensitive(CHARGES_FINANCIERES_TableModel->sqlTableName(),
                                                 	 	  YerothDatabase::CHARGES_FINANCIERES))
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_charges_financieres();
    }



    QSqlRecord record = CHARGES_FINANCIERES_TableModel->record(lw->lastSelectedItemForModification());


    dateEdit_date_de_reception
		->setDate(record.value(YerothDatabaseTableColumn::DATE_DE_RECEPTION).toDate());

    dateEdit_date_de_commande
		->setDate(record.value(YerothDatabaseTableColumn::DATE_DE_COMMANDE).toDate());


    lineEdit_departement
		->setText(GET_SQL_RECORD_DATA(record,
                                  	  YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

    lineEdit_reference_produit
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::REFERENCE));

    lineEdit_designation
		->setText(GET_SQL_RECORD_DATA(record,
                                   	  YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_LIGNE_BUDGETAIRE
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::CATEGORIE));

    lineEdit_nom_entreprise_fournisseur
		->setText(GET_SQL_RECORD_DATA(record,
                                      YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));


    double prix_unitaire = GET_SQL_RECORD_DATA(record,
                                               YerothDatabaseTableColumn::PRIX_UNITAIRE).toDouble();


    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));


    double prix_dachat = 0.0;


    YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
        if (currentUser->isManager())
        {
            prix_dachat =
            		GET_SQL_RECORD_DATA(record,
                                        YerothDatabaseTableColumn::PRIX_DACHAT).toDouble();
        }
    }


    lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM(prix_dachat));


    double quantite_restante =
    		GET_SQL_RECORD_DATA(record,
                                YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();


    lineEdit_quantite->setText(GET_DOUBLE_STRING_P(quantite_restante, 0));



    lineEdit_STATUT_DE_LACHAT->setText(GET_SQL_RECORD_DATA(record,
            YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR));

	lineEdit_MONTANT_TVA->setText(GET_SQL_RECORD_DATA(record,
            YerothDatabaseTableColumn::MONTANT_TVA));

	lineEdit_ref_RECU_DACHAT->setText(GET_SQL_RECORD_DATA(record,
            YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));

	lineEdit_LOCALISATION->setText(GET_SQL_RECORD_DATA(record,
            YerothDatabaseTableColumn::LOCALISATION));

	lineEdit_ID_commandeur->setText(GET_SQL_RECORD_DATA(record,
            YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT));


    doubleSpinBox_creer_charge_financiere_CMD_REPETITION->checkField();
}


void YerothAdminModifierWindow::modifier_CHARGE_FINANCIERE()
{
    if (modifier_CHARGE_FINANCIERE_check_fields())
    {
        YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
        modifier_CHARGE_FINANCIERE_main();
        YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
    }
}


void YerothAdminModifierWindow::modifier_CHARGE_FINANCIERE_main()
{
    YerothAdminListerWindow *lw = _allWindows->_adminListerWindow;

    YerothSqlTableModel *CHARGES_FINANCIERES_TableModel = lw->getCurSearchSqlTableModel();

    if (0 == CHARGES_FINANCIERES_TableModel)
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }
    else if (!YerothUtils::isEqualCaseInsensitive
             (CHARGES_FINANCIERES_TableModel->sqlTableName(), YerothDatabase::CHARGES_FINANCIERES))
    {
        CHARGES_FINANCIERES_TableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
    }


    QSqlRecord record =
    		CHARGES_FINANCIERES_TableModel->record(lw->lastSelectedItemForModification());


    bool success = false;


    QString retMsg =
    		QObject::tr("Les données de la CHARGE FINANCIÈRE '%1'")
    			.arg(GET_SQL_RECORD_DATA(record,
    				 YerothDatabaseTableColumn::DESIGNATION));

    if (success)
    {
        retMsg.append(QObject::tr(" ont été modifiées avec succès !"));

        YerothQMessageBox::information(this,
                                       QObject::tr("Yeroth-pgi-3.0 ~ admin-modifier-charge financière"),
                                       retMsg);

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_ligne_budgetaire);
        rendreInvisible();
    }
    else
    {
        qDebug() << "\t[reason for failing] " <<
                 CHARGES_FINANCIERES_TableModel->lastError();

        retMsg.append(QObject::tr(" n'ont pas pu être modifiées !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("Yeroth-pgi-3.0 ~ admin-modifier-charge financière"),
                                   retMsg);
    }
}


bool YerothAdminModifierWindow::modifier_CHARGE_FINANCIERE_check_fields()
{
    bool commande_repetition = doubleSpinBox_creer_charge_financiere_CMD_REPETITION->checkField();

    return commande_repetition;
}


void YerothAdminModifierWindow::clear_CHARGE_FINANCIERE_all_fields()
{
    dateEdit_date_de_reception->clear();
    dateEdit_date_de_commande->clear();

    lineEdit_departement->clear();
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_LIGNE_BUDGETAIRE->clear();
    lineEdit_quantite->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();

    lineEdit_STATUT_DE_LACHAT->clear();
	lineEdit_MONTANT_TVA->clear();
	lineEdit_ref_RECU_DACHAT->clear();
	lineEdit_LOCALISATION->clear();
	lineEdit_ID_commandeur->clear();

    textEdit_une_CHARGE_FINANCIERE->clear();
}


