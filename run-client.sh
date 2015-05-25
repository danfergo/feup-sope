#!/usr/bin/env bash

cd src
make -s

cd ../bin
./cliente /e 3
