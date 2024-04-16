/*
 * yeroth-erp-creer-groupe-de-clients-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_
#define YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-creer-groupe-de-clients-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerGroupeDeClientsWindow : public YerothWindowsCommons,
    									 private Ui_YerothCreerGroupeDeClientsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothCreerGroupeDeClientsWindow();

    inline ~YerothCreerGroupeDeClientsWindow()
    {
        delete _logger;
    }

    inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_creerGroupeDeClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    virtual inline void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    virtual inline void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("Remplisser le formulaire, et ensuite cliquer "
                                    		   	   "sur enregistrer !"));
    }

    void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private slots:

    inline void handle_select_table_widget_row(QTableWidgetItem *anItem)
    {
        _last_selected_table_widget_row = anItem->row();
    }

    bool IS_client_present_dans_un_groupe_de_clients(const QString &aClientGroupMemberName);

    void ajouter_un_membre_au_groupe_de_clients(const QString &aClientGroupMemberName);

    void annuler_la_creation_dun_groupe_de_clients();

    void supprimerUnMembreDunGroupeDeClients();

    bool creerEnregistrerUnGroupeDeClients();

private:

    void update_client_group_member_data(const QString &new_client_group_designation,
                                         const QString &new_client_group_db_ID,
                                         const QString &client_db_ID);

    inline void setLastSelectedTableWidgetRow(int lastSelectedTableWidgetRow)
    {
        _last_selected_table_widget_row = lastSelectedTableWidgetRow;
    }

    bool clientGroupAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    inline void populateComboBoxes()
    {
    	comboBox_creer_groupe_clients_programme_de_fidelite_de_clients
        	->populateComboBoxRawString(YerothDatabase::PROGRAMMES_DE_FIDELITE_CLIENTS,
                                  	    YerothDatabaseTableColumn::DESIGNATION);
    }

    virtual inline void setupLineEdits()
    {
        lineEdit_creer_groupe_clients_maximum_de_membres
			->setValidator(&YerothUtils::UintValidator);
    }

    void setupLineEditsQCompleters();


    int _last_selected_table_widget_row;

    YerothLogger *_logger;
};


#endif /* YEROTH_CREER_GROUPE_DE_CLIENTS_WINDOW_HPP_ */
