/*
 * yeroth-erp-3-0.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */


#ifdef YEROTH_SERVER

	#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"

#endif /* YEROTH_SERVER */

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QStringList>

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/utils/yeroth-erp-command-line-parser.hpp"

#include "src/utils/virtual-keyboard/yeroth-erp-key-emitter.hpp"

#include "src/yeroth-erp-windows.hpp"


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

#include "src/tests/utils/yeroth-erp-test-yeroth-erp-utils.hpp"

#include "src/tests/yeroth-erp-test-yeroth-erp-windows.hpp"

#include "src/tests/yeroth-erp-test-yeroth-erp-table-view.hpp"

#endif


#include "src/utils/yeroth-erp-command-line-parser.hpp"
#include "src/utils/yeroth-erp-config.hpp"
#include "src/utils/yeroth-erp-utils.hpp"
#include "src/utils/yeroth-erp-database.hpp"
#include "src/utils/yeroth-erp-logger.hpp"
#include "src/process/yeroth-erp-process-info.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <QtCore/QTranslator>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QCryptographicHash>
#include <QtCore/QLocale>

#include <QtCore/QTranslator>

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QStylePlugin>

#include <QtNetwork/QNetworkInterface>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>

#include <cstdlib>

#ifdef YEROTH_CHECK_MEMORY
	#include <mcheck.h>
#endif

class YerothMain
{
public:

    inline YerothMain(YerothLogger *logger)
    {
        _logger = logger;
    }

    inline ~YerothMain()
    {
    }

private:

    YerothLogger *_logger;
};


void readTexTemplateFiles(YerothLogger &logger)
{
    logger.log("main", "readTexTemplateFiles");


    {
    	// FR_template-lister-admin-objects.tex
    	QFile file_FR_template_bon_de_commande(FILE_YEROTH_ERP_3_0_TEMPLATE_PURCHASE_ORDER_SHEET_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_FR_template_bon_de_commande,
    			YerothUtils::FR_template_bon_de_commande_TEX);

    	// EN_template-lister-admin-objects.tex
    	QFile file_EN_template_bon_de_commande(FILE_YEROTH_ERP_3_0_TEMPLATE_PURCHASE_ORDER_SHEET_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_EN_template_bon_de_commande,
    			YerothUtils::EN_template_bon_de_commande_TEX);
    }

    {
    	// FR_template-lister-admin-objects.tex
    	QFile file_FR_template_lister_admin_objects(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ADMIN_OBJECTS_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_FR_template_lister_admin_objects,
    			YerothUtils::FR_template_lister_admin_objects_TEX);

    	// EN_template-lister-admin-objects.tex
    	QFile file_EN_template_lister_admin_objects(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ADMIN_OBJECTS_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_EN_template_lister_admin_objects,
    			YerothUtils::EN_template_lister_admin_objects_TEX);
    }

    {
    	// FR_template_employee_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_GroupTemplateFR(FILE_YEROTH_ERP_3_0_TEMPLATE_EMPLOYEES_GROUPS_HR_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEMPLOYEE_GroupTemplateFR,
    			YerothUtils::FR_template_EMPLOYEE_GROUP_LISTING_tex);

    	// EN_template_employee_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_GroupTemplateEN(FILE_YEROTH_ERP_3_0_TEMPLATE_EMPLOYEES_GROUPS_HR_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEMPLOYEE_GroupTemplateEN,
    			YerothUtils::EN_template_EMPLOYEE_GROUP_LISTING_tex);
    }

    {
    	// FR_template_employee_PAY_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_pay_GroupTemplateFR(FILE_YEROTH_ERP_3_0_TEMPLATE_PAY_GROUP_HR_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEMPLOYEE_pay_GroupTemplateFR,
    			YerothUtils::FR_template_employee_PAY_GROUP_LISTING_tex);

    	// EN_template_employee_PAY_GROUP_LISTING_tex
    	QFile fileEMPLOYEE_pay_GroupTemplateEN(FILE_YEROTH_ERP_3_0_TEMPLATE_PAY_GROUP_HR_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEMPLOYEE_pay_GroupTemplateEN,
    			YerothUtils::EN_template_employee_PAY_GROUP_LISTING_tex);
    }

    {
    	// FR_template_GROUPES_DUN_CLIENT_tex
    	QFile fileCLIENTGroupTemplateFR(FILE_YEROTH_ERP_3_0_TEMPLATE_CLIENT_GROUP_HR_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCLIENTGroupTemplateFR,
    			YerothUtils::FR_template_GROUPES_DUN_CLIENT_tex);

    	// EN_template_GROUPES_DUN_CLIENT_tex
    	QFile fileCLIENTGroupTemplateEN(FILE_YEROTH_ERP_3_0_TEMPLATE_CLIENT_GROUP_HR_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCLIENTGroupTemplateEN,
    			YerothUtils::EN_template_GROUPES_DUN_CLIENT_tex);
    }

    {
    	// FR_template_GROUPES_DUN_EMPLOYE_tex
    	QFile fileEmployeeGroupTemplateFR(FILE_YEROTH_ERP_3_0_TEMPLATE_EMPLOYEE_GROUP_HR_FR);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEmployeeGroupTemplateFR,
    			YerothUtils::FR_template_GROUPES_DUN_EMPLOYE_tex);

    	// EN_template_GROUPES_DUN_EMPLOYE_tex
    	QFile fileEmployeeGroupTemplateEN(FILE_YEROTH_ERP_3_0_TEMPLATE_EMPLOYEE_GROUP_HR_EN);

    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileEmployeeGroupTemplateEN,
    			YerothUtils::EN_template_GROUPES_DUN_EMPLOYE_tex);
    }

    {
    	QFile fileFinancialAccountingReportDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileFinancialAccountingReportDataTemplate,
    										  YerothUtils::FR_template_bilan_comptable_tex);

    	QFile fileFinancialAccountingReportDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_FINANCIAL_ACCOUNTING_REPORT_DATA_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileFinancialAccountingReportDataENTemplate,
    										  YerothUtils::EN_template_bilan_comptable_tex);
    }

    {
    	QFile fileClientLOYALTY_GROUP_LISTING_FR(FILE_YEROTH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTY_GROUP_LISTING_FR,
    										  YerothUtils::FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex);

    	QFile fileClientLOYALTY_GROUP_LISTING_EN(FILE_YEROTH_CLIENT_LOYALTY_GROUPS_LISTING_TEMPLATE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTY_GROUP_LISTING_EN,
    										  YerothUtils::EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex);
    }

    {
    	QFile fileClientLOYALTYprogramLISTING_FR(FILE_YEROTH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTYprogramLISTING_FR,
    										  YerothUtils::FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);

    	QFile fileClientLOYALTYprogramLISTING_EN(FILE_YEROTH_CLIENT_LOYALTY_PROGRAM_LISTING_TEMPLATE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTYprogramLISTING_EN,
    										  YerothUtils::EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex);
    }

    {
    	QFile fileClientLOYALTYprogramTemplateFR(FILE_YEROTH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTYprogramTemplateFR,
    										   YerothUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex);

    	QFile fileClientLOYALTYprogramTemplateEN(FILE_YEROTH_LOYALTY_MEMBERSHIP_CARD_TEMPLATE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileClientLOYALTYprogramTemplateEN,
    										  YerothUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex);
    }

    {
    	QFile fileSupplierDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierDataTemplate,
    										  YerothUtils::FR_template_fiche_dun_fournisseur_tex);

    	QFile fileSupplierDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_SUPPLIER_DATA_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierDataENTemplate,
    										  YerothUtils::EN_template_fiche_dun_fournisseur_tex);
    }

    {
    	QFile fileCustomerDataTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerDataTemplate,
    										  YerothUtils::FR_template_fiche_client_tex);

    	QFile fileCustomerDataENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_CUSTOMER_DATA_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerDataENTemplate,
    										  YerothUtils::EN_template_fiche_client_tex);
    }

    {
    	QFile barDiagTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_DIAG_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(barDiagTemplate,
    										  YerothUtils::FR_bar_diag_tex);

    	QFile barDiagENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_DIAG_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(barDiagENTemplate,
    										  YerothUtils::EN_bar_diag_tex);
    }

    {
    	QFile barChartTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_CHART_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(barChartTemplate,
    										  YerothUtils::FR_bar_chart_tex);

    	QFile barChartENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_BAR_CHART_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(barChartENTemplate,
    										  YerothUtils::EN_bar_chart_tex);
    }

    {
    	QFile ZERO_ventes_Template(FILE_YEROTH_ERP_3_0_TEMPLATE_ZERO_VENTES_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(ZERO_ventes_Template,
    										  YerothUtils::FR_ZERO_ventes_tex);

    	QFile ZERO_ventes_ENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_ZERO_VENTES_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(ZERO_ventes_ENTemplate,
    										  YerothUtils::EN_ZERO_ventes_tex);
    }

    {
    	QFile pieChartTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_PIE_CHART_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(pieChartTemplate,
    										  YerothUtils::FR_pie_chart_tex);

    	QFile pieChartENTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_PIE_CHART_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(pieChartENTemplate,
    										  YerothUtils::EN_pie_chart_tex);
    }

    {
    	QFile fileFactureSmall_fr(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileFactureSmall_fr,
    										  YerothUtils::FR_template_facture_petit_tex);

    	QFile fileFactureSmall_en(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_SMALL_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileFactureSmall_en,
    										  YerothUtils::EN_template_facture_petit_tex);
    }

    {
    	QFile file_FACTURE_grand_Fr(FILE_YEROTH_ERP_3_0_TEMPLATE_INVOICE_LARGE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_FACTURE_grand_Fr,
    										  YerothUtils::FR_template_FACTURE_GRAND_tex);

    	QFile file_FACTURE_grand_EN(FILE_YEROTH_ERP_3_0_TEMPLATE_INVOICE_LARGE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_FACTURE_grand_EN,
    										  YerothUtils::EN_template_FACTURE_GRAND_tex);
    }

    {
    	QFile file_three_D_bardiag_Fr(FILE_YEROTH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_three_D_bardiag_Fr,
    										  YerothUtils::FR_template_THREE_D_bardiag_tex);

    	QFile file_three_D_bardiag_En(FILE_YEROTH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_three_D_bardiag_En,
    										  YerothUtils::EN_template_THREE_D_bardiag_tex);
    }

    {
    	QFile file_RECU_grand_Fr(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_RECU_grand_Fr,
    										  YerothUtils::FR_template_RECU_GRAND_tex);

    	QFile file_RECU_grand_En(FILE_YEROTH_ERP_3_0_TEMPLATE_RECEIPT_LARGE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_RECU_grand_En,
    										  YerothUtils::EN_template_RECU_GRAND_tex);
    }

    {
    	QFile fileStockHistory(FILE_YEROTH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockHistory,
    										  YerothUtils::FR_template_historique_dun_stock_tex);

    	QFile fileStockHistoryEN(FILE_YEROTH_ERP_3_0_TEMPLATE_HISTORIQUE_DUN_STOCK_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockHistoryEN,
    										  YerothUtils::EN_template_historique_dun_stock_tex);
    }

    {

    	QFile filePayments(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(filePayments,
    										  YerothUtils::FR_template_journal_des_paiements_tex);

    	QFile filePaymentsEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_PAYMENTS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(filePaymentsEn,
    										  YerothUtils::EN_template_journal_des_paiements_tex);
    }

    {
    	QFile fileLandscape(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_SALES_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileLandscape,
    										  YerothUtils::FR_template_journal_des_ventes_tex);

    	QFile fileLandscapeEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_SALES_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileLandscapeEn,
    										  YerothUtils::EN_template_journal_des_ventes_tex);
    }

    {
    	QFile fileLandscapeTransactions(FILE_YEROTH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileLandscapeTransactions,
    										  YerothUtils::FR_template_liste_des_mouvements_de_stocks_tex);

    	QFile fileLandscapeTransactionsEN(FILE_YEROTH_ERP_3_0_TEMPLATE_MOUVEMENTSDESTOCKS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileLandscapeTransactionsEN,
    										  YerothUtils::EN_template_liste_des_mouvements_de_stocks_tex);
    }

    {
    	QFile fileSupplierTransactionListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierTransactionListingFr,
    										  YerothUtils::FR_template_transactions_dun_fournisseur_tex);

    	QFile fileSupplierTransactionListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_FOURNISSEUR_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierTransactionListingEn,
    										  YerothUtils::EN_template_transactions_dun_fournisseur_tex);
    }

    {
    	QFile fileCustomerTransactionListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerTransactionListingFr,
    										  YerothUtils::FR_template_transactions_dun_client_tex);

    	QFile fileCustomerTransactionListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_TRANSACTIONS_DUN_CLIENT_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerTransactionListingEn,
    										  YerothUtils::EN_template_transactions_dun_client_tex);
    }

    {
    	QFile fileSupplierListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_FOURNISSEURS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierListingFr,
    										  YerothUtils::FR_template_fournisseurs_tex);

    	QFile fileSupplierListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_FOURNISSEURS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileSupplierListingEn,
    										  YerothUtils::EN_template_fournisseurs_tex);
    }

    {
    	QFile fileCustomerAccountListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerAccountListingFr,
    										  YerothUtils::FR_template_comptes_clients_tex);

    	QFile fileCustomerAccountListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTES_CLIENTS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileCustomerAccountListingEn,
    										  YerothUtils::EN_template_comptes_clients_tex);
    }

    {
    	QFile fileStockListingFr(FILE_YEROTH_ERP_3_0_TEMPLATE_MARCHANDISES_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockListingFr,
    										  YerothUtils::FR_template_marchandises_tex);

    	QFile fileStockListingEn(FILE_YEROTH_ERP_3_0_TEMPLATE_MARCHANDISES_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockListingEn,
    										  YerothUtils::EN_template_marchandises_tex);
    }

    {
    	QFile fileListeAlertesFr(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ALERTES_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListeAlertesFr,
    										  YerothUtils::FR_template_lister_alertes_tex);

    	QFile fileListeAlertesEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ALERTES_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListeAlertesEn,
    										  YerothUtils::EN_template_lister_alertes_tex);
    }

    {
    	QFile fileListe_CHARGES_FINANCIERES_FR(FILE_YEROTH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListe_CHARGES_FINANCIERES_FR,
    										  YerothUtils::FR_template_CHARGES_FINANCIERES_tex);

    	QFile fileListe_FINANCIAL_EXPENSES_EN(FILE_YEROTH_ERP_3_0_TEMPLATE_CHARGES_FINANCIERES_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListe_FINANCIAL_EXPENSES_EN,
    										  YerothUtils::EN_template_CHARGES_FINANCIERES_tex);
    }

    {
    	QFile fileListeAchatsFr(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ACHATS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListeAchatsFr,
    										  YerothUtils::FR_template_lister_achats_tex);

    	QFile fileListeAchatsEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTE_ACHATS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileListeAchatsEn,
    										   YerothUtils::EN_template_lister_achats_tex);
    }

    {
    	QFile file_comptes_doperation_comptables_Fr(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTABILITE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_comptes_doperation_comptables_Fr,
											  YerothUtils::FR_template_comptabilite_tex);

    	QFile file_comptes_doperation_comptables_En(FILE_YEROTH_ERP_3_0_TEMPLATE_COMPTABILITE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(file_comptes_doperation_comptables_En,
											  YerothUtils::EN_template_comptabilite_tex);
    }

    {
    	QFile yeroth_erp_3D_BARDIAG_TEMPLATEFr(FILE_YEROTH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(yeroth_erp_3D_BARDIAG_TEMPLATEFr,
    										  YerothUtils::FR_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex);

    	QFile yeroth_erp_3D_BARDIAG_TEMPLATEEn(FILE_YEROTH_ERP_3_0_TEMPLATE_THREE_D_BARDIAG);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(yeroth_erp_3D_BARDIAG_TEMPLATEEn,
    										  YerothUtils::EN_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex);
    }

    {
    	QFile fileStockDeSecuriteFr(FILE_YEROTH_ERP_3_0_TEMPLATE_FICHE_STOCKS_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockDeSecuriteFr,
    										  YerothUtils::FR_template_lister_stock_tex);

    	QFile fileStockDeSecuriteEn(FILE_YEROTH_ERP_3_0_TEMPLATE_FICHE_STOCKS_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileStockDeSecuriteEn,
    										  YerothUtils::EN_template_lister_stock_tex);
    }

    {
    	QFile fileTransfertsTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileTransfertsTemplate,
    										  YerothUtils::FR_template_sortie_des_stocks_petit_tex);

    	QFile fileTransfertsTemplateEn(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_SMALL_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileTransfertsTemplateEn,
    										  YerothUtils::EN_template_sortie_des_stocks_petit_tex);
    }

    {
    	QFile fileTransfertsBigTemplate(FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_FR);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileTransfertsBigTemplate,
    										  YerothUtils::FR_template_sortie_des_stocks_grand_tex);

    	QFile fileTransfertsBigTemplateEn (FILE_YEROTH_ERP_3_0_TEMPLATE_LISTING_OUTGOING_STOCKS_LARGE_EN);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(fileTransfertsBigTemplateEn,
    										  YerothUtils::EN_template_sortie_des_stocks_grand_tex);
    }

    {
    	QFile barDiagTemplate2(FILE_YEROTH_ERP_3_0_TEMPLATE_1A_TEX);
    	YerothUtils::YEROTH_READ_FILE_CONTENT(barDiagTemplate2,
    										  YerothUtils::_1a_tex);
    }
}


void read_system_local_yeroth_configuration()
{
    QFile
    file
    (YerothERPConfig::
     FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

    if (!file.open(QFile::ReadOnly))
    {
        return;
    }

    QTextStream init_cfg(&file);
    QString aValue;
    QString line;
    QStringList list;

    do
    {
        line = init_cfg.readLine();

        list =
                        line.split(YEROTH_ERP_3_0_CONFIGURATION_FILE_SEPARATION_OPERATOR);

//              QDEBUG_STRING_OUTPUT_2("line", line);

        if (YerothUtils::isEqualCaseInsensitive
                ("local_parameter_full_path_pdf_reader", list.at(0)))
        {
            YerothERPConfig::pathToPdfReader = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_pdf_latex_executable_root_dir",
                  list.at(0)))
        {
            YerothERPConfig::pathToLatexSystemRootFolder =
                            list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_mariadb_client_executables_root_dir",
                  list.at(0)))
        {
            YerothERPConfig::pathToMARIA_DB_BASE_DIR = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_full_path_file_temporary_folder",
                  list.at(0)))
        {
            YerothERPConfig::temporaryFilesDir = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_file_system_device_thermal_printer",
                  list.at(0)))
        {
            YerothERPConfig::pathToThermalPrinterDeviceFile =
                            list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_type_receipt", list.at(0)))
        {
            YerothERPConfig::receiptType = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_IP_address_network_thermal_printer",
                  list.at(0)))
        {
            YerothERPConfig::IP_ADDRESS_NETWORK_printer =
                            list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_thermal_printer", list.at(0)))
        {
            YerothERPConfig::printer = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_SHOW_DATABASE_RAW_table_column", list.at(0)))
        {
            aValue = list.at(1).trimmed();

            if (aValue.toInt() == YerothERPConfig::HIDE_DATABASE_RAW_table_column)
            {
                YerothERPConfig::SHOW_DATABASE_RAW = false;
            }
            else
            {
                YerothERPConfig::SHOW_DATABASE_RAW = true;
            }
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_state_open_cash_register", list.at(0)))
        {
            aValue = list.at(1).trimmed();

            if (aValue.toInt() == YerothERPConfig::CLOSE_CASH_DRAWER)
            {
                YerothERPConfig::ouvrirRegistreDeCaisse = false;
            }
            else
            {
                YerothERPConfig::ouvrirRegistreDeCaisse = true;
            }
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_format_receipt", list.at(0)))
        {
            YerothERPConfig::receiptFormat = list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_starting_year_business_dashboard",
                  list.at(0)))
        {
            YerothERPConfig::annee_depart_rapports_chiffre_affaire_value =
                            list.at(1).trimmed();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_length_line_feed_thermal_print_character_count",
                  list.at(0)))
        {
            YerothERPConfig::
            LINE_FEED_CHARACTER_COUNT_FOR_THERMAL_SMALL_PRINTING =
                            list.at(1).trimmed().toInt();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_length_maximal_display_string", list.at(0)))
        {
            YerothERPConfig::max_string_display_length =
                            list.at(1).trimmed().toInt();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_length_standard_pagination_number",
                  list.at(0)))
        {
            YerothERPConfig::pagination_number_of_table_rows =
                            list.at(1).trimmed().toInt();
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 ("local_parameter_length_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT",
                  list.at(0)))
        {
            YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
                            list.at(1).trimmed().toInt();

            YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT =
        			YerothERPConfig::MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT;
        }
    }
    while (!line.isNull());
}


void read_yeroth_configuration(YerothLogger &logger,
                               YerothERPWindows &allWindows)
{
    YerothSqlTableModel &configurationsTableModel =
                    allWindows.getSqlTableModel_configurations();

    //***
    QSqlRecord configurationRecord =
                    configurationsTableModel.
                    record(YerothERPConfig::CONFIG_ALERT_PERIOD_TIME_INTERVAL);
    QString
    alertPeriodTimeIntervalValue(GET_SQL_RECORD_DATA
                                 (configurationRecord,
                                  "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.
                    record(YerothERPConfig::CONFIG_ALERT_QUANTITY_TIME_INTERVAL);
    QString
    alertQuantityTimeIntervalValue(GET_SQL_RECORD_DATA
                                   (configurationRecord,
                                    "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerothERPConfig::CONFIG_TVA_VALUE);
    QString
    tvaValue(GET_SQL_RECORD_DATA
             (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerothERPConfig::CONFIG_SALES_STRATEGY);
    QString
    salesStrategyValue(GET_SQL_RECORD_DATA
                       (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record(YerothERPConfig::CONFIG_CURRENCY);
    QString
    currencyValue(GET_SQL_RECORD_DATA
                  (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.
                    record(YerothERPConfig::CONFIG_PAGINATION_PAGE_BEGIN_YEAR);
    QString
    pageBeginYearValue(GET_SQL_RECORD_DATA
                       (configurationRecord, "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record
                    (YerothERPConfig::CONFIG_BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL);
    QString
    BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE(GET_SQL_RECORD_DATA
                                                     (configurationRecord,
                                                      "valeur_configuration"));


    //***
    configurationRecord =
                    configurationsTableModel.record
                    (YerothERPConfig::
                     CONFIG_DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3);

    QString DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE
    (GET_SQL_RECORD_DATA(configurationRecord, "valeur_configuration"));



    /*
     * Now setting the configuration values
     */
    YerothERPConfig::currency = currencyValue;
    YerothERPConfig::alert_period_time_interval =
                    alertPeriodTimeIntervalValue.toUInt();
    YerothERPConfig::alert_quantity_time_interval =
                    alertQuantityTimeIntervalValue.toUInt();
    YerothERPConfig::tva_value = (tvaValue.toDouble() / 100.0);
    YerothERPConfig::salesStrategy = salesStrategyValue;

    YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY =
                    DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3_VALUE;


    if (BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE.isEmpty())
    {
        YerothERPConfig::backup_restore_yeroth_erp_3_time_interval = 169200;
    }
    else
    {
        YerothERPConfig::backup_restore_yeroth_erp_3_time_interval =
                        BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL_VALUE.toUInt();
    }

    if (!pageBeginYearValue.isEmpty())
    {
        YerothERPConfig::annee_depart_pour_la_pagination = pageBeginYearValue;
    }
    else
    {
        YerothERPConfig::annee_depart_pour_la_pagination =
                        GET_CURRENT_DATE_YEAR;
    }


    logger.log("[main] read_yeroth_configuration",
               QString("Folder for backup: %1").
               arg(YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY));

    YerothUtils::YEROTH_CREATE_FOLDER
    (YerothERPConfig::fullpathToBACKUP_YEROTH_ERP_3_DIRECTORY, &logger);


    logger.log("[main] read_yeroth_configuration",
               QString("Folder for temporary files: %1").arg
               (YerothERPConfig::temporaryFilesDir));

    YerothUtils::YEROTH_CREATE_FOLDER(YerothERPConfig::temporaryFilesDir,
                                      &logger);
}


int main(int argc, char *argv[])
{

#ifdef YEROTH_CHECK_MEMORY
    mtrace();
#endif


#ifdef YEROTH_ERP_3_0_TOUCH_SCREEN

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

#endif

    YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER =
                    QString(std::getenv("YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER"))
						.trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_yr_db_runtime_verif_HOME_FOLDER =
                    QString(std::getenv("YR_DB_RUNTIME_VERIF_HOME_FOLDER"))
						.trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER =
                    QString(std::getenv("YEROTH_ERP_3_0_SYSTEM_DAEMON_HOME_FOLDER"))
						.trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER =
                    QString(std::getenv("YEROTH_ERP_3_0_HOME_FOLDER")).trimmed();

    YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER =
                    QString("%1/.yeroth_erp_3_0").
                    arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);


//    QDEBUG_STRING_OUTPUT_2("YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER",
//                      YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER);


    YerothERPConfig::
    FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE
        =
                        QString(std::
                                getenv
                                ("YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE")).
                        trimmed();

//    QDEBUG_STRING_OUTPUT_2("YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTIES",
//              YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_SYSTEM_LOCAL_CONFIGURATION_PROPERTY_FILE);

    YerothERPConfig::FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER
		.prepend('/')
		.prepend(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER);


    QApplication *app = new QApplication(argc, argv);


    YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION = app;


    YerothERPWindows::_FRENCH_TRANSLATOR = new QTranslator;

    YerothERPWindows::_ENGLISH_TRANSLATOR = new QTranslator;


    YerothERPWindows::_FRENCH_TRANSLATOR->load("/usr/share/qt5/translations/qtbase_fr.qm");

    QString path(QString("%1/bin/%2")
    				.arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
    					 TRANSLATION_FILE_ENGLISH));

    YerothERPWindows::_ENGLISH_TRANSLATOR->load(path);


    app->installTranslator(YerothERPWindows::_FRENCH_TRANSLATOR);

    app->installTranslator(YerothERPWindows::_ENGLISH_TRANSLATOR);


#ifdef YEROTH_FRANCAIS_LANGUAGE

    YerothMainWindow::LANGUE_ANGLAISE = false;

#else //YEROTH_ENGLISH_LANGUAGE

    YerothMainWindow::LANGUE_ANGLAISE = true;

#endif

    QString
    logFileName(QString("%1/%2").arg
                (YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER,
                 FILE_YEROTH_ERP_3_0_PUBLIC_LOG_INFO));

    YerothUtils::setMainWorkingDirectory(YerothERPConfig::
                                         YEROTH_ERP_3_0_HOME_FOLDER);

    YerothUtils::setLogFileName(logFileName);

    YerothLogger logger("yeroth-erp-3.0");

    logger.log("main",
               QString
               ("yeroth home folder (located in the environment variable YEROTH_ERP_3_0_HOME_FOLDER): %1").
               arg(YerothERPConfig::YEROTH_ERP_3_0_HOME_FOLDER));

    logger.log("main", QString("log file name: %1").arg(logFileName));

    YerothMain stockFacileMain(&logger);

    QString
    initCfg(QString("%1/%2").arg
            (YerothERPConfig::YEROTH_ERP_3_0_PROPERTIES_CONFIGURATION_FOLDER,
             FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES));

    logger.log("main",
               QString("%1 path: %2").arg
               (FILE_YEROTH_ERP_3_0_PUBLIC_USER_PROPERTIES, initCfg));

    YerothERPConfig::initYerothConfig(initCfg);

    read_system_local_yeroth_configuration();

    YerothDatabase database;

    YerothERPWindows allWindows(YerothERPWindows::_YEROTH_ERP_PGI_APPLICATION->desktop());

    QApplication::setStyle(QStyleFactory::
                           create(YerothUtils::YEROTH_QT_STYLE_CDE));

    YerothUtils::setAllWindows(&allWindows);


#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

    Test_YerothERPTestUtils::setAllWindows(&allWindows);

#endif

    bool yerothCmdLineParser_isUsed = false;

    YerothCommandLineParser yerothCmdLineParser;

    enum YerothCommandLineArgumentType yerothCmdLineParseResult =
                    yerothCmdLineParser.parseProgramArguments(argc, argv);

    switch (yerothCmdLineParseResult)
    {
    case VERSION:
        yerothCmdLineParser_isUsed = true;
        YerothUtils::YEROTH_PRINT_TO_STDOUT(YerothUtils::
                                            YEROTH_ERP_3_0_LAST_BUILD_ID);
        break;

    default:
        break;
    }

    if (yerothCmdLineParser_isUsed)
    {
        if (VERSION == yerothCmdLineParseResult)
        {
            return 0;
        }
    }

    qDebug() << QString("db_name: %1, "
                        "db_ip_address: %2, "
                        "db_connection_options: %3").arg(database.db_name(),
                                                         database.
                                                         db_ip_address(),
                                                         database.
                                                         db_connection_options
                                                         ());

    allWindows.setDatabase(&database);

    bool isDatabaseOpened = database.open();

    logger.log("main", "Database connection could be opened successfully");

    QStringList sql_table_list;

#ifdef YEROTH_FEATURES_COMPTABILITE_yes
    sql_table_list
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_OPERATIONS_COMPTABLES;
#endif

    sql_table_list
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_periodes_dappartenance_groupes_de_paie_hr
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_groupes_demployes_hr
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_groupes_de_paie_hr
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DOPERATIONS_FINANCIERES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_COMPTES_BANCAIRES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ENTREPRISE_INFO
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DE_VENTE
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TYPE_DE_PAIEMENT
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_PAIEMENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CREDIT_CARD_ISSUING_COMPANY
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_INIT_CONFIGURATION
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONFIGURATIONS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ROLES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_USERS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_TITRES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_LOCALISATIONS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CHARGES_FINANCIERES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_YR_DB_RUNTIME_VERIF
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ACHATS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_SERVICES_COMPLETES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_MARCHANDISES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_VENDU
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_STOCKS_SORTIES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_REMISES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CONDITIONS_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_COURRIERS_ALERTES
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_PROGRAMMES_DE_FIDELITES_CLIENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_GROUPES_DE_CLIENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CLIENTS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_FOURNISSEURS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_IMPRIMANTERESEAU_RECUS_PETITS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_DBMS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_DEPARTEMENTS_PRODUITS
            << FILE_YEROTH_ERP_3_0_SQL_TABLE_CATEGORIES
			<< FILE_YEROTH_ERP_3_0_SQL_TABLE_LIGNES_BUDGETAIRES;

    for (int k = 0; k < sql_table_list.size(); ++k)
    {
        YerothUtils::executer_fichier_sql(sql_table_list.at(k), &logger);
    }

    read_yeroth_configuration(logger, allWindows);

    readTexTemplateFiles(logger);

    allWindows.createAllYerothPosUserWindows();


    QApplication::setFont(QFont
                          (STRING_APPLICATION_WIDE_FONT_TYPE_YEROTH_ERP_3_0,
                           INT_APPLICATION_WIDE_FONT_SIZE_YEROTH_ERP_3_0));

    QApplication::setWindowIcon(QIcon
                                (":yeroth-erp-3-0-images/yeroth-erp-3-0-icon.png"));


//#define YEROTH_DEVEL_TARGET

#ifdef YEROTH_DEVEL_TARGET

    YerothERPConfig::THIS_SITE_LOCALISATION_NAME =
                    "YEROTH_ERP_3_0_TESTING_RD_BUILDING";

#endif

//      QDEBUG_STRING_OUTPUT_2("YerothERPConfig::THIS_SITE_LOCALISATION_NAME",
//                                                 YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

#ifndef YEROTH_ERP_3_0_TESTING_UNIT_TEST

    allWindows._mainWindow->rendreVisibleLocalTOCLASS(isDatabaseOpened);

    app->exec();

    database.close();

    YEROTH_DELETE_FREE_POINTER_NOW(YerothERPWindows::_ENGLISH_TRANSLATOR);

    YEROTH_DELETE_FREE_POINTER_NOW(YerothERPWindows::_FRENCH_TRANSLATOR);

    return 0;

#else

    Test_YerothUtils aYEROTH_TEST_SRC_UTILS_TestInstance;

    Test_YerothERPTableView aYEROTHERPTableView_TestInstance;


    int test_SRC_UTILS_ret_value =
                    QTest::qExec(&aYEROTH_TEST_SRC_UTILS_TestInstance, argc, argv);

    int test_YEROTHERPTableView__ret_value =
                    QTest::qExec(&aYEROTHERPTableView_TestInstance, argc, argv);


    QDEBUG_STRING_OUTPUT_2_N("Test_YerothUtils", test_SRC_UTILS_ret_value);

    QDEBUG_STRING_OUTPUT_2_N("Test_YerothERPTableView",
                             test_YEROTHERPTableView__ret_value);


    return 0;

#endif
}
