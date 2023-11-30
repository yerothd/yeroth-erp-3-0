/*
 * yeroth-erp-line-edit.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_
#define SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"

#include "src/widgets/table-view/yeroth-erp-table-view.hpp"


#include <QtCore/QStringList>

#include <QtWidgets/QLineEdit>


class QCompleter;
class QInputDialog;

class YerothUtils;
class YerothSqlTableModel;
class YerothTableView;


class YerothLineEdit : public QLineEdit
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothLineEdit(QWidget *parent = 0);

    virtual ~YerothLineEdit();

    inline bool isEmpty()
    {
        return text().isEmpty();
    }

    void enableForSearch(QString placeHoderText);

    void disableForSearch();

    bool checkField(const QPalette *aPalette = 0);

    bool checkFieldAndMinimalValue(double min, QPalette *aPalette = 0);

    inline void setRed()
    {
        setPalette(YerothUtils::YEROTH_RED_PALETTE);
    }

    void setYerothERPQLineEditDisplayFormat();

    void clearQCompleter();

    void clearField();

    void setYerothEnabled(bool enabled);

    inline QString text_LATEX(int truncate_pos = -1) const
    {
        return YerothUtils::LATEX_IN_OUT_handleForeignAccents(text(truncate_pos));
    }

    inline void setText(const QString &text)
    {
        QLineEdit::setText(QString(text.trimmed())
        					 .remove(YerothUtils::STAR_CHAR));
    }

    QString text(int truncate_pos = -1) const;

    inline QString getLastQCompleterText()
    {
        return _lastQCompleterText;
    }

    void myClear();

    inline bool set_show_ASSET()
    {
        return _show_ASSET;
    }

    inline void set_show_ASSET(bool a_show_ASSET)
    {
        _show_ASSET = a_show_ASSET;
    }

    void setupMyQCompleter(QString 				sqlTableName,
                           int 					listColumnIndex,
                           enum Qt::SortOrder 	sortOrder,
                           bool 				truncateString = true);

    void setupMyStaticQCompleter(QString 	sqlTableName,
                                 QString 	fieldName,
                                 bool 		returnPressedSignalActivated = true,
                                 QString 	aQSqlConditionStr = YerothUtils::EMPTY_STRING);

    inline void setupMyStaticQCompleter(QString sqlTableName,
                                        QString fieldName,
                                        QString aQSqlConditionStr)
    {
        setupMyStaticQCompleter(sqlTableName,
                                fieldName,
								true,
								aQSqlConditionStr);
    }

    void setupMyQCompleterALL(QString sqlTableName);

    void setupMyQCompleterFEFO(QString sqlTableName);

    void setupMyQCompleterFIFO(QString sqlTableName);

    void setupMyQCompleterLIFO(QString sqlTableName);

    void setupMyQCompleterCodebarALL(QString sqlTableName);

    void setupMyQCompleterCodebarFEFO(QString sqlTableName);

    void setupMyQCompleterCodebarFIFO(QString sqlTableName);

    void setupMyQCompleterCodebarLIFO(QString sqlTableName);


    inline QMap<QString, QString> &getStockNameToStockID()
    {
        return _stockNameToStockID;
    }

    inline QMap<QString, QString> &getStockreferenceCodebarToStockID()
    {
        return _stockReferenceToStockID;
    }


    void refreshCodebarCompleterList(QString strategy);

    void refreshCompleterList(QString strategy);

    inline QCompleter *getMyQCompleter()
    {
        return _searchQCompleter;
    }

    inline YerothSqlTableModel *getMySqlTableModel()
    {
        return _sqlTableModel;
    }

    inline YerothTableView *getMyTableView()
    {
        return _sqlTableModelView;
    }

    inline QStringList getCurrentStaticStringList()
    {
        return _currentStaticStringList;
    }

protected:

    virtual void hideEvent(QHideEvent *hideEvent);

    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

public slots:

    void clearQCompleterText();

    void isInputValid();

private:

    bool _show_ASSET;

    bool _firstTimeStyleSheetCheck;

    bool _forSearch;

    bool _wasMissingRequiredText;

    QInputDialog *_inputDialog;

    QString _aDefaultStyleSheet;

    QString _originalPlaceHolderText;

    QPalette _originalPaletteBeforeMissingInformation;

    YerothSqlTableModel *_sqlTableModel;

    YerothTableView *_sqlTableModelView;

    QCompleter *_searchQCompleter;

    QMap<QString, QString> _stockNameToStockID;

    QMap<QString, QString> _stockReferenceToStockID;

    QStringList _currentStaticStringList;

    QString _lastQCompleterText;
};

#endif /* SRC_WIDGETS_YEROTH_LINE_EDIT_HPP_ */
