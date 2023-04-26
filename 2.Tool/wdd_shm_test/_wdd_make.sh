#!/bin/bash

if [ "clean" == "$1" ]; then
# if [ "$1" -n ]; then
    echo "clean"
    rm -f shm_master shm_slave
    exit
fi




echo "make test"
gcc -o wdd_shm_test wdd_shm_test.c



