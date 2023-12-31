/*
 * admin-actions-subjects.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef ADMIN_ACTIONS_SUBJECTS_HPP_
#define ADMIN_ACTIONS_SUBJECTS_HPP_

enum AdminAction
{
    ACTION_CREER = 0,
    ACTION_LISTER = 1,
    ACTION_MODIFIER = 2,
    ACTION_SUPPRIMER = 3,
    ACTION_EXPORTER = 4,
    ACTION_IMPORTER = 5,
    ACTION_EFFACER = 6
};

enum AdminSujetAction
{
    SUJET_ACTION_ALERTE = 0,
    SUJET_ACTION_CHARGE_FINANCIERE = 1,
    SUJET_ACTION_CATEGORIE = 2,
    SUJET_ACTION_ligne_budgetaire = 3,
	SUJET_ACTION_COMPTE_BANCAIRE = 4,
    SUJET_ACTION_COMPTE_UTILISATEUR = 5,
    SUJET_ACTION_DEPARTEMENTS_DE_PRODUITS = 6,
    SUJET_ACTION_LOCALISATION = 7,
    SUJET_ACTION_REMISE = 8,
    SUJET_ACTION_DB = 9,
    SUJET_ACTION_TABLEAU = 10,
    SUJET_ACTION_DONNEES = 11
};


#endif /* ADMIN_ACTIONS_SUBJECTS_HPP_ */
