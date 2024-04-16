/*
 * yeroth-erp-user-magasinier.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_MAGASINIER_HPP_
#define SRC_YEROTH_ERP_USER_MAGASINIER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserMagasinier:public YerothPOSUser
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothPOSUserMagasinier(YerothERPWindows *
                                   allWindows):YerothPOSUser(allWindows)
    {
        _role = YerothUtils::ROLE_MAGASINIER;
    }

    virtual inline ~YerothPOSUserMagasinier()
    {
    }
};

#endif /* SRC_YEROTH_ERP_USER_MAGASINIER_HPP_ */
