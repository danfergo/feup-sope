#!/usr/bin/env bash

# cria folder
rm -rf bin
mkdir bin

# compile
gcc src/balcao.c  -Wall -o bin/balcao
gcc src/cliente.c  -Wall -o bin/cliente
gcc src/ger_cl.c  -Wall -o bin/ger_cl


# run
./bin/ger_cl mempatilhada 50