create table configurations (id int primary key, nom_configuration varchar(256), description_configuration varchar(256), valeur_configuration varchar(256));
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (0, "ALERT_PERIOD_TIME_INTERVAL", "Intervalle de temps de vérification du daemon pour les alertes sur la période de temps (en secondes)", "20");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (1, "ALERT_QUANTITY_TIME_INTERVAL", "Intervalle de temps de vérification du daemon pour les alertes sur la quantité en stock (en secondes)", "10");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (2, "CONFIG_TVA_VALUE", "Valeur de la TVA", "7.25");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (3, "CONFIG_SALES_STRATEGY", "Configuration de la stratégie de vente/sortie", "FIFO");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (4, "CONFIG_CURRENCY", "Configuration de la devise", "$");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (5, "CONFIG_PAGINATION_PAGE_BEGIN_YEAR", "Configuration de l'annee de depart pour la pagination", "2022");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (6, "BACKUP_DATABASE_YEROTH_ERP_3_TIME_INTERVAL", "Intervalle de temps de SAUVEGARDE automatique de la base de donnees yeroth_erp_3 (en secondes)", "21600");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (7, "DIRECTORY_FULL_PATH_FOR_BACKUP_DATABASE_YEROTH_ERP_3", "Chemin complet du répertoire de SAUVEGARDE automatique de la base de donnees yeroth_erp_3", "/opt/yeroth-erp-3-0-sql-backup-restore");
insert into configurations (id, nom_configuration, description_configuration, valeur_configuration) values (8, "YEROTH_ERP_3_0_BINARY_FULL_PATH_FOR_BACKUP_RESTORE", "Chemin complet du fichier binaire de lancement de YEROTH-ERP-3.0", "/opt/yeroth-erp-3-0-standalone/bin/yeroth-erp-3-0-standalone");
