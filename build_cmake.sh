#!/bin/bash


BOLD='\033[1m'
BRIGHT_WHITE='\033[1;37m'
RESET='\033[0m' 

BUILD_DIR='build'
BIN_DIR='bin'
BIN_NAME='velocirate'
INPUT_DIR='input'
DB_DIR='db'


configure() {
    echo -e ${BOLD}${BRIGHT_WHITE}"Configuring project with cmake...${RESET}"
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR}
}

build() {
    echo -e ${BOLD}${BRIGHT_WHITE}"Building project...${RESET}"
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc)
}

prep_run() {
    echo -e ${BOLD}${BRIGHT_WHITE}"Create default input / and db/ directories...${RESET}"
    mkdir ${INPUT_DIR}
    mkdir ${DB_DIR}
}


./clean_cmake.sh

configure
build

prep_run

echo ""
echo -e "${BOLD}${BRIGHT_WHITE}bin should be in $BIN_DIR e.g ${BIN_DIR}/${BIN_NAME}${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}E.g:${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}${BIN_DIR}/${BIN_NAME} -h${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}./test.sh to run with default dest for input dir e.g $INPUT_DIR and database dir e.g $DB_DIR"