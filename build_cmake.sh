#!/bin/bash

# Define directories
BUILD_DIR=build
BIN_DIR=bin

# Run cmake to configure the build
configure() {
    echo "Configuring project with cmake..."
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR}
}

# Build the project
build() {
    echo "Building project..."
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc)
}

# Main function
configure
build