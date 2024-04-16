/*
 * yeroth-erp-process-info.cpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUNDOU
 */

#include "yeroth-erp-process-info.hpp"

#include "src/utils/yeroth-erp-logger.hpp"

#include "src/utils/yeroth-erp-config.hpp"

#include <QtCore/QByteArray>

#include <QtCore/QDebug>

#include <QtCore/QFile>

#include <QtCore/QProcess>




bool YerothProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(QString a_daemon_process_ID_STR,
																QString executable_full_path,
																YerothLogger *logger /* = 0 */)
{
	if (logger)
	{
		logger->log
		("yeroth-erp-3-0 | YerothProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING");
	}

	QStringList progArguments;

	progArguments << "-a"
				  << a_daemon_process_ID_STR;

	QString PGREP_Program("/usr/bin/pgrep");

	QString yerothpsoutput_file(QString("%1/%2_psoutput")
									.arg(YerothERPConfig::temporaryFilesDir,
										 QString(".%1")
										 	 .arg(a_daemon_process_ID_STR)));

//	QDEBUG_STRING_OUTPUT_2(QString("%1: temporary psoutput file: ")
//								.arg(a_daemon_process_ID_STR),
//						   yerothpsoutput_file);

	int output_file_size =
			YerothERPProcess::start_PROCESS_AND_READ_PROCESS_output_INTO_FILE
								(PGREP_Program,
								 yerothpsoutput_file,
								 progArguments);

	if (output_file_size > 0)
	{
//		QDEBUG_STRING_OUTPUT_2_N(QString("%1; output_file_size: ")
//									.arg(a_daemon_process_ID_STR),
//								  output_file_size);

		return YerothUtils::GREP_YEROTH_FILE_CONTENT(yerothpsoutput_file,
													 executable_full_path,
													 a_daemon_process_ID_STR);
	}

	return false;
}

