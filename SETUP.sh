#!/bin/bash

mkdir lib
mkdir include
mkdir build
# go to engine/
cd engine/

# install "abc"
./abc.script
./KCbox.script

# back to lsv_final/
cd ../../

# make approximate
make
