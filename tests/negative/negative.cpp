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

#include "test_utilities.hpp"
#include <utest/utest.hpp>

using namespace utest;

struct DummyException { };

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

        .name("no file").run([] (TestParams& p) {
            TestAssert{p}.file(nullptr).fatal(true).fail();
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
            TestAssert{p}.equal(TestStatus::FAIL, TestAssert{p}.status());
        })

        .name("boolean").run([] (TestParams& p) {
            TestAssert{p}.is_true(false);
            TestAssert{p}.is_false(true);
        })

        .name("null").run([] (TestParams& p) {
            TestAssert{p}.not_equal(nullptr, nullptr);
            TestAssert{p}.equal(&p, nullptr);
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

            TestAssert{p}.equal(static_cast<signed char>(1),
                    static_cast<signed char>(2));

            TestAssert{p}.equal(static_cast<unsigned char>(1),
                    static_cast<unsigned char>(2));

            TestAssert{p}.equal(static_cast<signed short>(1),
                    static_cast<signed short>(2));

            TestAssert{p}.equal(static_cast<unsigned short>(1),
                    static_cast<unsigned short>(2));

            TestAssert{p}.equal(static_cast<signed int>(1),
                    static_cast<signed int>(2));

            TestAssert{p}.equal(static_cast<unsigned int>(1),
                    static_cast<unsigned int>(2));

            TestAssert{p}.equal(static_cast<signed long>(1),
                    static_cast<signed long>(2));

            TestAssert{p}.equal(static_cast<unsigned long>(1),
                    static_cast<unsigned long>(2));

            TestAssert{p}.equal(static_cast<signed long long>(1),
                    static_cast<signed long long>(2));

            TestAssert{p}.equal(static_cast<unsigned long long>(1),
                    static_cast<unsigned long long>(2));
        })

        .name("floating").run([] (TestParams& p) {
            TestAssert{p}.equal(1.0, 1.1);
            TestAssert{p}.equal(1.0f, 1.1f);
            TestAssert{p}.equal(1.1, 0.9);
            TestAssert{p}.not_equal(1.7, 1.7);
            TestAssert{p}.less_than(1.4, -1.3);
            TestAssert{p}.less_than_or_equal(1.8, -1.9);
            TestAssert{p}.less_than_or_equal(-1.0, -1.1);
            TestAssert{p}.greater_than(0.9, -1.4);
            TestAssert{p}.greater_than_or_equal(-0.1, 0.3);
            TestAssert{p}.greater_than_or_equal(0.8, 0.9);
        })

        .name("string").run([] (TestParams& p) {
            TestString::Buffer buffer;

            TestAssert{p}.not_equal('a', 0x61);
            TestAssert{p}.equal("Test", "Text");
            TestAssert{p}.not_equal("Test", "Test");
            TestAssert{p}.equal(TestString{"TEST"}.ignore_case(), "Text");
            TestAssert{p}.not_equal(TestString{"TEST"}.ignore_case(), "Test");
            TestAssert{p}.equal(5u, TestString::length("Test"));
            TestAssert{p}.equal("0.0", to_string(0.1, buffer));
        })

        .name("array").run([] (TestParams& p) {
            int array1[]{1, 2, 3, 4};
            int array2[]{1, 2, 3, 4};
            int array3[]{1, 2, 4, 3};

            TestAssert{p}.not_equal(TestSpan<int>{array1}, array2);
            TestAssert{p}.equal(TestSpan<int>{array1}, array3);
        })

        .name("union").run([] (TestParams& p) {
            DummyUnion dummy{};

            dummy.value = 0;
            TestAssert{p}.is_true(dummy);
            TestAssert{p}.not_equal(dummy, 0);

            dummy.value = 4;
            TestAssert{p}.is_false(dummy);
            TestAssert{p}.not_equal(dummy, 4);
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


static TestRunner g_exception([] (TestSuite&) {
    THROW(DummyException());
});
