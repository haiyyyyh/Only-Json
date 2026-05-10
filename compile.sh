#! /bin/bash

set -e

module_dir='tool'
cpp_v='-std=c++23'
cflag='-g3 -fno-exceptions -fno-rtti'
optimize='-O0 -fsanitize=address,undefined,leak'
# optimize='-O2 -fno-omit-frame-pointer'
compile_module='--precompile'
find_module='-fprebuilt-module-path=./tool'
cc='clang++'
lib='-stdlib=libstdc++'

if (( $# == 2 ))
then
    cflag=-O$2
fi

if (( $1 == 0 || $1 == 2))
then
    cd ${module_dir}
    echo "compile module"
    $cc $cpp_v $cflag $optimize $compile_module depend.cppm -o depend.pcm $lib
    cd ../
fi

if (( $1 == 1 || $1 == 2 ))
then
    echo "compile json"
    $cc $cpp_v $cflag $optimize src.cpp -o test.out $lib $find_module
fi