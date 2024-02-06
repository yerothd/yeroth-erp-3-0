/*
 * yeroth-erp-utils.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-utils.hpp"

#include "src/widgets/yeroth-erp-qstandard-item.hpp"

#include "yeroth-erp-logger.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include "src/widgets/widgets-table/yeroth-erp-table-widget.hpp"

#include "src/widgets/widgets-table/yeroth-erp-point-de-vente-table-widget.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-payment-processing-information.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/yeroth-erp-windows.hpp"

#include <string>

#include <cassert>

#include <QtCore/qmath.h>

#include <QtCore/QRegExp>

#include <QtCore/QDebug>

#include <QtCore/QDate>

#include <QtCore/QBuffer>

#include <QtCore/QProcess>

#include <QtGui/QImageReader>

#include <QtSql/QSqlDriver>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>

#include <QtSql/QSqlQuery>

#include <QtWidgets/QStyle>

#include <QtWidgets/QStyleFactory>

#include <QtWidgets/QStylePlugin>

#include <QtWidgets/QLabel>

#include <QtWidgets/QDialog>

#include <QtWidgets/QFileDialog>

#include <QtWidgets/QMessageBox>

#include <QtWidgets/QApplication>

#include <QtWidgets/QMainWindow>

#include <QtPrintSupport/QPrintDialog>

#include <QtCore/QPluginLoader>

#include <QtCore/QLibraryInfo>

#include <QtCore/QtPlugin>

YerothERPWindows *YerothUtils::_allWindows(0);

QString YerothUtils::_mainWorkingDirectory("");

QString YerothUtils::_logFileName("");

QString YerothUtils::_1a_tex("");

QString YerothUtils::FR_template_bilan_comptable_tex("");

QString YerothUtils::EN_template_bilan_comptable_tex("");

QString YerothUtils::FR_template_CLIENT_LOYALTY_GROUP_LISTING_tex("");

QString YerothUtils::EN_template_CLIENT_LOYALTY_GROUP_LISTING_tex("");

QString YerothUtils::FR_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex("");

QString YerothUtils::EN_template_CLIENT_LOYALTY_PROGRAM_LISTING_tex("");

QString YerothUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex("");

QString YerothUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex("");

QString YerothUtils::FR_template_bon_de_commande_TEX("");

QString YerothUtils::EN_template_bon_de_commande_TEX("");

QString YerothUtils::FR_template_lister_admin_objects_TEX("");

QString YerothUtils::EN_template_lister_admin_objects_TEX("");

QString YerothUtils::FR_template_EMPLOYEE_GROUP_LISTING_tex("");

QString YerothUtils::EN_template_EMPLOYEE_GROUP_LISTING_tex("");

QString YerothUtils::FR_template_employee_PAY_GROUP_LISTING_tex("");

QString YerothUtils::EN_template_employee_PAY_GROUP_LISTING_tex("");

QString YerothUtils::FR_template_fiche_dun_fournisseur_tex("");

QString YerothUtils::EN_template_fiche_dun_fournisseur_tex("");

QString YerothUtils::FR_template_fiche_client_tex("");

QString YerothUtils::EN_template_fiche_client_tex("");

QString YerothUtils::FR_template_historique_dun_stock_tex("");

QString YerothUtils::EN_template_historique_dun_stock_tex("");

QString YerothUtils::FR_template_journal_des_paiements_tex("");

QString YerothUtils::EN_template_journal_des_paiements_tex("");

QString YerothUtils::FR_template_journal_des_ventes_tex("");

QString YerothUtils::EN_template_journal_des_ventes_tex("");

QString YerothUtils::FR_template_liste_des_mouvements_de_stocks_tex("");

QString YerothUtils::EN_template_liste_des_mouvements_de_stocks_tex("");

QString YerothUtils::FR_template_sortie_des_stocks_grand_tex("");

QString YerothUtils::EN_template_sortie_des_stocks_grand_tex("");

QString YerothUtils::FR_template_lister_stock_tex("");

QString YerothUtils::EN_template_lister_stock_tex("");

QString YerothUtils::FR_template_sortie_des_stocks_petit_tex("");

QString YerothUtils::EN_template_sortie_des_stocks_petit_tex("");

QString YerothUtils::FR_template_transactions_dun_fournisseur_tex("");

QString YerothUtils::EN_template_transactions_dun_fournisseur_tex("");

QString YerothUtils::FR_template_transactions_dun_client_tex("");

QString YerothUtils::EN_template_transactions_dun_client_tex("");

QString YerothUtils::FR_template_fournisseurs_tex("");

QString YerothUtils::EN_template_fournisseurs_tex("");

QString YerothUtils::FR_template_comptes_clients_tex("");

QString YerothUtils::EN_template_comptes_clients_tex("");

QString YerothUtils::FR_template_marchandises_tex("");

QString YerothUtils::EN_template_marchandises_tex("");

QString YerothUtils::FR_template_lister_alertes_tex("");

QString YerothUtils::EN_template_lister_alertes_tex("");

QString YerothUtils::FR_template_CHARGES_FINANCIERES_tex("");

QString YerothUtils::EN_template_CHARGES_FINANCIERES_tex("");

QString YerothUtils::FR_template_lister_achats_tex("");

QString YerothUtils::EN_template_lister_achats_tex("");

QString YerothUtils::FR_template_comptabilite_tex("");

QString YerothUtils::EN_template_comptabilite_tex("");

QString YerothUtils::FR_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex("");

QString YerothUtils::EN_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex("");

QString YerothUtils::FR_template_THREE_D_bardiag_tex("");

QString YerothUtils::EN_template_THREE_D_bardiag_tex("");

QString YerothUtils::FR_template_GROUPES_DUN_CLIENT_tex("");

QString YerothUtils::EN_template_GROUPES_DUN_CLIENT_tex("");

QString YerothUtils::FR_template_GROUPES_DUN_EMPLOYE_tex("");

QString YerothUtils::EN_template_GROUPES_DUN_EMPLOYE_tex("");

QString YerothUtils::FR_template_FACTURE_GRAND_tex("");

QString YerothUtils::EN_template_FACTURE_GRAND_tex("");

QString YerothUtils::FR_template_RECU_GRAND_tex("");

QString YerothUtils::EN_template_RECU_GRAND_tex("");

QString YerothUtils::FR_template_facture_petit_tex("");

QString YerothUtils::EN_template_facture_petit_tex("");

QString YerothUtils::EN_ZERO_ventes_tex("");

QString YerothUtils::FR_ZERO_ventes_tex("");

QString YerothUtils::FR_pie_chart_tex("");

QString YerothUtils::FR_bar_chart_tex("");

QString YerothUtils::FR_bar_diag_tex("");

QString YerothUtils::EN_pie_chart_tex("");

QString YerothUtils::EN_bar_chart_tex("");

QString YerothUtils::EN_bar_diag_tex("");

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_alerte_OUI_NON_ToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_strategieindexToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_titreToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_roleToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_typedoperationfinancieresToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_typedeventeToUserViewString;

/*
 * All valid key values must be >= 0.
 */
QMap<int, QString> YerothUtils::_typedepaiementToUserViewString;

const QString YerothUtils::PREFIX_RESTE_FACTURE_CLIENT("FAC-");

const QString YerothUtils::PREFIX_RECU_PAIEMENT_CLIENT("PAI-");

const QString YerothUtils::PREFIX_RECU_VENDU("VEN-");

const QString YerothUtils::PREFIX_RECU_SORTIE("SOR-");

const QKeySequence YerothUtils::MESSAGE_DAIDE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_USER_TIP));

const QKeySequence YerothUtils::EXPORTER_AU_FORMAT_CSV_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_EXPORT_AS_CSV_FILE));

const QKeySequence YerothUtils::PARAMETRER_IMPRESSION_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_PRINTING_SETTING));

const QKeySequence YerothUtils::IMPRIMER_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_PRINT));

const QKeySequence YerothUtils::AFFICHER_LES_MARCHANDISES_NON_TERMINES_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_AFFICHER_LES_STOCKS_NON_TERMINES));

const QKeySequence YerothUtils::AFFICHER_LES_MARCHANDISES_TERMINES_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_AFFICHER_LES_STOCKS_TERMINES));

const QKeySequence YerothUtils::RECHERCHER_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_SEARCH));

const QKeySequence YerothUtils::REINITIALISER_RECHERCHE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_REINITIALIAZE));

const QKeySequence YerothUtils::REINITIALISER_QUI_SUI_JE_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_WHO_AM_I));

const QKeySequence YerothUtils::LISTER_STOCKS_QKEYSEQUENCE(
		QObject::tr(SHORTCUT_LIST_STOCKS));

QString YerothUtils::YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY("");

const QString YerothUtils::UTILISATEUR_NON_EXISTANT("unknown user");

const QString YerothUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING("varchar");

const QString YerothUtils::DATABASE_MYSQL_CHAR_TYPE_STRING("char");

const QString YerothUtils::DATABASE_MYSQL_DATE_TYPE_STRING("date");

const QString YerothUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING("double");

const QString YerothUtils::DATABASE_MYSQL_INT_TYPE_STRING("int");

const QString YerothUtils::DATABASE_MYSQL_TIME_TYPE_STRING("time");

const QString YerothUtils::ETHERNET_LOCALHOST_STRING_NAME("localhost");

const QString YerothUtils::ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS("127.0.0.1");

const QString YerothUtils::YEROTH_QT_STYLE_CDE("cde");

const unsigned int YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED(-2);

const QString YerothUtils::IMPRIMANTE_PDF("pdf");

const QString YerothUtils::IMPRIMANTE_EPSON_TM_T20ii("epson TM-T20ii");

#ifdef YEROTH_FRANCAIS_LANGUAGE

const QString YerothUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU("epson TM-T20ii (réseau)");

#else

const QString YerothUtils::IMPRIMANTE_EPSON_TM_T20ii_RESEAU(
		"epson TM-T20ii (on NETWORK)");

#endif

//This is 3 seconds (3000 milliseconds)
const unsigned int YerothUtils::PRINT_TIMEOUT(TIMEOUT_PRINT);

const QString YerothUtils::STRING_STYLESHEETS_YEROTH_ERP_3_0(
		"QLabel { font-size: 17px; color: rgb(53, 132, 228); }");


const QString YerothUtils::STRING_STYLESHEET_COLOR_YEROTHGREEN(
		"QLabel { color : rgb(0, 255, 100); }");


const QColor YerothUtils::YEROTH_WHITE_COLOR_FOR_EMPTY_MERCHANDISE_FILTERING(
		QColor(COLOUR_RGB_YEROTH_RED_178_34_34));

const QColor YerothUtils::YEROTH_WHITE_COLOR(
		QColor(COLOUR_RGB_YEROTH_WHITE_255_255_255));

const QColor YerothUtils::YEROTH_BLACK_COLOR(
		QColor(COLOUR_RGB_YEROTH_BLACK_0_0_0));

const QColor YerothUtils::YEROTH_BLUE_COLOR(
		QColor(COLOUR_RGB_YEROTH_BLUE_77_93_254));

const QColor YerothUtils::YEROTH_DARK_GREEN_COLOR(
		QColor(COLOUR_RGB_YEROTH_DARK_GREEN_47_67_67));

const QColor YerothUtils::YEROTH_GRAY_COLOR(
		QColor(COLOUR_RGB_YEROTH_GRAY_78_78_78));

const QColor YerothUtils::YEROTH_GREEN_COLOR(
		QColor(COLOUR_RGB_YEROTH_GREEN_0_255_100));

const QColor YerothUtils::YEROTH_YEROTHGREEN_COLOR(
		QColor(COLOUR_RGB_YEROTH_GREEN_2_160_70));

const QColor YerothUtils::YEROTH_YELLOW_COLOR(
		QColor(COLOUR_RGB_YEROTH_YELLOW_254_254_0));

const QColor YerothUtils::YEROTH_ORANGE_COLOR(
		QColor(COLOUR_RGB_YEROTH_ORANGE_243_162_0));

const QColor YerothUtils::YEROTH_RED_COLOR(
		QColor(COLOUR_RGB_YEROTH_RED_178_34_34));

const QColor YerothUtils::YEROTH_INDIGO_COLOR(
		QColor(COLOUR_RGB_YEROTH_INDIGO_83_0_125));

const QBrush YerothUtils::YEROTH_QBRUSH_GREEN(YEROTH_GREEN_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_YEROTHGREEN(YEROTH_YEROTHGREEN_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_BLUE(YEROTH_BLUE_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_ORANGE(YEROTH_ORANGE_COLOR);

const QBrush YerothUtils::YEROTH_QBRUSH_YEROTHGRAY(YEROTH_GRAY_COLOR);

const QPalette YerothUtils::YEROTH_BLACK_PALETTE(
		QColor(COLOUR_RGB_YEROTH_BLACK_0_0_0));

const QPalette YerothUtils::YEROTH_BLUE_PALETTE(QColor(0, 0, 255));

const QPalette YerothUtils::YEROTH_DARK_GREEN_PALETTE(YEROTH_DARK_GREEN_COLOR);

const QPalette YerothUtils::YEROTH_GRAY_PALETTE(QColor(237, 237, 237));

const QPalette YerothUtils::YEROTH_LIGHT_GREEN_PALETTE(YEROTH_GREEN_COLOR);

const QPalette YerothUtils::YEROTH_GREEN_PALETTE(YEROTH_YEROTHGREEN_COLOR);

const QPalette YerothUtils::YEROTH_ORANGE_PALETTE(YEROTH_ORANGE_COLOR);

const QPalette YerothUtils::YEROTH_INDIGO_PALETTE(YEROTH_INDIGO_COLOR);

const QPalette YerothUtils::YEROTH_RED_PALETTE(YEROTH_RED_COLOR);

const QPalette YerothUtils::YEROTH_WHITE_PALETTE(
		QColor(COLOUR_RGB_YEROTH_WHITE_255_255_255));

const QLocale YerothUtils::frenchLocale(
		QLocale(QLocale::French, QLocale::France));

const QLocale YerothUtils::englishLocale(
		QLocale(QLocale::English, QLocale::Zambia));

const QString YerothUtils::TVA_0_STRING(STRING_TVA_0_PERCENT);

const QString YerothUtils::DATE_FORMAT(FORMAT_DATE);

const QString YerothUtils::TIME_FORMAT_WITH_MILLISECONDS(FORMAT_TIME_WITH_MS);

const QString YerothUtils::TIME_FORMAT(FORMAT_TIME);

const QString YerothUtils::DB_DATE_FORMAT(FORMAT_DATE_DATABASE);

#ifdef YEROTH_FRANCAIS_LANGUAGE

const QString YerothUtils::STRING_OUI(STRING_TEXT_YES_FR);
const QString YerothUtils::STRING_NON(STRING_TEXT_NO_FR);

const QString YerothUtils::INFERIEUR_OU_EGAL(QObject::tr(STRING_MATH_INFERIOR_OR_EQUAL_FR));
const QString YerothUtils::SUPERIEUR(QObject::tr(STRING_MATH_SUPERIOR_FR));

#else //YEROTH_ENGLISH_LANGUAGE

const QString YerothUtils::STRING_OUI(STRING_TEXT_YES_EN);
const QString YerothUtils::STRING_NON(STRING_TEXT_NO_EN);

const QString YerothUtils::INFERIEUR_OU_EGAL(
		QObject::tr(STRING_MATH_INFERIOR_OR_EQUAL_EN));
const QString YerothUtils::SUPERIEUR(QObject::tr(STRING_MATH_SUPERIOR_EN));

#endif

const QString YerothUtils::MAGELAN_CLIENT("yr.db-runtime.verif");

const QString YerothUtils::MAGELAN_CLIENT_OBJECT(
		"/YR_DB_RUNTIME_VERIF_DBusClient");

const QString YerothUtils::MAGELAN_SERVER("com.yeroth-erp.server");

const QString YerothUtils::MAGELAN_SERVER_OBJECT("/");

const QString YerothUtils::YEROTH_ERP_3_0_LAST_BUILD_ID(
		"b13376b887f40fe3a752a4337b881210dbcc88ba");

const QString YerothUtils::MYSQL("mysql");

const QString YerothUtils::MYSQL_DUMP("mysqldump");

#ifdef YEROTH_SERVER

	#ifdef YEROTH_FRANCAIS_LANGUAGE

const QString YerothUtils::APROPOS_MSG("YEROTH-PGI-3.0-SERVER"
								   "\n(VERSION du YEROTH_BUILD_DATE à YEROTH_BUILD_DATE_TIMESTAMP)"
								   "\n\n par \n\n"
								   "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
								   "LAST BUILD ID: ''.\n\n"
								   "ON BUILD_COMPUTER: .\n\n"
								   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
								   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerothUtils::APPLICATION_NAME("YEROTH-PGI-3.0-SERVER");

	#else //YEROTH_ENGLISH_LANGUAGE

const QString YerothUtils::APROPOS_MSG("YEROTH-ERP-3.0-SERVER"
								   "\n(VERSION of YEROTH_BUILD_DATE at YEROTH_BUILD_DATE_TIMESTAMP)"
								   "\n\n by \n\n"
								   "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
								   "LAST BUILD ID: ''.\n\n"
								   "ON BUILD_COMPUTER: .\n\n"
								   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
								   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerothUtils::APPLICATION_NAME("YEROTH-ERP-3.0-SERVER");

	#endif //YEROTH_FRANCAIS_LANGUAGE
#endif

#ifdef YEROTH_CLIENT
const QString YerothUtils::APROPOS_MSG(QObject::
                                       trUtf8
                                       ("YEROTH-PGI-3.0-CLIENT"
                                        "\n(VERSION du YEROTH_BUILD_DATE à YEROTH_BUILD_DATE_TIMESTAMP)"
                                        "\n\n par \n\n"
                                        "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
                                        "LAST BUILD ID: ''.\n\n"
                                        "ON BUILD_COMPUTER: .\n\n"
                                        "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
                                        "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)"));
const
QString
YerothUtils::
APPLICATION_NAME
(QObject::
 trUtf8
 ("YEROTH-ERP-3.0-CLIENT"));
#endif

#ifdef YEROTH_STANDALONE

	#ifdef YEROTH_FRANCAIS_LANGUAGE

const QString YerothUtils::APROPOS_MSG("YEROTH-PGI-3.0-STANDALONE"
									   "\n(VERSION du YEROTH_BUILD_DATE à YEROTH_BUILD_DATE_TIMESTAMP)"
									   "\n\n par \n\n"
									   "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
									   "LAST BUILD ID: ''.\n\n"
									   "ON BUILD_COMPUTER: .\n\n"
									   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
									   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerothUtils::APPLICATION_NAME("YEROTH-PGI-3.0-STANDALONE");

	#else //YEROTH_ENGLISH_LANGUAGE

const QString YerothUtils::APROPOS_MSG("YEROTH-ERP-3.0-STANDALONE"
									   "\n(VERSION of YEROTH_BUILD_DATE at YEROTH_BUILD_DATE_TIMESTAMP)"
									   "\n\n by \n\n"
									   "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
									   "LAST BUILD ID: ''.\n\n"
									   "ON BUILD_COMPUTER: .\n\n"
									   "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
									   "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");

const QString YerothUtils::APPLICATION_NAME("YEROTH-ERP-3.0-STANDALONE");

	#endif //YEROTH_FRANCAIS_LANGUAGE

#endif

#ifdef YEROTH_ACADEMIC_EVALUATION_VERSION
const
QString
YerothUtils::
APROPOS_MSG
(QObject::
 trUtf8
 ("YEROTH-PGI-3.0-ACADEMIC_EVALUATION"
  "\n(VERSION du YEROTH_BUILD_DATE à YEROTH_BUILD_DATE_TIMESTAMP)"
  "\n\n par \n\n"
  "(c) (2015 - ...) DR.-ING. DIPL.-INF. XAVIER NOUNDOU.\n\n"
  "LAST BUILD ID: ''.\n\n"
  "ON BUILD_COMPUTER: .\n\n"
  "CLIENT DEDICATED COMPANY LOCALISATION SITE: .\n\n"
  "acknowledgments: JAN PELESKA ! (Universität Bremen in GERMANY)");
 const
 QString
 YerothUtils::
 APPLICATION_NAME
 (QObject::
  trUtf8
  ("YEROTH-ERP-3.0-ACADEMIC_EVALUATION"));
#endif

const unsigned int YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS(12);

const unsigned int YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS(12);

const QString YerothUtils::MYSQL_TRUE_LITERAL("1");

const QString YerothUtils::MYSQL_FALSE_LITERAL("0");

const QString YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR(";");

const QString YerothUtils::STRING_FRENCH_DIVERS(QObject::tr("DIVERS"));

const QString YerothUtils::EMPTY_STRING("");

const QChar YerothUtils::DOT_CHAR('.');

const QChar YerothUtils::STAR_CHAR('*');

const QChar YerothUtils::DASH_CHAR('_');

const QChar YerothUtils::PERCENT_CHAR('%');

const QChar YerothUtils::SLASH_CHAR('/');

const QRegExp YerothUtils::PasswordRegExp("");

const QRegExp YerothUtils::LINE_ENDING_STRING_REGEXP("[\r\n]");

const QRegExp YerothUtils::EMPTY_SPACE_REGEXP("\\s");

const int YerothUtils::YEROTH_PAGINATION_INT_VALIDATOR_MAXIMAL_VALUE = 999;

const QIntValidator YerothUtils::nbre_de_LINE_FEED_POUR_IMPRESSION_PETIT_THERMIQUE_Validator(
		1, 12);

const QIntValidator YerothUtils::Int_BACKUP_YEROTH_ERP_3_Validator(1, 172800);

const QRegExp YerothUtils::STRING_FOR_YEROTH_ERP_3_0_VALIDATOR_DB_COLUMN_NAME(
		"a-zA-Z0-9_");

const QRegExpValidator YerothUtils::STRING_FOR_YEROTH_ERP_3_0_IP_ADDRESS_VALIDATOR(
		QRegExp(
				QString("%1\\.%2\\.%3\\.%4").arg("[1-9]+", "[0-9]+", "[0-9]+",
						"[0-9]+")));

const QRegExpValidator YerothUtils::STRING_FOR_YEROTH_ERP_3_0_VALIDATOR(
		QRegExp(
				QString("[%1%2%3]*").arg(
						"a-zA-Z0-9º:;,~´`@§$£#%\\s\\.\\^\\\\(\\)\\&/\<>\\?\\!\\{\\}\\[\\]\\+\\-\{\}=_'|",
						"àéèêôîûÀÉÈÊÔÛÎ", "äëöïüÄËÖÏÜ")));


const QIntValidator YerothUtils::UINT_VALIDATOR_GREATER_THAN_ZERO(1, 444444444);


const QIntValidator YerothUtils::UintValidator(0, 444444444);


const QDoubleValidator YerothUtils::DoubleValidator(-444444444, 444444444, 2);


const QDoubleValidator YerothUtils::POSITIVE_DoubleValidator(0, 444444444, 2);


const QRegExpValidator YerothUtils::PasswordValidator(PasswordRegExp);



void YerothUtils::RELOAD_VIEW_STRINGS()
{
    YerothUtils::_alerte_OUI_NON_ToUserViewString.clear();

    YerothUtils::_strategieindexToUserViewString.clear();

    YerothUtils::_titreToUserViewString.clear();

    YerothUtils::_roleToUserViewString.clear();

    YerothUtils::_typedoperationfinancieresToUserViewString.clear();

    YerothUtils::_typedeventeToUserViewString.clear();

    YerothUtils::_typedepaiementToUserViewString.clear();



	YerothUtils::_alerte_OUI_NON_ToUserViewString.insert(
			YerothUtils::ALERTE_RESOLUE_NON_INDEX, YerothUtils::STRING_NON);

	YerothUtils::_alerte_OUI_NON_ToUserViewString.insert(
			YerothUtils::ALERTE_RESOLUE_OUI_INDEX, YerothUtils::STRING_OUI);


	YerothUtils::_strategieindexToUserViewString.insert(
			YerothUtils::STRATEGIE_ALL_COMBOBOX_INDEX,
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL);

	YerothUtils::_strategieindexToUserViewString.insert(
			YerothUtils::STRATEGIE_FEFO_COMBOBOX_INDEX,
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO);

	YerothUtils::_strategieindexToUserViewString.insert(
			YerothUtils::STRATEGIE_FIFO_COMBOBOX_INDEX,
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO);

	YerothUtils::_strategieindexToUserViewString.insert(
			YerothUtils::STRATEGIE_LIFO_COMBOBOX_INDEX,
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO);


	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_DR,
			QObject::tr("Dr."));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_ME,
			QObject::tr("Me."));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_MLLE,
			QObject::tr("Mlle"));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_MME,
			QObject::tr("Mme"));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_MR,
			QObject::tr("M."));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_PR,
			QObject::tr("Pr."));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_PROF,
			QObject::tr("Prof."));

	YerothUtils::_titreToUserViewString.insert(YerothUtils::TITRE_INDEFINI,
			QObject::tr("titre indéfini"));


	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_ADMINISTRATEUR,
			QObject::tr("administrateur"));

	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_MANAGER,
			QObject::tr("manager"));

	YerothUtils::_roleToUserViewString.insert(
			YerothUtils::ROLE_GESTIONNAIREDESTOCK,
			QObject::tr("gestionnaire de (d'immobilisation) stock"));

	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_MAGASINIER,
			QObject::tr("magasinier"));

	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_CAISSIER,
			QObject::tr("caissier"));

	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_VENDEUR,
			QObject::tr("vendeur"));

	YerothUtils::_roleToUserViewString.insert(YerothUtils::ROLE_INDEFINI,
			QObject::tr("role indéfini"));


	YerothUtils::_typedoperationfinancieresToUserViewString.insert(
			YerothUtils::OPERATION_FINANCIERE_VENTE, QObject::tr("vente"));

	YerothUtils::_typedoperationfinancieresToUserViewString.insert(
			YerothUtils::OPERATION_FINANCIERE_CHARGE_FINANCIERE,
			QObject::tr("charge financière"));

	YerothUtils::_typedoperationfinancieresToUserViewString.insert(
			YerothUtils::OPERATION_FINANCIERE_ACHAT_DE_MARCHANDISES,
			QObject::tr("achat de marchandises"));

	YerothUtils::_typedoperationfinancieresToUserViewString.insert(
			YerothUtils::OPERATION_FINANCIERE_INDEFINI,
			QObject::tr("indéfini"));


	YerothUtils::_typedeventeToUserViewString.insert(
			YerothUtils::VENTE_COMPTANT, QObject::tr("vente (comptant)"));

	YerothUtils::_typedeventeToUserViewString.insert(
			YerothUtils::VENTE_COMPTECLIENT_AVANCE_DE_CASH,
			QObject::tr("vente (AVANCE DE CASH)"));

	YerothUtils::_typedeventeToUserViewString.insert(
			YerothUtils::VENTE_COMPTE_CLIENT,
			QObject::tr("vente (compte client)"));

	YerothUtils::_typedeventeToUserViewString.insert(
			YerothUtils::VENTE_INDEFINI, QObject::tr("vente indéfini"));



	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_AVANCE_DE_CASH,
			QObject::tr("versement (AVANCE DE CASH)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_COMPTANT,
			QObject::tr("versement (comptant)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_CHEQUE,
			QObject::tr("versement (cheque)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_TELEPHONE,
			QObject::tr("versement (téléphone)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_BANCAIRE,
			QObject::tr("versement (bancaire)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_VIREMENT_BANCAIRE,
			QObject::tr("versement (virement bancaire)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_CHARGE_FINANCIERE_ANNULE,
			QObject::tr("versement (charge financière (annulée))"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::ENCAISSEMENT_INDEFINI,
			QObject::tr("versement indéfini"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_COMPTANT,
			QObject::tr("décaissement (comptant)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_CHEQUE,
			QObject::tr("décaissement (cheque)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_TELEPHONE,
			QObject::tr("décaissement (téléphone)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_BANCAIRE,
			QObject::tr("décaissement (bancaire)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_VIREMENT_BANCAIRE,
			QObject::tr("décaissement (virement bancaire)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_RETOUR_ACHAT_DUN_CLIENT,
			QObject::tr("décaissement (retour achat)"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_INDEFINI,
			QObject::tr("décaissement indéfini"));

	YerothUtils::_typedepaiementToUserViewString.insert(
			YerothUtils::DECAISSEMENT_POUR_PROGRAMME_DE_FIDELITE_CLIENT,
			QObject::tr("décaissement (fidélité clients)"));
}


void YerothUtils::YEROTH_resizeEvent(QWidget &a_current_widget_to_move,
                                     const QSize &a_current_windows_SIZE,
                                     const QSize &a_current_windows_PREVIOUS_SIZE)
{
	if (a_current_windows_PREVIOUS_SIZE.width() <= 0
			|| a_current_windows_PREVIOUS_SIZE.height() <= 0)
	{
		return;
	}

	double NEW_X = (a_current_widget_to_move.x()
			/ (double) a_current_windows_PREVIOUS_SIZE.width())
			* a_current_windows_SIZE.width();
	double NEW_Y = (a_current_widget_to_move.y()
			/ (double) a_current_windows_PREVIOUS_SIZE.height())
			* a_current_windows_SIZE.height();
	a_current_widget_to_move.move(qCeil(NEW_X), qCeil(NEW_Y));
}

bool YerothUtils::creerNouveauClient(QString &proposedCustomerName_IN_OUT,
									 YerothWindowsCommons *_callingWindow /* = 0 */)
{
	if (proposedCustomerName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerothSqlTableModel &customerSqlTableModel = _allWindows->getSqlTableModel_clients();

	QString customerTableFilter = QString("%1 = '%2'")
									.arg(YerothDatabaseTableColumn::NOM_ENTREPRISE,
										 proposedCustomerName_IN_OUT);

	customerSqlTableModel.yerothSetFilter_WITH_where_clause(
			customerTableFilter);

	int rows = customerSqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 870);

	if (rows > 0)
	{
		proposedCustomerName_IN_OUT.clear();

		proposedCustomerName_IN_OUT.append(
				GET_SQL_RECORD_DATA(customerSqlTableModel.record(0),
						YerothDatabaseTableColumn::NOM_ENTREPRISE));

		customerSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 880);

		return true;
	}
	else
	{
		customerSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 886);

		QSqlRecord record = customerSqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::ID,
				YerothERPWindows::getNextIdSqlTableModel_clients());

		record.setValue(YerothDatabaseTableColumn::LOCALISATION,
				YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

		record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
				proposedCustomerName_IN_OUT);

		record.setValue(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
				0.0);

		record.setValue(YerothDatabaseTableColumn::COMPTE_CLIENT, 0.0);

		QString retMsg(
				QObject::tr("L'entreprise cliente '%1'").arg(
						proposedCustomerName_IN_OUT));

		bool success = customerSqlTableModel.insertNewRecord(record,
															 0,
															 "src/utils/yeroth-erp-utils.cpp",
															 908,
															 proposedCustomerName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerothQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));
			if (0 != _callingWindow)
			{
				YerothQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerothUtils::creerNouveauFournisseur(
		QString &proposedFournisseurName_IN_OUT,
		YerothWindowsCommons *_callingWindow /* = 0 */)
{
	if (proposedFournisseurName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerothSqlTableModel &fournisseurSqlTableModel =
			_allWindows->getSqlTableModel_fournisseurs();

	QString fournisseurFilter = QString("%1 = '%2'").arg(
			YerothDatabaseTableColumn::NOM_ENTREPRISE,
			proposedFournisseurName_IN_OUT);

	fournisseurSqlTableModel.yerothSetFilter_WITH_where_clause(
			fournisseurFilter);

	int rows = fournisseurSqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 920);

	if (rows > 0)
	{
		proposedFournisseurName_IN_OUT.clear();

		proposedFournisseurName_IN_OUT.append(
				GET_SQL_RECORD_DATA(fournisseurSqlTableModel.record(0),
						YerothDatabaseTableColumn::NOM_ENTREPRISE));

		fournisseurSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 933);

		return true;
	}
	else
	{
		fournisseurSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 939);

		QSqlRecord record = fournisseurSqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::LOCALISATION,
				YerothERPConfig::THIS_SITE_LOCALISATION_NAME);

		record.setValue(YerothDatabaseTableColumn::ID,
				YerothERPWindows::getNextIdSqlTableModel_fournisseurs());

		record.setValue(YerothDatabaseTableColumn::EMPLOYE, false);

		record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE,
				proposedFournisseurName_IN_OUT);

		record.setValue(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR, 0.0);

		QString retMsg(
				QObject::tr("L'entreprise fournisseur '%1'").arg(
						proposedFournisseurName_IN_OUT));

		bool success = fournisseurSqlTableModel.insertNewRecord(record,
																0,
																"src/utils/yeroth-erp-utils.cpp",
																1012,
																proposedFournisseurName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerothQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			fournisseurSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 982);

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));

			if (0 != _callingWindow)
			{
				YerothQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			fournisseurSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1003);

			return true;
		}
	}

	return false;
}

bool YerothUtils::creerNouveauDepartementDeProduits(
		QString &proposedProductDepartmentName_IN_OUT,
		YerothWindowsCommons *_callingWindow
		/* = 0 */
		)
{
	if (proposedProductDepartmentName_IN_OUT.isEmpty())
	{
		return false;
	}

	YerothSqlTableModel &departements_produits_SqlTableModel =
			_allWindows->getSqlTableModel_departements_produits();

	QString productDepartmentFilter = QString("%1 = '%2'").arg(
			YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			proposedProductDepartmentName_IN_OUT);

	departements_produits_SqlTableModel.yerothSetFilter_WITH_where_clause(
			productDepartmentFilter);

	int rows = departements_produits_SqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1033);

	if (rows > 0)
	{
		proposedProductDepartmentName_IN_OUT.clear();

		proposedProductDepartmentName_IN_OUT.append(
				GET_SQL_RECORD_DATA(
						departements_produits_SqlTableModel.record(0),
						YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT));

		departements_produits_SqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1044);

		return true;
	}
	else
	{
		departements_produits_SqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1050);

		QSqlRecord record = departements_produits_SqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::ID,
				YerothERPWindows::getNextIdSqlTableModel_departements_produits());
		record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
				proposedProductDepartmentName_IN_OUT);
		record.setValue(
				YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
				YerothUtils::EMPTY_STRING);
		QString retMsg(
				QObject::tr("Le département de produits '%1'").arg(
						proposedProductDepartmentName_IN_OUT));

		bool success = departements_produits_SqlTableModel.insertNewRecord(record,
																		   0,
																		   "src/utils/yeroth-erp-utils.cpp",
																		   1118,
																		   proposedProductDepartmentName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));
			if (0 != _callingWindow)
			{
				YerothQMessageBox::warning(_callingWindow,
						QObject::tr("échec"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));
			if (0 != _callingWindow)
			{
				YerothQMessageBox::information(_callingWindow,
						QObject::tr("succès"), retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug()
                        << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerothUtils::check_IF_departement_produit_exists(
		QString &proposedProductDepartmentName)
{
	return (YerothUtils::execQueryRowCount(
			QString("select %1 from %2 where %3='%4'").arg(
					YerothDatabaseTableColumn::ID,
					YerothDatabase::DEPARTEMENTS_PRODUITS,
					YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
					proposedProductDepartmentName)) > 0);
}

bool YerothUtils::creerNouvelleCategorie(QString &proposedCategorieName_IN_OUT,
		QString &proposedProductDepartmentName,
		YerothWindowsCommons *_callingWindow
		/* = 0 */
		)
{
	if (proposedCategorieName_IN_OUT.isEmpty()
			|| proposedProductDepartmentName.isEmpty())
	{
		return false;
	}

	if (!YerothUtils::check_IF_departement_produit_exists(
			proposedProductDepartmentName))
	{
		QString retMsg(
				QObject::tr("1 département de produits '%1' N'EXISTE PAS !").arg(
						proposedProductDepartmentName));
		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow, QObject::tr("échec"),
					retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
            qDebug()
                    << retMsg;
#endif
		}

		return false;
	}

	YerothSqlTableModel &categorieSqlTableModel = _allWindows->getSqlTableModel_categories();

	QString categorieFilter = QString("%1 = '%2'")
								.arg(YerothDatabaseTableColumn::NOM_CATEGORIE,
									 proposedCategorieName_IN_OUT);

	categorieSqlTableModel.yerothSetFilter_WITH_where_clause(categorieFilter);

	int rows = categorieSqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1218);

	if (rows > 0)
	{
		proposedCategorieName_IN_OUT.clear();

		proposedCategorieName_IN_OUT
			.append(GET_SQL_RECORD_DATA(categorieSqlTableModel.record(0),
										YerothDatabaseTableColumn::NOM_CATEGORIE));

		categorieSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1228);

		return true;
	}
	else
	{
		categorieSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1234);

		QSqlRecord record = categorieSqlTableModel.record();

		record.setValue(YerothDatabaseTableColumn::ID,
						YerothERPWindows::getNextIdSqlTableModel_categories());

		record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
						proposedProductDepartmentName);

		record.setValue(YerothDatabaseTableColumn::NOM_CATEGORIE,
						proposedCategorieName_IN_OUT);

		record.setValue(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE,
						YerothUtils::EMPTY_STRING);

		QString retMsg = QObject::tr("La catégorie '%1'")
							.arg(proposedCategorieName_IN_OUT);

		bool success = categorieSqlTableModel.insertNewRecord(record,
															  0,
															  "src/utils/yeroth-erp-utils.cpp",
															  1253,
															  proposedCategorieName_IN_OUT);

		if (!success)
		{
			retMsg.append(QObject::tr(" n'a pas pu être créer !"));

			if (0 != _callingWindow)
			{
				YerothQMessageBox::warning(_callingWindow,
										   QObject::tr("échec"),
										   retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug() << retMsg;
#endif
			}

			return false;
		}
		else
		{
			retMsg.append(QObject::tr(" a été créer avec succès !"));

			if (0 != _callingWindow)
			{
				YerothQMessageBox::information(_callingWindow,
											   QObject::tr("succès"),
											   retMsg);
			}
			else
			{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST
                qDebug() << retMsg;
#endif
			}

			return true;
		}
	}

	return false;
}

bool YerothUtils::GREP_YEROTH_FILE_CONTENT(const QString &file_full_path,
										   const QString &EXECUTABLE_full_path,
										   const QString &keyword_IN)
{
	QStringList progArguments;

	progArguments << keyword_IN << file_full_path;

	QProcess GREP_PROCESS;

	bool checkProcessFinished = YerothERPProcess::startAndWaitForFinished(
			GREP_PROCESS, "/bin/grep", progArguments, 1000);
	if (checkProcessFinished)
	{
		QString checkAlertDeamonProcessOutput(
				GREP_PROCESS.readAllStandardOutput().trimmed());

		return checkAlertDeamonProcessOutput.contains(EXECUTABLE_full_path);
	}

	return false;
}

void YerothUtils::YEROTH_READ_FILE_CONTENT(QFile &file, QString &fileContentVar)
{
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
		QString line;
		do
		{
			line = stream.readLine().trimmed();
			fileContentVar.append(line).append("\n");
		} while (!line.isNull());
		file.close();
	}
}

bool YerothUtils::YEROTH_CREATE_FOLDER(const QString &aFullPathDir,
		YerothLogger *aLogger
		/* = 0 */
		)
{
	QFileInfo aFullPathDirInfo(aFullPathDir);
	if (!aFullPathDirInfo.exists())
	{
		if (0 != aLogger)
		{
			aLogger->log(
					"YerothUtils::YEROTH_CREATE_FOLDER | read_yeroth_configuration",
					QString("Folder '%1' for temporary files does not exist!").arg(
							aFullPathDir));
		}

		QDir tempFileDir;
		if (tempFileDir.mkpath(aFullPathDir))
		{
			if (0 != aLogger)
			{
				aLogger->log(
						"YerothUtils::YEROTH_CREATE_FOLDER | read_yeroth_configuration",
						QString("Created folder '%1' for temporary files").arg(
								aFullPathDir));
			}

			return true;
		}
		else
		{
			if (0 != aLogger)
			{
				aLogger->log(
						"YerothUtils::YEROTH_CREATE_FOLDER | read_yeroth_configuration",
						QString(
								"Could not Create folder '%1' for temporary files!").arg(
								aFullPathDir));
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

bool YerothUtils::GZIP_YEROTH_FILE(
		const QString &program_working_directory_full_path,
		const QString &file_full_path)
{
	QStringList gzipPROG_ARGS;
	gzipPROG_ARGS.append(file_full_path);
	int file_size =
			YerothERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE(
					"/usr/bin/gzip", program_working_directory_full_path,
					QString("%1.gz").arg(file_full_path), gzipPROG_ARGS);
	return (file_size >= 0);
}

void YerothUtils::REMOVE_STRING_FROM_SPLIT_STAR_SEPARATED_DB_STRING(
		QString &SEPARATED_RESULTING_QSTRING_IN_OUT,
		const QString &aCur_db_STRING)
{
	QStringList separated_resulting_qstringlist;
	YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(separated_resulting_qstringlist,
			SEPARATED_RESULTING_QSTRING_IN_OUT);
	SEPARATED_RESULTING_QSTRING_IN_OUT.clear();
	separated_resulting_qstringlist.removeAll(aCur_db_STRING);
	uint SIZE = separated_resulting_qstringlist.size();
	for (uint i = 0; i < SIZE; ++i)
	{
		if (i < SIZE - 1)
		{
			SEPARATED_RESULTING_QSTRING_IN_OUT.append(
					QString("%1%2").arg(separated_resulting_qstringlist.at(i),
							YerothUtils::STAR_CHAR));
		}
		else
		{
			SEPARATED_RESULTING_QSTRING_IN_OUT.append(
					separated_resulting_qstringlist.at(i));
		}
	}
}

/**
 * returns true if '*' separated string has REALLY GOT UPDATED WITH A NEW STRING VALUE 'aNewValue' !
 */
bool YerothUtils::APPEND_NEW_ELEMENT_TO_STAR_SEPARATED_DB_STRING(
		const QString &aNewValue,
		QString &aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT)
{
	if (!aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.isEmpty())
	{
		if (YerothUtils::CONTAINS_SPLIT_STAR_SEPARATED_DB_STRING(
				aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT, aNewValue))
		{
			return false;
		}

		if (aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.endsWith("*"))
		{
			aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(aNewValue);
		}
		else
		{
			aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(
					QString("%1%2").arg("*", aNewValue));
		}
	}
	else
	{
		aCur_db_STRING_STAR_SEPARATED_VALUE_IN_OUT.append(aNewValue);
	}

	return true;
}

enum service_stock_already_exist_type YerothUtils::IS_STOCK_DESIGNATION_OR_REFERENCE_UNIQUE(
		const QString &aStockServiceReference,
		const QString &aStockServiceCategory,
		const QString &aStockServiceDesignation,
		QString &curExistingReferenceCategoryDesignation_PRODUCT_in_out)
{
	curExistingReferenceCategoryDesignation_PRODUCT_in_out.clear();
	QSqlRecord record;
	int marchandisesTableModelRowCount = 0;
	YerothSqlTableModel &marchandisesTableModel =
			_allWindows->getSqlTableModel_marchandises();
	/** Verifies stock reference **/
	{
		marchandisesTableModel.yerothSetFilter_WITH_where_clause(
				QString("%1 = '%2'").arg(YerothDatabaseTableColumn::REFERENCE,
						aStockServiceReference));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1389);

		record.clear();
		QString stockDesignation;
		QString stockReference;
		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			stockDesignation = GET_SQL_RECORD_DATA(record,
					YerothDatabaseTableColumn::DESIGNATION);
			stockReference = GET_SQL_RECORD_DATA(record,
					YerothDatabaseTableColumn::REFERENCE);
			if (YerothUtils::isEqualCaseInsensitive(stockReference,
					aStockServiceReference))
			{
				if (!YerothUtils::isEqualCaseInsensitive(aStockServiceCategory,
						GET_SQL_RECORD_DATA(record,
								YerothDatabaseTableColumn::CATEGORIE))
						|| !YerothUtils::isEqualCaseInsensitive(
								aStockServiceDesignation,
								GET_SQL_RECORD_DATA(record,
										YerothDatabaseTableColumn::DESIGNATION)))
				{
					curExistingReferenceCategoryDesignation_PRODUCT_in_out =
							stockDesignation;

					marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1438);

					return SERVICE_REFERENCE_EXISTS;
				}
			}
		}
	}

	/** Verifies stock designation **/
	{
		marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1448);

		record.clear();
		QString stockDesignation;
		marchandisesTableModel.yerothSetFilter_WITH_where_clause(
				QString("%1 = '%2'").arg(YerothDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1431);

		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			stockDesignation = GET_SQL_RECORD_DATA(record,
					YerothDatabaseTableColumn::DESIGNATION);
			if (YerothUtils::isEqualCaseInsensitive(stockDesignation,
					aStockServiceDesignation))
			{
				if (!YerothUtils::isEqualCaseInsensitive(aStockServiceCategory,
						GET_SQL_RECORD_DATA(record,
								YerothDatabaseTableColumn::CATEGORIE))
						|| !YerothUtils::isEqualCaseInsensitive(
								aStockServiceReference,
								GET_SQL_RECORD_DATA(record,
										YerothDatabaseTableColumn::REFERENCE)))
				{
					curExistingReferenceCategoryDesignation_PRODUCT_in_out =
							stockDesignation;

					marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1478);

					return SERVICE_STOCK_DESIGNATION_EXIST;
				}
			}
		}
	}

	/** Verifies stock category **/
	{
		marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1488);

		record.clear();
		QString serviceStockCategory;
		/**
		 * IT IS SUFFICIENT TO ONLY CHECK AGAINST STOCK DESIGNATION
		 * BECAUSE IT IS ONLY IT THAT MUST BE NON EMPTY.
		 *
		 * STOCK REFERENCE COULD BE EMPTY AT TIME!
		 */
		marchandisesTableModel.yerothSetFilter_WITH_where_clause(
				QString("%1 = '%2' AND %3 != '%4'").arg(
						YerothDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation,
						YerothDatabaseTableColumn::CATEGORIE,
						aStockServiceCategory));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1477);

		for (int k = 0; k < marchandisesTableModelRowCount; ++k)
		{
			record = marchandisesTableModel.record(k);
			serviceStockCategory = GET_SQL_RECORD_DATA(record,
					YerothDatabaseTableColumn::CATEGORIE);
			if (!YerothUtils::isEqualCaseInsensitive(serviceStockCategory,
					aStockServiceCategory))
			{
				curExistingReferenceCategoryDesignation_PRODUCT_in_out =
						serviceStockCategory;

				marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1519);

				return SERVICE_STOCK_CATEGORY_EXIST;
			}
		}
	}

	/** Verifies whether merchandise already exists **/
	{
		marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1528);

		marchandisesTableModel.yerothSetFilter_WITH_where_clause(
				QString("%1 = '%2' AND %3 = '%4' AND %5='%6'").arg(
						YerothDatabaseTableColumn::REFERENCE,
						aStockServiceReference,
						YerothDatabaseTableColumn::DESIGNATION,
						aStockServiceDesignation,
						YerothDatabaseTableColumn::CATEGORIE,
						aStockServiceCategory));

		marchandisesTableModelRowCount =
				marchandisesTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1508);

		if (marchandisesTableModelRowCount > 0)
		{
			return SERVICE_STOCK_EXIST_IN_MERCHANDISE_DB_TABLE;
		}
	}

	marchandisesTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1548);
	return SERVICE_STOCK_UNDEFINED;
}

QString YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(int day_of_week)
{
	switch (day_of_week)
	{
	case 1:
		return YerothMainWindow::STRING_MONDAY;

	case 2:
		return YerothMainWindow::STRING_TUESDAY;

	case 3:
		return YerothMainWindow::STRING_WEDNESDAY;

	case 4:
		return YerothMainWindow::STRING_THURSDAY;

	case 5:
		return YerothMainWindow::STRING_FRIDAY;

	case 6:
		return YerothMainWindow::STRING_SATURDAY;

	case 7:
		return YerothMainWindow::STRING_SUNDAY;

	default:
		return YerothUtils::EMPTY_STRING;
	}
}

bool YerothUtils::UPDATE_PREVIOUS_SELLING_PRICE_IN_ProductList(
		const YerothERPStockMarchandiseData &aServiceStockData,
		YerothWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;
	QString stockItemUpdateSellingPriceQuery(
			QString("UPDATE %1 SET %2='%3', %4='%5' WHERE %6='%7'").arg(
					YerothDatabase::MARCHANDISES,
					YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
					aServiceStockData._prix_vente_precedent,
					YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
					aServiceStockData._prix_vente_en_gros_precedent,
					YerothDatabaseTableColumn::DESIGNATION,
					aServiceStockData._designation));

	success = YerothUtils::execQuery(stockItemUpdateSellingPriceQuery);

	return success;
}

bool YerothUtils::UPDATE_PREVIOUS_BUYING_PRICE_IN_ProductList(
		const YerothERPStockMarchandiseData &aServiceStockData,
		YerothWindowsCommons *_callingWindow /* = 0 */)
{
	bool ret_value = YerothUtils::execQuery(
			QString("UPDATE %1 SET %2='%3' WHERE %4='%5'").arg(
					YerothDatabase::MARCHANDISES,
					YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
					aServiceStockData._prix_dachat_precedent,
					YerothDatabaseTableColumn::DESIGNATION,
					aServiceStockData._designation), 0);

	return ret_value;
}

bool YerothUtils::insertStockItemInProductList(
		YerothERPStockMarchandiseData &aServiceStockData_IN_OUT,
		YerothWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;

	if (!YerothUtils::creerNouvelleCategorie(
			aServiceStockData_IN_OUT._categorie,
			aServiceStockData_IN_OUT._nom_departement_produit, _callingWindow))
	{
		QString retMsg(
				QObject::tr(
						"La désignation de la catégorie ne doit pas être vide !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("création d'une catégorie"), retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}

		return false;
	}

	YerothSqlTableModel &productListSqlTableModel =
			_allWindows->getSqlTableModel_marchandises();

	QSqlRecord record = productListSqlTableModel.record();

	record.setValue(YerothDatabaseTableColumn::ID,
			YerothERPWindows::getNextIdSqlTableModel_marchandises());

	record.setValue(YerothDatabaseTableColumn::IS_SERVICE,
			aServiceStockData_IN_OUT._isService);

	if (aServiceStockData_IN_OUT._isService)
	{
		record.setValue(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
				aServiceStockData_IN_OUT._nom_entreprise_client);
	}

	record.setValue(YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
			YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_dachat_precedent));

	record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
			YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_vente_en_gros_precedent));

	record.setValue(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
			YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(
					aServiceStockData_IN_OUT._prix_vente_precedent));

	record.setValue(YerothDatabaseTableColumn::REFERENCE,
			aServiceStockData_IN_OUT._reference);

	record.setValue(YerothDatabaseTableColumn::DESIGNATION,
			aServiceStockData_IN_OUT._designation);

	record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			aServiceStockData_IN_OUT._nom_departement_produit);

	record.setValue(YerothDatabaseTableColumn::CATEGORIE,
			aServiceStockData_IN_OUT._categorie);

	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
			aServiceStockData_IN_OUT._description);

	if (!aServiceStockData_IN_OUT._designation.isEmpty())
	{
		if (productListSqlTableModel.Is_SearchQSqlTable(
				YerothDatabaseTableColumn::REFERENCE,
				aServiceStockData_IN_OUT._reference) > 0
				|| productListSqlTableModel.Is_SearchQSqlTable(
						YerothDatabaseTableColumn::DESIGNATION,
						aServiceStockData_IN_OUT._designation) > 0)
		{
			productListSqlTableModel.yerothSetFilter_WITH_where_clause(
					QString("%1='%2' AND %3='%4' AND %5='%6'")
						.arg(YerothDatabaseTableColumn::REFERENCE,
							 aServiceStockData_IN_OUT._reference,
							 YerothDatabaseTableColumn::DESIGNATION,
							 aServiceStockData_IN_OUT._designation,
							 YerothDatabaseTableColumn::CATEGORIE,
							 aServiceStockData_IN_OUT._categorie));

			int rows = productListSqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 1770);

			if (rows > 0)
			{
				record.clear();

				record = productListSqlTableModel.record(0);

				QSqlRecord aMarchandiseUpdateRecord(record);

				if (!aServiceStockData_IN_OUT._prix_dachat_precedent.isEmpty())
				{
					aMarchandiseUpdateRecord.setValue(
							YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
							YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED
										(aServiceStockData_IN_OUT._prix_dachat_precedent));
				}

				if (!aServiceStockData_IN_OUT._prix_vente_precedent.isEmpty())
				{
					aMarchandiseUpdateRecord
						.setValue(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
								  YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED
								  	  (aServiceStockData_IN_OUT._prix_vente_precedent));
				}

				productListSqlTableModel.updateRecord(0,
													  aMarchandiseUpdateRecord,
													  "src/utils/yeroth-erp-utils.cpp",
													  1796,
													  aServiceStockData_IN_OUT._designation);


				productListSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 1803);

				return true;
			}
		}
	}

	success = productListSqlTableModel.insertNewRecord(record,
													   0,
													   "src/utils/yeroth-erp-utils.cpp",
													   1810,
													   aServiceStockData_IN_OUT._designation);

	QString retMsg(
			QObject::tr("Le stock '%1'").arg(
					aServiceStockData_IN_OUT._designation));

	if (success)
	{
		retMsg.append(
				QObject::tr(
						" a été enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::information(_callingWindow,
					QObject::tr("enregistrement de l'article type "
							"dans la liste des marchandises"), retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}
	else
	{
		retMsg.append(
				QObject::tr(
						" n'a pas pu être enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("échec de l'enregistrement de "
							"l'article type dans la liste des marchandises"),
					retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}

	return success;
}

bool YerothUtils::insert_SERVICE_ItemInProductList(
		YerothERPServiceData &aServiceStockData_IN_OUT,
		YerothWindowsCommons *_callingWindow /* = 0 */)
{
	bool success = false;

	if (!YerothUtils::creerNouvelleCategorie(
			aServiceStockData_IN_OUT._categorie,
			aServiceStockData_IN_OUT._nom_departement_produit, _callingWindow))
	{
		QString retMsg(
				QObject::tr(
						"La désignation de la catégorie ne doit pas être vide !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("création d'une catégorie"), retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}

		return false;
	}

	YerothSqlTableModel &productListSqlTableModel =
			_allWindows->getSqlTableModel_marchandises();

	QSqlRecord record = productListSqlTableModel.record();

	record.setValue(YerothDatabaseTableColumn::ID,
			YerothERPWindows::getNextIdSqlTableModel_marchandises());

	record.setValue(YerothDatabaseTableColumn::IS_SERVICE,
			YerothUtils::MYSQL_TRUE_LITERAL);

	record.setValue(YerothDatabaseTableColumn::REFERENCE,
			aServiceStockData_IN_OUT._reference);

	record.setValue(YerothDatabaseTableColumn::DESIGNATION,
			aServiceStockData_IN_OUT._designation);

	record.setValue(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			aServiceStockData_IN_OUT._nom_departement_produit);

	record.setValue(YerothDatabaseTableColumn::CATEGORIE,
			aServiceStockData_IN_OUT._categorie);

	record.setValue(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
			aServiceStockData_IN_OUT._description);

	success = productListSqlTableModel.insertNewRecord(record,
													   0,
													   "src/utils/yeroth-erp-utils.cpp",
													   1923,
													   aServiceStockData_IN_OUT._nom_departement_produit);


	QString retMsg = QObject::tr("Le service '%1 (%2)'")
						.arg(aServiceStockData_IN_OUT._designation,
							 aServiceStockData_IN_OUT._reference);


	if (success)
	{
		retMsg.append(
				QObject::tr(
						" a été enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::information(_callingWindow,
					QObject::tr("enregistrement de l'article type "
								"dans la liste des marchandises"),
								retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}
	else
	{
		retMsg.append(
				QObject::tr(
						" n'a pas pu être enregistré dans la liste des marchandises !"));

		if (0 != _callingWindow)
		{
			YerothQMessageBox::warning(_callingWindow,
					QObject::tr("échec de l'enregistrement de "
							"l'article type dans la liste des marchandises"),
					retMsg);
		}
		else
		{
#ifdef YEROTH_ERP_3_0_TESTING_UNIT_TEST

            qDebug() << retMsg;

#endif
		}
	}

	return success;
}

QString YerothUtils::YEROTH_TRUNCATE_STRING_ACCORDING_TO_SETTING(
		const QString &aString_IN)
{
	QString aString_OUT(aString_IN);

	if (aString_OUT.length() > YerothERPConfig::max_string_display_length)
	{
		aString_OUT.truncate(YerothERPConfig::max_string_display_length);

		aString_OUT.append(".");
	}

	return aString_OUT;
}

QString YerothUtils::GET_STRING_WITH_NUMBERS_WITHIN_KEPT_FOR_LATEX(
		const QString &aStringWITHBLANKSPACEWITHIN)
{
	QString resultString(aStringWITHBLANKSPACEWITHIN);

	unsigned int nextPosIn_RESULT_STRING = 0;

	for (unsigned int k = 0; k < aStringWITHBLANKSPACEWITHIN.length(); ++k)
	{
		if (aStringWITHBLANKSPACEWITHIN.at(k).isDigit())
		{
			resultString.replace(nextPosIn_RESULT_STRING, 1,
					QString("$%1$").arg(aStringWITHBLANKSPACEWITHIN.at(k)));

			nextPosIn_RESULT_STRING += 3;
		}
		else
		{
			nextPosIn_RESULT_STRING += 1;
		}
	}

	return resultString;
}

double YerothUtils::get_prix_dachat_wheter_exists(const QString &stocksID)
{
	QString prixDachatQueryString(
			QString("SELECT %1 FROM %2 WHERE %3='%4'").arg(
					YerothDatabaseTableColumn::PRIX_DACHAT,
					YerothDatabase::ACHATS,
					YerothDatabaseTableColumn::STOCKS_ID, stocksID));

	//    QDEBUG_STRING_OUTPUT_2("prixDachatQueryString", prixDachatQueryString);

	QSqlQuery prixDachatQuery;

	int achatQuerySize = YerothUtils::execQuery(prixDachatQuery,
			prixDachatQueryString);

	double prix_dachat = 0.0;

	if (achatQuerySize > 0 && prixDachatQuery.next())
	{
		prix_dachat = prixDachatQuery.value(0).toDouble();
	}

	return prix_dachat;
}

bool YerothUtils::REMOVE_QSTRING_LIST(
		QStringList &a_qstring_list_to_remove_elements,
		QStringList &a_qstring_list)
{
	bool ret_value = false;

	for (uint i = 0; i < a_qstring_list.size(); ++i)
	{
		ret_value = ret_value
				|| a_qstring_list_to_remove_elements.removeOne(
						a_qstring_list.at(i));
	}

	return ret_value;
}

bool YerothUtils::start_db_transaction()
{
	QSqlDriver *driver = _allWindows->getDatabase().driver();

	if (0 != driver && driver->hasFeature(QSqlDriver::Transactions))
	{
		bool couldStartDBTransaction = _allWindows->getDatabase().transaction();

		if (!couldStartDBTransaction)
		{
			qDebug() << QString("Couldn't start a database transaction !");

			qDebug() << _allWindows->getDatabase().lastError();
		}

		return couldStartDBTransaction;
	}

	return false;
}

bool YerothUtils::rollback_db_transaction()
{
	bool could_rollback_db_transaction = _allWindows->getDatabase().roolback();

	if (!could_rollback_db_transaction)
	{
		qDebug() << QString("Couldn't roolback a database transaction !");

		qDebug() << _allWindows->getDatabase().lastError();
	}

	return could_rollback_db_transaction;
}

bool YerothUtils::commit_db_transaction()
{
	bool couldCommitDBTransaction = _allWindows->getDatabase().commit();

	if (!couldCommitDBTransaction)
	{
		qDebug() << QString("Couldn't commit a database transaction !");

		qDebug() << _allWindows->getDatabase().lastError();
	}

	return couldCommitDBTransaction;
}

void YerothUtils::setAllWindows(YerothERPWindows *allWindows)
{
	_allWindows = allWindows;

	YerothTableView::YEROTH_TABLE_VIEW_ALL_WINDOWS_POINTER = allWindows;

	YerothPOSDialogCommons::setAllWindows(allWindows);

	YerothWindowsCommons::setAllWindows(allWindows);

	YerothPOSAdminWindowsCommons::setAllWindows(allWindows);
}

YerothDatabase* YerothUtils::getDatabase()
{
	if (0 != _allWindows)
	{
		return &_allWindows->getDatabase();
	}

	return 0;
}

void YerothUtils::writeStringToQFilewithUTF8Encoding(
		QFile &latexContentOutputFile, QString aLatexFileContentString)
{
	if (latexContentOutputFile.open(QFile::WriteOnly))
	{
		latexContentOutputFile.write(aLatexFileContentString.toUtf8());
	}

	latexContentOutputFile.close();
}

const QString YerothUtils::getCurrentAdminWindowReceiptsFormatAccordingToLanguage(
		const QString &currentReceiptFormatValue)
{
	QString resultReceiptFormat(currentReceiptFormatValue);

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                            QObject::tr(RECEIPT_FORMAT_SMALL_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_SMALL_EN);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR)))
    {
        resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN);
    }

#else

	if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
			QObject::tr(RECEIPT_FORMAT_SMALL_EN)))
	{
		resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_SMALL_FR);
	}
	else if (YerothUtils::isEqualCaseInsensitive(currentReceiptFormatValue,
			QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
	{
		resultReceiptFormat = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
	}

#endif

	return resultReceiptFormat;
}

void YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage(
		const QString &currentFacturesTypeValue)
{
	//qDebug() << QString("YerothUtils::saveCurrentAdminWindowFacturesTypeAccordingToLanguage");

	assert(0 != _allWindows);

	QString currentFacturesTypeValue_FR(YerothERPConfig::receiptFormat);

#ifdef YEROTH_ENGLISH_LANGUAGE

    if (YerothUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                            QObject::tr(RECEIPT_FORMAT_SMALL_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_SMALL_FR);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A5PAPER_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_LARGE_A5PAPER_FR);
    }
    else if (YerothUtils::isEqualCaseInsensitive(currentFacturesTypeValue,
                                                 QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_EN)))
    {
        currentFacturesTypeValue_FR = QObject::tr(RECEIPT_FORMAT_LARGE_A4PAPER_FR);
    }

#else

	currentFacturesTypeValue_FR = currentFacturesTypeValue;

#endif

	YerothERPConfig::receiptFormat = currentFacturesTypeValue;
//    qDebug() << QString("++ saveCu... currentFacturesTypeValue_FR: %1")
//                              .arg(currentFacturesTypeValue_FR);
}


bool YerothUtils::executer_fichier_sql(const QString &fileName,
                                       YerothLogger  *logger)
{
	QFile file(fileName);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//QDEBUG_STRING_OUTPUT_2("YerothUtils::executer_fichier_sql",
        //                        fileName);

		QMessageBox::critical(0, QObject::tr("Cannot open file"),
				QObject::tr(
						"Unable to open file establish a database connection.\n"
								"This example needs SQLite support. Please read "
								"the Qt SQL driver documentation for information how "
								"to build it.\n\n"
								"Click Cancel to exit."), QMessageBox::Cancel);

		return false;
	}

	static QSqlQuery query;

	QTextStream in(&file);

	query.clear();

	QString line;

	bool success = false;

	while (!in.atEnd())
	{
		line = in.readLine();

		success = query.exec(line);

		if (logger)
		{
			logger->log("YerothUtils::executer_fichier_sql",
					QString("[%1] %2").arg(
							success ?
									BOOLEAN_STRING_TRUE : BOOLEAN_STRING_FALSE,
							line));
		}

		if (!success)
		{
			if (logger)
			{
				logger->log("YerothUtils::executer_fichier_sql",
						QString("[reason for failing] %1").arg(
								query.lastError().text()));
			}
		}
	}

	return true;
}


double YerothUtils::YEROTH_CONVERT_QSTRING_TO_DOUBLE_LOCALIZED(const QString &aDoubleQString)
{
	double aDoubleNum = 0.0;

	bool ok = true;

	aDoubleNum = aDoubleQString.toDouble(&ok);

	if (false == ok)
	{
		if (YerothMainWindow::LANGUE_ANGLAISE)
		{
			aDoubleNum = YerothUtils::englishLocale.toDouble(aDoubleQString, &ok);
		}
		else
		{
			aDoubleNum = YerothUtils::frenchLocale.toDouble(aDoubleQString, &ok);
		}
	}

	return aDoubleNum;
}


int YerothUtils::execQueryRowCount(const QString &strQuery,
								   YerothLogger  *logger)
{
	//qDebug() << "[YerothUtils][execQuery]";

	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();
	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t[reason for failing] " << query.lastError();
		}
	}

	return query.size();
}

void YerothUtils::qDebugStrings(const QString &firstString,
		const QStringList &aStringList)
{
	qDebug() << QString("++ %1: ").arg(firstString) << aStringList;
}

void YerothUtils::qDebugStrings(const QString &firstString,
		const QString &secondString /* = YerothUtils::EMPTY_STRING */)
{
	qDebug() << QString("++ %1: %2").arg(firstString, secondString);
}

bool YerothUtils::execQuery(const QString &strQuery, YerothLogger *logger)
{
	//qDebug() << "[YerothUtils][execQuery]";

	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();

	//qDebug() << "\t[" << success << "]" << query.executedQuery();
	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t" << strQuery;

			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}
	}

	return success;
}

int YerothUtils::execQuery(QSqlQuery &query, const QString &strQuery,
		YerothLogger *logger)
{
	//qDebug() << "[YerothUtils][execQuery]";
	query.prepare(strQuery);

	bool success = query.exec();

	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << strQuery << "\t\t[reason for failing] "
					<< query.lastError().text() << "\n";
		}
		else
		{
			qDebug() << "\t\t" << strQuery;

			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}

		return -1;
	}

	return query.size();
}

int YerothUtils::execQuery(QSqlQuery &query, YerothLogger *logger)
{
	//qDebug() << "[YerothUtils][execQuery]";

	bool success = query.exec();
	//qDebug() << "\t[" << success << "]" << query.executedQuery();

	if (logger)
	{
		logger->log("execQuery",
				QString("[%1] %2").arg(BOOL_TO_STRING(success),
						query.executedQuery()));
	}

	if (!success)
	{
		if (logger)
		{
			*logger << "\t\t[reason for failing] " << query.lastError().text()
					<< "\n";
		}
		else
		{
			qDebug() << "\t\t[reason for failing] " << query.lastError().text();
		}

		return -1;
	}

	return query.size();
}

void YerothUtils::loadPixmapFromDB(QLabel &label_image, QVariant imageVariant,
		const char *format)
{
	QPixmap pixmap;

	pixmap.loadFromData(imageVariant.toByteArray(), format);

	label_image.setAlignment(Qt::AlignCenter);

	label_image.setFrameShape(QFrame::NoFrame);

	label_image.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	label_image.setBackgroundRole(QPalette::Background);

	label_image.setAutoFillBackground(true);

	label_image.setScaledContents(true);

	label_image.setPixmap(pixmap);
}

void YerothUtils::savePixmapToByteArray(QByteArray &bytes,
		const QPixmap &pixmap, const char *format)
{
	QBuffer buffer(&bytes);

	buffer.open(QIODevice::WriteOnly);

	pixmap.save(&buffer, format);
}

void YerothUtils::savePixmapToFile(const QString &aFileFullPath,
		const QPixmap &pixmap, const char *format)
{
	QFile aFile(aFileFullPath);

	aFile.open(QIODevice::WriteOnly);

	pixmap.save(&aFile, format);
}

void YerothUtils::supprimer_image(QLabel &aLabel_image_produit)
{
	aLabel_image_produit.clear();

	aLabel_image_produit.setAutoFillBackground(false);
}

void YerothUtils::selectionner_image(QWidget *parent, QLabel &label_image)
{
	QString path;
	path =
			QFileDialog::getOpenFileName(parent, "Choisir une photo du produit",
					QString::null,
					QObject::tr(
							"Images (*.png *.xpm *.jpg *.jpeg *.gif *.PNG *.XPM *.JPG *.JPEG *.GIF)"));

	QImageReader image_reader(path);

	QImage image;

	image_reader.setAutoTransform(true);

	if (image_reader.read(&image))
	{
		QPixmap pixMap(QPixmap::fromImage(image));

		label_image.setAlignment(Qt::AlignCenter);

		label_image.setFrameShape(QFrame::NoFrame);

		label_image.setSizePolicy(QSizePolicy::Expanding,
				QSizePolicy::Expanding);

		label_image.setBackgroundRole(QPalette::Background);

		label_image.setAutoFillBackground(true);

		label_image.setScaledContents(true);

		label_image.setPixmap(pixMap);

		label_image.setEnabled(true);
	}
}

void YerothUtils::CREATE_AN_EMPTY_FILE(const QString &aFileFullPath)
{
	QImage image(":YEROTH.JPG");

	QPixmap pixMap(QPixmap::fromImage(image));

	QFile empty_jpg_yeroth_file(aFileFullPath);

	pixMap.save(&empty_jpg_yeroth_file, "JPG");

	empty_jpg_yeroth_file.close();
}

QString YerothUtils::get_text_sql_escaped(const QVariant &qv)
{
	QString retStr;

	switch (qv.type())
	{
	case QVariant::UInt:
		return QString::number(qv.toUInt());

	case QVariant::Int:
		return QString::number(qv.toInt());

	case QVariant::Double:
		return QString::number(qv.toDouble(), 'f', 2);

	case QVariant::ULongLong:
		return QString::number(qv.toULongLong());

	case QVariant::LongLong:
		return QString::number(qv.toLongLong());

	case QVariant::Char:
		retStr = QString(qv.toChar());
		break;

	case QVariant::String:
		retStr = qv.toString();
		break;

	case QVariant::Bool:
		retStr.append(qv.toBool());
		break;

	case QVariant::Date:
		retStr = DATE_TO_STRING(qv.toDate());
		break;

	case QVariant::Time:
		retStr = TIME_TO_STRING(qv.toTime());
		break;

	default:
		retStr = QString(qv.type());
		break;
	}

	return YerothUtils::prepareSqlStr(retStr);
}

QString YerothUtils::get_text(const QVariant &qv)
{
	QString retStr;

	switch (qv.type())
	{
	case QVariant::UInt:
		return QString::number(qv.toUInt());

	case QVariant::Int:
		return QString::number(qv.toInt());

	case QVariant::Double:
		return QString::number(qv.toDouble(), 'f', 2);

	case QVariant::ULongLong:
		return QString::number(qv.toULongLong());

	case QVariant::LongLong:
		return QString::number(qv.toLongLong());

	case QVariant::Char:
		if (!qv.isNull())
		{
			return QString(qv.toChar());
		}
		break;

	case QVariant::String:
		if (!qv.isNull())
		{
			return QString(qv.toString());
		}
		break;

	case QVariant::Bool:
		retStr.append(qv.toBool());
		break;

	case QVariant::Date:
		return DATE_TO_STRING(qv.toDate());

	case QVariant::Time:
		return TIME_TO_STRING(qv.toTime());

	default:
		break;
	}

	return retStr;
}

void YerothUtils::getCenterPosition(unsigned desktopWidth,
		unsigned desktopHeight, const QWidget &aWidget, QPoint &result)
{
	int x = desktopWidth / 2 - aWidget.width() / 2;

	int y = desktopHeight / 2 - aWidget.height() / 2 - 25;

	result.setX(x);

	result.setY(y);
}

void YerothUtils::createTableModelHeaders_DEPRECATED(YerothSqlTableModel 	&tableModel,
												     QStandardItemModel 	&stdItemModel,
													 QStringList 			&tableModelHeaders,
													 QStringList 			&tableModelRawHeaders_IN_OUT)
{
	tableModelRawHeaders_IN_OUT.clear();

	tableModelHeaders.clear();

	QString viewStrHdr;

	QString strHdr;

	//Nous mettons les noms des colones
	for (unsigned k = 0; k < tableModel.columnCount(); ++k)
	{
		strHdr = tableModel.record(0).fieldName(k);

		if (!YerothERPConfig::SHOW_DATABASE_RAW)
		{
			viewStrHdr = YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(strHdr);
		}
		else
		{
			viewStrHdr = strHdr;
		}

		tableModelRawHeaders_IN_OUT.append(strHdr);

		tableModelHeaders.append(viewStrHdr);

		stdItemModel.setHeaderData(k, Qt::Horizontal, viewStrHdr);
	}
}


void YerothUtils::createTableModelHeaders(YerothSqlTableModel 	&tableModel,
										  QStandardItemModel 	&stdItemModel,
										  QStringList 			&tableModelHeaders,
										  QStringList 			&tableModelRawHeaders_IN_OUT,
										  YerothWindowsCommons 	*A_CALLING_WINDOW,
										  bool 					CHARGES_FINANCIERES_WINDOW /* = false*/)
{
	tableModelRawHeaders_IN_OUT.clear();

	tableModelHeaders.clear();

	QString viewStrHdr;

	QString strHdr;


	if (0 != A_CALLING_WINDOW)
	{
		if (!CHARGES_FINANCIERES_WINDOW)
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false, false);
		}
		else
		{
			A_CALLING_WINDOW->selectionner_champs_db_visibles(false, true);
		}
	}


	//Nous mettons les noms des colones
	for (unsigned k = 0; k < tableModel.columnCount(); ++k)
	{
		strHdr = tableModel.record(0).fieldName(k);

		if (!YerothERPConfig::SHOW_DATABASE_RAW)
		{
			viewStrHdr = YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(strHdr);
		}
		else
		{
			viewStrHdr = strHdr;
		}

		tableModelRawHeaders_IN_OUT.append(strHdr);

		tableModelHeaders.append(viewStrHdr);

		stdItemModel.setHeaderData(k, Qt::Horizontal, viewStrHdr);
	}
}


QString YerothUtils::generateSqlLike__AS_IS(QString sqlTableColumn,
		QString searchStr)
{
	QString result;

	searchStr.replace("'", "''");

	result.append(sqlTableColumn)
          .append(QString(" LIKE '%1'")
                    .arg(searchStr));

	return result;
}

QString YerothUtils::generateSqlLike(QString sqlTableColumn, QString searchStr)
{
	QString result;

	searchStr.replace("'", "''");

	result.append(sqlTableColumn)
          .append(" LIKE '%")
          .append(searchStr)
          .append("%' ");

	return result;
}

void YerothUtils::addFiltre(YerothLineEdit *aLineEdit, QString filterName,
		QString &yerothFiltre_in_out)
{
	if (aLineEdit && !aLineEdit->text().isEmpty())
	{
		yerothFiltre_in_out.append(
				QString("%1: %2").arg(filterName, aLineEdit->text())).append(
				", ");
	}
}

int YerothUtils::getComboBoxDatabaseQueryValue(
		const QString &comboBoxStringValue, QMap<int, QString> &toViewStringMAP)
{
	int databaseQueryValue = -1;

	QMapIterator<int, QString> it(toViewStringMAP);

	while (it.hasNext())
	{
		it.next();

		databaseQueryValue = it.key();

		if (YerothUtils::isEqualCaseInsensitive(comboBoxStringValue,
				it.value()))
		{
			return databaseQueryValue;
		}
	}

	return databaseQueryValue;
}

void YerothUtils::GET_YEROTH_VIEW_RECORD_WIDTH_LAST_SELECTED_ID(
		YerothSqlTableModel &aYerothSqlTableModel_in_out,
		QSqlRecord &resultSQL_RECORD_in_out)
{
	aYerothSqlTableModel_in_out.yerothSetFilter_WITH_where_clause(
			QString("%1 = '%2'").arg(YerothDatabaseTableColumn::ID,
					YerothERPWindows::get_last_lister_selected_row_db_ID()));

	resultSQL_RECORD_in_out = aYerothSqlTableModel_in_out.record(0);
}

double YerothUtils::GET_QUANTITE_TOTALE_EN_STOCK(
		const QString &nom_departement_produit, const QString &nom_categorie,
		const QString &designation_produit)
{
	double quantite_totale_en_stock = 0.0;

	QString QUERY_QUANTITE_TOTALE_STOCK = QString(
			"select * from %1 where %2='%3' AND %4='%5' AND %6='%7'").arg(
			YerothDatabase::STOCKS,
			YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
			nom_departement_produit, YerothDatabaseTableColumn::CATEGORIE,
			nom_categorie, YerothDatabaseTableColumn::DESIGNATION,
			designation_produit);

	QSqlQuery a_qsql_query;

	int query_size = YerothUtils::execQuery(a_qsql_query,
			QUERY_QUANTITE_TOTALE_STOCK);

	for (uint i = 0; i < query_size && a_qsql_query.next(); ++i)
	{
		quantite_totale_en_stock += a_qsql_query.value(
				YerothDatabaseTableColumn::QUANTITE_TOTALE).toDouble();
	}

	return quantite_totale_en_stock;
}

int YerothUtils::STOCK_PURCHASE_RECORDS_QUANTITY(int stockId)
{
	QString strAchatsQuery(
			QString("SELECT %1 FROM %2 WHERE %3 = '%4'").arg(
					YerothDatabaseTableColumn::ID, YerothDatabase::ACHATS,
					YerothDatabaseTableColumn::STOCKS_ID,
					QString::number(stockId)));

	QSqlQuery query;

	int achatQuerySize = YerothUtils::execQuery(query, strAchatsQuery);

	return achatQuerySize;
}

void YerothUtils::getColumnListString(QStringList &columnStringList,
		const QString &tableName, const QString &fieldName,
		QString conditionStr /* = YerothUtils::EMPTY_STRING */)
{
	columnStringList.clear();

	QString strQuery(
			QString("SELECT %1 FROM %2 ORDER BY `%3` ASC").arg(fieldName,
					tableName, fieldName));

	if (!conditionStr.isEmpty())
	{
		strQuery = QString("SELECT %1 FROM %2 WHERE %3 ORDER BY `%4` ASC").arg(
				fieldName, tableName, conditionStr, fieldName);
	}

	//qDebug() << "++ query: " << strQuery;
	QSqlQuery query;

	query.prepare(strQuery);

	bool success = query.exec();
	//qDebug() << "[" << success << "]" << query.executedQuery();

	if (success)
	{
		QSqlRecord rec = query.record();

		while (query.next())
		{
			QString currentItem(query.value(0).toString());

			if (!currentItem.isEmpty()
					&& !columnStringList.contains(currentItem))
			{
				columnStringList.append(currentItem);
			}
		}
	}
}


QString YerothUtils::GET_YEROTH_QT_OBJECT_NAME(QObject *an_object)
{
	QString YR_OBJECT_NAME;

	if (0 != an_object)
	{
		YerothAdminListerWindow *admin_lister_window
			= dynamic_cast<YerothAdminListerWindow *>(an_object);

		if (0 != admin_lister_window)
		{
			QTabWidget *YR_tabWidget_lister =
					admin_lister_window->get_tabwidget_lister();


			assert(0 != YR_tabWidget_lister);


			QString a_current_tab_index_STRING = QString::number(YR_tabWidget_lister->currentIndex());


			YR_OBJECT_NAME = QString("%1_%2")
							  .arg(admin_lister_window->objectName().trimmed(),
								   a_current_tab_index_STRING);


			YR_OBJECT_NAME = YR_OBJECT_NAME.toUpper();
		}
		else
		{
			YR_OBJECT_NAME = an_object->objectName().toUpper();
		}
	}

	return YR_OBJECT_NAME;
}


bool YerothUtils::checkIf_KEYWORD_ALREADY_EXISTS(YerothWindowsCommons &aCallingWindow,
												 YerothSqlTableModel  &aSqlTableModel,
												 QLineEdit 	  		  &aYerothLineEdit_DB_TO_SEARCH_COLUMN_NAME,
												 const QString 		  &for_user_view_db_column_description,
												 const QString 		  &aDB_TABLE_COLUMN_NAME,
												 int 				  aCurrentClientDetailDBID /* = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	if (aYerothLineEdit_DB_TO_SEARCH_COLUMN_NAME.text().isEmpty())
	{
		return false;
	}

	// ** check if customer account with same name exist
	QString nom_entreprise_filter(
			QString("LOWER(%1) = LOWER('%2')").arg(aDB_TABLE_COLUMN_NAME,
					aYerothLineEdit_DB_TO_SEARCH_COLUMN_NAME.text()));

	//              qDebug() << QString("++ nom_entreprise_filter: %1")
	//                                              .arg(nom_entreprise_filter);

	aSqlTableModel.yerothSetFilter_WITH_where_clause(nom_entreprise_filter);

	int clientsTableModelRowCount = aSqlTableModel.rowCount();

	if (clientsTableModelRowCount > 0)
	{
		if (YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
				!= aCurrentClientDetailDBID)
		{
			for (int k = 0; k < clientsTableModelRowCount; ++k)
			{
				QSqlRecord clientRecord = aSqlTableModel.record(k);

				int clientRecordDBID(GET_SQL_RECORD_DATA(
						clientRecord,
						YerothDatabaseTableColumn::ID).toInt());

				if (aCurrentClientDetailDBID != clientRecordDBID)
				{
					aSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 2873);

					QString retMsg(
							QObject::tr("Une %1 nommé(e) '%2' existe déjà "
									"dans la base de données !").arg(
									for_user_view_db_column_description,
									aYerothLineEdit_DB_TO_SEARCH_COLUMN_NAME.text()));

					YerothQMessageBox::warning(&aCallingWindow,
							QObject::tr("%1 déjà existant(e)").arg(
									for_user_view_db_column_description),
							retMsg);

					return true;
				}
			}
		}
		else
		{
			QString retMsg(
					QObject::tr("Une %1 nommé(e) '%2' existe déjà "
							"dans la base de données !").arg(
							for_user_view_db_column_description,
							aYerothLineEdit_DB_TO_SEARCH_COLUMN_NAME.text()));

			YerothQMessageBox::warning(&aCallingWindow,
					QObject::tr("%1 déjà existant(e)").arg(
							for_user_view_db_column_description), retMsg);

			return true;
		}
	}

	aSqlTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 2906);

	return false;
}

bool YerothUtils::checkIfCustomerAccountAlreadyExist_NOMENTREPRISE(
		YerothWindowsCommons &aCallingWindow,
		YerothSqlTableModel &aClientTableModel,
		YerothLineEdit &aYerothLineEdit_nom_entreprise,
		int aCurrentClientDetailDBID /* = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	// ** check if customer account with same name exist
	QString nom_entreprise_filter(
			QString("LOWER(%1) = LOWER('%2')").arg(
					YerothDatabaseTableColumn::NOM_ENTREPRISE,
					aYerothLineEdit_nom_entreprise.text()));

	//              qDebug() << QString("++ nom_entreprise_filter: %1")
	//                                              .arg(nom_entreprise_filter);

	aClientTableModel.yerothSetFilter_WITH_where_clause(nom_entreprise_filter);

	int clientsTableModelRowCount = aClientTableModel.rowCount();

	if (clientsTableModelRowCount > 0)
	{
		if (YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
				!= aCurrentClientDetailDBID)
		{
			for (int k = 0; k < clientsTableModelRowCount; ++k)
			{
				QSqlRecord clientRecord = aClientTableModel.record(k);

				int clientRecordDBID(GET_SQL_RECORD_DATA(clientRecord,
						YerothDatabaseTableColumn::ID).toInt());

				if (aCurrentClientDetailDBID != clientRecordDBID)
				{
					aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 2944);

					QString retMsg(
							QObject::tr(
									"Une entreprise nommée '%1' existe déjà "
											"dans la base de données !").arg(
									aYerothLineEdit_nom_entreprise.text()));

					YerothQMessageBox::warning(&aCallingWindow,
							QObject::tr("compte client déjà existant"),
							retMsg);

					return true;
				}
			}
		}
		else
		{
			QString retMsg(
					QObject::tr("Une entreprise nommée '%1' existe déjà "
							"dans la base de données !").arg(
							aYerothLineEdit_nom_entreprise.text()));

			YerothQMessageBox::warning(&aCallingWindow,
					QObject::tr("compte client déjà existant"), retMsg);

			return true;
		}
	}

	aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 2974);

	return false;
}

bool YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCE_REGISTRE_DU_COMMERCE(
		YerothWindowsCommons &aCallingWindow,
		YerothSqlTableModel &aClientTableModel,
		YerothLineEdit &aYerothLineEdit_reference_registre_du_commerce,
		int aCurrentClientDetailDBID /* = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */)
{
	// ** check if customer account with same trade registry number exist
	if (!aYerothLineEdit_reference_registre_du_commerce.isEmpty())
	{
		QString reference_du_registre_du_commerce_filter(
				QString("LOWER(%1) = LOWER('%2')").arg(
						YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
						aYerothLineEdit_reference_registre_du_commerce.text()));
		aClientTableModel.yerothSetFilter_WITH_where_clause(
				reference_du_registre_du_commerce_filter);
		int clientsTableModelRowCount = aClientTableModel.rowCount();
		if (clientsTableModelRowCount > 0)
		{
			if (YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
					!= aCurrentClientDetailDBID)
			{
				for (int k = 0; k < clientsTableModelRowCount; ++k)
				{
					QSqlRecord clientRecord = aClientTableModel.record(k);
					int clientRecordDBID(GET_SQL_RECORD_DATA
					(clientRecord,
							YerothDatabaseTableColumn::
							ID).toInt());
					if (aCurrentClientDetailDBID != clientRecordDBID)
					{
						aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 3009);
						QString retMsg(
								QObject::tr(
										"Une entreprise avec la référence "
												"régistre du commerce '%1' existe déjà "
												"dans la base de données !").arg(
										aYerothLineEdit_reference_registre_du_commerce.text()));
						YerothQMessageBox::warning(&aCallingWindow,
								QObject::tr("compte client déjà existant"),
								retMsg);
						return true;
					}
				}
			}
			else
			{
				QString retMsg(
						QObject::tr("Une entreprise avec la référence "
								"régistre du commerce '%1' existe déjà "
								"dans la base de données !").arg(
								aYerothLineEdit_reference_registre_du_commerce.text()));
				YerothQMessageBox::warning(&aCallingWindow,
						QObject::tr("compte client déjà existant"), retMsg);
				return true;
			}
		}
		aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 3035);
	}

	return false;
}

bool YerothUtils::checkIfCustomerAccountAlreadyExist_REFERENCECLIENT(
		YerothWindowsCommons &aCallingWindow,
		YerothSqlTableModel &aClientTableModel,
		YerothLineEdit &aYerothLineEdit_reference_client,
		int aCurrentClientDetailDBID
		/* = YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED */
		)
{
	// ** check if customer account with same reference exist
	if (!aYerothLineEdit_reference_client.isEmpty())
	{
		QString reference_client_filter(
				QString("LOWER(%1) = LOWER('%2')").arg(
						YerothDatabaseTableColumn::REFERENCE_CLIENT,
						aYerothLineEdit_reference_client.text()));
		aClientTableModel.yerothSetFilter_WITH_where_clause(
				reference_client_filter);
		int clientsTableModelRowCount = aClientTableModel.rowCount();
		if (clientsTableModelRowCount > 0)
		{
			if (YerothUtils::CURRENT_CLIENT_DB_ID_UNDEFINED
					!= aCurrentClientDetailDBID)
			{
				for (int k = 0; k < clientsTableModelRowCount; ++k)
				{
					QSqlRecord clientRecord = aClientTableModel.record(k);
					int clientRecordDBID(GET_SQL_RECORD_DATA
					(clientRecord,
							YerothDatabaseTableColumn::
							ID).toInt());
					if (aCurrentClientDetailDBID != clientRecordDBID)
					{
						aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 3073);
						QString retMsg(
								QObject::tr(
										"Une entreprise avec la référence '%1' existe déjà "
												"dans la base de données !").arg(
										aYerothLineEdit_reference_client.text()));
						YerothQMessageBox::warning(&aCallingWindow,
								QObject::tr("compte client déjà existant"),
								retMsg);
						return true;
					}
				}
			}
			else
			{
				QString retMsg(
						QObject::tr(
								"Une entreprise avec la référence '%1' existe déjà "
										"dans la base de données !").arg(
								aYerothLineEdit_reference_client.text()));
				YerothQMessageBox::warning(&aCallingWindow,
						QObject::tr("compte client déjà existant"), retMsg);
				return true;
			}
		}
		aClientTableModel.resetFilter("src/utils/yeroth-erp-utils.cpp", 3098);
	}

	return false;
}

bool YerothUtils::IS_SQL_TABLE_COLUMN_MAXIMUM_SIZE_REACHED(
		int a_sql_table_column_CURRENT_history_row_data_size,
		int a_sql_table_column_NEXT_WRITE_row_data_size,
		int a_sql_table_column_MAXIMUM_SIZE)
{
	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_MAXIMUM_SIZE", a_sql_table_column_MAXIMUM_SIZE);

	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_CURRENT_history_row_data_size",
	//a_sql_table_column_CURRENT_history_row_data_size);

	if (a_sql_table_column_MAXIMUM_SIZE
			<= a_sql_table_column_CURRENT_history_row_data_size)
	{
		return true;
	}

	//QDEBUG_STRING_OUTPUT_2_N("a_sql_table_column_NEXT_WRITE_row_data_size",
	//a_sql_table_column_NEXT_WRITE_row_data_size);

	if (a_sql_table_column_NEXT_WRITE_row_data_size
			>= a_sql_table_column_MAXIMUM_SIZE)
	{
		return true;
	}

	return false;
}

QString YerothUtils::getFileNameWithUserIDAndCurrentTime(
		const QString &fileName)
{
	QString resultFileName(fileName);
	YerothPOSUser *aUser = 0;
	if (0 != YerothUtils::_allWindows)
	{
		aUser = YerothUtils::_allWindows->getUser();
		if (0 != aUser)
		{
			resultFileName.append(QString("-%1").arg(aUser->nom_utilisateur()));
		}
	}

	resultFileName = YerothUtils::getFileNameWithCurrentTime(resultFileName);
	return resultFileName;
}

QString YerothUtils::getWindowTitleWithStrategy(QMainWindow *aMainWindow,
		QString localVisibleStrategy
		/* = "" */
		)
{
	QString aNewTitle(aMainWindow->windowTitle());
	QString currentStockListingStrategy(localVisibleStrategy);
	if (localVisibleStrategy.isEmpty())
	{
		currentStockListingStrategy = YerothERPConfig::salesStrategy;
	}

	if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL))
	{
		aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_ALL,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO))
	{
		aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO))
	{
		aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
				currentStockListingStrategy);
	}
	else if (aNewTitle.contains(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO))
	{
		aNewTitle.replace(YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
				currentStockListingStrategy);
	}
	else
	{
		if (!aNewTitle.contains(YerothERPConfig::salesStrategy))
		{
			aNewTitle.append(" - ").append(currentStockListingStrategy);
		}
	}

	return aNewTitle;
}

void YerothUtils::fillDBTableColumnNameToDBTableColumnType_TEST(
		const QString &aDBTableName,
		QMap<QString, QString> &aDBbTableColumnToType_IN_OUT,
		QMap<QString, bool> &aDBTableColumnToIsNotNULL_IN_OUT)
{
	aDBbTableColumnToType_IN_OUT.clear();
	aDBTableColumnToIsNotNULL_IN_OUT.clear();
	QString strShowColumnQuery(
			QString("SHOW COLUMNS FROM %1").arg(aDBTableName));
	QSqlQuery query;
	bool dbFieldNullAble = false;
	QString dbFieldName;
	QString dbFieldType;
	int querySize = YerothUtils::execQuery(query, strShowColumnQuery);
	for (int j = 0; j < querySize && query.next(); ++j)
	{
		dbFieldName = query.value(0).toString();
		dbFieldType = query.value(1).toString();
		dbFieldNullAble = (query.value(2).toString() == "NO") ? false : true;
		if (false == dbFieldNullAble)
		{
			aDBTableColumnToIsNotNULL_IN_OUT.insert(dbFieldName,
					dbFieldNullAble);
		}

		aDBbTableColumnToType_IN_OUT.insert(dbFieldName, dbFieldType);
	}
}

QString YerothUtils::appendPDFReceiptFormat(QString aStr)
{
	QString resultStr(aStr);
	if (resultStr.contains(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER))
	{
		resultStr.replace(YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
				YerothERPConfig::receiptFormat);
	}
	else if (resultStr.contains(YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER))
	{
		resultStr.replace(YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
				YerothERPConfig::receiptFormat);
	}
	else if (resultStr.contains(YerothERPConfig::RECEIPT_FORMAT_PETIT))
	{
		resultStr.replace(YerothERPConfig::RECEIPT_FORMAT_PETIT,
				YerothERPConfig::receiptFormat);
	}
	else
	{
		if (!resultStr.contains(YerothERPConfig::receiptFormat))
		{
			resultStr.append(
					QObject::tr("\t[ factures pdf au format: '%1' ]").arg(
							YerothERPConfig::receiptFormat));
		}
	}

	return resultStr;
}


QString YerothUtils::GET_LOCALISATION_FOR_EMPLOYEE_PAY_GROUP
                        (const QString &an_employee_PAY_GROUP)
{
	QString CUR_GET_employee_pay_group_LOCALISATION_ONLY =
        QString("select %1 from %2 where %3='%4'")
            .arg(YerothDatabaseTableColumn::LOCALISATION,
                 YerothDatabase::GROUPES_DE_PAIE_hr,
                 YerothDatabaseTableColumn::DESIGNATION,
                 an_employee_PAY_GROUP);

	QSqlQuery a_qsql_query;

	int query_size =
        YerothUtils::execQuery(a_qsql_query,
                               CUR_GET_employee_pay_group_LOCALISATION_ONLY);

	if (query_size > 0   &&
        a_qsql_query.next())
	{
		return a_qsql_query.value(0).toString();
	}

	return YerothUtils::EMPTY_STRING;
}


QString YerothUtils::GET_LOCALISATION_FOR_CLIENT_LOYALTY_PROGRAM
                        (const QString &client_loyalty_program)
{
	QString CUR_GET_CLIENT_LOYALTY_PROGRAM_LOCALISATION_ONLY =
        QString("select %1 from %2 where %3='%4'")
            .arg(YerothDatabaseTableColumn::LOCALISATION,
                 YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                 YerothDatabaseTableColumn::DESIGNATION,
                 client_loyalty_program);

	QSqlQuery a_qsql_query;

	int query_size =
        YerothUtils::execQuery(a_qsql_query,
                               CUR_GET_CLIENT_LOYALTY_PROGRAM_LOCALISATION_ONLY);

	if (query_size > 0   &&
        a_qsql_query.next())
	{
		return a_qsql_query.value(0).toString();
	}

	return YerothUtils::EMPTY_STRING;
}


QString YerothUtils::getStrategySqlQueryStr(QString aSelectStmt,
		QString aStrategy)
{
	QString resultStrategyQueryStr;
	if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 ASC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_PEREMPTION);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 ASC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_PEREMPTION);
		}
	}
	else if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 ASC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_ENTREE);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 ASC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_ENTREE);
		}
	}
	else if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO, aStrategy))
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC, %3 DESC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_ENTREE);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC, %3 DESC").arg(
					aSelectStmt, YerothDatabaseTableColumn::DESIGNATION,
					YerothDatabaseTableColumn::DATE_ENTREE);
		}
	}
	else	//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
	{
		if (!aSelectStmt.contains("ORDER BY", Qt::CaseInsensitive))
		{
			resultStrategyQueryStr = QString("%1 ORDER BY %2 ASC").arg(
					aSelectStmt, YerothDatabaseTableColumn::ID);
		}
		else
		{
			resultStrategyQueryStr = QString("%1, %2 ASC").arg(aSelectStmt,
					YerothDatabaseTableColumn::ID);
		}
	}

	return resultStrategyQueryStr;
}


void YerothUtils::infosEntreprise(YerothPOSAdminWindowsCommons 	&aYerothPOSAdminQMainWindow,
								  const QString 				&infos)
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        YerothQMessageBox::information(&aYerothPOSAdminQMainWindow,
        							   QString("Information about the company"),
									   infos);
    }
    else
    {
        YerothQMessageBox::information(&aYerothPOSAdminQMainWindow,
        							   QString("Informations sur l'entreprise"),
									   infos);
    }
}


//!!! #define DOUBLE_FROM_LOCALIZED_STRING(NUM) YerothUtils::frenchLocale.toDouble(NUM) !!!

double YerothUtils::localized_string__get_double_FROM_LOCALIZED_STRING(QString DOUBLE_NUMBER_STRING)
{
	double result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toDouble(DOUBLE_NUMBER_STRING);
	}
	else
	{
		result = YerothUtils::frenchLocale.toDouble(DOUBLE_NUMBER_STRING);
	}

	return result;
}


QString YerothUtils::localized_string__get_number_string(int INT_NUMBER)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toString(INT_NUMBER);
	}
	else
	{
		result = YerothUtils::frenchLocale.toString(INT_NUMBER);
	}

	return result;
}


QString YerothUtils::localized_string__get_double_string(double DOUBLE_NUMBER)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', 2);
	}
	else
	{
		result = YerothUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', 2);
	}

	return result;
}


QString YerothUtils::localized_string__get_double_string_P(double DOUBLE_NUMBER,
													 	   int NUMBER_OF_DECIMALS_after)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after);
	}
	else
	{
		result = YerothUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after);
	}

	return result;
}


QString YerothUtils::localized_string__get_CURRENCY_double_string(double DOUBLE_NUMBER)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', 2)
										   .append(" ")
										   .append(YerothERPConfig::currency);
	}
	else
	{
		result = YerothUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', 2)
										  .append(" ")
										  .append(YerothERPConfig::currency);
	}

	return result;
}


QString YerothUtils::localized_string__get_CURRENCY_double_string_P(double DOUBLE_NUMBER,
															  	    int NUMBER_OF_DECIMALS_after)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after)
										   .append(" ")
										   .append(YerothERPConfig::currency);
	}
	else
	{
		result = YerothUtils::frenchLocale.toString(DOUBLE_NUMBER, 'f', NUMBER_OF_DECIMALS_after)
										  .append(" ")
										  .append(YerothERPConfig::currency);
	}

	return result;
}


QString YerothUtils::localized_string__get_MONTH_NAME_LOCALIZED(int month_name)
{
	QString result;

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		result = YerothUtils::englishLocale.monthName(month_name);
	}
	else
	{
		result = YerothUtils::frenchLocale.monthName(month_name);
	}

	return result;
}


void YerothUtils::getCurrentSimplifiedDate(QString 		&date_IN_OUT,
										   const QDate 	&aDate)
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	date_IN_OUT.append(QString(", The %1 at %2")
    						.arg(aDate.toString("dd-MM-yyyy"),
    							 CURRENT_TIME));

    	date_IN_OUT = YerothUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
    else
    {
    	date_IN_OUT.append(QString(", Le %1 à %2")
							.arg(aDate.toString("dd-MM-yyyy"),
								 CURRENT_TIME));

    	date_IN_OUT = YerothUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
}


void YerothUtils::getCurrentLocaleDate(QString 		&date_IN_OUT,
									   const QDate 	&aDate)
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        QString d(YerothUtils::englishLocale.toString(aDate));

        int firstCommaIndex = d.indexOf(",", 0);

        d.replace(firstCommaIndex, 1, " ");

        date_IN_OUT.append(", ").append(d);

        date_IN_OUT = YerothUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);
    }
    else
    {
        date_IN_OUT.append(", le ")
        		   .append(YerothUtils::frenchLocale.toString(aDate));

        //qDebug() << "++ before date: " << date_IN_OUT << " (" << date_IN_OUT.indexOf(QString::fromUtf8("û")) << ")";
        date_IN_OUT = YerothUtils::LATEX_IN_OUT_handleForeignAccents(date_IN_OUT);

        //qDebug() << "++ after date: " << date_IN_OUT << " (" << date_IN_OUT.indexOf(QString::fromUtf8("û")) << ")";
    }
}


bool YerothUtils::slot_connecter_localisation(QWidget &aWidget,
											  YerothERPWindows *allWindows,
											  const QString &localisation)
{
	YerothSqlTableModel &localisationsSqlTableModel =
			allWindows->getSqlTableModel_localisations();

	localisationsSqlTableModel.yerothSetFilter_WITH_where_clause(
			GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_LOCALISATION,
					localisation));

	int rowCount = localisationsSqlTableModel.easySelect("src/utils/yeroth-erp-utils.cpp", 3418);

	if (rowCount <= 0)
	{
		//qDebug() << "++ pas de localisation au nom de: " << localisation;
		return false;
	}

	QSqlRecord localisationsRecord = localisationsSqlTableModel.record(0);

	QString addresseIP(
			GET_SQL_RECORD_DATA(localisationsRecord,
					YerothDatabaseTableColumn::ADRESSE_IP));

	QString widgetTitle(aWidget.windowTitle());

	if (addresseIP.isEmpty())
	{
		QString retMsg;

		retMsg.append(QObject::tr("La localisation %1 n'a pas d'adresse IP enregistrée !")
							.arg(localisation));

		QString msg(QObject::tr("%1 - pas d'adresse IP !").arg(widgetTitle));

		YerothQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		return false;
	}
	else
	{
		if (YerothUtils::isEqualCaseInsensitive(
				ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS, addresseIP))
		{
			QString retMsg;

			retMsg.append(QObject::tr("La localisation '%1' est "
										  "déjà connectée par l'adresse "
										  "IP loopback ('%2') !")
								.arg(localisation,
									 YerothUtils::ETHERNET_LOCALHOST_LOOPBACK_IP_ADDRESS));

			YerothQMessageBox::information(_allWindows->_mainWindow,
										   QObject::tr("%1 - Connection par adresse IP !").arg(widgetTitle),
										   retMsg);

			return false;
		}
	}

	YerothDatabase &database = allWindows->getDatabase();

	database.close();

	database.set_db_ip_address(addresseIP);

	if (database.open())
	{
		YerothERPConfig::_distantSiteConnected = true;
		YerothERPConfig::_connectedSiteIPAddress = addresseIP;
		YerothERPConfig::_connectedSite = localisation;

		allWindows->reinitialiseSqlTableModels();

		QString retMsg(
				QObject::tr("Connecter à la localisation %1 !").arg(
						localisation));

		QString msg(
				QObject::tr("%1 - Connection à la localisation !").arg(
						widgetTitle));

		YerothQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		return true;
	}
	else
	{
		QString retMsg(
				QObject::tr(
						"Impossible de se connecter à la localisation %1 !").arg(
								localisation));
		QString msg(
				QObject::tr("%1 - Pas de connection à la localisation !").arg(
						widgetTitle));

		YerothQMessageBox::information(_allWindows->_mainWindow,
									   msg,
									   retMsg);

		qDebug() << database.lastError();

		slot_deconnecter_localisation(allWindows);
	}

	return false;
}


bool YerothUtils::slot_deconnecter_localisation(YerothERPWindows *allWindows)
{
	if (!YerothERPConfig::_distantSiteConnected)
	{
		return true;
	}

	YerothDatabase &database = allWindows->getDatabase();
	database.close();
	YerothERPConfig::_distantSiteConnected = false;
	YerothERPConfig::_connectedSite.clear();
	database.set_db_ip_address(YerothERPConfig::_db_ip_address);
	if (!database.open())
	{
		QString errMsg(
				QObject::tr("Impossible de se connecter au serveur '%1' "
						"de base de données !\n"
						"Contacter l'administrateur de yeroth\n\n"
						"Cliquer sur 'Cancel' pour terminer %2").arg(
						database.db_type(), YerothUtils::APPLICATION_NAME));
		QMessageBox::critical(0, YerothUtils::APPLICATION_NAME, errMsg,
				QMessageBox::Cancel);
		qDebug() << database.lastError();
		return false;
		//logger.log("main",
		//QString("Impossible de se connecter au serveur MYSQL: %1 !").arg(db.lastError().text()));
	}
	//else
	//{
	allWindows->reinitialiseSqlTableModels();
	return true;
	//}
}

QString YerothUtils::colorLatexTextInput(const QString &colorSpec,
		const QString &latexString_in)
{
	return QString(
			QString("\\textcolor{%1}{%2}").arg(colorSpec, latexString_in));
}

QString YerothUtils::LATEX_IN_OUT_handleForeignAccents(
		const QString &texText_in)
{
	if (texText_in.isEmpty())
	{
		return YerothUtils::EMPTY_STRING;
	}

	QString tempText(texText_in);
	tempText =
			tempText.isEmpty() ?
					"\"\"" : tempText.replace(EMPTY_SPACE_REGEXP, "\\ ");
	tempText.replace("º", "\\textdegree");
	tempText.replace("#", "\\#");
	tempText.replace("_", "\\_");
	tempText.replace("%", "\\%");
	tempText.replace("&", "\\&");
	tempText.replace("$", "\\$");
	tempText.replace(QString::fromUtf8("ç"), "\\c{c}");
	tempText.replace(QString::fromUtf8("è"), "\\`e");
	tempText.replace(QString::fromUtf8("é"), "\\'e");
	tempText.replace(QString::fromUtf8("ê"), "\\^e");
	tempText.replace(QString::fromUtf8("à"), "\\`a");
	tempText.replace(QString::fromUtf8("â"), "\\^a");
	tempText.replace(QString::fromUtf8("ô"), "\\^o");
	tempText.replace(QString::fromUtf8("î"), "\\^i");
	tempText.replace(QString::fromUtf8("û"), "\\^u");
	tempText.replace(QString::fromUtf8("È"), "\\`E");
	tempText.replace(QString::fromUtf8("É"), "\\'E");
	tempText.replace(QString::fromUtf8("Ê"), "\\^E");
	tempText.replace(QString::fromUtf8("À"), "\\`A");
	tempText.replace(QString::fromUtf8("Â"), "\\^A");
	tempText.replace(QString::fromUtf8("Ô"), "\\^O");
	tempText.replace(QString::fromUtf8("Î"), "\\^I");
	tempText.replace(QString::fromUtf8("Û"), "\\^U");
	return tempText;
}

void YerothUtils::cleanUpTexTableLastString(QString &texTable_in_out)
{
	int lastPos = texTable_in_out.size() - 1;
	if (YerothUtils::isEqualCaseInsensitive("&", texTable_in_out.at(lastPos)))
	{
		texTable_in_out.remove(lastPos, 1);
	}
}

void YerothUtils::handleFactureTexTableItemText(int texTableColumnCount,
		QString &texTable_IN_OUT, int itemTextColumnPosition,
		const QString &itemText)
{
	QString resultItemText(itemText);
	if (!itemText.isEmpty())
	{
		resultItemText = LATEX_IN_OUT_handleForeignAccents(resultItemText);
		texTable_IN_OUT.append(resultItemText);
	}

	if (itemTextColumnPosition < texTableColumnCount - 1)
	{
		texTable_IN_OUT.append(" &");
	}
	else
	{
		texTable_IN_OUT.append(" \\\\").append("\n");
	}
}


void YerothUtils::handleTexTableItemText(int 			texTableColumnCount,
										 QString 		&texTable_IN_OUT,
										 int 			itemTextColumnPosition,
										 const QString  &itemText)
{
	QString resultItemText(LATEX_IN_OUT_handleForeignAccents(itemText));

	if (!resultItemText.isEmpty())
	{
		texTable_IN_OUT.append(resultItemText);
	}

	if (itemTextColumnPosition < texTableColumnCount - 1)
	{
		texTable_IN_OUT.append(" &");
	}
	else
	{
		texTable_IN_OUT.append(" \\\\").append("\n");
	}
}


QString YerothUtils::IMPRIMER_FACTURE_FOR_CLIENT_ACCOUNT_SALE_INVOICE_FACTURE(
		YerothWindowsCommons &current_printing_window,
		YEROTH_CLIENT_ACCOUNT_SALE_info &yeroth_client_account_sale_info)
{
	YerothPOSUser *yerothUser = _allWindows->getUser();
	YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();
	QString factureTexDocument;
	QString factureDate(infoEntreprise.getVille_LATEX());
	YerothUtils::getCurrentLocaleDate(factureDate);

    YerothUtils::get_FACTURE_INVOICE_TexDocumentString(factureTexDocument);

	if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::RECEIPT_FORMAT_GRAND_A4PAPER,
			YerothERPConfig::receiptFormat))
	{
		factureTexDocument.replace("YEROTHPAPERSPEC", "a4paper");
	}
	else if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::RECEIPT_FORMAT_GRAND_A5PAPER,
			YerothERPConfig::receiptFormat))
	{
		factureTexDocument.replace("YEROTHPAPERSPEC", "a5paper");
	}

	factureTexDocument.replace("YEROTHTYPEDEVENTE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getSaleType()));
	factureTexDocument.replace("YEROTHLOCALISATIONEMISSIONINVOICE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getSiteOfInvoiceEmission()));
	factureTexDocument.replace("YEROTHFACTUREINVOICEPRODUCTREFERENCE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getProductReference()));
	factureTexDocument.replace("YEROTHFACTUREINVOICEPRODUCTNAME",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getProductName()));
	factureTexDocument.replace("YEROTHFACTUREINVOICECATEGORY",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getCategory()));
	factureTexDocument.replace("YEROTHFACTUREINVOICESALEQUANTITY",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getQuantity()));
	factureTexDocument.replace("YEROTHFACTUREINVOICEUNITPRICE",
			GET_DOUBLE_STRING(
					yeroth_client_account_sale_info.getUnitPrice().toDouble()));
	factureTexDocument.replace("YEROTHFACTUREINVOICETAX",
			GET_DOUBLE_STRING(
					yeroth_client_account_sale_info.getTaxes().toDouble()));
	QString referenceFACTURECompteClient(
			YerothUtils::GET_REFERENCE_RESTE_FACTURE_CLIENT(
					yeroth_client_account_sale_info.get_db_ID()));
	factureTexDocument.replace("YEROTHNUMEROFACTUREINVOICE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					referenceFACTURECompteClient));
	/*
	 * LA RÉFÉRENCE DU DERNIER PAIEMENT.
	 * ELLE EST UTILISÉ DANS LE CHAMPS TEXTUEL: 'notre référence'
	 * DE LA FACTURE PRÉSENTÉE AU CLIENT.
	 *
	 * 'notre référence' PERMET AU VENDEUR DE NE PAS GÉNÉRER PLUS
	 * D'1 FACTURE AU RISQUE DE PERTURBER LE CLIENT.
	 */
	YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;
	QString query_last_payment_FOR_THIS_INVOICE_GENERATION(
			QString(
					"SELECT %1 from %2 WHERE %3='%4' ORDER BY CONCAT(%5,' ',%6) desc limit 1").arg(
					YerothDatabaseTableColumn::ID, YerothDatabase::PAIEMENTS,
					YerothDatabaseTableColumn::REFERENCE,
					yeroth_client_account_sale_info.getProductReference(),
					YerothDatabaseTableColumn::DATE_PAIEMENT,
					YerothDatabaseTableColumn::HEURE_PAIEMENT));
//      QDEBUG_STRING_OUTPUT_2("query_last_payment_FOR_THIS_INVOICE_GENERATION",
//                                                 query_last_payment_FOR_THIS_INVOICE_GENERATION);
	QSqlQuery a_qsql_query;
	int a_query_size_FOR_INVOICE = YerothUtils::execQuery(a_qsql_query,
			query_last_payment_FOR_THIS_INVOICE_GENERATION);
	bool found_last_payment_CORRESPONDING_TO_THIS_INVOICE_GENERATION =
			(a_query_size_FOR_INVOICE > 0 && a_qsql_query.next());
	if (found_last_payment_CORRESPONDING_TO_THIS_INVOICE_GENERATION)
	{
		QString db_ID_of_last_payment = a_qsql_query.value(
				YerothDatabaseTableColumn::ID).toString();
		factureTexDocument.replace(
				"YEROTHVENTECOMPTECLIENTREFERENCEINVOICEFACTURE",
				YerothUtils::LATEX_IN_OUT_handleForeignAccents(
						QString("%1_%2").arg(referenceFACTURECompteClient,
								db_ID_of_last_payment)));
	}
	else
	{
		factureTexDocument.replace(
				"YEROTHVENTECOMPTECLIENTREFERENCEINVOICEFACTURE",
				YerothUtils::LATEX_IN_OUT_handleForeignAccents(
						referenceFACTURECompteClient));
	}

	YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;
	factureTexDocument.replace("YEROTHFACTUREINVOICEREMAININGAMOUNT",
			GET_CURRENCY_STRING_NUM(
					yeroth_client_account_sale_info.getTotalRemainingAmount().toDouble()));
	QString YEROTHMONTANTLITERAL(
			QString("%1 %2").arg(
					GET_SPELLED_NUMBER(
							yeroth_client_account_sale_info.getTotalRemainingAmount().toDouble()),
					YerothERPConfig::currency));
	factureTexDocument.replace("YEROTHMONTANTLITERAL",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					YEROTHMONTANTLITERAL.toUpper()));
	factureTexDocument.replace("YEROTHFACTUREINVOICETOTALATC",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getTaxes()));
	factureTexDocument.replace("YEROTHFACTUREINVOICETOTALWITHCURRENCYATC",
			QString("%1").arg(
					GET_DOUBLE_STRING(
							yeroth_client_account_sale_info.getTotalAmountAtc().toDouble())));
	factureTexDocument.replace("YEROTHENTREPRISE",
			infoEntreprise.getNomCommercial_LATEX());
	factureTexDocument.replace("YEROTHACTIVITESENTREPRISE",
			infoEntreprise.getSecteursActivitesTex());
	factureTexDocument.replace("YEROTHBOITEPOSTALE",
			infoEntreprise.getBoitePostal());
	factureTexDocument.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
	factureTexDocument.replace("YEROTHLOCALISATION_SORTIE",
			infoEntreprise.getLocalisation_LATEX());
	factureTexDocument.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
	factureTexDocument.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
	factureTexDocument.replace("YEROTHTELEPHONE",
			infoEntreprise.getTelephone());
	factureTexDocument.replace("YEROTHDATE", factureDate);
	factureTexDocument.replace("YEROTHHEUREVENTE",
	CURRENT_TIME);
	factureTexDocument.replace("YEROTHVENDEUR", yerothUser->nom_completTex());
	factureTexDocument.replace("YEROTHCOMPTEBANCAIRENR",
			infoEntreprise.getNumeroCompteBancaire());
	factureTexDocument.replace("YEROTHCONTRIBUABLENR",
			infoEntreprise.getNumeroDeContribuable());
	factureTexDocument.replace("YEROTHAGENCECOMPTEBANCAIRE",
			infoEntreprise.getAgenceCompteBancaireTex());
//    QString YEROTHMONTANTLITERAL(QString("'%1' %2")
//                                                              .arg(YerothUtils_SPELL_NUMBERS::YEROTH__spell_numbers(_sommeTotal),
//                                                                       YerothERPConfig::currency));
	factureTexDocument.replace("YEROTHCLIENT",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientName()));
	factureTexDocument.replace("CLIENTYEROTHREPRESENTANT",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientRepresentantName()));
	factureTexDocument.replace("CLIENTYEROTHCITY",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientCity()));
	factureTexDocument.replace("CLIENTYEROTHPOBOX",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientPoBox()));
	factureTexDocument.replace("CLIENTYEROTHMAIL",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientEmail()));
	factureTexDocument.replace("CLIENTYEROTHPHONE",
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					yeroth_client_account_sale_info.getClientPhone()));
	QString prefixFileName;
#ifdef YEROTH_FRANCAIS_LANGUAGE
    prefixFileName.
    append
    (YerothUtils::
     getUniquePrefixFileInTemporaryFilesDir
     ("yeroth-erp-vente-facture-grand"));
#endif
#ifdef YEROTH_ENGLISH_LANGUAGE
    prefixFileName.
    append
    (YerothUtils::
     getUniquePrefixFileInTemporaryFilesDir
     ("yeroth-erp-sale-receipt-big"));
#endif //qDebug() << "++\n" << factureTexDocument;
	QFile tmpLatexFile(prefixFileName + "tex");
	if (tmpLatexFile.open(QFile::WriteOnly))
	{
		tmpLatexFile.write(factureTexDocument.toUtf8());
	}

	tmpLatexFile.close();
	return YerothERPProcess::compileLatex(prefixFileName);
}


void YerothUtils::getFactureTexTableString(QString 			&texTable_IN_OUT,
										   QTableWidget 	&tableStandardItemModel,
										   int 				quantiteVendue,
										   double 			totalTVA,
										   double 			sommeTotal)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
			"\\resizebox{\\textwidth}{!}{\n"
			"\\centering\n"
			"\\begin{tabular}"
			"{|");
	/**
	 * We reduce the header items of the table by 1 element
	 * (at position 4: stock quantity field) because the
	 * stock quantity field shall not be part of the
	 * information printed to the customer.
	 */
	int texTableColumnCount = tableStandardItemModel.columnCount();
	//Tex table header
	for (int k = 0; k < texTableColumnCount; ++k)
	{
		if (k == YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		if (YerothPointDeVenteTableWidget::TOTAL_COLUMN == k
				|| YerothPointDeVenteTableWidget::TOTAL_TVA_COLUMN == k)
		{
			texTable_IN_OUT.append("r|");
		}
		else if (YerothPointDeVenteTableWidget::DESIGNATION_COLUMN == k
				|| YerothPointDeVenteTableWidget::CATEGORIE_COLUMN == k)
		{
			texTable_IN_OUT.append("l|");
		}
		else
		{
			texTable_IN_OUT.append("r|");
		}
	}

	texTable_IN_OUT.append("} \\hline\n");	//Table header end
	QTableWidgetItem *item;
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString emptyString;
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, emptyString);
		}
	}
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString itemText(item->text().prepend("\\textbf{").append("}"));
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, itemText);
		}
	}
	for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
	{
		if (k == YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK)
		{
			continue;
		}

		item = tableStandardItemModel.horizontalHeaderItem(k);
		if (item)
		{
			QString emptyString;
			handleTexTableItemText(tableStandardItemModel.columnCount(),
					texTable_IN_OUT, k, emptyString);
		}
	}

	cleanUpTexTableLastString(texTable_IN_OUT);
	texTable_IN_OUT.append("\\hline\n");
	//Tex table body
	for (int j = 0; j < tableStandardItemModel.rowCount(); ++j)
	{
		for (int k = 0; k < tableStandardItemModel.columnCount(); ++k)
		{
			if (k == YerothPointDeVenteTableWidget::QTE_TOTAL_STOCK)
			{
				continue;
			}

			item = tableStandardItemModel.item(j, k);
			if (item)
			{
				QString itemText(item->text());
				if (itemText.length()
						> YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS)
				{
					itemText.truncate(
							YerothUtils::FACTURE_GRAND_STRING_MAX_CHARS - 1);
					itemText.append(".");
					//qDebug() << QString("article name after truncate: %1").arg(articleName);
				}

				handleFactureTexTableItemText(
						tableStandardItemModel.columnCount(), texTable_IN_OUT,
						k, itemText);
			}
			else
			{
				if (k < tableStandardItemModel.columnCount() - 1)
				{
					texTable_IN_OUT.append("\"\" &");
				}
				else
				{
					texTable_IN_OUT.append("\"\"  \\hline\n");
				}
			}
		}

		cleanUpTexTableLastString(texTable_IN_OUT);
		texTable_IN_OUT.append("\\hline\n");
	}

	QString quantiteVendueStr(
			LATEX_IN_OUT_handleForeignAccents(GET_NUM_STRING(quantiteVendue)));
	quantiteVendueStr.prepend("\\textbf{").append("}");
	QString totalTVAStr(
			LATEX_IN_OUT_handleForeignAccents(GET_DOUBLE_STRING(totalTVA)));
	totalTVAStr.prepend("\\textbf{").append("}");
	QString sommeTotalStr(
			LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(sommeTotal)));
	sommeTotalStr.prepend("\\textbf{").append("}");
	QString totalStr;
#ifdef YEROTH_FRANCAIS_LANGUAGE
    totalStr.
    append
    ("TOTAUX");
#endif
#ifdef YEROTH_ENGLISH_LANGUAGE
    totalStr.
    append
    ("TOTAL");
#endif
	texTable_IN_OUT.append(
			QString(
					"\\multicolumn{%1}{|c}{\\textbf{%2}} & & %3 & & %4 & %5 \\\\ \\hline \n").arg(
					GET_NUM_STRING(tableStandardItemModel.columnCount() - 6),
					totalStr, quantiteVendueStr, totalTVAStr, sommeTotalStr));
	//Removes the empty character "" from Latex output
	texTable_IN_OUT.replace("\"\"", "");
	texTable_IN_OUT.append("\\end{tabular}}\n"
			"\\end{table*}\n");
}


void YerothUtils::getFactureSmallTexTableString(QString &texTable_IN_OUT,
		QTableWidget &tableStandardItemModel, int quantiteVendue,
		double totalTVA, double sommeTotal, double montantRecuDuCient, /* = 0.0 */
		double montantARembourserAuClient
		/* = 0.0 */
		)
{
	texTable_IN_OUT.append("\\begin{table*}[!htbp]\n"
			"\\resizebox{\\textwidth}{!}{"
			"\\centering\n"
			"\\begin{tabular}{lrr}\n");
	int tableColumnCount = tableStandardItemModel.columnCount();
	QTableWidgetItem *item = 0;
	for (int k = 0; k < tableColumnCount; ++k)
	{
		if (k == YerothPointDeVenteTableWidget::DESIGNATION_COLUMN
				|| k == YerothPointDeVenteTableWidget::QTE_COLUMN
				|| k == YerothPointDeVenteTableWidget::TOTAL_COLUMN)
		{
			item = tableStandardItemModel.horizontalHeaderItem(k);
			if (item)
			{
				QString itemText(item->text());
				handleTexTableItemText(tableStandardItemModel.columnCount(),
						texTable_IN_OUT, k, itemText);
			}
		}
	}
	cleanUpTexTableLastString(texTable_IN_OUT);
	texTable_IN_OUT.append("\\hline\n");
	QString articleName;
	int articleNameLength;
	QTableWidgetItem *articleNameItem;
	QTableWidgetItem *articleQteItem;
	QTableWidgetItem *totalTtcItem;
	int tableRowCount = tableStandardItemModel.rowCount();
	for (int k = 0; k < tableRowCount; ++k)
	{
		articleNameItem = tableStandardItemModel.item(k,
				YerothPointDeVenteTableWidget::DESIGNATION_COLUMN);
		articleQteItem = tableStandardItemModel.item(k,
				YerothPointDeVenteTableWidget::QTE_COLUMN);
		totalTtcItem = tableStandardItemModel.item(k,
				YerothPointDeVenteTableWidget::TOTAL_COLUMN);
		if (articleNameItem && articleQteItem && totalTtcItem)
		{
			articleName = articleNameItem->text();
			articleNameLength = articleName.length();
			//qDebug() << QString("article name: %1, articleNameLength: %2")
			//                          .arg(articleName, articleNameLength);
			if (articleNameLength
					> YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS)
			{
				articleName.truncate(
						YerothUtils::FACTURE_PETIT_NOM_ARTICLE_MAX_CHARS - 1);
				//qDebug() << QString("article name after truncate: %1").arg(articleName);
			}

			texTable_IN_OUT.append(
					QString("%1. %2 & $x%3$ & %4 \\\\ \n").arg(
							QString::number(k + 1),
							YerothUtils::LATEX_IN_OUT_handleForeignAccents(
									articleName), articleQteItem->text(),
							YerothUtils::LATEX_IN_OUT_handleForeignAccents(
									totalTtcItem->text())));
		}
	}

	//We now add the sum
	QString sommeTotalStr(
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(sommeTotal)));
	texTable_IN_OUT.append(
			QString("\\hline\n & $%1$ & %2 \\\\ \n").arg(
					QString::number(quantiteVendue), sommeTotalStr));
	texTable_IN_OUT.append(QString(" & & \\\\ \n"));
	QString totalTVAStr(
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(totalTVA)));
	QString montantRecuDuClientStr(
			YerothUtils::LATEX_IN_OUT_handleForeignAccents(
					GET_CURRENCY_STRING_NUM(montantRecuDuCient)));
	QString montantARembourserAuClientStr;
	if (montantARembourserAuClient >= 0)
	{
		montantARembourserAuClientStr =
				YerothUtils::LATEX_IN_OUT_handleForeignAccents(
						GET_CURRENCY_STRING_NUM(montantARembourserAuClient));
	}
	else
	{
		montantARembourserAuClientStr =
				YerothUtils::LATEX_IN_OUT_handleForeignAccents(
						GET_CURRENCY_STRING_NUM(0.0));
	}

	texTable_IN_OUT.append(
			QObject::tr("dont total TVA: & & %1\\\\ \n").arg(totalTVAStr));
	texTable_IN_OUT.append(
			QObject::tr("Montant re\\c{c}u: & & %1\\\\ \n").arg(
					montantRecuDuClientStr));
	texTable_IN_OUT.append(
			QObject::tr("Montant remi: & & %1\\ \n").arg(
					montantARembourserAuClientStr));
	texTable_IN_OUT.append("\\end{tabular}}\n"
			"\\end{table*}\n");
}


void YerothUtils::refreshSalesStrategy(YerothSqlTableModel &curStocksTableModel,
		YerothLineEdit *lineEdit_recherche_article,
		YerothLineEdit *lineEdit_recherche_article_codebar)
{
	QString curStocksTableModelName = curStocksTableModel.sqlTableName();
	if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FIFO,
			YerothERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterFIFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarFIFO(
				curStocksTableModelName);
	}
	else if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_LIFO,
			YerothERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterLIFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarLIFO(
				curStocksTableModelName);
	}
	else if (YerothUtils::isEqualCaseInsensitive(
			YerothERPConfig::STRATEGIE_VENTE_SORTIE_FEFO,
			YerothERPConfig::salesStrategy))
	{
		lineEdit_recherche_article->setupMyQCompleterFEFO(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarFEFO(
				curStocksTableModelName);
	}
	else	//YerothConfig::STRATEGIE_VENTE_SORTIE_ALL
	{
		lineEdit_recherche_article->setupMyQCompleterALL(
				curStocksTableModelName);
		lineEdit_recherche_article_codebar->setupMyQCompleterCodebarALL(
				curStocksTableModelName);
	}
}


void YerothUtils::get_FACTURE_INVOICE_TexDocumentString(QString &texDocumentString_in_out)
{
	QString printString("\n \\vspace*{0.9cm}\n");

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		printString
			.append("\\textbf{INVOICE \\textcolor{yerothColorRed}{TOTAL remaining AMOUNT ALL TAXES} "
					"COMPRISED FOR yourself IS OF "
					"\\textcolor{yerothColorRed}{'YEROTHFACTUREINVOICEREMAININGAMOUNT'} (YEROTHMONTANTLITERAL)}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(EN_template_FACTURE_GRAND_tex);
	}
	else
	{
		printString
			.append("\\textbf{Arr\\^et\\'e LE \\textcolor{yerothColorRed}{RESTE à PAYER TTC} pour cet achat au compte client "
				    "est de \\textcolor{yerothColorRed}{'YEROTHFACTUREINVOICEREMAININGAMOUNT'} (YEROTHMONTANTLITERAL)}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(FR_template_FACTURE_GRAND_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::get_GROUPES_DUN_client_TexDocumentString(QString &texDocumentString_in_out,
														   QString &GROUPES_DUN_CLIENT__tex_table)
{
	texDocumentString_in_out.clear();

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_GROUPES_DUN_CLIENT_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_GROUPES_DUN_CLIENT_tex);
	}

	texDocumentString_in_out.append(GROUPES_DUN_CLIENT__tex_table).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::get_GROUPES_DUN_EMPLOYE_TexDocumentString(QString &texDocumentString_in_out,
														    QString &GROUPES_DUN_EMPLOYE__tex_table)
{
	texDocumentString_in_out.clear();

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_GROUPES_DUN_EMPLOYE_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_GROUPES_DUN_EMPLOYE_tex);
	}

	texDocumentString_in_out.append(GROUPES_DUN_EMPLOYE__tex_table).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::get_RECU_TexDocumentString(QString &texDocumentString_in_out,
											 QString &printString)
{
	printString.append("\n \\vspace*{0.9cm}\n");

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		printString.append("The total amount of this receipt all taxes comprised is of \\textbf{YEROTHMONTANTLITERAL}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(EN_template_RECU_GRAND_tex);
	}
	else
	{
		printString.append("Arr\\^et\\'e le pr\\'esent re\\c{c}u au montant total TTC de \\textbf{YEROTHMONTANTLITERAL}.\n");

		texDocumentString_in_out.clear();
		texDocumentString_in_out.append(FR_template_RECU_GRAND_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


void YerothUtils::get_RECU_Small_TexDocumentString(QString &texDocumentString_in_out,
												   QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(EN_template_facture_petit_tex);
	}
	else
	{
		texDocumentString_in_out.append(FR_template_facture_petit_tex);
	}

	texDocumentString_in_out.append(printString).append("\n");
	texDocumentString_in_out.append("\\end{document}");
}


bool YerothUtils::SAVE_AS_csv_file(YerothWindowsCommons    &aCallingWindow,
                                   YerothTableWidget       &aTableView,
                                   const QString           &csvFileName,
                                   const QString           &strMessage,
                                   int                     row_MAX_TO_GO_export /* = -1 */)
{
	QAbstractItemModel *tableModel = aTableView.model();

	if (0 == tableModel)
	{
		return false;
	}


	int tableModelRowCount = tableModel->rowCount();

	int tableModelColumnCount = tableModel->columnCount();


	if (tableModelRowCount <= 0  ||
	    tableModelColumnCount <= 0)
	{
        YerothQMessageBox::information(&aCallingWindow,
                                       QObject::tr("No CSV data"),
                                       QObject::tr("No CSV data to save out !"));
		return false;
	}


	QString csvFileContent;

	QHeaderView *a_calling_window_qheaderview = aTableView.horizontalHeader();

	if (0 == a_calling_window_qheaderview)
	{
        YerothQMessageBox::information(&aCallingWindow,
                                       QObject::tr("Bad deployment"),
                                       QObject::tr("Table bad deployment !"));

		return false;
	}


	QVariant anItem;

	QString anItemText;

	for (int k = 0; k < a_calling_window_qheaderview->count(); ++k)
	{
		anItem = tableModel->headerData(k, Qt::Horizontal);

		if (anItem.isValid())
		{
			anItemText = anItem.toString();

			csvFileContent.append(QString("\"%1\"%2 ")
                                    .arg(anItemText,
                                         YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}
	}


	csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");



	int MAX_TABLE_MODDEL_ROW_COUNT__to_export = tableModelRowCount;


	if (row_MAX_TO_GO_export > -1)
	{
        MAX_TABLE_MODDEL_ROW_COUNT__to_export = row_MAX_TO_GO_export;
	}


	QMap<int, QVariant> item_data;

	for (unsigned int j = 0; j < MAX_TABLE_MODDEL_ROW_COUNT__to_export; ++j)
	{
		for (unsigned int k = 0; k < tableModelColumnCount; ++k)
		{
			item_data = tableModel->itemData(tableModel->index(j, k));

            anItemText = item_data.value(Qt::DisplayRole).toString();

            csvFileContent
                .append(QString("\"%1\"%2 ")
                         .arg(anItemText,
                              YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}

		csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");
	}


	QString yerothStocksListingCSVFileName
				(QString("%1/%2")
					.arg(YerothERPConfig::temporaryFilesDir,
						 csvFileName));


	yerothStocksListingCSVFileName =
				FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);


        yerothStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Type in a '.csv' file name ",
										   yerothStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
                                             .arg(strMessage));


    yerothStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerothStocksListingCSVFileName);

	if (tmpFile.open(QFile::WriteOnly))
	{
		tmpFile.write(csvFileContent.toUtf8());
	}

	tmpFile.close();

	return true;
}


bool YerothUtils::export_csv_file(YerothWindowsCommons &aCallingWindow,
                                  YerothTableView &aTableView,
                                  QList<int> databaseTableColumnsToIgnore,
                                  const QString &csvFileName,
                                  const QString &strMessage)
{
	YerothPOSQStandardItemModel *tableModel = aTableView.getStandardItemModel();
	if (0 == tableModel)
	{
		return false;
	}

	int tableModelRowCount = tableModel->rowCount();
	int tableModelColumnCount = tableModel->columnCount();
	if (tableModelRowCount <= 0 || tableModelColumnCount <= 0
			|| tableModelColumnCount <= databaseTableColumnsToIgnore.count())
	{
		YerothQMessageBox::information(&aCallingWindow,
				QObject::tr("pas de données à exporter au format csv"),
				QObject::tr(
						"Il n'y a pas de données à exporter au format csv !"));
		return false;
	}

	QString csvFileContent;
	QHeaderView *a_calling_window_qheaderview = aTableView.horizontalHeader();
	if (0 == a_calling_window_qheaderview)
	{
		YerothQMessageBox::information(&aCallingWindow,
				QObject::tr("MAUVAIS DÉPLOIEMENT"),
				QObject::tr("MAUVAIS DÉPLOIEMENT du tableau!"));
		return false;
	}

	YerothQStandardItem *anItem = 0;
	QString anItemText;
	int a_logical_index_k = 0;
	for (int k = 0; k < a_calling_window_qheaderview->count(); ++k)
	{
		a_logical_index_k = a_calling_window_qheaderview->logicalIndex(k);
		if (databaseTableColumnsToIgnore.contains(a_logical_index_k))
		{
			continue;
		}

		anItem = (YerothQStandardItem*) tableModel->horizontalHeaderItem(
				a_logical_index_k);
		if (0 != anItem)
		{
			anItemText = anItem->text();
			csvFileContent.append(
					QString("\"%1\"%2 ").arg(anItemText,
							YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
		}
	}

	csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");

	for (unsigned int j = 0; j < tableModelRowCount; ++j)
	{
		for (unsigned int k = 0; k < tableModelColumnCount; ++k)
		{
			if (databaseTableColumnsToIgnore.contains(k))
			{
				continue;
			}

			anItem = (YerothQStandardItem*) tableModel->item(j, k);

			if (0 != anItem)
			{
				anItemText = anItem->get_rawUNMODIFIED_FOR_USER_VIEWING_text();
//                QDEBUG_STRING_OUTPUT_2("anItemText", anItemText);

				csvFileContent
					.append(QString("\"%1\"%2 ")
								.arg(anItemText,
								     YerothUtils::CSV_FILE_SEPARATION_SEMI_COLON_STRING_CHAR));
			}
		}

		csvFileContent.remove(csvFileContent.size() - 2, 2).append("\n");
	}


	QString yerothStocksListingCSVFileName
				(QString("%1/%2")
					.arg(YerothERPConfig::temporaryFilesDir,
						 csvFileName));


	yerothStocksListingCSVFileName =
				FILE_NAME_USERID_CURRENT_TIME(yerothStocksListingCSVFileName);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        yerothStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Type in '.csv' file name ",
										   yerothStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
										   	   .arg(strMessage));
    }
    else
    {
        yerothStocksListingCSVFileName
            = QFileDialog::getSaveFileName(&aCallingWindow,
                             	 	 	   "Saisir le nom du fichier '.csv'",
										   yerothStocksListingCSVFileName,
										   QString("%1 \"*.csv\" (*.csv)")
										   	   .arg(strMessage));
    }


    yerothStocksListingCSVFileName.append(".csv");

    QFile tmpFile(yerothStocksListingCSVFileName);

	if (tmpFile.open(QFile::WriteOnly))
	{
		tmpFile.write(csvFileContent.toUtf8());
	}

	tmpFile.close();

	return true;
}


void YerothUtils::getLatexFinancialAccountingReportData(QString &texDocumentString_in_out)
{
    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(YerothUtils::EN_template_bilan_comptable_tex);
    }
    else
    {
    	texDocumentString_in_out.append(YerothUtils::FR_template_bilan_comptable_tex);
    }
}


bool YerothUtils::import_csv_file_content(QString aCsvFileFullPath,
										  QStringList &wordList_IN_OUT)
{
	wordList_IN_OUT.clear();

	QFile aCsvFile(aCsvFileFullPath);

	if (!aCsvFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QTextStream aCsvFileTextStream(&aCsvFile);

	wordList_IN_OUT.append(aCsvFileTextStream.readAll().split(YerothUtils::LINE_ENDING_STRING_REGEXP));

	return true;
}


void YerothUtils::getLatexCLIENT_LOYALTY_CARD_template(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out
    		.append(YerothUtils::EN_template_carte_de_fidelite_client_TEMPLATE_tex);
    }
    else
    {
    	texDocumentString_in_out
    		.append(YerothUtils::FR_template_carte_de_fidelite_client_TEMPLATE_tex);
    }
}


void YerothUtils::getLatex_FINANCIALexpense_Data(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerothUtils::EN_template_bon_de_commande_TEX));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerothUtils::FR_template_bon_de_commande_TEX));
    }
}


void YerothUtils::getLatexSupplierData(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerothUtils::EN_template_fiche_dun_fournisseur_tex));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
    				.arg(YerothUtils::FR_template_fiche_dun_fournisseur_tex));
    }
}


void YerothUtils::getLatexCustomerData(QString &texDocumentString_in_out)
{
	texDocumentString_in_out.clear();

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
					.arg(YerothUtils::EN_template_fiche_client_tex));
    }
    else
    {
    	texDocumentString_in_out.append(
    			QString("%1\n\\end{document}")
					.arg(YerothUtils::FR_template_fiche_client_tex));
    }
}


void YerothUtils::getSortieDesStocksTexDocumentString(QString &texDocumentString_in_out,
													  QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerothUtils::EN_template_sortie_des_stocks_grand_tex,
						 printString));
	}
	else
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerothUtils::FR_template_sortie_des_stocks_grand_tex,
						 printString));
	}
}


void YerothUtils::getSortieDesStocksSmallTexDocumentString(QString &texDocumentString_in_out,
														   QString &printString)
{
	texDocumentString_in_out.clear();

	if (YerothMainWindow::LANGUE_ANGLAISE)
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerothUtils::EN_template_sortie_des_stocks_petit_tex,
						 printString));
	}
	else
	{
		texDocumentString_in_out.append(
				QString("%1%2\n\\end{document}")
					.arg(YerothUtils::FR_template_sortie_des_stocks_petit_tex,
						 printString));
	}
}

