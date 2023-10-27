/*
 * yeroth-erp-modifier-compte-client.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_MODIFIER_COMPTE_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_MODIFIER_COMPTE_CLIENT_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-modifier-compte-client-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class QContextMenuEvent;

class YerothSqlTableModel;

class YerothModifierCompteClientWindow : public YerothWindowsCommons,
    									 private Ui_YerothModifierCompteClientWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothModifierCompteClientWindow();

    inline ~YerothModifierCompteClientWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_modifierCompteClientWindow;
    }

    virtual void definirCaissier();

    virtual void definirManager();

    virtual void definirVendeur();

    virtual void definirGestionaireDesStocks();

    virtual void definirMagasinier();

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    void rendreVisible(YerothSqlTableModel *clientTableModel,
                       YerothSqlTableModel *stocksTableModel,
                       bool 			   show_TabDESCRIPTION_CLIENT = false);

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
                                       QObject::tr("Modifier les informations sur le compte client, "
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

    void supprimer_image_compte_client();

    bool customerAccountAlreadyExist();

    bool INSERT_IF_NEEDED_CLIENT_AVANCE_DE_CASH(double current_client_account_AVANCE_DE_CASH);

    void annuler();

    void actualiserCompteClient();

    void supprimerCompteClient();

private:

    void setupLineEdits();

    bool modifier_client_check_fields();

    void clear_all_fields();

    void showClientDetail();


    YerothLogger *_logger;

    YerothSqlTableModel *_curClientTableModel;

    double _CLIENT_PREVIOUS_AVANCE_DE_CASH;

    int _curClientDetailDBID;
};


#endif /* SRC_WINDOWS_YEROTH_MODIFIER_COMPTE_CLIENT_WINDOW_HPP_ */
