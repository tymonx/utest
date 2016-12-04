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
 * @file utest/test.cpp
 *
 * @brief Test implementation
 */

#include <utest/test.hpp>
#include <utest/test_suite.hpp>
#include <utest/test_runner.hpp>
#include <utest/test_reporter.hpp>
#include <utest/test_size.hpp>

#include <utest/test_message/test.hpp>

#include "test_register.hpp"

using utest::Test;
using utest::TestStatus;

static utest::TestReporter* g_default[1] {&utest::TestReporter::get_default()};

Test::Test() noexcept :
    m_reporters{g_default}
{ }

void Test::report(const TestMessage& test_message) noexcept {
    for (auto reporter : m_reporters) {
        if (reporter) {
            reporter->report(test_message);
        }
    }
}

Test& Test::run() noexcept {
    TestSize tests_executed{0};
    TestSize tests_passed{0};
    TestSize tests_failed{0};

    auto& test_runs = TestRegister::get_instance();
    auto tests_registered = test_runs.size();

    report(test_message::TestBegin{
        tests_registered
    });

    m_status = TestStatus::PASS;

    for (auto& test_run : test_runs) {
        TestSuite test_suite{*this};
        auto test_passed = test_run(test_suite);

        if (test_passed) {
            ++tests_passed;
        }
        else {
            ++tests_failed;
        }

        ++tests_executed;
    }

    if (tests_failed) {
        m_status = TestStatus::FAIL;
    }

    report(test_message::TestEnd{
        tests_registered,
        tests_executed,
        tests_passed,
        tests_failed
    });

    return *this;
}
