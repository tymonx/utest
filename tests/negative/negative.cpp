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
 * @file tests/negative.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>

#include <cstdlib>

using namespace utest;

#if defined(UTEST_USE_EXCEPTIONS)
#define THROW(_throw_exception)   do {throw _throw_exception;} while(0)
#else
#define THROW(_throw_exception)   do { } while(0)
#endif

struct DummyException { };

int main() {
    return TestStatus::FAIL == Test{}.color().run().status()
        ? EXIT_SUCCESS : EXIT_FAILURE;
}

static TestRunner g([] (TestSuite& test_suite) {
    test_suite.file(__FILE__).line(__LINE__).fatal(false)

    .name("basic").run([] (TestCase& test_case) {
        test_case.file(__FILE__).line(__LINE__).fatal(false)

        .name("fail").run([] (TestParams& p) {
            TestAssert{p}.fail();
        })

        .name("fatal fail").run([] (TestParams& p) {
            TestAssert{p}.fatal(true).fail();
        })

        .name("explanation").run([] (TestParams& p) {
            TestAssert{p}.fail() << "explanation"
                << 1u
                << -1
                << float(0.333)
                << double(2.111)
                << true
                << false
                << &p
                << static_cast<const void*>(nullptr)
                << nullptr;
        })

        .name("set").run([] (TestParams& p) {
            TestAssert{p}.file(__FILE__).line(__LINE__).fatal(false).fail();
        })

        .name("boolean").run([] (TestParams& p) {
            TestAssert{p}.is_true(false);
            TestAssert{p}.is_false(true);
        })

        .name("integral").run([] (TestParams& p) {
            TestAssert{p}.equal(1, 2);
            TestAssert{p}.not_equal(-1, -1);
            TestAssert{p}.less_than(1, -1);
            TestAssert{p}.less_than_or_equal(1, -1);
            TestAssert{p}.less_than_or_equal(3, -1);
            TestAssert{p}.greater_than(-1, 1);
            TestAssert{p}.greater_than_or_equal(-1, 1);
            TestAssert{p}.greater_than_or_equal(0, 1);
        })

        .name("floating").run([] (TestParams& p) {
            TestAssert{p}.equal(1.0, 1.1);
            TestAssert{p}.equal(1.1, 0.9);
            TestAssert{p}.not_equal(1.7, 1.7);
            TestAssert{p}.less_than(1.4, -1.3);
            TestAssert{p}.less_than_or_equal(1.8, -1.9);
            TestAssert{p}.less_than_or_equal(-1.0, -1.1);
            TestAssert{p}.greater_than(0.9, -1.4);
            TestAssert{p}.greater_than_or_equal(-0.1, 0.3);
            TestAssert{p}.greater_than_or_equal(0.8, 0.9);
            TestAssert{p}.is_true(TestNumber{0.0}.is_signed());
            TestAssert{p}.is_true(TestNumber{-0.0}.is_unsigned());
        })

        .name("string").run([] (TestParams& p) {
            TestNumber::Buffer buffer;

            TestAssert{p}.equal("Test", "Text");
            TestAssert{p}.not_equal("Test", "Test");
            TestAssert{p}.equal(TestString{"TEST"}.ignore_case(), "Text");
            TestAssert{p}.not_equal(TestString{"TEST"}.ignore_case(), "Test");
            TestAssert{p}.equal(5, TestString::length("Test"));
            TestAssert{p}.equal("0.0", to_string(0.1, buffer));
        })

        .name("exception").run([] (TestParams& p) {
            TestAssert{p}.no_throw([] (TestParams&) {
                THROW(std::runtime_error("error!"));
            });

            TestAssert{p}.no_throw([] (TestParams&) {
                THROW(DummyException());
            });

            TestAssert{p}.any_throw([] (TestParams&) { });

            TestAssert{p}.expected_throw<std::bad_exception>([] (TestParams&) {
                THROW(std::runtime_error("error!"));
            });

            TestAssert{p}.expected_throw<std::bad_exception>([] (TestParams&) {
                THROW(DummyException());
            });

            TestAssert{p}.expected_throw<std::bad_exception>([] (TestParams&) {

            });

            TestAssert{p}.expected_throw<std::exception>([] (TestParams&) {

            });

            TestAssert{p}.expected_throw<std::exception>([] (TestParams&) {
                THROW(DummyException());
            });
        })

        .name("exception from test case 1").run([] (TestParams&) {
            THROW(std::runtime_error("error!"));
        })

        .name("exception from test case 2").run([] (TestParams&) {
            THROW(DummyException());
        })

        .setup([] (TestParams&) {
            THROW(std::runtime_error("error!"));
        })

        .name("exception from test case setup 1").run(nullptr)

        .setup([] (TestParams&) {
            THROW(DummyException());
        })

        .name("exception from test case setup 2").run(nullptr)

        .setup(nullptr)
        .teardown([] (TestParams&) {
            THROW(std::runtime_error("error!"));
        })

        .name("exception from test case teardown 1").run(nullptr)

        .setup(nullptr)
        .teardown([] (TestParams&) {
            THROW(DummyException());
        })

        .name("exception from test case teardown 2").run(nullptr)
        ;

        THROW(std::runtime_error("error!"));
    })

    .name("exception from test suite").run([] (TestCase&) {
        THROW(DummyException());
    });

    THROW(std::runtime_error("error!"));
});
