/*
 * yeroth-erp-process-info.hpp
 *
 *      Author: DR.-ING. DIPL.-INF. XAVIER NOUMBISSI NOUNDOU
 */

#ifndef SRC_UTILS_YEROTH_PROCESS_INFO_HPP_
#define SRC_UTILS_YEROTH_PROCESS_INFO_HPP_

#include "src/include/yeroth-erp-3-0-software.text-configuration.hpp"


#include "src/process/yeroth-erp-process.hpp"


#include <QtCore/QString>


class YerothUtils;
class YerothLogger;

class YerothProcessInfo
{
public:

    YEROTH_CLASS_OPERATORS inline YerothProcessInfo()
    {
    }

    inline ~YerothProcessInfo()
    {
    }

    static bool CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(QString a_daemon_process_ID_STR,
    													QString executable_full_path,
    													YerothLogger *logger = 0);

    inline static bool check_Yr_Db_Runtime_Verif_Daemon_Running(YerothLogger *logger = 0)
    {
    	return
    	  YerothProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(
    			  	  	  	  	  YerothERPConfig::YR_DB_RUNTIME_VERIF_SYSTEM_DAEMON_ID_STR,
								  YerothUtils::GET_YR_DB_RUNTIME_VERIF_EXECUTABLE_FULL_PATH(),
								  logger);
    }

    inline static bool checkYerothERPAlertDaemonRunning(YerothLogger *logger = 0)
    {
    	return
    	  YerothProcessInfo::CHECK_WHETHER_DAEMON_PROCESS_IS_RUNNING(
    			  	  	  	  	  YerothERPConfig::YEROTH_ERP_3_0_SYSTEM_DAEMON_ID_STR,
								  YerothUtils::getYerothAlertDaemonExecutableFullpath(),
								  logger);
    }
};


#endif /* SRC_UTILS_YEROTH_PROCESS_INFO_HPP_ */
