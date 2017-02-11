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
 * @file utest/test_extension/evil_defines.hpp
 *
 * @brief Test extension with evil test macros
 */

#ifndef UTEST_TEST_EXTENSION_EVIL_DEFINES_HPP
#define UTEST_TEST_EXTENSION_EVIL_DEFINES_HPP

#define UTEST_RUNNER() \
    static utest::TestRunner _g_test_runner(\
            [] (utest::TestSuite& _test_suite) {\
        _test_suite.file(__FILE__).line(__LINE__);

#define UTEST_RUNNER_END });

#define UTEST_SUITE(_test_name) \
    _test_suite.file(__FILE__).line(__LINE__).name(_test_name).run(\
            [] (utest::TestCase& _test_case) {

#define UTEST_SUITE_END });

#define UTEST_SUITE_CONTEXT(...) \
    _test_suite.context(__VA_ARGS__)

#define UTEST_CASE(_test_name) \
    _test_case.file(__FILE__).line(__LINE__).name(_test_name).run(\
            [] (utest::TestParams& _test_params) {

#define UTEST_CASE_END });

#define UTEST_CASE_CONTEXT(...) \
    _test_case.context(__VA_ARGS__)

#define UTEST_CASE_SETUP() \
    _test_case.setup([] (utest::TestParams& _test_params) {

#define UTEST_CASE_SETUP_END });

#define UTEST_CASE_TEARDOWN() \
    _test_case.teardown([] (utest::TestParams& _test_params) {

#define UTEST_CASE_TEARDOWN_END });

#define UTEST_CONTEXT(...) \
    _test_params.context<__VA_ARGS__>()

#define UTEST_EXPECT_EQ(val1, val2) \
    utest::TestAssert{_test_params}.file(__FILE__).line(__LINE__).fatal(false)\
        .equal(val1, val2)

#define UTEST_EXPECT_NE(val1, val2) \
    utest::TestAssert{_test_params}.file(__FILE__).line(__LINE__).fatal(false)\
        .not_equal(val1, val2)

#define UTEST_ASSERT_EQ(val1, val2) \
    utest::TestAssert{_test_params}.file(__FILE__).line(__LINE__).fatal(true)\
        .equal(val1, val2)

#define UTEST_ASSERT_NE(val1, val2) \
    utest::TestAssert{_test_params}.file(__FILE__).line(__LINE__).fatal(true)\
        .not_equal(val1, val2)

#endif /* UTEST_TEST_EXTENSION_EVIL_DEFINES_HPP */
