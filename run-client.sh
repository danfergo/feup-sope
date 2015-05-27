#!/usr/bin/env bash

cd src
make -s

cd ../bin
./ger_cl /x 10
