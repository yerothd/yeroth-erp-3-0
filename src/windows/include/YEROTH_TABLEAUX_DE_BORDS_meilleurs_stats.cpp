/*
 * YEROTH_TABLEAUX_DE_BORDS_meilleurs_stats.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
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


void YerothTableauxDeBordWindow::meilleursStats(QString fileName,
                                                QString fieldId,
                                                int size,
                                                bool service /* = false */)
{
    _logger->log("meilleursStats");

    QString strQuery;

    if (!service)
    {
        strQuery.append(QString
                        ("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5 WHERE is_service = '0' AND %6 >= '%7' AND %8 <= '%9'").
                        arg(fieldId,
                            YerothDatabaseTableColumn::QUANTITE_VENDUE,
                            YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
                            YerothDatabaseTableColumn::MONTANT_TVA,
                            YerothDatabase::STOCKS_VENDU,
                            YerothDatabaseTableColumn::DATE_VENTE,
                            DATE_TO_DB_FORMAT_STRING
                            (dateEdit_rapports_debut->date()),
                            YerothDatabaseTableColumn::DATE_VENTE,
                            DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->
                                                     date())));
    }
    else				//service == true
    {
        strQuery.append(QString
                        ("SELECT %1, %2, (round(%3, 2) - round(%4, 2)) FROM %5"
                         " WHERE date_vente >= '%6' AND date_vente <= '%7' AND"
                         " %8=0 AND %9=1").arg(fieldId,
                                               YerothDatabaseTableColumn::
                                               QUANTITE_VENDUE,
                                               YerothDatabaseTableColumn::
                                               MONTANT_TOTAL_VENTE,
                                               YerothDatabaseTableColumn::
                                               MONTANT_TVA,
                                               YerothDatabase::SERVICES_COMPLETES,
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_rapports_debut->date
                                                ()),
                                               DATE_TO_DB_FORMAT_STRING
                                               (dateEdit_rapports_fin->date
                                                ()),
                                               YerothDatabaseTableColumn::
                                               MONTANT_A_REMBOURSER,
                                               YerothDatabaseTableColumn::
                                               IS_SERVICE));
    }


    strQuery.append(QString(" AND %1 = '%2'").arg
                    (YerothDatabaseTableColumn::LOCALISATION,
                     _current_succursale_STRING));


    QSqlQuery query;

    unsigned int SIZE_FOR_CSV_FILE_CONTENT =
                    (size > QUANTITE_9) ? QUANTITE_9 : size;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

//    qDebug() << "++ strQuery: " << strQuery;
//
//    qDebug() << "++ querySize: " << querySize;

    QString fieldIdValue;
    double montant_total_vente = 0.0;
    double total = 0.0;

    double quantite_vendue = 0.0;

    QList < Yeroth_MAP_COMPLEX_Item * >nomEntrepriseFournisseurToVentes;

    if (querySize > 0)
    {
        while (query.next())
        {
            fieldIdValue = query.value(0).toString();
            quantite_vendue = query.value(1).toDouble();
            montant_total_vente = query.value(2).toDouble();

            int idx = -1;
            for (int i = 0; i < nomEntrepriseFournisseurToVentes.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive
                        (nomEntrepriseFournisseurToVentes.value(i)->_itemName,
                         fieldIdValue))
                {
                    idx = i;
                    break;
                }
            }

            if (-1 == idx)
            {
                Yeroth_MAP_COMPLEX_Item *aNewItem =
                                new Yeroth_MAP_COMPLEX_Item(fieldIdValue,
                                                            montant_total_vente,
                                                            quantite_vendue);
                nomEntrepriseFournisseurToVentes.push_back(aNewItem);
            }
            else
            {
                Yeroth_MAP_COMPLEX_Item *anItem =
                                nomEntrepriseFournisseurToVentes.value(idx);
                anItem->_itemValue += montant_total_vente;
                anItem->_itemSecondValue += quantite_vendue;
            }

            total += montant_total_vente;
        }
    }

    qSort(nomEntrepriseFournisseurToVentes.begin(),
          nomEntrepriseFournisseurToVentes.end(),
          Yeroth_MAP_COMPLEX_Item::lessThan);

    //Remove all items with a zero value
    for (int j = 0; j < nomEntrepriseFournisseurToVentes.size(); ++j)
    {
        if (0 == nomEntrepriseFournisseurToVentes.at(j)->_itemValue)
        {
            nomEntrepriseFournisseurToVentes.removeAt(j);
        }
    }

    QString csvFileContent;
    QString label;
    QString curValueStr;
    QString curSecondValueStr;
    QString pourcentageStr;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append
    (YerothUtils::LATEX_IN_OUT_handleForeignAccents
     (QObject::tr("D\\'etails en %1:").arg
      (YerothERPConfig::currency))).prepend("\\textbf{").append("}\n");

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    csvFileContent.append("Nom, Total\n");

    double pourcentage = 0;
    double curValue = 0;

    for (int j = nomEntrepriseFournisseurToVentes.size() - 1, k = 0;
            j > -1 && k < size; --j, ++k)
    {
        curValue = nomEntrepriseFournisseurToVentes.value(j)->_itemValue;

        curValueStr = GET_CURRENCY_STRING_NUM(curValue);

        curSecondValueStr =
                        GET_DOUBLE_STRING(nomEntrepriseFournisseurToVentes.value(j)->
                                          _itemSecondValue);

        pourcentage = (curValue / total) * 100.0;
        pourcentageStr = QString::number(pourcentage, 'f', 3);

        /*qDebug() << QString("++ value: %1, name %2, total: %3, pourcentage: %4")
           .arg(QString::number(caissierToVentes.value(j)->_itemValue, 'f', 9),
           caissierToVentes.value(j)->_itemName,
           QString::number(total, 'f', 9),
           QString::number(pourcentage, 'f', 9)); */

        label =
                        QString("\"%1\"").arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents
                                              (nomEntrepriseFournisseurToVentes.
                                               at(j)->_itemName));

        if (k < SIZE_FOR_CSV_FILE_CONTENT)
        {
            csvFileContent.append(QString("%1, %2\n").
                                  arg(label, pourcentageStr));
            ++_csvFileItemSize;
        }

        _reportTexFileEndString.append(QObject::tr
                                       ("\\item %1: \\textbf{%2} (quantité vendue: %3)\n").
                                       arg(label,
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (curValueStr),
                                           YerothUtils::
                                           LATEX_IN_OUT_handleForeignAccents
                                           (curSecondValueStr)));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;

        pourcentage = 0;
    }

    _reportTexFileEndString.append("\\end{enumerate}");

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    //clean up
    for (int k = 0; k < nomEntrepriseFournisseurToVentes.size(); ++k)
    {
        delete nomEntrepriseFournisseurToVentes.value(k);
    }

    QFile csvFile(fileName);

    if (!csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        _logger->log("meilleursStats", csvFile.errorString());
    }
    else
    {
        QTextStream outCsvFile(&csvFile);

        outCsvFile << csvFileContent;

        //qDebug() << QString("\t%1: \n%2").arg(fileName, csvFileContent);

        csvFile.close();
    }
}
