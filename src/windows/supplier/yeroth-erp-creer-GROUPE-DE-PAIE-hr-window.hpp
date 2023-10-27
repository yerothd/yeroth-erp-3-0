/*
 * yeroth-erp-creer-UN-GROUPE-DE-PAIE-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_
#define YEROTH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_

#include "../../../ui_yeroth-erp-creer-GROUPE-DE-PAIE-hr-window.h"

#include <QtWidgets/QMessageBox>
#include "src/windows/yeroth-erp-window-commons.hpp"

class YerothERPWindows;
class YerothSqlTableModel;

class YerothCreerUnGroupeDePaieWindow : public YerothWindowsCommons,
    									private Ui_YerothCreerUnGroupeDePaieWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothCreerUnGroupeDePaieWindow();

    inline ~YerothCreerUnGroupeDePaieWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_YerothCreerGroupeDePaieWindow;
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
                                       QObject::tr("Remplisser le formulaire, et ensuite cliquer "
                                    		   	   "sur enregistrer !"));
    }

    void deconnecter_utilisateur();

protected:

    virtual void setupShortcuts();

private slots:

    void HANDLE_lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation
            (const QString &text_changed)
    {
    	lineEdit_creer_un_GroupeDePaie_MONTANT_A_PAYER_MENSUEL_visualisation
			->setText(GET_DOUBLE_STRING(text_changed.toDouble()));
    }

    void annuler_la_creation_dun_GROUPE_DE_PAIE_hr();

    bool creerEnregistrerUnGroupeDePaieHr();

private:

    bool payGroupAlreadyExists();

    bool check_fields();

    void clear_all_fields();

    void updatePopulateComboBoxes();

    inline void populateComboBoxes()
    {
        comboBox_creer_un_GroupeDePaie_localisation
			->populateComboBoxRawString(YerothDatabase::LOCALISATIONS,
										YerothDatabaseTableColumn::NOM_LOCALISATION);
    }

    virtual void setupLineEdits();

    int _last_selected_table_widget_row;

    YerothLogger *_logger;
};


#endif /* YEROTH_CREER_UN_GROUPE_DE_PAIE_WINDOW_HPP_ */
