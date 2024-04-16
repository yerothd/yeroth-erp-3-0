/*
 * yeroth-erp-abstract-class-yerothsearch-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_
#define SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_


#include "src/widgets/yeroth-erp-combo-box.hpp"

#include "src/widgets/yeroth-erp-line-edit.hpp"


#include <QtCore/QObject>

#include <QtCore/QMap>

#include <QtCore/QString>


class YerothComboBox;
class YerothLineEdit;


class YerothAbstractClassYerothSearchWindow
{

public:

    YEROTH_CLASS_OPERATORS
    typedef
    void (YerothAbstractClassYerothSearchWindow::*fptr)(YerothSqlTableModel
                                                        &);

    inline YerothAbstractClassYerothSearchWindow(QString
                                                 aDBYerothSqlTableName):_currentlyFiltered
        (false), _searchFilter(YerothUtils::EMPTY_STRING),
        _dbYerothSqlTableName(aDBYerothSqlTableName), _yerothSqlTableModel(0),
        _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString(0),
        _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString(0)
    {
    }

    virtual ~YerothAbstractClassYerothSearchWindow();

public slots:
    virtual void viewYerothTableViewFirstPage(YerothSqlTableModel
                                              &
                                              curYerothSqlTableModel_IN)
        = 0;

    virtual void viewYerothTableViewLastPage(YerothSqlTableModel &
                                             curYerothSqlTableModel_IN) = 0;

    virtual void viewYerothTableViewPreviousPage(YerothSqlTableModel &
                                                 curYerothSqlTableModel_IN) =
                                                                 0;

    virtual void viewYerothTableViewNextPage(YerothSqlTableModel &
                                             curYerothSqlTableModel_IN) = 0;

    template < class aYerothMainWindowClassType >
    inline fptr Get__viewYerothTableViewFirstPage__FPointer(aYerothMainWindowClassType *
                                                            aThisRef)
    {
        return (fptr) & aYerothMainWindowClassType::viewYerothTableViewFirstPage;
    }

    template < class aYerothMainWindowClassType >
    inline fptr Get__viewYerothTableViewLastPage__FPointer(aYerothMainWindowClassType *
                                                           aThisRef)
    {
        return (fptr) & aYerothMainWindowClassType::viewYerothTableViewLastPage;
    }

    template < class aYerothMainWindowClassType >
    inline fptr Get__viewYerothTableViewPreviousPage__FPointer(aYerothMainWindowClassType
                                                               * aThisRef)
    {
        return (fptr) &
               aYerothMainWindowClassType::viewYerothTableViewPreviousPage;
    }

    template < class aYerothMainWindowClassType >
    inline fptr Get__viewYerothTableViewNextPage__FPointer(aYerothMainWindowClassType *
                                                           aThisRef)
    {
        return (fptr) & aYerothMainWindowClassType::viewYerothTableViewNextPage;
    }

protected slots:

	virtual inline void setupLineEditsQCompleters(QObject *aThis,
												  bool A_CHARGE_FINANCIERE_window = false)
	{
		setupLineEditsQCompleters(aThis, YerothUtils::EMPTY_STRING, A_CHARGE_FINANCIERE_window);
	}

    virtual void setupLineEditsQCompleters(QObject *aThis,
                                           QString aConditionStr,
										   bool A_CHARGE_FINANCIERE_window = false);

    virtual void textChangedSearchLineEditsQCompleters() = 0;

protected:

    virtual inline void setYerothSqlTableModel(YerothSqlTableModel *
                                               aYerothSqlTableModel)
    {
        _yerothSqlTableModel = aYerothSqlTableModel;
    }

    virtual inline void clearSearchFilter()
    {
        _searchFilter.clear();
    }

    virtual inline bool isCurrentlyFiltered()
    {
        return _currentlyFiltered;
    }

    virtual void setCurrentlyFiltered(bool currentlyFiltered);

    virtual void clearLineEditsQCompleters();

    virtual void resetLineEditsQCompleters(QObject *aThis);

    virtual inline void set_filtrer_font()
    {
    }

    bool _currentlyFiltered;

    QString _searchFilter;

    QString _dbYerothSqlTableName;

    YerothSqlTableModel *_yerothSqlTableModel;

    /**
     * for use with _yeroth_WINDOW_QComboBox_SearchDBFieldColumnString,
     * and _yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString
     */
    QSet < QString > _yeroth_WINDOW_references_dbColumnString;

    YerothComboBox *_yeroth_WINDOW_QComboBox_SearchDBFieldColumnString;

    YerothLineEdit *_yeroth_WINDOW_QLineEdit_SearchDBFieldColumnString;

    QMap < YerothComboBox **, QString > _comboBoxesToANDContentForSearch;

    QMap < YerothLineEdit **, QString > _lineEditsToANDContentForSearch;
};


#endif /* SRC_UTILS_WINDOWS_YEROTH_ERP_ABSTRACT_CLASS_YEROTHSEARCH_WINDOW_HPP_ */
