/*
 * yeroth-erp-client-account-sale-info.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_CLIENT_ACCOUNT_SALE_INFO_HPP_
#define SRC_YEROTH_CLIENT_ACCOUNT_SALE_INFO_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QString>
#include <QtCore/QDebug>

#include "src/utils/yeroth-erp-utils.hpp"

class YerothUtils;

class YEROTH_CLIENT_ACCOUNT_SALE_info
{
public:

    YEROTH_CLASS_OPERATORS inline YEROTH_CLIENT_ACCOUNT_SALE_info()
    {
    }

    inline ~YEROTH_CLIENT_ACCOUNT_SALE_info()
    {
    }


    inline const QString get_db_ID()
    {
        return _db_ID;
    }

    inline const QString &getCategory()
    {
        return _category;
    }

    inline const QString &getClientEmail()
    {
        return _client_email;
    }

    inline const QString &getClientName()
    {
        return _client_name;
    }

    inline const QString &getClientCity()
    {
        return _client_city;
    }

    inline const QString &getClientPhone()
    {
        return _client_phone;
    }

    inline const QString &getClientPoBox()
    {
        return _client_po_box;
    }

    inline const QString &getClientRepresentantName()
    {
        return _client_representant_name;
    }

    inline const QString &getProductName()
    {
        return _product_name;
    }

    inline const QString &getProductReference()
    {
        return _product_reference;
    }

    inline const QString &getQuantity()
    {
        return _quantity;
    }

    inline const QString &getSaleType()
    {
        return _sale_type;
    }

    inline const QString &getTaxes()
    {
        return _taxes;
    }

    inline const QString &getTotalRemainingAmount()
    {
        return _total_remaining_amount;
    }

    inline const QString &getTotalAmountAtc()
    {
        return _total_amount_ATC;
    }

    inline const QString &getUnitPrice()
    {
        return _unit_price;
    }

    inline const QString &getSiteOfInvoiceEmission()
    {
        return _site_of_invoice_emission;
    }


    QString _db_ID;

    QString _site_of_invoice_emission;


    QString _client_city;

    QString _client_name;

    QString _client_representant_name;

    QString _client_po_box;

    QString _client_email;

    QString _client_phone;


    QString _sale_type;

    QString _product_reference;

    QString _product_name;

    QString _category;

    QString _quantity;

    QString _unit_price;

    QString _taxes;

    QString _total_remaining_amount;

    QString _total_amount_ATC;
};


#endif /* SRC_YEROTH_CLIENT_ACCOUNT_SALE_INFO_HPP_ */
