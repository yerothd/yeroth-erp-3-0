/*
 * yeroth-erp-GROUPES-dun-employe-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_

#include "../../../ui_yeroth-erp-GROUPES-dun-employe-hr-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class YerothSqlTableModel;


class QContextMenuEvent;


class YerothGROUPES_DUN_EMPLOYE_Window : public YerothWindowsCommons,
    								 	 private Ui_YerothGROUPES_DUN_EMPLOYE_Window
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothGROUPES_DUN_EMPLOYE_Window();

    inline virtual ~YerothGROUPES_DUN_EMPLOYE_Window()
    {
        delete _logger;
    }

    inline virtual QToolBar *getQMainWindowToolBar()
    {
        return toolBar_menuGROUPES_DE_PAIE_DUN_EMPLOYE_Window;
    }

    inline virtual void definirCaissier()
    {
        definirPasDeRole();
    }

    virtual void definirManager();

    virtual void definirVendeur();

    inline virtual void definirGestionaireDesStocks()
    {
        definirPasDeRole();
    }

    inline virtual void definirMagasinier()
    {
        definirPasDeRole();
    }

    virtual void definirPasDeRole();

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
                               YerothSqlTableModel *stocksTableModel);


public slots:

	virtual bool imprimer_pdf_document();


	virtual void incrementFontSize__OF_TABLE();

	virtual void decrementFontSize__OF_TABLE();


    inline virtual void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    inline virtual void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un groupe d'employés dans le tableau "
                                        "des groupes d'1 employé, ensuite "
                                        "cliquer sur l'opération que vous souhaitez réaliser !"));
    }


    void handle_DATE_DEBUT_et_DATE_FIN_dappartenance(int row);


protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();


private slots:

    bool CREATE_A_NEW_PERIODE_DE_TEMPS_APPARTENANCE();


    void handle_activer_DATES_DAPPARTENANCE(int state);


    void handle_VALIDER_button();


    inline void handle_DATE_DEBUT_et_DATE_FIN_dappartenance(const QModelIndex &aQModelIndex)
    {
        handle_DATE_DEBUT_et_DATE_FIN_dappartenance(aQModelIndex.row());
    }


    inline void ajouter_appartenance(const QString &un_groupe_DEMPLOYES_hr)
    {
        YerothUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION
                        (this,
                         &un_groupe_DEMPLOYES_hr,
                         &YerothGROUPES_DUN_EMPLOYE_Window::executer_ajouter_appartenance);
    }


    void executer_ajouter_appartenance(const QString *un_groupe_DEMPLOYES_hr);


    inline void retirer_CET_EMPLOYE_du_groupe_selectionne()
    {
        YerothUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK
                        (this,
                         &YerothGROUPES_DUN_EMPLOYE_Window::EXECUTER_retirer_cet_employe_du_groupe_selectionne);
    }


    bool EXECUTER_retirer_cet_employe_du_groupe_selectionne();


    void afficher_au_detail(const QModelIndex &modelIndex);


    inline void afficher_au_detail()
    {
        afficher_au_detail(tableWidget_Groupes_Dun_Employe->currentIndex());
    }


    void afficher_tous_les_groupes_DUN_EMPLOYE_hr();


    void enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();


private:

    void disable_yeroth_widgets();

    void get_PRINT_OUT_TexTableString(QString &texTable_IN_OUT);

    inline void setupLineEdits()
    {
    	lineEdit_Groupes_Dun_Employe_nombre_de_groupes->setYerothEnabled(false);
    }

    void populateComboBoxes();


    YerothLogger *_logger;

    YerothSqlTableModel *_curEMPLOYEEgroups_TableModel;

    double  _somme_SALAIRE_CUMULE_A_DATE;

    QString _curEMPLOYEE_groupe_de_paie_hr;

    QString _curEMPLOYEE_NOM_ENTREPRISE;

    QString _curEmployeeDBID;

    QMap<QString, double> _map_GROUPE_SELECTIONNE__to__salaire;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_GROUPES_DE_PAIE_DUN_employe_WINDOW_HPP_ */
