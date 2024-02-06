/**
 * yeroth-erp-admin-lister-window.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef YEROTH_ADMIN_LISTER_WINDOW_HPP_
#define YEROTH_ADMIN_LISTER_WINDOW_HPP_

#include "../../../ui_yeroth-erp-admin-lister-window.h"


#include "src/widgets/table-view/yeroth-erp-table-view-with-pagination.hpp"


#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/admin/admin-actions-subjects.hpp"


#include <QtWidgets/QMessageBox>


class QStandardItemModel;

class YerothERPWindows;
class YerothUtils;
class YerothLogger;
class YerothAdminSearchForm;

class YerothAdminListerWindow : public YerothPOSAdminWindowsCommons,
    							private Ui_YerothAdminListerWindow
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

	YerothAdminListerWindow();

    ~YerothAdminListerWindow();

    virtual inline QTabWidget *get_tabwidget_lister()
    {
    	return tabWidget_lister;
    }

    virtual inline QToolBar *getQMainWindowToolBar()
    {
        return toolBar_adminListerWindow;
    }

    virtual void definirPasDeRole();

    virtual void definirAdministrateur();

    virtual void definirManager();

    void self_reset_view(int currentIndex);


    MACRO_TO_DEFINE_VIEWING_PAGE_NUMBER_FOR_TABLEVIEW(label_numero_page_derniere,
    												  label_numero_page_courante)


    void rendreVisible(unsigned selectedSujetAction);


    inline void setCurSearchSqlTableModel(YerothSqlTableModel *aSqlTableModel)
    {
        _curSearchSqlTableModel = aSqlTableModel;
    }


    inline YerothSqlTableModel *getCurSearchSqlTableModel()
    {
        return _curSearchSqlTableModel;
    }


public slots:

	MACRO_TO_DEFINE_VIEWING_POINTERS_PAGE_SLOTS(tableView_lister_categorie)

    virtual void imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR();

    virtual inline void imprimer_pdf_document_WITH_A_YEROTH_PROGRESS_BAR(int pageFROM,
                                                                         int pageTO)
    {
        YerothProgressBar ((YerothWindowsCommons *) this,
                            QString("rgb(%1)").arg(COLOUR_RGB_STRING_YEROTH_WHITE_255_255_255))
                          ((YerothWindowsCommons *) this,
                            &pageFROM,
                            &pageTO,
                            &YerothWindowsCommons::imprimer_pdf_document_WITH_PAGES_SPECIFICATION);
    }


    virtual inline int current_tabWidget_index()
    {
        return tabWidget_lister->currentIndex();
    }


    void handleCurrentChanged(int index);


    void creer();


    void modifier();


	int LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(YerothTableViewWITHpagination  &a_table_view_to_list_show,
							  	  	  	  	 YerothSqlTableModel 			&aSqlTableModel);

	virtual YerothTableViewWITHpagination *GET_CURRENT_TABLEVIEW();


	void RESET_VIEW_CHARGES_FINANCIERES();

    void RESET_VIEW_utilisateur();

    void RESET_VIEW_localisation();

    void RESET_VIEW_departements_de_produits();

    void RESET_VIEW_categorie();

    void RESET_VIEW_LIGNE_BUDGETAIRE();

    void RESET_VIEW_compte_bancaire();

    void RESET_VIEW_alerte();

    void RESET_VIEW_remise();


	virtual void SETUP_PRINT();


	void LISTER_CHARGES_FINANCIERES(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_utilisateur(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_localisation(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_departements_de_produits(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_categorie(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_LIGNE_BUDGETAIRE(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_compte_bancaire(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_alerte(YerothSqlTableModel *aSqlTableModel = 0);

    void lister_remise(YerothSqlTableModel *aSqlTableModel = 0);


    void afficher_au_detail();


    void afficher_detail_CHARGE_FINANCIERE();

    void afficher_detail_utilisateur();

    void afficher_detail_localisation();

    void afficher_detail_departements_de_produits();

    void afficher_detail_categorie();

    void afficher_detail_LIGNE_BUDGETAIRE();

    void afficher_detail_compte_bancaire();

    void afficher_detail_remise();

    void afficher_detail_alerte();


    void supprimer();


    void supprimer_utilisateur();

    void supprimer_localisation();

    void supprimer_categorie();

    void supprimer_LIGNE_BUDGETAIRE();

    void supprimer_compte_bancaire();

    void supprimer_departement_de_produit();

    void supprimer_remise();

    void supprimer_alerte();


    virtual QString get_latex_template_print_pdf_content();


    inline void annuler()
    {
        YerothPOSAdminWindowsCommons::menu();
    }


    void rechercher();


    void reinitialiser();


    void set_admin_rechercher_font();


    inline int lastSelectedItemForModification()
    {
        return _lastItemSelectedForModification;
    }


    void handleItemModification(const QModelIndex &index);


    inline bool isAlertCurrentlyFiltered() const
    {
        return _alertCurrentlyFiltered;
    }

    inline bool isProductDepartmentCurrentlyFiltered() const
    {
        return _productDepartmentCurrentlyFiltered;
    }

    inline bool isCategoryCurrentlyFiltered() const
    {
        return _categoryCurrentlyFiltered;
    }

    inline bool is_LIGNE_BUDGETAIRE_CurrentlyFiltered() const
    {
        return _LIGNE_BUDGETAIRE_CurrentlyFiltered;
    }

    inline bool isBankAccountCurrentlyFiltered() const
    {
        return _bankAccountCurrentlyFiltered;
    }

    inline bool isSiteCurrentlyFiltered() const
    {
        return _siteCurrentlyFiltered;
    }

    inline bool isUserCurrentlyFiltered() const
    {
        return _userCurrentlyFiltered;
    }

    inline bool isCharge_financieres_CurrentlyFiltered() const
    {
        return _charges_financieres_CurrentlyFiltered;
    }

    inline bool isDiscountCurrentlyFiltered() const
    {
        return _discountCurrentlyFiltered;
    }

    inline void setDiscountCurrentlyFiltered(bool discountCurrentlyFiltered)
    {
        _discountCurrentlyFiltered = discountCurrentlyFiltered;
    }

    inline void setAlertCurrentlyFiltered(bool alertCurrentlyFiltered)
    {
        _alertCurrentlyFiltered = alertCurrentlyFiltered;
    }

    inline void setProductDepartmentCurrentlyFiltered(bool productDepartmentCurrentlyFiltered)
    {
        _productDepartmentCurrentlyFiltered = productDepartmentCurrentlyFiltered;
    }

    inline void setCategoryCurrentlyFiltered(bool categoryCurrentlyFiltered)
    {
        _categoryCurrentlyFiltered = categoryCurrentlyFiltered;
    }

    inline void setLIGNE_BUDGETAIRE_CurrentlyFiltered(bool LIGNE_BUDGETAIRE_CurrentlyFiltered)
    {
    	_LIGNE_BUDGETAIRE_CurrentlyFiltered = LIGNE_BUDGETAIRE_CurrentlyFiltered;
    }

    inline void setBankAccountCurrentlyFiltered(bool bankAccountCurrentlyFiltered)
    {
        _bankAccountCurrentlyFiltered = bankAccountCurrentlyFiltered;
    }

    inline void setSiteCurrentlyFiltered(bool siteCurrentlyFiltered)
    {
        _siteCurrentlyFiltered = siteCurrentlyFiltered;
    }

    inline void setCharge_financieres_CurrentlyFiltered(bool charges_financieres_CurrentlyFiltered)
    {
    	_charges_financieres_CurrentlyFiltered = charges_financieres_CurrentlyFiltered;
    }

    inline void setUserCurrentlyFiltered(bool userCurrentlyFiltered)
    {
        _userCurrentlyFiltered = userCurrentlyFiltered;
    }


protected:

    virtual void contextMenuEvent(QContextMenuEvent *event);


private:

    bool _alertCurrentlyFiltered;

    bool _productDepartmentCurrentlyFiltered;

    bool _categoryCurrentlyFiltered;

    bool _LIGNE_BUDGETAIRE_CurrentlyFiltered;

    bool _bankAccountCurrentlyFiltered;

    bool _userCurrentlyFiltered;

    bool _siteCurrentlyFiltered;

    bool _charges_financieres_CurrentlyFiltered;

    bool _discountCurrentlyFiltered;

    QMap<QString, QString> _LISTER_tab_TO_tabTitle;

    QFont *_pushButton_admin_rechercher_font;

    YerothLogger *_logger;

    YerothAdminSearchForm *_adminSearchForm;

    YerothSqlTableModel *_curSearchSqlTableModel;

    int _lastItemSelectedForModification;
};


#define MACRO_SET_ADMIN_RECHERCHER_FONT(X) 						\
{																\
	if (true == X)												\
	{															\
		_pushButton_admin_rechercher_font->setUnderline(true);	\
	}															\
	else														\
	{															\
		_pushButton_admin_rechercher_font->setUnderline(false);	\
	}															\
}

#endif /* YEROTH_ADMIN_LISTER_WINDOW_HPP_ */
