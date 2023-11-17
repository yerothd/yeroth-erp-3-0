/*
 * yeroth-erp-database-table-column.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "yeroth-erp-database-table-column.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


QMap<QString, QString> YerothDatabaseTableColumn::_tableColumnToUserViewString;


const QString YerothDatabaseTableColumn::IS_SERVICE("is_service");

const QString YerothDatabaseTableColumn::IS_VENTE_EN_GROS("is_vente_en_gros");

const QString YerothDatabaseTableColumn::ID_ALERTE("id_alerte");

const QString YerothDatabaseTableColumn::ID("id");

const QString YerothDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME("error_accepted_runtime_monitor_NAME");

const QString YerothDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_ID("error_accepted_runtime_monitor_name_ID");

const QString YerothDatabaseTableColumn::ERROR_ACCEPTED_RUNTIME_MONITOR_NAME_DESCRIPTION("error_accepted_runtime_monitor_name_DESCRIPTION");

const QString YerothDatabaseTableColumn::REFEREUR_CLIENT("refereur_client");

const QString YerothDatabaseTableColumn::REFEREUR_CLIENT_ID("refereur_client_ID");

const QString YerothDatabaseTableColumn::GROUPES_DU_CLIENT("groupes_du_client");

const QString YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID("groupes_du_client_ID");

const QString YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR("groupe_de_paie_hr");

const QString YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE("groupes_dun_employe");

const QString YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID("groupes_dun_employe_ID");

const QString YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF("historique_stock_actif");

const QString YerothDatabaseTableColumn::HISTORIQUE_STOCK("historique_stock");

const QString YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID("membres_du_groupe_db_ID");

const QString YerothDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES("pourcentage_TAXES_IMPOSABLES");

const QString YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS("pourcentage_du_rabais");

const QString YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES("maximum_de_membres");

const QString YerothDatabaseTableColumn::CLIENTS_ID("clients_id");

const QString YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT("reference_recu_paiement_client");

const QString YerothDatabaseTableColumn::REFERENCE_RECU_VENDU("reference_recu_vendu");

const QString YerothDatabaseTableColumn::REFERENCE_RECU_SORTIE("reference_recu_sortie");

const QString YerothDatabaseTableColumn::ENREGISTREUR_STOCK("enregistreur_stock");

const QString YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT("nom_utilisateur_du_commandeur_de_lachat");

const
QString
YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR
("statut_de_lachat_au_fournisseur");

const QString YerothDatabaseTableColumn::memo_sale_for_sale("memo_sale_for_sale");

const QString YerothDatabaseTableColumn::REFERENCE("reference");

const QString YerothDatabaseTableColumn::COMPTE_FOURNISSEUR("compte_fournisseur");
const QString
YerothDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS
("compte_client_PROGRAMME_DE_FIDELITE_CLIENTS");
const
QString
YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH
("compte_client_AVANCE_DE_CASH");
const
QString YerothDatabaseTableColumn::COMPTE_CLIENT("compte_client");
const QString
YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT
("dette_maximale_compte_client");

const QString YerothDatabaseTableColumn::NUMERO_UNIQUE("numero_unique");

const QString YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT("nom_departement_produit");
const
QString YerothDatabaseTableColumn::CATEGORIE("categorie");
const QString YerothDatabaseTableColumn::QUANTITE("quantite");
const QString YerothDatabaseTableColumn::STOCK_DALERTE("stock_dalerte");
const QString YerothDatabaseTableColumn::STOCK_MINIMUM("stock_minimum");
const QString YerothDatabaseTableColumn::STOCK_MAXIMUM("stock_maximum");
const QString YerothDatabaseTableColumn::QUANTITE_VENDUE("quantite_vendue");
const QString YerothDatabaseTableColumn::QUANTITE_TOTALE("quantite_totale");
const QString YerothDatabaseTableColumn::QUANTITE_SORTIE("quantite_sortie");
const QString YerothDatabaseTableColumn::DESCRIPTION_GROUPE("description_groupe");
const QString
YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR
("description_fournisseur");
const
QString YerothDatabaseTableColumn::DESCRIPTION_PRODUIT("description_produit");
const QString YerothDatabaseTableColumn::PRIX_UNITAIRE("prix_unitaire");
const QString YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS("prix_unitaire_en_gros");
const QString YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT("reference_recu_dachat");
const QString
YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS
("programme_de_fidelite_clients");
const
QString YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT("prix_dachat_precedent");
const QString YerothDatabaseTableColumn::PRIX_DACHAT("prix_dachat");
const QString YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT("prix_vente_precedent");

const QString YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT("prix_vente_en_gros_precedent");

const QString YerothDatabaseTableColumn::PRIX_VENTE("prix_vente");

const QString YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS("prix_vente_en_gros");

const QString YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE("reference_du_compte_bancaire");

const QString YerothDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE("agence_du_compte_bancaire");

const QString YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE("intitule_du_compte_bancaire");

const QString YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire("montant_RESTANT_de_la_ligne_budgetaire");

const QString YerothDatabaseTableColumn::montant_de_la_ligne_budgetaire("montant_de_la_ligne_budgetaire");

const QString YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire("intitule_de_la_ligne_budgetaire");

const QString YerothDatabaseTableColumn::INSTITUT_BANCAIRE("institut_bancaire");

const QString YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE("description_du_compte_bancaire");

const QString YerothDatabaseTableColumn::DESCRIPTION_charge_financiere("description_charge_financiere");

const QString YerothDatabaseTableColumn::DESCRIPTION_ligne_budgetaire("description_ligne_budgetaire");

const QString YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT("description_departement_produit");

const QString YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE("description_categorie");

const QString YerothDatabaseTableColumn::IS_SOUS_CATEGORIE("is_sous_categorie");

const QString YerothDatabaseTableColumn::MANAGER("manager");

const QString YerothDatabaseTableColumn::DEPARTEMENT("departement");

const QString YerothDatabaseTableColumn::POSITION_FONCTION("position_fonction");

const QString YerothDatabaseTableColumn::NOM_CATEGORIE("nom_categorie");

const QString YerothDatabaseTableColumn::LIGNE_BUDGETAIRE("ligne_budgetaire");

const QString YerothDatabaseTableColumn::MONTANT_TVA("montant_tva");

const QString YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS("montant_tva_en_gros");

const QString YerothDatabaseTableColumn::TVA("tva");

const QString YerothDatabaseTableColumn::MARGE_BENEFICIAIRE("marge_beneficiaire");

const QString YerothDatabaseTableColumn::MARGE_BENEFICIAIRE_EN_GROS("marge_beneficiaire_en_gros");

const QString YerothDatabaseTableColumn::MONTANT_RECU("montant_recu");

const QString YerothDatabaseTableColumn::MONTANT_A_REMBOURSER("montant_a_rembourser");

const QString YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE("montant_total_vente");

const QString YerothDatabaseTableColumn::MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS("montant_rabais_programme_de_fidelite_clients");

const QString YerothDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL("service_supplement_ajout_commercial");

const QString YerothDatabaseTableColumn::REMISE_PRIX("remise_prix");

const QString YerothDatabaseTableColumn::REMISE_POURCENTAGE("remise_pourcentage");

const QString YerothDatabaseTableColumn::LOCALISATION("localisation");

const QString YerothDatabaseTableColumn::LOCALISATION_STOCK("localisation_stock");

const QString YerothDatabaseTableColumn::QUANTITE_PAR_LOT("quantite_par_lot");

const QString YerothDatabaseTableColumn::LOTS_ENTRANT("lots_entrant");

const QString YerothDatabaseTableColumn::EMPLOYE("employe");

const QString YerothDatabaseTableColumn::IMAGE_FOURNISSEUR("image_fournisseur");

const QString YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT("image_compte_client");

const QString YerothDatabaseTableColumn::LOGO_ENTREPRISE("logo_entreprise");

const QString YerothDatabaseTableColumn::IMAGE_PRODUIT("image_produit");

const QString YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE("date_de_debut_dappartenance");

const QString YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE("date_de_fin_dappartenance");

const QString YerothDatabaseTableColumn::DATE_PEREMPTION("date_peremption");

const QString YerothDatabaseTableColumn::DATE_DE_RECEPTION("date_de_reception");

const QString YerothDatabaseTableColumn::DATE_DE_COMMANDE("date_de_commande");

const QString YerothDatabaseTableColumn::DATE_ENTREE("date_entree");

const QString YerothDatabaseTableColumn::PRENOM("prenom");

const QString YerothDatabaseTableColumn::NOM("nom");

const QString YerothDatabaseTableColumn::NOM_RECEPTEUR("nom_recepteur");

const QString YerothDatabaseTableColumn::NOM_MAGASINIER("nom_magasinier");

const QString YerothDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER("nom_utilisateur_magasinier");

const QString YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER("nom_utilisateur_caissier");

const QString YerothDatabaseTableColumn::NOM_CAISSIER("nom_caissier");

const QString YerothDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE("nom_complet_createur_remise_alerte");

const QString YerothDatabaseTableColumn::NOM_COMPLET("nom_complet");

const QString YerothDatabaseTableColumn::NOM_LOCALISATION("nom_localisation");

const QString YerothDatabaseTableColumn::MESSAGE_PDV_RECU_PETIT("message_PDV_recu_petit");

const QString YerothDatabaseTableColumn::NOM_UTILISATEUR("nom_utilisateur");

const QString YerothDatabaseTableColumn::STOCKS_ID("stocks_id");

const QString YerothDatabaseTableColumn::STOCK_MANAGER("stock_manager");

const QString YerothDatabaseTableColumn::MOT_PASSE("mot_passe");

const QString YerothDatabaseTableColumn::TITRE("titre");

const QString YerothDatabaseTableColumn::APPELATION_TITRE("appelation_titre");

const QString YerothDatabaseTableColumn::ROLE("role");

const QString YerothDatabaseTableColumn::NOM_ROLE("nom_role");

const QString YerothDatabaseTableColumn::EMAIL("email");

const QString YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR("reference_fournisseur");

const QString YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS("reference_programme_de_fidelite_clients");

const QString YerothDatabaseTableColumn::REFERENCE_GROUPE("reference_groupe");

const QString YerothDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr("reference_groupe_de_paie_hr");

const QString YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr("reference_groupe_demployes_hr");

const QString YerothDatabaseTableColumn::REFERENCE_CLIENT("reference_client");

const QString YerothDatabaseTableColumn::NUMERO_TELEPHONE_1("numero_telephone_1");

const QString YerothDatabaseTableColumn::NUMERO_TELEPHONE_2("numero_telephone_2");

const QString YerothDatabaseTableColumn::LIEU_NAISSANCE("lieu_naissance");

const QString YerothDatabaseTableColumn::DATE_NAISSANCE("date_naissance");

const QString YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT("quartier_entreprise_client");

const QString YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT("ville_entreprise_client");

const QString YerothDatabaseTableColumn::VILLE("ville");

const QString YerothDatabaseTableColumn::QUARTIER("quartier");

const QString YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT("region_etat_entreprise_client");

const QString YerothDatabaseTableColumn::PROVINCE_ETAT("province_etat");

const QString YerothDatabaseTableColumn::PAYS("pays");

const QString YerothDatabaseTableColumn::ADRESSE("adresse");

const QString YerothDatabaseTableColumn::ADRESSE_IP("adresse_ip");

const QString YerothDatabaseTableColumn::ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE("adresse_ip_reseau_imprimante_thermique");

const QString YerothDatabaseTableColumn::BOITE_POSTALE("boite_postale");

const QString YerothDatabaseTableColumn::STATUT_RESOLU("statut_resolu");

const QString YerothDatabaseTableColumn::SIEGE_SOCIAL("siege_social");

const QString YerothDatabaseTableColumn::NOTES("notes");

const QString YerothDatabaseTableColumn::NOM_ENTREPRISE("nom_entreprise");

const QString YerothDatabaseTableColumn::SECTEURS_DACTIVITES("secteurs_dactivites");

const QString YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE("reference_registre_du_commerce");

const
QString
YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise
("CARTE_DE_FIDELITE_CLIENT_nom_entreprise");

const
QString
YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege
("CARTE_DE_FIDELITE_CLIENT_siege");

const
QString
YerothDatabaseTableColumn::

CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele
("CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele");

const
QString
YerothDatabaseTableColumn::

CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client
("CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client");

const
QString
YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email
("CARTE_DE_FIDELITE_CLIENT_email");

const
QString
YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone
("CARTE_DE_FIDELITE_CLIENT_telephone");

const
QString YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE("numero_contribuable");
const QString YerothDatabaseTableColumn::NOM_REPRESENTANT("nom_representant");
const QString YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT("nom_entreprise_client");
const QString
YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR
("nom_entreprise_fournisseur");

const QString YerothDatabaseTableColumn::DESCRIPTION_GROUPE_DE_PAIE_HR
("description_groupes_de_paie_hr");

const QString YerothDatabaseTableColumn::DESCRIPTION_GROUPE_DEMPLOYES_HR
("description_groupe_demployes_hr");

const QString YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS
("description_programme_de_fidelite_clients");

const
QString YerothDatabaseTableColumn::DESCRIPTION_CLIENT("description_client");
const QString YerothDatabaseTableColumn::DESIGNATION("designation");
const QString YerothDatabaseTableColumn::DESIGNATION_REMISE("designation_remise");
const QString YerothDatabaseTableColumn::DESIGNATION_ALERTE("designation_alerte");
const QString YerothDatabaseTableColumn::CONDITION_ALERTE("condition_alerte");
const QString YerothDatabaseTableColumn::NOM_ENCAISSEUR("nom_encaisseur");
const QString YerothDatabaseTableColumn::DATE_PAIEMENT("date_paiement");

const QString YerothDatabaseTableColumn::MONTANT_PAYE("montant_paye");

const QString YerothDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL("MONTANT_A_PAYER_MENSUEL");

const QString YerothDatabaseTableColumn::DATE_VENTE("date_vente");
const QString YerothDatabaseTableColumn::DATE_DEBUT("date_debut");
const QString YerothDatabaseTableColumn::DATE_FIN("date_fin");
const QString YerothDatabaseTableColumn::DATE_SORTIE("date_sortie");
const QString YerothDatabaseTableColumn::HEURE_PAIEMENT("heure_paiement");
const QString YerothDatabaseTableColumn::HEURE_VENTE("heure_vente");
const QString YerothDatabaseTableColumn::HEURE_SORTIE("heure_sortie");
const QString YerothDatabaseTableColumn::DESTINATAIRE("destinataire");

const QString YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE("nom_complet_destinataire");

const QString YerothDatabaseTableColumn::REMISE_NOTES("remise_notes");

const QString YerothDatabaseTableColumn::MESSAGE_ALERTE("message_alerte");

const QString YerothDatabaseTableColumn::DATE_CREATION("date_creation");

const QString YerothDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE("type_doperation_financiere");

const QString YerothDatabaseTableColumn::TYPE_DE_PAIEMENT("type_de_paiement");

const QString YerothDatabaseTableColumn::TYPE_DE_VENTE("type_de_vente");

const QString YerothDatabaseTableColumn::NOTIFICATIONS("notifications");

const QString YerothDatabaseTableColumn::REMISE_RESOLUE("remise_resolue");

const QString YerothDatabaseTableColumn::ALERTE_RESOLUE("alerte_resolue");

const QString YerothDatabaseTableColumn::LOCALISATION_SORTIE("localisation_sortie");

const QString YerothDatabaseTableColumn::LOCALISATION_ENTREE("localisation_entree");

const QString YerothDatabaseTableColumn::LOCALISATION_SORTIE_ID("localisation_sortie_id");

const QString YerothDatabaseTableColumn::LOCALISATION_ENTREE_ID("localisation_entree_id");


void YerothDatabaseTableColumn::RETRANSLATE()
{
    _tableColumnToUserViewString.clear();

    _tableColumnToUserViewString
        .insert(YerothDatabaseTableColumn::MONTANT_RABAIS_PROGRAMME_DE_FIDELITE_CLIENTS,
                QObject::tr("Rabais FIDÉLITÉ CLIENTS (%1)")
                  .arg(YerothERPConfig::currency));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                                        QObject::tr("Service supplément (ajout commercial)"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REMISE_PRIX,
                                        QObject::tr("Remise (%1)").arg
                                        (YerothERPConfig::currency));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::IS_SERVICE,
                                        QObject::tr("Service"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::IS_VENTE_EN_GROS,
           QObject::tr("Vente en gros"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ID_ALERTE,
                                        QObject::tr("ID ALERTE"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ID,
                                        QObject::tr("ID"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFEREUR_CLIENT,
           QObject::tr("Référeur client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFEREUR_CLIENT_ID,
           QObject::tr("ID Référeur client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::GROUPES_DU_CLIENT,
           QObject::tr("Groupes du client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::GROUPES_DU_CLIENT_ID,
           QObject::tr("Groupes du client (ID)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::GROUPE_DE_PAIE_HR,
           QObject::tr("Groupe de paie (hr)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE,
           QObject::tr("Groupes d'1 employé"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::GROUPES_DUN_EMPLOYE_ID,
           QObject::tr("Groupes d'un employé (ID)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::HISTORIQUE_STOCK_ACTIF,
           QObject::tr("Historique ACTIF"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::HISTORIQUE_STOCK,
           QObject::tr("Historique stock"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MEMBRES_DU_GROUPE_db_ID,
           QObject::tr("Membres du groupe (leurs ID)"));

	_tableColumnToUserViewString.
	    insert(YerothDatabaseTableColumn::POURCENTAGE_TAXES_IMPOSABLES,
	       QObject::tr("TAXES IMPOSABLES (%)"));

	_tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::POURCENTAGE_DU_RABAIS,
           QObject::tr("Rabais (%)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MAXIMUM_DE_MEMBRES,
           QObject::tr("MAX membres"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::CLIENTS_ID,
                                        QObject::tr("ID client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_RECU_PAIEMENT_CLIENT,
           QObject::tr("Réf. reçu paiement"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_RECU_VENDU,
           QObject::tr("Réf. reçu vente"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_RECU_SORTIE,
           QObject::tr("Réf. reçu transfert"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::ENREGISTREUR_STOCK,
           QObject::tr("Gestionnaire stock"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::NOM_UTILISATEUR_DU_COMMANDEUR_DE_LACHAT,
     QObject::tr("Nom d'utilisateur du commandeur de l'achat"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::STATUT_DE_LACHAT_AU_FOURNISSEUR,
           QObject::tr("Statut achat fournisseur"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::memo_sale_for_sale,
                                        QObject::tr("MÉMO POUR RECHERCHE"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REFERENCE,
                                        QObject::tr("Référence"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::COMPTE_FOURNISSEUR,
           QObject::tr("Compte fournisseur"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::COMPTE_CLIENT_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Compte FIDÉLITÉ CLIENT"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::COMPTECLIENT_AVANCE_DE_CASH,
           QObject::tr("Compte client (CASH avancé)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::COMPTE_CLIENT,
           QObject::tr("Compte client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DETTE_MAXIMALE_COMPTE_CLIENT,
           QObject::tr("Dette maximale"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NUMERO_UNIQUE,
           QObject::tr("Numéro unique"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_DEPARTEMENT_PRODUIT,
           QObject::tr("Département"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::CATEGORIE,
                                        QObject::tr("Catégorie"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUANTITE,
                                        QObject::tr("Quantité"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::STOCK_MINIMUM,
           QObject::tr("Stock minimum"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::STOCK_MAXIMUM,
           QObject::tr("Stock maximum"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::STOCK_DALERTE,
           QObject::tr("Stock d'alerte"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::QUANTITE_VENDUE,
           QObject::tr("Qté"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::QUANTITE_TOTALE,
           QObject::tr("Qté totale"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::QUANTITE_SORTIE,
           QObject::tr("Qté sortie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_GROUPE,
           QObject::tr("Description groupe"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_FOURNISSEUR,
           QObject::tr("Description fournisseur"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_PRODUIT,
           QObject::tr("Description produit"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_UNITAIRE,
           QObject::tr("Prix unitaire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_UNITAIRE_EN_GROS,
           QObject::tr("Prix unitaire (en gros)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_RECU_DACHAT,
           QObject::tr("Référence achat"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PROGRAMME_DE_FIDELITE_CLIENTS,
           QObject::tr("Programme de fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_DACHAT_PRECEDENT,
           QObject::tr("Prix d'achat précédent"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRIX_DACHAT,
                                        QObject::tr("Prix d'achat"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_VENTE_PRECEDENT,
           QObject::tr("Prix vente précédent"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS_PRECEDENT,
           QObject::tr("P.V. (en gros) précédent"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRIX_VENTE,
                                        QObject::tr("Prix vente (TTC)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PRIX_VENTE_EN_GROS,
           QObject::tr("Prix vente TTC (en gros)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_DU_COMPTE_BANCAIRE,
           QObject::tr("Réf. compte bancaire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::AGENCE_DU_COMPTE_BANCAIRE,
           QObject::tr("Agence bancaire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::INTITULE_DU_COMPTE_BANCAIRE,
           QObject::tr("Intitulé compte bancaire"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::montant_RESTANT_de_la_ligne_budgetaire,
           QObject::tr("RESTE LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::montant_de_la_ligne_budgetaire,
           QObject::tr("MONTANT LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::INTITULE_DE_LA_ligne_budgetaire,
           QObject::tr("Intitulé ligne BUDGÉTAIRE"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::INSTITUT_BANCAIRE,
           QObject::tr("Institut compte bancaire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_DU_COMPTE_BANCAIRE,
           QObject::tr("Description compte bancaire"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::DESCRIPTION_charge_financiere,
				QObject::tr("Description CHARGE FINANCIÈRE"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::DESCRIPTION_ligne_budgetaire,
				QObject::tr("Description LIGNE BUDGÉTAIRE"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_DEPARTEMENT_PRODUIT,
           QObject::tr("Description dép. produit"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_CATEGORIE,
           QObject::tr("Description catégorie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::IS_SOUS_CATEGORIE,
           QObject::tr("EST 1 catégorie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_CATEGORIE,
           QObject::tr("Nom Catégorie"));

    _tableColumnToUserViewString
		.insert(YerothDatabaseTableColumn::LIGNE_BUDGETAIRE,
				QObject::tr("Ligne Budgétaire"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MONTANT_TVA,
                                        QObject::tr("Montant TVA"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_TVA_EN_GROS,
           QObject::tr("Montant TVA (en gros)"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::TVA,
                                        QObject::tr("TVA"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE,
           QObject::tr("Bénéfice"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MARGE_BENEFICIAIRE_EN_GROS,
           QObject::tr("Bénéfice (gros)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_RECU,
           QObject::tr("Montant reçu"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_A_REMBOURSER,
           QObject::tr("Montant à rembourser"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_TOTAL_VENTE,
           QObject::tr("Total vente"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::SERVICE_SUPPLEMENT_AJOUT_COMMERCIAL,
                                        QObject::tr("Service supplément (Ajout commercial)"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::REMISE_PRIX,
                                        QObject::tr("Remise prix"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REMISE_POURCENTAGE,
           QObject::tr("Remise (%)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION,
           QObject::tr("Localisation"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION_STOCK,
           QObject::tr("Localisation stock"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::QUANTITE_PAR_LOT,
           QObject::tr("Qté par lot"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOTS_ENTRANT,
           QObject::tr("Lots entrant"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::IMAGE_FOURNISSEUR,
           QObject::tr("Image fournisseur"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::IMAGE_COMPTE_CLIENT,
           QObject::tr("Image client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOGO_ENTREPRISE,
           QObject::tr("Logo de l'entreprise"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::IMAGE_PRODUIT,
           QObject::tr("Image produit"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_DE_DEBUT_DAPPARTENANCE,
           QObject::tr("Début d'appartenance"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_DE_FIN_DAPPARTENANCE,
           QObject::tr("Fin d'appartenance"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_PEREMPTION,
           QObject::tr("Date péremption"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_DE_RECEPTION,
           QObject::tr("Date réception"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_DE_COMMANDE,
           QObject::tr("Date commande"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_ENTREE,
                                        QObject::tr("Date entrée"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PRENOM,
                                        QObject::tr("Prénom"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM,
                                        QObject::tr("Nom"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_RECEPTEUR,
           QObject::tr("Nom récepteur"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_MAGASINIER,
           QObject::tr("Nom magasinier"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_UTILISATEUR_MAGASINIER,
           QObject::tr("ID magasinier"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_UTILISATEUR_CAISSIER,
           QObject::tr("ID caissier"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_CAISSIER,
           QObject::tr("Nom caissier"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_COMPLET_CREATEUR_REMISE_ALERTE,
           QObject::tr("Créateur"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_COMPLET,
                                        QObject::tr("Nom complet"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_LOCALISATION, QObject::tr("Nom"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MESSAGE_PDV_RECU_PETIT,
           QObject::tr("MESSAGE PDV REÇU PETIT"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_UTILISATEUR, QObject::tr("ID"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCKS_ID,
                                        QObject::tr("Stocks ID"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MANAGER, QObject::tr("Manager"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DEPARTEMENT, QObject::tr("Département empl."));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::POSITION_FONCTION, QObject::tr("Position/fonction"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::STOCK_MANAGER, QObject::tr("Stock manager"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::MOT_PASSE,
                                        QObject::tr("Mot de passe"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::TITRE,
                                        QObject::tr("Titre"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::APPELATION_TITRE,
           QObject::tr("Titre appelation"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ROLE,
                                        QObject::tr("rôle"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOM_ROLE,
                                        QObject::tr("nom rôle"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::EMAIL,
                                        QObject::tr("Émail"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_FOURNISSEUR,
           QObject::tr("Référence fourn."));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::REFERENCE_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Référence fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_GROUPE,
           QObject::tr("Référence groupe"));

    _tableColumnToUserViewString.
        insert(YerothDatabaseTableColumn::REFERENCE_GROUPE_DE_PAIE_hr,
               QObject::tr("Référence gr. de paie (HR)"));

    _tableColumnToUserViewString.
        insert(YerothDatabaseTableColumn::REFERENCE_GROUPE_DEMPLOYES_hr,
               QObject::tr("Référence gr. d'employés (HR)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_CLIENT,
           QObject::tr("Référence client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NUMERO_TELEPHONE_1,
           QObject::tr("Numéro téléphone 1"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NUMERO_TELEPHONE_2,
           QObject::tr("Numéro téléphone 2"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LIEU_NAISSANCE,
           QObject::tr("Lieu naissance"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_NAISSANCE,
           QObject::tr("Date naissance"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::QUARTIER_ENTREPRISE_CLIENT,
           QObject::tr("Quartier client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::VILLE_ENTREPRISE_CLIENT,
           QObject::tr("Ville client"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::VILLE,
                                        QObject::tr("Ville"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::QUARTIER,
                                        QObject::tr("Quartier"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REGION_ETAT_ENTREPRISE_CLIENT,
           QObject::tr("Région/État client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::PROVINCE_ETAT,
           QObject::tr("Province/État"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::PAYS,
                                        QObject::tr("Pays"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ADRESSE,
                                        QObject::tr("Adresse"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::ADRESSE_IP,
                                        QObject::tr("Adresse IP"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::ADRESSE_IP_RESEAU_IMPRIMANTE_THERMIQUE,
           QObject::tr("Adresse IP réseau imprimante ther."));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::BOITE_POSTALE,
           QObject::tr("Boîte postale"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::SIEGE_SOCIAL,
           QObject::tr("Siège social"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::NOTES,
                                        QObject::tr("Notes"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_ENTREPRISE,
           QObject::tr("Nom Entreprise"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::SECTEURS_DACTIVITES,
           QObject::tr("Secteurs d'activités"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REFERENCE_REGISTRE_DU_COMMERCE,
           QObject::tr("Régistre de commerce"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_nom_entreprise,
     QObject::tr("Nom de l'entreprise (carte)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_siege,
           QObject::tr("Siège (carte)"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::
     CARTE_DE_FIDELITE_CLIENT_service_de_gestion_clientele,
     QObject::tr("GESTION CLIENTÈLE (carte)"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::
     CARTE_DE_FIDELITE_CLIENT_service_du_programme_de_fidelite_client,
     QObject::tr("SERVICE DU PROGRAMME DE FIDÉLITÉ (carte)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_email,
           QObject::tr("Émail (carte)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::CARTE_DE_FIDELITE_CLIENT_telephone,
           QObject::tr("Téléphone (carte)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NUMERO_CONTRIBUABLE,
           QObject::tr("Numéro contribuable"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_REPRESENTANT,
           QObject::tr("Nom représentant"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_ENTREPRISE_CLIENT,
           QObject::tr("Nom client"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_ENTREPRISE_FOURNISSEUR,
           QObject::tr("Nom fournisseur"));

    _tableColumnToUserViewString.insert
    (YerothDatabaseTableColumn::DESCRIPTION_PROGRAMME_DE_FIDELITE_CLIENTS,
     QObject::tr("Description prog. de fidélité clients"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESCRIPTION_CLIENT,
           QObject::tr("Description client"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DESIGNATION,
                                        QObject::tr("Désignation"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESIGNATION_REMISE,
           QObject::tr("Désignation remise"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESIGNATION_ALERTE,
           QObject::tr("Désignation alerte"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::CONDITION_ALERTE,
           QObject::tr("Condition alerte"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_ENCAISSEUR,
           QObject::tr("Nom encaisseur"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_PAIEMENT,
           QObject::tr("Date paiement"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_A_PAYER_MENSUEL,
           QObject::tr("À PAYER (mensuel)"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MONTANT_PAYE,
           QObject::tr("Montant payé"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_VENTE,
                                        QObject::tr("Date vente"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_DEBUT,
                                        QObject::tr("Date début"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_FIN,
                                        QObject::tr("Date fin"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::DATE_SORTIE,
                                        QObject::tr("Date sortie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::HEURE_PAIEMENT,
           QObject::tr("Heure paiement"));

    _tableColumnToUserViewString.insert(YerothDatabaseTableColumn::HEURE_VENTE,
                                        QObject::tr("Heure vente"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::HEURE_SORTIE,
           QObject::tr("Heure sortie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DESTINATAIRE,
           QObject::tr("Destinataire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOM_COMPLET_DESTINATAIRE,
           QObject::tr("Nom complet destinataire"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REMISE_NOTES, QObject::tr("Notes"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::MESSAGE_ALERTE,
           QObject::tr("Message d'alerte"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::DATE_CREATION,
           QObject::tr("Date de création"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::TYPE_DOPERATION_FINANCIERE,
           QObject::tr("Type d'opération comptable"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::TYPE_DE_PAIEMENT,
           QObject::tr("Type de paiement"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::TYPE_DE_VENTE,
           QObject::tr("Type de vente"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::NOTIFICATIONS,
           QObject::tr("Notifications"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::REMISE_RESOLUE,
           QObject::tr("Remise résolue"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::ALERTE_RESOLUE,
           QObject::tr("Alerte résolue"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION_SORTIE,
           QObject::tr("Local. sortie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION_ENTREE,
           QObject::tr("Local. entrée"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION_SORTIE_ID,
           QObject::tr("ID local. sortie"));

    _tableColumnToUserViewString.
    insert(YerothDatabaseTableColumn::LOCALISATION_ENTREE_ID,
           QObject::tr("ID local. entrée"));

    //TODO: add an assertion to make sure that all keys, and values are unique
}
