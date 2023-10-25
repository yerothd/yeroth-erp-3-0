/*
 * yeroth-erp-main-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_MAIN_WINDOW_HPP_
#define SRC_YEROTH_MAIN_WINDOW_HPP_

#include "../../ui_yeroth-erp-main-window.h"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "yeroth-erp-window-commons.hpp"


#include <tuple>

#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothSqlTableModel;
class YerothLogger;
class YerothUtils;
class YerothWindowsCommons;
class YerothPOSUser;

class YerothMainWindow : public YerothWindowsCommons,
						 public Ui_YerothMainWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothMainWindow();

    inline ~YerothMainWindow()
    {
        delete _logger;
    }

    inline virtual QToolBar *getQMainWindowToolBar()
    {
        return toolBar_mainWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirAdministrateur();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    void setup_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    void write_AFTER_SETUP_YEROTH_ERP_3_0_SERVER_PARAMETERS_DISPLAY();

    inline static QString GET_YEROTH_ERP_WINDOW_TITLE()
    {
    	return YerothMainWindow::LANGUE_ANGLAISE ?
    			QString("YEROTH-ERP-3.0") :
				QString("Yeroth-pgi-3.0");
    }

    inline static QString get_TRANSLATED_WindowName(const char *WINDOW_TITLE)
    {
        return QString("%1 - %2")
        			.arg(YerothMainWindow::GET_YEROTH_ERP_WINDOW_TITLE(),
        				 QObject::tr(WINDOW_TITLE));
    }

    inline static QString get_TRANSLATED_WindowName(QString WINDOW_TITLE)
    {
        return get_TRANSLATED_WindowName(WINDOW_TITLE.toStdString().c_str());
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    void rendreVisibleLocalTOCLASS(bool isDatabaseOpened = false);


    static bool LANGUE_ANGLAISE;


private slots:

    virtual void ON_action_START_YR_QVGE_dynamic_analysis_tool();

	void CHANGER_DE_LANGUE();


public slots:

    inline virtual void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual void help();

    virtual void deconnecter_utilisateur();

    void cleanupAllOtherWindows();


protected:

    virtual void setupShortcuts();


private:

    void CHANGER_DE_LANGUE__text__TRANSLATION();


public:

    static QString STRING_MONDAY;

    static QString STRING_TUESDAY;

    static QString STRING_WEDNESDAY;

    static QString STRING_THURSDAY;

    static QString STRING_FRIDAY;

    static QString STRING_SATURDAY;

    static QString STRING_SUNDAY;


private:

    void calculate_presentation_geometry_of_push_buttons_in_MAIN_WINDOW();


    YerothLogger *_logger;

    QList<YerothPushButton *> _main_window_buttons;

    QMap<uint, std::tuple <int, int>> _main_window_button_TO_page_position;
};


#define GET_YEROTH_ERP_WINDOW_TITLE_MACRO YerothMainWindow::GET_YEROTH_ERP_WINDOW_TITLE()



#endif /* SRC_YEROTH_MAIN_WINDOW_HPP_ */
