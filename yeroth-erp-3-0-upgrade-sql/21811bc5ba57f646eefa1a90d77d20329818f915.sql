alter table fournisseurs ADD column groupes_de_lemploye varchar(3000) AFTER position_fonction;
alter table fournisseurs ADD column groupes_de_lemploye_ID varchar(5000) AFTER groupes_de_lemploye;
