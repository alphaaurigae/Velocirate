#!/bin/bash

INPUT_DIR='input'
DB_DIR='db'


mkdir ${INPUT_DIR}
mkdir ${DB_DIR}

echo -e "${BOLD}${BRIGHT_WHITE}bin/velocirate --inputpath input/ --database db/${RESET}"

bin/velocirate --inputpath input/ --database db/

ls -la $INPUT_DIR
ls -la $DB_DIR