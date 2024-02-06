/**
 * yeroth-erp-admin-search-form.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-admin-search-form.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include <QtCore/QDebug>

#include <QtCore/QRegExp>

#include <QtCore/QStringList>

#include <QtSql/QSqlError>

#include <QtWidgets/QLabel>

#include <QtWidgets/QFormLayout>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QCompleter>



YerothAdminSearchForm::YerothAdminSearchForm(YerothERPWindows  	*allWindows,
											 QWidget 			*parent)
:QDialog(parent),
 _curSujetAction(SUJET_ACTION_ALERTE),
 _curSqlTableModel(0),
 _logger(0),
 _allWindows(allWindows)
{
    setupUi(this);

    _logger = new YerothLogger("YerothAdminSearchForm");

    setFixedSize(width(),
    			 height());


    connect(checkBox_EXACT_STRING_MATCH,
            SIGNAL(stateChanged(int)),
			this,
            SLOT(setupLineEditsQCompleters()));


    pushButton_annuler->enable(this, SLOT(reinitialiser()));
}


void YerothAdminSearchForm::rendreVisible(int tabWidjetListerIdx)
{
    _logger->log("rendreVisible");

    setupLineEditsQCompleters(tabWidjetListerIdx);

    lineEdit_terme_recherche->setFocus();

    show();
}


void YerothAdminSearchForm::rendreInvisible()
{
    _logger->log("rendreInvisible");

    clear_all_fields();

    close();
}


QString YerothAdminSearchForm::generate_qstring_FILTER(QString corresponding_DB_column_KeyValue,
                                                       QString aTableColumnFieldContentForSearch)
{
    QString qstring_filter;

    if (!checkBox_EXACT_STRING_MATCH->isChecked())
    {
        qstring_filter =
            YerothUtils::generateSqlLike(corresponding_DB_column_KeyValue,
                                         aTableColumnFieldContentForSearch);
    }
    else
    {
        qstring_filter =
            YerothUtils::generateSqlIs(corresponding_DB_column_KeyValue,
                                       aTableColumnFieldContentForSearch);
    }

//    QDEBUG_STRING_OUTPUT_2("qstring_filter",
//                            qstring_filter);

    return qstring_filter;
}


void YerothAdminSearchForm::setupLineEditsQCompleters()
{
    setupLineEditsQCompleters(_allWindows
                                ->_adminListerWindow
                                    ->current_tabWidget_index());
}


void YerothAdminSearchForm::setupLineEditsQCompleters(int tabWidjetListerIdx)
{
    Qt::MatchFlags a_filter_mode = Qt::MatchContains;

    if (!checkBox_EXACT_STRING_MATCH->isChecked())
    {
        a_filter_mode = Qt::MatchContains;
    }
    else
    {
        a_filter_mode = Qt::MatchStartsWith;
    }

    switch (tabWidjetListerIdx)
    {

    case SUJET_ACTION_ALERTE:

        lineEdit_terme_recherche->enableForSearch(QObject::tr("nom de l'alerte"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::ALERTES,
                                	  YerothDatabaseTableColumn::DESIGNATION_ALERTE,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);


        _curSujetAction = SUJET_ACTION_ALERTE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_alertes();
        break;


    case SUJET_ACTION_CHARGE_FINANCIERE:

        lineEdit_terme_recherche
 			->enableForSearch(QObject::tr("désignation (BON DE COMMANDE)"));


         lineEdit_terme_recherche
         	->setupMyStaticQCompleter(YerothDatabase::CHARGES_FINANCIERES,
                                 	  YerothDatabaseTableColumn::DESIGNATION,
 									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

         _curSujetAction = SUJET_ACTION_CHARGE_FINANCIERE;

         _curSqlTableModel = &_allWindows->getSqlTableModel_charges_financieres();
        break;


    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom du département de produits"));

        lineEdit_terme_recherche
        	->setupMyStaticQCompleter(YerothDatabase::DEPARTEMENTS_PRODUITS,
                                  	  YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS;

        _curSqlTableModel = &_allWindows->getSqlTableModel_departements_produits();
        break;


    case SUJET_ACTION_CATEGORIE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de la catégorie"));

        lineEdit_terme_recherche
        	->setupMyStaticQCompleter(YerothDatabase::CATEGORIES,
                                	  YerothDatabaseTableColumn::NOM_CATEGORIE,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_CATEGORIE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_categories();
        break;


    case SUJET_ACTION_ligne_budgetaire:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("INTITULÉ DE LA LIGNE BUDGÉTAIRE"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::LIGNES_BUDGETAIRES,
                                	  YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_ligne_budgetaire;

        _curSqlTableModel = &_allWindows->getSqlTableModel_lignes_budgetaires();
        break;


    case SUJET_ACTION_COMPTE_BANCAIRE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("référence du compte bancaire"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::COMPTES_BANCAIRES,
                                	  YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_COMPTE_BANCAIRE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_comptes_bancaires();
        break;


    case SUJET_ACTION_COMPTE_UTILISATEUR:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de l'utilisateur"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::USERS,
                                	  YerothDatabaseTableColumn::NOM_COMPLET,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_COMPTE_UTILISATEUR;

        _curSqlTableModel = &_allWindows->getSqlTableModel_users();
        break;


    case SUJET_ACTION_LOCALISATION:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("nom de la localisation"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::LOCALISATIONS,
                                	  YerothDatabaseTableColumn::NOM_LOCALISATION,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_LOCALISATION;

        _curSqlTableModel = &_allWindows->getSqlTableModel_localisations();
        break;


    case SUJET_ACTION_REMISE:

        lineEdit_terme_recherche
			->enableForSearch(QObject::tr("désignation de la remise"));

        lineEdit_terme_recherche
			->setupMyStaticQCompleter(YerothDatabase::REMISES,
                                	  YerothDatabaseTableColumn::DESIGNATION_REMISE,
									  false,
									  YerothUtils::EMPTY_STRING,
									  a_filter_mode);

        _curSujetAction = SUJET_ACTION_REMISE;

        _curSqlTableModel = &_allWindows->getSqlTableModel_remises();
        break;

    default:
        break;
    }


    connect(lineEdit_terme_recherche->getMyQCompleter(),
            SIGNAL(activated(const QString &)),
			this,
            SLOT(rechercher(const QString &)));
}


void YerothAdminSearchForm::reinitialiser()
{
    _logger->log("reinitialiser");

    lineEdit_terme_recherche->clear();

    if (_curSqlTableModel)
    {
        _curSqlTableModel->resetFilter();
    }

    setCurSqlTableModel(0);

    rendreInvisible();
}


void YerothAdminSearchForm::rechercher(const QString &itemName)
{
    //qDebug() << "++ YerothAdminSearchForm::rechercher | itemName: " << itemName;

    QString searchString(itemName);

    QString filter;

    switch (_curSujetAction)
    {
    case SUJET_ACTION_ALERTE:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::DESIGNATION_ALERTE,
                                         searchString);
        break;


    case SUJET_ACTION_CHARGE_FINANCIERE:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::DESIGNATION,
                                         searchString);
        break;


    case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
                                         searchString);
        break;


    case SUJET_ACTION_CATEGORIE:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::NOM_CATEGORIE,
                                         searchString);
        break;


    case SUJET_ACTION_ligne_budgetaire:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
                                         searchString);
        break;


    case SUJET_ACTION_COMPTE_BANCAIRE:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
                                         searchString);
        break;


    case SUJET_ACTION_COMPTE_UTILISATEUR:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::NOM_COMPLET,
                                         searchString);
        break;


    case SUJET_ACTION_LOCALISATION:
        filter = generate_qstring_FILTER(YerothDatabaseTableColumn::NOM_LOCALISATION,
                                         searchString);
        break;


    default:
        break;
    }

    //qDebug() << "++ YerothAdminSearchForm::rechercher, searchString: " << searchString;
    if (0 != _curSqlTableModel)
    {
        _curSqlTableModel->yerothSetFilter_WITH_where_clause(filter);

//        qDebug() << "++ filter: "
//                 << filter << ", _curSqlTableModel: "
//                 << _curSqlTableModel->sqlTableName();

        int curSqlTableModelRows =
        		_curSqlTableModel->easySelect("src/admin/yeroth-erp-admin-search-form.cpp",
        									  390);

        if (curSqlTableModelRows > 0)
        {
            switch (_curSujetAction)
            {
            case SUJET_ACTION_ALERTE:
                _allWindows->_adminListerWindow->setAlertCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_alerte(_curSqlTableModel);
                break;


            case SUJET_ACTION_CHARGE_FINANCIERE:
                _allWindows->_adminListerWindow->setCharge_financieres_CurrentlyFiltered(true);
                _allWindows->_adminListerWindow->LISTER_CHARGES_FINANCIERES(_curSqlTableModel);
                break;


            case SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS:
                _allWindows->_adminListerWindow->setProductDepartmentCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_departements_de_produits(_curSqlTableModel);
                break;


            case SUJET_ACTION_CATEGORIE:
                _allWindows->_adminListerWindow->setCategoryCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_categorie(_curSqlTableModel);
                break;


            case SUJET_ACTION_ligne_budgetaire:
                _allWindows->_adminListerWindow->setLIGNE_BUDGETAIRE_CurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_LIGNE_BUDGETAIRE(_curSqlTableModel);
                break;


            case SUJET_ACTION_COMPTE_BANCAIRE:
                _allWindows->_adminListerWindow->setBankAccountCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_compte_bancaire(_curSqlTableModel);
                break;


            case SUJET_ACTION_COMPTE_UTILISATEUR:
                _allWindows->_adminListerWindow-> setUserCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_utilisateur(_curSqlTableModel);
                break;


            case SUJET_ACTION_LOCALISATION:
                _allWindows->_adminListerWindow->setSiteCurrentlyFiltered(true);
                _allWindows->_adminListerWindow->lister_localisation(_curSqlTableModel);
                break;


            default:
                break;
            }
        }
    }


    rendreInvisible();
}

