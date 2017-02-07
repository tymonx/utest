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

if [ -z ${TARGET+x} ]; then
    TARGET=gcc
fi

if [ -z ${C_COMPILER+x} ]; then
    C_COMPILER=gcc
fi

if [ -z ${CXX_COMPILER+x} ]; then
    CXX_COMPILER=g++
fi

if [ -z ${TOOLCHAIN_TAR+x} ]; then
    TOOLCHAIN_TAR=https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download/gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2
fi

case $TARGET in
gcc)
    mkdir build
    cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON -DTHREADS=OFF .. && make
    ./bin/simple
    ;;
clang)
    mkdir build
    cd build
    cmake -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON .. && make
    ./bin/simple
    ;;
gcc-arm-none-eabi)
    wget ${TOOLCHAIN_TAR} -O /tmp/gcc-arm-none-eabi.tar.bz2

    mkdir /tmp/gcc-arm-none-eabi && tar xvf /tmp/gcc-arm-none-eabi.tar.bz2 \
        -C /tmp/gcc-arm-none-eabi --strip-components 1

    if [[ ! $PATH =~ /tmp/gcc-arm-none-eabi/bin ]]; then
        export PATH=/tmp/gcc-arm-none-eabi/bin:$PATH
    fi

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake \
        -DEXAMPLES=ON -DSEMIHOSTING=ON .. && make
    ;;
clang-arm-none-eabi)
    wget ${TOOLCHAIN_TAR} -O /tmp/gcc-arm-none-eabi.tar.bz2

    mkdir /tmp/gcc-arm-none-eabi && tar xvf /tmp/gcc-arm-none-eabi.tar.bz2 \
        -C /tmp/gcc-arm-none-eabi --strip-components 1

    if [[ ! $PATH =~ /tmp/gcc-arm-none-eabi/bin ]]; then
        export PATH=/tmp/gcc-arm-none-eabi/bin:$PATH
    fi

    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-clang-arm-none-eabi.cmake \
        -DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER \
        -DEXAMPLES=ON -DSEMIHOSTING=ON .. && make
    ;;
*)
    ;;
esac
