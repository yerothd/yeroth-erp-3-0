/*
 * YEROTH_RUNTIME_VERIFIER.hpp
 *
 *  Created on: Dec 10, 2022
 *      Author: yer
 */


#include "YEROTH_RUNTIME_VERIFIER.hpp"


#include "src/utils/yeroth-erp-utils.hpp"





uint YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT(0);

uint YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index(0);

QStringList YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings;

const QString YEROTH_RUNTIME_VERIFIER::DBUS_TIME_FORMAT_WITH_MILLISECONDS(DBUS_FORMAT_TIME_WITH_MS);



YEROTH_RUNTIME_VERIFIER::YEROTH_RUNTIME_VERIFIER()
:a_YR_DB_RUNTIME_VERIF_instance(0)
{
	QString systemYerothService = "yr.db-runtime.verif";

	a_YR_DB_RUNTIME_VERIF_instance =
			new com::yeroth::rd::IYRruntimeverification(systemYerothService,
														"/YR_DB_RUNTIME_VERIF_Main",
														QDBusConnection::systemBus(),
														this);

	startTimer(3000);
}


void YEROTH_RUNTIME_VERIFIER::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (0 != a_YR_DB_RUNTIME_VERIF_instance &&
    	a_YR_DB_RUNTIME_VERIF_instance->isValid())
    {
        //qDebug() << "connected";
    }
    else
    {
    	//qDebug() << "disconnected";
    }
}


void YEROTH_RUNTIME_VERIFIER::YR_slot_refresh_DELETE_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
	a_YR_DB_RUNTIME_VERIF_instance->YR_slot_refresh_DELETE_DB_MYSQL(in0,
																	in1);

	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"DELETE",
						sql_table_name,
						"ERP_PGI",
						"runtime_verif",
						QString::number(in1));

		if (YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YEROTH_RUNTIME_VERIFIER::YR_slot_refresh_INSERT_DB_MYSQL__CALLED(QString in0 /* = "" */,
 											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
	a_YR_DB_RUNTIME_VERIF_instance->YR_slot_refresh_INSERT_DB_MYSQL(in0,
																	in1);

	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"INSERT",
						sql_table_name,
						"ERP_PGI",
						"runtime_verif",
						QString::number(in1));

		if (YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YEROTH_RUNTIME_VERIFIER::YR_slot_refresh_SELECT_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
	a_YR_DB_RUNTIME_VERIF_instance->YR_slot_refresh_SELECT_DB_MYSQL(in0,
																	in1);

	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"SELECT",
						sql_table_name,
						"ERP_PGI",
						"runtime_verif",
						QString::number(in1));

		if (YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}


void YEROTH_RUNTIME_VERIFIER::YR_slot_refresh_UPDATE_DB_MYSQL__CALLED(QString in0 /* = "" */,
   											 	 	 	 	 	 	  uint in1 /* = 1 */)
{
	a_YR_DB_RUNTIME_VERIF_instance->YR_slot_refresh_UPDATE_DB_MYSQL(in0,
																	in1);

	QStringList db_op_INFORMATIONS = in0.split(";", Qt::KeepEmptyParts);

	QString sql_table_name = db_op_INFORMATIONS.at(0);

	QString signal_source_target =
				QString("%1;%2.%3;%4;%5;%6")
				.arg(DBUS_CURRENT_TIME_WITH_MILLISECONDS,
						"UPDATE",
						sql_table_name,
						"ERP_PGI",
						"runtime_verif",
						QString::number(in1));

		if (YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index >
			YEROTH_RUNTIME_VERIFIER::_MAX_LAST_SIGNALS_SOURCE_TARGET_COUNT)
		{
			YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index = 0;
		}
		else
		{
			++YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index;
		}

		YEROTH_RUNTIME_VERIFIER::_LAST_SIGNALS_source_target_strings
							.insert(YEROTH_RUNTIME_VERIFIER::_current_MAX_LAST_SIGNALS_SOURCE_TARGET_index,
									signal_source_target);
}



