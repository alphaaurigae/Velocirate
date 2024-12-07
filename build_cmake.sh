#!/bin/bash

BUILD_DIR='build'
BIN_DIR='bin'
INPUT_DIR='input'
DB_DIR='db'

configure() {
    echo "Configuring project with cmake..."
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR}
}

build() {
    echo "Building project..."
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc)
}

prep_run() {
    echo "Create input / and db/ dir..."
    mkdir ${INPUT_DIR}
    mkdir ${DB_DIR}
}

configure
build
prep_run