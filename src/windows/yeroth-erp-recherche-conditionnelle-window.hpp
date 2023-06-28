/*
 * yeroth-erp-recherche-conditionnelle-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_
#define SRC_YEROTH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_

#include "../../ui_yeroth-erp-recherche-conditionnelle-window.h"

#include "src/yeroth-erp-windows.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"



class YerothRechercheConditionnelleWindow : public YerothWindowsCommons,
    										public Ui_YerothRechercheConditionnelleWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothRechercheConditionnelleWindow();

    inline ~YerothRechercheConditionnelleWindow()
    {
        delete _logger;
    }

    inline virtual QToolBar *getQMainWindowToolBar()
    {
        return toolBar_recherche_conditionnelle;
    }


    void rendreVisible(QStringList 	&db_column_to_SEARCH_FILTER_APPEND_LIST,
                       QPalette		&aPalette);

    inline void rendreInvisible()
    {
        setVisible(false);
    }


public slots:

    virtual void updateYerothLineEditQCompleter_1(const QString &
                                                  currentDBColumnString);

    virtual void updateYerothLineEditQCompleter_2(const QString &
                                                  currentDBColumnString);

    virtual void updateYerothLineEditQCompleter_3(const QString &
                                                  currentDBColumnString);

    virtual void updateYerothLineEditQCompleter_4(const QString &
                                                  currentDBColumnString);

    void reset_filter();

private:

    void populateComboBoxes();

    void setupLineEdits();

public:

    static YerothWindowsCommons
    *_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;

protected:

    QStringList _db_column_to_SEARCH_FILTER_APPEND_LIST;


    YerothLogger *_logger;
};

#endif /* SRC_YEROTH_RECHERCHE_CONDITIONNELLE_WINDOW_HPP_ */
