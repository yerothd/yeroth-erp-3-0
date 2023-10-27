/*
 * yeroth-erp-GROUPES-DE-PAIE-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_groupes_de_paie_WINDOW_HPP_
#define YEROTH_ERP_groupes_de_paie_WINDOW_HPP_

#include "../../../ui_yeroth-erp-GROUPES-DE-PAIE-hr-window.h"

#include <QtCore/QDebug>

#include <QtCore/QVector>

#include <QtWidgets/QMessageBox>

#include <QtGui/QContextMenuEvent>

#include "src/windows/yeroth-erp-window-commons.hpp"


class YerothPOSDialogCommons;
class QStandardItemModel;
class QContextMenuEvent;
class QProcess;

class YerothSqlTableModel;

class YerothERPGROUPES_DE_PAIE_Window : public YerothWindowsCommons,
    									private Ui_YerothERPGROUPES_DE_PAIE_Window,
										public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothERPGROUPES_DE_PAIE_Window();

    virtual ~YerothERPGROUPES_DE_PAIE_Window();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_GROUPES_DE_PAIE_Window;
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

    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_numero_page_derniere,
    												  label_numero_page_courante)

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_groupes_de_paie)

	virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("1) Sélectionner un 'programme de fidélité clients' dans le"
                                    		   	   " tableau de cette page, ensuite cliquer sur"
                                    		   	   " l'opération que vous souhaitez"
                                    		   	   " réaliser !"));
    }

    bool export_csv_file();

    virtual QString get_latex_template_print_pdf_content();

    void afficher_au_detail();

    inline void afficher_au_detail(const QModelIndex &modelIndex)
    {
        afficher_au_detail();
    }

    void refineYerothLineEdits();

    bool filtrer();

    void reinitialiser_elements_filtrage();

    void reinitialiser_recherche();

    void set_filtrer_font();

private:

    void initialiser_FILTRES();

private slots:

    void handle_filtres_checkBox(int state);

    void supprimer_PLUSIEURS_GROUPES_DE_PAIE_hr(YerothSqlTableModel &EMPLOYEE_pay_group_TableModel);

    void supprimer_un_GROUPE_DE_PAIE_hr();

    void afficher_GROUPES_DE_PAIE_hr(YerothSqlTableModel &EMPLOYEE_pay_group_TableModel);

    inline void afficher_GROUPES_DE_PAIE_hr()
    {
        afficher_GROUPES_DE_PAIE_hr(*_cur_PAY_GROUP_TableModel);
    }

    bool filtrer_groupes_de_paie_hr();

protected:

    virtual void reinitialiser_colones_db_visibles();

    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual inline void hideEvent(QHideEvent *hideEvent)
    {
        YerothWindowsCommons::CLOSE_WINDOW_COMMON_FOR_SEARCH_CONDITION_FILTERING_PURPOSE();
    }

    virtual void setupShortcuts();

    void resetFilter();

protected slots:

    virtual void slot_reinitialiser_colones_db_visibles();

    virtual void textChangedSearchLineEditsQCompleters();

private:

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString _localStrategy;

    YerothLogger *_logger;

    QStringList _historiqueStock;

    QString _EMPLOYEE_pay_group_DateCreationFilter;

    QFont *_pushButton_filtrer_font;

    YerothSqlTableModel *_cur_PAY_GROUP_TableModel;
};

#endif /* YEROTH_ERP_groupes_de_paie_WINDOW_HPP_ */
