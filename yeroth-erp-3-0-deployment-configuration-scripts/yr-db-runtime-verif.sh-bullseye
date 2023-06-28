#! /bin/sh
# /etc/init.d/yr-db-runtime-verif.sh
#

### BEGIN INIT INFO
# Provides: yr-db-runtime-verif.sh
# Required-Start:    $local_fs $syslog $remote_fs networking mysql
# Required-Stop:     $local_fs $syslog $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Enable RUNTIME VERIFICATION system of ''Yeroth-erp-3.0''
### END INIT INFO

export DISPLAY=:0.0

kill_yr_db_runtime_verif_daemon()
{
		pgrep -a yr-db-runtime \
				| grep "${YR_DB_RUNTIME_VERIF_HOME_FOLDER}"/bin/yr-db-runtime-verif \
				| awk '//{print $1}' | xargs kill -9 > /dev/null 2>&1
}

[ -f /etc/environment ] && . /etc/environment

# Carry out specific functions when asked to by the system
case "$1" in
  start)
    echo "Starting script yr-db-runtime-verif.sh"
    xvfb-run --auto-servernum "${YR_DB_RUNTIME_VERIF_HOME_FOLDER}"/bin/yr-db-runtime-verif > /dev/null 2>&1 
    ;;
  stop)
    echo "Stopping script yr-db-runtime-verif.sh"
    kill_yr_db_runtime_verif_daemon
    ;;
  restart)
    echo "Restarting script yr-db-runtime-verif.sh"
    kill_yr_db_runtime_verif_daemon
    xvfb-run --auto-servernum "${YR_DB_RUNTIME_VERIF_HOME_FOLDER}"/bin/yr-db-runtime-verif > /dev/null 2>&1
    ;;
  force-reload)
    echo "Force reload script yr-db-runtime-verif.sh"
    kill_yr_db_runtime_verif_daemon
    xvfb-run --auto-servernum "${YR_DB_RUNTIME_VERIF_HOME_FOLDER}"/bin/yr-db-runtime-verif > /dev/null 2>&1
    ;;
  status)
    echo "Check status of script yr-db-runtime-verif.sh"
    pgrep -a yr-db-runtime | grep "${YR_DB_RUNTIME_VERIF_HOME_FOLDER}"/bin/yr-db-runtime-verif 
    ;;
  *)
    echo "Usage: /etc/init.d/yr-db-runtime-verif.sh {start|stop|restart|force-reload|status}"
    exit 1
    ;;
esac

exit 0
