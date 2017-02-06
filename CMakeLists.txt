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

cmake_minimum_required(VERSION 3.1)
project(utest C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

if (CMAKE_TOOLCHAIN_FILE AND CMAKE_SYSTEM_NAME MATCHES "Generic")
    option(EXCEPTIONS "Enable/disable exceptions" OFF)
    option(RTTI "Enable/disable rtti" OFF)
    option(THREADS "Enable/disable threads" OFF)
else()
    option(EXCEPTIONS "Enable/disable exceptions" ON)
    option(RTTI "Enable/disable rtti" ON)
    option(THREADS "Enable/disable threads" ON)
endif()

if (CMAKE_BUILD_TYPE MATCHES "MinSizeRel")
    option(LTO "Enable/disable link time optimization" ON)
else()
    option(LTO "Enable/disable link time optimization" OFF)
endif()

option(EXAMPLES "Enable/disable examples" OFF)

include(AddGnuCompiler)
include(AddClangCompiler)
include(AddThreads)

include_directories(include)

install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")

add_subdirectory(src)

if (EXAMPLES)
    add_subdirectory(examples)
endif()
