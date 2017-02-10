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

function download_qemu {
    mkdir -p $QEMU_ROOT/archive

    if [ ! -e "$QEMU_ROOT/archive/qemu-$QEMU_VERSION.tar.xz" ]; then
        wget http://download.qemu-project.org/qemu-$QEMU_VERSION.tar.xz \
            -O $QEMU_ROOT/archive/qemu-$QEMU_VERSION.tar.xz
    else
        echo "Qemu already downloaded"
    fi
}

function unpack_qemu {
    if [ ! -d "$QEMU_ROOT/$QEMU_VERSION" ]; then
        mkdir -p $QEMU_ROOT/$QEMU_VERSION && tar \
            -xf $QEMU_ROOT/archive/qemu-$QEMU_VERSION.tar.xz \
            -C $QEMU_ROOT/$QEMU_VERSION --strip-components 1
    else
        echo "Qemu already unpacked"
    fi
}

function build_qemu {
    if [ ! -f "$QEMU_ROOT/$QEMU_VERSION/bin/qemu-arm" ]; then
        cd $QEMU_ROOT/$QEMU_VERSION
        ./configure --target-list=arm-linux-user --prefix=`pwd`
        make
        make install
        cd -
    else
        echo "Qemu already installed"
    fi
}

function download_toolchain {
    mkdir -p $TOOLCHAIN_ROOT

    if [ ! -e "$TOOLCHAIN_ROOT/$TOOLCHAIN_TAR" ]; then
        wget $TOOLCHAIN_URL/$TOOLCHAIN_TAR -O $TOOLCHAIN_ROOT/$TOOLCHAIN_TAR
    fi
}

function install_toolchain {
    if [ ! -d "$TOOLCHAIN_DIR" ]; then
        mkdir -p $TOOLCHAIN_DIR && tar -xf $TOOLCHAIN_ROOT/$TOOLCHAIN_TAR \
            -C $TOOLCHAIN_DIR --strip-components 1
    fi
}

case $TOOLCHAIN in
*-arm-none-eabi)
    echo "Prepare tools..."

    QEMU_ROOT=$HOME/qemu
    QEMU_VERSION=2.8.0

    TOOLCHAIN_ROOT=$HOME/toolchains
    TOOLCHAIN_DIR=/tmp/gcc-arm-none-eabi

    download_qemu
    unpack_qemu
    build_qemu

    case $VERSION in
    4.7*)
        TOOLCHAIN_URL=https://launchpad.net/gcc-arm-embedded/4.7/4.7-2014-q2-update/+download
        TOOLCHAIN_TAR=gcc-arm-none-eabi-4_7-2014q2-20140408-linux.tar.bz2

        download_toolchain
        install_toolchain
        ;;
    4.8*)
        TOOLCHAIN_URL=https://launchpad.net/gcc-arm-embedded/4.8/4.8-2014-q3-update/+download
        TOOLCHAIN_TAR=gcc-arm-none-eabi-4_8-2014q3-20140805-linux.tar.bz2

        download_toolchain
        install_toolchain
        ;;
    4.9*)
        TOOLCHAIN_URL=https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update/+download
        TOOLCHAIN_TAR=gcc-arm-none-eabi-4_9-2015q3-20150921-linux.tar.bz2

        download_toolchain
        install_toolchain
        ;;
    5.4*)
        TOOLCHAIN_URL=https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q3-update/+download
        TOOLCHAIN_TAR=gcc-arm-none-eabi-5_4-2016q3-20160926-linux.tar.bz2

        download_toolchain
        install_toolchain
        ;;
    *)
        TOOLCHAIN_URL=https://developer.arm.com/-/media/Files/downloads/gnu-rm/6-2016q4
        TOOLCHAIN_TAR=gcc-arm-none-eabi-6_2-2016q4-20161216-linux.tar.bz2

        download_toolchain
        install_toolchain
        ;;
    esac
    ;;
*)
    echo "Setup step omitted"
    ;;
esac
