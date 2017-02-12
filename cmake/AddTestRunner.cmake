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

if (NOT TESTS)
    return()
endif()

set(TEST_RUNNER)

if (CMAKE_SYSTEM_NAME MATCHES "Generic")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "cortex*")
        find_program(QEMU_ARM_COMMAND qemu-arm)

        if (QEMU_ARM_COMMAND)
            message(STATUS "Tests will be run through ${QEMU_ARM_COMMAND}")
            set(TEST_RUNNER ${QEMU_ARM_COMMAND})
        else()
            message(WARNING "Tests cannot be run, qemu-arm not found")
            unset(TEST_RUNNER)
        endif()
    endif()
elseif (MEMORY_CHECK AND CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_program(VALGRIND_COMMAND valgrind)

    if (VALGRIND_COMMAND)
        message(STATUS "Tests will be run through ${VALGRIND_COMMAND}")
        set(TEST_RUNNER ${VALGRIND_COMMAND}
            -v
            --tool=memcheck
            --leak-check=full
            --show-reachable=yes
            --track-origins=yes
            --error-exitcode=1
        )
    endif()
endif()
