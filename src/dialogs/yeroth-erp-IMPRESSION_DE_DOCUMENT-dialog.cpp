/*
 * yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-IMPRESSION_DE_DOCUMENT-dialog.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/print_latex_pdf/yeroth-erp-print_yeroth_latex_util_pdf.hpp"


YerothWindowsCommons *YerothIMPRESSION_DE_DOCUMENT_Dialog::_current_window_to_print;

YerothTableViewPRINT_UTILITIES_TEX_TABLE *YerothIMPRESSION_DE_DOCUMENT_Dialog::
	_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE;


const QString YerothIMPRESSION_DE_DOCUMENT_Dialog::
	DIALOG_WINDOW_TITLE(QString("%1 - %2")
						 .arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
							  QObject::tr("IMPRESSION DE DOCUMENT")));


YerothIMPRESSION_DE_DOCUMENT_Dialog::YerothIMPRESSION_DE_DOCUMENT_Dialog()
:YerothPOSDialogCommons(YerothIMPRESSION_DE_DOCUMENT_Dialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint)
{
    setupUi(this);

    radioButton_IMPRESSION_HORIZONTAL->setChecked(true);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerothUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this,
								   *_currentPosition);

    setupLineEdits();

    setFixedSize(width(), height());

    connect(radioButton_IMPRESSION_VERTICAL,
            SIGNAL(toggled(bool)),
            this,
            SLOT(SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()));

    connect(radioButton_IMPRESSION_HORIZONTAL,
            SIGNAL(toggled(bool)),
            this,
            SLOT(SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()));


    connect(lineEdit_pageFROM,
            SIGNAL(textChanged(const QString &)),
            this,
			SLOT(enable_MAX_TABLE_ROW_COUNT()));

    connect(lineEdit_pageTO,
            SIGNAL(textChanged(const QString &)),
            this,
			SLOT(enable_MAX_TABLE_ROW_COUNT()));


    connect(pushButton_annuler, SIGNAL(clicked()), this, SLOT(annuler()));

    connect(pushButton_valider, SIGNAL(clicked()), this, SLOT(valider()));
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::setupLineEdits()
{
    lineEdit_IMPRESSION_LIGNES->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);
    lineEdit_IMPRESSION_LIGNES->setYerothEnabled(false);

    lineEdit_pageFROM->setValidator(&YerothUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);
    lineEdit_pageTO->setValidator(&YerothUtils::UINT_VALIDATOR_GREATER_THAN_ZERO);
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::show()
{
	retranslateUi(this);

    if (0 != _current_window_to_print)
    {
        if (YerothUtils::isEqualCaseInsensitive(_current_window_to_print->get_PRINTING_PARAMETER_printing_position(),
                 	 	 	 	 	 	 	    YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_PORTRAIT_SPECIFICATION))
        {
            radioButton_IMPRESSION_VERTICAL->setChecked(true);
        }
        else if (YerothUtils::isEqualCaseInsensitive(_current_window_to_print->get_PRINTING_PARAMETER_printing_position(),
                  YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_LANDSCAPE_SPECIFICATION))
        {
            radioButton_IMPRESSION_HORIZONTAL->setChecked(true);
        }

        if (-1 != _current_window_to_print->get_PRINTING_PARAMETER_pageFROM())
        {
            lineEdit_pageFROM->setText(QString::number(_current_window_to_print->get_PRINTING_PARAMETER_pageFROM()));
        }
        else
        {
            lineEdit_pageFROM->setText(YerothUtils::EMPTY_STRING);
        }

        if (-1 != _current_window_to_print->get_PRINTING_PARAMETER_pageTO())
        {
            lineEdit_pageTO->setText(QString::number(_current_window_to_print->get_PRINTING_PARAMETER_pageTO()));
        }
        else
        {
            lineEdit_pageTO->setText(YerothUtils::EMPTY_STRING);
        }
    }

    SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

    move(*_currentPosition);

    YerothPOSDialogCommons::show();
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::SET_NOMBRE_DE_LIGNES_PAR_DEFAUT_after_RADIOBUTTON_TOGGLE()
{
    if (radioButton_IMPRESSION_VERTICAL->isChecked())
    {
        if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
        {
            _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_VERTICAL_PRINT_POSITION();
        }
    }
    else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
    {
        if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
        {
            _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE->SET_HORIZONTAL_PRINT_POSITION();
        }
    }

    if (lineEdit_IMPRESSION_LIGNES->isReadOnly())
    {
        if (radioButton_IMPRESSION_VERTICAL->isChecked())
        {
            lineEdit_IMPRESSION_LIGNES->setText
				(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
        }
        else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
        {
            lineEdit_IMPRESSION_LIGNES->setText
				(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
        }
    }
    else
    {
        SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();
    }
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::SET_NOMBRE_DE_LIGNES_PAR_DEFAUT()
{
    if (lineEdit_IMPRESSION_LIGNES->isReadOnly())
    {
        if (radioButton_IMPRESSION_VERTICAL->isChecked())
        {
            lineEdit_IMPRESSION_LIGNES->setText
				(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
        }
        else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
        {
            lineEdit_IMPRESSION_LIGNES->setText
				(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
        }
    }
    else
    {
        if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
        {
            if (0 != _current_window_to_print)
            {
                lineEdit_IMPRESSION_LIGNES
					->setText(QString::number(
								_current_window_to_print
									->get_PRINTING_PARAMETER_print_table_row_count()));
            }
            else
            {
                lineEdit_IMPRESSION_LIGNES
					->setText(QString::number(
								_yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE
									->GET_NOMBRE_DE_LIGNES()));
            }
        }
        else
        {
            if (radioButton_IMPRESSION_VERTICAL->isChecked())
            {
                lineEdit_IMPRESSION_LIGNES
					->setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4PORTRAIT));
            }
            else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
            {
                lineEdit_IMPRESSION_LIGNES
					->setText(QString::number(YerothTableViewPRINT_UTILITIES_TEX_TABLE::_STANDARD_INITIAL_MAX_TABLE_ROW_COUNT_A4LANDSCAPE));
            }
        }
    }
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::enable_MAX_TABLE_ROW_COUNT()
{
    QString string_pageFROM = lineEdit_pageFROM->text();

    QString string_pageTO = lineEdit_pageTO->text();

    uint pageFROM = string_pageFROM.toUInt();

    uint pageTO = string_pageTO.toUInt();

    if (!string_pageFROM.isEmpty() &&
    	!string_pageTO.isEmpty())
    {
        if (pageFROM > pageTO)
        {
            YerothQMessageBox::warning(_current_window_to_print,
                                       QObject::tr("IMPRESSION, NUMÉRO DE PAGES"),
                                       QObject::tr("La numéro de la page de départ doit "
                                    		   	   "être inférieur au numéro de la page terminale"));
            return;
        }

        lineEdit_IMPRESSION_LIGNES
			->setPalette(YerothUtils::YEROTH_WHITE_PALETTE);

        lineEdit_IMPRESSION_LIGNES->setYerothEnabled(true);
    }
    else
    {
        SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

        lineEdit_IMPRESSION_LIGNES
			->setPalette(YerothUtils::YEROTH_BLACK_PALETTE);

        lineEdit_IMPRESSION_LIGNES->setYerothEnabled(false);
    }
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::
		SET_CURRENT_WINDOW_TO_PRINT_PRINTING_PARAMETERS_AND_POSITION(int pageFROM,
                                                             	 	 int pageTO)
{
    if (0 != _current_window_to_print)
    {
        _current_window_to_print->set_PRINTING_PARAMETER_pageFROM(pageFROM);

        _current_window_to_print->set_PRINTING_PARAMETER_pageTO(pageTO);

        if (radioButton_IMPRESSION_VERTICAL->isChecked())
        {
            _current_window_to_print
				->set_PRINTING_PARAMETER_printing_position(
						YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_PORTRAIT_SPECIFICATION);
        }
        else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
        {
            _current_window_to_print
				->set_PRINTING_PARAMETER_printing_position(
						YerothTableViewPRINT_UTILITIES_TEX_TABLE::_A4PAGE_LANDSCAPE_SPECIFICATION);
        }
    }
}


void YerothIMPRESSION_DE_DOCUMENT_Dialog::valider()
{
    if (0 != _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE)
    {
        QString nbre_de_lignes_string = lineEdit_IMPRESSION_LIGNES->text();

        if (radioButton_IMPRESSION_VERTICAL->isChecked())
        {
            _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE
				->SET_VERTICAL_PRINT_POSITION();
        }
        else if (radioButton_IMPRESSION_HORIZONTAL->isChecked())
        {
            _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE
				->SET_HORIZONTAL_PRINT_POSITION();
        }

        if (!lineEdit_IMPRESSION_LIGNES->isReadOnly())
        {
            _yeroth_CURRENT_DOCUMENT_PRINT_UTILITIES_TEX_TABLE
				->SET_NOMBRE_DE_LIGNES(nbre_de_lignes_string.toUInt());
        }
    }

    rendreInvisible();

    if (0 != _current_window_to_print)
    {
        _current_window_to_print->set_PARAMETER_TABLE_COLUMN_ORDER();

        QString string_pageFROM = lineEdit_pageFROM->text();

        QString string_pageTO = lineEdit_pageTO->text();

//              QDEBUG_STRING_OUTPUT_1("YEROTH-1");

        if (string_pageFROM.isEmpty() &&
        	string_pageTO.isEmpty())
        {
//                      QDEBUG_STRING_OUTPUT_1("YEROTH-2");

            SET_NOMBRE_DE_LIGNES_PAR_DEFAUT();

            SET_CURRENT_WINDOW_TO_PRINT_PRINTING_PARAMETERS_AND_POSITION(-1,
                                                                         -1);

            _current_window_to_print
				->set_PRINTING_PARAMETER_print_table_row_count(lineEdit_IMPRESSION_LIGNES->text().toUInt());

            _current_window_to_print->imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();
        }
        else
        {
            uint pageFROM = string_pageFROM.toUInt();

            uint pageTO = string_pageTO.toUInt();

            if (pageFROM > pageTO)
            {
                YerothQMessageBox::warning(_current_window_to_print,
                                           QObject::tr("IMPRESSION, NUMÉRO DE PAGES"),
                                           QObject::tr("La numéro de la page de départ doit "
                                        		   	   "être inférieur au numéro de la page terminale"));

                show();

                return;
            }

            SET_CURRENT_WINDOW_TO_PRINT_PRINTING_PARAMETERS_AND_POSITION(pageFROM,
            															 pageTO);

            _current_window_to_print
				->set_PRINTING_PARAMETER_print_table_row_count(lineEdit_IMPRESSION_LIGNES->text().toUInt());

            _current_window_to_print
				->imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR(pageFROM,
                                                             	   pageTO);
        }

        YerothPOSUser *aUser = _allWindows->getUser();

        if (0 != aUser)
        {
            aUser->save_user_personal_PRINTING_PARAMETER_settings();
        }
    }
}
