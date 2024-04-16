/*
 * YEROTH_TABLEAUX_DE_BORDS_analyse_comparee_jour_semaine.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */


#include "../yeroth-erp-tableaux-de-bord-window.hpp"


#include "src/yeroth-erp-windows.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/process/yeroth-erp-process.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include "src/utils/yeroth-erp-map-COMPLEX-ITEM.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <unistd.h>

#include <QtWidgets/QFileDialog>

#include <QtCore/QDateTime>

#include <QtCore/QMapIterator>

#include <QtCore/QtMath>

#include <QtCore/QPair>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>





void YerothTableauxDeBordWindow::analyse_comparee_jour_semaine_VENTES_BENEFICES()
{
    _logger->log("analyse_comparee_jour_semaine_VENTES_BENEFICES");

    if (dateEdit_chiffre_daffaire_jour_semaine_fin->date() <
            dateEdit_chiffre_daffaire_jour_semaine_debut->date())
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("évolution du chiffre d'affaire"),
                                   QObject::tr
                                   ("Le jour de 'début' doit être "
                                    "antérieur au jour de la 'fin' !"));
        return;
    }

    QMap<int, double> dayOfWeek__TO__businessturnover;

    QMap<int, double> dayOfWeek__TO__PROFITS;

    qint64 dates_range =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->date().
                    daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;


    QString string_chiffre_daffaire_semaine_query;

    QString string_benefice_semaine_query;

    int current_day_of_week;

    QSqlQuery qsql_query;

    QDateTime current_day_date =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

    double a_temp_day_of_week_business_turnover = 0.0;

    double a_temp_day_of_week_PROFIT = 0.0;

    double current_day_business_turnover = 0.0;

    double current_day_PROFIT = 0.0;

    int current_query_size;

    qint64 i = 0;

    bool filtreActif = false;

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    QString yerothFiltre;

    QString databaseTableColumn;

    QString
    comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText
                                       ());

    QString
    textFromLineEditEvolutionSujets
    (comboBox_evolution_objets_value->currentText());

    do
    {
        current_day_of_week = current_day_date.date().dayOfWeek();

        string_chiffre_daffaire_semaine_query =
                        QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5'").arg
                        (YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                         YerothDatabaseTableColumn::MONTANT_TVA, YerothDatabase::STOCKS_VENDU,
                         YerothDatabaseTableColumn::DATE_VENTE,
                         DATE_TO_DB_FORMAT_STRING(current_day_date));

        string_benefice_semaine_query =
                        QString("SELECT %1 FROM %2 WHERE %3='%4'").arg
                        (YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
                         YerothDatabase::STOCKS_VENDU, YerothDatabaseTableColumn::DATE_VENTE,
                         DATE_TO_DB_FORMAT_STRING(current_day_date));


        string_chiffre_daffaire_semaine_query.append(QString
                                                     (" AND %1 = '%2'").arg
                                                     (YerothDatabaseTableColumn::LOCALISATION,
                                                      _current_succursale_STRING));


        string_benefice_semaine_query.append(QString(" AND %1 = '%2'").arg
                                             (YerothDatabaseTableColumn::
                                              LOCALISATION,
                                              _current_succursale_STRING));


        bool filtreActif = false;

        AUCUN_FILTRE = QObject::tr("aucun filtre !");

        yerothFiltre.clear();

        databaseTableColumn.clear();

        comboBoxEvolutionObjetsCurrentText =
                        comboBox_evolution_objets->currentText();

        textFromLineEditEvolutionSujets =
                        comboBox_evolution_objets_value->currentText();


        if (YerothUtils::isEqualCaseInsensitive
                (comboBoxEvolutionObjetsCurrentText,
                 YerothTableauxDeBordWindow::OBJET_CAISSIERS))
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_CAISSIERS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_CATEGORIES))
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_CATEGORIES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_CLIENTS))
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_CLIENTS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_ARTICLES))
        {
            if (textFromLineEditEvolutionSujets.isEmpty())
            {
                filtreActif = false;

                yerothFiltre.append(AUCUN_FILTRE);
            }
            else
            {
                filtreActif = true;

                yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                    (YerothTableauxDeBordWindow::
                                     OBJET_ARTICLES,
                                     textFromLineEditEvolutionSujets));

                databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
            }
        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
        {

            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::
                                 OBJET_FOURNISSEURS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_QUARTIER))
        {

            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_QUARTIER,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;

        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_VILLES))
        {

            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_VILLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

        }
        else if (YerothUtils::isEqualCaseInsensitive
                 (comboBoxEvolutionObjetsCurrentText,
                  YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS))
        {

            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::
                                 OBJET_REGIONS_ETATS,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

        }

        //              qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
        //                                              .arg(databaseTableColumn, comboBox_evolution_objets->currentText());

        if (true == filtreActif)
        {
            if (textFromLineEditEvolutionSujets.isEmpty())
            {
                YerothQMessageBox::warning(this,
                                           QObject::tr
                                           ("paramètre manquant"),
                                           QObject::tr
                                           ("Vous devez spécifier un paramètre dans "
                                            "le champs de texte 'nom' !"));
                return;
            }
            else
            {
                if (!databaseTableColumn.isEmpty())
                {
                    string_benefice_semaine_query.append(QString
                                                         (" AND %1='%2'").arg
                                                         (databaseTableColumn,
                                                          textFromLineEditEvolutionSujets));

                    string_chiffre_daffaire_semaine_query.append(QString
                                                                 (" AND %1='%2'").
                                                                 arg
                                                                 (databaseTableColumn,
                                                                  textFromLineEditEvolutionSujets));
                }
            }
        }

//              qDebug() << "++ string_achats_semaine_query: " << string_achats_semaine_query;

        qsql_query.clear();

        current_query_size =
                        YerothUtils::execQuery(qsql_query, string_benefice_semaine_query);

        if (current_query_size > 0)
        {
            while (qsql_query.next())
            {
                current_day_PROFIT = qsql_query.value(0).toDouble();

                QString STRING_current_day_of_week =
                                YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                (current_day_of_week);

                if (!YerothUtils::isEqualCaseInsensitive
                        (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                {
                    a_temp_day_of_week_PROFIT =
                                    dayOfWeek__TO__PROFITS[current_day_of_week];

                    dayOfWeek__TO__PROFITS.insert(current_day_of_week,
                                                  current_day_PROFIT +
                                                  a_temp_day_of_week_PROFIT);
                }
            }
        }

//              qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

        qsql_query.clear();

        current_query_size =
                        YerothUtils::execQuery(qsql_query,
                                               string_chiffre_daffaire_semaine_query);

        if (current_query_size > 0)
        {
            while (qsql_query.next())
            {
                current_day_business_turnover =
                                qsql_query.value(0).toDouble();

                QString STRING_current_day_of_week =
                                YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                (current_day_of_week);

                if (!YerothUtils::isEqualCaseInsensitive
                        (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                {
                    a_temp_day_of_week_business_turnover =
                                    dayOfWeek__TO__businessturnover[current_day_of_week];

                    dayOfWeek__TO__businessturnover.insert(current_day_of_week,
                                                           current_day_business_turnover
                                                           +
                                                           a_temp_day_of_week_business_turnover);
                }
            }
        }

        current_day_date = current_day_date.addDays(1);

        ++i;
    }
    while (i < dates_range);

    //qDebug() << dayOfWeek__TO__PROFITS
    //qDebug() << dayOfWeek__TO__businessturnover;

    _reportTexFileEndString.clear();


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QObject::tr("Details in %1:")
                            .arg(YerothERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QObject::tr("Détails en %1:")
                            .arg(YerothERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    //Fill in the PDF file which amount of money for each day of week.

    double somme_totale_ventes_jour_semaine = 0.0;

    QMap<int, QString> ventes_info;

    QMapIterator<int, double> it_ventes(dayOfWeek__TO__businessturnover);

    while (it_ventes.hasNext())
    {
        it_ventes.next();

        somme_totale_ventes_jour_semaine += it_ventes.value();

        if (YerothMainWindow::LANGUE_ANGLAISE)
        {
            ventes_info.insert(it_ventes.key(),
                               QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, ")
                                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it_ventes.key())),
                                     YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(it_ventes.value()))));
        }
        else
        {
            ventes_info.insert(it_ventes.key(),
                               QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, ")
                                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it_ventes.key())),
                                   YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(it_ventes.value()))));
        }
    }

    for (uint j = 1; j <= 7; ++j)
    {
        if (!ventes_info.contains(j))
        {
//                      QDEBUG_STRING_OUTPUT_2_N("j - VENTES 0", j);

            if (YerothMainWindow::LANGUE_ANGLAISE)
            {
                ventes_info.insert(j,
                                   QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, ")
                                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                            (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                         GET_CURRENCY_STRING_NUM(0.0)));
            }
            else
            {
                ventes_info.insert(j,
                                   QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, ")
                                    .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                            (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                         GET_CURRENCY_STRING_NUM(0.0)));
            }

        }
    }

//      qDebug() << "VENTES ventes_info: " << ventes_info;

//      QDEBUG_STRING_OUTPUT_2_N("somme_totale_ventes_jour_semaine", somme_totale_ventes_jour_semaine);

    double somme_totale_benefices_jour_semaine = 0.0;

    QString current_complete_string_benefice_info;

    QString current_prefix_string_benefice_info;


    QMapIterator<int, double> it_benefices(dayOfWeek__TO__PROFITS);

    QList<uint> benefices_jours_semaines_presents;

    for (uint j = 1; j <= 7; ++j)
    {
        if (it_benefices.hasNext())
        {
            it_benefices.next();

            somme_totale_benefices_jour_semaine += it_benefices.value();

            current_prefix_string_benefice_info =
                ventes_info.value(it_benefices.key());

            if (!current_prefix_string_benefice_info.isEmpty())
            {
                benefices_jours_semaines_presents.append(it_benefices.key());

                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_benefice_info =
                        current_prefix_string_benefice_info
                            .append(QString("profits $\\rightarrow %1$\n")
                                     .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                            (GET_CURRENCY_STRING_NUM(it_benefices.value()))));
                }
                else
                {
                    current_complete_string_benefice_info =
                        current_prefix_string_benefice_info
                            .append(QString("profits $\\rightarrow %1$\n")
                                     .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                            (GET_CURRENCY_STRING_NUM(it_benefices.value()))));
                }


                ventes_info.insert(it_benefices.key(),
                                   current_complete_string_benefice_info);
            }
        }
    }

//              qDebug() << "VENTES - BENEFICES COMPLETES I, ventes_info: " << ventes_info;

    for (uint j = 1; j <= 7; ++j)
    {
        if (!benefices_jours_semaines_presents.contains(j))
        {
//                      QDEBUG_STRING_OUTPUT_2_N("!achats_jours_semaines_presents.contains(j)", j);

            if (!ventes_info.contains(j))
            {
                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_benefice_info =
                        QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, profit $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                        GET_CURRENCY_STRING_NUM(0.0),
                        GET_CURRENCY_STRING_NUM(0.0));
                }
                else
                {
                    current_complete_string_benefice_info =
                        QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, bénéfice $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                        GET_CURRENCY_STRING_NUM(0.0),
                        GET_CURRENCY_STRING_NUM(0.0));
                }
            }
            else
            {
                current_prefix_string_benefice_info = ventes_info.value(j);

                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_benefice_info =
                        QString("%1 profit $\\rightarrow %2$\n")
                            .arg(current_prefix_string_benefice_info,
                                 GET_CURRENCY_STRING_NUM(0.0));
                }
                else
                {
                   current_complete_string_benefice_info =
                        QString("%1 bénéfice $\\rightarrow %2$\n")
                            .arg(current_prefix_string_benefice_info,
                                 GET_CURRENCY_STRING_NUM(0.0));
                }

            }

            ventes_info.insert(j, current_complete_string_benefice_info);
        }
    }

    for (uint j = 1; j <= 7; ++j)
    {
        _reportTexFileEndString.append(ventes_info.value(j));
    }

//      qDebug() << "VENTES-ACHATS-COMPLETE ventes_achats_info: " << ventes_achats_info;

    _reportTexFileEndString.append("\\end{enumerate}\n");

    QMap<int, QString> barItems;

    double MAX_AMOUNT_VENTES_BENEFICES = somme_totale_benefices_jour_semaine +
                                         somme_totale_ventes_jour_semaine;

    double ratio_ventes = 0.0;

    double ratio_benefices = 0.0;

    const int TICKS = 100;

    const double MAX_RATIO = 900.0;

    it_ventes.toFront();

    while (it_ventes.hasNext())
    {
        it_ventes.next();

        ratio_ventes =
                        (it_ventes.value() * MAX_RATIO) / MAX_AMOUNT_VENTES_BENEFICES;

        barItems.insert(it_ventes.key(),
                        QString("\\baritem{%1}{%2}{gray}\n").arg
                        (YerothUtils::LATEX_IN_OUT_handleForeignAccents
                         (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                          (it_ventes.key())), QString::number(ratio_ventes,
                                                              'f', 2)));
    }

    for (uint k = 1; k <= 7; ++k)
    {
        if (!barItems.contains(k))
        {
            barItems.insert(k,
                            QString("\\baritem{%1}{0}{gray}\n").arg
                            (YerothUtils::LATEX_IN_OUT_handleForeignAccents
                             (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                              (k))));
        }
    }

//              QDEBUG_QSTRINGLIST_OUTPUT("barItems - VENTES", barItems.values().join(" "));

    it_benefices.toFront();

    QString sub_bar_benefice;

    while (it_benefices.hasNext())
    {
        it_benefices.next();

        ratio_benefices =
                        (it_benefices.value() * MAX_RATIO) / MAX_AMOUNT_VENTES_BENEFICES;

        sub_bar_benefice =
                        QString(" \\subbaritem{}{%1}{purplish}\n").arg(QString::number
                                                                       (ratio_benefices,
                                                                        'f', 2));

        if (barItems.contains(it_benefices.key()))
        {
            barItems.insert(it_benefices.key(),
                            QString("%1%2").arg(barItems.
                                                value(it_benefices.key()),
                                                sub_bar_benefice));
        }
    }

    for (uint k = 1; k <= 7; ++k)
    {
        if (!barItems.contains(k))
        {
            sub_bar_benefice = QString(" \\subbaritem{}{0}{purplish}\n");

            barItems.insert(k, QString("%1%2").arg(barItems.value(k),
                                                   sub_bar_benefice));
        }
    }

//      qDebug() << "++ barItems: " << barItems.values().join(YerothUtils::EMPTY_STRING);

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);


    texDocument.replace("YEROTHBARPERGROUP", "");
    texDocument.replace("YEROTHBARITEMS",
                        barItems.values().join(YerothUtils::EMPTY_STRING));
    texDocument.replace("YEROTHTICKS", QString::number(TICKS));


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio of days of week profits.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio of days of week income.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", "Income Level");
    }
    else
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio des bénéfices jour--semaine.}{purplish}");

        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire jour--semaine.");

        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerothUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerothUtils::FR_bar_diag_tex);
    }


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }

    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS)  ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CATEGORIES) ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerothFiltre.clear();
            yerothFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerothFiltre = YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
    }

    texDocument2.replace("YEROTHMENTION", "");

    texDocument2.replace("YEROTHFILTRE",
                         yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", _reportTexFileEndString);

    texDocument2.replace("YEROTHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());

    texDocument2.replace("YEROTHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());

    texDocument2.replace("YEROTHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());

    texDocument2.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR",
                         QString("%1 %2")
                            .arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
                                 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument2.replace("YEROTHSUCCURSALE",
                         YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

    texDocument2.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);

    texDocument2.replace("YEROTHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());

    texDocument2.replace("YEROTHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());

    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Chart illustrating days of week income from %1 to %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }
    else
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire par jour de la semaine du %1 au %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);


    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire-jour-semaine"));

    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

//qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;

    QString texRm(tmpFile.fileName().remove(".tex"));

    progArguments << texRm;

//          qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

//          qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

//qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


//#################### #################### -------------- #################### ####################


void YerothTableauxDeBordWindow::
        GEN_THREE_DIMENSION_analyse_comparee_jour_semaine_ACHATS_VENTES(QString yerothFiltre,
                                                                        const QString &textFromLineEditEvolutionSujets,
                                                                        const QString &comboBoxEvolutionObjetsCurrentText)
{
    _GENERATE_THREE_D_BARD_DIAGRAM = false;

    QSqlQuery qsql_query;

    QStringList article_designation;

    QString
    article_designation_query =
                    QString("SELECT UNIQUE %1 FROM %2 WHERE %3='0'").arg
                    (YerothDatabaseTableColumn::DESIGNATION,
                     YerothDatabase::STOCKS_VENDU, YerothDatabaseTableColumn::IS_SERVICE);

    //QDEBUG_STRING_OUTPUT_2("article_designation_query", article_designation_query);


    int
    current_query_size =
                    YerothUtils::execQuery(qsql_query, article_designation_query);

    if (current_query_size > 0)
    {
        while (qsql_query.next())
        {
            article_designation.append(qsql_query.value(0).toString());
        }

        qsql_query.clear();
    }
    else
    {
        QString
        retMsg(QObject::tr
               ("Pas d'articles correspondants à la requête !"));

        YerothQMessageBox::information(this,
                                       QObject::tr("pas d'articles !"),
                                       retMsg);
        return;
    }


    //QDEBUG_QSTRINGLIST_OUTPUT("article_designation", article_designation);


    QString string_achats_semaine_query;

    QString string_chiffre_daffaire_semaine_query;


    QMap<int, double> dayOfWeek_TO_Purchases;

    QMap<QString, QMap<int, double>> article_TO_dayOfWeekPurchases;

    qint64
    dates_range =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->date().
                    daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;

    int current_day_of_week;


    double MAX_AMOUNT_ACHATS_VENTES = 0.0;

    double a_temp_day_of_week_purchases = 0.0;

    double a_temp_day_of_week_business_turnover = 0.0;

    QString current_designation;

    QDateTime
    current_day_date =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

    double current_day_purchases = 0.0;

    double current_day_business_turnover = 0.0;

    qint64 date_range_count = 0;

    QString STRING_current_day_of_week;

    for (uint k = 0; k < article_designation.size(); ++k)
    {
        current_designation = article_designation.at(k);

        current_day_date =
                        dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

        date_range_count = 0;

        do
        {
            current_day_of_week = current_day_date.date().dayOfWeek();

            string_achats_semaine_query =
                            QString("SELECT (%1 * %2) FROM %3 WHERE %4='%5' AND %6='%7'")
                                .arg(YerothDatabaseTableColumn::PRIX_DACHAT,
                                     YerothDatabaseTableColumn::QUANTITE_TOTALE,
                                     YerothDatabase::ACHATS, YerothDatabaseTableColumn::DATE_ENTREE,
                                     DATE_TO_DB_FORMAT_STRING(current_day_date),
                                     YerothDatabaseTableColumn::DESIGNATION,
                                     current_designation);
            //QDEBUG_STRING_OUTPUT_2("string_achats_semaine_query", string_achats_semaine_query);

            //QDEBUG_STRING_OUTPUT_2("string_chiffre_daffaire_semaine_query", string_chiffre_daffaire_semaine_query);

            qsql_query.clear();


            current_query_size =
                            YerothUtils::execQuery(qsql_query, string_achats_semaine_query);


            if (current_query_size > 0)
            {
                while (qsql_query.next())
                {
                    current_day_purchases = qsql_query.value(0).toDouble();

                    STRING_current_day_of_week =
                                    YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                    (current_day_of_week);

                    if (!YerothUtils::isEqualCaseInsensitive
                            (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                    {
                        //QDEBUG_STRING_OUTPUT_2("STRING_current_day_of_week", STRING_current_day_of_week);

                        dayOfWeek_TO_Purchases =
                                        article_TO_dayOfWeekPurchases.value
                                        (current_designation);

                        a_temp_day_of_week_purchases =
                                        dayOfWeek_TO_Purchases.value(current_day_of_week);

                        dayOfWeek_TO_Purchases.insert(current_day_of_week,
                                                      current_day_purchases +
                                                      a_temp_day_of_week_purchases);

                        article_TO_dayOfWeekPurchases.insert
                        (current_designation, dayOfWeek_TO_Purchases);

                        MAX_AMOUNT_ACHATS_VENTES =
                                        MAX_AMOUNT_ACHATS_VENTES +
                                        dayOfWeek_TO_Purchases.value(current_day_of_week);

                        //qDebug() << "article_TO_dayOfWeekPurchases: " << article_TO_dayOfWeekPurchases;
                    }
                }
            }

            //qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

            current_day_date = current_day_date.addDays(1);

            ++date_range_count;
        }
        while (date_range_count < dates_range);
    }



    QMap<int, double> dayOfWeek_TO_Businessturnover;

    QMap<QString, QMap<int, double>> article_TO_dayOfWeekBusinessturnover;


    for (uint k = 0; k < article_designation.size(); ++k)
    {
        current_designation = article_designation.at(k);

        current_day_date =
                        dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

        date_range_count = 0;

        do
        {
            current_day_of_week = current_day_date.date().dayOfWeek();

            string_chiffre_daffaire_semaine_query =
                            QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5' AND %6='%7'").arg
                            (YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                             YerothDatabaseTableColumn::MONTANT_TVA,
                             YerothDatabase::STOCKS_VENDU,
                             YerothDatabaseTableColumn::DATE_VENTE,
                             DATE_TO_DB_FORMAT_STRING(current_day_date),
                             YerothDatabaseTableColumn::DESIGNATION, current_designation);

            //QDEBUG_STRING_OUTPUT_2("string_chiffre_daffaire_semaine_query", string_chiffre_daffaire_semaine_query);

            qsql_query.clear();

            current_query_size =
                            YerothUtils::execQuery(qsql_query,
                                                   string_chiffre_daffaire_semaine_query);

            if (current_query_size > 0)
            {
                while (qsql_query.next())
                {
                    current_day_business_turnover =
                                    qsql_query.value(0).toDouble();

                    STRING_current_day_of_week =
                                    YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                    (current_day_of_week);

                    if (!YerothUtils::isEqualCaseInsensitive
                            (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                    {
                        //QDEBUG_STRING_OUTPUT_2("STRING_current_day_of_week - 2", STRING_current_day_of_week);

                        dayOfWeek_TO_Businessturnover =
                                        article_TO_dayOfWeekBusinessturnover.value
                                        (current_designation);

                        a_temp_day_of_week_purchases =
                                        dayOfWeek_TO_Businessturnover.value
                                        (current_day_of_week);

                        dayOfWeek_TO_Businessturnover.insert
                        (current_day_of_week,
                         current_day_purchases +
                         a_temp_day_of_week_purchases);

                        article_TO_dayOfWeekBusinessturnover.insert
                        (current_designation, dayOfWeek_TO_Businessturnover);

                        MAX_AMOUNT_ACHATS_VENTES =
                                        MAX_AMOUNT_ACHATS_VENTES +
                                        dayOfWeek_TO_Businessturnover.value
                                        (current_day_of_week);

                        //qDebug() << "article_TO_dayOfWeekBusinessturnover: " << article_TO_dayOfWeekBusinessturnover;
                    }
                }
            }

            current_day_date = current_day_date.addDays(1);

            ++date_range_count;
        }
        while (date_range_count < dates_range);
    }


    if (MAX_AMOUNT_ACHATS_VENTES <= 0)
    {
        //QDEBUG_STRING_OUTPUT_2_N("MAX_AMOUNT_ACHATS_VENTES", MAX_AMOUNT_ACHATS_VENTES);

        QString
        retMsg(QObject::tr
               ("Pas d'achats et de ventes correspondants à la requête !"));

        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("pas d'achats et de ventes !"),
                                       retMsg);

        return;
    }


    //QDEBUG_STRING_OUTPUT_2_N("MAX_AMOUNT_ACHATS_VENTES", MAX_AMOUNT_ACHATS_VENTES);

    //qDebug() << "article_TO_dayOfWeekPurchases: " << article_TO_dayOfWeekPurchases;
    //qDebug() << "article_TO_dayOfWeekBusinessturnover: " << article_TO_dayOfWeekBusinessturnover;

    _reportTexFileEndString.clear();
    /*
    #ifdef YEROTH_FRANCAIS_LANGUAGE
    	_reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    			QString("D\\'etails en %1:")
    			.arg(YerothERPConfig::currency)));
    #endif

    #ifdef YEROTH_ENGLISH_LANGUAGE
    	_reportTexFileEndString.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents(
    			QString("Details in %1:")
    			.arg(YerothERPConfig::currency)));
    #endif

    	_reportTexFileEndString.prepend("\\textbf{").append("}\n");
    	_reportTexFileEndString.append("\\begin{enumerate}[1.]\n");
    	*/


    QString YEROTHDEFINETOPCOLUMNCOLORS;

    for (int i = 1; i <= 7; ++i)
    {
        YEROTHDEFINETOPCOLUMNCOLORS.append(QString("\\def\\%1col{%2}\n"
                                                   "\\def\\%3topcol{%4}\n").
                                           arg
                                           (GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM
                                            (i),
                                            YEROTH_DTL_SET_BAR_PIE_COLOR_ARRAY
                                            [0],
                                            GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM
                                            (i),
                                            YEROTH_DTL_SET_BAR_PIE_COLOR_ARRAY
                                            [1]));
    }


    //QDEBUG_STRING_OUTPUT_2("YEROTHDEFINETOPCOLUMNCOLORS", YEROTHDEFINETOPCOLUMNCOLORS);


    //Fill in the PDF file which amount of money for each day of week.
    QMapIterator<QString, QMap<int, double>> it_ventes(article_TO_dayOfWeekBusinessturnover);

    QMapIterator<QString, QMap<int, double>> it_achats(article_TO_dayOfWeekPurchases);


    QMap<int, QString> day_TO_QStringBaritem;

    QMap<QString, QMap<int, QString>> PRODUCT_TO_barItems;

    QMap<int, double> it_dayOfWeek_TO_Businessturnover;

    double ratio_achats = 0.0;

    double ratio_ventes = 0.0;

    const double MAX_RATIO_THREE_D = 900.0;

    QString current_bar_top_item_STRING;

    uint ventes_count = 1;

    QString current_week_day;

    QList<int>days_of_week;

    QMap<QString, QString> weekday_TO_col;


    uint col_counter = 1;


    it_ventes.toFront();

    while (it_ventes.hasNext())
    {
        it_ventes.next();

        current_designation = it_ventes.key();

        day_TO_QStringBaritem = PRODUCT_TO_barItems.value(current_designation);

        it_dayOfWeek_TO_Businessturnover = it_ventes.value();

        days_of_week = it_dayOfWeek_TO_Businessturnover.keys();

        //qDebug() << "days_of_week: " << days_of_week;

        uint col_counter = 1;

        uint k = 1;

        for (uint j = 0; j < days_of_week.size(); ++j)
        {
            k = days_of_week.at(j);

            current_week_day =
                            YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(k);

            //QDEBUG_STRING_OUTPUT_2(QString::number(k), current_week_day);

            if (!weekday_TO_col.contains(current_week_day))
            {
                weekday_TO_col.insert(current_week_day,
                                      GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM
                                      (col_counter));
            }

            ++col_counter;
        }

        for (uint j = 0; j < days_of_week.size(); ++j)
        {
            k = days_of_week.at(j);

            ratio_ventes =
                            (it_dayOfWeek_TO_Businessturnover.value(k) * MAX_RATIO_THREE_D) /
                            MAX_AMOUNT_ACHATS_VENTES;

            current_bar_top_item_STRING = day_TO_QStringBaritem.value(k);

            current_week_day =
                            YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(k);

            if (0 != j)
            {
                current_bar_top_item_STRING.append(QString
                                                   ("\\subbaritem{%1}{%2}{\\%3col}\n").
                                                   arg(YerothUtils::
                                                       LATEX_IN_OUT_handleForeignAccents
                                                       (current_week_day),
                                                       QString::number
                                                       (ratio_ventes, 'f', 2),
                                                       weekday_TO_col.value
                                                       (current_week_day)));
            }
            else
            {
                current_bar_top_item_STRING.append(QString
                                                   ("\\baritem{%1}{%2}{\\%3col}\n").
                                                   arg(YerothUtils::
                                                       LATEX_IN_OUT_handleForeignAccents
                                                       (current_week_day),
                                                       QString::number
                                                       (ratio_ventes, 'f', 2),
                                                       weekday_TO_col.value
                                                       (current_week_day)));
            }


            day_TO_QStringBaritem.insert(k, current_bar_top_item_STRING);

            PRODUCT_TO_barItems.insert(current_designation,
                                       day_TO_QStringBaritem);

            //QDEBUG_STRING_OUTPUT_2("current_week_day - GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM(k); 1",
            //                                         QString("%1 - %2")
            //                                                 .arg(current_week_day,
            //                                                              weekday_TO_col.value(current_week_day)));
        }

        ++ventes_count;
    }


    //              QDEBUG_QSTRINGLIST_OUTPUT("barItems - VENTES", barItems.values().join(" "));

    it_achats.toFront();

    QString sub_bar_achat;

    QMap<int, double> it_dayOfWeek_TO_Purchases;

    uint achats_count = 1;

    while (it_achats.hasNext())
    {
        it_achats.next();

        current_designation = it_achats.key();

        day_TO_QStringBaritem = PRODUCT_TO_barItems.value(current_designation);

        it_dayOfWeek_TO_Purchases = it_achats.value();

        days_of_week = it_dayOfWeek_TO_Purchases.keys();

        uint k = 1;

        //qDebug() << "weekday_TO_col: " << weekday_TO_col;

        for (uint j = 0; j < days_of_week.size(); ++j)
        {
            k = days_of_week.at(j);

            current_week_day =
                            YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(k);

            //QDEBUG_STRING_OUTPUT_2(QString::number(k), current_week_day);

            if (!weekday_TO_col.contains(current_week_day))
            {
                weekday_TO_col.insert(current_week_day,
                                      GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM
                                      (col_counter));
            }

            ++col_counter;
        }


        for (uint j = 0; j < days_of_week.size(); ++j)
        {
            k = days_of_week.at(j);

            ratio_achats =
                            (it_dayOfWeek_TO_Purchases.value(k) * MAX_RATIO_THREE_D) /
                            MAX_AMOUNT_ACHATS_VENTES;

            current_bar_top_item_STRING = day_TO_QStringBaritem.value(k);

            current_week_day =
                            YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(k);

            sub_bar_achat =
                            QString("\t\\subtopbaritem{}{%1}{\\%2topcol}\n").arg
                            (YerothUtils::LATEX_IN_OUT_handleForeignAccents
                             (QString::number(ratio_achats, 'f', 2)),
                             weekday_TO_col.value(current_week_day));

            current_bar_top_item_STRING.append(sub_bar_achat);

            day_TO_QStringBaritem.insert(k, current_bar_top_item_STRING);

            PRODUCT_TO_barItems.insert(current_designation,
                                       day_TO_QStringBaritem);

            //QDEBUG_STRING_OUTPUT_2(
            //              QString("current_week_day - GET_SPELLED_NUMBER_FOR_THREED_BAR_DIAGRAM(%1); 11").arg(k),
            //              QString("%1 - %2").arg(current_week_day,
            //                                                         weekday_TO_col.value(current_week_day)));

            //QDEBUG_STRING_OUTPUT_2("current_bar_top_item_STRING - 2", current_bar_top_item_STRING);
        }

        ++achats_count;
    }


    QString YEROTHDEFINEBARSUBTOPBARITEMS;

    QStringList products_designations = PRODUCT_TO_barItems.keys();

    for (uint i = 0; i < products_designations.size(); ++i)
    {
        current_designation = products_designations.at(i);

        day_TO_QStringBaritem = PRODUCT_TO_barItems.value(current_designation);

        for (uint k = 1; k <= 7; ++k)
        {
            YEROTHDEFINEBARSUBTOPBARITEMS.append(QString("%1").
                                                 arg(day_TO_QStringBaritem.
                                                     value(k)));
        }

        YEROTHDEFINEBARSUBTOPBARITEMS.append("%---\n");
    }

    //QDEBUG_STRING_OUTPUT_2("YEROTHDEFINEBARSUBTOPBARITEMS", YEROTHDEFINEBARSUBTOPBARITEMS);

    QString YEROTHSUBGROUPS;

    for (uint k = 0; k < products_designations.size(); ++k)
    {
        current_designation = products_designations.at(k);

        if (PRODUCT_TO_barItems.value(current_designation).isEmpty())
        {
            continue;
        }

        if (k < products_designations.size() - 1)
        {
            YEROTHSUBGROUPS.append(QString(" \\centering %1 \& ").arg
                                   (YerothUtils::
                                    LATEX_IN_OUT_handleForeignAccents
                                    (current_designation.toLower())));
        }
        else
        {
            YEROTHSUBGROUPS.append(QString(" \\centering %1 ").arg
                                   (YerothUtils::
                                    LATEX_IN_OUT_handleForeignAccents
                                    (current_designation.toLower())));
        }
    }


    //      qDebug() << "++ barItems: " << barItems.values().join(YerothUtils::EMPTY_STRING);

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
    	texDocument.append(YerothUtils::EN_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex);
    }
    else
    {
    	texDocument.append(YerothUtils::FR_template_yeroth_erp_3D_BARDIAG_TEMPLATE_tex);
    }


    texDocument.replace("YEROTHSUBGROUPS", YEROTHSUBGROUPS);

    texDocument.replace("YEROTHDEFINETOPCOLUMNCOLORS", YEROTHDEFINETOPCOLUMNCOLORS);

    texDocument.replace("YEROTHDEFINEBARSUBTOPBARITEMS", YEROTHDEFINEBARSUBTOPBARITEMS);
    texDocument.replace("YEROTHBASELEGEND",
                        QObject::tr("\\diagLegenditem{Ratio du chiffre d'affaire jour--semaine.}{\\onecol}"));

    texDocument.replace("YEROTHTOPLEGEND",
                        QObject::tr("\\diagLegenditem{Ratio des achats jour--semaine.}{\\onetopcol}"));

    texDocument.replace("YEROTHX", QObject::tr("Jours de semaines."));

    texDocument.replace("YEROTHZ",
                        QObject::tr("Ratio du chiffre d'affaire jour--semaine."));

    QString fileName1(YerothERPConfig::temporaryFilesDir +
    				  "/YEROTH-ERP-3D-BARDIAG-TEMPLATE.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();


    //QDEBUG_STRING_OUTPUT_2("texDocument", texDocument);


    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerothUtils::EN_template_THREE_D_bardiag_tex);
    }
    else
    {
        texDocument2.append(YerothUtils::FR_template_THREE_D_bardiag_tex);
    }


    QString
    factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                (infoEntreprise.getVille_LATEX()));
    YerothUtils::getCurrentLocaleDate(factureDate);


    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");

    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS)
            ||
            YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                                YerothTableauxDeBordWindow::OBJET_CATEGORIES)
            ||
            YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                                YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerothFiltre.clear();
            yerothFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerothFiltre =
                        YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
    }


    QString longDateDebut;
    QString longDateFin;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::englishLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }
    else
    {
        longDateDebut =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
        QString("'%1'").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                            (YerothUtils::frenchLocale.toString
                             (dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }


    texDocument2.replace("YEROTHMENTION", "");

    texDocument2.replace("YEROTHFILTRE",
                         yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHSUBJECT",
                         QObject::tr
                         ("Diagramme représentatif des pourcentages "
                          "DES ACHATS ET DES VENTES par jour de semaine."));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", _reportTexFileEndString);

    texDocument2.replace("YEROTHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());

    texDocument2.replace("YEROTHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());

    texDocument2.replace("YEROTHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());
    texDocument2.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());

    texDocument2.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR",
                         QString("%1 %2")
                            .arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
                                 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument2.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);

    texDocument2.replace("YEROTHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());

    texDocument2.replace("YEROTHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());

    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("YEROTH-ERP-3D-BARDIAG-TEMPLATE.tex", fileName1);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagram illustrating PURCHASE sales "
                                     "percentages per days of week from %1 to %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }
    else
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagramme représentatif des pourcentages "
                                     "DES ACHATS ET DES VENTES par jour de semaine du %1 au %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString
    fileName(FILE_NAME_USERID_CURRENT_TIME
             ("evolution-chiffre-affaire-jour-semaine_trois_D"));
    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");
    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }
    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //          qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    //          qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}


//#################### #################### -------------- #################### ####################


void YerothTableauxDeBordWindow::analyse_comparee_jour_semaine_ACHATS_VENTES()
{
    _logger->log("analyse_comparee_jour_semaine_ACHATS_VENTES");

    if (dateEdit_chiffre_daffaire_jour_semaine_fin->date() <
            dateEdit_chiffre_daffaire_jour_semaine_debut->date())
    {
        YerothQMessageBox::warning(this,
                                   QObject::tr
                                   ("évolution du chiffre d'affaire"),
                                   QObject::tr
                                   ("Le jour de 'début' doit être "
                                    "antérieur au jour de la 'fin' !"));
        return;
    }

    QMap<int, double> dayOfWeek__TO__purchases;

    QMap<int, double> dayOfWeek__TO__businessturnover;

    qint64 dates_range =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->date().
                    daysTo(dateEdit_chiffre_daffaire_jour_semaine_fin->date()) + 1;


    QString string_achats_semaine_query;

    QString string_chiffre_daffaire_semaine_query;

    int current_day_of_week;

    QSqlQuery qsql_query;

    QDateTime current_day_date =
                    dateEdit_chiffre_daffaire_jour_semaine_debut->dateTime();

    double a_temp_day_of_week_purchases = 0.0;

    double a_temp_day_of_week_business_turnover = 0.0;

    double current_day_purchases = 0.0;

    double current_day_business_turnover = 0.0;

    int current_query_size;

    qint64 i = 0;

    bool filtreActif = false;

    QString AUCUN_FILTRE(QObject::tr("aucun filtre !"));

    QString yerothFiltre;

    QString databaseTableColumn;

    QString comboBoxEvolutionObjetsCurrentText(comboBox_evolution_objets->currentText());

    QString textFromLineEditEvolutionSujets(comboBox_evolution_objets_value->currentText());


    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS))
    {
        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_CAISSIERS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_CAISSIER;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CATEGORIES))
    {
        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_CATEGORIES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerothDatabaseTableColumn::CATEGORIE;
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_CLIENTS,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn = YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_ARTICLES))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            _GENERATE_THREE_D_BARD_DIAGRAM = true;

            filtreActif = false;

            yerothFiltre.append(AUCUN_FILTRE);
        }
        else
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_ARTICLES,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn = YerothDatabaseTableColumn::DESIGNATION;
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_FOURNISSEURS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_FOURNISSEURS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_QUARTIER))
    {

        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            _GENERATE_THREE_D_BARD_DIAGRAM = true;

            filtreActif = false;

            yerothFiltre.append(AUCUN_FILTRE);
        }
        else
        {
            filtreActif = true;

            yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                                (YerothTableauxDeBordWindow::OBJET_QUARTIER,
                                 textFromLineEditEvolutionSujets));

            databaseTableColumn =
                            YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT;
        }
    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_VILLES))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").
                            arg(YerothTableauxDeBordWindow::OBJET_VILLES,
                                textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT;

    }
    else if (YerothUtils::isEqualCaseInsensitive
             (comboBoxEvolutionObjetsCurrentText,
              YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS))
    {

        filtreActif = true;

        yerothFiltre.append(QObject::tr("filtre: %1 = %2").arg
                            (YerothTableauxDeBordWindow::OBJET_REGIONS_ETATS,
                             textFromLineEditEvolutionSujets));

        databaseTableColumn =
                        YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT;

    }

    //              qDebug() << QString("databaseTableColumn: %1, currentTextFromComboBoxEvolutionSujets: %2")
    //                                              .arg(databaseTableColumn, comboBox_evolution_objets->currentText());

    //QDEBUG_STRING_OUTPUT_2("_GENERATE_THREE_D_BARD_DIAGRAM", BOOL_TO_STRING(_GENERATE_THREE_D_BARD_DIAGRAM));


    if (_GENERATE_THREE_D_BARD_DIAGRAM)
    {
        GEN_THREE_DIMENSION_analyse_comparee_jour_semaine_ACHATS_VENTES
        (yerothFiltre, textFromLineEditEvolutionSujets,
         comboBoxEvolutionObjetsCurrentText);

        return;
    }


    do
    {
        current_day_of_week = current_day_date.date().dayOfWeek();

        string_achats_semaine_query =
                        QString("SELECT (%1 * %2) FROM %3 WHERE %4='%5'").arg
                        (YerothDatabaseTableColumn::PRIX_DACHAT,
                         YerothDatabaseTableColumn::QUANTITE_TOTALE, YerothDatabase::ACHATS,
                         YerothDatabaseTableColumn::DATE_ENTREE,
                         DATE_TO_DB_FORMAT_STRING(current_day_date));


        string_chiffre_daffaire_semaine_query =
                        QString("SELECT (%1 - %2) FROM %3 WHERE %4='%5'").arg
                        (YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                         YerothDatabaseTableColumn::MONTANT_TVA, YerothDatabase::STOCKS_VENDU,
                         YerothDatabaseTableColumn::DATE_VENTE,
                         DATE_TO_DB_FORMAT_STRING(current_day_date));


        if (true == filtreActif)
        {
            if (textFromLineEditEvolutionSujets.isEmpty())
            {
                YerothQMessageBox::warning(this,
                                           QObject::tr
                                           ("paramètre manquant"),
                                           QObject::tr
                                           ("Vous devez spécifier un paramètre dans "
                                            "le champs de texte 'nom' !"));
                return;
            }
            else
            {
                if (!databaseTableColumn.isEmpty())
                {
                    string_achats_semaine_query.append(QString
                                                       (" AND %1='%2'").arg
                                                       (databaseTableColumn,
                                                        textFromLineEditEvolutionSujets));

                    string_chiffre_daffaire_semaine_query.append(QString
                                                                 (" AND %1='%2'").
                                                                 arg
                                                                 (databaseTableColumn,
                                                                  textFromLineEditEvolutionSujets));
                }
            }
        }

        //QDEBUG_STRING_OUTPUT_2("string_achats_semaine_query", string_achats_semaine_query);

        //QDEBUG_STRING_OUTPUT_2("string_chiffre_daffaire_semaine_query", string_chiffre_daffaire_semaine_query);

        qsql_query.clear();


        current_query_size =
                        YerothUtils::execQuery(qsql_query, string_achats_semaine_query);


        if (current_query_size > 0)
        {
            while (qsql_query.next())
            {
                current_day_purchases = qsql_query.value(0).toDouble();

                QString STRING_current_day_of_week =
                                YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                (current_day_of_week);

                if (!YerothUtils::isEqualCaseInsensitive
                        (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                {
                    a_temp_day_of_week_purchases =
                                    dayOfWeek__TO__purchases[current_day_of_week];

                    dayOfWeek__TO__purchases.insert(current_day_of_week,
                                                    current_day_purchases +
                                                    a_temp_day_of_week_purchases);
                }
            }
        }

        //              qDebug() << "++ string_chiffre_daffaire_semaine_query: " << string_chiffre_daffaire_semaine_query;

        qsql_query.clear();

        current_query_size =
                        YerothUtils::execQuery(qsql_query,
                                               string_chiffre_daffaire_semaine_query);

        if (current_query_size > 0)
        {
            while (qsql_query.next())
            {
                current_day_business_turnover =
                                qsql_query.value(0).toDouble();

                QString STRING_current_day_of_week =
                                YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                                (current_day_of_week);

                if (!YerothUtils::isEqualCaseInsensitive
                        (YerothUtils::EMPTY_STRING, STRING_current_day_of_week))
                {
                    a_temp_day_of_week_business_turnover =
                                    dayOfWeek__TO__businessturnover[current_day_of_week];

                    dayOfWeek__TO__businessturnover.insert(current_day_of_week,
                                                           current_day_business_turnover
                                                           +
                                                           a_temp_day_of_week_business_turnover);
                }
            }
        }

        current_day_date = current_day_date.addDays(1);

        ++i;
    }
    while (i < dates_range);

    //      qDebug() << dayOfWeek__TO__purchases;
    //      qDebug() << dayOfWeek__TO__businessturnover;

    _reportTexFileEndString.clear();


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QObject::tr("Details in %1:")
                            .arg(YerothERPConfig::currency)));
    }
    else
    {
        _reportTexFileEndString
            .append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (QObject::tr("Détails en %1:")
                            .arg(YerothERPConfig::currency)));
    }


    _reportTexFileEndString.prepend("\\textbf{").append("}\n");
    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    //Fill in the PDF file which amount of money for each day of week.

    double somme_totale_ventes_jour_semaine = 0.0;

    QMap<int, QString> ventes_achats_info;

    QMapIterator<int, double> it(dayOfWeek__TO__businessturnover);

    while (it.hasNext())
    {
        it.next();

        somme_totale_ventes_jour_semaine += it.value();

        if (YerothMainWindow::LANGUE_ANGLAISE)
        {
            ventes_achats_info.insert(it.key(),
                                      QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, ")
                                        .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
                                             YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                (GET_CURRENCY_STRING_NUM(it.value()))));
        }
        else
        {
            ventes_achats_info.insert(it.key(),
                                      QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, ")
                                        .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
                                             YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                (GET_CURRENCY_STRING_NUM(it.value()))));
        }
    }
    for (uint j = 1; j <= 7; ++j)
    {
        if (!ventes_achats_info.contains(j))
        {
            //                      QDEBUG_STRING_OUTPUT_2_N("j - VENTES 0", j);
            if (YerothMainWindow::LANGUE_ANGLAISE)
            {
                ventes_achats_info.insert(j,
                                          QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, ")
                                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                                 GET_CURRENCY_STRING_NUM(0.0)));
            }
            else
            {
                ventes_achats_info.insert(j,
                                          QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, ")
                                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                                 GET_CURRENCY_STRING_NUM(0.0)));
            }
        }
    }

    //      qDebug() << "VENTES ventes_achats_info: " << ventes_achats_info;

    //      QDEBUG_STRING_OUTPUT_2_N("somme_totale_ventes_jour_semaine", somme_totale_ventes_jour_semaine);

    double somme_totale_achats_jour_semaine = 0.0;

    QString current_complete_string_vente_info;

    QString current_prefix_string_vente_info;


    QMapIterator<int, double> it_achats(dayOfWeek__TO__purchases);

    QList<uint> achats_jours_semaines_presents;

    for (uint j = 1; j <= 7; ++j)
    {
        if (it_achats.hasNext())
        {
            it_achats.next();

            somme_totale_achats_jour_semaine += it_achats.value();

            current_prefix_string_vente_info =
                            ventes_achats_info.value(it_achats.key());
            if (!current_prefix_string_vente_info.isEmpty())
            {
                achats_jours_semaines_presents.append(it_achats.key());

                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_vente_info =
                        current_prefix_string_vente_info.append
                            (QString("purchases $\\rightarrow %1$\n")
                                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(it_achats.value()))));
                }
                else
                {
                    current_complete_string_vente_info =
                        current_prefix_string_vente_info.append
                            (QString("achats $\\rightarrow %1$\n")
                                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                        (GET_CURRENCY_STRING_NUM(it_achats.value()))));
                }

                ventes_achats_info.insert(it_achats.key(),
                                          current_complete_string_vente_info);
            }
        }
    }

    //              qDebug() << "VENTES - ACHATS COMPLETES I, ventes_achats_info: " << ventes_achats_info;

    for (uint j = 1; j <= 7; ++j)
    {
        if (!achats_jours_semaines_presents.contains(j))
        {
            //                      QDEBUG_STRING_OUTPUT_2_N("!achats_jours_semaines_presents.contains(j)", j);
            if (!ventes_achats_info.contains(j))
            {
                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_vente_info =
                        QString("\\item \\textbf{%1}: sales $\\rightarrow %2$, purchases $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                     GET_CURRENCY_STRING_NUM(0.0),
                                     GET_CURRENCY_STRING_NUM(0.0));
                }
                else
                {
                    current_complete_string_vente_info =
                        QString("\\item \\textbf{%1}: ventes $\\rightarrow %2$, purchases $\\rightarrow %3$\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(j)),
                                     GET_CURRENCY_STRING_NUM(0.0),
                                     GET_CURRENCY_STRING_NUM(0.0));
                }
            }
            else
            {
                current_prefix_string_vente_info = ventes_achats_info.value(j);

                if (YerothMainWindow::LANGUE_ANGLAISE)
                {
                    current_complete_string_vente_info =
                        QString("%1 purchases $\\rightarrow %2$\n")
                            .arg(current_prefix_string_vente_info,
                                 GET_CURRENCY_STRING_NUM(0.0));
                }
                else
                {
                    current_complete_string_vente_info =
                        QString("%1 achats $\\rightarrow %2$\n")
                            .arg(current_prefix_string_vente_info,
                                 GET_CURRENCY_STRING_NUM(0.0));
                }
            }

            ventes_achats_info.insert(j, current_complete_string_vente_info);
        }
    }


    for (uint j = 1; j <= 7; ++j)
    {
        _reportTexFileEndString.append(ventes_achats_info.value(j));
    }


    //      qDebug() << "VENTES-ACHATS-COMPLETE ventes_achats_info: " << ventes_achats_info;

    _reportTexFileEndString.append("\\end{enumerate}\n");

    QMap<int, QString> barItems;

    double MAX_AMOUNT_ACHATS_VENTES = somme_totale_achats_jour_semaine +
                                      somme_totale_ventes_jour_semaine;

    double ratio_achats = 0.0;

    double ratio_ventes = 0.0;

    const int TICKS = 100;

    const double MAX_RATIO = 900.0;

    it.toFront();

    while (it.hasNext())
    {
        it.next();

        ratio_ventes = (it.value() * MAX_RATIO) / MAX_AMOUNT_ACHATS_VENTES;

        barItems.insert(it.key(),
                        QString("\\baritem{%1}{%2}{gray}\n")
                            .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                    (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT(it.key())),
                                     QString::number(ratio_ventes, 'f', 2)));
    }

    for (uint k = 1; k <= 7; ++k)
    {
        if (!barItems.contains(k))
        {
            barItems.insert(k,
                            QString("\\baritem{%1}{0}{gray}\n").arg
                            (YerothUtils::LATEX_IN_OUT_handleForeignAccents
                             (YerothUtils::GET_DAYOFWEEK_FROM_QT_INT_CONSTANT
                              (k))));
        }
    }

    //              QDEBUG_QSTRINGLIST_OUTPUT("barItems - VENTES", barItems.values().join(" "));

    it_achats.toFront();

    QString sub_bar_achat;

    while (it_achats.hasNext())
    {
        it_achats.next();

        ratio_achats =
                        (it_achats.value() * MAX_RATIO) / MAX_AMOUNT_ACHATS_VENTES;

        sub_bar_achat =
                        QString(" \\subbaritem{}{%1}{purplish}\n").
                        arg(QString::number(ratio_achats, 'f', 2));

        if (barItems.contains(it_achats.key()))
        {
            barItems.insert(it_achats.key(),
                            QString("%1%2").arg(barItems.
                                                value(it_achats.key()),
                                                sub_bar_achat));
        }
    }

    for (uint k = 1; k <= 7; ++k)
    {
        if (!barItems.contains(k))
        {
            sub_bar_achat = QString(" \\subbaritem{}{0}{purplish}\n");

            barItems.insert(k, QString("%1%2").arg(barItems.value(k),
                                                   sub_bar_achat));
        }
    }

    //      qDebug() << "++ barItems: " << barItems.values().join(YerothUtils::EMPTY_STRING);

    QProcess aProcess;

    aProcess.setWorkingDirectory(YerothERPConfig::temporaryFilesDir);

    QString texDocument;
    texDocument.append(YerothUtils::_1a_tex);


    texDocument.replace("YEROTHBARPERGROUP", "");

    texDocument.replace("YEROTHBARITEMS",
                        barItems.values().join(YerothUtils::EMPTY_STRING));

    texDocument.replace("YEROTHTICKS", QString::number(TICKS));


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio of days of week purchasing.}{purplish}");
        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio of days of week income.");
        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE", "Income Level");
    }
    else
    {
        texDocument.replace("YEROTHLEGENDANALYSECOMPAREE",
                            "\\diagLegenditem{Ratio des achats jour--semaine.}{purplish}");
        texDocument.replace("YEROTHDIAGRAMMETITRE",
                            "Ratio du chiffre d'affaire jour--semaine.");
        texDocument.replace("YEROTHNIVEAUCHIFFREAFFAIRE",
                            "Niveau du chiffre d'affaire");
    }


    QString fileName1(YerothERPConfig::temporaryFilesDir + "/1a.tex");

    QFile tmpFile1(fileName1);

    if (tmpFile1.open(QFile::WriteOnly))
    {
        tmpFile1.write(texDocument.toUtf8());
    }
    tmpFile1.close();

    YerothInfoEntreprise &infoEntreprise = _allWindows->getInfoEntreprise();

    QString texDocument2;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.append(YerothUtils::EN_bar_diag_tex);
    }
    else
    {
        texDocument2.append(YerothUtils::FR_bar_diag_tex);
    }


    QString factureDate(YerothUtils::LATEX_IN_OUT_handleForeignAccents(infoEntreprise.getVille_LATEX()));

    YerothUtils::getCurrentLocaleDate(factureDate);

    QString longDateDebut;
    QString longDateFin;


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        longDateDebut =
            QString("'%1'")
                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
            QString("'%1'")
                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerothUtils::englishLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }
    else
    {
        longDateDebut =
            QString("'%1'")
                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_debut->date())));

        longDateFin =
            QString("'%1'")
                .arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                        (YerothUtils::frenchLocale.toString(dateEdit_chiffre_daffaire_jour_semaine_fin->date())));
    }


    //qDebug() << "++ type fact. rapports - chiffe affaire: " << YerothConfig::typeOfFacturation;


    texDocument2.replace("YEROTHPAPERSPEC", "a4paper");


    if (YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CAISSIERS)  ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CATEGORIES) ||
        YerothUtils::isEqualCaseInsensitive(comboBoxEvolutionObjetsCurrentText,
                                            YerothTableauxDeBordWindow::OBJET_CLIENTS))
    {
        if (textFromLineEditEvolutionSujets.isEmpty())
        {
            yerothFiltre.clear();
            yerothFiltre.append(QObject::tr("aucun filtre !"));
        }
    }
    else
    {
        yerothFiltre = YerothUtils::LATEX_IN_OUT_handleForeignAccents(yerothFiltre);
    }


    texDocument2.replace("YEROTHMENTION", "");

    texDocument2.replace("YEROTHFILTRE",
                         yerothFiltre.prepend("''").append("''"));

    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEDEBUT", longDateDebut);
    texDocument2.replace("YEROTHCHIFFREAFFAIREDATEFIN", longDateFin);
    texDocument2.replace("YEROTHCHARTFIN", _reportTexFileEndString);
    texDocument2.replace("YEROTHENTREPRISE",
                         infoEntreprise.getNomCommercial_LATEX());

    texDocument2.replace("YEROTHACTIVITESENTREPRISE",
                         infoEntreprise.getSecteursActivitesTex());

    texDocument2.replace("YEROTHBOITEPOSTALE",
                         infoEntreprise.getBoitePostal());

    texDocument2.replace("YEROTHVILLE", infoEntreprise.getVille_LATEX());
    texDocument2.replace("YEROTHPAYS", infoEntreprise.getPaysTex());
    texDocument2.replace("YEROTHEMAIL", infoEntreprise.getEmail_LATEX());
    texDocument2.replace("YEROTHTELEPHONE", infoEntreprise.getTelephone());
    texDocument2.replace("YEROTHDATE", factureDate);

    texDocument2.replace("YEROTHNOMUTILISATEUR",
                         QString("%1 %2")
                            .arg(YerothUtils::getAllWindows()->getUser()->titreTex(),
                                 YerothUtils::getAllWindows()->getUser()->nom_completTex()));

    texDocument2.replace("YEROTHSUCCURSALE",
                         YerothUtils::LATEX_IN_OUT_handleForeignAccents
						 	 (YerothERPConfig::THIS_SITE_LOCALISATION_NAME));

    texDocument2.replace("YEROTHHEUREDIMPRESSION", CURRENT_TIME);
    texDocument2.replace("YEROTHCOMPTEBANCAIRENR",
                         infoEntreprise.getNumeroCompteBancaire());
    texDocument2.replace("YEROTHCONTRIBUABLENR",
                         infoEntreprise.getNumeroDeContribuable());
    texDocument2.replace("YEROTHAGENCECOMPTEBANCAIRE",
                         infoEntreprise.getAgenceCompteBancaireTex());

    texDocument2.replace("1a.tex", fileName1);


    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Chart illustrating days of week income from %1 to %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }
    else
    {
        texDocument2.replace("YEROTHTITREDOCUMENT",
                             QString("Diagramme r\\'epr\\'esentatif des chiffres"
                                     " d'affaire par jour de la semaine du %1 au %2.")
                                .arg(longDateDebut,
                                     longDateFin));
    }


    //qDebug() << "++ test: " << texDocument2;

    YerothUtils::LATEX_IN_OUT_handleForeignAccents(texDocument2);

    QString fileName(FILE_NAME_USERID_CURRENT_TIME("evolution-chiffre-affaire-jour-semaine"));

    fileName.append(".");

    QString tmpFilePrefix(YerothERPConfig::temporaryFilesDir + "/" + fileName);

    QFile tmpFile(tmpFilePrefix + "tex");

    if (tmpFile.open(QFile::WriteOnly))
    {
        tmpFile.write(texDocument2.toUtf8());
    }

    tmpFile.close();

    //qDebug() << "++ tmpFile: " << tmpFile.fileName();

    QStringList progArguments;
    QString texRm(tmpFile.fileName().remove(".tex"));
    progArguments << texRm;

    //          qDebug() << "++ file name to latex compile bar diag: " << texRm;

    aProcess.start(YerothERPConfig::pathToLatex(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("%1.dvi").arg(texRm);

    aProcess.start(YerothERPConfig::pathToDvips(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << QString("-dALLOWPSTRANSPARENCY");
    progArguments << QString("%1.ps").arg(texRm);
    progArguments << QString("%1.pdf").arg(texRm);

    aProcess.start(YerothERPConfig::pathToPs2Pdf(), progArguments);
    aProcess.waitForFinished(-1);

    progArguments.clear();
    progArguments << tmpFilePrefix + "pdf";

    //          qDebug() << "++ test it: " << tmpFilePrefix + "pdf";

    aProcess.startDetached(YerothERPConfig::pathToPdfReader, progArguments);

    //qDebug() << "\texit status evince: " << _aProcess->exitStatus();
}
