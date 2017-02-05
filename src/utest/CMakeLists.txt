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

add_subdirectory(test_reporter)
add_subdirectory(test_writer)
add_subdirectory(test_thread)

add_library(utest-core OBJECT
    test.cpp
    test_assert.cpp
    test_case.cpp
    test_exception.cpp
    test_list.cpp
    test_thread.cpp
    test_number.cpp
    test_value.cpp
    test_params.cpp
    test_register.cpp
    test_reporter.cpp
    test_runner.cpp
    test_string.cpp
    test_suite.cpp
    test_writer.cpp
)

add_library(utest STATIC
    $<TARGET_OBJECTS:utest-core>
    $<TARGET_OBJECTS:utest-threads>
    $<TARGET_OBJECTS:utest-reporters>
    $<TARGET_OBJECTS:utest-writers>
)

set_target_properties(utest PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

add_library(utest-main STATIC test_main.cpp)

set_target_properties(utest-main PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

install(TARGETS utest utest-main
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
    set_source_files_properties(
        test.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-global-constructors"
    )

    set_source_files_properties(
        test_writer.cpp
        test_thread.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-exit-time-destructors"
    )

    set_source_files_properties(
        test_reporter.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-global-constructors -Wno-exit-time-destructors"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set_source_files_properties(
        test_register.cpp
        test_reporter.cpp
        test_writer.cpp
        PROPERTIES
        COMPILE_FLAGS "-Wno-suggest-attribute=const"
    )
endif()
