/*
 * yeroth-erp-modifier-compte-client-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */


#include "yeroth-erp-modifier-compte-client-window.hpp"

#include "src/yeroth-erp-windows.hpp"


#include <unistd.h>

#include <QtSql/QSqlRecord>

#include <QtGui/QContextMenuEvent>


YerothModifierCompteClientWindow::YerothModifierCompteClientWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothModifierCompteClientWindow")),
 _CLIENT_PREVIOUS_AVANCE_DE_CASH(-1.0),
 _curClientDetailDBID(-2)
{
    _windowName = QString("%1 - %2")
                	.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                		 QObject::tr("modifier un client"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_YELLOW_254_254_0,
                     COLOUR_RGB_STRING_YEROTH_BLACK_0_0_0);

    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserCompteClient,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler()));
    connect(actionActualiserCompteClient, SIGNAL(triggered()), this,
            SLOT(actualiserCompteClient()));
    connect(actionSupprimerCompteClient, SIGNAL(triggered()), this,
            SLOT(supprimerCompteClient()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
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

void YerothModifierCompteClientWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}

void YerothModifierCompteClientWindow::setupLineEdits()
{
    lineEdit_modifier_compteclient_reference_client->setYerothEnabled(false);

    lineEdit_modifier_compteclient_AVANCE_DE_CASH->setYerothEnabled(false);
    lineEdit_modifier_compteclient_AVANCE_DE_CASH->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);

    lineEdit_modifier_compteclient_dette_maximale->setYerothEnabled(false);
    lineEdit_modifier_compteclient_dette_maximale->
    setValidator(&YerothUtils::POSITIVE_DoubleValidator);
}


void YerothModifierCompteClientWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_modifierCompteClientWindow->palette());
    menu.addAction(actionActualiserCompteClient);
    menu.addAction(actionSupprimerCompteClient);
    menu.exec(event->globalPos());
}

void YerothModifierCompteClientWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}

void YerothModifierCompteClientWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserCompteClient,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_clients->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);
}

void YerothModifierCompteClientWindow::definirCaissier()
{
    _logger->log("definirCaissier - definirPasDeRole()");

    definirPasDeRole();
}

void YerothModifierCompteClientWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->enable(this, SLOT(supprimerCompteClient()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_actualiser->enable(this, SLOT(actualiserCompteClient()));

    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_supprimer_image->enable(this,
                                       SLOT(supprimer_image_compte_client
                                            ()));
}


void YerothModifierCompteClientWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserCompteClient, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerCompteClient, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_clients->enable(this, SLOT(clients()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_actualiser->enable(this, SLOT(actualiserCompteClient()));

    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_supprimer_image->enable(this,
                                       SLOT(supprimer_image_compte_client
                                            ()));
}


void YerothModifierCompteClientWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks - definirPasDeRole()");

    definirPasDeRole();
}

void YerothModifierCompteClientWindow::definirMagasinier()
{
    _logger->log("definirMagasinier - definirPasDeRole()");

    definirPasDeRole();
}


bool YerothModifierCompteClientWindow::modifier_client_check_fields()
{
    bool nom_entreprise =
                    lineEdit_modifier_compteclient_nom_entreprise->checkField();

    return nom_entreprise;
}


void YerothModifierCompteClientWindow::clear_all_fields()
{
    lineEdit_modifier_compteclient_reference_registre_du_commerce->clear();
    lineEdit_modifier_compteclient_reference_client->clear();
    lineEdit_modifier_compteclient_nom_entreprise->clearField();
    lineEdit_modifier_compteclient_nom_representant->clearField();
    lineEdit_modifier_compteclient_quartier->clear();
    lineEdit_modifier_compteclient_ville->clear();
    lineEdit_modifier_compteclient_province_etat->clear();
    lineEdit_modifier_compteclient_pays->clear();
    lineEdit_modifier_compteclient_boite_postale->clear();
    lineEdit_modifier_compteclient_siege_social->clear();
    lineEdit_modifier_compteclient_email->clear();
    lineEdit_modifier_compteclient_numero_telephone_1->clear();
    lineEdit_modifier_compteclient_numero_telephone_2->clear();
    lineEdit_modifier_compteclient_numero_contribuable->clear();
    lineEdit_modifier_compteclient_dette_maximale->clear();
    lineEdit_modifier_compteclient_AVANCE_DE_CASH->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}


void YerothModifierCompteClientWindow::supprimer_image_compte_client()
{
    QSqlRecord record;

    _allWindows->
    _clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString
    nomEntreprise(GET_SQL_RECORD_DATA
                  (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QVariant
    image_produit(record.value
                  (YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT));

    if (image_produit.toByteArray().isEmpty())
    {
        QString
        msg(QObject::tr
            ("Le client '%1' n'a pas d'image enregistrée !").arg
            (nomEntreprise));

        YerothQMessageBox::information(this,
                                       QObject::tr("suppression de l'image"),
                                       msg);

        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de l'image de ce client \""));

    msgSupprimer.append(nomEntreprise);
    msgSupprimer.append("\" ?");

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr
                                        ("suppression de l'image ce client"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        record.setValue(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT,
                        QVariant(QVariant::ByteArray));

        bool resRemoved =
                        _allWindows->_clientsWindow->
                        SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        if (resRemoved)
        {
            label_image_produit->clear();

            label_image_produit->setAutoFillBackground(false);

            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr("L'image de ce client \""));
            msgSupprimer.append(nomEntreprise);
            msgSupprimer.append(QObject::tr("\" a été supprimée !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de l'image du client avec succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr("L'image de ce client \""));
            msgSupprimer.append(nomEntreprise);
            msgSupprimer.append(QObject::tr
                                ("\" ne pouvait pas être supprimée !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("échec de la suppression de l'image d'un client"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


bool YerothModifierCompteClientWindow::customerAccountAlreadyExist()
{
    bool result = false;

    // ** check if customer account with same name exist
    bool resultNomEntreprise =
                    YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
                                                                                  *_curClientTableModel,
                                                                                  *lineEdit_modifier_compteclient_nom_entreprise,
                                                                                  _curClientDetailDBID);

    result = result || resultNomEntreprise;

    // ** check if customer account with same trade registry number exist
    bool resultReferenceRegistreDuCommerce =
                    YerothUtils::
                    checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this,
                                                                                      *_curClientTableModel,
                                                                                      *lineEdit_modifier_compteclient_reference_registre_du_commerce,
                                                                                      _curClientDetailDBID);

    result = result || resultReferenceRegistreDuCommerce;

    // ** check if customer account with same reference exist
    bool resultReferenceClient =
                    YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(*this,
                                                                                    *_curClientTableModel,
                                                                                    *lineEdit_modifier_compteclient_reference_client,
                                                                                    _curClientDetailDBID);

    result = result || resultReferenceClient;


    return result;
}


bool
YerothModifierCompteClientWindow::INSERT_IF_NEEDED_CLIENT_AVANCE_DE_CASH
(double current_client_account_AVANCE_DE_CASH)
{
    bool
    successPaiementsInsert = false;

    YerothSqlTableModel &paiementsSqlTableModel =
                    _allWindows->getSqlTableModel_paiements();

    QSqlRecord
    paiementsRecord = paiementsSqlTableModel.record();


    QString
    curNomDuClient = lineEdit_modifier_compteclient_nom_entreprise->text();

    paiementsRecord.setValue(YerothDatabaseTableColumn::DATE_PAIEMENT,
                             GET_CURRENT_DATE);
    paiementsRecord.setValue(YerothDatabaseTableColumn::HEURE_PAIEMENT,
                             CURRENT_TIME);
    paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                             curNomDuClient);


    QString
    utilisateurCourantNomComplet;

    YerothPOSUser *
    aUser = _allWindows->getUser();

    if (0 != aUser)
    {
        utilisateurCourantNomComplet = aUser->nom_complet();

        paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR,
                                 utilisateurCourantNomComplet);
    }
    else
    {
        paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENCAISSEUR,
                                 QObject::tr("inconnu(e)"));
    }


    paiementsRecord.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 0);

    paiementsRecord.
    setValue(YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
             current_client_account_AVANCE_DE_CASH);


    paiementsRecord.setValue(YerothDatabaseTableColumn::MONTANT_PAYE,
                             current_client_account_AVANCE_DE_CASH);

    paiementsRecord.setValue(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
                             YerothUtils::ENCAISSEMENT_AVANCE_DE_CASH);

    paiementsRecord.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                             curNomDuClient);


    paiementsRecord.setValue(YerothDatabaseTableColumn::NOTES,
                             QObject::tr("AVANCE DE CASH (%1), pour future utilisation par 1 'client: %2' !")
							 	 .arg(GET_CURRENCY_STRING_NUM(current_client_account_AVANCE_DE_CASH),
							 		  curNomDuClient));


    int IDforReceipt = YerothERPWindows::getNextIdSqlTableModel_paiements();

    QString referenceRecuPaiementClient =
    		YerothUtils::GET_REFERENCE_RECU_PAIEMENT_CLIENT(QString::number(IDforReceipt));

    paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
             	 	 	 	 referenceRecuPaiementClient);

    paiementsRecord.setValue(YerothDatabaseTableColumn::REFERENCE,
                             referenceRecuPaiementClient);


    successPaiementsInsert =
                    paiementsSqlTableModel.insertNewRecord(paiementsRecord,
                    									  this,
														  "src/windows/crm/modifier/yeroth-erp-modifier-compte-client-window.cpp",
														  491,
														  curNomDuClient);

    if (successPaiementsInsert)
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("paiements - succès"),
                                       QObject::tr
                                       ("L'AVANCE DE CASH DU CLIENT %1 (%2), "
                                        "avec référence 'reçu de paiement %3', a été "
                                        "insérée dans le tableau des PAIEMENTS !").
                                       arg(curNomDuClient,
                                           GET_CURRENCY_STRING_NUM
                                           (current_client_account_AVANCE_DE_CASH),
                                           referenceRecuPaiementClient));
    }
    else
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr("paiements - échec"),
                                   QObject::tr
                                   ("1 entrée dans le tableau des paiements n'a pas pu "
                                    "être faites pour L'AVANCE DE CASH DU CLIENT %1 !").
                                   arg(curNomDuClient));
    }

    return successPaiementsInsert;
}


void YerothModifierCompteClientWindow::annuler()
{
    rendreInvisible();

    _allWindows->_clientsDetailWindow->rendreVisible(_curClientTableModel,
                                                     _curStocksTableModel);
}


void YerothModifierCompteClientWindow::actualiserCompteClient()
{
    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    if (modifier_client_check_fields())
    {
        if (customerAccountAlreadyExist())
        {
            return;
        }

        QSqlRecord record;

        _allWindows->
        _clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
        (record);

        bool currentCompanyNameChanged = false;

        QString
        oldCompanyName(GET_SQL_RECORD_DATA
                       (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

//        qDebug() << QString("++ oldCompanyName: %1, company name: %2")
//                                      .arg(oldCompanyName, lineEdit_modifier_compteclient_nom_entreprise->text());

        if (!YerothUtils::isEqualCaseInsensitive(oldCompanyName,
                                                 lineEdit_modifier_compteclient_nom_entreprise->
                                                 text()))
        {
            currentCompanyNameChanged = true;
        }


        double client_currently_changed_AVANCE_DE_CASH_montant =
                        lineEdit_modifier_compteclient_AVANCE_DE_CASH->text().toDouble();

        if (_CLIENT_PREVIOUS_AVANCE_DE_CASH !=
                client_currently_changed_AVANCE_DE_CASH_montant
                && client_currently_changed_AVANCE_DE_CASH_montant >= 0)
        {
            /*
             * AJOUT DE CETTE AVANCE DE CASH DANS LE tableau des paiements !
             */
            if (!INSERT_IF_NEEDED_CLIENT_AVANCE_DE_CASH
                    (client_currently_changed_AVANCE_DE_CASH_montant))
            {
                return;
            }
        }


        record.setValue(YerothDatabaseTableColumn::REFERENCE_CLIENT,
                        lineEdit_modifier_compteclient_reference_client->text
                        ());
        record.
        setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 lineEdit_modifier_compteclient_reference_registre_du_commerce->
                 text());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                        lineEdit_modifier_compteclient_nom_entreprise->
                        text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT,
                        lineEdit_modifier_compteclient_nom_representant->
                        text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER,
                        lineEdit_modifier_compteclient_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE,
                        lineEdit_modifier_compteclient_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_modifier_compteclient_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS,
                        lineEdit_modifier_compteclient_pays->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL,
                        lineEdit_modifier_compteclient_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_modifier_compteclient_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL,
                        lineEdit_modifier_compteclient_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_modifier_compteclient_numero_telephone_1->
                        text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_modifier_compteclient_numero_telephone_2->
                        text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                        lineEdit_modifier_compteclient_numero_contribuable->
                        text());

        record.setValue(YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
                        client_currently_changed_AVANCE_DE_CASH_montant);

        record.
        setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
                 lineEdit_modifier_compteclient_dette_maximale->text().
                 toDouble());

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CLIENT,
                        textEdit_modifier_compteclient_description_du_client->
                        toPlainText());


        if (0 != label_image_produit->pixmap())
        {
            QByteArray bytes;
            YerothUtils::savePixmapToByteArray(bytes,
                                               *label_image_produit->pixmap(),
                                               "JPG");
            record.setValue(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT,
                            QVariant::fromValue(bytes));
        }

        bool success =
                        _allWindows->
                        _clientsWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        (record);

        if (success)
        {
            //Handling of table "services_completes"
            if (currentCompanyNameChanged)
            {
                QString
                stocksVenduCompanyNameQuery(QString
                                            ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                            arg(YerothDatabase::
                                                SERVICES_COMPLETES,
                                                YerothDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT,
                                                lineEdit_modifier_compteclient_nom_entreprise->
                                                text(),
                                                YerothDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT,
                                                oldCompanyName));

                YerothUtils::execQuery(stocksVenduCompanyNameQuery, 0);
            }

            //Handling of table "stocks_vendu"
            if (currentCompanyNameChanged)
            {
                QString
                stocksVenduCompanyNameQuery(QString
                                            ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                            arg(YerothDatabase::
                                                STOCKS_VENDU,
                                                YerothDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT,
                                                lineEdit_modifier_compteclient_nom_entreprise->
                                                text(),
                                                YerothDatabaseTableColumn::
                                                NOM_ENTREPRISE_CLIENT,
                                                oldCompanyName));

                YerothUtils::execQuery(stocksVenduCompanyNameQuery, 0);
            }

            //Handling of table "paiements"
            if (currentCompanyNameChanged)
            {
                QString
                paiementsCompanyNameQuery(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerothDatabase::PAIEMENTS,
                                              YerothDatabaseTableColumn::
                                              NOM_ENTREPRISE,
                                              lineEdit_modifier_compteclient_nom_entreprise->
                                              text(),
                                              YerothDatabaseTableColumn::
                                              NOM_ENTREPRISE,
                                              oldCompanyName));

                YerothUtils::execQuery(paiementsCompanyNameQuery, 0);
            }
        }


        QString retMsg(QObject::tr("Les données du client '%1'").arg
                       (lineEdit_modifier_compteclient_nom_entreprise->text
                        ()));

        if (success)
        {
            retMsg.
            append(QObject::tr(" ont été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("modification - succès"), retMsg);

            _allWindows->_clientsDetailWindow->rendreVisible(_curClientTableModel, _curStocksTableModel);

            rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " <<
                     _curClientTableModel->lastError();

            retMsg.
            append(QObject::tr(" n'ont pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("modification - échec"), retMsg);
        }
    }

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
}


void YerothModifierCompteClientWindow::supprimerCompteClient()
{
    QSqlRecord record;

    bool success =
                    _allWindows->
                    _clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du client '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("supprimer"),
                                        msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _clientsWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW();

        //qDebug() << "YerothModifierCompteClientWindow::supprimer_ce_stock() " << resRemoved;

        if (resRemoved && _curClientTableModel->select())
        {
            msgSupprimer =
                            QObject::tr("Le client '%1' a été supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("supprimer - succès"),
                                           msgSupprimer);

            clients();
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le client '%1' ne pouvait pas être supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("échec de la suppression d'un client"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


void YerothModifierCompteClientWindow::rendreInvisible()
{
    clear_all_fields();

    tabWidget_modifier->setCurrentIndex(0);

    YerothWindowsCommons::rendreInvisible();
}


void YerothModifierCompteClientWindow::rendreVisible(YerothSqlTableModel *clientTableModel,
                                                     YerothSqlTableModel *stocksTableModel,
                                                     bool 				 show_TabDESCRIPTION_CLIENT /* = false */)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curClientTableModel = clientTableModel;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_db_ID();
    showClientDetail();

    //QDEBUG_STRING_OUTPUT_2("show_TabDESCRIPTION_CLIENT", BOOL_TO_STRING(show_TabDESCRIPTION_CLIENT));

    if (!show_TabDESCRIPTION_CLIENT)
    {
        tabWidget_modifier->setCurrentIndex(0);
    }
    else
    {
        tabWidget_modifier->setCurrentIndex(1);
    }

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("modifier un client"),
    								 _curClientTableModel);
}


void YerothModifierCompteClientWindow::showClientDetail()
{
    QSqlRecord record;

    _allWindows->_clientsWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

    _curClientDetailDBID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

    lineEdit_modifier_compteclient_reference_client
		->setText(GET_SQL_RECORD_DATA(record,
								      YerothDatabaseTableColumn::REFERENCE_CLIENT));

    lineEdit_modifier_compteclient_reference_registre_du_commerce
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

    lineEdit_modifier_compteclient_nom_entreprise
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::NOM_ENTREPRISE));

    lineEdit_modifier_compteclient_nom_representant
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::NOM_REPRESENTANT));

    lineEdit_modifier_compteclient_quartier
		->setText(GET_SQL_RECORD_DATA(record,
									  YerothDatabaseTableColumn::QUARTIER));

    lineEdit_modifier_compteclient_ville->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerothDatabaseTableColumn::VILLE));

    lineEdit_modifier_compteclient_province_etat->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerothDatabaseTableColumn::PROVINCE_ETAT));

    lineEdit_modifier_compteclient_pays->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerothDatabaseTableColumn::PAYS));

    lineEdit_modifier_compteclient_boite_postale->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerothDatabaseTableColumn::BOITE_POSTALE));

    lineEdit_modifier_compteclient_siege_social->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerothDatabaseTableColumn::SIEGE_SOCIAL));

    lineEdit_modifier_compteclient_email->setText(GET_SQL_RECORD_DATA
                                                  (record,
                                                   YerothDatabaseTableColumn::EMAIL).
                                                  toLower());

    lineEdit_modifier_compteclient_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

    lineEdit_modifier_compteclient_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    lineEdit_modifier_compteclient_numero_contribuable->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));


    YerothPOSUser *currentUser = _allWindows->getUser();

    if (0 != currentUser && !currentUser->isManager())
    {
        lineEdit_modifier_compteclient_AVANCE_DE_CASH->
        setAlignment(Qt::AlignRight);

        lineEdit_modifier_compteclient_AVANCE_DE_CASH->setYerothEnabled(false);

        lineEdit_modifier_compteclient_AVANCE_DE_CASH->setText
        (GET_CURRENCY_STRING_NUM
         (GET_SQL_RECORD_DATA
          (record,
           YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH).
          toDouble()));
    }
    else
    {
        lineEdit_modifier_compteclient_AVANCE_DE_CASH->
        setAlignment(Qt::AlignLeft);

        lineEdit_modifier_compteclient_AVANCE_DE_CASH->setYerothEnabled(true);

        _CLIENT_PREVIOUS_AVANCE_DE_CASH =
                        GET_SQL_RECORD_DATA(record,
                                            YerothDatabaseTableColumn::
                                            COMPTECLIENT_AVANCE_DE_CASH).toDouble();

        lineEdit_modifier_compteclient_AVANCE_DE_CASH->setText
        (GET_SQL_RECORD_DATA
         (record, YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH));
    }

    lineEdit_modifier_compteclient_dette_maximale->setText
    (GET_CURRENCY_STRING_NUM
     (GET_SQL_RECORD_DATA
      (record,
       YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT).toDouble()));

    textEdit_modifier_compteclient_description_du_client->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_CLIENT));

    QVariant
    img(record.value(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
