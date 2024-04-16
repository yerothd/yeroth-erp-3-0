/*
 * YEROTH_TABLEAUX_DE_BORDS_zero_stats_stocks.cpp
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


void YerothTableauxDeBordWindow::ZERO_stats_stocks(QString fileName,
                                                   QString fieldId)
{
    _logger->log("ZERO_stats_stocks");

    QString strQuery(QString("SELECT %1, prix_dachat, quantite_totale "
                             "FROM %2 s "
                             "WHERE s.prix_vente > 0 AND s.%3 NOT IN "
                             "(select %4 "
                             "from %5 sv "
                             "where sv.%6 = s.%7 "
                             "AND date_vente >= '%8' AND date_vente <= '%9')").
                     arg(fieldId, YerothDatabase::STOCKS, fieldId, fieldId,
                         YerothDatabase::STOCKS_VENDU, fieldId, fieldId,
                         DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->
                                                  date()),
                         DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->
                                                  date())));


    strQuery.append(QString(" AND %1 = '%2'").arg
                    (YerothDatabaseTableColumn::LOCALISATION,
                     _current_succursale_STRING));


    //QDEBUG_STRING_OUTPUT_2("YerothTableauxDeBordWindow::ZERO_stats, strQuery", strQuery);

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

//    QDEBUG_STRING_OUTPUT_2_N("querySize", querySize);

    double prix_dachat = 0.0;

    double valeur_dinventaire = 0.0;

    double quantite_totale = 0.0;

    QString fieldIdValue;

    Yeroth_MAP_COMPLEX_Item *anItem = 0;

    QList < Yeroth_MAP_COMPLEX_Item * >allItems;

    if (querySize > 0)
    {
        while (query.next())
        {
            fieldIdValue = query.value(0).toString();

            prix_dachat = query.value(1).toDouble();

            quantite_totale = query.value(2).toDouble();

            int idx = -1;
            for (int i = 0; i < allItems.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive
                        (allItems.value(i)->_itemName, fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }


            if (-1 == idx)
            {
                anItem = new Yeroth_MAP_COMPLEX_Item(fieldIdValue, 0.0);

                anItem->_itemName = fieldIdValue;

                allItems.push_back(anItem);
            }
            else
            {
                anItem = allItems.value(idx);
            }

            valeur_dinventaire = (quantite_totale * prix_dachat);

            anItem->_itemValue += quantite_totale;

            anItem->_itemSecondValue += valeur_dinventaire;
        }
    }

    qSort(allItems.begin(), allItems.end(),
          Yeroth_MAP_COMPLEX_Item::lessThan_second_value);

    QString csvFileContent;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double valeur_dinventaire_toute_marchandise = 0.0;

    QString label;
    QString value;

    for (int j = allItems.size() - 1, k = 0; j > -1; --j, ++k)
    {
        label.clear();

        label.append(QString("\"%1\"").arg
                     (YerothUtils::LATEX_IN_OUT_handleForeignAccents
                      (allItems.at(j)->_itemName)));

        value.clear();

        value.append(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                     (GET_DOUBLE_STRING(allItems.at(j)->_itemValue)));

        valeur_dinventaire = allItems.at(j)->_itemSecondValue;

        valeur_dinventaire_toute_marchandise += valeur_dinventaire;

        csvFileContent.prepend(QString("%1; %2; %3; %4\n").arg
                               (allItems.at(j)->_itemName, value,
                                GET_DOUBLE_STRING(valeur_dinventaire),
                                QString::number(0.0)));

        _reportTexFileEndString.append(QObject::tr("\\item %1 | "
                                                       "Qté en stock: \"%2\" | "
                                                       "Valeur d'inventaire: \"%3\"\n").
                                       arg(label, value,
                                           GET_CURRENCY_STRING_NUM
                                           (valeur_dinventaire)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}" "\\vspace{1em}");

    _reportTexFileEndString.append(QObject::tr
                                   ("\\underline{\\textbf{SOMME (VALEUR D'INVENTAIRE TOUT STOCK): %1}}").
                                   arg(GET_CURRENCY_STRING_NUM
                                       (valeur_dinventaire_toute_marchandise)));

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    csvFileContent.prepend(QObject::tr
                           ("Nom; Quantité totale en stock; Valeur d'inventaire (%1); Total chiffre d'affaire\n").
                           arg(YerothERPConfig::currency));

    //clean up
    for (int k = 0; k < allItems.size(); ++k)
    {
        delete allItems.value(k);
    }

    QFile csvFile(fileName);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        _logger->log("ZERO_stats", csvFile.errorString());
    }
    else
    {
        QTextStream outCsvFile(&csvFile);

        outCsvFile << csvFileContent;

        //qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

        csvFile.close();
    }
}
