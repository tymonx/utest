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

if (__GCC_ARM_NONE_EABI)
    return()
endif()
set(__GCC_ARM_NONE_EABI 1)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(CMAKE_LINKER arm-none-eabi-ld CACHE STRING "GNU ARM linker")
set(CMAKE_AR arm-none-eabi-ar CACHE STRING "GNU ARM archiver")

execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=libc.a
    OUTPUT_VARIABLE GCC_ARM_NONE_EABI_ROOT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-file-name=liblto_plugin.so
    OUTPUT_VARIABLE GCC_ARM_NONE_EABI_LTO_PLUGIN
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

get_filename_component(GCC_ARM_NONE_EABI_ROOT
    "${GCC_ARM_NONE_EABI_DIR}"
    PATH
)

get_filename_component(GCC_ARM_NONE_EABI_ROOT
    "${GCC_ARM_NONE_EABI_DIR}/.."
    REALPATH
)

set(CMAKE_FIND_ROOT_PATH ${GCC_ARM_NONE_EABI_ROOT})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "Build type")
endif()

if (CMAKE_BUILD_TYPE MATCHES "MinSizeRel" AND NOT LTO)
    set(LTO ON CACHE BOOL "Link time optimization")
endif()

if (NOT CMAKE_SYSTEM_PROCESSOR)
    set(CMAKE_SYSTEM_PROCESSOR "cortex-m3" CACHE STRING "ARM processor type")
endif()

message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")

set(GCC_ARM_NONE_EABI_FLAGS "-mthumb -mcpu=${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_C_FLAGS "${GCC_ARM_NONE_EABI_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${GCC_ARM_NONE_EABI_FLAGS} ${CMAKE_CXX_FLAGS}")

if (STARTUP_CODE)
    get_filename_component(STARTUP_CODE "${STARTUP_CODE}" REALPATH)

    if (EXISTS "${STARTUP_CODE}")
        message(STATUS "STARTUP_CODE=${STARTUP_CODE}")
        set(STARTUP_CODE "${STARTUP_CODE}" CACHE FILEPATH "Startup code file")
    else()
        message(FATAL_ERROR "STARTUP_CODE=${STARTUP_CODE} is not found")
    endif()
else()
    message(STATUS "STARTUP_CODE variable is not defined")
endif()

if (LINKER_SCRIPT)
    get_filename_component(LINKER_SCRIPT "${LINKER_SCRIPT}" REALPATH)

    if (EXISTS "${LINKER_SCRIPT}")
        message(STATUS "LINKER_SCRIPT=${LINKER_SCRIPT}")
        set(LINKER_SCRIPT "${LINKER_SCRIPT}" CACHE FILEPATH "Linker script file")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT}")
    else()
        message(FATAL_ERROR "LINKER_SCRIPT=${LINKER_SCRIPT} is not found")
    endif()
else()
    message(STATUS "LINKER_SCRIPT variable is not defined")
endif()

if (SYSTEM_INIT)
    get_filename_component(SYSTEM_INIT "${SYSTEM_INIT}" REALPATH)

    if (EXISTS "${SYSTEM_INIT}")
        message(STATUS "SYSTEM_INIT=${SYSTEM_INIT}")
        set(SYSTEM_INIT "${SYSTEM_INIT}" CACHE FILEPATH
            "System initialization source code file")
    else()
        message(FATAL_ERROR "SYSTEM_INIT=${SYSTEM_INIT} is not found")
    endif()
else()
    message(STATUS "SYSTEM_INIT variable is not defined. "
        "System initialization runtime won't be used")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -D__NO_SYSTEM_INIT")
endif()

if (START)
    message(STATUS "START=${START}")
    set(START "${START}" CACHE STRING "Start function name")
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -D__START=${START}")
else()
    message(STATUS "START variable is not defined. Used default")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=nano.specs")

if (SEMIHOSTING)
    message(STATUS "Semihosting is enabled")
    set(SEMIHOSTING ON CACHE BOOL "Enable/disable semihosting")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --specs=rdimon.specs")
else()
    message(STATUS "Semihosting is disabled")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" CACHE STRING "C compile flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "C++ compile flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
    CACHE STRING "Linker flags")

set(CMAKE_C_LINK_EXECUTABLE
    <CMAKE_C_COMPILER>
    <FLAGS>
    <CMAKE_C_LINK_FLAGS>
    <LINK_FLAGS>
    -Wl,-Map=<TARGET>.map
    <OBJECTS>
    ${STARTUP_CODE}
    ${SYSTEM_INIT}
    -o <TARGET>
    <LINK_LIBRARIES>
)

set(CMAKE_C_CREATE_STATIC_LIBRARY
    <CMAKE_AR>
    qc
    --plugin ${GCC_ARM_NONE_EABI_LTO_PLUGIN}
    <TARGET>
    <LINK_FLAGS>
    <OBJECTS>
)

string(REPLACE ";" " " CMAKE_C_CREATE_STATIC_LIBRARY
    "${CMAKE_C_CREATE_STATIC_LIBRARY}")

set(CMAKE_CXX_CREATE_STATIC_LIBRARY ${CMAKE_C_CREATE_STATIC_LIBRARY})

string(REPLACE "_C_" "_CXX_" CMAKE_CXX_LINK_EXECUTABLE
    "${CMAKE_C_LINK_EXECUTABLE}")

string(REPLACE ";" " " CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_LINK_EXECUTABLE}")
string(REPLACE ";" " " CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE}")

set(BUILD_SHARED_LIBS OFF)
