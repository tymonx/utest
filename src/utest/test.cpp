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
#include <utest/test_thread.hpp>

#include <utest/test_message/test.hpp>

#include "test_register.hpp"

#include <array>

using utest::Test;
using utest::TestStatus;

static std::array<utest::TestReporter*, 1> g_default{{
    &utest::TestReporter::get_default()
}};

Test::Test() noexcept :
    m_thread(TestThread::get_default()),
    m_reporters{g_default}
{ }

Test::Test(const TestReporters& test_reporters) noexcept :
    m_thread(TestThread::get_default()),
    m_reporters{test_reporters}
{ }

Test& Test::color(bool enable) noexcept {
    for (auto reporter : m_reporters) {
        if (reporter) {
            reporter->color(enable);
        }
    }
    return *this;
}

void Test::report(const TestMessage& test_message) noexcept {
    for (auto reporter : m_reporters) {
        if (reporter) {
            reporter->report(test_message);
        }
    }
}

Test& Test::run() noexcept {
    TestSize test_suites_passed{0};
    TestSize test_suites_failed{0};
    TestSize test_cases_passed{0};
    TestSize test_cases_failed{0};

    auto& test_runs = TestRegister::get_instance();
    m_status = TestStatus::PASS;

    report(test_message::TestBegin{
        test_runs.size()
    });

    for (auto& test_run : test_runs) {
        TestSuite test_suite{*this};
        test_run(test_suite);

        test_suites_passed += test_suite.passed();
        test_suites_failed += test_suite.failed();

        test_cases_passed += test_suite.test_cases_passed();
        test_cases_failed += test_suite.test_cases_failed();
    }

    if (test_suites_failed || test_cases_failed) {
        m_status = TestStatus::FAIL;
    }

    report(test_message::TestEnd{
        test_suites_passed,
        test_suites_failed,
        test_cases_passed,
        test_cases_failed
    });

    return *this;
}
