/*
 * yeroth-erp-modifier-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */


#include "yeroth-erp-modifier-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"


#include <unistd.h>


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtGui/QContextMenuEvent>


YerothModifierWindow::YerothModifierWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothModifierWindow")),
 _montantTva(0.0),
 _IS_AN_ASSET(false),
 _tvaCheckBoxPreviousState(false),
 _tvaPercent(YerothUtils::getTvaStringWithPercent())
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("modifier (ré-approvisionner) un(e) stock / immobilisation");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    textEdit_description->setYerothEnabled(false);


    setupLineEdits();

    setupLineEditsQCompleters();

    setupDateTimeEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);



    connect(checkBox_HISTORIQUE,
            SIGNAL(stateChanged(int)),
            this, SLOT(handle_HISTORIQUE_CHECKBOX(int)));


    connect(checkBox_achat, SIGNAL(stateChanged(int)), this,
            SLOT(handle_achat_checkBox(int)));

    connect(checkBox_re_approvisionnement, SIGNAL(stateChanged(int)),
            this, SLOT(handle_re_approvisionnement_checkBox(int)));

    connect(checkBox_tva, SIGNAL(clicked(bool)), this,
            SLOT(handleTVACheckBox(bool)));

    connect(lineEdit_prix_dachat,
            SIGNAL(textChanged(const QString &)),
            this,
            SLOT(calculate_and_display_ALL_benefit_buying_price_percentage
                 ()));

    connect(lineEdit_prix_vente, SIGNAL(textChanged(const QString &)), this,
            SLOT(edited_prix_vente(const QString &)));

    connect(lineEdit_prix_vente, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente()));

    connect(lineEdit_prix_vente_EN_GROS,
            SIGNAL(textChanged(const QString &)), this,
            SLOT(edited_prix_vente_EN_GROS(const QString &)));

    connect(lineEdit_prix_vente_EN_GROS, SIGNAL(editingFinished()), this,
            SLOT(display_prix_vente_EN_GROS()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionActualiser, SIGNAL(triggered()), this,
            SLOT(actualiser_stock()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionSupprimer, SIGNAL(triggered()), this,
            SLOT(supprimer_ce_stock()));
    connect(actionSupprimerImage, SIGNAL(triggered()), this,
            SLOT(supprimer_image_stock()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(alertes()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
#endif

    setupShortcuts();
}

void YerothModifierWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothModifierWindow::handle_HISTORIQUE_CHECKBOX(int aState)
{
    if (checkBox_HISTORIQUE->isChecked())
    {
        checkBox_HISTORIQUE->setText(QObject::tr("historique actif"));
    }
    else
    {
        checkBox_HISTORIQUE->
        setText(QObject::tr("historique désactiver"));
    }
}


void YerothModifierWindow::handle_achat_checkBox(int aState)
{
    checkBox_achat->setReadOnly(!checkBox_re_approvisionnement->isChecked());

    if (checkBox_achat->isChecked())
    {
        if (checkBox_re_approvisionnement->isChecked())
        {
            lineEdit_prix_dachat->setYerothEnabled(true);
        }
        else
        {
            lineEdit_prix_dachat->setYerothEnabled(false);
        }
    }

    handleVisibilityPrixDachat(checkBox_achat->isChecked());

    calculate_and_display_benefit_buying_price_percentage();
}


void YerothModifierWindow::handle_re_approvisionnement_checkBox(int state)
{
    checkBox_achat->setReadOnly(!checkBox_re_approvisionnement->isChecked());

    if (checkBox_re_approvisionnement->isChecked())
    {
        checkBox_achat->setVisible(true);


        lineEdit_prix_dachat->setYerothEnabled(checkBox_achat->isChecked());

        handleVisibilityPrixDachat(checkBox_achat->isChecked());


        lineEdit_quantite_restante->setYerothEnabled(true);
        lineEdit_reference_recu_dachat->setYerothEnabled(true);
        lineEdit_nom_entreprise_fournisseur->setYerothEnabled(true);
    }
    else
    {
        int achatQuerySize =
                        YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(_currentStockID);

        if (achatQuerySize > 0)
        {
            checkBox_achat->setVisible(true);
            checkBox_achat->setChecked(true);
        }
        else
        {
            checkBox_achat->setVisible(false);
            checkBox_achat->setChecked(false);
        }

        handleVisibilityPrixDachat(checkBox_achat->isChecked());

        lineEdit_prix_dachat->setText(_currentPrixDachat);

        lineEdit_prix_dachat->setYerothEnabled(false);

        lineEdit_quantite_restante->setYerothEnabled(false);
        lineEdit_reference_recu_dachat->setYerothEnabled(false);
        lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
    }
}


void YerothModifierWindow::actualiser_stock()
{
    if (!check_fields())
    {
        return;
    }

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_EN_GROS =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (!_IS_AN_ASSET
            && !YerothUtils::isProfitable(prix_vente, prix_dachat, _montantTva))
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr("pas profitable"),
                                   QObject::tr
                                   ("Le prix de vente doit être "
                                    "supérieure ou égal au prix d'achat !"));

        return;
    }


    bool correctDatePeremption = true;

    if (dateEdit_date_peremption->date() <= GET_CURRENT_DATE)
    {
        QString warnMsg(QObject::tr
                        ("La date de péremption n'est pas postdatée !\n\n"
                         "Continuer avec l'actualisation des données d'1 immobilisation OU stock ?"));

        if (QMessageBox::Cancel ==
                YerothQMessageBox::question(this,
                                            QObject::tr
                                            ("actualiser les détails d'1 immobilisation OU stock"),
                                            warnMsg, QMessageBox::Cancel,
                                            QMessageBox::Ok))
        {
            correctDatePeremption = false;
        }
    }

    if (!correctDatePeremption)
    {
        return;
    }


    bool CONTINUE_AVEC_PRIX_DACHAT_ZERO = true;

    if (prix_dachat <= 0)
    {
        QString warnMsg(QObject::tr
                        ("Le prix d'achat est égal à zéro (0) %1 !\n\n"
                         "Continuer avec l'actualisation des données d'1 immobilisation OU stock ?").
                        arg(YerothERPConfig::currency));

        if (QMessageBox::Cancel ==
                YerothQMessageBox::question(this,
                                            QObject::tr
                                            ("actualiser les détails d'1 immobilisation OU stock"),
                                            warnMsg, QMessageBox::Cancel,
                                            QMessageBox::Ok))
        {
            CONTINUE_AVEC_PRIX_DACHAT_ZERO = false;
        }
    }

    if (!CONTINUE_AVEC_PRIX_DACHAT_ZERO)
    {
        return;
    }


    QString msgEnregistrer(QObject::tr("Poursuivre avec la "
                                           "modification (le ré-approvisionnement) "
                                           "d'1 immobilisation OU stock '%1' ?").
                           arg(lineEdit_designation->text()));

    if (QMessageBox::Ok !=
            YerothQMessageBox::question(this,
                                        QObject::tr("modification"),
                                        msgEnregistrer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        msgEnregistrer.clear();

        msgEnregistrer.append(QObject::tr
                              ("Vous avez annulé la modification "
                               "des détails d'1 immobilisation OU stock '%1' !").
                              arg(lineEdit_designation->text()));

        YerothQMessageBox::information(this, QObject::tr("annulation"),
                                       msgEnregistrer, QMessageBox::Ok);

        return;
    }

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    description_produit(GET_SQL_RECORD_DATA
                        (record,
                         YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));

    if (!YerothUtils::isEqualCaseInsensitive(description_produit,
                                             textEdit_description->toPlainText
                                             ()))
    {
        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
                        textEdit_description->toPlainText());
    }

    if (checkBox_HISTORIQUE->isChecked())
    {
        record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF, 1);
    }
    else
    {
        record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF, 0);
    }

    record.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                    lineEdit_localisation_produit->text());

    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA, _montantTva);

    record.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                    _montantTva_EN_GROS);


    double prix_unitaire_ht = prix_vente - _montantTva;

    double prix_unitaire_en_gros_ht = prix_vente_EN_GROS - _montantTva_EN_GROS;


    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                    prix_unitaire_ht);

    record.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                    prix_unitaire_en_gros_ht);

    record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT, prix_dachat);


    if (!_IS_AN_ASSET)
    {
        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE, prix_vente);

        record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                        prix_vente_EN_GROS);
    }

    record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                    lineEdit_nom_entreprise_fournisseur->text());

    record.setValue(YerothDatabaseTableColumn::STOCK_DALERTE,
                    lineEdit_stock_dalerte->text().toDouble());

    record.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                    dateEdit_date_peremption->date());

    if (label_image_produit->pixmap())
    {
        QByteArray bytes;
        YerothUtils::savePixmapToByteArray(bytes,
                                           *label_image_produit->pixmap(),
                                           "JPG");
        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT, bytes);
    }

    double quantite_en_re_approvisionement = 0;

    if (checkBox_re_approvisionnement->isChecked())
    {
        double ancienne_quantite_totale = GET_SQL_RECORD_DATA(record,
                                                              YerothDatabaseTableColumn::QUANTITE_TOTALE).
                                          toDouble();

        double quantite_totale =
                        lineEdit_quantite_restante->text().toDouble();

        quantite_en_re_approvisionement =
                        quantite_totale - ancienne_quantite_totale;

        if (quantite_en_re_approvisionement > 0)
        {
            record.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                            quantite_totale);

            record.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
                            lineEdit_reference_recu_dachat->text());

            bool historique_stock_actif = GET_SQL_RECORD_DATA(record,
                                                              YerothDatabaseTableColumn::
                                                              HISTORIQUE_STOCK_ACTIF).toInt
                                          ();

            QString historiqueStock = GET_SQL_RECORD_DATA(record,
                                                          YerothDatabaseTableColumn::HISTORIQUE_STOCK);

            QString historiqueStockReApprovisionement;

            if (historique_stock_actif)
            {
                historiqueStockReApprovisionement =
                                YerothHistoriqueStock::creer_mouvement_stock
                                (MOUVEMENT_DE_STOCK_RE_APPROVISIONNEMENT, _currentStockID,
                                 GET_CURRENT_DATE, ancienne_quantite_totale,
                                 quantite_en_re_approvisionement, quantite_totale);
            }

            historiqueStock.append(YerothHistoriqueStock::SEPARATION_EXTERNE).
            append(historiqueStockReApprovisionement);

            record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
                            historique_stock_actif);

            record.setValue(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
                            historiqueStock);
        }
        else
        {
            QString retMsg(QObject::tr
                           ("En mode ré-approvisionnement, la quantité "
                            "restante doit être supérieure à "
                            "l'ancienne quantité restante !"));

            YerothQMessageBox::warning(this, QObject::tr("échec"),
                                       retMsg);

            lineEdit_quantite_restante->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::
                                                 QUANTITE_TOTALE));

            return;
        }
    }

    bool success =
                    _allWindows->
                    _stocksWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    retMsg(QObject::tr
           ("Les détails d'1 immobilisation OU stock '%1").arg
           (lineEdit_designation->text()));

    if (success)
    {
        YerothERPStockMarchandiseData a_stock_data;

        if (!_IS_AN_ASSET)
        {
            a_stock_data._designation = lineEdit_designation->text();
            a_stock_data._prix_dachat_precedent = lineEdit_prix_dachat->text();
            a_stock_data._prix_vente_precedent = lineEdit_prix_vente->text();
            a_stock_data._prix_vente_en_gros_precedent =
                            lineEdit_prix_vente_EN_GROS->text();

            YerothUtils::UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList
            (a_stock_data, this);
        }

        if (checkBox_achat->isChecked())
        {
            if (!_IS_AN_ASSET)
            {
                YerothUtils::UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList
                (a_stock_data, this);
            }

            if (checkBox_re_approvisionnement->isChecked() &&
                    quantite_en_re_approvisionement > 0)
            {
                ajouter_nouveau_re_approvisionnement_achat
                (quantite_en_re_approvisionement, _currentStockID);
            }
        }

        update_achat_deja_existant(record,
                                   prix_unitaire_ht, prix_unitaire_en_gros_ht);


        retMsg.
        append(QObject::tr("' ont été actualisés avec succès !"));

        YerothQMessageBox::information(this, QObject::tr("succès"),
                                       retMsg);
    }
    else
    {
        retMsg.append(QObject::tr
                      ("' n'ont pas pu être actualisés avec succès !"));

        YerothQMessageBox::warning(this, QObject::tr("échec"), retMsg);
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    _allWindows->_stocksWindow->rendreVisible(_curStocksTableModel);

    rendreInvisible();
}


void YerothModifierWindow::edited_prix_vente(const QString &newPrixVente)
{
    _lastEditedPrixVente = newPrixVente;

    calculate_and_display_benefit_buying_price_percentage();
}


void YerothModifierWindow::edited_prix_vente_EN_GROS(const QString &
                                                     newPrixVente_en_gros)
{
    _lastEditedPrixVente_EN_GROS = newPrixVente_en_gros;

    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void
YerothModifierWindow::
calculate_and_display_ALL_benefit_buying_price_percentage()
{
    calculate_and_display_benefit_buying_price_percentage();
    calculate_and_display_benefit_buying_price_percentage_EN_GROS();
}


void YerothModifierWindow::calculate_and_display_benefit_buying_price_percentage()
{
    if (checkBox_achat->isChecked())
    {
        double prix_dachat = lineEdit_prix_dachat->text().toDouble();
        double prix_vente = lineEdit_prix_vente->text().toDouble();
        //double montantTva = 0.0;

        if (YerothUtils::isProfitable(prix_vente, prix_dachat, _montantTva))
        {
            double pourcentage_benefice_prix_dachat =
                            POURCENTAGE_YEROTH_GET_VALUE(YerothUtils::getMargeBeneficiaire
                                                         (prix_vente,
                                                          prix_dachat,
                                                          _montantTva),
                                                         prix_dachat);

            lineEdit_pourcentage_prix_dachat_prix_de_vente->setText(QString
                                                                    ("%1%").arg
                                                                    (QString::
                                                                     number
                                                                     (pourcentage_benefice_prix_dachat,
                                                                      'f',
                                                                      2)));
        }
        else
        {
            lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");
        }
    }
    else
    {
        lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");
    }
}


void
YerothModifierWindow::
calculate_and_display_benefit_buying_price_percentage_EN_GROS()
{
    if (checkBox_achat->isChecked())
    {
        double
        prix_dachat = lineEdit_prix_dachat->text().toDouble();

        double
        prix_vente_EN_GROS = lineEdit_prix_vente_EN_GROS->text().toDouble();

        if (YerothUtils::isProfitable
                (prix_vente_EN_GROS, prix_dachat, _montantTva_EN_GROS))
        {
            double
            pourcentage_benefice_prix_dachat_EN_GROS =
                            POURCENTAGE_YEROTH_GET_VALUE(YerothUtils::getMargeBeneficiaire
                                                         (prix_vente_EN_GROS,
                                                          prix_dachat,
                                                          _montantTva_EN_GROS),
                                                         prix_dachat);

            lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
            (QString("%1%").arg
             (QString::number
              (pourcentage_benefice_prix_dachat_EN_GROS, 'f', 2)));
        }
        else
        {
            lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
            ("0.00 %");
        }
    }
    else
    {
        lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText
        ("0.00 %");
    }
}


void YerothModifierWindow::handleVisibilityPrixDachat(bool anEditable)
{
    label_prix_dachat->setVisible(anEditable);

    lineEdit_prix_dachat->setVisible(anEditable);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setVisible(anEditable);

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setVisible
    (anEditable);
}


void YerothModifierWindow::ajouter_nouveau_re_approvisionnement_achat(double
                                                                      quantiteEnReApprovisionement,
                                                                      int stockId)
{
    YerothSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    QSqlRecord nouveauAchatRecord = achatSqlTableModel.record();

    int next_achat_id = YerothERPWindows::getNextIdSqlTableModel_achats();

    QString utilisateurCourrantNomComplet;

    YerothPOSUser *aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourrantNomComplet.append(aUser->nom_complet());
    }

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::ID, next_achat_id);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::STOCKS_ID, stockId);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                                lineEdit_reference_produit->text());
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::DESIGNATION,
                                lineEdit_designation->text());
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::CATEGORIE,
                                lineEdit_categorie_produit->text());
    nouveauAchatRecord.
    setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
             lineEdit_nom_departement_produit->text());

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
                                utilisateurCourrantNomComplet);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::QUANTITE_TOTALE,
                                quantiteEnReApprovisionement);

    QString reference_recu_dachat = lineEdit_reference_recu_dachat->text();

    nouveauAchatRecord.
    setValue(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
             reference_recu_dachat);

    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_en_gros =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    double prix_dachat = lineEdit_prix_dachat->text().toDouble();

    if (prix_dachat < 0)
    {
        prix_dachat = 0;
    }

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_DACHAT,
                                prix_dachat);

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE,
                                prix_vente);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
                                prix_vente_en_gros);

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA,
                                _montantTva);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
                                _montantTva_EN_GROS);

    double prix_unitaire_ht = prix_vente - _montantTva;

    double prix_unitaire_en_gros_ht = prix_vente_en_gros - _montantTva_EN_GROS;


    if (!_IS_AN_ASSET)
    {
        double marge_beneficiaire =
                        YerothUtils::getMargeBeneficiaire(prix_vente, prix_dachat,
                                                          _montantTva);

        nouveauAchatRecord.
        setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
                 marge_beneficiaire);
    }
    else
    {
        nouveauAchatRecord.
        setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE, 0.0);
    }

    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                                prix_unitaire_ht);

    nouveauAchatRecord.
    setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
             prix_unitaire_en_gros_ht);

    nouveauAchatRecord.
    setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
             lineEdit_nom_entreprise_fournisseur->text());
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION,
                                YerothERPConfig::THIS_SITE_LOCALISATION_NAME);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                                lineEdit_localisation_produit->text());
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::DATE_ENTREE,
                                GET_CURRENT_DATE);
    nouveauAchatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                                dateEdit_date_peremption->date());

    bool achatSuccess = achatSqlTableModel
    						.insertNewRecord(nouveauAchatRecord,
    										 this,
											 "src/windows/stocks/yeroth-erp-modifier-window.cpp",
											 776);

    QString achatRetMsg(QObject::tr
                        ("1 nouveau achat a été enregistré dans la base de données !"));

    if (!achatSuccess)
    {
        achatRetMsg =
                        QObject::tr
                        ("Échec de l'enregistrement d'1 nouveau achat dans la base de données !");
    }

    YerothQMessageBox::information(this,
                                   QObject::tr("ajout d'1 nouveau achat"),
                                   achatRetMsg);
}


void YerothModifierWindow::update_achat_deja_existant(const QSqlRecord &aStockRecord,
                                                      double aPrixUnitaireHT,
                                                      double aPrixUnitaire_EN_GROS_HT)
{
    //Handling of table "achats"
    YerothSqlTableModel &achatSqlTableModel =
                    _allWindows->getSqlTableModel_achats();

    achatSqlTableModel.yerothSetFilter_WITH_where_clause(QString("%1='%2'").
                                                         arg
                                                         (YerothDatabaseTableColumn::
                                                          STOCKS_ID,
                                                          QString::
                                                          number
                                                          (_currentStockID)));

    int achatSqlTableModel_RESULT_RowCount =
    		achatSqlTableModel.easySelect("src/windows/stocks/yeroth-erp-modifier-windows.cpp", 814);

    QString curDatePeremption = GET_SQL_RECORD_DATA(aStockRecord,
                                                    YerothDatabaseTableColumn::DATE_PEREMPTION);

    QDate
    curDatePeremptionFormatedFORDB(GET_DATE_FROM_STRING(curDatePeremption));

    double marge_beneficiaire = 0.0;
    double prix_vente = lineEdit_prix_vente->text().toDouble();
    double anAchatPrixDachat = 0.0;

    QSqlRecord anAchatRecord;

    for (int k = 0; k < achatSqlTableModel_RESULT_RowCount; ++k)
    {
        anAchatRecord = achatSqlTableModel.record(k);

        anAchatRecord.setValue(YerothDatabaseTableColumn::DATE_PEREMPTION,
                               DATE_TO_DB_FORMAT_STRING
                               (curDatePeremptionFormatedFORDB));

        anAchatRecord.setValue(YerothDatabaseTableColumn::LOCALISATION_STOCK,
                               lineEdit_localisation_produit->text());

        anAchatPrixDachat =
                        GET_SQL_RECORD_DATA(anAchatRecord,
                                            YerothDatabaseTableColumn::PRIX_DACHAT).
                        toDouble();

        anAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE,
                               aPrixUnitaireHT);

        anAchatRecord.
        setValue(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
                 aPrixUnitaire_EN_GROS_HT);

        anAchatRecord.setValue(YerothDatabaseTableColumn::PRIX_VENTE,
                               prix_vente);

        if (!_IS_AN_ASSET)
        {
            marge_beneficiaire = YerothUtils::getMargeBeneficiaire(prix_vente,
                                                                   anAchatPrixDachat,
                                                                   _montantTva);

            anAchatRecord.
             setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE, marge_beneficiaire);
        }
        else
        {
            anAchatRecord.
             setValue(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE, 0.0);
        }


        achatSqlTableModel.updateRecord(k,
        								anAchatRecord,
										"src/windows/stocks/yeroth-erp-modifier-window.cpp",
										871);
    }

    achatSqlTableModel.resetFilter();
}


void YerothModifierWindow::setupLineEdits()
{
    lineEdit_quantite_restante->setValidator(&YerothUtils::UintValidator);
    lineEdit_stock_dalerte->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);
    lineEdit_prix_vente->setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    if (checkBox_re_approvisionnement->isChecked())
    {
        lineEdit_quantite_restante->setYerothEnabled(true);
        lineEdit_reference_recu_dachat->setYerothEnabled(true);
        lineEdit_nom_entreprise_fournisseur->setYerothEnabled(true);
    }
    else
    {
        lineEdit_quantite_restante->setYerothEnabled(false);
        lineEdit_reference_recu_dachat->setYerothEnabled(false);
        lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
    }


    lineEdit_localisation_produit->setYerothEnabled(true);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setYerothEnabled(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setYerothEnabled
    (false);

    lineEdit_prix_dachat->setYerothEnabled(false);
    lineEdit_prix_vente->setYerothEnabled(false);
    lineEdit_prix_vente_EN_GROS->setYerothEnabled(false);
    lineEdit_reference_produit->setYerothEnabled(false);
    lineEdit_designation->setYerothEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
    lineEdit_nom_departement_produit->setYerothEnabled(false);
    lineEdit_categorie_produit->setYerothEnabled(false);
    lineEdit_stock_dalerte->setYerothEnabled(true);
    lineEdit_tva->setYerothEnabled(false);

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());

    calculate_and_display_benefit_buying_price_percentage();
}


void YerothModifierWindow::setupLineEditsQCompleters()
{
    lineEdit_nom_entreprise_fournisseur->setupMyStaticQCompleter
    (YerothDatabase::FOURNISSEURS, YerothDatabaseTableColumn::NOM_ENTREPRISE);
}


void YerothModifierWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothModifierWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_enregistrer->disable(this);
    pushButton_supprimer_limage_du_stock->disable(this);
    pushButton_selectionner_image->disable(this);
}


void YerothModifierWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(stocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_stock()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
}


void YerothModifierWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiser, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerImage, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));

    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_annuler->enable(this, SLOT(stocks()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
    pushButton_enregistrer->enable(this, SLOT(actualiser_stock()));
    pushButton_supprimer_limage_du_stock->enable(this,
                                                 SLOT(supprimer_image_stock
                                                      ()));
    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
}


bool YerothModifierWindow::check_fields()
{
    bool prix_dachat = lineEdit_prix_dachat->checkField();

    bool prix_vente = lineEdit_prix_vente->checkField();

    if (_IS_AN_ASSET)
    {
        prix_vente = true;
    }

    return prix_dachat && prix_vente;
}


void YerothModifierWindow::clear_all_fields()
{
    checkBox_re_approvisionnement->setChecked(false);

    checkBox_tva->setChecked(false);

    lineEdit_pourcentage_prix_dachat_prix_de_vente->setText("0.00 %");

    lineEdit_pourcentage_prix_dachat_prix_de_vente_EN_GROS->setText("0.00 %");

    lineEdit_reference_produit->clearField();
    lineEdit_designation->clearField();
    lineEdit_nom_departement_produit->clearField();
    lineEdit_categorie_produit->clearField();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_stock_dalerte->clearField();
    lineEdit_prix_dachat->clearField();
    lineEdit_prix_vente->clearField();
    lineEdit_prix_vente_EN_GROS->clear();
    lineEdit_localisation_produit->clear();
    lineEdit_tva->clearField();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);

    textEdit_description->clear();
}


void YerothModifierWindow::display_prix_vente()
{
    if (_lastEditedPrixVente != lineEdit_prix_vente->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente = lineEdit_prix_vente->text().toDouble();

        _montantTva = prix_vente * YerothERPConfig::tva_value;

        prix_vente = prix_vente + _montantTva;

        lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));
    }
}


void YerothModifierWindow::display_prix_vente_EN_GROS()
{
    if (_lastEditedPrixVente_EN_GROS != lineEdit_prix_vente_EN_GROS->text())
    {
        return;
    }

    if (checkBox_tva->isChecked())
    {
        double prix_vente_EN_GROS =
                        lineEdit_prix_vente_EN_GROS->text().toDouble();

        _montantTva_EN_GROS = prix_vente_EN_GROS * YerothERPConfig::tva_value;

        prix_vente_EN_GROS = prix_vente_EN_GROS + _montantTva_EN_GROS;

        lineEdit_prix_vente_EN_GROS->
        setText(QString::number(prix_vente_EN_GROS, 'f', 2));
    }
}


void YerothModifierWindow::handleTVACheckBox(bool clicked)
{
    double prix_vente = lineEdit_prix_vente->text().toDouble();

    double prix_vente_EN_GROS =
                    lineEdit_prix_vente_EN_GROS->text().toDouble();

    if (clicked && checkBox_tva->isChecked())
    {
        if (false == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = true;
        }
        else
        {
            _tvaCheckBoxPreviousState = false;
        }
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = prix_vente * YerothERPConfig::tva_value;

        _montantTva_EN_GROS = prix_vente_EN_GROS * YerothERPConfig::tva_value;

        prix_vente = prix_vente + _montantTva;

        prix_vente_EN_GROS = prix_vente_EN_GROS + _montantTva_EN_GROS;
    }
    else
    {
        //qDebug() << "\t ++_montantTva: " << _montantTva;
        _montantTva = 0;

        _montantTva_EN_GROS = 0;

        if (true == _tvaCheckBoxPreviousState)
        {
            _tvaCheckBoxPreviousState = false;

            prix_vente = prix_vente / (1 + YerothERPConfig::tva_value);

            prix_vente_EN_GROS =
                            prix_vente_EN_GROS / (1 + YerothERPConfig::tva_value);
        }
    }

    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    lineEdit_prix_vente_EN_GROS->
    setText(QString::number(prix_vente_EN_GROS, 'f', 2));
}


void YerothModifierWindow::supprimer_ce_stock()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    msgSupprimer(QObject::tr
                 ("Poursuivre avec la suppression du stock '%1' ?").arg
                 (GET_SQL_RECORD_DATA
                  (record, YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("suppression"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _stocksWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerothModifierWindow::supprimer_ce_stock() " << resRemoved;

        stocks();

        if (resRemoved)
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("Le stock '%1' a été supprimé !").arg
                                (GET_SQL_RECORD_DATA
                                 (record,
                                  YerothDatabaseTableColumn::DESIGNATION)));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("1 immobilisation OU stock '%1' ne pouvait pas être supprimé(e) !").
                                arg(GET_SQL_RECORD_DATA
                                    (record,
                                     YerothDatabaseTableColumn::
                                     DESIGNATION)));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothModifierWindow::supprimer_image_stock()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    stockName(GET_SQL_RECORD_DATA
              (record, YerothDatabaseTableColumn::DESIGNATION));

    QVariant
    image_produit(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (image_produit.toByteArray().isEmpty())
    {
        QString
        msg(QObject::tr
            ("Le stock '%1' n'a pas d'image enregistrée !").arg
            (stockName));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("suppression de l'image d'1 immobilisation OU stock"),
                                       msg);

        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de l'image du stock \""));

    msgSupprimer.append(stockName);
    msgSupprimer.append("\" ?");

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression de l'image d'1 immobilisation OU stock"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        record.setValue(YerothDatabaseTableColumn::IMAGE_PRODUIT,
                        QVariant(QVariant::ByteArray));

        bool success_removed_STOCK_IMAGE =
                        _allWindows->
                        _stocksWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        (record);

        label_image_produit->clear();

        label_image_produit->setAutoFillBackground(false);

        if (success_removed_STOCK_IMAGE)
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("L'image du stock '%1' a été supprimée !").
                                arg(stockName));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de l'image du stock avec succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();

            msgSupprimer.append(QObject::tr
                                ("L'image du stock '%1' ne pouvait pas être supprimée !").
                                arg(stockName));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("échec de la suppression de l'image d'1 immobilisation OU stock"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothModifierWindow::rendreInvisible()
{
    clear_all_fields();

    _lastEditedPrixVente.clear();

    _montantTva = 0;

    _tvaPercent = YerothUtils::getTvaStringWithPercent();

    _tvaCheckBoxPreviousState = false;

    dateEdit_date_peremption->reset();

    YerothWindowsCommons::rendreInvisible();
}


void YerothModifierWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    checkBox_tva->setReadOnly(true);

    setupLineEditsQCompleters();

    showItem();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("modifier (ré-approvisionner) un(e) stock / immobilisation"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerothModifierWindow::setStockSpecificWidgetVisible(bool visible)
{
    label_reference_recu_dachat->setVisible(visible);
    lineEdit_reference_recu_dachat->setVisible(visible);

    label_stock_dalerte->setVisible(visible);
    lineEdit_stock_dalerte->setVisible(visible);

    label_localisation_du_stock->setVisible(visible);
    lineEdit_localisation_produit->setVisible(visible);

    dateEdit_date_peremption->setVisible(visible);
    label_date_peremption->setVisible(visible);
}


void YerothModifierWindow::showItem()
{
    QSqlRecord record;

    _allWindows->
    _stocksWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    bool is_service = GET_SQL_RECORD_DATA(record,
                                          YerothDatabaseTableColumn::IS_SERVICE).
                      toInt();

    if (is_service)
    {
        return;
    }

    QString recordID =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);

    _currentStockID = recordID.toInt();

    int achatQuerySize =
                    YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(_currentStockID);

    checkBox_achat->setVisible((achatQuerySize > 0));

    checkBox_achat->setChecked((achatQuerySize > 0));

    if (achatQuerySize > 0)
    {
        handle_achat_checkBox(Qt::Checked);
    }
    else
    {
        handle_achat_checkBox(Qt::Unchecked);
    }


    bool historique_stock_actif = GET_SQL_RECORD_DATA(record,
                                                      YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF).
                                  toInt();


    if (historique_stock_actif)
    {
        checkBox_HISTORIQUE->setYerothChecked(true,
                                              QObject::tr
                                              ("historique actif"));
    }
    else
    {
        checkBox_HISTORIQUE->setYerothChecked(false,
                                              QObject::tr
                                              ("historique désactiver"));
    }


    setStockSpecificWidgetVisible(true);


    if (!YerothUtils::IS_QSQL_RECORD_AN_ASSET(record))
    {
        _IS_AN_ASSET = false;

        lineEdit_prix_vente->setYerothEnabled(true);

        lineEdit_prix_vente_EN_GROS->setYerothEnabled(true);
    }
    else
    {
        _IS_AN_ASSET = true;

        lineEdit_prix_vente->setYerothEnabled(false);

        lineEdit_prix_vente_EN_GROS->setYerothEnabled(false);
    }


    label_fournisseur->setText(QObject::tr("fournisseur"));

    lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::REFERENCE));

    lineEdit_designation->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_stock_dalerte->setText(GET_SQL_RECORD_DATA
                                    (record,
                                     YerothDatabaseTableColumn::STOCK_DALERTE));

    lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA
                                            (record,
                                             YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));


    _currentPrixDachat =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::PRIX_DACHAT);

    lineEdit_prix_dachat->setText(_currentPrixDachat);

    YerothPOSUser *currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
        if (!currentUser->isManager() ||
                !currentUser->isGestionaireDesStocks())
        {
            lineEdit_reference_recu_dachat->setYerothEnabled(false);
            lineEdit_prix_dachat->setYerothEnabled(false);
        }
        else
        {
            lineEdit_reference_recu_dachat->setYerothEnabled(true);
            lineEdit_prix_dachat->setYerothEnabled(true);
        }
    }

    double prix_vente = GET_SQL_RECORD_DATA(record,
                                            YerothDatabaseTableColumn::
                                            PRIX_VENTE).toDouble();

    double prix_vente_EN_GROS = GET_SQL_RECORD_DATA(record,
                                                    YerothDatabaseTableColumn::
                                                    PRIX_VENTE_EN_GROS).
                                toDouble();

    _montantTva =
                    GET_SQL_RECORD_DATA(record,
                                        YerothDatabaseTableColumn::MONTANT_TVA).toDouble();
    //qDebug() << "\t++ showItem, _montantTva: " << _montantTva;
    if (_montantTva > 0)
    {
        checkBox_tva->setChecked(true);
        _tvaCheckBoxPreviousState = true;
    }

    lineEdit_tva->setText(YerothUtils::getTvaStringWithPercent());

    lineEdit_prix_vente->setText(QString::number(prix_vente, 'f', 2));

    lineEdit_prix_vente_EN_GROS->
    setText(QString::number(prix_vente_EN_GROS, 'f', 2));

    double quantite_restante = GET_SQL_RECORD_DATA(record,
                                                   YerothDatabaseTableColumn::QUANTITE_TOTALE).
                               toDouble();

    lineEdit_quantite_restante->
    setText(QString::number(quantite_restante, 'f', 2));
    textEdit_description->
    setText(GET_SQL_RECORD_DATA
            (record, YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));

    QString
    date_peremption(GET_SQL_RECORD_DATA
                    (record, YerothDatabaseTableColumn::DATE_PEREMPTION));

    dateEdit_date_peremption->setYerothEnabled(true);
    dateEdit_date_peremption->setMyDate(GET_DATE_FROM_STRING
                                        (date_peremption));
    lineEdit_nom_departement_produit->setText(GET_SQL_RECORD_DATA
                                              (record,
                                               YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));
    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::CATEGORIE));
    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::LOCALISATION_STOCK));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
