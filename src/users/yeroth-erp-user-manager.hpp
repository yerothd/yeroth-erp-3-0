/*
 * yeroth-erp-user-manager.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_MANAGER_HPP_
#define SRC_YEROTH_ERP_USER_MANAGER_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserManager:public YerothPOSUser
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothPOSUserManager(YerothERPWindows *
                                allWindows):YerothPOSUser(allWindows)
    {
        _role = YerothUtils::ROLE_MANAGER;
    }

    virtual inline ~YerothPOSUserManager()
    {
    }
};


#endif /* SRC_YEROTH_ERP_USER_MANAGER_HPP_ */
