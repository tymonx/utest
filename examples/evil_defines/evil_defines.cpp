/*!
 * @copright
 * Copyright (c) 2017, Tymoteusz Blazejczyk
 * All rights reserved.
 *
 * @copright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * @copright
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * @copright
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * @copright
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * @copright
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file examples/evil_defines.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>
#include <utest/test_extension/evil_defines.hpp>

UTEST_RUNNER()
    UTEST_SUITE("integer compare")
        int x = 0;

        UTEST_CASE_CONTEXT(x);

        UTEST_CASE_SETUP()
            UTEST_CONTEXT(int) = 5;
        UTEST_CASE_SETUP_END

        UTEST_CASE("equal")
            UTEST_EXPECT_EQ(UTEST_CONTEXT(int), 5);
        UTEST_CASE_END

        UTEST_CASE_SETUP()
            UTEST_CONTEXT(int) = 4;
        UTEST_CASE_SETUP_END

        UTEST_CASE("not equal")
            UTEST_EXPECT_NE(UTEST_CONTEXT(int), 5);
        UTEST_CASE_END
    UTEST_SUITE_END
UTEST_RUNNER_END
