#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-standalone.sh
# @author: Dr.-Ing. XAVIER NOUNDOU
# @email: YEROTH.D@GMAIL.COM

if [[ "${@}" == *"s"* || "${@}" == *"h"* ]]; then
		yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_STANDALONE "${@}"
		exit
fi

if [[ "${@}" == *"e"* ]]; then
	OUT_CMD_TIME=$(time yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_STANDALONE "${@}" > /dev/null 2>&1) && \
		mv bin/yeroth-erp-3-0 bin/yeroth-erp-3-0-standalone-ENGLISH
else
	OUT_CMD_TIME=$(time yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_STANDALONE "${@}" > /dev/null 2>&1) && \
		mv bin/yeroth-erp-3-0 bin/yeroth-erp-3-0-standalone
fi

val=$(cat YEROTH_STANDALONE.error.log | grep 'error\|ERROR\|Error' | wc -l)

if [ ${val} -eq 0 ]; then
		echo "[compilation avec succes]"
else
		echo "[compilation avec des erreurs (${val})]"
fi

echo ${OUT_CMD_TIME}
