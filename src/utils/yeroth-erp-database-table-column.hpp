/*
 * yeroth-erp-database-table-column.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_UTILS_YEROTH_DATABASE_TABLES_COLUMNS_HPP_
#define SRC_UTILS_YEROTH_DATABASE_TABLES_COLUMNS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include <QtCore/QMap>

#include <QtCore/QString>

class YerothDatabaseTableColumn
{

public:

    YEROTH_CLASS_OPERATORS

	inline YerothDatabaseTableColumn()
	{
		RETRANSLATE();
	}

    inline ~YerothDatabaseTableColumn()
    {
    }

    static void RETRANSLATE();

    static const QString IS_SERVICE;

    static const QString IS_VENTE_EN_GROS;

    static const QString ID_ALERTE;

    static const QString ID;

    static const QString ERROR_ACCEPTED_RUNTIME_MONITOR_NAME;

    static const QString ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_ID;

    static const QString ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_DESCRIPTION;

    static const QString REFEREUR_CLIENT;

    static const QString REFEREUR_CLIENT_ID;

    static const QString GROUPES_DU_CLIENT;

    static const QString GROUPES_DU_CLIENT_ID;

    static const QString GROUPE_DE_PAIE_HR;

    static const QString GROUPES_DUN_EMPLOYE;

    static const QString GROUPES_DUN_EMPLOYE_ID;

    static const QString HISTORIQUE_STOCK_ACTIF;

    static const QString HISTORIQUE_STOCK;

    static const QString MEMBRES_DU_GROUPE_db_ID;

	static const QString POURCENTAGE_TAXES_IMPOSABLES;

	static const QString POURCENTAGE_DU_RABAIS;

    static const QString MAXIMUM_DE_MEMBRES;

    static const QString CLIENTS_ID;

    static const QString REFERENCE_RECU_PAIEMENT_CLIENT;

    static const QString REFERENCE_RECU_VENDU;

    static const QString REFERENCE_RECU_SORTIE;

    static const QString ENREGISTREUR_STOCK;

    static const QString NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT;

    static const QString STATUT_DE_LACHAT_AU_FOURNISSEUR;

    static const QString memo_sale_for_sale;

    static const QString REFERENCE;

    static const QString COMPTE_FOURNISSEUR;

    static const QString COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS;

    static const QString COMPTECLIENT_AVANCE_DE_CASH;

    static const QString COMPTE_CLIENT;

    static const QString DETTE_MAXIMALE_COMPTE_CLIENT;

    static const QString NUMERO_UNIQUE;

    static const QString NOM_DEPARTEMENT_PRODUIT;

    static const QString CATEGORIE;

    static const QString QUANTITE;

    static const QString STOCK_DALERTE;

    static const QString STOCK_MINIMUM;

    static const QString STOCK_MAXIMUM;

    static const QString QUANTITE_VENDUE;

    static const QString QUANTITE_TOTALE;

    static const QString QUANTITE_SORTIE;

    static const QString DESCRIPTION_GROUPE;

    static const QString DESCRIPTION_FOURNISSEUR;

    static const QString DESCRIPTION_PRODUIT;

    static const QString PRIX_UNITAIRE;

    static const QString PRIX_UNITAIRE_EN_GROS;

    static const QString REFERENCE_RECU_DACHAT;

    static const QString PROGRAMME_DE_FIDELITE_CLIENTS;

    static const QString PRIX_DACHAT_PRECEDENT;

    static const QString PRIX_DACHAT;

    static const QString PRIX_VENTE_PRECEDENT;

    static const QString PRIX_VENTE_EN_GROS_PRECEDENT;

    static const QString PRIX_VENTE;

    static const QString PRIX_VENTE_EN_GROS;

    static const QString REFERENCE_DU_COMPTE_BANCAIRE;

    static const QString AGENCE_DU_COMPTE_BANCAIRE;

    static const QString INTITULE_DU_COMPTE_BANCAIRE;

    static const QString INTITULE_COMPTE_DOPERATION_DANS_LIGNE_BUDGETAIRE;

    static const QString montant_RESTANT_de_la_ligne_budgetaire;

    static const QString montant_de_la_ligne_budgetaire;

    static const QString INTITULE_DE_LA_ligne_budgetaire;

    static const QString INSTITUT_BANCAIRE;

    static const QString DESCRIPTION_DU_COMPTE_BANCAIRE;

    static const QString DESCRIPTION_charge_financiere;

    static const QString DESCRIPTION_ligne_budgetaire;

    static const QString DESCRIPTION_DEPARTEMENT_PRODUIT;

    static const QString DESCRIPTION_CATEGORIE;

    static const QString IS_SOUS_CATEGORIE;

    static const QString MANAGER;

    static const QString DEPARTEMENT;

    static const QString POSITION_FONCTION;

    static const QString NOM_CATEGORIE;

    static const QString LIGNE_BUDGETAIRE;

    static const QString MONTANT_TVA;

    static const QString MONTANT_TVA_EN_GROS;

    static const QString TVA;

    static const QString MARGE_BENEFICIAIRE;

    static const QString MARGE_BENEFICIAIRE_EN_GROS;

    static const QString MONTANT_RECU;

    static const QString MONTANT_A_REMBOURSER;

    static const QString MONTANT_TOTAL_VENTE;

    static const QString MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS;

    static const QString SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL;

    static const QString REMISE_PRIX;

    static const QString REMISE_POURCENTAGE;

    static const QString LOCALISATION;

    static const QString LOCALISATION_STOCK;

    static const QString QUANTITE_PAR_LOT;

    static const QString LOTS_ENTRANT;

    static const QString EMPLOYE;

    static const QString IMAGE_FOURNISSEUR;

    static const QString IMAGE_COMPTE_CLIENT;

    static const QString LOGO_ENTREPRISE;

    static const QString IMAGE_PRODUIT;

    static const QString DATE_DE_DEBUT_DAPPARTENANCE;

    static const QString DATE_DE_FIN_DAPPARTENANCE;

    static const QString DATE_PEREMPTION;

    static const QString DATE_DE_RECEPTION;

    static const QString DATE_DE_COMMANDE;

    static const QString DATE_ENTREE;

    static const QString PRENOM;

    static const QString NOM;

    static const QString NOM_RECEPTEUR;

    static const QString NOM_MAGASINIER;

    static const QString NOM_UTILISATEUR_MAGASINIER;

    static const QString NOM_UTILISATEUR_CAISSIER;

    static const QString NOM_CAISSIER;

    static const QString NOM_COMPLET_CREATEUR_REMISE_ALERTE;

    static const QString NOM_COMPLET;

    static const QString NOM_LOCALISATION;

    static const QString MESSAGE_PDV_RECU_PETIT;

    static const QString NOM_UTILISATEUR;

    static const QString STOCKS_ID;

    static const QString STOCK_MANAGER;

    static const QString MOT_PASSE;

    static const QString TITRE;

    static const QString APPELATION_TITRE;

    static const QString ROLE;

    static const QString NOM_ROLE;

    static const QString EMAIL;

    static const QString REFERENCE_FOURNISSEUR;

    static const QString REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS;

    static const QString REFERENCE_GROUPE;

    static const QString REFERENCE_GROUPE_DE_PAIE_hr;

    static const QString REFERENCE_GROUPE_DEMPLOYES_hr;

    static const QString REFERENCE_CLIENT;

    static const QString NUMERO_TELEPHONE_1;

    static const QString NUMERO_TELEPHONE_2;

    static const QString LIEU_NAISSANCE;

    static const QString DATE_NAISSANCE;

    static const QString QUARTIER_ENTREPRISE_CLIENT;

    static const QString VILLE_ENTREPRISE_CLIENT;

    static const QString VILLE;

    static const QString QUARTIER;

    static const QString REGION_ETAT_ENTREPRISE_CLIENT;

    static const QString PROVINCE_ETAT;

    static const QString PAYS;

    static const QString ADRESSE;

    static const QString ADRESSE_IP;

    static const QString ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE;

    static const QString BOITE_POSTALE;

    static const QString STATUT_RESOLU;

    static const QString SIEGE_SOCIAL;

    static const QString NOTES;

    static const QString NOM_ENTREPRISE;

    static const QString SECTEURS_DACTIVITES;

    static const QString CARTE_DE_FIDELITE_CLIENT_nom_entreprise;

    static const QString CARTE_DE_FIDELITE_CLIENT_siege;

    static const QString CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele;

    static const QString CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client;

    static const QString CARTE_DE_FIDELITE_CLIENT_email;

    static const QString CARTE_DE_FIDELITE_CLIENT_telephone;

    static const QString NUMERO_CONTRIBUABLE;

    static const QString REFERENCE_REGISTRE_DU_COMMERCE;

    static const QString NOM_REPRESENTANT;

    static const QString NOM_ENTREPRISE_CLIENT;

    static const QString NOM_ENTREPRISE_FOURNISSEUR;

	static const QString DESCRIPTION_GROUPE_DE_PAIE_HR;

    static const QString DESCRIPTION_GROUPE_DEMPLOYES_HR;

    static const QString DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS;

    static const QString DESCRIPTION_CLIENT;

    static const QString DESIGNATION;

    static const QString DESIGNATION_REMISE;

    static const QString DESIGNATION_ALERTE;

    static const QString CONDITION_ALERTE;

    static const QString NOM_ENCAISSEUR;

    static const QString DATE_PAIEMENT;

    static const QString MONTANT_PAYE;

    static const QString MONTANT_A_PAYER_MENSUEL;

    static const QString DATE_VENTE;

    static const QString DATE_DEBUT;

    static const QString DATE_FIN;

    static const QString DATE_SORTIE;

    static const QString HEURE_PAIEMENT;

    static const QString HEURE_VENTE;

    static const QString HEURE_SORTIE;

    static const QString DESTINATAIRE;

    static const QString NOM_COMPLET_DESTINATAIRE;

    static const QString REMISE_NOTES;

    static const QString MESSAGE_ALERTE;

    static const QString DATE_CREATION;

    static const QString TYPE_DOPERATION_FINANCIERE;

    static const QString TYPE_DE_PAIEMENT;

    static const QString TYPE_DE_VENTE;

    static const QString NOTIFICATIONS;

    static const QString REMISE_RESOLUE;

    static const QString ALERTE_RESOLUE;

    static const QString LOCALISATION_SORTIE;

    static const QString LOCALISATION_ENTREE;

    static const QString LOCALISATION_SORTIE_ID;

    static const QString LOCALISATION_ENTREE_ID;


    static QMap<QString, QString> _tableColumnToUserViewString;
};

#endif /* SRC_UTILS_YEROTH_DATABASE_TABLES_COLUMNS_HPP_ */
