#!/bin/bash

BUILD_DIR='build'
BIN_DIR='bin'
INPUT_DIR='input'
DB_DIR='db'

clean() {
    echo "Cleaning build, bin, input and db directories..."
    rm -rf ${BUILD_DIR} ${BIN_DIR} ${DB_DIR} ${DB_DIR}
}

clean
