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

if (CMAKE_BUILD_TYPE MATCHES "Coverage")
    get_filename_component(COMPILER_ROOT ${CMAKE_CXX_COMPILER} DIRECTORY)
    get_filename_component(COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME)

    if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
        string(REGEX REPLACE "([a-z0-9-]*)clang.*" "\\1"
            COMPILER_PREFIX ${COMPILER_NAME})

        string(REGEX REPLACE ".*clang([a-z0-9-]*)" "\\1"
            COMPILER_SUFFIX ${COMPILER_NAME})

        find_program(GCOV_COMMAND
            ${COMPILER_PREFIX}llvm-cov${COMPILER_SUFFIX}
            llvm-cov${COMPILER_SUFFIX}
            ${COMPILER_PREFIX}llvm-cov
            llvm-cov
            HINTS ${COMPILER_ROOT}
        )

        if (GCOV_COMMAND)
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
            set(LLVM_GCOV_SCRIPT llvm-gcov.sh)

            file(WRITE ${CMAKE_BINARY_DIR}/${LLVM_GCOV_SCRIPT}
                "#!/bin/sh\n"
                "exec ${GCOV_COMMAND} gcov \"$@\"\n"
            )

            file(INSTALL ${CMAKE_BINARY_DIR}/${LLVM_GCOV_SCRIPT}
                DESTINATION ${CMAKE_BINARY_DIR}/bin
                FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)

            set(GCOV_COMMAND ${CMAKE_BINARY_DIR}/bin/llvm-gcov.sh)
        endif()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES GNU)
        string(REGEX REPLACE "([a-z0-9-]*)gcc.*" "\\1"
            COMPILER_PREFIX ${COMPILER_NAME})

        string(REGEX REPLACE ".*gcc([a-z0-9-]*)" "\\1"
            COMPILER_SUFFIX ${COMPILER_NAME})

        find_program(GCOV_COMMAND
            ${COMPILER_PREFIX}gcov${COMPILER_SUFFIX}
            gcov${COMPILER_SUFFIX}
            ${COMPILER_PREFIX}gcov
            gcov
            HINTS ${COMPILER_ROOT}
        )
    else()
        message(FATAL_ERROR
            "Code coverage is only supported by Clang and GNU compiler")
    endif()

    if (NOT GCOV_COMMAND)
        message(FATAL_ERROR "Cannot find gcov command")
    endif()

    message(STATUS "Found gcov: ${GCOV_COMMAND}")

    find_program(LCOV_COMMAND lcov)
    if (NOT LCOV_COMMAND)
        message(FATAL_ERROR "Cannot find lcov command")
    endif()

    message(STATUS "Found lcov: ${LCOV_COMMAND}")

    find_program(GENHTML_COMMAND genhtml)
    if (NOT GENHTML_COMMAND)
        message(FATAL_ERROR "Cannot find genhtml command")
    endif()

    message(STATUS "Found genhtml: ${GENHTML_COMMAND}")

    add_custom_target(code_coverage
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
        COMMAND ${LCOV_COMMAND}
            --zerocounters
            --directory .
        COMMAND ${LCOV_COMMAND}
            --capture
            --initial
            --gcov-tool ${GCOV_COMMAND}
            --base-directory ${CMAKE_SOURCE_DIR}
            --directory .
            --output-file coverage.base
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target test
        COMMAND ${LCOV_COMMAND}
            --capture
            --gcov-tool ${GCOV_COMMAND}
            --base-directory ${CMAKE_SOURCE_DIR}
            --directory .
            --output-file coverage.run
        COMMAND ${LCOV_COMMAND}
            --add-tracefile coverage.base
            --add-tracefile coverage.run
            --output-file coverage.total
        COMMAND ${LCOV_COMMAND}
            --gcov-tool ${GCOV_COMMAND}
            --extract coverage.total */src/*.cpp
            --extract coverage.total */src/*.hpp
            --extract coverage.total */include/utest/*.hpp
            --output-file coverage.info
        COMMAND ${GENHTML_COMMAND}
            --legend
            --title "µTest - unit testing framework"
            --demangle-cpp
            --output-directory coverage
            coverage.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    message(STATUS "Enabled code coverage support")
else ()
    message(STATUS "Disabled code coverage support")
endif()
