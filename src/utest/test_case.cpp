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
 * @file utest/test_case.cpp
 *
 * @brief Test case implementation
 */

#include <utest/test_suite.hpp>
#include <utest/test_case.hpp>
#include <utest/test_params.hpp>
#include <utest/test_reporter.hpp>
#include <utest/test_exception.hpp>

#include <utest/test_message/test_case.hpp>

using utest::TestCase;

void TestCase::test_execute(TestParams& test_params, TestFunction test_run) {
    if (test_run) {
        if (test_params.set_jump()) {
            test_run(test_params);
            if (TestStatus::PASS == m_status) {
                m_status = test_params.status();
            }
        }
        else {
            m_status = TestStatus::FAIL;
        }
    }
}

TestCase::TestCase(const TestSuite& test_suite) noexcept :
    m_reporters{test_suite.m_reporters},
    m_file{test_suite.m_file},
    m_non_fatal{test_suite.m_non_fatal}
{ }

void TestCase::report(const TestMessage& test_message) noexcept {
    for (auto reporter : m_reporters) {
        if (reporter) {
            reporter->report(test_message);
        }
    }
}

void TestCase::run_setup(TestParams& test_params) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    try {
        test_execute(test_params, m_setup);
    }
    catch (const std::exception& e) {
        m_status = TestStatus::FAIL;
        report(test_message::TestCaseSetupException{*this, e.what()});
    }
    catch (...) {
        m_status = TestStatus::FAIL;
        report(test_message::TestCaseSetupException{*this});
    }
#else
    test_execute(test_params, m_setup);
#endif
    report(test_message::TestCaseSetup{*this});
}

void TestCase::run_teardown(TestParams& test_params) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    try {
        test_execute(test_params, m_teardown);
    }
    catch (const std::exception& e) {
        m_status = TestStatus::FAIL;
        report(test_message::TestCaseTeardownException{*this, e.what()});
    }
    catch (...) {
        m_status = TestStatus::FAIL;
        report(test_message::TestCaseTeardownException{*this});
    }
#else
    test_execute(test_params, m_teardown);
#endif
    report(test_message::TestCaseTeardown{*this});
}

TestCase& TestCase::run(TestFunction test_run) noexcept {
    m_status = TestStatus::PASS;
    TestParams test_params{*this};

    report(test_message::TestCaseBegin{*this});
    run_setup(test_params);

    if (TestStatus::PASS == m_status) {
#if defined(UTEST_USE_EXCEPTIONS)
        try {
            test_execute(test_params, test_run);
        }
        catch (const std::exception& e) {
            m_status = TestStatus::FAIL;
            report(test_message::TestCaseException{*this, e.what()});
        }
        catch (...) {
            m_status = TestStatus::FAIL;
            report(test_message::TestCaseException{*this});
        }
#else
        test_execute(test_params, test_run);
#endif
    }

    run_teardown(test_params);

    if (TestStatus::PASS == m_status) {
        ++m_passed;
    }
    else {
        ++m_failed;
    }

    report(test_message::TestCaseEnd{*this});

    return *this;
}
