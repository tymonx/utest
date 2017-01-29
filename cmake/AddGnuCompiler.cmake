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

if (NOT CMAKE_CXX_COMPILER_ID MATCHES GNU)
    return()
endif ()

set(CXX_FLAGS
    -std=c++14
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
            -Wl,--strip-all
        )

        if (LTO)
            set(CXX_FLAGS ${CXX_FLAGS} -flto)
            set(LINKER_FLAGS ${LINKER_FLAGS} -flto)
        endif()
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
            -Wl,--strip-all
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
        -Wl,--strip-all
    )
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CXX_FLAGS ${CXX_FLAGS}
        -O0
        -g3
        -ggdb
    )

    set(LINKER_FLAGS " ")
elseif (CMAKE_BUILD_TYPE MATCHES "Coverage")
    set(CXX_FLAGS ${CXX_FLAGS}
        -O0
        -g
        --coverage
    )

    set(LINKER_FLAGS ${LINKER_FLAGS}
        --coverage
    )
endif()

if (NOT EXCEPTIONS)
    set(CXX_FLAGS ${CXX_FLAGS} "-fno-exceptions")
endif()

if (NOT RTTI)
    set(CXX_FLAGS ${CXX_FLAGS} "-fno-rtti")
endif()

set(CXX_FLAGS ${CXX_FLAGS}
    -Waggregate-return
    -Wall
    -Wcast-qual
    -Wcomments
    -Wconditionally-supported
    -Wconversion
    -Wctor-dtor-privacy
    -Wdate-time
    -Wdisabled-optimization
    -Wdouble-promotion
    -Weffc++
    -Wendif-labels
    -Wenum-compare
    -Wextra
    -Wfloat-equal
    -Wformat=2
    -Winline
    -Winvalid-pch
    -Wlogical-op
    -Wno-long-long
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wnoexcept
    -Wold-style-cast
    -Wopenmp-simd
    -Woverloaded-virtual
    -Wpacked
    -Wpedantic
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wsign-promo
    -Wstack-protector
    -Wstrict-null-sentinel
    -Wstrict-overflow=2
    -Wsuggest-attribute=const
    -Wsuggest-attribute=noreturn
    -Wsuggest-attribute=format
    -Wsuggest-final-methods
    -Wsuggest-final-types
    -Wsuggest-override
    -Wswitch-default
    -Wswitch-enum
    -Wtrampolines
    -Wundef
    -Wuninitialized
    -Wunknown-pragmas
    -Wunused
    -Wvector-operation-performance
    -Wzero-as-null-pointer-constant
)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 6.3)
    set(CXX_FLAGS ${CXX_FLAGS}
        -Wduplicated-cond
        -Whsa
        -Wignored-attributes
        -Wnull-dereference
        -Wplacement-new=2
        -Wvirtual-inheritance
    )
endif()

string(REPLACE ";" " " LINKER_FLAGS "${LINKER_FLAGS}")
