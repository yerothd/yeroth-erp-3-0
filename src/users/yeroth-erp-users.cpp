/*
 * yeroth-erp-users.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-users.hpp"


#include "yeroth-erp-user-settings.hpp"

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"

#include "src/yeroth-erp-windows.hpp"

#include "src/utils/yeroth-erp-utils.hpp"

#include "src/utils/yeroth-erp-sqltable-model.hpp"


#include <QtCore/QTextCodec>

#include <QtCore/QDate>

#include <QtCore/QString>


YerothPOSUser::YerothPOSUser(YerothERPWindows *allWindows)
:_role(YerothUtils::ROLE_INDEFINI),
 _write_read_user_setting_file_YEROTH_SEMAPHORE(0),
 _user_personal_settings(0),
 _allWindows(allWindows)
{
	_write_read_user_setting_file_YEROTH_SEMAPHORE = new QSemaphore(1);

    _user_personal_settings = new YerothERPUserSettings;
}


YerothPOSUser::~YerothPOSUser()
{
    YEROTH_DELETE_FREE_POINTER_NOW(_write_read_user_setting_file_YEROTH_SEMAPHORE);

    YEROTH_DELETE_FREE_POINTER_NOW(_user_personal_settings);
}


QString YerothPOSUser::toString()
{
    QString userInfo;

    if (YerothMainWindow::LANGUE_ANGLAISE)
    {
        userInfo.append(QString("1) Given names: %1\n"
                                "2) Last names: %2\n"
                                "3) Title: %3\n"
                                "4) User ID: %4\n"
                                "5) Email: %5\n"
                                "6) Phone number 1: %6\n"
                                "7) Phone number 2: %7\n"
                                "8) Role: %8\n"
                                "9) Localisation: %9\n")
        		.arg(prenom(),
        			 nom(),
					 titre(),
					 nom_utilisateur(),
					 email(),
					 numero_telephone_1(),
					 numero_telephone_2(),
					 getRole(),
					 localisation()));
    }
    else
    {
        userInfo.append(QString("1) Prénoms: %1\n"
                                "2) Noms: %2\n"
                                "3) Titre: %3\n"
                                "4) Identifiant de l'utilisateur: %4\n"
                                "5) Émail: %5\n"
                                "6) Numéro de téléphone 1: %6\n"
                                "7) Numéro de téléphone 2: %7\n"
                                "8) Rôle: %8\n"
                                "9) Localisation: %9\n")
        		.arg(prenom(),
        			 nom(),
					 titre(),
					 nom_utilisateur(),
					 email(),
					 numero_telephone_1(),
					 numero_telephone_2(),
					 getRole(),
					 localisation()));
    }


    return userInfo;
}


QString YerothPOSUser::getRole()
{
    QString role;

    switch (_role)
    {
    case YerothUtils::ROLE_ADMINISTRATEUR:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_ADMINISTRATEUR));
        break;

    case YerothUtils::ROLE_MANAGER:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_MANAGER));
        break;

    case YerothUtils::ROLE_MAGASINIER:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_MAGASINIER));
        break;

    case YerothUtils::ROLE_VENDEUR:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_VENDEUR));
        break;

    case YerothUtils::ROLE_GESTIONNAIREDESTOCK:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_GESTIONNAIREDESTOCK));
        break;

    case YerothUtils::ROLE_CAISSIER:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_CAISSIER));
        break;

    case YerothUtils::ROLE_INDEFINI:
        role.append(YerothUtils::_roleToUserViewString
        			.value(YerothUtils::ROLE_INDEFINI));
        break;

    default:
        break;
    }

    return role;
}


QString YerothPOSUser::nom_complet_truncated_FOR_SMALL_RECEIPT()
{
    QString nom_complet_truncated(nom_complet().trimmed());

    if (nom_complet_truncated.length() > 23)
    {
        nom_complet_truncated.truncate(23);
        nom_complet_truncated.append(".");
    }

    return nom_complet_truncated;
}


void YerothPOSUser::PRINT_PARAMETERS_TO_STRING()
{
    if (0 != _user_personal_settings)
    {
        _user_personal_settings->PRINT_PARAMETERS_TO_STRING();
    }
}


void YerothPOSUser::cleanup_user_personal_PRINTING_PARAMETER_settings()
{
    _write_read_user_setting_file_YEROTH_SEMAPHORE->acquire(1);

    if (0 != _user_personal_settings)
    {
        _user_personal_settings->cleanup_user_settings();
    }

    _write_read_user_setting_file_YEROTH_SEMAPHORE->release(1);
}


void YerothPOSUser::read_user_personal_PRINTING_PARAMETER_settings
		(YerothWindowsCommons *a_current_window_to_table_print_as_parameter /* = 0 */)
{
    _write_read_user_setting_file_YEROTH_SEMAPHORE->acquire(1);

    if (0 != _user_personal_settings)
    {
        enum RESULT_PRINTING_PARAMETER resultat_lecture_des_parametres_locaux =
        		_user_personal_settings
					->lire_les_parametres_locaux(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID,
												 a_current_window_to_table_print_as_parameter);

        //              if (READ_PRINTING_PARAMETER_SUCCESSFUL == resultat_lecture_des_parametres_locaux)
        //              {
        //                      QDEBUG_STRING_OUTPUT_2("read_user_personal_PRINTING_PARAMETER_settings", "READ_PRINTING_PARAMETER_SUCCESSFUL");
        //              }
        //              else
        //              {
        //                      QDEBUG_STRING_OUTPUT_2("read_user_personal_PRINTING_PARAMETER_settings", "! READ_PRINTING_PARAMETER_SUCCESSFUL");
        //              }
    }

    _write_read_user_setting_file_YEROTH_SEMAPHORE->release(1);
}


void YerothPOSUser::read_user_personal_PRINTING_PARAMETER_settings_QTABLEWIDGET
		(YerothWindowsCommons *a_current_window_to_table_print_as_parameter /* = 0 */)
{
    _write_read_user_setting_file_YEROTH_SEMAPHORE->acquire(1);

    if (0 != _user_personal_settings)
    {
        enum RESULT_PRINTING_PARAMETER resultat_lecture_des_parametres_locaux =
        		_user_personal_settings
					->lire_les_parametres_locaux_QTABLEWIDGET(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID,
															  a_current_window_to_table_print_as_parameter);

        //              if (READ_PRINTING_PARAMETER_SUCCESSFUL == resultat_lecture_des_parametres_locaux)
        //              {
        //                      QDEBUG_STRING_OUTPUT_2("read_user_personal_PRINTING_PARAMETER_settings", "READ_PRINTING_PARAMETER_SUCCESSFUL");
        //              }
        //              else
        //              {
        //                      QDEBUG_STRING_OUTPUT_2("read_user_personal_PRINTING_PARAMETER_settings", "! READ_PRINTING_PARAMETER_SUCCESSFUL");
        //              }
    }

    _write_read_user_setting_file_YEROTH_SEMAPHORE->release(1);
}


void YerothPOSUser::save_user_personal_PRINTING_PARAMETER_settings
		(YerothWindowsCommons *a_current_window_to_table_print_as_parameter /* = 0 */)
{
    _write_read_user_setting_file_YEROTH_SEMAPHORE->acquire(1);

    if (0 != _user_personal_settings)
    {
    	if (0 == a_current_window_to_table_print_as_parameter)
    	{
            _user_personal_settings
				->enregistrer_les_parametres_locaux
					(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID);
    	}
    	else
    	{
            _user_personal_settings
				->enregistrer_les_parametres_locaux__QTABLEWIDGET
					(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID,
					 a_current_window_to_table_print_as_parameter);
    	}
    }

    _write_read_user_setting_file_YEROTH_SEMAPHORE->release(1);
}


void YerothPOSUser::create_user_personal_settings_file()
{
    _write_read_user_setting_file_YEROTH_SEMAPHORE->acquire(1);

    QByteArray md5Hash_mot_passe(MD5_HASH(_nom_utilisateur));

    _user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID =
    		QString("%1/%2")
				.arg(YerothERPConfig::YEROTH_ERP_3_0_USER_LOCAL_SETTINGS_FOLDER,
					 QString(md5Hash_mot_passe.toHex()));

    //      QDEBUG_STRING_OUTPUT_2("_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID",
    //                                                 _user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID);

    if (0 != _user_personal_settings)
    {
        enum RESULT_PRINTING_PARAMETER resultat_lecture_des_parametres_locaux;

        resultat_lecture_des_parametres_locaux =
        		_user_personal_settings
					->lire_les_parametres_locaux
						(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID);

        if (PRINTING_PRAMATER_WINDOW_NOT_YET_DEFINED == resultat_lecture_des_parametres_locaux ||
        	PRINTING_PARAMETER_FILE_DOESNT_EXIT == resultat_lecture_des_parametres_locaux)
        {
            _user_personal_settings
				->enregistrer_les_parametres_locaux
					(_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID,
					 PRINTING_PRAMATER_WINDOW_NOT_YET_DEFINED);
        }
        //      QDEBUG_STRING_OUTPUT_2("_user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID",
        //                                                 _user_setting_disk_saving_file_name_FROM_MD5_HEX_USER_ID);
    }

    _write_read_user_setting_file_YEROTH_SEMAPHORE->release(1);
}



