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
 * @file utest/test_reporter/google_test_embedded.cpp
 *
 * @brief Test reporter Google Test embedded implementation
 */

#include <utest/test_reporter/google_test_embedded.hpp>
#include <utest/test_message/test.hpp>
#include <utest/test_message/test_suite.hpp>
#include <utest/test_message/test_case.hpp>
#include <utest/test_message/test_assert.hpp>
#include <utest/test_message/test_runner.hpp>

using utest::TestMessage;
using utest::test_reporter::GoogleTestEmbedded;
using namespace utest::test_message;

void GoogleTestEmbedded::close_explanation() noexcept {
    if (m_explanation) {
        m_explanation = false;
        write("\n");
    }
}

void GoogleTestEmbedded::report(const TestMessage& test_message) noexcept {
    switch (test_message.type()) {
    case TestMessage::TEST_BEGIN:
        write("Test begin\n");
        break;
    case TestMessage::TEST_SUITE_BEGIN:
        write("Test suite begin: ");
        write(get<TestSuiteBegin>(test_message).name());
        write("\n");
        break;
    case TestMessage::TEST_CASE_BEGIN:
        write("Test case begin: ");
        write(get<TestCaseBegin>(test_message).name());
        write("\n");
        break;
    case TestMessage::TEST_CASE_SETUP:
        write("Test case setup begin\n");
        break;
    case TestMessage::TEST_ASSERT_EXPLANATION:
        m_explanation = true;
        write(get<TestAssertExplanation>(test_message).explanation());
        break;
    case TestMessage::TEST_ASSERT_TRUE:
        write("Test assert true\n");
        break;
    case TestMessage::TEST_ASSERT_FALSE:
        write("Test assert false\n");
        break;
    case TestMessage::TEST_ASSERT_EQUAL:
        write("Test assert equal number/string/object\n");
        break;
    case TestMessage::TEST_ASSERT_NOT_EQUAL:
        write("Test assert not equal number/string/object\n");
        break;
    case TestMessage::TEST_ASSERT_END:
        close_explanation();
        break;
    case TestMessage::TEST_CASE_TEARDOWN:
        write("Test case teardown begin\n");
        break;
    case TestMessage::TEST_CASE_END:
        write("Test case end\n");
        break;
    case TestMessage::TEST_SUITE_END:
        write("Test suite end\n");
        break;
    case TestMessage::TEST_END:
        write("Test end\n");
        break;
    case TestMessage::TEST_RUNNER_EXCEPTION:
        write("Test runner exception: ");
        write(get<TestRunnerException>(test_message).what());
        write("\n");
        break;
    case TestMessage::TEST_SUITE_EXCEPTION:
        write("Test suite exception: ");
        write(get<TestSuiteException>(test_message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_EXCEPTION:
        write("Test case exception: ");
        write(get<TestCaseException>(test_message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_SETUP_EXCEPTION:
        write("Test case setup exception: ");
        write(get<TestCaseSetupException>(test_message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_TEARDOWN_EXCEPTION:
        write("Test case teardown exception: ");
        write(get<TestCaseTeardownException>(test_message).what());
        write("\n");
        break;
    default:
        break;
    }
}

GoogleTestEmbedded::~GoogleTestEmbedded() noexcept { }
