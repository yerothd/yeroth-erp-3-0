/*
 * yeroth-erp-calcul-salaire-EMPLOYE-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_
#define YEROTH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_


#include "src/windows/yeroth-erp-window-commons.hpp"

#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothSqlTableModel;


class YerothERPCalculSalaireEMPLOYE : public QObject
{
	Q_OBJECT


public:


    YEROTH_CLASS_OPERATORS


    inline YerothERPCalculSalaireEMPLOYE()
    :_sommeTotal(0.0),
     _calling_window(0)
    {
        __QMESSAGE_BOX_STYLE_SHEET =
                        QString("QMessageBox {background-color: rgb(%1);}"
                                "QMessageBox QLabel {color: rgb(%2);}")
                            .arg(COLOUR_RGB_STRING_YEROTH_GREEN_2_160_70,
                                 COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255);
    }


	YerothERPCalculSalaireEMPLOYE(YerothWindowsCommons *calling_window);


    inline ~YerothERPCalculSalaireEMPLOYE()
    {
    }

    double calculate_PAY_GROUP_MONEY_BENEFITS(QString &a_nom_entreprise_EMPLOYEE,
                                              QString &PAY_GROUP);


    double CALCULATE_EMPLOYEE_SALARY(const QString &a_nom_entreprise_client);


private:

    double _sommeTotal;

    QString __QMESSAGE_BOX_STYLE_SHEET;

    YerothWindowsCommons *_calling_window;
};


#endif /* YEROTH_CALCUL_SALAIRE_EMPLOYE_WINDOW_HPP_ */
