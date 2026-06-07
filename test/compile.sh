#! /bin/bash

set -e

cpp_v='-std=c++17'
cflag='-fno-omit-frame-pointer -fno-exceptions -fno-rtti'
optimize='-O0 -g3 -fsanitize=address,undefined,leak'
# optimize='-O0 -g3'
cc='clang++'
lib='-stdlib=libstdc++'

if (( $1 == 1 ))
then
    optimize='-O1 -DNDEBUG -g'
elif (( $1 == 2 ))
then
    optimize='-O2 -DNDEBUG -g'
fi

# cmd="$cc $cpp_v $cflag $optimize SAX-parser.cpp -o test.out $lib"
cmd="$cc $cpp_v $cflag $optimize test.cpp -o test.out $lib"

echo ${cmd}
$cmd