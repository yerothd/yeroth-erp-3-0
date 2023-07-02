/*
 * yeroth-erp-admin-windows-commons.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#include "src/admin/yeroth-erp-admin-windows-commons.hpp"

#include "src/yeroth-erp-windows.hpp"


#include "src/widgets/yeroth-erp-select-db-qcheckbox.hpp"

#include "src/dialogs/yeroth-erp-generic-select-db-field-dialog.hpp"


#include <QtCore/QProcess>

#include <QtCore/QObject>

#include <QtWidgets/QDesktopWidget>


YerothERPWindows *YerothPOSAdminWindowsCommons::_allWindows(0);

QPoint *YerothPOSAdminWindowsCommons::_centerPosition(new QPoint);



YerothPOSAdminWindowsCommons::~YerothPOSAdminWindowsCommons()
{
	YerothERPGenericSelectDBFieldDialog *cur_dbfield_dialog = 0;

	for (uint i = 0;
		 i < sqlTableName__TO__selectDBFieldDialog.size();
		 ++i)
	{
		if (0 != cur_dbfield_dialog)
		{
			delete cur_dbfield_dialog;
		}
	}
}


void YerothPOSAdminWindowsCommons::menu()
{
    _allWindows->_adminWindow->rendreVisible();
    rendreInvisible();
}


void YerothPOSAdminWindowsCommons::retour_menu_principal()
{
    _allWindows->_mainWindow->rendreVisible(_allWindows->_adminWindow->
                                            getStocksTableModel());
    rendreInvisible();
}


void YerothPOSAdminWindowsCommons::
		CLOSE_SELECT_EXPORT_DB_DIALOG(YerothSqlTableModel 			*aSqlTableModel /* = 0 */,
									  YerothTableViewWITHpagination *a_table_view_to_list_show /* = 0 */)
{
	YerothERPGenericSelectDBFieldDialog *cur_dbfield_dialog = 0;

	for (uint i = 0;
		 i < sqlTableName__TO__selectDBFieldDialog.size();
		 ++i)
	{
		if (0 != cur_dbfield_dialog)
		{
			cur_dbfield_dialog->close();
		}
	}

	if (0 != aSqlTableModel 		&&
		0 != a_table_view_to_list_show)
	{
		_allWindows->_adminListerWindow
			->LIST_SHOW_TABLE_VIEW_WITH_PAGINATION(*a_table_view_to_list_show,
				 	 	 	 	 	 	 	   	   *aSqlTableModel);
	}
}


void YerothPOSAdminWindowsCommons::
		selectionner_champs_db_visibles(bool SETUP_DIALOG_BOX_GEOMETRY /* = true */,
										bool CHARGES_FINANCIERES_WINDOW /* = false */)
{
	if (0 != _selectExportDBQDialog)
	{
		if (0 != _selectExportDBQDialog->_associatedWindow)
		{
			YerothTableView *a_table_view =
					_selectExportDBQDialog->_associatedWindow->GET_YEROTH_TABLE_VIEW();

			if (0 != a_table_view)
			{
				QString ASSOCIATED_qsql_database = a_table_view->getSqlTableName();

				_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.clear();

				setup_select_configure_dbcolumn(ASSOCIATED_qsql_database);
			}
		}
	}


	if (_visibleDBColumnNameStrList.size() >= 0)
    {
        _visibleQCheckboxs.clear();
        _visibleQCheckboxs.resize(_visibleDBColumnNameStrList.size());
    }


    QString aDBColumnName;

    QString aDBColumnName_VIEW_STRING;

    for (unsigned k = 0; k < _dbtablecolumnNameToDBColumnIndex.size(); ++k)
    {
        aDBColumnName = _dbtablecolumnNameToDBColumnIndex.key(k);

        if (_NOT_VISIBLE_FOR_USER_DB_TABLE_COLUMN_NAME.contains(aDBColumnName))
        {
            continue;
        }


        if (! YerothERPConfig::SHOW_DATABASE_RAW)
        {
        	if (! CHARGES_FINANCIERES_WINDOW)
        	{
        		YerothDatabaseTableColumn::_tableColumnToUserViewString
					.insert(YerothDatabaseTableColumn::CATEGORIE,
							QObject::tr("Catégorie"));
        	}
        	else
        	{
        		YerothDatabaseTableColumn::_tableColumnToUserViewString
					.insert(YerothDatabaseTableColumn::CATEGORIE,
							QObject::tr("LIGNE BUDGÉTAIRE"));
        	}

            aDBColumnName_VIEW_STRING =
                            YEROTH_DATABASE_TABLE_COLUMN_TO_USER_VIEW_STRING(aDBColumnName);
        }
        else
        {
            aDBColumnName_VIEW_STRING = aDBColumnName;
        }


        if (!aDBColumnName_VIEW_STRING.isEmpty())
        {
            _db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName
				.insert(aDBColumnName_VIEW_STRING,
						aDBColumnName);
        }
    }


    //A QMAP AUTOMATICALLY SORT ITS KEYS !
    QStringList all_dbfieldColumnName_IN_ALPHABETIC_ORDER
					(_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName.keys());


    if (SETUP_DIALOG_BOX_GEOMETRY)
    {
    	YerothSelectDBQCheckBox *aQCheckBox = 0;

    	for (unsigned k = 0; k < all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size();
    			++k)
    	{
    		aQCheckBox = new YerothSelectDBQCheckBox(_selectExportDBQDialog,
    												 &_visibleDBColumnNameStrList);


    		aDBColumnName_VIEW_STRING = all_dbfieldColumnName_IN_ALPHABETIC_ORDER.at(k);

    		aDBColumnName =
    				_db_VIEW_STRING_ALPHABETIC_ORDER_To_dbtablecolumnName
						.value(aDBColumnName_VIEW_STRING);

    		aQCheckBox->setObjectName(aDBColumnName);

    		aQCheckBox->setGeometry(QRect(getDialogBox_Xn_coordinate(k),
    								getDialogBox_Yn_coordinate(k),
									YerothWindowsCommons::CHECKBOX_YEROTH_FOR_DB_TABLE_COLUMN_FILTERING_WIDTH_SIZE,
									25));

    		aQCheckBox->setText(aDBColumnName_VIEW_STRING);

    		if (_visibleDBColumnNameStrList.contains(aDBColumnName))
    		{
    			aQCheckBox->setChecked(true);
    		}

    		connect(aQCheckBox,
    				SIGNAL(clicked(bool)),
					aQCheckBox,
					SLOT(handle_visible_db_field_checkBox(bool)));

    		_visibleQCheckboxs.append(aQCheckBox);
    	}

    	_selectExportDBQDialog
			->setWindowTitle(QObject::tr("%1 - sélectionner des colones "
    									 "visibles ('%2' colones)")
								.arg(GET_YEROTH_ERP_WINDOW_TITLE_MACRO,
									 QString::number(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size())));

    	_selectExportDBQDialog
			->setFixedSize(getDialogBox_LONGUEUR(all_dbfieldColumnName_IN_ALPHABETIC_ORDER.size()),
												 382.5);

    	_selectExportDBQDialog->showAsModalDialogWithParent(*this);
    }
}


void YerothPOSAdminWindowsCommons::setup_select_configure_dbcolumn(const QString &aSqlTableName)
{
	if (aSqlTableName.isEmpty())
	{
		return ;
	}


	bool is_PALETTE_COLOR_SET = true;


	if (sqlTableName__TO__selectDBFieldDialog.contains(aSqlTableName))
	{
	    _selectExportDBQDialog = sqlTableName__TO__selectDBFieldDialog.value(aSqlTableName);
	}
	else
	{
		is_PALETTE_COLOR_SET = false;

	    _selectExportDBQDialog =
	    		new YerothERPGenericSelectDBFieldDialog(_allWindows,
	    												this);

	    sqlTableName__TO__selectDBFieldDialog.insert(aSqlTableName,
	    											 _selectExportDBQDialog);
	}


	if (!is_PALETTE_COLOR_SET  	  &&
		0 != getQMainWindowToolBar())
	{
		_selectExportDBQDialog->setPalette(getQMainWindowToolBar()->palette());
	}

	_selectExportDBQDialog->setStyleSheet(qMessageBoxStyleSheet());


	_varchar_dbtable_column_name_list.clear();

	_DBFieldNamesToPrintLeftAligned.clear();

	_DBFieldNamesToPrintRightAligned.clear();

	_dbtablecolumnNameToDBColumnIndex.clear();


	QString strShowColumnQuery = QString("SHOW COLUMNS FROM %1")
    												.arg(aSqlTableName);

	QSqlQuery query;

	int querySize = YerothUtils::execQuery(query, strShowColumnQuery);

	unsigned int columnIdx = -1;

	for (int k = 0; k < querySize && query.next(); ++k)
	{
		QString fieldName(query.value(0).toString());

		QString type(query.value(1).toString());

		columnIdx = columnIdx + 1;

		if (type.contains(YerothUtils::DATABASE_MYSQL_VARCHAR_TYPE_STRING) ||
			YerothUtils::isEqualCaseInsensitive(fieldName,
												YerothDatabaseTableColumn::ID))
		{
			_varchar_dbtable_column_name_list.insert(fieldName);
		}

		if (type.contains(YerothUtils::DATABASE_MYSQL_CHAR_TYPE_STRING) ||
			type.contains(YerothUtils::DATABASE_MYSQL_TIME_TYPE_STRING) ||
			type.contains(YerothUtils::DATABASE_MYSQL_DATE_TYPE_STRING))
		{
			_DBFieldNamesToPrintLeftAligned.insert(columnIdx);
		}
		else if (type.contains(YerothUtils::DATABASE_MYSQL_DOUBLE_TYPE_STRING) ||
				 type.contains(YerothUtils::DATABASE_MYSQL_INT_TYPE_STRING))
		{
			_DBFieldNamesToPrintRightAligned.insert(columnIdx);
		}

		_dbtablecolumnNameToDBColumnIndex.insert(fieldName, columnIdx);
	}
}


void YerothPOSAdminWindowsCommons::infosEntreprise()
{
    YerothUtils::infosEntreprise(*this,
                                 _allWindows->getInfoEntreprise().
                                 toString());
}

void YerothPOSAdminWindowsCommons::getManuelUtilisateurPDF()
{
    YerothPOSUser *user = _allWindows->getUser();
    if (user)
    {
        QStringList progArguments;
        QProcess aProcess;

        if (user->isAdmin() || user->isManager())
        {
            progArguments <<
                          YerothERPConfig::
                          FILE_ABSOLUTEPATH_YEROTH_ERP_3_0_MANUEL_DE_LUTILISATEUR_MANAGER;
            aProcess.startDetached(YerothERPConfig::pathToPdfReader,
                                   progArguments);
        }
    }
}

void YerothPOSAdminWindowsCommons::mySetupUi(QMainWindow *aWindow)
{
    aWindow->setFixedSize(aWindow->width(), aWindow->height());
    aWindow->move(*_centerPosition);
}

void YerothPOSAdminWindowsCommons::qui_suis_je()
{
    YerothQMessageBox::information(this, QObject::tr("qui suis je ?"),
                                   _allWindows->getUser()->toString());
}

void YerothPOSAdminWindowsCommons::changer_utilisateur()
{
    _allWindows->_changerUtilisateurDialog->setParent(this,
                                                      Qt::Popup | Qt::Dialog);
    _allWindows->_changerUtilisateurDialog->show();
    _allWindows->_changerUtilisateurDialog->activateWindow();
}

void YerothPOSAdminWindowsCommons::deconnecter_utilisateur()
{
    _allWindows->definirPasDeRole();
    _allWindows->_mainWindow->rendreVisible(&_allWindows->
                                            getSqlTableModel_stocks());
    setVisible(false);
}


void YerothPOSAdminWindowsCommons::rendreVisible(YerothSqlTableModel *stocksTableModel)
{
    _curStocksTableModel = stocksTableModel;
    setVisible(true);
}
