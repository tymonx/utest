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
 * @file examples/string.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>

#include <cstdlib>
#include <string>

using std::string;
using namespace utest;

int main() {
    return TestStatus::PASS == Test().run().status()
        ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void test_case_execution(TestCase& test_case) {
    test_case
    .name("size is zero").run([] (TestParams& p) {
        const auto& str = p.context<string>();
        TestAssert{p}.equal(0u, str.size());
    })

    .name("empty is true").run([] (TestParams& p) {
        const auto& str = p.context<string>();
        TestAssert{p}.is_true(str.empty());
    })

    .name("text equal").run([] (TestParams& p) {
        auto& str = p.context<string>();

        str = "Text";
        TestAssert{p}.equal(str, str);
        TestAssert{p}.equal("Text", str);
        TestAssert{p}.equal(0, 1) << 0 << " != " << 1;
        TestAssert{p}.equal(str, "Test") << str << " != Test";
        TestAssert{p}.equal(TestString{"TEXT"}.ignore_case(), str);
        TestAssert{p}.not_equal(TestString{"TEXT"}.ignore_case(), str);
        TestAssert{p}.fail();

        TestAssert{p}.no_throw([] (TestParams& pp) {
            pp.context<string>().at(10);
        });

        TestAssert{p}.any_throw([] (TestParams& pp) {
            pp.context<string>().at(0);
        });

        TestAssert{p}.expected_throw<std::out_of_range>([] (TestParams& pp) {
            pp.context<string>().at(0);
        });

        TestAssert{p}.expected_throw<std::runtime_error>([] (TestParams& pp) {
            pp.context<string>().at(10);
        });
    });
}

static TestRunner g([] (TestSuite& test_suite) {
    test_suite.file(__FILE__)
    .fatal(false)

    .name("string - dynamic allocation").run([] (TestCase& test_case) {
        test_case.setup([] (TestParams& p) {
            p.context(new string());
        });

        test_case.teardown([] (TestParams& p) {
            delete p.context<string*>();
        });

        test_case_execution(test_case);
    })

    .name("string - stack allocation").run([] (TestCase& test_case) {
        string test_string;
        test_case.context(test_string);

        test_case.setup([] (TestParams& p) {
            p.context<string>().clear();
        });

        test_case.teardown({});

        test_case_execution(test_case);
    })

    .name("string - unitialized storage").run([] (TestCase& test_case) {
        TestObject<string> test_object;
        test_case.context(test_object);

        test_case.setup([] (TestParams& p) {
            new (p.context()) string();
        });

        test_case.teardown([] (TestParams& p) {
            p.context<string>().~basic_string();
        });

        test_case_execution(test_case);
    });
});
