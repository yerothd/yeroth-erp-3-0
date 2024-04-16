/*
 * yeroth-erp-LIST-MAP-COMPLEX-ITEM.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_LIST_MAP_COMPLEX_ITEM_HPP_
#define SRC_YEROTH_LIST_MAP_COMPLEX_ITEM_HPP_


#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "yeroth-erp-map-COMPLEX-ITEM.hpp"


#include <QtCore/QtAlgorithms>

#include <QtCore/QString>


class Yeroth_LIST_map_COMPLEX_ITEM
{
public:

    inline Yeroth_LIST_map_COMPLEX_ITEM()
    {
    }

    virtual inline ~Yeroth_LIST_map_COMPLEX_ITEM()
    {
        clear();
    }

    virtual void clear();

    virtual const Yeroth_MAP_COMPLEX_Item *find_item(const QString
                                                     an_item_name);

    virtual void insert_item(const QString &an_item_name,
                             double aDOUBLE_value);

    virtual inline void q_sort()
    {
        qSort(_my_list_complex_items.begin(),
              _my_list_complex_items.end(), Yeroth_MAP_COMPLEX_Item::lessThan);
    }

    virtual inline int isEmpty()
    {
        return (_my_list_complex_items.size() <= 0);
    }

    virtual inline int size()
    {
        return _my_list_complex_items.size();
    }

    virtual inline const QList < Yeroth_MAP_COMPLEX_Item * > &q_list()
    {
        return _my_list_complex_items;
    }

    virtual void print(const QString &a_label = YerothUtils::EMPTY_STRING);

private:

    QList < Yeroth_MAP_COMPLEX_Item * >_my_list_complex_items;
};

#endif /* SRC_YEROTH_LIST_MAP_COMPLEX_ITEM_HPP_ */
