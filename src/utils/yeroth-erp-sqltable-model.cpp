/*
 * yeroth-erp-sqltable-model.cpp
 *
  *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-sqltable-model.hpp"

#include "src/utils/yeroth-erp-database-table-column.hpp"

#include "src/utils/yeroth-erp-logger.hpp"


#ifdef YEROTH_SERVER

	#include "src/yeroth-erp-windows.hpp"

	#include "src/dbus/YEROTH_RUNTIME_VERIFIER.hpp"

#endif


#include <QtCore/QDebug>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlError>

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QMessageBox>


YerothSqlTableModel::YerothSqlTableModel():_logger(0), _fromQSqlQuery(false)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerothSqlTableModel::YerothSqlTableModel(const QSqlQuery &aQSqlQuery):
    _logger(0),
    _fromQSqlQuery(true)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    yerothSetQuery(aQSqlQuery);

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerothSqlTableModel::YerothSqlTableModel(const QString &sqlTableName,
                                         bool setTableName /* = true */)
    :
    _sqlTableName(sqlTableName),
    _logger(0),
    _fromQSqlQuery(false)
{
    _logger = new YerothLogger("YerothSqlTableModel");

    _logger->log("YerothSqlTableModel",
                 QString
                 ("table name: %1, edit strategy: QSqlTableModel::OnFieldChange").
                 arg(sqlTableName));

    if (setTableName)
    {
        setTable(sqlTableName);
    }

    setEditStrategy(QSqlTableModel::OnFieldChange);
}


YerothSqlTableModel::~YerothSqlTableModel()
{
    YEROTH_DELETE_FREE_POINTER_NOW(_logger);
}


int YerothSqlTableModel::yeroth_RESET_specify_filter_FROM_SELECT_STATEMENT(const
                                                                           QString
                                                                           &
                                                                           filter)
{
    QString curYerothSelectStatement(yerothSelectStatement());

    curYerothSelectStatement.remove(filter);

//      QDEBUG_STRING_OUTPUT_2("RESET filter", curYerothSelectStatement);

    int queryResultSize = yerothSetQueryRowCount(curYerothSelectStatement);

    return queryResultSize;
}


int YerothSqlTableModel::yeroth_specify_filter_FROM_SELECT_STATEMENT(const
                                                                     QString &
                                                                     filter)
{
    QString curYerothSelectStatement(yerothSelectStatement());

    curYerothSelectStatement.append(QString(" %1").arg(filter));

//      QDEBUG_STRING_OUTPUT_2("filter", curYerothSelectStatement);

    int queryResultSize = yerothSetQueryRowCount(curYerothSelectStatement);

    return queryResultSize;
}


int YerothSqlTableModel::Is_SearchQSqlTable(QString tableColumn,
                                            QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_IS_STMT(tableColumn, searchTerm));

    yerothSetFilter_WITH_where_clause(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Is_SearchQSqlTable",
                 QString
                 ("(table: %1, column: %2) | queryFilter (%3) | row count: %4").
                 arg(_sqlTableName, tableColumn, queryFilter,
                     QString::number(filterRowCount)));

    resetFilter("src/utils/yeroth-erp-sqltable-model.cpp", 135);

    return filterRowCount;
}


int YerothSqlTableModel::_Is_SearchQSqlTable(enum YerothSqlQueryType,
                                             QString tableColumn,
                                             QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_IS_STMT
                             (tableColumn,
                              searchTermsList.at(k))).append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_IS_STMT
                             (tableColumn, searchTermsList.at(k)));
        }
    }

    yerothSetFilter_WITH_where_clause(queryTerm);

    int filterRowCount = rowCount();

    resetFilter("src/utils/yeroth-erp-sqltable-model.cpp");

    return filterRowCount;
}

int YerothSqlTableModel::Like_SearchQSqlTable(QString tableColumn,
                                              QString searchTerm)
{
    QString queryFilter(GENERATE_SQL_LIKE_STMT(tableColumn, searchTerm));

    yerothSetFilter_WITH_where_clause(queryFilter);

    int filterRowCount = rowCount();

    _logger->log("Like_SearchQSqlTable",
                 QString
                 ("(table: %1, column: %2) | queryFilter (%3) | row count: %4").
                 arg(_sqlTableName).arg(tableColumn).arg(queryFilter).
                 arg(filterRowCount));

    /*qDebug() <<
       QString("YerothSqlTableModel | Like_SearchQSqlTable (table: %1, column: %2) \n\t\t| queryFilter (%3) | row count: %4")
       .arg(_sqlTableName)
       .arg(tableColumn)
       .arg(queryFilter)
       .arg(filterRowCount); */

    resetFilter("src/utils/yeroth-erp-sqltable-model.cpp");

    return filterRowCount;
}

int YerothSqlTableModel::_Like_SearchQSqlTable(enum YerothSqlQueryType,
                                               QString tableColumn,
                                               QStringList &searchTermsList)
{
    QString queryTerm;

    const int size = searchTermsList.size();
    const int max = size - 1;

    for (int k = 0; k < size; ++k)
    {
        if (k < max)
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT
                             (tableColumn,
                              searchTermsList.at(k))).append(" AND ");
        }
        else
        {
            queryTerm.append(GENERATE_SQL_LIKE_STMT
                             (tableColumn, searchTermsList.at(k)));
        }
    }

    yerothSetFilter_WITH_where_clause(queryTerm);

    int filterRowCount = rowCount();

    resetFilter("src/utils/yeroth-erp-sqltable-model.cpp");

    return filterRowCount;
}

bool YerothSqlTableModel::insertNewRecord(QSqlRecord  &record,
                                          QMainWindow *parent /* = 0 */,
					    				  QString 	  cpp_file /* = "" */,
										  int	 	  cpp_LINE_nr /* = -1 */,
										  QString 	  an_INSERTED_VALUE /* = YerothUtils::EMPTY_STRING */)
{
    _logger->log("insertNewRecord",
                 QString("Table name: %1")
				 	 .arg(sqlTableName()));

    resetFilter("src/utils/yeroth-erp-sqltable-model.cpp", 241);

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    bool success = insertRecord(-1, record);

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    //qDebug() << QString("YerothSqlTableModel::insertNewRecord, success: %1")
    //                          .arg(BOOL_TO_STRING(success));

    if (success)
    {
#ifdef YEROTH_SERVER

    	YEROTH_RUNTIME_VERIFIER *dbusServer = YerothUtils::getAllWindows()->dbusServer();

    	dbusServer->YR_slot_refresh_INSERT_DB_MYSQL__CALLED
						(QString("%1.%2;%3;%4")
							.arg(_sqlTableName,
								 an_INSERTED_VALUE,
    							 cpp_file,
    							 QString::number(cpp_LINE_nr)));


#endif //YEROTH_SERVER
    }
    else
    {
        QString retMsg =
        	QObject::tr("Une erreur (base de données) est survenue pendant l'insertion dans le tableau %1 !\n")
                   .arg(sqlTableName());

        retMsg.append(QObject::tr("Contactez l'administrateur de Yeroth-pgi-3.0 !"));

        if (0 != parent)
        {
            QMessageBox::warning(parent,
                                 QObject::tr("sqltablemodel - insertNewRecord"), retMsg);
        }

        qDebug() << QString("%1 - reason for failing: %2")
						.arg(sqlTableName(),
                             lastError().text());

        _logger->log("insertNewRecord",
                     QString("reason for failing: %1")
					 	 .arg(lastError().text()));
    }

    return success;
}


//bool YerothSqlTableModel::Record(QSqlRecord &record)
//{
//    _logger->log("updateRecord",
//                 QString("Table name: %1").arg(sqlTableName()));
//
//    bool success = false;
//
//    QString aCurFieldValue;
//
//    QString dbFieldIDValue;
//
//    QString aCurFieldName;
//
//    QString SQL_UPDATE_STRING_COMMAND = QString("UPDATE %1 SET ")
//                                                                              .arg(_sqlTableName);
//
//    for (int k = 0; k < record.count(); ++k)
//    {
//      aCurFieldName.clear();
//
//      aCurFieldName.append(record.fieldName(k));
//
//      if (!YerothUtils::isEqualCaseInsensitive(YerothDatabaseTableColumn::ID, aCurFieldName))
//      {
//              if (!aCurFieldName.contains("date_"))
//              {
//                      aCurFieldValue = YerothUtils::get_text(record.value(aCurFieldName));
//
//              SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
//                                                                                      .arg(aCurFieldName,
//                                                                                               aCurFieldValue));
//              }
//              else
//              {
//                      aCurFieldValue = DATE_TO_DB_FORMAT_STRING(record.value(aCurFieldName).toDate());
//
////                            QDEBUG_STRING_OUTPUT_2("aCurFieldValue (DATE_TO_DB_FORMAT_STRING)", aCurFieldValue);
//
//                      if (!aCurFieldValue.isEmpty())
//                      {
//                              SQL_UPDATE_STRING_COMMAND.append(QString("%1='%2', ")
//                                                                                                      .arg(aCurFieldName,
//                                                                                                               aCurFieldValue));
//                      }
//              }
//      }
//      else
//      {
//              dbFieldIDValue = YerothUtils::get_text(record.value(aCurFieldName));
//      }
//    }
//
//    int len = SQL_UPDATE_STRING_COMMAND.length();
//
//    SQL_UPDATE_STRING_COMMAND.remove(len - 2, 2);
//
//      SQL_UPDATE_STRING_COMMAND.append(QString(" WHERE %1 = '%2';")
//                                                                              .arg(YerothDatabaseTableColumn::ID,
//                                                                                       dbFieldIDValue));
//
////    QDEBUG_STRING_OUTPUT_2("SQL_UPDATE_STRING_COMMAND", SQL_UPDATE_STRING_COMMAND);
//
//    success = YerothUtils::execQuery(SQL_UPDATE_STRING_COMMAND);
//
//    return success;
//}


bool YerothSqlTableModel::updateRecord(unsigned    row,
									   QSqlRecord &record,
					 				   QString 	   cpp_file /* = "" */,
									   int	  	   cpp_LINE_nr /* = -1 */,
									   QString 	   an_INSERTED_VALUE /* = "" */)
{
    _logger->log("updateRecord",
                 QString("Table name: %1")
				 	 .arg(sqlTableName()));

    YEROTH_ERP_3_0_START_DATABASE_TRANSACTION;

    bool success = setRecord(row, record);

    YEROTH_ERP_3_0_COMMIT_DATABASE_TRANSACTION;

    if (!success)
    {
        _logger->log("updateRecord",
                     QString("updating row %1. reason for failing: %2")
					 	 .arg(QString::number(row),
					 		  lastError().text()));
    }
    else
    {

    #ifdef YEROTH_SERVER

    	YEROTH_RUNTIME_VERIFIER *dbusServer = YerothUtils::getAllWindows()->dbusServer();

    	dbusServer->YR_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString("%1.%2;%3;%4")
    															.arg(_sqlTableName,
    																 an_INSERTED_VALUE,
																	 cpp_file,
																	 QString::number(cpp_LINE_nr)));

    #endif //YEROTH_SERVER

        _logger->log("updateRecord",
                     QString("updated row %1 with success")
					 	 .arg(row));
    }

    return success;
}


void YerothSqlTableModel::resetFilter(QString	cpp_file 	/* = "" */,
 	  	 	 	 	 	 	 	 	  int	  	cpp_LINE_nr /* = -1 */)
{
    yerothSetFilter_WITH_where_clause(YerothUtils::EMPTY_STRING);

    bool ret_SELECT_VALUE = select();

#ifdef YEROTH_SERVER

    if (true == ret_SELECT_VALUE)
    {
    	YEROTH_RUNTIME_VERIFIER *dbusServer = YerothUtils::getAllWindows()->dbusServer();

    	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
																.arg(_sqlTableName,
																     cpp_file,
																	 QString::number(cpp_LINE_nr)));
    }

#endif //YEROTH_SERVER

}


int YerothSqlTableModel::easySelect(QString cpp_file 	 /* = "" */,
	  	  	  	  	  	  	  	    int	  	cpp_LINE_nr /* = -1 */)
{
    bool success = select();

    if (!success)
    {
        return -1;
    }

#ifdef YEROTH_SERVER

    YEROTH_RUNTIME_VERIFIER *dbusServer = YerothUtils::getAllWindows()->dbusServer();

	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
															.arg(_sqlTableName,
															     cpp_file,
																 QString::number(cpp_LINE_nr)));

#endif //YEROTH_SERVER


    return rowCount();
}


bool YerothSqlTableModel::yerothSetSort(int 			column,
										Qt::SortOrder 	order,
										QString 		cpp_file /* = "" */,
										int	  	 		cpp_LINE_nr /* = -1 */)
{
    setSort(column, order);

    bool ret_SELECT_VALUE = select();

#ifdef YEROTH_SERVER

    if (true == ret_SELECT_VALUE)
    {
    	YEROTH_RUNTIME_VERIFIER *dbusServer = YerothUtils::getAllWindows()->dbusServer();

    	dbusServer->YR_slot_refresh_SELECT_DB_MYSQL__CALLED(QString("%1;%2;%3")
    															.arg(_sqlTableName,
    															     cpp_file,
    																 QString::number(cpp_LINE_nr)));
    }

#endif //YEROTH_SERVER

    return ret_SELECT_VALUE;
}

int YerothSqlTableModel::yerothSetQueryRowCount(const QString &STRING_aSqlQuery)
{
    QSqlQueryModel::setQuery(STRING_aSqlQuery);

    if (lastError().isValid())
    {
        qDebug() << "++ YerothSqlTableModel::yerothSetQuery(QString): \n\t"
                 << STRING_aSqlQuery << "\n" << lastError();

        return -1;
    }

    int queryResultRowCount = QSqlQueryModel::rowCount();

    return queryResultRowCount;
}


bool YerothSqlTableModel::yerothSetQuery(const QString &aSqlQuery)
{
    QSqlQueryModel::setQuery(aSqlQuery);

    if (lastError().isValid())
    {
        qDebug() << "++ YerothSqlTableModel::yerothSetQuery(QString): \n\t"
                 << aSqlQuery << "\n" << lastError();

        return false;
    }

    return true;
}
