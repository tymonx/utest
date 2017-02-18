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
 * @file tests/positive.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>

using namespace utest;

#if defined(UTEST_USE_EXCEPTIONS)
#define THROW(_throw_exception)   do {throw _throw_exception;} while(0)
#else
#define THROW(_throw_exception)   do { } while(0)
#endif

static TestRunner g([] (TestSuite& test_suite) {
    test_suite.file(__FILE__).line(__LINE__).fatal(false)

    .name("basic").run([] (TestCase& test_case) {
        test_case.file(__FILE__).line(__LINE__).fatal(false)

        .name("dummy").run([] (TestParams& p) {
            TestAssert{p};
        })

        .name("explanation").run([] (TestParams& p) {
            TestAssert{p} << "explanation"
                << 1u
                << -1
                << float(0.333)
                << double(2.111)
                << true
                << false
                << &p
                << nullptr;
        })

        .name("set").run([] (TestParams& p) {
            TestAssert{p}.file(__FILE__).line(__LINE__).fatal(false);
            TestAssert{p}.equal(__FILE__, TestAssert{p}.file());
            TestAssert{p}.equal(0u, TestAssert{p}.line());
            TestAssert{p}.equal(TestStatus::PASS, TestAssert{p}.status());
        })

        .name("boolean").run([] (TestParams& p) {
            TestAssert{p}.is_true(true);
            TestAssert{p}.is_false(false);
        })

        .name("integral").run([] (TestParams& p) {
            TestAssert{p}.equal(1, 1);
            TestAssert{p}.not_equal(-1, 1);
            TestAssert{p}.less_than(-1, 1);
            TestAssert{p}.less_than_or_equal(-1, 1);
            TestAssert{p}.less_than_or_equal(-1, -1);
            TestAssert{p}.greater_than(1, -1);
            TestAssert{p}.greater_than_or_equal(1, -1);
            TestAssert{p}.greater_than_or_equal(1, 1);
        })

        .name("floating").run([] (TestParams& p) {
            TestAssert{p}.equal(1.0, 1);
            TestAssert{p}.equal(1.1, 1.1);
            TestAssert{p}.not_equal(1.6, 1.7);
            TestAssert{p}.less_than(-1.4, 1.3);
            TestAssert{p}.less_than_or_equal(-1.8, 1.9);
            TestAssert{p}.less_than_or_equal(-1.0, -1.0);
            TestAssert{p}.greater_than(0.9, -1.4);
            TestAssert{p}.greater_than_or_equal(0.1, -0.3);
            TestAssert{p}.greater_than_or_equal(1.0, 1.0);
        })

        .name("string").run([] (TestParams& p) {
            TestString::Buffer buffer;

            TestAssert{p}.equal("Test", "Test");
            TestAssert{p}.not_equal("Test", "Text");
            TestAssert{p}.equal(TestString{"TEST"}.ignore_case(), "Test");
            TestAssert{p}.not_equal(TestString{"TEST"}.ignore_case(), "Text");
            TestAssert{p}.equal(4u, TestString::length("Test"));
            TestAssert{p}.equal("0.0", to_string(0.0, buffer));
            TestAssert{p}.equal("1.0", to_string(1.0, buffer));
            TestAssert{p}.equal("-1.0", to_string(-1.0, buffer));
            TestAssert{p}.equal("1.0e1", to_string(10.0, buffer));
            TestAssert{p}.equal("-1.0e1", to_string(-10.0, buffer));
            TestAssert{p}.equal("-1.0e-1", to_string(-0.10, buffer));
        })

        .name("exception").run([] (TestParams& p) {
            TestAssert{p}.no_throw([] (TestParams&) { });

            TestAssert{p}.any_throw([] (TestParams&) {
                THROW(std::runtime_error("error!"));
            });

            TestAssert{p}.expected_throw<std::bad_exception>([] (TestParams&) {
                THROW(std::bad_exception());
            });

            TestAssert{p}.expected_throw<std::exception>([] (TestParams&) {
                THROW(std::runtime_error("error!"));
            });
        })
        ;
    });
});
