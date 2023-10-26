/*
 * yeroth-erp-progress-bar.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-progress-bar.hpp"


#include "src/admin/yeroth-erp-admin-windows-commons.hpp"


#include "src/windows/yeroth-erp-window-commons.hpp"


YerothProgressBar::YerothProgressBar(YerothPOSAdminWindowsCommons *aYerothParentAdminWindow,
                                     QString PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec /* = "rgb(0, 255, 70)" */)
:QProgressBar((QWidget *) aYerothParentAdminWindow),
 _setup(false)
{
    setVisible(false);

    QString RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND
				= COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48;

    if (0 != aYerothParentAdminWindow)
    {
        QToolBar *aYerothCommonWindowToolBar =
        		aYerothParentAdminWindow->getQMainWindowToolBar();

        if (0 != aYerothCommonWindowToolBar)
        {
            const QColor &aQColor = aYerothCommonWindowToolBar->palette().color(QPalette::Button);

            RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND =
                            QString("%1, %2, %3").arg(QString::number(aQColor.red()),
                                                      QString::number(aQColor.green()),
                                                      QString::number(aQColor.blue()));
        }
    }

    setStyleSheet(QString("QProgressBar "
                          "{text-align: center; "
                          "border: 2px solid white; "
                          "border-radius: 1px; color: %1;}"
                          "QProgressBar::chunk {"
                          "background-color: rgb(%2); "
                          "color: rgb(%3); "
                          "width: 3px;}")
    				.arg(PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec,
                         RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND,
    					 COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67));

    int width = qCeil(((QMainWindow *) parent())->width() / 2.0);

    int X = qCeil(width / 2.0);

    int Y = qCeil(((QMainWindow *) parent())->height() / 2.0);

    setFixedWidth(width);

    move(X, Y);
}


YerothProgressBar::YerothProgressBar(YerothWindowsCommons *aYerothParentWindow,
                                     QString PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec /* '= "rgb(0, 0, 0)" */)
:QProgressBar((QWidget *) aYerothParentWindow),
 _setup(false)
{
    setVisible(false);

    QString RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND
				= COLOUR_RGB_STRING_YEROTH_FIREBRICK_RED_255_48_48;

    if (0 != aYerothParentWindow)
    {
        QToolBar *aYerothCommonWindowToolBar =
        		aYerothParentWindow->getQMainWindowToolBar();

        if (0 != aYerothCommonWindowToolBar)
        {
            const QColor &aQColor =
            		aYerothCommonWindowToolBar->palette().color(QPalette::Button);

            RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND =
                            QString("%1, %2, %3").arg(QString::number(aQColor.red()),
                                                      QString::number(aQColor.green()),
                                                      QString::number(aQColor.blue()));
        }
    }

    setStyleSheet(QString("QProgressBar "
                          "{text-align: center; "
                          "border: 2px solid white; "
                          "border-radius: 1px; color: %1;}"
                          "QProgressBar::chunk {"
                          "background-color: rgb(%2); "
                          "color: rgb(%3); "
                          "width: 3px;}")
    				.arg(PROGRESS_BAR_TEXT_COLOR_RGB_simple_spec,
                         RGB_COLOR_TO_DISPLAY_AT_YEROTH_PROGRESS_BAR_BACKGROUND,
    					 COLOUR_RGB_STRING_YEROTH_DARK_GREEN_47_67_67));

    int width = qCeil(((QMainWindow *) parent())->width() / 2.0);

    int X = qCeil(width / 2.0);

    int Y = qCeil(((QMainWindow *) parent())->height() / 2.0);

    setFixedWidth(width);

    move(X, Y);
}



