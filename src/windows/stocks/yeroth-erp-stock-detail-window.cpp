/*
 * yeroth-erp-stock-detail-window.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "src/yeroth-erp-windows.hpp"


#include "yeroth-erp-stock-detail-window.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>


YerothStockDetailWindow::YerothStockDetailWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothDetailWindow"))
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("détails d'un stock");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


    checkBox_HISTORIQUE->setReadOnly(true);

    checkBox_service->setReadOnly(true);

    checkBox_achat->setReadOnly(true);

    setupLineEdits();

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    dateEdit_date_dentree_stock->setYerothEnabled(false);
    dateEdit_date_peremption->setYerothEnabled(false);
    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
    pushButton_retour->enable(this, SLOT(stocks()));

    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionMenu, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionModifier, SIGNAL(triggered()), this,
            SLOT(modifier_les_articles()));
    connect(actionSupprimer, SIGNAL(triggered()), this,
            SLOT(supprimer_ce_stock()));
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

void YerothStockDetailWindow::setupLineEdits()
{
    lineEdit_localisation_produit->setYerothEnabled(false);
    lineEdit_reference_produit->setYerothEnabled(false);
    lineEdit_designation->setYerothEnabled(false);
    lineEdit_quantite_restante->setYerothEnabled(false);
    lineEdit_reference_recu_dachat->setYerothEnabled(false);
    lineEdit_prix_dachat->setYerothEnabled(false);
    lineEdit_prix_unitaire->setYerothEnabled(false);
    lineEdit_prix_vente->setYerothEnabled(false);
    textEdit_description->setYerothEnabled(false);
    lineEdit_quantite_initiale->setYerothEnabled(false);
    lineEdit_stock_dalerte->setYerothEnabled(false);
    lineEdit_categorie_produit->setYerothEnabled(false);
    lineEdit_nom_entreprise_fournisseur->setYerothEnabled(false);
}

void YerothStockDetailWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.setPalette(toolBar_detailWindow->palette());
    menu.addAction(actionSupprimer);
    menu.exec(event->globalPos());
}

void YerothStockDetailWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_menu->disable(this);
    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->disable(this);
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, true);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_modifier->enable(this, SLOT(modifier_les_articles()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->enable(this, SLOT(supprimer_ce_stock()));
}


void YerothStockDetailWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}


void YerothStockDetailWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);

#ifdef YEROTH_SERVER
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_modifier->enable(this, SLOT(modifier_les_articles()));
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}

void YerothStockDetailWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionModifier, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSupprimer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_entrer->disable(this);
    pushButton_modifier->disable(this);
    pushButton_menu->enable(this, SLOT(menu()));
    pushButton_supprimer->disable(this);
}

void YerothStockDetailWindow::modifier_les_articles()
{
    rendreInvisible();
    _allWindows->_modifierWindow->rendreVisible(_curStocksTableModel);
}

void YerothStockDetailWindow::rendreInvisible()
{
    checkBox_achat->setChecked(true);
    lineEdit_reference_produit->clear();
    lineEdit_designation->clear();
    lineEdit_categorie_produit->clear();
    lineEdit_quantite_restante->clear();
    lineEdit_nom_entreprise_fournisseur->clear();
    lineEdit_prix_dachat->clear();
    lineEdit_prix_unitaire->clear();
    lineEdit_prix_vente->clear();
    textEdit_description->clear();
    lineEdit_localisation_produit->clear();
    label_image_produit->clear();

    YerothWindowsCommons::rendreInvisible();
}


void YerothStockDetailWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;
    //qDebug() << "++ last selected row: " << YerothERPWindows::get_last_lister_selected_row_db_ID();

    setDetailViewFilter(_curStocksTableModel);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("détails d'un stock"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerothStockDetailWindow::setStockAchatValuesVisibility(bool aVisibility)
{
    if (aVisibility)
    {
        lineEdit_quantite_restante->setFixedWidth(104);
    }
    else
    {
        lineEdit_quantite_restante->setFixedWidth(205);
    }

    checkBox_achat->setVisible(aVisibility);

    checkBox_achat->setChecked(aVisibility);

    label_prix_dachat->setVisible(aVisibility);

    lineEdit_prix_dachat->setVisible(aVisibility);

    label_reference_recu_dachat->setVisible(aVisibility);

    lineEdit_reference_recu_dachat->setVisible(aVisibility);
}


void YerothStockDetailWindow::setStockSpecificWidgetVisibility(bool aVisibility)
{
    if (aVisibility)
    {
        lineEdit_quantite_restante->setFixedWidth(104);
    }
    else
    {
        lineEdit_quantite_restante->setFixedWidth(205);
    }

    label_reference_recu_dachat->setVisible(aVisibility);
    lineEdit_reference_recu_dachat->setVisible(aVisibility);

    label_stock_dalerte->setVisible(aVisibility);
    lineEdit_stock_dalerte->setVisible(aVisibility);

    label_localisation_du_stock->setVisible(aVisibility);
    lineEdit_localisation_produit->setVisible(aVisibility);


    dateEdit_date_dentree_stock->setVisible(aVisibility);
    dateEdit_date_peremption->setVisible(aVisibility);
    label_date_peremption->setVisible(aVisibility);

    label_prix_dachat->setVisible(aVisibility);
    lineEdit_prix_dachat->setVisible(aVisibility);

    checkBox_achat->setVisible(aVisibility);
}


void
YerothStockDetailWindow::showItem_YerothERPAbstractClassDetailViewFOR_WINDOW
()
{
    _curStocksTableModel->yerothSetFilter_WITH_where_clause(QString
                                                            ("%1 = '%2'").arg
                                                            (YerothDatabaseTableColumn::ID,
                                                             YerothERPWindows::
                                                             get_last_lister_selected_row_db_ID
                                                             ()));

    QSqlRecord
    record = _curStocksTableModel->record(0);

    bool
    is_service = GET_SQL_RECORD_DATA(record,
                                     YerothDatabaseTableColumn::IS_SERVICE).
                 toInt();

    checkBox_service->setChecked(is_service);

    if (is_service)
    {
        setStockSpecificWidgetVisibility(false);

        checkBox_service->setVisible(true);

        label_fournisseur->setText(QObject::tr("client"));
        lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT));
    }
    else
    {
        setStockSpecificWidgetVisibility(true);

        checkBox_service->setVisible(false);

        bool
        historique_stock_actif = GET_SQL_RECORD_DATA(record,
                                                     YerothDatabaseTableColumn::
                                                     HISTORIQUE_STOCK_ACTIF).toInt
                                 ();


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


        label_fournisseur->setText(QObject::tr("fournisseur"));
        lineEdit_nom_entreprise_fournisseur->setText(GET_SQL_RECORD_DATA
                                                     (record,
                                                      YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR));
    }

    lineEdit_reference_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::REFERENCE));

    lineEdit_designation->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerothDatabaseTableColumn::DESIGNATION));

    lineEdit_categorie_produit->setText(GET_SQL_RECORD_DATA
                                        (record,
                                         YerothDatabaseTableColumn::CATEGORIE));

    double
    prix_unitaire = GET_SQL_RECORD_DATA(record,
                                        YerothDatabaseTableColumn::PRIX_UNITAIRE).
                    toDouble();

    lineEdit_prix_unitaire->setText(GET_CURRENCY_STRING_NUM(prix_unitaire));

    double
    prix_dachat = 0.0;

    setStockAchatValuesVisibility(false);

    QString
    recordID = GET_SQL_RECORD_DATA(record, YerothDatabaseTableColumn::ID);

    int
    achatRecordSize = YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(recordID);

    YerothPOSUser *
    currentUser = YerothUtils::getAllWindows()->getUser();

    if (0 != currentUser)
    {
        if (currentUser->isManager() || currentUser->isGestionaireDesStocks())
        {
            prix_dachat =
                            GET_SQL_RECORD_DATA(record,
                                                YerothDatabaseTableColumn::PRIX_DACHAT).
                            toDouble();

            if (achatRecordSize > 0)
            {
                setStockAchatValuesVisibility(true);

                lineEdit_prix_dachat->setText(GET_CURRENCY_STRING_NUM
                                              (prix_dachat));

                lineEdit_reference_recu_dachat->setText(GET_SQL_RECORD_DATA
                                                        (record,
                                                         YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT));
            }
        }
    }

    double
    prix_vente = GET_SQL_RECORD_DATA(record,
                                     YerothDatabaseTableColumn::PRIX_VENTE).
                 toDouble();

    lineEdit_prix_vente->setText(GET_CURRENCY_STRING_NUM(prix_vente));

    double
    prix_vente_en_gros = GET_SQL_RECORD_DATA(record,
                                             YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS).
                         toDouble();

    lineEdit_prix_vente_en_gros->setText(GET_CURRENCY_STRING_NUM
                                         (prix_vente_en_gros));

    lineEdit_localisation_produit->setText(GET_SQL_RECORD_DATA
                                           (record,
                                            YerothDatabaseTableColumn::LOCALISATION_STOCK));

    textEdit_description->setText(GET_SQL_RECORD_DATA
                                  (record,
                                   YerothDatabaseTableColumn::DESCRIPTION_PRODUIT));


    double
    quantite_restante = GET_SQL_RECORD_DATA(record,
                                            YerothDatabaseTableColumn::QUANTITE_TOTALE).
                        toDouble();

    lineEdit_quantite_restante->setText(GET_DOUBLE_STRING_P
                                        (quantite_restante, 2));

    double
    stock_dalerte = GET_SQL_RECORD_DATA(record,
                                        YerothDatabaseTableColumn::STOCK_DALERTE).
                    toDouble();

    lineEdit_stock_dalerte->setText(GET_DOUBLE_STRING_P(stock_dalerte, 2));

    dateEdit_date_dentree_stock->setDate(record.
                                         value(YerothDatabaseTableColumn::
                                               DATE_ENTREE).toDate());

    dateEdit_date_peremption->setDate(record.
                                      value(YerothDatabaseTableColumn::
                                            DATE_PEREMPTION).toDate());

    double
    lots_entrant = GET_SQL_RECORD_DATA(record,
                                       YerothDatabaseTableColumn::LOTS_ENTRANT).
                   toDouble();

    double
    quantite_par_lot = GET_SQL_RECORD_DATA(record,
                                           YerothDatabaseTableColumn::QUANTITE_PAR_LOT).
                       toDouble();

    double
    quantite_initiale = lots_entrant * quantite_par_lot;

    if (quantite_initiale < quantite_restante)
    {
        //Cas d'un stock vendu et returne en stock
        lineEdit_quantite_initiale->setText(GET_DOUBLE_STRING_P
                                            (quantite_restante, 2));
    }
    else
    {
        lineEdit_quantite_initiale->setText(GET_DOUBLE_STRING_P
                                            (quantite_initiale, 2));
    }

    QVariant
    img(record.value(YerothDatabaseTableColumn::IMAGE_PRODUIT));

    if (!img.isNull())
    {
        YerothUtils::loadPixmapFromDB(*label_image_produit, img, "JPG");
    }
    else
    {
        label_image_produit->setAutoFillBackground(false);
    }

    resetDetailViewFilter(_curStocksTableModel);
}


void YerothStockDetailWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothStockDetailWindow::supprimer_ce_stock()
{
    _curStocksTableModel->yerothSetFilter_WITH_where_clause(QString
                                                            ("%1 = '%2'").arg
                                                            (YerothDatabaseTableColumn::ID,
                                                             YerothERPWindows::
                                                             get_last_lister_selected_row_db_ID
                                                             ()));

    QSqlRecord record = _curStocksTableModel->record(0);

    QString msgSupprimer(QObject::tr
                         ("Poursuivre avec la suppression de l'article '%1' ?").
                         arg(GET_SQL_RECORD_DATA
                             (record,
                              YerothDatabaseTableColumn::DESIGNATION)));

    if (QMessageBox::Ok ==
            YerothQMessageBox::question(this,
                                        QObject::tr("suppression d'un article"),
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
            msgSupprimer =
                            QObject::tr("Le stock '%1' a été supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::DESIGNATION));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("supprimer - succès"),
                                           msgSupprimer);
        }
        else
        {
            msgSupprimer =
                            QObject::tr
                            ("Le stock '%1' ne pouvait pas être supprimé !").arg
                            (GET_SQL_RECORD_DATA
                             (record, YerothDatabaseTableColumn::DESIGNATION));

            YerothQMessageBox::information(this,
                                           QObject::tr
                                           ("supprimer - échec"),
                                           msgSupprimer);
        }
    }
    else
    {
    }

    resetDetailViewFilter(_curStocksTableModel);

//    _curStocksTableModel->setFilter(getPreviousBEFORETHISWindowsEntering_SQL_TABLE_Filter());
}
