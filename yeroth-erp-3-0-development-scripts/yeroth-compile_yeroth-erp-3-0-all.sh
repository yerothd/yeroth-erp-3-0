#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-all.sh
# @author: Dr.-Ing. XAVIER NOUMBISSI NOUNDOU
# @email: YEROTH.D@GMAIL.COM

#yeroth-compile_yeroth-erp-3-0-standalone.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-server.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-client.sh "$@"; \
  yeroth-compile_yeroth-erp-3-0-academic.sh "$@"
