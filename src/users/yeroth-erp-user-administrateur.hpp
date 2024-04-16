/*
 * yeroth-erp-user-administrateur.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_
#define SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_

#include "yeroth-erp-users.hpp"

class YerothPOSUserAdministrateur:public YerothPOSUser
{
Q_OBJECT public:

    YEROTH_CLASS_OPERATORS
    inline YerothPOSUserAdministrateur(YerothERPWindows *
                                       allWindows):YerothPOSUser(allWindows)
    {
        _role = YerothUtils::ROLE_ADMINISTRATEUR;
    }

    virtual inline ~YerothPOSUserAdministrateur()
    {
    }
};

#endif /* SRC_YEROTH_ERP_USER_ADMINISTRATEUR_HPP_ */
