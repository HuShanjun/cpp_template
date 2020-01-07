#!/bin/bash

mk_dir(){
    if [ ! -d $1 ]; then
        mkdir $1
    fi
}

mk_dir 'build'
mk_dir 'bin'

cd build
make clean
cmake ..
make -j8