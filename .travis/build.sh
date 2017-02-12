#!/bin/bash
# Copyright (c) 2017, Tymoteusz Blazejczyk
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

set -e
set -u

if [ -z ${TOOLCHAIN+x} ]; then
    TOOLCHAIN=gcc
fi

if [ -z ${C_COMPILER+x} ]; then
    C_COMPILER=gcc
fi

if [ -z ${CXX_COMPILER+x} ]; then
    CXX_COMPILER=g++
fi

case $TOOLCHAIN in
gcc)
    mkdir -p build
    cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON -DTHREADS=OFF -DTESTS=ON .. && make
    valgrind --version
    valgrind --help
    valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all \
        --errors-for-leak-kinds=all --error-exitcode=1 ./tests/test_simple

    make test
    cd -
    ;;
clang)
    mkdir -p build
    cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON -DTESTS=ON .. && make
    make test
    cd -
    ;;
gcc-arm-none-eabi)
    mkdir -p build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake \
        -DEXAMPLES=ON -DSEMIHOSTING=ON -DTESTS=ON .. && make
    make test
    cd -
    ;;
clang-arm-none-eabi)
    mkdir -p build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-clang-arm-none-eabi.cmake \
        -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON -DSEMIHOSTING=ON -DTESTS=ON .. && make
    make test
    cd -
    ;;
*)
    ;;
esac
