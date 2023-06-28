/*
 * yeroth-erp-recherche-conditionnelle-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-recherche-conditionnelle-window.hpp"


YerothWindowsCommons *
YerothRechercheConditionnelleWindow::
_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;


YerothRechercheConditionnelleWindow::YerothRechercheConditionnelleWindow()
:YerothWindowsCommons("yeroth-erp-recherche-conditionnelle-stock"),
 _logger(new YerothLogger("YerothRechercheConditionnelleWindow"))
{
	_windowName = YerothMainWindow::get_TRANSLATED_WindowName("recherche conditionnelle");

    _windowName_WITH_NO_MAINTENANCE = _windowName;

    setupUi(this);

    mySetupUi(this);

    QMESSAGE_BOX_STYLE_SHEET =
                    QString("QMessageBox {background-color: rgb(%1);}"
                            "QMessageBox QLabel {color: rgb(%2);}").arg
                    (COLOUR_RGB_STRING_YEROTH_ORANGE_243_162_0,
                     COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);

    pushButton_CONDITIONAL_SEARCH_RESET_filter->enable(this,
                                                       SLOT(reset_filter()));
}


void YerothRechercheConditionnelleWindow::rendreVisible(QStringList &db_column_to_SEARCH_FILTER_APPEND_LIST,
                                                        QPalette 	&aPalette)
{
    toolBar_recherche_conditionnelle->setPalette(aPalette);

    _db_column_to_SEARCH_FILTER_APPEND_LIST =
                    db_column_to_SEARCH_FILTER_APPEND_LIST;


    populateComboBoxes();

    setupLineEdits();

    YEROTH_set_windowName_TRANSLATED(YerothMainWindow::get_TRANSLATED_WindowName("recherche conditionnelle"),
      								 _curStocksTableModel);

    setVisible(true);
}


void YerothRechercheConditionnelleWindow::updateYerothLineEditQCompleter_1(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yeroth_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_1->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_1->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerothRechercheConditionnelleWindow::updateYerothLineEditQCompleter_2(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yeroth_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_2->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_2->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerothRechercheConditionnelleWindow::updateYerothLineEditQCompleter_3(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yeroth_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_3->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_3->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerothRechercheConditionnelleWindow::updateYerothLineEditQCompleter_4(const
                                                                           QString
                                                                           &
                                                                           currentDBColumnString)
{
    QString correspondingDBFieldKeyValue =
                    YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN(currentDBColumnString);

    if (correspondingDBFieldKeyValue.isEmpty())
    {
        return;
    }

    if (!correspondingDBFieldKeyValue.isEmpty())
    {
        if (!_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_yeroth_references_dbColumnString().contains(correspondingDBFieldKeyValue))
        {
            lineEdit_nom_element_string_db_4->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, true);
        }
        else
        {
            lineEdit_nom_element_string_db_4->setupMyStaticQCompleter
            (_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_dbYerothSqlTableName_WINDOWS_TABLE_VIEW_FOR_SEARCH
             (), correspondingDBFieldKeyValue, false);
        }
    }
}


void YerothRechercheConditionnelleWindow::reset_filter()
{
    lineEdit_nom_element_string_db_1->myClear();
    lineEdit_nom_element_string_db_2->myClear();
    lineEdit_nom_element_string_db_3->myClear();
    lineEdit_nom_element_string_db_4->myClear();

    comboBox_element_string_db_1->resetYerothComboBox();
    comboBox_element_string_db_2->resetYerothComboBox();
    comboBox_element_string_db_3->resetYerothComboBox();
    comboBox_element_string_db_4->resetYerothComboBox();
}


void YerothRechercheConditionnelleWindow::populateComboBoxes()
{
    if (0 ==
            YerothRechercheConditionnelleWindow::
            _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING)
    {
        return;
    }

    reset_filter();

    comboBox_element_string_db_1->clear();
    comboBox_element_string_db_2->clear();
    comboBox_element_string_db_3->clear();
    comboBox_element_string_db_4->clear();

//      qDebug() << YerothRechercheConditionnelleWindow::_current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING;

    QObject::disconnect(lineEdit_nom_element_string_db_1, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_2, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_3, 0, 0, 0);
    QObject::disconnect(lineEdit_nom_element_string_db_4, 0, 0, 0);

    QObject::connect(lineEdit_nom_element_string_db_1,
                     SIGNAL(textChanged(const QString &)),
                     YerothRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_2,
                     SIGNAL(textChanged(const QString &)),
                     YerothRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_3,
                     SIGNAL(textChanged(const QString &)),
                     YerothRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(lineEdit_nom_element_string_db_4,
                     SIGNAL(textChanged(const QString &)),
                     YerothRechercheConditionnelleWindow::
                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING,
                     SLOT(textChangedSearchLineEditsQCompleters()));

    QObject::connect(comboBox_element_string_db_1,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerothLineEditQCompleter_1
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_2,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerothLineEditQCompleter_2
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_3,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerothLineEditQCompleter_3
                          (const QString &)));

    QObject::connect(comboBox_element_string_db_4,
                     SIGNAL(currentTextChanged(const QString &)),
                     this,
                     SLOT(updateYerothLineEditQCompleter_4
                          (const QString &)));


    YerothUtils::REMOVE_QSTRING_LIST(_db_column_to_SEARCH_FILTER_APPEND_LIST,
                                     YerothRechercheConditionnelleWindow::
                                     _current_window_TABLE_TO_SEARCH_WITH_CONDITION_FILTERING->get_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME
                                     ());


    _db_column_to_SEARCH_FILTER_APPEND_LIST.
    removeAll(YerothUtils::EMPTY_STRING);


    comboBox_element_string_db_1->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerothDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_1->currentText()));

    comboBox_element_string_db_2->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerothDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_2->currentText()));

    comboBox_element_string_db_3->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerothDatabaseTableColumn::_tableColumnToUserViewString);

    _db_column_to_SEARCH_FILTER_APPEND_LIST.removeOne
    (YEROTH_USER_VIEW_STRING_TO_DATABASE_TABLE_COLUMN
     (comboBox_element_string_db_3->currentText()));

    comboBox_element_string_db_4->addItems
    (_db_column_to_SEARCH_FILTER_APPEND_LIST,
     YerothDatabaseTableColumn::_tableColumnToUserViewString);


    comboBox_element_string_db_1->setCurrentIndex(0);
    comboBox_element_string_db_2->setCurrentIndex(0);
    comboBox_element_string_db_3->setCurrentIndex(0);
    comboBox_element_string_db_4->setCurrentIndex(0);
}


void YerothRechercheConditionnelleWindow::setupLineEdits()
{
    lineEdit_nom_element_string_db_1->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_2->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_3->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
    lineEdit_nom_element_string_db_4->enableForSearch(QObject::tr
                                                      ("valeur à rechercher"));
}
