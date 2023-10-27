/*
 * yeroth-erp-groupes-dun-client-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_
#define SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_

#include "../../../ui_yeroth-erp-groupes-dun-client-window.h"


#include "src/windows/yeroth-erp-window-commons.hpp"


#include <QtCore/QDebug>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>


class YerothSqlTableModel;


class QContextMenuEvent;


class YerothGroupesDunClientWindow : public YerothWindowsCommons,
									 private Ui_YerothGroupesDunClientWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothGroupesDunClientWindow();

    virtual inline ~YerothGroupesDunClientWindow()
    {
        delete _logger;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_menuGroupesDunClientWindow;
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

    virtual void rendreVisible(YerothSqlTableModel *clientTableModel,
                               YerothSqlTableModel *stocksTableModel);

public slots:

    virtual bool export_csv_file();

    virtual bool imprimer_UNE_LISTE_DE_groupes_du_client_JUSQUAU_selectionne_WITH_A_YEROTH_PROGRESS_BAR()
    {
    }

	virtual bool imprimer_pdf_document();

	virtual void incrementFontSize__OF_TABLE();

	virtual void decrementFontSize__OF_TABLE();

    virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr
                                       ("Sélectionner un compte client dans le tableau "
                                        "des groupes d'1 client, ensuite "
                                        "cliquer sur l'opération que vous souhaitez réaliser !"));
    }

protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void setupShortcuts();

private slots:

    inline void ajouter_appartenance(const QString &
                                     un_groupe_de_clients)
    {
        YerothUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION(this,
                                                              &un_groupe_de_clients,
                                                              &YerothGroupesDunClientWindow::
                                                              executer_ajouter_appartenance);
    }

    void executer_ajouter_appartenance(const QString *un_groupe_de_clients);

    inline void retirer_ce_client_du_groupe_selectionne()
    {
        YerothUtils::run_FUNCTION_ROUNDED_WITH_DB_TRANSACTION_WITH_ROLLBACK(this,
                                                                            &YerothGroupesDunClientWindow::
                                                                            EXECUTER_retirer_ce_client_du_groupe_selectionne);
    }

    bool EXECUTER_retirer_ce_client_du_groupe_selectionne();

    void afficher_au_detail(const QModelIndex &modelIndex);

    inline void afficher_au_detail()
    {
        afficher_au_detail(tableWidget_groupes_dun_client->currentIndex());
    }

    void afficher_tous_les_groupes_du_client();


private slots:

    void enable_yeroth_widgets_ON_POSITIVE_QTABLE_WIDGET_ROW_COUNT();


private:

    void disable_yeroth_widgets();

    void get_PRINT_OUT_TexTableString(QString       &texTable_IN_OUT,
                                      int           row_MAX_TO_GO_export = -1);

    inline void setupLineEdits()
    {
    	lineEdit_groupes_dun_client_nombre_de_groupes->setYerothEnabled(false);
    }

    void populateComboBoxes();


    YerothLogger *_logger;

    YerothSqlTableModel *_curClientsTableModel;

    QString _curClient_NOM_ENTREPRISE;

    QString _curClientDBID;
};

#endif /* SRC_WINDOWS_YEROTH_ERP_GROUPE_DUN_CLIENT_WINDOW_HPP_ */
