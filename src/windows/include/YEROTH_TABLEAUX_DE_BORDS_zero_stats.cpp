/*
 * YEROTH_TABLEAUX_DE_BORDS_zero_stats.cpp
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


/*void YerothTableauxDeBordWindow::ZERO_stats(QString fileName,
											QString fieldId)
{
    _logger->log("ZERO_stats");

    QString strQuery(QString("SELECT %1, quantite_totale "
    						 "FROM %2 s "
    						 "WHERE s.%3 NOT IN "
    						 	 	"(select %4 "
    						 	 	"from %5 sv "
    						 	 	"where sv.%6 = s.%7 "
    						 	 	"AND date_vente >= '%8' AND date_vente <= '%9')")
    					.arg(fieldId,
							 YerothDatabase::STOCKS,
							 fieldId,
							 fieldId,
							 YerothDatabase::STOCKS_VENDU,
							 fieldId,
							 fieldId,
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_debut->date()),
							 DATE_TO_DB_FORMAT_STRING(dateEdit_rapports_fin->date())));


	strQuery.append(QString(" AND %1 = '%2'")
						.arg(YerothDatabaseTableColumn::LOCALISATION,
							 _current_succursale_STRING));

//    QDEBUG_STRING_OUTPUT_2("YerothTableauxDeBordWindow::ZERO_stats, strQuery", strQuery);

    QSqlQuery query;

    int querySize = YerothUtils::execQuery(query, strQuery, _logger);

    double quantite_totale = 0.0;

    QString fieldIdValue;

    Yeroth_MAP_COMPLEX_Item *anItem = 0;

    QList<Yeroth_MAP_COMPLEX_Item *> allItems;

    if (querySize > 0)
    {
        while(query.next())
        {
        	fieldIdValue = query.value(0).toString();

        	quantite_totale = query.value(1).toDouble();

            int idx = -1;
            for(int i = 0; i < allItems.size(); ++i)
            {
                if (YerothUtils::isEqualCaseInsensitive(allItems.value(i)->_itemName, fieldIdValue))
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

            anItem->_itemValue += quantite_totale;
        }
    }

    qSort(allItems.begin(), allItems.end(), Yeroth_MAP_COMPLEX_Item::lessThan);

    QString csvFileContent;

    _reportTexFileEndString.clear();

    _reportTexFileEndString.append("\\begin{enumerate}[1.]\n");

    _csvFileItemSize = 0;

    double valeur_dinventaire_toute_marchandise = 0.0;

    QString label;
    QString value;

    for(int j = allItems.size() - 1, k = 0; j > -1; --j, ++k)
    {
        label = QString("\"%1\"")
        			.arg(YerothUtils::LATEX_IN_OUT_handleForeignAccents(allItems.at(j)->_itemName));

        value = YerothUtils::LATEX_IN_OUT_handleForeignAccents(
        			GET_DOUBLE_STRING(allItems.at(j)->_itemValue));

        csvFileContent.prepend(QString("%1; %2; %3\n")
        						.arg(label,
        							 value,
									 QString::number(0.0)));

        _reportTexFileEndString.append(QObject::tr("\\item %1 | "
        											   "Qté en stock: \"%2\"\n")
        									.arg(label,
        										 value));

        //qDebug() << "++ reportTexFileEndString: " << _reportTexFileEndString;
        ++_csvFileItemSize;
    }

    _reportTexFileEndString.append("\\end{enumerate}");

    //qDebug() << "++ test, _csvFileItemSize: " << caissierToVentes.size();

    csvFileContent.prepend(QObject::tr("Nom; Quantité totale en stock; Total chiffre d'affaire\n"));

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
}*/
