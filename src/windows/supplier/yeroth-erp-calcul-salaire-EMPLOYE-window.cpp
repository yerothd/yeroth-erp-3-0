/*
 * yeroth-erp-calcul-salaire-EMPLOYE-window.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-calcul-salaire-EMPLOYE-window.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtSql/QSqlRecord>


YerothERPCalculSalaireEMPLOYE::
    YerothERPCalculSalaireEMPLOYE(YerothWindowsCommons *calling_window)
:_sommeTotal(0.0),
 _calling_window(calling_window)
{
    __QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);
}


double YerothERPCalculSalaireEMPLOYE::
            calculate_PAY_GROUP_MONEY_BENEFITS(QString  &a_nom_entreprise_EMPLOYEE,
                                               QString  &PAY_GROUP)
{
    double result = 0.0;


    YerothERPWindows *ALL_WINDOWS = YerothUtils::getAllWindows();


    YerothSqlTableModel &periodes_dappartenance_groupes_de_paie_hr_SqlTableModel =
        ALL_WINDOWS->getSqlTableModel_periodes_dappartenance_groupes_de_paie_hr();


    QString condition_exist_already =
        QString("(%1) AND (%2)")
            .arg(GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::NOM_ENTREPRISE,
                                      a_nom_entreprise_EMPLOYEE),
                 GENERATE_SQL_IS_STMT(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                      PAY_GROUP));


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .yerothSetFilter_WITH_where_clause(condition_exist_already);


    int query_size = periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
                        .easySelect("src/windows/supplier/yeroth-erp-fournisseur-details-window.cpp", 632);


    if (query_size <= 0)
    {
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .resetFilter("src/windows/supplier/yeroth-erp-fournisseur-details-window.cpp", 638);

        return result;
    }


    QDate begin_DATE, end_DATE;


    QSqlRecord periodes_dappartenance_RECORD =
        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel.record(0);


    begin_DATE = periodes_dappartenance_RECORD
                    .value(YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE)
                            .toDate();

    end_DATE = periodes_dappartenance_RECORD
                    .value(YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE)
                            .toDate();


    QDate current_date = GET_CURRENT_DATE;


    bool AUTHORIZED_PAYMENT_DATE = (begin_DATE <= current_date &&
                                    current_date <= end_DATE);


    if (!AUTHORIZED_PAYMENT_DATE)
    {
        if (0 != _calling_window)
        {
            YerothQMessageBox::information(_calling_window,
                                           QObject::tr("calcul de salaire d'1 employé"),
                                           QObject::tr("LE GROUPE DE PAIEMENT [\"%1\"] "
                                                       "N'A PLUS DE dates de validités courantes "
                                                       "pour l'employé [%2] !")
                                            .arg(PAY_GROUP,
                                                 a_nom_entreprise_EMPLOYEE));
        }

        periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
            .resetFilter("src/windows/supplier/yeroth-erp-fournisseur-details-window.cpp", 676);

        return result;
    }


    periodes_dappartenance_groupes_de_paie_hr_SqlTableModel
        .resetFilter("src/windows/supplier/yeroth-erp-fournisseur-details-window.cpp", 683);



    QString SELECT_GROUPE_DE_PAIE =
                QString("select * from %1 where %2='%3'")
                    .arg(YerothDatabase::GROUPES_DE_PAIE_hr,
                         YerothDatabaseTableColumn::DESIGNATION,
                         PAY_GROUP);


    QSqlQuery a_qsql_query;

    int query_size_3 = YerothUtils::execQuery(a_qsql_query,
                                              SELECT_GROUPE_DE_PAIE);

    if (query_size_3 <= 0)
    {
        return 0.0;
    }

    a_qsql_query.next();

    QSqlRecord aQSqlRecord = a_qsql_query.record();

    double AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS =
                GET_SQL_RECORD_DATA(aQSqlRecord,
                                    YerothDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL)
                                        .toDouble();

    result = AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS;


    //QDEBUG_STRING_OUTPUT_2_N("original amount to be paid TO EMPLOYEE", _sommeTotal);


    double pourcentage_TAXES_IMPOSABLES =
                GET_SQL_RECORD_DATA(aQSqlRecord,
                                    YerothDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES).toDouble();


    double TVA_money =
                    (pourcentage_TAXES_IMPOSABLES *
                     AMOUNT_TO_BE_PAID_to_employee__NO__MONEY_BENEFITS) / 100.0;

    result = result - TVA_money;

    return result;
}


double YerothERPCalculSalaireEMPLOYE::
        CALCULATE_EMPLOYEE_SALARY(const QString  &a_nom_entreprise_EMPLOYEE,
                                  bool           CALL_update_lineedits_and_labels /* = false */)
{
    //QDEBUG_STRING_OUTPUT_2("a_nom_entreprise_EMPLOYEE",
    //                        a_nom_entreprise_EMPLOYEE);

    if (a_nom_entreprise_EMPLOYEE.isEmpty())
    {
        return 0.0;
    }

    /*
     * I search all client group of customer 'a_nom_entreprise_client'.
     */
    QString SELECT_EMPLOYEE_GROUP
                (QString("select %1 from %2 where %3='%4' AND %5='1'")
                    .arg(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
                         YerothDatabase::FOURNISSEURS,
                         YerothDatabaseTableColumn::NOM_ENTREPRISE,
                         a_nom_entreprise_EMPLOYEE,
                         YerothDatabaseTableColumn::EMPLOYE));

    //QDEBUG_STRING_OUTPUT_2("SELECT_EMPLOYEE_GROUP",
    //                        SELECT_EMPLOYEE_GROUP);

    QSqlQuery a_qsql_query;

    int query_size = YerothUtils::execQuery(a_qsql_query,
                                            SELECT_EMPLOYEE_GROUP);

    if (query_size <= 0)
    {
        return 0.0;
    }


    QString employee_group_id;

    a_qsql_query.next();

    employee_group_id = a_qsql_query
                        .value(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID).toString();

    //QDEBUG_STRING_OUTPUT_2("employee_group_id", employee_group_id);

    if (employee_group_id.isEmpty())
    {
        return 0.0;
    }


    QStringList EMPLOYEE_GroupId_list;

    YerothUtils::SPLIT_STAR_SEPARATED_DB_STRING(EMPLOYEE_GroupId_list,
                                                employee_group_id);


//      QDEBUG_QSTRINGLIST_OUTPUT("EMPLOYEE_GroupId_list", EMPLOYEE_GroupId_list);


//      QDEBUG_STRING_OUTPUT_2("YerothERPConfig::THIS_SITE_LOCALISATION_NAME",
//                                                 YerothERPConfig::THIS_SITE_LOCALISATION_NAME);


    QString employeeGroup_db_ID;

    QString EMPLOYEE_group_PAY_GR0UP_LOCALISATION;

    QString EMPLOYEE_group_PAY_GR0UP;

    QString CUR_SELECT_employee_PAY_GROUP;

    double cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS = 0.0;


    for (uint k = 0; k < EMPLOYEE_GroupId_list.size(); ++k)
    {
        employeeGroup_db_ID = EMPLOYEE_GroupId_list.at(k);

        if (employeeGroup_db_ID.isEmpty())
        {
            continue;
        }

        CUR_SELECT_employee_PAY_GROUP =
                        QString("select %1 from %2 where %3='%4'")
                            .arg(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
                                 YerothDatabase::GROUPES_DEMPLOYES_hr,
                                 YerothDatabaseTableColumn::ID,
                                 employeeGroup_db_ID);

        //QDEBUG_STRING_OUTPUT_2("CUR_SELECT_employee_PAY_GROUP",
        //                        CUR_SELECT_employee_PAY_GROUP);

        a_qsql_query.clear();

        query_size = YerothUtils::execQuery(a_qsql_query,
                                            CUR_SELECT_employee_PAY_GROUP);


        for (int j = 0;
             j < query_size && a_qsql_query.next();
             ++j)
        {
            EMPLOYEE_group_PAY_GR0UP =
                a_qsql_query.value(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR).toString();

            if (EMPLOYEE_group_PAY_GR0UP.isEmpty())
            {
                continue;
            }

            EMPLOYEE_group_PAY_GR0UP_LOCALISATION =
                            YerothUtils::GET_LOCALISATION_FOR_EMPLOYEE_PAY_GROUP
                                (EMPLOYEE_group_PAY_GR0UP);

            //QDEBUG_STRING_OUTPUT_2("EMPLOYEE_group_PAY_GR0UP_LOCALISATION",
            //                        EMPLOYEE_group_PAY_GR0UP_LOCALISATION);

            if (!EMPLOYEE_group_PAY_GR0UP_LOCALISATION.isEmpty() &&
                !YerothUtils::isEqualCaseInsensitive(YerothERPConfig::THIS_SITE_LOCALISATION_NAME,
                                                     EMPLOYEE_group_PAY_GR0UP_LOCALISATION))
            {
                continue;
            }

            //QDEBUG_STRING_OUTPUT_1(QString("employeeGroup_db_ID (%1), EMPLOYEE_group_PAY_GR0UP => %2")
            //                         .arg(employeeGroup_db_ID,
            //                              EMPLOYEE_group_PAY_GR0UP));

            cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS =
                calculate_PAY_GROUP_MONEY_BENEFITS(const_cast<QString &>(a_nom_entreprise_EMPLOYEE),
                                                   EMPLOYEE_group_PAY_GR0UP);


            _sommeTotal += cur_EMPLOYEE_group_PAY_GR0UP_money_BENEFITS;
        }
    }


    return _sommeTotal;
    //lineEdit_EMPLOYE_SALAIRE_MENSUEL
    //->setText(GET_CURRENCY_STRING_NUM(_sommeTotal));
}


