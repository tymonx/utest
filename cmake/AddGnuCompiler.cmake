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

message(STATUS "${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

if (NOT EXCEPTIONS)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-exceptions)
endif()

if (NOT RTTI)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)
endif()

if (LTO)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -flto)
endif()

if (WARNINGS_INTO_ERRORS)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Werror)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Release" OR NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")

    set(CMAKE_CXX_FLAGS_RELEASE
        -O3
        -s
        -DNDEBUG
        -fdata-sections
        -ffunction-sections
    )

    if (NOT CMAKE_SYSTEM_NAME MATCHES "Generic")
        set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}
            -fstack-protector-all
        )

        if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
            set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE}
                -fstack-protector-strong
            )
        endif()
    endif()

    set(CMAKE_EXE_LINKER_FLAGS_RELEASE
        -Wl,--gc-sections
        -Wl,--strip-all
        -z noexecstack
        -z relro
        -z now
    )

    string(REPLACE ";" " " CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS_RELEASE
        "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
elseif (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    set(CMAKE_CXX_FLAGS_MINSIZEREL
        -Os
        -DNDEBUG
        -fdata-sections
        -ffunction-sections
    )

    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
        -Wl,--gc-sections
    )

    string(REPLACE ";" " " CMAKE_CXX_FLAGS_MINSIZEREL
        "${CMAKE_CXX_FLAGS_MINSIZEREL}")

    string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
        "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL}")
elseif (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG
        -O0
        -g3
        -ggdb
    )

    set(CMAKE_EXE_LINKER_FLAGS_DEBUG)

    string(REPLACE ";" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS_DEBUG
        "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
elseif (CMAKE_BUILD_TYPE MATCHES "Coverage")
    set(CMAKE_CXX_FLAGS_COVERAGE
        -O0
        -g
        --coverage
    )

    set(CMAKE_EXE_LINKER_FLAGS_COVERAGE)

    string(REPLACE ";" " " CMAKE_CXX_FLAGS_COVERAGE
        "${CMAKE_CXX_FLAGS_COVERAGE}")

    string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS_COVERAGE
        "${CMAKE_EXE_LINKER_FLAGS_COVERAGE}")
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    -fstrict-aliasing
    -pedantic
    -Wall
    -Wcast-qual
    -Wcomments
    -Wconversion
    -Wctor-dtor-privacy
    -Wdisabled-optimization
    -Wendif-labels
    -Wenum-compare
    -Wfloat-equal
    -Wformat=2
    -Wformat-nonliteral
    -Winit-self
    -Winvalid-pch
    -Wlogical-op
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wno-long-long
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wpacked
    -Wparentheses
    -Wpointer-arith
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wsign-promo
    -Wstack-protector
    -Wstrict-null-sentinel
    -Wstrict-overflow=2
    -Wsuggest-attribute=noreturn
    -Wswitch-default
    -Wswitch-enum
    -Wundef
    -Wuninitialized
    -Wunknown-pragmas
    -Wunused
    -Wunused-function
    -Wwrite-strings
)

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6.0)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wduplicated-cond
        -Whsa
        -Wignored-attributes
        -Wmisleading-indentation
        -Wnull-dereference
        -Wplacement-new=2
        -Wshift-negative-value
        -Wshift-overflow=2
        -Wvirtual-inheritance
    )
endif()

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wdouble-promotion
        -Wsized-deallocation
        -Wsuggest-final-methods
        -Wsuggest-final-types
        -Wsuggest-override
        -Wtrampolines
        -Wvector-operation-performance
        -Wzero-as-null-pointer-constant
    )
endif()

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wconditionally-supported
        -Wdate-time
        -Weffc++
        -Wextra
        -Winline
        -Wopenmp-simd
    )
endif()

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wpedantic
        -Wsuggest-attribute=format
    )
endif()

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.6)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wnoexcept
    )
endif()

if (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.9)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
        -Wno-missing-field-initializers
    )
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
