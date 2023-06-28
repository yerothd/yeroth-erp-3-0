#!/bin/bash
# @file: yeroth-compile_yeroth-erp-3-0-client.sh
# @author: Dr.-Ing. XAVIER NOUMBISSI NOUNDOU
# @email: YEROTH.D@GMAIL.COM



if [[ "${@}" == *"s"* || "${@}" == *"h"* ]]; then
		yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_CLIENT "${@}"
		exit
fi

if [[ "${@}" == *"e"* ]]; then
	OUT_CMD_TIME=$(time yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_CLIENT "${@}" > /dev/null 2>&1) && \
		mv bin/yeroth-erp-3-0 bin/yeroth-erp-3-0-server-ENGLISH
else
	OUT_CMD_TIME=$(time yeroth-compile_yeroth-erp-3-0.sh -v YEROTH_CLIENT "${@}" > /dev/null 2>&1) && \
		mv bin/yeroth-erp-3-0 bin/yeroth-erp-3-0-server
fi

val=$(cat YEROTH_CLIENT.error.log | grep 'error\|ERROR\|Error' | wc -l)

if [ ${val} -eq 0 ]; then
		echo "[compilation avec succes]"
else
		echo "[compilation avec des erreurs (${val})]"
fi

echo ${OUT_CMD_TIME}
