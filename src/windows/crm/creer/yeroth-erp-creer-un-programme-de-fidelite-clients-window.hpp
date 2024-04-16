/*
 * yeroth-erp-creer-un-programme-de-fidelite-clients-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_
#define YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_

#include "../../../../ui_yeroth-erp-creer-un-programme-de-fidelite-clients-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerUnProgrammeDeFideliteClientsWindow : public YerothWindowsCommons,
    												  private Ui_YerothCreerUnProgrammeDeFideliteClientsWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothCreerUnProgrammeDeFideliteClientsWindow();

    inline ~YerothCreerUnProgrammeDeFideliteClientsWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerothCreerProgrammeDeFideliteClientsWindow;
    }

    virtual inline void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual inline void definirVendeur()
    {
        definirPasDeRole();
    }

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
                                       QObject::tr
                                       ("Remplisser le formulaire, et ensuite cliquer "
                                        "sur enregistrer !"));
    }

    void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private slots:

    void annuler_la_creation_dun_programme_de_fidelite_de_clients();

    bool creerEnregistrerUnProgrammeDeFideliteClients();

private:

    bool royaltyClientProgramAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    void updatePopulateComboBoxes();

    inline void populateComboBoxes()
    {
        comboBox_creer_un_programme_de_fidelite_clients_localisation
			->populateComboBoxRawString(YerothDatabase::LOCALISATIONS,
										YerothDatabaseTableColumn::NOM_LOCALISATION);
    }


    int _last_selected_table_widget_row;

    YerothLogger *_logger;
};


#endif /* YEROTH_CREER_UN_PROGRAMME_DE_FIDELITE_CLIENTS_WINDOW_HPP_ */
