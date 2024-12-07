#!/bin/bash

INPUT_DIR='input'
DB_DIR='db'

mkdir ${INPUT_DIR}
mkdir ${DB_DIR}

bin/velocirate --inputpath input/ --database db/