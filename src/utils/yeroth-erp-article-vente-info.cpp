/*
 * yeroth-erp-article-vente-info.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-article-vente-info.hpp"

#include <QtCore/QDebug>


void YerothArticleVenteInfo::print()
{
    qDebug() << QString("[ArticleVenteInfo][print]"
                        "	stockReference: %1"
                        "	stockName: %2"
    					"	MEMO_SALE_for_SEARCH: %3"
                        "	stockID: %4"
                        "	_qte_en_stock: %5"
                        "	_prix_unitaire: %6"
                        "	_quantite_a_vendre: %7"
                        "	_montant_tva: %8"
                        "	_AJOUT_COMMERCIAL: %9"
                        "	_remise_prix: %10"
                        "	_remise_pourcentage: %11").
             arg(_stockReference,
            	 _stockName,
				 _MEMO_SALE_for_SEARCH,
				 _stockID,
                 QString::number(_quantite_en_stock),
                 QString::number(_prix_unitaire),
                 QString::number(_quantite_a_vendre),
                 QString::number(_montant_tva),
				 QString::number(_AJOUT_COMMERCIAL),
				 QString::number(_remise_prix),
                 QString::number(_remise_pourcentage));
}


double YerothArticleVenteInfo::prix_vente()
{
    double result_prix_vente = 0.0;

    if (!_effectuer_vente_en_gros)
    {
        result_prix_vente =
                        (_prix_unitaire + _montant_tva - _remise_prix) * _quantite_a_vendre;
    }
    else
    {
        result_prix_vente =
                        (_prix_unitaire_en_gros + _montant_tva_en_gros -
                         _remise_prix) * _quantite_a_vendre;
    }

    result_prix_vente = result_prix_vente + _AJOUT_COMMERCIAL;

    return result_prix_vente;
}
