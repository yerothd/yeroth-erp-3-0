/*
 * yeroth-erp-credit-card-info.cpp
 *
 *   Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */



#include "yeroth-erp-carte-credit-info.hpp"



void YerothPOSCreditCardInfo::clearAllFields()
{
    _issuingCompany.clear();

    _nameOnCard.clear();

    _postalCode.clear();

    _cardNumber.clear();

    _expiryDate_YY.clear();

    _expiryDate_MM.clear();

    _cvCode.clear();
}
