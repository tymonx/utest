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

function run_tests {
    if [[ $BUILD_TYPE =~ Coverage ]]; then
        echo "Run code coverage"
        make code_coverage
        coveralls-lcov coverage.info
    else
        echo "Run tests"
        make test
    fi
}

if [ -z ${TOOLCHAIN+x} ]; then
    TOOLCHAIN=gcc
fi

if [ -z ${C_COMPILER+x} ]; then
    C_COMPILER=gcc
fi

if [ -z ${CXX_COMPILER+x} ]; then
    CXX_COMPILER=g++
fi

if [ -z ${BUILD_TYPE+x} ]; then
    BUILD_TYPE=Release
fi

case $TOOLCHAIN in
gcc)
    mkdir -p build && cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DEXAMPLES=ON -DTESTS=ON -DTHREADS=OFF \
        -DWARNINGS_INTO_ERRORS=ON .. && make
    run_tests
    cd -
    ;;
clang)
    mkdir -p build && cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DEXAMPLES=ON -DTESTS=ON \
        -DWARNINGS_INTO_ERRORS=ON .. && make
    run_tests
    cd -
    ;;
gcc-arm-none-eabi)
    mkdir -p build && cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DEXAMPLES=ON -DTESTS=ON \
        -DSEMIHOSTING=ON -DWARNINGS_INTO_ERRORS=ON .. && make
    run_tests
    cd -
    ;;
clang-arm-none-eabi)
    mkdir -p build && cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-clang-arm-none-eabi.cmake \
        -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DEXAMPLES=ON -DTESTS=ON -DLTO=OFF \
        -DSEMIHOSTING=ON -DWARNINGS_INTO_ERRORS=ON .. && make
    run_tests
    cd -
    ;;
*)
    ;;
esac
