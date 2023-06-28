create table stocks_vendu (id int primary key NOT NULL, stocks_id int NOT NULL, clients_id int, date_vente date NOT NULL, heure_vente time NOT NULL, nom_entreprise_client varchar(256), quartier_entreprise_client varchar(256), ville_entreprise_client varchar(256), region_etat_entreprise_client varchar(256), reference varchar(256), nom_departement_produit varchar(256) NOT NULL, categorie varchar(256) NOT NULL, designation varchar(256) NOT NULL, nom_entreprise_fournisseur varchar(256), prix_unitaire double NOT NULL, quantite_vendue double NOT NULL, montant_tva double NOT NULL, service_supplement_ajout_commercial double NOT NULL, remise_prix double NOT NULL, montant_rabais_programme_de_fidelite_clients double, remise_pourcentage double NOT NULL, montant_total_vente double NOT NULL, montant_recu double NOT NULL, montant_a_rembourser double NOT NULL, localisation varchar(256) NOT NULL, date_peremption varchar(256), nom_caissier varchar(256) NOT NULL, nom_utilisateur_caissier varchar(256) NOT NULL, type_de_vente int(12) NOT NULL, compte_client_PROGRAMME_DE_FIDELITE_CLIENTS double, compte_client_AVANCE_DE_CASH double, compte_client double, reference_recu_vendu varchar(256) NOT NULL, marge_beneficiaire double, localisation_stock varchar(6500), historique_stock varchar(6500), historique_stock_actif tinyint, is_vente_en_gros boolean NOT NULL, is_service boolean NOT NULL, memo_sale_for_sale varchar(256));
