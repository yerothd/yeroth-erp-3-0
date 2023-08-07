/*
 * yeroth-erp-creer-UN-GROUPE-DE-PAIE-hr-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-creer-GROUPE-DE-PAIE-hr-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlIndex>

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-historique-stock.hpp"



YerothCreerUnGroupeDePaieWindow::YerothCreerUnGroupeDePaieWindow()
:YerothWindowsCommons(),
 _logger(new YerothLogger("YerothCreerUnGroupeDePaieWindow"))
{
    _windowName = QString("%1 - %2")
    				.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                         QObject::tr("CRÉER 1 GROUPE DE PAIE"));

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);


//    _NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
//              << ;


    setup_select_configure_dbcolumn(YerothDatabase::GROUPES_DE_PAIE_hr );

    doubleSpinBox_creer_un_GroupeDePaie_pourcentage_TAXES_IMPOSABLES->setMinimum(0.0);

    doubleSpinBox_creer_un_GroupeDePaie_pourcentage_TAXES_IMPOSABLES->setMaximum(100.0);


    setupLineEdits();

    populateComboBoxes();


    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);


    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_FOURNISSEURS->disable(this);
    pushButton_enregistrer->disable(this);


    connect(lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL,
            SIGNAL(textChanged(const QString &)),
            this,
            SLOT(HANDLE_lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation(const QString &)));


    /** Menu actions */
    connect(actionChanger_utilisateur, SIGNAL(triggered()), this, SLOT(changer_utilisateur()));
    connect(actionAppeler_aide, SIGNAL(triggered()), this, SLOT(help()));
    connect(actionDeconnecter_utilisateur, SIGNAL(triggered()), this, SLOT(deconnecter_utilisateur()));
    connect(actionMenu_principal, SIGNAL(triggered()), this, SLOT(menu()));
    connect(actionFournisseurs, SIGNAL(triggered()), this, SLOT(fournisseurs()));
    connect(actionAnnuler, SIGNAL(triggered()), this, SLOT(annuler_la_creation_dun_GROUPE_DE_PAIE_hr()));
    connect(actionEnregistrer, SIGNAL(triggered()), this, SLOT(creerEnregistrerUnGroupeDePaieHr()));
    connect(actionFermeture, SIGNAL(triggered()), this, SLOT(fermeture()));
    connect(actionA_propos, SIGNAL(triggered()), this, SLOT(apropos()));
    connect(actionInformationEntreprise, SIGNAL(triggered()), this, SLOT(infosEntreprise()));
    connect(actionQui_suis_je, SIGNAL(triggered()), this, SLOT(qui_suis_je()));
    connect(actionAdministration, SIGNAL(triggered()), this, SLOT(administration()));



#ifdef YEROTH_CLIENT
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
#endif

    setupShortcuts();
}


void YerothCreerUnGroupeDePaieWindow::deconnecter_utilisateur()
{
    clear_all_fields();
    YerothWindowsCommons::deconnecter_utilisateur();
}


void YerothCreerUnGroupeDePaieWindow::updatePopulateComboBoxes()
{
    QStringList new_items;

    new_items.append(YerothUtils::EMPTY_STRING);

    new_items.append(YerothERPConfig::THIS_SITE_LOCALISATION_NAME);


    comboBox_creer_un_GroupeDePaie_localisation->populateComboBoxRawString
    (YerothDatabase::LOCALISATIONS,
     YerothDatabaseTableColumn::NOM_LOCALISATION);


    comboBox_creer_un_GroupeDePaie_localisation->addItem_AFTER_POPULATE
    (new_items);
}


void YerothCreerUnGroupeDePaieWindow::setupLineEdits()
{
    lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation
        ->setYerothEnabled(false);

    lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL
        ->setValidator(&YerothUtils::POSITIVE_DoubleValidator);
}


void YerothCreerUnGroupeDePaieWindow::setupShortcuts()
{
    setupShortcutActionMessageDaide(*actionAppeler_aide);
    setupShortcutActionQuiSuisJe(*actionQui_suis_je);
}


void YerothCreerUnGroupeDePaieWindow::definirPasDeRole()
{
    _logger->log("definirPasDeRole");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, false);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, false);

    pushButton_annuler->disable(this);
    pushButton_menu_principal->disable(this);
    pushButton_FOURNISSEURS->disable(this);
    pushButton_enregistrer->disable(this);

}


void YerothCreerUnGroupeDePaieWindow::definirManager()
{
    _logger->log("definirManager");

    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionInformationEntreprise, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionA_propos, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionChanger_utilisateur, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionDeconnecter_utilisateur,
                                           true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionMenu_principal, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFournisseurs, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAdministration, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAppeler_aide, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionQui_suis_je, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionAnnuler, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionEnregistrer, true);
    YEROTH_ERP_WRAPPER_QACTION_SET_ENABLED(actionFermeture, true);


    pushButton_annuler->enable(this,
                               SLOT
                               (annuler_la_creation_dun_GROUPE_DE_PAIE_hr
                                ()));
    pushButton_menu_principal->enable(this, SLOT(menu()));
    pushButton_FOURNISSEURS->enable(this, SLOT(fournisseurs()));
    pushButton_enregistrer->enable(this,
                                   SLOT
                                   (creerEnregistrerUnGroupeDePaieHr
                                    ()));
}


bool YerothCreerUnGroupeDePaieWindow::payGroupAlreadyExists()
{
    bool result = false;

    YerothSqlTableModel &GROUPE_DE_PAIE_HR_TableModel =
                    _allWindows->getSqlTableModel_groupes_de_paie_hr();

    // ** check if customer account with same name exist
    result = YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(*this,
                                                         GROUPE_DE_PAIE_HR_TableModel,
                                                         *lineEdit_creer_un_GroupeDePaie_designation,
                                                         QObject::tr("GROUPE DE PAIE hr"),
                                                         YerothDatabaseTableColumn::DESIGNATION);

    return result;
}


bool YerothCreerUnGroupeDePaieWindow::check_fields()
{
    bool result = false;

    bool GroupeDePaie_designation =
                        lineEdit_creer_un_GroupeDePaie_designation->checkField();

    bool GroupeDePaie_MONTANT_A_PAYER =
    		lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL->checkField();

    result = GroupeDePaie_designation &&
    		 GroupeDePaie_MONTANT_A_PAYER;

    return result;
}


void YerothCreerUnGroupeDePaieWindow::clear_all_fields()
{
    lineEdit_creer_un_GroupeDePaie_reference->clear();

    lineEdit_creer_un_GroupeDePaie_designation->clearField();

    lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL->clearField();

    textEdit_creer_un_GroupeDePaie_description->clear();

    doubleSpinBox_creer_un_GroupeDePaie_pourcentage_TAXES_IMPOSABLES->setValue(0.0);

    comboBox_creer_un_GroupeDePaie_localisation->resetYerothComboBox();
}


void YerothCreerUnGroupeDePaieWindow::rendreInvisible()
{
    clear_all_fields();
    setVisible(false);
}


void YerothCreerUnGroupeDePaieWindow::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
	retranslateUi(this);

    _curStocksTableModel = stocksTableModel;

    updatePopulateComboBoxes();

    check_fields();

    lineEdit_creer_un_GroupeDePaie_designation->setFocus();

    setVisible(true);

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("CRÉER 1 GROUPE DE PAIE"),
    								 _curStocksTableModel);
}


void YerothCreerUnGroupeDePaieWindow::annuler_la_creation_dun_GROUPE_DE_PAIE_hr()
{
    clear_all_fields();

    GROUPES_DE_PAIE_hr();
}


bool YerothCreerUnGroupeDePaieWindow::creerEnregistrerUnGroupeDePaieHr()
{
    if (!check_fields())
    {
        if (QMessageBox::Ok ==
                YerothQMessageBox::warning(this,
                						   "",
                                           QObject::tr("Remplisser tous les champs obligatoires !")))
        {
        }
        else
        {
        }

        return false;
    }

    if (payGroupAlreadyExists())
    {
        return false;
    }

    QString retMsg(QObject::tr("LE GROUPE DE PAIE '"));

    YerothSqlTableModel &groupe_de_paie_HR_ProgramTableModel =
                    _allWindows->getSqlTableModel_groupes_de_paie_hr();


    QSqlRecord record = groupe_de_paie_HR_ProgramTableModel.record();


    QString new_pay_group_designation = lineEdit_creer_un_GroupeDePaie_designation->text();

    int NOUVEAU_GROUPE_PAIE_HR_db_ID =
    		YerothERPWindows::getNextIdSqlTableModel_groupes_de_paie_hr();

    record.setValue(YerothDatabaseTableColumn::ID, NOUVEAU_GROUPE_PAIE_HR_db_ID);

    record.setValue(YerothDatabaseTableColumn::DATE_CREATION,
                    DATE_TO_DB_FORMAT_STRING(GET_CURRENT_DATE));

    record.setValue(YerothDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr,
    				lineEdit_creer_un_GroupeDePaie_reference->text());

    record.setValue(YerothDatabaseTableColumn::DESIGNATION,
                    new_pay_group_designation);

    if (!comboBox_creer_un_GroupeDePaie_localisation->isEmpty())
    {
        record.setValue(YerothDatabaseTableColumn::LOCALISATION,
                        comboBox_creer_un_GroupeDePaie_localisation->currentText());
    }

    record.setValue(YerothDatabaseTableColumn::DESCRIPTION_GROUPE_DE_PAIE_HR,
    				textEdit_creer_un_GroupeDePaie_description->toPlainText());

    record.setValue(YerothDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL,
    				lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL->text().toDouble());

    record.setValue(YerothDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES,
                    doubleSpinBox_creer_un_GroupeDePaie_pourcentage_TAXES_IMPOSABLES->value());


    retMsg.append(lineEdit_creer_un_GroupeDePaie_designation->text());

    bool success = groupe_de_paie_HR_ProgramTableModel
    					.insertNewRecord(record,
    									 this,
										 "src/windows/supplier/yeroth-erp-creer-UN-GROUPE-DE-PAIE-hr-window.cpp",
										 358,
										 new_pay_group_designation);

    if (!success)
    {
        retMsg.append(QObject::tr("' n'a pas pu être créer !"));

        YerothQMessageBox::warning(this,
                                   QObject::tr("CRÉER 1 GROUPE DE PAIE (hr)"),
                                   retMsg);
        return false;
    }

    groupe_de_paie_HR_ProgramTableModel.select();

    retMsg.append(QObject::tr("' a été créer avec succès !"));

    YerothQMessageBox::information(this,
                                   QObject::tr
                                   ("CRÉER 1 GROUPE DE PAIE (hr)"),
                                   retMsg);

    clear_all_fields();

    GROUPES_DE_PAIE_hr();

    return true;
}
