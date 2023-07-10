
void YerothAdminCreateWindow::populate_CHARGE_FINANCIERE_ComboBoxes()
{
    _logger->log("populate_CHARGE_FINANCIERE_ComboBoxes");

    comboBox_creer_nom_departement
		->populateComboBoxRawString(YerothDatabase::DEPARTEMENTS_PRODUITS,
									YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT);

    comboBox_creer_LIGNE_BUDGETAIRE
		->populateComboBoxRawString(YerothDatabase::LIGNES_BUDGETAIRES,
									YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire);

    comboBox_creer_fournisseur
		->populateComboBoxRawString(YerothDatabase::FOURNISSEURS,
									YerothDatabaseTableColumn::NOM_ENTREPRISE);
}


bool YerothAdminCreateWindow::creer_charge_financiere()
{
	bool result = false;

    if (creer_charge_financiere_CHECK_fields())
    {
        QString proposed_nom_departement_produit =
        		comboBox_creer_nom_departement->currentText();

        if (!proposed_nom_departement_produit.isEmpty())
        {
            if (!YerothUtils::creerNouveauDepartementDeProduits(proposed_nom_departement_produit, this))
            {
            	return false;
            }
        }


        YerothERPServiceData a_service_achat_au_fournisseur_data;

        a_service_achat_au_fournisseur_data._nom_departement_produit =
        				proposed_nom_departement_produit;

        a_service_achat_au_fournisseur_data._categorie =
                        comboBox_creer_LIGNE_BUDGETAIRE->currentText();

        a_service_achat_au_fournisseur_data._description =
                        textEdit_creer_une_CHARGE_FINANCIERE->toPlainText();

        a_service_achat_au_fournisseur_data._designation =
        				lineEdit_creer_designation->text();

        a_service_achat_au_fournisseur_data._reference =
                        lineEdit_creer_reference->text();


        QString proposed_Categorie_Name =
        		comboBox_creer_LIGNE_BUDGETAIRE->currentText();


        QString proposed_Fournisseur_Client_Name =
        		comboBox_creer_fournisseur->currentText();

        if (!proposed_Fournisseur_Client_Name.isEmpty())
        {
            if (!YerothUtils::creerNouveauFournisseur
                    (proposed_Fournisseur_Client_Name, this))
            {
                return false;
            }
        }


        double quantite_totale = doubleSpinBox_quantite->value();

        double prix_unitaire = -1 * lineEdit_creer_prix_dachat_alunite->text().toDouble();

        double prix_dachat_du_service = quantite_totale * prix_unitaire;

        double prix_dachat_du_service_POSITIF = -1.0 * prix_dachat_du_service;


        QString utilisateurCourantNomComplet;

        YerothPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            utilisateurCourantNomComplet.append(aUser->nom_complet());
        }


        bool successAchatDeServiceInsert = false;


    	YerothSqlTableModel &LIGNES_BUDGETAIRES_SqlTableModel =
    			_allWindows->getSqlTableModel_lignes_budgetaires();

    	QSqlRecord LIGNES_BUDGETAIRES_query_RECORD;

    	double montant_de_la_ligne_budgetaire_RESTANTE = 0.0;

        {
        	/*
        	 * VÉRIFIONS SI CETTE LIGNE BUGÉTAIRE DISPOSE D'ASSEZ
        	 * D'ARGENT pour cet achat !
        	 */

            QString yeroth_LIGNES_BUDGETAIRES_SqlTableModelFilter =
            		QString("%1 = '%2'")
    					.arg(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
    						 proposed_Categorie_Name);

        	LIGNES_BUDGETAIRES_SqlTableModel
    			.yerothSetFilter_WITH_where_clause
    				(yeroth_LIGNES_BUDGETAIRES_SqlTableModelFilter);


        	QString retMsg_ECHEC_inserer_charge_financiere;

        	int rows = LIGNES_BUDGETAIRES_SqlTableModel
        					.easySelect("src/admin/creer/creer-CHARGE_FINANCIERE.cpp", 115);

        	if (rows <= 0)
        	{
        		retMsg_ECHEC_inserer_charge_financiere =
        				QObject::tr("LA LIGNE BUDGÉTAIRE '%1' est inexistante !")
    						.arg(proposed_Categorie_Name);

                YerothQMessageBox::warning(this,
                                           QObject::tr("enregistrement ACHAT SUR LIGNE BUDGÉTAIRE (1) - échec"),
    									   retMsg_ECHEC_inserer_charge_financiere);

                LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/admin/creer/creer-CHARGE_FINANCIERE.cpp");

                return false;
        	}


        	LIGNES_BUDGETAIRES_query_RECORD = LIGNES_BUDGETAIRES_SqlTableModel.record(0);

        	montant_de_la_ligne_budgetaire_RESTANTE =
        			GET_SQL_RECORD_DATA(LIGNES_BUDGETAIRES_query_RECORD,
        								YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire).toDouble();

        	if (montant_de_la_ligne_budgetaire_RESTANTE < prix_dachat_du_service_POSITIF)
        	{
        		retMsg_ECHEC_inserer_charge_financiere =
        				QObject::tr("LA LIGNE BUDGÉTAIRE '%1' DÉTIENT UN \n"
        								"MONTANT INSUFFISANT: ('%2') (requis pour \n"
        								"la charge financière !)!")
    						.arg(proposed_Categorie_Name,
    							 QString::number(montant_de_la_ligne_budgetaire_RESTANTE));

                YerothQMessageBox::warning(this,
                                           QObject::tr("enregistrement ACHAT SUR LIGNE BUDGÉTAIRE (2) - échec"),
    									   retMsg_ECHEC_inserer_charge_financiere);

                LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/admin/creer/creer-CHARGE_FINANCIERE.cpp",
                											 152);

                return false;
        	}
        }

        {
            YerothSqlTableModel &charges_financieresSqlTableModel =
                            _allWindows->getSqlTableModel_charges_financieres();

            QSqlRecord CHARGE_FINANCIERE_Record = charges_financieresSqlTableModel.record();

            int achat_de_service_id_to_save =
                            YerothERPWindows::getNextIdSqlTableModel_charges_financieres();

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::ID,
                                 achat_de_service_id_to_save);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::REFERENCE,
                                 lineEdit_creer_reference->text());

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                                 lineEdit_creer_designation->text());

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                 a_service_achat_au_fournisseur_data._nom_departement_produit);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::CATEGORIE,
                                 a_service_achat_au_fournisseur_data._categorie);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                                 utilisateurCourantNomComplet);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                                 quantite_totale);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                                 _MONTANT_TVA_CHARGE_FINANCIERE);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                                 prix_unitaire);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT,
                                 prix_dachat_du_service);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::DESCRIPTION_charge_financiere,
            		textEdit_creer_une_CHARGE_FINANCIERE->toPlainText());

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
            					 proposed_Fournisseur_Client_Name);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                                 YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

            CHARGE_FINANCIERE_Record.setValue(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
                                 GET_CURRENT_DATE);

            if (!YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
            								   	   	   	     charges_financieresSqlTableModel,
															 *lineEdit_creer_designation,
															 QObject::tr("Désignation"),
															 YerothDatabaseTableColumn::DESIGNATION)  &&
            	!YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
            		            						     charges_financieresSqlTableModel,
            												 *lineEdit_creer_reference,
            												 QObject::tr("Référence"),
            												 YerothDatabaseTableColumn::REFERENCE)	)
            {
                successAchatDeServiceInsert =
                		charges_financieresSqlTableModel
        					.insertNewRecord(CHARGE_FINANCIERE_Record,
        									 this,
        									 "src/admin/creer/creer-CHARGE_FINANCIERE.cpp",
        									 221,
        									 lineEdit_creer_designation->text());
            }
//            else
//            {
//        		QString retMsg_ECHEC_inserer_charge_financiere =
//        				QObject::tr("1 charge financière avec la \n"
//        							"désignation '%1' est déjà existante !")
//    						.arg(lineEdit_creer_designation->text());
//
//                YerothQMessageBox::warning(this,
//                                           QObject::tr("enregistrement ACHAT SUR LIGNE BUDGÉTAIRE (3) - échec"),
//    									   retMsg_ECHEC_inserer_charge_financiere);
//            }

        }

        bool successPaiementsInsert = false;

        {
            YerothERPPaymentProcessingInformation payment_processing_info;

            payment_processing_info._nom_entreprise =
                            proposed_Fournisseur_Client_Name;

            payment_processing_info._type_de_paiement =
                            YerothUtils::DECAISSEMENT_COMPTANT;

            payment_processing_info._montant_paye = prix_dachat_du_service;

            payment_processing_info._designation =
            		a_service_achat_au_fournisseur_data._designation;

            payment_processing_info._paiement_intitule_compte_bancaire =
            		GET_SQL_RECORD_DATA(LIGNES_BUDGETAIRES_query_RECORD,
            							YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE);

            payment_processing_info._reference =
            		a_service_achat_au_fournisseur_data._reference;

            bool is_supplier_payment = true;

            successPaiementsInsert =
                            payment_processing_info.save_payment_info_record(is_supplier_payment);
        }


        if (!successAchatDeServiceInsert ||
        	!successPaiementsInsert)
        {
        	LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/admin/creer/creer-CHARGE_FINANCIERE.cpp");

            return false;
        }


        /*
         * UPDATING TABLE lignes_budgetaires WITH A NEW REMAINING
         * AMOUNT for the current modified budget line !
         */
        double NOUVEAU_MONTANT_montant_de_la_ligne_budgetaire_RESTANTE =
        		montant_de_la_ligne_budgetaire_RESTANTE - prix_dachat_du_service_POSITIF;

        LIGNES_BUDGETAIRES_query_RECORD
    		.setValue(YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
    				  NOUVEAU_MONTANT_montant_de_la_ligne_budgetaire_RESTANTE);

        LIGNES_BUDGETAIRES_SqlTableModel.updateRecord(0,
        											  LIGNES_BUDGETAIRES_query_RECORD,
    												  "src/admin/creer/creer-CHARGE_FINANCIERE.cpp",
    												  259);


        QString achatRetMsg(QObject::tr("La charge financière '%1' au fournisseur '%2'")
        						.arg(lineEdit_creer_designation->text(),
        							 proposed_Fournisseur_Client_Name));

        if (successAchatDeServiceInsert)
        {
            achatRetMsg.append(QObject::tr(" a été enregistrée dans la base de données !"));

            YerothQMessageBox::information(this,
                                           QObject::tr("ajout d'une charge financière - succès"),
										   achatRetMsg);
        }
        else
        {
            achatRetMsg.append(QObject::tr(" n'a pas pu être enregistrée dans la base de données !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr("ajout d'1 charge financière - échec"),
                                       achatRetMsg);
        }


        QString paiementsRetMsg(QObject::tr("La charge financière '%1' au fournisseur '%2'")
        							.arg(lineEdit_creer_designation->text(),
        								 proposed_Fournisseur_Client_Name));

        if (successPaiementsInsert)
        {
            paiementsRetMsg.append(QObject::tr(" a été répertoriée dans le tableau des paiements !"));

            YerothQMessageBox::information(this,
                                           QObject::tr("ajout d'une charge financière - répertorié - succès"),
                                           paiementsRetMsg);
        }
        else
        {
            paiementsRetMsg.append
				(QObject::tr(" n'a pas pu être répertoriée dans le tableau des paiements !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr("ajout d'une charge financière - répertorié - échec"),
                                       paiementsRetMsg);
        }

        LIGNES_BUDGETAIRES_SqlTableModel.resetFilter("src/admin/creer/creer-CHARGE_FINANCIERE.cpp", 309);


        rendreInvisible();

        _allWindows->_adminListerWindow->rendreVisible(SUJET_ACTION_CHARGE_FINANCIERE);


        return true;
    }
    else
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("Yeroth-pgi-3.0 ~ admin-créer-LIGNE BUDGÉTAIRE"),
                                       QObject::tr("BIEN VOULOIR REMPLIR TOUS LES "
                                    		   	   "CHAMPS DE TEXTES OBLIGATOIRES !"));

        result = false;
    }

    return result;
}


bool YerothAdminCreateWindow::creer_charge_financiere_CHECK_fields()
{
	bool departement = comboBox_creer_nom_departement->checkField();

	bool designation = lineEdit_creer_designation->checkField();

	bool reference = lineEdit_creer_reference->checkField();

	bool fournisseur = comboBox_creer_fournisseur->checkField();

	bool quantite = doubleSpinBox_quantite->checkField();

	bool intitule_de_la_LIGNE_BUDGETAIRE = comboBox_creer_LIGNE_BUDGETAIRE->checkField();

    bool prix_dachat_alunite = lineEdit_creer_prix_dachat_alunite->checkField();

    bool DESCRIPTION_DE_LA_CHARGE_FINANCIERE = textEdit_creer_une_CHARGE_FINANCIERE->checkField();


    return departement							&&
    	   designation 							&&
		   reference							&&
		   quantite 							&&
		   fournisseur							&&
		   intitule_de_la_LIGNE_BUDGETAIRE 		&&
    	   prix_dachat_alunite  				&&
		   DESCRIPTION_DE_LA_CHARGE_FINANCIERE;
}


void YerothAdminCreateWindow::clear_CHARGE_FINANCIERE_all_fields()
{
	textEdit_creer_une_CHARGE_FINANCIERE->clear();

	comboBox_creer_nom_departement->clearField();
	comboBox_creer_LIGNE_BUDGETAIRE->clearField();
	lineEdit_creer_reference->clearField();
	lineEdit_creer_designation->clearField();
	comboBox_creer_fournisseur->clearField();
	lineEdit_creer_prix_dachat_alunite->clearField();
	lineEdit_creer_prix_dachat_total->clearField();
	doubleSpinBox_creer_charge_financiere_CMD_REPETITION->setValue(1.0);
}




