#! /bin/bash

set -e

cpp_v='-std=c++23'
cflag='-fno-omit-frame-pointer -fno-exceptions -fno-rtti'
optimize='-O0 -g3 -fsanitize=address,undefined,leak'
cc='clang++'
lib='-stdlib=libstdc++'

if (( $1 == 1 ))
then
    optimize=-O1
fi

$cc $cpp_v $cflag $optimize json.cpp -o test.out $lib
# $cc $cpp_v $cflag $optimize test.cpp -o test.out $lib -ljsoncpp