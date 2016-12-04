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

if (NOT CMAKE_CXX_COMPILER_ID MATCHES Clang)
    return()
endif ()

set(CXX_FLAGS
    -std=c++14
    -fPIE
    -fPIC
)

set(LINKER_FLAGS)

if (CMAKE_BUILD_TYPE MATCHES "Release" OR NOT CMAKE_BUILD_TYPE)
    if (CMAKE_TOOLCHAIN_FILE AND CMAKE_SYSTEM_NAME MATCHES "Generic")
        set(CXX_FLAGS ${CXX_FLAGS}
            -Os
            -s
            -DNDEBUG
            -fdata-sections
            -ffunction-sections
        )

        set(LINKER_FLAGS ${LINKER_FLAGS}
            -Wl,--gc-sections
        )
    else()
        set(CXX_FLAGS ${CXX_FLAGS}
            -O3
            -s
            -DNDEBUG
            -fdata-sections
            -ffunction-sections
            -fstack-protector-strong
        )

        set(LINKER_FLAGS ${LINKER_FLAGS}
            -Wl,--gc-sections
            -z noexecstack
            -z relro
            -z now
        )
    endif()
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    set(CXX_FLAGS ${CXX_FLAGS}
        -Os
        -DNDEBUG
        -fdata-sections
        -ffunction-sections
    )

    set(LINKER_FLAGS ${LINKER_FLAGS}
        -Wl,--gc-sections
    )
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CXX_FLAGS ${CXX_FLAGS}
        -O0
        -g3
        -ggdb
    )
endif()

if (NOT EXCEPTIONS)
    set(CXX_FLAGS ${CXX_FLAGS} "-fno-exceptions")
endif()

if (NOT RTTI)
    set(CXX_FLAGS ${CXX_FLAGS} "-fno-rtti")
endif()

set(CXX_FLAGS ${CXX_FLAGS}
    -Weverything
    -Wno-padded
    -Wno-covered-switch-default
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
)
