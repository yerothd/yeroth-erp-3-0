/*
 * yeroth-erp-user-caissier.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_CAISSIER_HPP_
#define SRC_YEROTH_ERP_USER_CAISSIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserCaissier:public YerothPOSUser
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothPOSUserCaissier(YerothERPWindows *
                                 allWindows):YerothPOSUser(allWindows)
    {
        _role = YerothUtils::ROLE_CAISSIER;
    }

    virtual inline ~YerothPOSUserCaissier()
    {
    }
};

#endif /* SRC_YEROTH_ERP_USER_CAISSIER_HPP_ */
