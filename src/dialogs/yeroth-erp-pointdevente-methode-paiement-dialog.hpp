/*
 * yeroth-erp-pointdevente-methode-paiement-dialog.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_
#define YEROTH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_

#include "../../ui_yeroth-erp-pointdevente-methode-paiement-dialog.h"

#include "yeroth-erp-dialog-commons.hpp"

class QPoint;

class YerothWindowsCommons;
class YerothERPWindows;

class YerothPOSVenteMethodePaiementDialog:public YerothPOSDialogCommons,
    private Ui_YerothPOSVenteMethodePaiementDialog
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS YerothPOSVenteMethodePaiementDialog(YerothERPWindows *allWindows,
                                                               YerothWindowsCommons *parent);

    inline ~YerothPOSVenteMethodePaiementDialog()
    {
        delete _currentPosition;
    }

    inline void setPushbuttonCompteClient__AVANCE_DE_CASH_Enabled(bool enabled)
    {
        pushButton_compte_client_AVANCE_DE_CASH->setEnabled(enabled);
    }

    virtual QPoint *getCurrentPosition()
    {
        return _currentPosition;
    }

public slots:
    virtual void show();

    void paiement_carte_de_credit();

    void paiement_comptant();

    void effectuer_checkout_compteclient_AVANCE_DE_CASH();

    void paiement_compteclient_AVANCE_DE_CASH();

private:

    static const QString DIALOG_WINDOW_TITLE;

    QPoint *_currentPosition;
};


#endif /* YEROTH_ERP_VENTE_METHODE_PAIEMENT_DIALOG_HPP_ */
