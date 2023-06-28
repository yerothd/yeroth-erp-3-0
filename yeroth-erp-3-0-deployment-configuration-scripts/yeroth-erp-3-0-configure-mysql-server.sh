#!/bin/bash
# @author: Dr.-Ing. XAVIER NOUMBISSI NOUNDOU

sudo service mysql stop

sudo /usr/bin/mysqld_safe &

sleep 5s

sudo expect /tmp/yeroth-erp-3-0-configure-mysql-server-set-root-pwd.exp
