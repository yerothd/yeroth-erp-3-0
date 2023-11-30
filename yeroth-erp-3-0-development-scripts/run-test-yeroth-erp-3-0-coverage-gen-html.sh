#!/bin/sh
#@author: Dr.-Ing. XAVIER NOUNDOU

############################################################################################################
# You should edit the following 3 paths when necessary
############################################################################################################

# Get the path to the current folder
SCRIPT_DIR=$(dirname $0)

# SRC_DIR is the directory containing the .gcno files (%{buildDir} in Qt Creator)
SRC_DIR="${YEROTH_ERP_3_0_HOME_FOLDER}"

# COV_DIR is the directory where the coverage results will be stored
COV_DIR="${YEROTH_ERP_3_0_HOME_FOLDER}/yeroth-tests-coverage-information"

############################################################################################################

# Path where the HTML files should be saved
HTML_RESULTS="${COV_DIR}""/html"

# Create the html folder if it does not exists
mkdir -p ${HTML_RESULTS}

# Generate our initial info
lcov -d "${SRC_DIR}" -c -o "${COV_DIR}/yeroth-tests-coverage.info"
 
# Remove some paths/files which we don't want to calculate the code coverage (e.g. third party libraries) and generate a new coverage file filtered (feel free to edit it when necessary)
lcov -r "${COV_DIR}/yeroth-tests-coverage.info" "*Qt*.framework*" "*.h" "*/tests/*" "*.moc" "*moc_*.cpp" "*/test/*" "*/build*/*" -o "${COV_DIR}/yeroth-tests-coverage-filtered.info"
 
# Generate the HTML files
genhtml -o "${HTML_RESULTS}" \
	--title "JEOVAH_NISSI_COVERAGE" \
	--demangle-cpp --num-spaces 2 --sort \
	--function-coverage --branch-coverage --legend \	
	"${COV_DIR}/yeroth-tests-coverage-filtered.info"
 
# Reset our counts
lcov -d "${COV_DIR}" -z
 
# Open the index.html
firefox "${HTML_RESULTS}/index.html"&
