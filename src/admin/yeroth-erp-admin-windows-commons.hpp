/*
 * yeroth-erp-admin-windows-commons.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_
#define SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/windows/yeroth-erp-window-commons.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/widgets/yeroth-erp-qmessage-box.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>

class YerothERPWindows;

class YerothPOSAdminWindowsCommons : public YerothWindowsCommons
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS

    inline YerothPOSAdminWindowsCommons(QString windowName)
    :_curStocksTableModel(0),
	 _windowName_WITH_NO_MAINTENANCE(windowName),
     _windowName(windowName)
    {
    }

    ~YerothPOSAdminWindowsCommons();

    inline virtual QString qMessageBoxStyleSheet()
    {
        return QMESSAGE_BOX_STYLE_SHEET;
    }

    inline QString getWindowName()
    {
        return _windowName;
    }

    virtual QToolBar *getQMainWindowToolBar() = 0;

    inline virtual void YEROTH_ERP_ADMIN_WRAPPER_QACTION_SET_ENABLED(QAction 	*anAction,
                                                                     bool		aBooleanValue)
    {
        anAction->setVisible(aBooleanValue);
    }

    inline static void setCenterPosition(QPoint *desktopCenterPosition)
    {
        _centerPosition = desktopCenterPosition;
    }

    inline static void setAllWindows(YerothERPWindows *allWindows)
    {
        _allWindows = allWindows;
    }

    inline virtual void definirCaissier()
    {
    }

    inline virtual void definirManager()
    {
    }

    inline virtual void definirVendeur()
    {
    }

    inline virtual void definirGestionaireDesStocks()
    {
    }

    inline virtual void definirMagasinier()
    {
    }

    inline virtual void definirPasDeRole()
    {
    }

    virtual void rendreVisible(YerothSqlTableModel *stocksTableModel);

    inline virtual void rendreInvisible()
    {
        setVisible(false);
    }

    inline static YerothERPWindows *getAllWindows()
    {
        return _allWindows;
    }

public slots:

    virtual void menu();

    virtual void retour_menu_principal();

    virtual void changer_utilisateur();

    virtual void deconnecter_utilisateur();

    inline virtual YerothSqlTableModel *getStocksTableModel()
    {
        return _curStocksTableModel;
    }

    inline virtual void fermeture()
    {
        close();
    }

    inline virtual void apropos()
    {
        YerothQMessageBox::about(this,
        						 YerothUtils::APPLICATION_NAME,
                                 YerothUtils::APROPOS_MSG);
    }

    virtual void CLOSE_SELECT_EXPORT_DB_DIALOG();

	virtual void selectionner_champs_db_visibles(bool SETUP_DIALOG_BOX_GEOMETRY = true,
												 bool CHARGES_FINANCIERES_WINDOW = false);

    virtual void setup_select_configure_dbcolumn(const QString &aSqlTableName);

    virtual void infosEntreprise();

    virtual void qui_suis_je();

    inline virtual void help()
    {
    }

    void getManuelUtilisateurPDF();

protected:

    void mySetupUi(QMainWindow *aWindow);

    static YerothERPWindows *_allWindows;

    YerothSqlTableModel *_curStocksTableModel;

    QString QMESSAGE_BOX_STYLE_SHEET;

    QString _windowName_WITH_NO_MAINTENANCE;

    QString _windowName;

    QMap<QString, YerothERPGenericSelectDBFieldDialog *> sqlTableName__TO__selectDBFieldDialog;

    static QPoint *_centerPosition;
};

#endif /* SRC_YEROTH_ERP_ADMIN_WINDOWS_COMMONS_HPP_ */
