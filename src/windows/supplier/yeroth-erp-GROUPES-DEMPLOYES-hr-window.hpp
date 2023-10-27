/*
 * yeroth-erp-GROUPES-DEMPLOYES-hr-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ERP_groupes_demployes_hr_WINDOW_HPP_
#define YEROTH_ERP_groupes_demployes_hr_WINDOW_HPP_

#include "../../../ui_yeroth-erp-GROUPES-DEMPLOYES-hr-window.h"

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

class YerothERPGroupesDemployesHRWindow : public YerothWindowsCommons,
    									  private Ui_YerothERPGroupesDemployesHRWindow,
										  public YerothAbstractClassYerothSearchWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothERPGroupesDemployesHRWindow();

    virtual ~YerothERPGroupesDemployesHRWindow();

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_GROUPESDEMPLOYESWindow;
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

    virtual void rendreInvisible();

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

public slots:

    MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_GROUPESDEMPLOYES)

	virtual inline void apropos()
    {
        YerothWindowsCommons::apropos(this);
    }

    virtual inline void help()
    {
        YerothQMessageBox::information(this,
                                       QObject::tr("aide"),
                                       QObject::tr("1) Sélectionner un 'groupe de clients' dans le"
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

    void supprimer_PLUSIEURS_GROUPESDEMPLOYES(YerothSqlTableModel &
                                                aClientGroupTableModel);

    void supprimer_GROUPE_DEMPLOYES();

    void afficher_GROUPESDEMPLOYES(YerothSqlTableModel &
                                     aClientGroupTableModel);

    inline void afficher_GROUPESDEMPLOYES()
    {
        afficher_GROUPESDEMPLOYES(*_curEMPLOYEEGroupTableModel);
    }

    bool filtrer_GROUPESDEMPLOYES();

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

    void clear_all_fields();

    void populateComboBoxes();

    void setupLineEdits();

    void setupDateTimeEdits();

    QString _localStrategy;

    YerothLogger *_logger;

    QStringList _historiqueStock;

    QString _EmployeeGroupDateCreationFilter;

    QFont *_pushButton_filtrer_font;

    YerothSqlTableModel *_curEMPLOYEEGroupTableModel;
};

#endif /* YEROTH_ERP_groupes_demployes_hr_WINDOW_HPP_ */
