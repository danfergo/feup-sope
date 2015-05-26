#!/usr/bin/env bash

cd src
make -s

cd ../bin
./cliente /f 3
