/*
 * yeroth-erp-modifier-fournisseur-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */


#include "yeroth-erp-modifier-fournisseur-window.hpp"

#include "src/yeroth-erp-windows.hpp"


#include <unistd.h>

#include <QtSql/QSqlRecord>

#include <QtGui/QContextMenuEvent>


YerothModifierFournisseurWindow::YerothModifierFournisseurWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothModifierFournisseurWindow")),
 _curFournisseurDetailDBID(-2)
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("modifier un fournisseur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    setupLineEdits();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
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
    connect(actionActualiserFournisseur, SIGNAL(triggered()), this,
            SLOT(actualiserFournisseur()));
    connect(actionSupprimerFournisseur, SIGNAL(triggered()), this,
            SLOT(supprimerFournisseur()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(entrer()));
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

void YerothModifierFournisseurWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothModifierFournisseurWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_modifierFournisseurWindow->palette());
    menu.addAction(actionActualiserFournisseur);
    menu.addAction(actionSupprimerFournisseur);
    menu.exec(event->globalPos());
}


void YerothModifierFournisseurWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothModifierFournisseurWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);
}


void YerothModifierFournisseurWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);
}


void YerothModifierFournisseurWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_actualiser->enable(this, SLOT(actualiserFournisseur()));

    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_supprimer_image->enable(this,
                                       SLOT(supprimer_image_fournisseur()));
}


void YerothModifierFournisseurWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->enable(this, SLOT(supprimerFournisseur()));
    pushButton_annuler->enable(this, SLOT(annuler()));
    pushButton_actualiser->enable(this, SLOT(actualiserFournisseur()));

    pushButton_selectionner_image->enable(this,
                                          SLOT(selectionner_image_produit
                                               ()));
    pushButton_supprimer_image->enable(this,
                                       SLOT(supprimer_image_fournisseur()));
}


void YerothModifierFournisseurWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);
}

void YerothModifierFournisseurWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionActualiserFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimerFournisseur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    pushButton_fournisseurs->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_annuler->disable(this);
    pushButton_actualiser->disable(this);

    pushButton_selectionner_image->disable(this);
    pushButton_supprimer_image->disable(this);
}


bool YerothModifierFournisseurWindow::modifier_fournisseur_check_fields()
{
    bool nom_entreprise =
                    lineEdit_modifier_fournisseur_nom_entreprise->checkField();

    return nom_entreprise;
}


void YerothModifierFournisseurWindow::clear_all_fields()
{
    lineEdit_modifier_fournisseur_reference_registre_du_commerce->clear();
    lineEdit_modifier_fournisseur_reference_fournisseur->clear();
    lineEdit_modifier_fournisseur_nom_entreprise->clearField();
    lineEdit_modifier_fournisseur_nom_representant->clearField();
    lineEdit_modifier_fournisseur_quartier->clear();
    lineEdit_modifier_fournisseur_ville->clear();
    lineEdit_modifier_fournisseur_province_etat->clear();
    lineEdit_modifier_fournisseur_pays->clear();
    lineEdit_modifier_fournisseur_boite_postale->clear();
    lineEdit_modifier_fournisseur_siege_social->clear();
    lineEdit_modifier_fournisseur_email->clear();
    lineEdit_modifier_fournisseur_numero_telephone_1->clear();
    lineEdit_modifier_fournisseur_numero_telephone_2->clear();
    lineEdit_modifier_fournisseur_numero_contribuable->clear();

	lineEdit_employe_SUPERIEUR_hierarchique->clear();
	lineEdit_employe_DEPARTEMENT->clear();
	lineEdit_EMPLOYE_position_fonction->clear();

    label_image_produit->clear();
    label_image_produit->setAutoFillBackground(false);
}


void YerothModifierFournisseurWindow::supprimer_image_fournisseur()
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    QString
    nomEntreprise(GET_SQL_RECORD_DATA
                  (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

    QVariant
    image_produit(record.value
                  (YerothDatabaseTableColumn::IMAGE_FOURNISSEUR));

    if (image_produit.toByteArray().isEmpty())
    {
        QString msg(QObject::tr
                    ("Le fournisseur '%1' n'a pas d'image enregistrée !").arg
                    (nomEntreprise));

        YerothQMessageBox::information(this,
                                       QObject::tr("suppression de l'image"),
                                       msg);

        label_image_produit->clear();
        label_image_produit->setAutoFillBackground(false);

        return;
    }


    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de l'image de ce fournisseur \""));

    msgSupprimer.append(nomEntreprise);
    msgSupprimer.append("\" ?");

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression de l'image"),
                                        msgSupprimer, QMessageBox::Cancel,
                                        QMessageBox::Ok))
    {
        record.setValue(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR,
                        QVariant(QVariant::ByteArray));

        bool resRemoved =
                        _allWindows->
                        _fournisseursWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        (record);

        if (resRemoved)
        {
            label_image_produit->clear();

            label_image_produit->setAutoFillBackground(false);

            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr("L'image de ce fournisseur \""));
            msgSupprimer.append(nomEntreprise);
            msgSupprimer.append(QObject::tr("\" a été supprimée !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de l'image - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr("L'image de ce fournisseur \""));
            msgSupprimer.append(nomEntreprise);
            msgSupprimer.append(QObject::tr
                                ("\" ne pouvait pas être supprimée !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression de l'image - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }
}


bool YerothModifierFournisseurWindow::customerAccountAlreadyExist()
{
    bool result = false;

    // ** check if customer account with same name exist
    bool resultNomEntreprise =
                    YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(*this,
                                                                                  *_curFournisseurTableModel,
                                                                                  *lineEdit_modifier_fournisseur_nom_entreprise,
                                                                                  _curFournisseurDetailDBID);

    result = result || resultNomEntreprise;

    // ** check if customer account with same trade registry number exist
    bool resultReferenceRegistreDuCommerce =
                    YerothUtils::
                    checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(*this,
                                                                                      *_curFournisseurTableModel,
                                                                                      *lineEdit_modifier_fournisseur_reference_registre_du_commerce,
                                                                                      _curFournisseurDetailDBID);

    result = result || resultReferenceRegistreDuCommerce;

    // ** check if customer account with same reference exist
    bool resultReferenceFournisseur =
                    YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(*this,
                                                                                    *_curFournisseurTableModel,
                                                                                    *lineEdit_modifier_fournisseur_reference_fournisseur,
                                                                                    _curFournisseurDetailDBID);

    result = result || resultReferenceFournisseur;


    return result;
}


void YerothModifierFournisseurWindow::annuler()
{
    _allWindows->_fournisseurDetailsWindow
		->rendreVisible(_curFournisseurTableModel,
						_curStocksTableModel,
						_SHOW_EMPLOYEE);

    rendreInvisible();
}


void YerothModifierFournisseurWindow::actualiserFournisseur()
{
    if (modifier_fournisseur_check_fields())
    {
        if (customerAccountAlreadyExist())
        {
            return;
        }

        YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

        QSqlRecord record;

        _allWindows->_fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        bool currentCompanyNameChanged = false;

        QString
        oldCompanyName(GET_SQL_RECORD_DATA
                       (record, YerothDatabaseTableColumn::NOM_ENTREPRISE));

        //qDebug() << QString("++ oldCompanyName: %1, company name: %2")
        //                              .arg(oldCompanyName, lineEdit_modifier_fournisseur_nom_entreprise->text());

        if (!YerothUtils::isEqualCaseSensitive(oldCompanyName,
                                               lineEdit_modifier_fournisseur_nom_entreprise->text()))
        {
            currentCompanyNameChanged = true;
        }

        record.setValue(YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR,
                        lineEdit_modifier_fournisseur_reference_fournisseur->
                        text());
        record.
        setValue(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
                 lineEdit_modifier_fournisseur_reference_registre_du_commerce->
                 text());
        record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                        lineEdit_modifier_fournisseur_nom_entreprise->text());
        record.setValue(YerothDatabaseTableColumn::NOM_REPRESENTANT,
                        lineEdit_modifier_fournisseur_nom_representant->
                        text());
        record.setValue(YerothDatabaseTableColumn::QUARTIER,
                        lineEdit_modifier_fournisseur_quartier->text());
        record.setValue(YerothDatabaseTableColumn::VILLE,
                        lineEdit_modifier_fournisseur_ville->text());
        record.setValue(YerothDatabaseTableColumn::PROVINCE_ETAT,
                        lineEdit_modifier_fournisseur_province_etat->text());
        record.setValue(YerothDatabaseTableColumn::PAYS,
                        lineEdit_modifier_fournisseur_pays->text());
        record.setValue(YerothDatabaseTableColumn::SIEGE_SOCIAL,
                        lineEdit_modifier_fournisseur_siege_social->text());
        record.setValue(YerothDatabaseTableColumn::BOITE_POSTALE,
                        lineEdit_modifier_fournisseur_boite_postale->text());
        record.setValue(YerothDatabaseTableColumn::EMAIL,
                        lineEdit_modifier_fournisseur_email->text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1,
                        lineEdit_modifier_fournisseur_numero_telephone_1->
                        text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2,
                        lineEdit_modifier_fournisseur_numero_telephone_2->
                        text());
        record.setValue(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE,
                        lineEdit_modifier_fournisseur_numero_contribuable->text());

        record.setValue(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR,
                        textEdit_modifier_fournisseur_description_du_fournisseur->
                        toPlainText());

        if (_SHOW_EMPLOYEE)
        {
            record.setValue(YerothDatabaseTableColumn::MANAGER,
            		lineEdit_employe_SUPERIEUR_hierarchique->text());

            record.setValue(YerothDatabaseTableColumn::DEPARTEMENT,
            		lineEdit_employe_DEPARTEMENT->text());

        	record.setValue(YerothDatabaseTableColumn::POSITION_FONCTION,
        			lineEdit_EMPLOYE_position_fonction->text());
        }
        else
        {

        }


        if (0 != label_image_produit->pixmap())
        {
            QByteArray bytes;
            YerothUtils::savePixmapToByteArray(bytes,
                                               *label_image_produit->pixmap(),
                                               "JPG");
            record.setValue(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR,
                            QVariant::fromValue(bytes));
        }

        bool success =_allWindows
        				->_fournisseursWindow->SQL_UPDATE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW(record);

        if (success)
        {
            //Handling of table "stocks_vendu"
            if (currentCompanyNameChanged)
            {
                QString
                stocksVenduCompanyNameQuery(QString
                                            ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                            arg(YerothDatabase::STOCKS_VENDU,
                                                YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                                                lineEdit_modifier_fournisseur_nom_entreprise->text(),
                                                YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                                                oldCompanyName));

                YerothUtils::execQuery(stocksVenduCompanyNameQuery, 0);
            }

            //Handling of table "charges_financieres"
            if (currentCompanyNameChanged)
            {
                QString
                charges_financieres_CompanyNameQuery(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerothDatabase::CHARGES_FINANCIERES,
                                              YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                                              lineEdit_modifier_fournisseur_nom_entreprise->text(),
                                              YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
                                              oldCompanyName));

                YerothUtils::execQuery(charges_financieres_CompanyNameQuery, 0);
            }

            //Handling of table "paiements"
            if (currentCompanyNameChanged)
            {
                QString
                paiementsCompanyNameQuery(QString
                                          ("UPDATE %1 SET %2='%3' WHERE %4='%5'").
                                          arg(YerothDatabase::PAIEMENTS,
                                              YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                              lineEdit_modifier_fournisseur_nom_entreprise->text(),
                                              YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                              oldCompanyName));

                YerothUtils::execQuery(paiementsCompanyNameQuery, 0);
            }
        }

        YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

        QString retMsg(QObject::tr("Les données du fournisseur '%1'").arg
                       (lineEdit_modifier_fournisseur_nom_entreprise->text
                        ()));

        if (success)
        {
            retMsg.
            append(QObject::tr(" ont été modifiées avec succès !"));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("modification - succès"), retMsg);

            _allWindows->_fournisseurDetailsWindow
				->rendreVisible(_curFournisseurTableModel,
								_curStocksTableModel,
								_SHOW_EMPLOYEE);

            rendreInvisible();
        }
        else
        {
            qDebug() << "\t[reason for failing] " <<
                     _curFournisseurTableModel->lastError();

            retMsg.
            append(QObject::tr(" n'ont pas pu être modifiées !"));

            YerothQMessageBox::warning(this,
                                       QObject::tr
                                       ("modification - échec"), retMsg);
        }
    }
}


void YerothModifierFournisseurWindow::supprimerFournisseur()
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression du fournisseur '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerothDatabaseTableColumn::NOM_ENTREPRISE)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this, QObject::tr("suppression"),
                                        msgSupprimer,
                                        QMessageBox::Cancel, QMessageBox::Ok))
    {
        bool resRemoved =
                        _allWindows->
                        _fournisseursWindow->SQL_DELETE_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
                        ();

        //qDebug() << "YerothModifierFournisseurWindow::supprimer_ce_stock() " << resRemoved;

        if (resRemoved)
        {
            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr
                                ("Le fournisseur '%1' a été supprimé !").arg
                                (GET_SQL_RECORD_DATA
                                 (record,
                                  YerothDatabaseTableColumn::NOM_ENTREPRISE)));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("suppression - succès"),
                                           msgSupprimer);

            fournisseurs();
        }
        else
        {
            msgSupprimer.clear();
            msgSupprimer.append(QObject::tr
                                ("Le fournisseur '%1' ne pouvait pas être supprimé !").
                                arg(GET_SQL_RECORD_DATA
                                    (record,
                                     YerothDatabaseTableColumn::
                                     NOM_ENTREPRISE)));

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


void YerothModifierFournisseurWindow::rendreInvisible()
{
    clear_all_fields();
    YerothWindowsCommons::rendreInvisible();
}


void YerothModifierFournisseurWindow::rendreVisible(YerothSqlTableModel *fournisseurTableModel,
                                                    YerothSqlTableModel *stocksTableModel,
                                                    bool show_TabDESCRIPTION_SUPPLIER /* = false */,
													bool show_EMPLOYE /* = false */)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    _curFournisseurTableModel = fournisseurTableModel;

    _SHOW_EMPLOYEE = show_EMPLOYE;

    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_db_ID();

    if (true == _SHOW_EMPLOYEE)
    {
    	label_employe_SUPERIEUR_hierarchique->setVisible(true);
    	label_employe_DEPARTEMENT->setVisible(true);
		label_EMPLOYE_position_fonction->setVisible(true);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(true);
		lineEdit_employe_DEPARTEMENT->setVisible(true);
		lineEdit_EMPLOYE_position_fonction->setVisible(true);
    }
    else
    {
    	label_employe_SUPERIEUR_hierarchique->setVisible(false);
    	label_employe_DEPARTEMENT->setVisible(false);
		label_EMPLOYE_position_fonction->setVisible(false);

		lineEdit_employe_SUPERIEUR_hierarchique->setVisible(false);
		lineEdit_employe_DEPARTEMENT->setVisible(false);
		lineEdit_EMPLOYE_position_fonction->setVisible(false);
    }

    showFournisseurDetail(_SHOW_EMPLOYEE);

    if (!show_TabDESCRIPTION_SUPPLIER)
    {
        tabWidget_modifier_fournisseur->setCurrentIndex(0);
    }
    else
    {
        tabWidget_modifier_fournisseur->setCurrentIndex(1);
    }

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("modifier un fournisseur"),
    								 _curFournisseurTableModel);
}


void YerothModifierFournisseurWindow::showFournisseurDetail(bool show_EMPLOYE /* = false */)
{
    QSqlRecord record;

    _allWindows->
    _fournisseursWindow->SQL_QUERY_YEROTH_TABLE_VIEW_LAST_SELECTED_ROW
    (record);

    _curFournisseurDetailDBID =
                    GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID).toInt();

    lineEdit_modifier_fournisseur_reference_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR));

    lineEdit_modifier_fournisseur_reference_registre_du_commerce->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE));

    lineEdit_modifier_fournisseur_nom_entreprise->setText(GET_SQL_RECORD_DATA
                                                          (record,
                                                           YerothDatabaseTableColumn::NOM_ENTREPRISE));

    lineEdit_modifier_fournisseur_nom_representant->setText(GET_SQL_RECORD_DATA
                                                            (record,
                                                             YerothDatabaseTableColumn::NOM_REPRESENTANT));

    lineEdit_modifier_fournisseur_quartier->setText(GET_SQL_RECORD_DATA
                                                    (record,
                                                     YerothDatabaseTableColumn::QUARTIER));

    lineEdit_modifier_fournisseur_ville->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerothDatabaseTableColumn::VILLE));

    lineEdit_modifier_fournisseur_province_etat->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerothDatabaseTableColumn::PROVINCE_ETAT));

    lineEdit_modifier_fournisseur_pays->setText(GET_SQL_RECORD_DATA
                                                (record,
                                                 YerothDatabaseTableColumn::PAYS));

    lineEdit_modifier_fournisseur_boite_postale->setText(GET_SQL_RECORD_DATA
                                                         (record,
                                                          YerothDatabaseTableColumn::BOITE_POSTALE));

    lineEdit_modifier_fournisseur_siege_social->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::SIEGE_SOCIAL));

    lineEdit_modifier_fournisseur_email->setText(GET_SQL_RECORD_DATA
                                                 (record,
                                                  YerothDatabaseTableColumn::EMAIL));

    if (true == show_EMPLOYE)
    {
    	lineEdit_employe_SUPERIEUR_hierarchique
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::MANAGER));

    	lineEdit_employe_DEPARTEMENT
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::DEPARTEMENT));

    	lineEdit_EMPLOYE_position_fonction
			->setText(GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::POSITION_FONCTION));
    }
    else
    {
    	lineEdit_employe_SUPERIEUR_hierarchique->clear();
    	lineEdit_employe_DEPARTEMENT->clear();
    	lineEdit_EMPLOYE_position_fonction->clear();
    }

    lineEdit_modifier_fournisseur_numero_telephone_1->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_1));

    lineEdit_modifier_fournisseur_numero_telephone_2->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_TELEPHONE_2));

    lineEdit_modifier_fournisseur_numero_contribuable->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE));

    textEdit_modifier_fournisseur_description_du_fournisseur->setText
    (GET_SQL_RECORD_DATA
     (record, YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR));

    QVariant img(record.value(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }
}
