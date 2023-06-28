
bool YerothAdminCreateWindow::creer_compte_bancaire()
{
    if (creer_compte_bancaire_check_fields())
    {
        QString
        retMsg(QObject::tr
               ("Le compte bancaire avec pour INTITULÉ DE COMPTE BANCAIRE '"));

        YerothSqlTableModel &compteBancaireSqlTableModel =
                        _allWindows->getSqlTableModel_comptes_bancaires();

        compteBancaireSqlTableModel
			.yerothSetFilter_WITH_where_clause(QString("LOWER(%1) = LOWER('%2')")
												.arg(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
													 lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->text()));

        int compteBancaireTableModelRowCount =
                        compteBancaireSqlTableModel.easySelect();

        if (compteBancaireTableModelRowCount > 0)
        {
            QSqlRecord record = compteBancaireSqlTableModel.record(0);
            QString
            duplicateCompteBancaire(GET_SQL_RECORD_DATA
                                    (record,
                                     YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE));

            retMsg.append(QObject::tr("%1' existe déjà ! ").arg
                          (duplicateCompteBancaire));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ administration ~ créer ~ compte bancaire"),
                                       retMsg);

            compteBancaireSqlTableModel.resetFilter("src/admin/creer/creer-compte-bancaire.cpp");

            return false;
        }

        compteBancaireSqlTableModel.resetFilter("src/admin/creer/creer-compte-bancaire.cpp");

        QSqlRecord record = compteBancaireSqlTableModel.record();

        record.setValue(YerothDatabaseTableColumn::ID,
                        YerothERPWindows::getNextIdSqlTableModel_comptes_bancaires());

        record.setValue(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                 	    lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->text());

        record.setValue(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
                        lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->text());

        record.setValue(YerothDatabaseTableColumn::INSTITUT_BANCAIRE,
                        lineEdit_creer_compte_bancaire_institut_bancaire->text());

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE,
                 	 	textEdit_creer_compte_bancaire_description_du_compte->toPlainText());

        bool success =
        		compteBancaireSqlTableModel
					.insertNewRecord(record,
									 _allWindows->_adminCreateWindow,
									 "src/admin/creer/creer-compte-bancaire.cpp",
									 61,
									 lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->text());

        if (!success)
        {
            retMsg.append(QObject::tr("%1' n'a pas pu être créer ! ").
                          arg
                          (lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->
                           text()));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-compte bancaire"),
                                       retMsg);
            return false;
        }

        retMsg.append(QObject::tr("%1' a été créer avec succès ! ").
                      arg
                      (lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->
                       text()));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-compte bancaire"),
                                       retMsg);

        clear_compte_bancaire_all_fields();

        rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_COMPTE_BANCAIRE);

        return true;
    }
    else
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer-compte bancaire"),
                                       QObject::tr
                                       ("BIEN VOULOIR REMPLIR TOUS LES "
                                        "CHAMPS DE TEXTES OBLIGATOIRES !"));

        return false;
    }

    return false;
}

bool YerothAdminCreateWindow::creer_compte_bancaire_check_fields()
{
    bool reference_du_compte_bancaire =
                    lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->checkField
                    ();
    bool intitule_du_compte_bancaire =
                    lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->checkField();
    bool institut_bancaire =
                    lineEdit_creer_compte_bancaire_institut_bancaire->checkField();

    return reference_du_compte_bancaire &&
           intitule_du_compte_bancaire && institut_bancaire;
}

void YerothAdminCreateWindow::clear_compte_bancaire_all_fields()
{
    lineEdit_creer_compte_bancaire_reference_du_compte_bancaire->clearField();
    lineEdit_creer_compte_bancaire_intitule_du_compte_bancaire->clearField();
    lineEdit_creer_compte_bancaire_institut_bancaire->clearField();
    textEdit_creer_compte_bancaire_description_du_compte->clear();
}
