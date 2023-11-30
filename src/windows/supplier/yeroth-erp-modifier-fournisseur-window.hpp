/*
 * yeroth-erp-modifier-fournisseur.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_

#include "../../../ui_yeroth-erp-modifier-fournisseur-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtWidgets/QMessageBox>


class QContextMenuEvent;

class YerothSqlTableModel;


class YerothModifierFournisseurWindow : public YerothWindowsCommons,
    									private Ui_YerothModifierFournisseurWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothModifierFournisseurWindow();

    inline ~YerothModifierFournisseurWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_modifierFournisseurWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *fournisseurTableModel,
                       YerothSqlTableModel *stocksTableModel,
                       bool 			   show_TabDESCRIPTION_SUPPLIER = false,
					   bool 			   show_EMPLOYE = false);

public slots:

    virtual void deconnecter_utilisateur();

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Modifier les informations sur le compte fournisseur, "
                                        "et ensuite cliquer sur le bouton 'actualiser'!"));
    }

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private slots:

    inline void selectionner_image_produit()
    {
        YerothWindowsCommons::selectionner_image(*label_image_produit);
    }

    void supprimer_image_fournisseur();

    bool customerAccountAlreadyExist();

    void annuler();

    void actualiserFournisseur();

    void supprimerFournisseur();

private:

    bool modifier_fournisseur_check_fields();

    void clear_all_fields();

    void showFournisseurDetail(bool show_EMPLOYE = false);


    inline void setupLineEdits()
    {
        lineEdit_modifier_fournisseur_reference_fournisseur->setYerothEnabled
        (false);
    }


    YerothLogger 		*_logger;

    YerothSqlTableModel *_curFournisseurTableModel;

    bool 				_SHOW_EMPLOYEE;

    int 				_curFournisseurDetailDBID;
};


#endif /* SRC_WINDOWS_YEROTH_MODIFIER_FOURNISSEUR_WINDOW_HPP_ */
