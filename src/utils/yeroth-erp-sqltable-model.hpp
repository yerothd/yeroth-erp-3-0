/*
 * yeroth-erp-sqltable-model.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_SQLTABLE_MODEL_HPP_
#define YEROTH_SQLTABLE_MODEL_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QStringList>

#include <QtSql/QSqlRelationalTableModel>


class QMainWindow;
class YerothLogger;


enum YerothSqlQueryType
{
    OR = 0,
    AND = 1
};


class YerothSqlTableModel : public QSqlTableModel
{

public:

    YEROTH_CLASS_OPERATORS

	YerothSqlTableModel();

    YerothSqlTableModel(const QSqlQuery &aQSqlQuery);

    YerothSqlTableModel(const QString 	&sqlTableName,
    					bool 			setTableName = true);

    ~YerothSqlTableModel();

    inline bool isFromQSqlQuery()
    {
        return _fromQSqlQuery;
    }

    virtual int yeroth_RESET_specify_filter_FROM_SELECT_STATEMENT(const QString &filter);

    virtual int yeroth_specify_filter_FROM_SELECT_STATEMENT(const QString &filter);

    virtual inline void yerothSetFilter_WITH_where_clause(const QString &filter)
    {
        QSqlTableModel::setFilter(filter.trimmed());
    }

    inline void changeEditStrategy(QSqlTableModel::EditStrategy strategy)
    {
        setEditStrategy(strategy);
    }

    int Is_SearchQSqlTable(QString tableColumn,
    					   QString searchTerm);

    int _Is_SearchQSqlTable(enum 		YerothSqlQueryType,
                            QString 	tableColumn,
                            QStringList &searchTermsList);

    inline int OR_Is_SearchQSqlTable(QString 		tableColumn,
                                     QStringList 	&searchTermsList)
    {
        return _Is_SearchQSqlTable(OR, tableColumn, searchTermsList);
    }

    inline int AND_Is_SearchQSqlTable(QString tableColumn,
                                      QStringList &searchTermsList)
    {
        return _Is_SearchQSqlTable(AND, tableColumn, searchTermsList);
    }

    int Like_SearchQSqlTable(QString tableColumn, QString searchTerm);

    int _Like_SearchQSqlTable(enum 			YerothSqlQueryType,
                              QString 		tableColumn,
                              QStringList 	&searchTermsList);

    inline int OR_Like_SearchQSqlTable(QString 		tableColumn,
                                       QStringList 	&searchTermsList)
    {
        return _Like_SearchQSqlTable(OR, tableColumn, searchTermsList);
    }

    inline int AND_Like_SearchQSqlTable(QString 	tableColumn,
                                        QStringList &searchTermsList)
    {
        return _Like_SearchQSqlTable(AND, tableColumn, searchTermsList);
    }

    bool insertNewRecord(QSqlRecord  &record,
    					 QMainWindow *parent = 0,
    					 QString 	 cpp_file = "",
						 int	  	 cpp_LINE_nr = -1,
						 QString 	 an_INSERTED_VALUE = "");

//      bool updateRecord(QSqlRecord &record);

    bool updateRecord(unsigned	 row,
    				  QSqlRecord &record,
 					  QString 	 cpp_file = "",
					  int	  	 cpp_LINE_nr = -1,
					  QString 	 an_INSERTED_VALUE = "");

    /*inline bool removeRow(int row)
       {
       return  removeRow(row);
       } */

    void resetFilter(QString	cpp_file 	= "",
			  	  	 int	  	cpp_LINE_nr = -1);

    inline QString sqlTableName()
    {
        return _sqlTableName;
    }

    inline void yerothSetQuery(const QSqlQuery &query)
    {
        QSqlTableModel::setQuery(query);
    }

    int easySelect(QString 	 cpp_file 	 = "",
			  	   int	  	 cpp_LINE_nr = -1);

    bool yerothSetSort(int 				column,
    				   Qt::SortOrder 	order,
					   QString 	 		cpp_file = "",
					   int	  	 		cpp_LINE_nr = -1);

    int yerothSetQueryRowCount(const QString &STRING_aSqlQuery);

    bool yerothSetQuery(const QString &aSqlQuery);

    inline QString yerothSelectStatement()
    {
        return QSqlTableModel::selectStatement();
    }

private:

    QString _sqlTableName;

    YerothLogger *_logger;

    bool _fromQSqlQuery;
};

#endif /* YEROTH_SQLTABLE_MODEL_HPP_ */
