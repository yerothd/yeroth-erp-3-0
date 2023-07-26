/*
 * yeroth-erp-main-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-main-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-info-entreprise.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/dialogs/yeroth-erp-changer-utilisateur-dialog.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>



QString YerothMainWindow::STRING_MONDAY("lundi");

QString YerothMainWindow::STRING_TUESDAY("mardi");

QString YerothMainWindow::STRING_WEDNESDAY("mercredi");

QString YerothMainWindow::STRING_THURSDAY("jeudi");

QString YerothMainWindow::STRING_FRIDAY("vendredi");

QString YerothMainWindow::STRING_SATURDAY("samedi");

QString YerothMainWindow::STRING_SUNDAY("dimanche");




#ifdef YEROTH_FRANCAIS_LANGUAGE
	bool YerothMainWindow::LANGUE_ANGLAISE = false;
#endif

#ifdef YEROTH_ENGLISH_LANGUAGE
	bool YerothMainWindow::LANGUE_ANGLAISE = true;
#endif


YerothMainWindow::YerothMainWindow()
:YerothWindowsCommons(),
_logger(new YerothLogger("YerothMainWindow"))
{
    _windowName = QString("%1 - %2").arg(YerothMainWindow::GET_YEROTH_ERP_WINDOW_TITLE(),
                                         QObject::tr("fenêtre de l'utilisateur"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString
                    ("QMessageBox {messagebox-text-interaction-flags: 5; background-color: rgb(%1);}"
                     "QMessageBox QLabel {color: rgb(%2);} ").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    _curStocksTableModel = &_allWindows->getSqlTableModel_stocks();

    label_display_yeroth->setStyleSheet(YerothUtils::STRING_STYLESHEETS_YEROTH_ERP_3_0);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_acceder_aux_comptes_de_lentreprise, false);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);


    _main_window_button_TO_page_position.insert(0, std::make_tuple(193, 122));
    _main_window_button_TO_page_position.insert(1, std::make_tuple(300, 122));
    _main_window_button_TO_page_position.insert(2, std::make_tuple(407, 122));
    _main_window_button_TO_page_position.insert(3, std::make_tuple(514, 122));

    _main_window_button_TO_page_position.insert(4, std::make_tuple(193, 215));
    _main_window_button_TO_page_position.insert(5, std::make_tuple(300, 215));
    _main_window_button_TO_page_position.insert(6, std::make_tuple(407, 215));
    _main_window_button_TO_page_position.insert(7, std::make_tuple(514, 215));

    _main_window_button_TO_page_position.insert(8, std::make_tuple(193, 308));
    _main_window_button_TO_page_position.insert(9, std::make_tuple(300, 308));
    _main_window_button_TO_page_position.insert(10,
                                                std::make_tuple(407, 308));
    _main_window_button_TO_page_position.insert(11,
                                                std::make_tuple(514, 308));


    _main_window_buttons.push_back(pushButton_fournisseurs);
    _main_window_buttons.push_back(pushButton_charges_financieres);
    _main_window_buttons.push_back(pushButton_entrer);
    _main_window_buttons.push_back(pushButton_ventes);
    _main_window_buttons.push_back(pushButton_CRM_clients);
    _main_window_buttons.push_back(pushButton_afficher_stocks);
    _main_window_buttons.push_back(pushButton_marchandises_SERVICES);
    _main_window_buttons.push_back(pushButton_sortir);
    _main_window_buttons.push_back(pushButton_paiements);
    _main_window_buttons.push_back(pushButton_rapports);
    _main_window_buttons.push_back(pushButton_mouvementsDeStocks);
    _main_window_buttons.push_back(pushButton_vendre);


    connect(action_TRADUIRE_EN_ANGLAIS,
    		SIGNAL(triggered()),
			this,
            SLOT(CHANGER_DE_LANGUE()));

    connect(actionChanger_utilisateur, SIGNAL(triggered()), this,
            SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionGuide_PRATIQUE_DE_LUTILISATEUR, SIGNAL(triggered()), this,
            SLOT(getManuelUtilisateurPDF()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this,
            SLOT(deconnecter_utilisateur()));
    connect(actionAdministration, SIGNAL(triggered()), this,
            SLOT(administration()));
    connect(actionEntrer, SIGNAL(triggered()), this, SLOT(entrer()));
    connect(actionStocks, SIGNAL(triggered()), this, SLOT(stocks()));
    connect(actionMarchandises, SIGNAL(triggered()), this,
            SLOT(afficherMarchandises()));
    connect(actionCHARGES_FINANCIERES, SIGNAL(triggered()), this,
            SLOT(charges_financieres()));
    connect(actionSortir, SIGNAL(triggered()), this, SLOT(sortir()));
    connect(actionTableauxDeBords, SIGNAL(triggered()), this,
            SLOT(tableaux_de_bords()));
    connect(actionVente, SIGNAL(triggered()), this, SLOT(ventes()));
    connect(actionVendre, SIGNAL(triggered()), this, SLOT(vendre()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionPaiements, SIGNAL(triggered()), this, SLOT(paiements()));
    connect(actionClients, SIGNAL(triggered()), this, SLOT(clients()));
    connect(actionMouvementsDeStocks, SIGNAL(triggered()), this,
            SLOT(mouvements_de_stocks()));
    connect(actionFournisseurs, SIGNAL(triggered()), this,
            SLOT(fournisseurs()));
    connect(actionAchatsStocks, SIGNAL(triggered()), this,
            SLOT(achats_de_stocks()));
    connect(actionAlertes, SIGNAL(triggered()), this, SLOT(courrier()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this,
            SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this,
            SLOT(qui_suis_je()));

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    connect(action_acceder_aux_comptes_de_lentreprise, SIGNAL(triggered()),
            this, SLOT(comptabilite()));
#endif

    setupShortcuts();
}


void YerothMainWindow::CHANGER_DE_LANGUE__text__TRANSLATION()
{
    STRING_MONDAY = QObject::tr("Lundi");

    STRING_TUESDAY = QObject::tr("Mardi");

    STRING_WEDNESDAY = QObject::tr("Mercredi");

    STRING_THURSDAY = QObject::tr("Jeudi");

    STRING_FRIDAY = QObject::tr("Vendredi");

    STRING_SATURDAY = QObject::tr("Samedi");

    STRING_SUNDAY = QObject::tr("Dimanche");


    retranslateUi(this);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        action_TRADUIRE_EN_ANGLAIS->setText("TRADUIRE EN FRANÇAIS");
    }
    else
    {
        action_TRADUIRE_EN_ANGLAIS->setText("TRANSLATE INTO ENGLISH");
    }
}


void YerothMainWindow::calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW()
{
//      QDEBUG_STRING_OUTPUT_1("calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW");

    YerothPushButton *next_to_draw_push_button = 0;

    uint next_position_to_draw = 0;

    for (uint k = 0; k < _main_window_buttons.size(); ++k)
    {
        next_to_draw_push_button = _main_window_buttons.at(k);

        if (0 != next_to_draw_push_button)
        {
            if (next_to_draw_push_button->isAuthorizedForCurrentUser())
            {
//                              QDEBUG_STRING_OUTPUT_2(next_to_draw_push_button->text(),
//                                                                         QString("(%1, %2)")
//                                                                                      .arg(QString::number(std::get<0>(_main_window_button_TO_page_position.value(next_position_to_draw))),
//                                                                                               QString::number(std::get<1>(_main_window_button_TO_page_position.value(next_position_to_draw)))));

                next_to_draw_push_button->setGeometry(std::get < 0 >
                                                      (_main_window_button_TO_page_position.
                                                       value
                                                       (next_position_to_draw)),
                                                      std::get < 1 >
                                                      (_main_window_button_TO_page_position.
                                                       value
                                                       (next_position_to_draw)),
                                                      next_to_draw_push_button->
                                                      width(),
                                                      next_to_draw_push_button->
                                                      height());

                ++next_position_to_draw;
            }
        }
    }
}


void YerothMainWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothMainWindow::definirCaissier()
{
    _logger->log("definirCaissier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, true);
#endif

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

#endif

    pushButton_fournisseurs->enable(this, SLOT(fournisseurs()));
    pushButton_CRM_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->enable(this,
                                           SLOT(charges_financieres()));
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_rapports->enable(this, SLOT(tableaux_de_bords()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}


void YerothMainWindow::definirVendeur()
{
    _logger->log("definirVendeur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->enable(this, SLOT(clients()));
    pushButton_paiements->enable(this, SLOT(paiements()));
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->enable(this, SLOT(ventes()));
    pushButton_vendre->enable(this, SLOT(vendre()));

    label_display_yeroth->setVisible(false);
}


void YerothMainWindow::definirGestionaireDesStocks()
{
    _logger->log("definirGestionaireDesStocks");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#endif

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->enable(this, SLOT(entrer()));
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_marchandises_SERVICES->enable(this,
                                             SLOT(afficherMarchandises()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirAdministrateur()
{

#ifdef YEROTH_CLIENT

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

#else //YEROTH_SERVER, YEROTH_STANDALONE, YEROTH_ACADEMIC_EVALUSATION_VERSION

    _logger->log("definirAdministrateur");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    _allWindows->_adminWindow->definirAdministrateur();

#endif

    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirMagasinier()
{
    _logger->log("definirMagasinier");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, true);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_rapports->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->enable(this, SLOT(stocks()));
    pushButton_sortir->enable(this, SLOT(sortir()));
    pushButton_mouvementsDeStocks->enable(this,
                                          SLOT(mouvements_de_stocks()));
    label_display_yeroth->setVisible(false);
}

void YerothMainWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(action_TRADUIRE_EN_ANGLAIS, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEntrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionPaiements, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVente, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMouvementsDeStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionClients, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (action_acceder_aux_comptes_de_lentreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionCHARGES_FINANCIERES, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMarchandises, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionSortir, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionTableauxDeBords, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionVendre, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAchatsStocks, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAlertes, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED
    (actionGuide_PRATIQUE_DE_LUTILISATEUR, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);

    pushButton_fournisseurs->disable(this);
    pushButton_CRM_clients->disable(this);
    pushButton_paiements->disable(this);
    pushButton_charges_financieres->disable(this);
    pushButton_entrer->disable(this);
    pushButton_afficher_stocks->disable(this);
    pushButton_marchandises_SERVICES->disable(this);
    pushButton_sortir->disable(this);
    pushButton_rapports->disable(this);
    pushButton_mouvementsDeStocks->disable(this);
    pushButton_ventes->disable(this);
    pushButton_vendre->disable(this);

    label_display_yeroth->setVisible(true);
}


void YerothMainWindow::setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY()
{
    YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY =
                    QString("database ip address: \"%1\" - "
                            "database table: \"%2\" - "
                            "database options: \"%3\"").arg(_allWindows->getDatabase().
                                                            db_ip_address(),
                                                            _allWindows->getDatabase().
                                                            db_name(),
                                                            _allWindows->getDatabase().
                                                            db_connection_options());
}


void YerothMainWindow::write_AFTER_SETUP_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY()
{
    setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    if (_allWindows->getDatabase().isOpen())
    {
        label_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
							 .arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));

        _allWindows->_adminWindow
			->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));

        _allWindows->_pdVenteWindow
			->label_PDV_display_yeroth_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255));
    }
    else
    {
        label_display_yeroth_erp_3_0_server_parameters
			->setStyleSheet(QString("color: rgb(%1);")
							 .arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));

        _allWindows->_adminWindow
			->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));

        _allWindows->_pdVenteWindow
        	->label_PDV_display_yeroth_erp_3_0_server_parameters
				->setStyleSheet(QString("color: rgb(%1);")
								 .arg(COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48));
    }


    label_display_yeroth_erp_3_0_server_parameters
		->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

    _allWindows->_adminWindow
		->label_ADMIN_WINDOW_display_yeroth_erp_3_0_server_parameters
			->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);

    _allWindows->_pdVenteWindow
		->label_PDV_display_yeroth_erp_3_0_server_parameters
			->setText(YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY);
}


void YerothMainWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	CHANGER_DE_LANGUE__text__TRANSLATION();

    calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW();

    YerothWindowsCommons::rendreVisible(stocksTableModel);

    rendreVisibleLocalTOCLASS(_allWindows->getDatabase().isOpen());
}


void YerothMainWindow::rendreVisibleLocalTOCLASS(bool isDatabaseOpened /* = false */)
{
    retranslateUi(this);

	CHANGER_DE_LANGUE__text__TRANSLATION();

	write_AFTER_SETUP_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    show();
}


void YerothMainWindow::CHANGER_DE_LANGUE()
{
	if (0 == YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION)
	{
		return ;
	}

	if (!YerothMainWindow::LANGUE_ANGLAISE)
	{
		//QDEBUG_STRING_OUTPUT_1("YerothERPWindows::_ENGLISH_TRANSLATOR");

		YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION
			->installTranslator(YerothERPWindows::_ENGLISH_TRANSLATOR);

		YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION
			->removeTranslator(YerothERPWindows::_FRENCH_TRANSLATOR);


	    YerothMainWindow::LANGUE_ANGLAISE = true;
	}
	else
	{
		//QDEBUG_STRING_OUTPUT_1("YerothERPWindows::_FRENCH_TRANSLATOR");

		YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION
			->installTranslator(YerothERPWindows::_FRENCH_TRANSLATOR);

		YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION
			->removeTranslator(YerothERPWindows::_ENGLISH_TRANSLATOR);

	    YerothMainWindow::LANGUE_ANGLAISE = false;
	}

	retranslateUi(this);

	YerothDatabaseTableColumn::RETRANSLATE();

	CHANGER_DE_LANGUE__text__TRANSLATION();

	write_AFTER_SETUP_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();
}


void YerothMainWindow::help()
{
//#define YEROTH_DEVEL_TARGET

#ifndef YEROTH_DEVEL_TARGET
    YerothPOSUser *currentUser = _allWindows->getUser();

    if (0 != currentUser && !currentUser->isPasDeRole())
    {
        YerothQMessageBox::information(this, QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
                                        "2) Déconnectez vous en cliquant sur le deuxième icône !\n\n"
                                        "3) Lorsque vous êtes enregistré, choisissez une opération "
                                        "à réaliser en cliquant "
                                        "sur le bouton correspondant !"));
    }
    else
    {
        YerothQMessageBox::information(this, QObject::tr("aide"),
                                       QObject::tr
                                       ("1) Veuillez vous enregistrer en cliquant sur le premier icône !\n\n"
                                        "2) Recevez un message d'aide en cliquant sur le deuxieme icone !"));
    }
#else

    QDEBUG_STRING_OUTPUT_2
    ("YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER",
     YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

    YerothUtils::YEROTH_CREATE_FOLDER
    (YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);

    YerothERPConfig::THIS_SITE_LOCALISATION_NAME = "YEROTH_RD_TEST";

    _allWindows->_adminWindow->start_TESTING_MAINTENANCE();

    //YerothUserLAdministrateur *u = new YerothUserLAdministrateur(_allWindows);
    //YerothUserLeCaissier *u = new YerothUserLeCaissier(_allWindows);
    //YerothUserLeMagasinier *u = new YerothUserLeMagasinier(_allWindows);

    YerothERPConfig::tva_value = 7.25 / 100.0;

    YerothERPConfig::currency = "FCFA";

    YerothERPConfig::receiptFormat =
                    YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER;

    YerothERPConfig::printer = YerothUtils::IMPRIMANTE_PDF;

    YerothPOSUserManager *u = new YerothPOSUserManager(_allWindows);
    u->set_titre("Prof.");
    u->set_prenom("Xavier");
    u->set_nom("Noumbissi Noundou");
    u->set_nom_utilisateur("yeroth-nissi");
    u->set_localisation("Yaoundé");
    u->set_nom_complet("DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU");

    _allWindows->setUser(u);

#endif
}


void YerothMainWindow::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();

    YerothPOSUser *currentUser = _allWindows->getUser();

    if (0 != currentUser)
    {
        currentUser->setRole(YerothUtils::ROLE_INDEFINI);
    }
}


void YerothMainWindow::cleanupAllOtherWindows()
{
    if (0 != _allWindows->getUser())
    {
        definirPasDeRole();
        _allWindows->hideAllWindows();
        show();
    }
}
