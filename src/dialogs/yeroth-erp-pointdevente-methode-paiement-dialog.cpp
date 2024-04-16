/*
 * yeroth-erp-pointdevente-methode-paiement-dialog.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include"src/yeroth-erp-windows.hpp"

#include "yeroth-erp-pointdevente-methode-paiement-dialog.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include"src/utils/yeroth-erp-utils.hpp"



#include <cassert>

#include <QtCore/QPoint>

#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QDialog>

#include <QtWidgets/QMainWindow>


const QString
YerothPOSVenteMethodePaiementDialog::

DIALOG_WINDOW_TITLE(QString("%1 - %2").arg
                    (GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                     QObject::tr("Méthodes de Paiement")));

YerothPOSVenteMethodePaiementDialog::YerothPOSVenteMethodePaiementDialog(YerothERPWindows *allWindows, YerothWindowsCommons *parent):YerothPOSDialogCommons
    (YerothPOSVenteMethodePaiementDialog::DIALOG_WINDOW_TITLE),
    _currentPosition(new QPoint)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerothUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this, *_currentPosition);

    setWindowTitle(YerothPOSVenteMethodePaiementDialog::DIALOG_WINDOW_TITLE);

    setFixedSize(width(), height());

    pushButton_comptant->setFocus();

    connect(pushButton_carte_de_credit, SIGNAL(clicked()), this,
            SLOT(paiement_carte_de_credit()));

    connect(pushButton_comptant, SIGNAL(clicked()), this,
            SLOT(paiement_comptant()));

    connect(pushButton_compte_client_AVANCE_DE_CASH, SIGNAL(clicked()), this,
            SLOT(paiement_compteclient_AVANCE_DE_CASH()));
}

void YerothPOSVenteMethodePaiementDialog::show()
{
    move(*_currentPosition);

    pushButton_comptant->setFocus();

    YerothPOSDialogCommons::show();
}


void YerothPOSVenteMethodePaiementDialog::paiement_carte_de_credit()
{
    _allWindows->_pdVenteWindow->
    set_paiement_par_carte_credit_carte_debit(true);

    YerothPOSCreditCardInfo *currentCreditCardInfo =
                    _allWindows->_pdVenteWindow->getCurrentCreditCardInfo();

    assert(0 != currentCreditCardInfo);

    currentCreditCardInfo->clearAllFields();

    _allWindows->
    _pdVenteMethodePaiementCarteCreditDialog->setCreditCardInfoDataStructure
    (currentCreditCardInfo);

    rendreInvisible();

    _allWindows->
    _pdVenteMethodePaiementCarteCreditDialog->showAsModalDialogWithParent
    (*_allWindows->_pdVenteWindow);
}


void YerothPOSVenteMethodePaiementDialog::paiement_comptant()
{
    _allWindows->_pdVenteWindow->set_paiement_comptant(true);

    rendreInvisible();

    _allWindows->
    _pdVenteMethodePaiementComptantEntreeDialog->showAsModalDialogWithParent
    (*_allWindows->_pdVenteWindow);
}


void
YerothPOSVenteMethodePaiementDialog::
effectuer_checkout_compteclient_AVANCE_DE_CASH()
{
    _allWindows->_pdVenteWindow->
    effectuer_checkout_compteclient_AVANCE_DE_CASH();
}


void YerothPOSVenteMethodePaiementDialog::paiement_compteclient_AVANCE_DE_CASH()
{
    _allWindows->_pdVenteWindow->set_paiment_compteclient_AVANCE_DE_CASH(true);

    rendreInvisible();

    _allWindows->_pdVenteWindow->
    effectuer_checkout_compteclient_AVANCE_DE_CASH();
}
