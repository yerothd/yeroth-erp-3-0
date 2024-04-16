/*
 * yeroth-erp-user.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#ifndef SRC_YEROTH_ERP_USERS_HPP_
#define SRC_YEROTH_ERP_USERS_HPP_


#include "src/users/yeroth-erp-user-settings.hpp"

#include "src/utils/yeroth-erp-utils.hpp"


#include <QtCore/QSemaphore>

#include <QtCore/QString>

#include <QtCore/QDate>

#include <QtSql/QSqlRecord>


class QString;
class QDate;

class YerothERPWindows;
class YerothSqlTableModel;

class YerothERPUserSettings;
class YerothPOSUserAdministrateur;
class YerothPOSUserManager;
class YerothERPVendeur;
class YerothPOSUserGestionaireDesStocks;
class YerothPOSUserMagasinier;
class YerothPOSUserCaissier;

class YerothPOSUser : public QObject
{
	Q_OBJECT

public:

    YEROTH_CLASS_OPERATORS


	YerothPOSUser(YerothERPWindows *allWindows);


    virtual ~YerothPOSUser();


    enum YerothUtils::USER_ROLE role()
    {
        return _role;
    }


    QString toString();


    inline void setRole(enum YerothUtils::USER_ROLE aRole)
    {
        _role = aRole;
    }

    QString getRole();

    inline QString prenom()
    {
        return _prenom;
    }

    inline QString nom()
    {
        return _nom;
    }

    inline QString nom_utilisateur()
    {
        return _nom_utilisateur;
    }

    inline QString nom_complet()
    {
        return _nom_complet;
    }

    QString nom_complet_truncated_FOR_SMALL_RECEIPT();

    inline QString nom_completTex()
    {
        return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_nom_complet);
    }

    inline QString nom_complet_truncated_FOR_SMALL_RECEIPT_Tex()
    {
        return YerothUtils::LATEX_IN_OUT_handleForeignAccents
        			(nom_complet_truncated_FOR_SMALL_RECEIPT());
    }

    inline QString localisation()
    {
        return _localisation;
    }

    inline QString email()
    {
        return _email;
    }

    inline QString numero_telephone_1()
    {
        return _numero_telephone_1;
    }

    inline QString numero_telephone_2()
    {
        return _numero_telephone_2;
    }

    inline QString lieu_naissance()
    {
        return _lieu_naissance;
    }

    inline QString date_naissance()
    {
        return _date_naissance;
    }

    inline QString titre()
    {
        return _titre;
    }

    inline QString titreTex()
    {
        return YerothUtils::LATEX_IN_OUT_handleForeignAccents(_titre);
    }

    inline void set_prenom(const QString &prenom)
    {
        _prenom = prenom;
    }

    inline void set_nom(const QString &nom)
    {
        _nom = nom;
    }

    inline void set_nom_utilisateur(const QString &nom_utilisateur)
    {
        _nom_utilisateur = nom_utilisateur;
    }

    inline void set_nom_complet(const QString &nom_complet)
    {
        _nom_complet = nom_complet;
    }

    inline void set_localisation(const QString &localisation)
    {
        _localisation = localisation;
    }

    inline void set_email(const QString &email)
    {
        _email = email;
    }

    inline void set_numero_telephone_1(const QString &numero_telephone_1)
    {
        _numero_telephone_1 = numero_telephone_1;
    }

    inline void set_numero_telephone_2(const QString &numero_telephone_2)
    {
        _numero_telephone_2 = numero_telephone_2;
    }

    inline void set_lieu_naissance(const QString &lieu_naissance)
    {
        _lieu_naissance = lieu_naissance;
    }

    inline void set_date_naissance(const QString &date_naissance)
    {
        _date_naissance = date_naissance;
    }

    inline void set_titre(const QString &titre)
    {
        _titre = titre;
    }

    virtual inline bool isPasDeRole()
    {
        return _role == YerothUtils::ROLE_INDEFINI;
    }

    virtual inline bool isAdmin()
    {
        return _role == YerothUtils::ROLE_ADMINISTRATEUR;
    }

    virtual inline bool isManager()
    {
        return _role == YerothUtils::ROLE_MANAGER;
    }

    virtual inline bool isVendeur()
    {
        return _role == YerothUtils::ROLE_VENDEUR;
    }

    virtual inline bool isMagasinier()
    {
        return _role == YerothUtils::ROLE_MAGASINIER;
    }

    virtual inline bool isGestionaireDesStocks()
    {
        return _role == YerothUtils::ROLE_GESTIONNAIREDESTOCK;
    }

    virtual inline bool isCaissier()
    {
        return _role == YerothUtils::ROLE_CAISSIER;
    }

    void PRINT_PARAMETERS_TO_STRING();

    inline QString GET_WINDOWS_PAGE_FROM_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
        			_user_personal_settings
						->GET_WINDOWS_PAGE_FROM_KEY_TO_STORE(a_window_object_name) :
					YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_PAGE_TO_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_WINDOWS_PAGE_TO_KEY_TO_STORE(a_window_object_name) :
				   YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_A4_PRINTING_POSITION_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_WINDOWS_A4_PRINTING_POSITION_KEY_TO_STORE(a_window_object_name) :
				   YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_USERSQL_TABLE_ROW_COUNT_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_WINDOWS_USERSQL_TABLE_ROW_COUNT_KEY_TO_STORE(a_window_object_name) :
				   YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_PRINT_TABLE_font_size_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_WINDOWS_PRINT_TABLE_font_size_KEY_TO_STORE(a_window_object_name) :
			   YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_PRINT_TABLE_ROW_COUNT_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 !=
                _user_personal_settings) ?
               _user_personal_settings->GET_WINDOWS_PRINT_TABLE_ROW_COUNT_KEY_TO_STORE
               (a_window_object_name) : YerothUtils::EMPTY_STRING;
    }

    inline QString GET_WINDOWS_TABLE_COLUMN_ORDER_KEY_TO_STORE(const QString &a_window_object_name)
    {
        return (0 !=
                _user_personal_settings) ?
               _user_personal_settings->GET_WINDOWS_TABLE_COLUMN_ORDER_KEY_TO_STORE
               (a_window_object_name) : YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE(const QString &a_key)
    {
        return (0 !=
                _user_personal_settings) ?
               _user_personal_settings->GET_PRINTING_PARAMETER_VALUE_FROM_KEY(a_key) :
               YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE_page_from(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
				   	   (GET_WINDOWS_PAGE_FROM_KEY_TO_STORE(a_window_object_name)) :
               YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE_page_to(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
               (GET_WINDOWS_PAGE_TO_KEY_TO_STORE(a_window_object_name)) :
               YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE_printing_position(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
				   	   (GET_WINDOWS_A4_PRINTING_POSITION_KEY_TO_STORE(a_window_object_name)) :
               YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE_USERSQL_table_row_count(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
				   	   (GET_WINDOWS_USERSQL_TABLE_ROW_COUNT_KEY_TO_STORE(a_window_object_name)) :
				YerothUtils::EMPTY_STRING;
    }

    inline QString get_PARAMETER_VALUE_table_font_size(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
				   	   (GET_WINDOWS_PRINT_TABLE_font_size_KEY_TO_STORE(a_window_object_name)) :
                YerothUtils::EMPTY_STRING;
    }

    inline QString get_PRINTING_PARAMETER_VALUE_print_table_row_count(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings
			   	   ->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
				   	   (GET_WINDOWS_PRINT_TABLE_ROW_COUNT_KEY_TO_STORE(a_window_object_name)) :
				YerothUtils::EMPTY_STRING;
    }

    inline QString get_PARAMETER_VALUE_table_column_order(const QString &a_window_object_name)
    {
        return (0 != _user_personal_settings) ?
               _user_personal_settings->GET_PRINTING_PARAMETER_VALUE_FROM_KEY
               (GET_WINDOWS_TABLE_COLUMN_ORDER_KEY_TO_STORE(a_window_object_name)) :
               YerothUtils::EMPTY_STRING;
    }


    void cleanup_user_personal_PRINTING_PARAMETER_settings();


    void read_user_personal_PRINTING_PARAMETER_settings
			(YerothWindowsCommons *a_current_window_to_table_print_as_parameter = 0);


    void read_user_personal_PRINTING_PARAMETER_settings_QTABLEWIDGET
			(YerothWindowsCommons *a_current_window_to_table_print_as_parameter = 0);


    void save_user_personal_PRINTING_PARAMETER_settings
			(YerothWindowsCommons *a_current_window_to_table_print_as_parameter = 0);


    void create_user_personal_settings_file();


protected:

    enum YerothUtils::USER_ROLE _role;

    QString _prenom;
    QString _nom;
    QString _nom_utilisateur;
    QString _localisation;
    QString _email;
    QString _numero_telephone_1;
    QString _numero_telephone_2;
    QString _lieu_naissance;
    QString _date_naissance;
    QString _titre;
    QString _nom_complet;

    QString _user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID;


    QSemaphore *_write_read_user_setting_file_YEROTH_SEMAPHORE;

    YerothERPUserSettings *_user_personal_settings;


private:

    YerothERPWindows *_allWindows;
};

#endif /* SRC_YEROTH_ERP_USERS_HPP_ */
