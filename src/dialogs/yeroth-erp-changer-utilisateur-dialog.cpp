/*
 * yeroth-erp-dialog-changer-utilisateur.cpp
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "src/yeroth-erp-windows.hpp"


#include "src/windows/yeroth-erp-main-window.hpp"

#include "yeroth-erp-changer-utilisateur-dialog.hpp"

#include "src/users/yeroth-erp-users.hpp"

#include "src/users/yeroth-erp-user-manager.hpp"

#include "src/users/yeroth-erp-user-vendeur.hpp"

#include "src/users/yeroth-erp-user-gestionaire-des-stocks.hpp"

#include "src/users/yeroth-erp-user-magasinier.hpp"

#include "src/users/yeroth-erp-user-caissier.hpp"

#include "src/users/yeroth-erp-user-administrateur.hpp"


#include <QtWidgets/QDesktopWidget>

#include <QtWidgets/QMainWindow>

#include <QtSql/QSqlQuery>

#include <QtSql/QSqlRecord>

#include <QtSql/QSqlError>



const QString
YerothPOSChangerUtilisateurDialog::

DIALOG_WINDOW_TITLE(QString("%1 - %2").arg
                    (GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
                     QObject::tr("changer d'utilisateur")));

YerothPOSChangerUtilisateurDialog::YerothPOSChangerUtilisateurDialog(YerothERPWindows *allWindows,
																	 QMainWindow *parent)
:YerothPOSDialogCommons(YerothPOSChangerUtilisateurDialog::DIALOG_WINDOW_TITLE),
 _currentPosition(new QPoint),
 _push_button_password(0),
 _user(0)
{
    setupUi(this);

    QDesktopWidget &desktopWidget = _allWindows->desktopWidget();

    YerothUtils::getCenterPosition(desktopWidget.width(),
                                   desktopWidget.height(),
                                   *this, *_currentPosition);

    lineEdit_mot_passe->setEchoMode(QLineEdit::Password);

    setFixedSize(width(), height());

    connect(pushButton_annuler, SIGNAL(clicked()), this, SLOT(annuler()));
    connect(pushButton_valider, SIGNAL(clicked()), this, SLOT(valider()));


    _push_button_password = new YerothPushButtonPASSWORD(this,
                                                         lineEdit_mot_passe,
                                                         pushButton_cacher_le_mot_de_passe,
                                                         pushButton_voir_le_mot_de_passe);

    comboBox_company_site->setYerothEditable(true);
}


YerothPOSChangerUtilisateurDialog::~YerothPOSChangerUtilisateurDialog()
{
    delete _currentPosition;

    YEROTH_DELETE_FREE_POINTER_NOW(_push_button_password);

    if (_user)
    {
        delete _user;

        _user = 0;
    }
}

void YerothPOSChangerUtilisateurDialog::annuler()
{
    if (0 != _push_button_password)
    {
        _push_button_password->cacher_le_mot_de_passe();
    }

    close();
}


void YerothPOSChangerUtilisateurDialog::valider()
{
    //qDebug() << "DepotFacileDialogChangerUtilisateur::valider()";
    QString mot_passe(lineEdit_mot_passe->text());

    QString nom_utilisateur(lineEdit_nom_utilisateur->text().toLower());

    if (!mot_passe.isEmpty())
    {
        mot_passe.append(nom_utilisateur);

        YerothSqlTableModel &usersSqlTableModel =
                        _allWindows->getSqlTableModel_users();

        QString searchUserFilter;

        QByteArray md5Hash_mot_passe(MD5_HASH(mot_passe));

        searchUserFilter.append(GENERATE_SQL_IS_STMT
                                (YerothDatabaseTableColumn::NOM_UTILISATEUR,
                                 nom_utilisateur));

        usersSqlTableModel.yerothSetFilter_WITH_where_clause(searchUserFilter);

        int usersSqlTableModelRowCount =
        		usersSqlTableModel.easySelect("src/dialogs/yeroth-erp-changer-utilisateur-window.cpp", 123);

        if (usersSqlTableModelRowCount > 0)
        {
            QSqlRecord userRecord = usersSqlTableModel.record(0);
            QByteArray md5Hash(userRecord.value("mot_passe").toByteArray());

            if (md5Hash != md5Hash_mot_passe)
            {
                YerothQMessageBox::information(_allWindows->_mainWindow,
                                               QObject::tr
                                               ("enregistrement de l'utilisateur"),
                                               QObject::tr
                                               ("Votre nom d'utilisateur "
                                                "ou bien votre mot de passe "
                                                "est incorrect !"));
                return;
            }

            int role = GET_SQL_RECORD_DATA(userRecord,
                                           YerothDatabaseTableColumn::
                                           ROLE).toInt();

            YerothPOSUser *user = createUser(userRecord, role);

            if (!user)
            {
                YerothQMessageBox::information(_allWindows->_mainWindow,
                                               QObject::tr
                                               ("enregistrement de l'utilisateur"),
                                               QObject::tr
                                               ("Votre nom d'utilisateur "
                                                "ou bien votre mot de passe "
                                                "est incorrect !"));
                return;
            }

            if (_user)
            {
                delete _user;
            }

            _user = user;

            //Check if the user has new alerts.
            checkCourriersAlertes();

            _allWindows->CURRENTLY_CHANGING_USER = true;

            _allWindows->_mainWindow->cleanupAllOtherWindows();

            _allWindows->CURRENTLY_CHANGING_USER = false;

            _allWindows->setUser(user);

            if (YerothUtils::ROLE_ADMINISTRATEUR == role)
            {
                _allWindows->_mainWindow->rendreInvisible();
                _allWindows->_mainWindow->definirAdministrateur();
                _allWindows->_adminWindow->rendreVisible(&_allWindows->
                                                         getSqlTableModel_stocks
                                                         ());
            }
            else if (YerothUtils::ROLE_CAISSIER == role)
            {
                _allWindows->_mainWindow->rendreInvisible();
                _allWindows->_ventesWindow->resetFilter(&_allWindows->
                                                        getSqlTableModel_stocks_vendu
                                                        ());
                _allWindows->_pdVenteWindow->rendreVisible(&_allWindows->
                                                           getSqlTableModel_stocks
                                                           ());
            }
            else if (YerothUtils::ROLE_MAGASINIER == role)
            {
                _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                                        getSqlTableModel_stocks
                                                        ());
            }
            else if (YerothUtils::ROLE_VENDEUR == role)
            {
                _allWindows->_ventesWindow->resetFilter(&_allWindows->
                                                        getSqlTableModel_stocks_vendu
                                                        ());
                _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                                        getSqlTableModel_stocks
                                                        ());
            }
            else if (YerothUtils::ROLE_GESTIONNAIREDESTOCK == role)
            {
                _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                                        getSqlTableModel_stocks
                                                        ());
            }
            else			//YerothUtils::ROLE_MANAGER
            {
                //qDebug() << "++ YerothUtils::ROLE_MANAGER";
                //qDebug() << "++ _allWindows->_caisseWindow: " << _allWindows->_caisseWindow;
                _allWindows->_ventesWindow->resetFilter(&_allWindows->
                                                        getSqlTableModel_stocks_vendu
                                                        ());

                if (!comboBox_company_site->isEmpty())
                {
                	YerothUtils::slot_connecter_localisation(*this,
                                                         	 _allWindows,
															 comboBox_company_site->currentText());
                }

                //qDebug() << "++ YerothUtils::ROLE_MANAGER, _allWindows->_caisseWindow->resetFilter()";
                _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                                        getSqlTableModel_stocks
                                                        ());
            }

            close();
        }
        else
        {
            YerothQMessageBox::information(_allWindows->_mainWindow,
                                           QObject::tr
                                           ("enregistrement de l'utilisateur"),
                                           QObject::tr
                                           ("Votre nom d'utilisateur "
                                            "ou bien votre mot de passe "
                                            "est incorrect !"));
            return;
        }
    }
    else
    {
        YerothQMessageBox::information(_allWindows->_mainWindow,
                                       QObject::tr
                                       ("enregistrement de l'utilisateur"),
                                       QObject::tr("Votre nom d'utilisateur "
                                                   "ou bien votre mot de passe "
                                                   "est incorrect !"));
    }
}


YerothPOSUser *YerothPOSChangerUtilisateurDialog::createUser(QSqlRecord &userRecord,
                                                             int role)
{
    YerothPOSUser *user = 0;

    if (YerothUtils::ROLE_MANAGER == role)
    {
        user = new YerothPOSUserManager(_allWindows);
    }
    if (YerothUtils::ROLE_VENDEUR == role)
    {
        user = new YerothERPUserVendeur(_allWindows);
    }
    if (YerothUtils::ROLE_GESTIONNAIREDESTOCK == role)
    {
        user = new YerothPOSUserGestionaireDesStocks(_allWindows);
    }
    else if (YerothUtils::ROLE_MAGASINIER == role)
    {
        user = new YerothPOSUserMagasinier(_allWindows);
    }
    else if (YerothUtils::ROLE_CAISSIER == role)
    {
        user = new YerothPOSUserCaissier(_allWindows);
    }
    else if (YerothUtils::ROLE_ADMINISTRATEUR == role)
    {

#ifdef YEROTH_CLIENT
        QString retMsg(QObject::tr
                       ("La version cliente de YEROTH ne vous donne pas accès à l'administration !"));

        QMessageBox::information(this,
                                 QObject::tr
                                 ("pas d'accès à l'administration"), retMsg);
        return 0;
#endif

        user = new YerothPOSUserAdministrateur(_allWindows);
    }

    if (0 != user)
    {
        user->set_nom_utilisateur(GET_SQL_RECORD_DATA
                                  (userRecord,
                                   YerothDatabaseTableColumn::NOM_UTILISATEUR));
        user->set_prenom(GET_SQL_RECORD_DATA
                         (userRecord, YerothDatabaseTableColumn::PRENOM));
        user->set_nom(GET_SQL_RECORD_DATA
                      (userRecord, YerothDatabaseTableColumn::NOM));
        user->set_lieu_naissance(GET_SQL_RECORD_DATA
                                 (userRecord,
                                  YerothDatabaseTableColumn::LIEU_NAISSANCE));
        user->set_date_naissance(GET_SQL_RECORD_DATA
                                 (userRecord,
                                  YerothDatabaseTableColumn::DATE_NAISSANCE));
        user->set_localisation(GET_SQL_RECORD_DATA
                               (userRecord,
                                YerothDatabaseTableColumn::LOCALISATION));
        user->set_email(GET_SQL_RECORD_DATA
                        (userRecord, YerothDatabaseTableColumn::EMAIL));
        user->set_numero_telephone_1(GET_SQL_RECORD_DATA
                                     (userRecord,
                                      YerothDatabaseTableColumn::
                                      NUMERO_TELEPHONE_1));
        user->
        set_numero_telephone_2(GET_SQL_RECORD_DATA
                               (userRecord,
                                YerothDatabaseTableColumn::
                                NUMERO_TELEPHONE_2));

        user->set_titre(YerothUtils::_titreToUserViewString.
                        value(GET_SQL_RECORD_DATA
                              (userRecord,
                               YerothDatabaseTableColumn::TITRE).toInt()));

        user->set_nom_complet(QString("%1 %2").arg(user->prenom(),
                                                   user->nom()));
    }

    return user;
}


void YerothPOSChangerUtilisateurDialog::checkCourriersAlertes()
{
    YerothSqlTableModel &courriersAlertesTable =
                    _allWindows->getSqlTableModel_courriers_alertes();

    QString
    courriersAlertesFilter(GENERATE_SQL_IS_STMT
                           (YerothDatabaseTableColumn::DESTINATAIRE,
                            lineEdit_nom_utilisateur->text()));

    courriersAlertesFilter.append(" AND ").append(GENERATE_SQL_IS_STMT
                                                  (YerothDatabaseTableColumn::ALERTE_RESOLUE,
                                                   "0"));
    courriersAlertesTable.
    yerothSetFilter_WITH_where_clause(courriersAlertesFilter);

    int alertsNr = courriersAlertesTable.rowCount();

    if (courriersAlertesTable.select() && alertsNr > 0)
    {
        QApplication::beep();

        QString
        aMsg(QObject::tr("Vous avez '%1' alertes non résolues !").arg
             (QString::number(courriersAlertesTable.rowCount())));

        YerothQMessageBox::information(_allWindows->_listerAlertesWindow,
                                       QObject::tr
                                       ("changer d'utilisateur - alertes"),
                                       aMsg);
    }
}


void YerothPOSChangerUtilisateurDialog::show()
{
	comboBox_company_site
    	->populateComboBoxRawString(YerothDatabase::LOCALISATIONS,
                              	    YerothDatabaseTableColumn::NOM_LOCALISATION);

    move(*_currentPosition);

    comboBox_company_site->setFocus();

    YerothPOSDialogCommons::show();
}


void YerothPOSChangerUtilisateurDialog::closeEvent(QCloseEvent *closeEvent)
{
    lineEdit_nom_utilisateur->clear();
    lineEdit_mot_passe->clear();
    comboBox_company_site->setFocus();

    YerothPOSDialogCommons::closeEvent(closeEvent);
}
