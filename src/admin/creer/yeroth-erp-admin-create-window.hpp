/*
 * yeroth-erp-admin-create-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ADMIN_CREATE_WINDOW_HPP_
#define YEROTH_ADMIN_CREATE_WINDOW_HPP_

#include "../../../ui_yeroth-erp-admin-create-window.h"


#include "src/admin/yeroth-erp-admin-windows-commons.hpp"


#include <QtWidgets/QMessageBox>


class YerothERPWindows;
class YerothLogger;

class YerothAdminCreateWindow : public YerothPOSAdminWindowsCommons,
								private Ui_YerothAdminCreateWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS


	YerothAdminCreateWindow();

    inline ~YerothAdminCreateWindow()
    {
    }

    inline virtual QToolBar *getQMainWindowToolBar()
    {
        return toolBar_adminCreateWindow;
    }

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    virtual void rendreVisible(unsigned selectedSujetAction,
    						   bool 	a_show_item_CHARGE_FINANCIERE = false);

    virtual void rendreInvisible();


public slots:

    void showDestinataireNomComplet(const QString &
                                    destinataireId);

    void HANDLE_lineEdit_creer_montant_RESTANT_de_la_LIGNE_BUDGETAIRE(const QString &text_changed)
    {
    	lineEdit_creer_montant_RESTANT_de_la_LIGNE_BUDGETAIRE
			->setText(GET_DOUBLE_STRING(text_changed.toDouble()));
    }


    void showProduitInfo(const QString &nomProduit);

    void showDatePeremption(const QString &nomProduit);


    void lister();

    void annuler();


    void handleCurrentChanged();


    bool creer_utilisateur();

    bool creer_localisation();

    bool creer_departements_de_produits();

    bool creer_categorie();

    bool creer_charge_financiere();

    bool creer_ligne_budgetaire();

    bool creer_compte_bancaire();

    bool creer_remise();

    bool creer_alerte();


    void radioButtons_quantite();

    void radioButtons_periode_temps();


    inline virtual void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr
                                       ("Yeroth-pgi-3.0 ~ admin-créer"),
                                       QObject::tr
                                       ("Remplissez les informations nécessaire, et "
                                        "ensuite cliquer sur 'Valider'.\n"
                                        "Cliquez sur 'Annuler' si vous souhaiter annuler "
                                        "l'opération !"), QMessageBox::Ok);
    }


protected slots:

	void calculate_tva_value(const QString &VALEUR_TVA);

	void display_montant_prix_dachat_total(const QString &prix_dachat_alunite);

	void display_montant_prix_dachat_totalby_spinbox(double quantite);


protected:

    virtual void hideEvent(QHideEvent *hideEvent);


private:

    void show_item_CHARGE_FINANCIERE();

    void setupLineEdits();

    void setupDateTimeEdits();


    void populateUtilisateurComboBoxes();

    void populateAlerteComboBoxes();

    void populateLocalisationComboBoxes();

    void populate_CHARGE_FINANCIERE_ComboBoxes();

    void populate_LIGNE_BUDGETAIRE_ComboBoxes();

    void populateRemiseComboBoxes();


    void clear_utilisateur_all_fields();

    void clear_departements_de_produits_all_fields();

    void clear_categorie_all_fields();

    void clear_CHARGE_FINANCIERE_all_fields();

    void clear_LIGNE_BUDGETAIRE_all_fields();

    void clear_localisation_all_fields();

    void clear_alerte_all_fields();

    void clear_compte_bancaire_all_fields();

    void clear_remise_all_fields();


    bool creer_utilisateur_check_fields();

    bool creer_departements_de_produits_check_fields();

    bool creer_categorie_check_fields();

    bool creer_charge_financiere_CHECK_fields();

    bool creer_ligne_budgetaire_CHECK_fields();

    bool creer_localisation_check_fields();

    bool creer_alerte_check_fields();

    bool creer_compte_bancaire_check_fields();

    bool creer_remise_check_fields();


    void creer_utilisateur_check_fields_entry();

    void creer_charge_financiere_check_fields_entry();

    void creer_alerte_check_fields_entry();

    void creer_remise_check_fields_entry();


    double _VALEUR_TVA;

    double _MONTANT_TVA_CHARGE_FINANCIERE;


    double _montant_PRIX_DACHAT_TOTAL;



    YerothLogger *_logger;
};

#endif /* YEROTH_ADMIN_CREATE_WINDOW_HPP_ */
