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
 * @file utest/test_reporter/google_test.cpp
 *
 * @brief Test reporter Google Test embedded implementation
 */

#include <utest/test_reporter/google_test.hpp>

#include <utest/test_string.hpp>
#include <utest/test_status.hpp>

#include <utest/test_message/test.hpp>
#include <utest/test_message/test_suite.hpp>
#include <utest/test_message/test_case.hpp>
#include <utest/test_message/test_assert.hpp>
#include <utest/test_message/test_runner.hpp>

using utest::TestString;
using utest::TestMessage;
using utest::test_reporter::GoogleTest;
using namespace utest::test_message;

static constexpr TestString ENTRY       {"[==========] "};
static constexpr TestString SECTION     {"[----------] "};
static constexpr TestString RUN         {"[ RUN      ] "};
static constexpr TestString OK          {"[       OK ] "};
static constexpr TestString FAILED      {"[  FAILED  ] "};
static constexpr TestString PASSED      {"[  PASSED  ] "};
static constexpr TestString TEST        {" test"};
static constexpr TestString TEST_SUITE  {" test suite"};
static constexpr TestString FROM        {" from "};
static constexpr TestString ENDL        {"\n"};

void GoogleTest::close_explanation() noexcept {
    if (m_explanation) {
        m_explanation = false;
        write(ENDL);
    }
}

void GoogleTest::report(const TestMessage& message) noexcept {
    switch (message.type()) {
    case TestMessage::TEST_BEGIN:
        test_begin(message);
        break;
    case TestMessage::TEST_SUITE_BEGIN:
        test_suite_begin(message);
        break;
    case TestMessage::TEST_CASE_BEGIN:
        test_case_begin(message);
        break;
    case TestMessage::TEST_CASE_SETUP:
        break;
    case TestMessage::TEST_ASSERT_EXPLANATION:
        m_explanation = true;
        write(get<TestAssertExplanation>(message).explanation());
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
        break;
    case TestMessage::TEST_CASE_END:
        test_case_end(message);
        break;
    case TestMessage::TEST_SUITE_END:
        test_suite_end(message);
        break;
    case TestMessage::TEST_END:
        test_end(message);
        break;
    case TestMessage::TEST_RUNNER_EXCEPTION:
        write("Test runner exception: ");
        write(get<TestRunnerException>(message).what());
        write("\n");
        break;
    case TestMessage::TEST_SUITE_EXCEPTION:
        write("Test suite exception: ");
        write(get<TestSuiteException>(message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_EXCEPTION:
        write("Test case exception: ");
        write(get<TestCaseException>(message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_SETUP_EXCEPTION:
        write("Test case setup exception: ");
        write(get<TestCaseSetupException>(message).what());
        write("\n");
        break;
    case TestMessage::TEST_CASE_TEARDOWN_EXCEPTION:
        write("Test case teardown exception: ");
        write(get<TestCaseTeardownException>(message).what());
        write("\n");
        break;
    default:
        break;
    }
}

void GoogleTest::display_entry() noexcept {
    color(TestColor::GREEN);
    write(ENTRY);
    color(TestColor::DEFAULT);
}

void GoogleTest::display_section() noexcept {
    color(TestColor::GREEN);
    write(SECTION);
    color(TestColor::DEFAULT);
}

void GoogleTest::display_run() noexcept {
    color(TestColor::GREEN);
    write(RUN);
    color(TestColor::DEFAULT);
}

void GoogleTest::display_ok() noexcept {
    color(TestColor::GREEN);
    write(OK);
    color(TestColor::DEFAULT);
}

void GoogleTest::display_passed() noexcept {
    color(TestColor::GREEN);
    write(PASSED);
    color(TestColor::DEFAULT);
}

void GoogleTest::display_failed() noexcept {
    color(TestColor::RED);
    write(FAILED);
    color(TestColor::DEFAULT);
}

void GoogleTest::test_begin(const TestMessage& message) noexcept {
    const auto& msg = get<TestBegin>(message);

    display_entry();
    write("Running registered ", msg.tests_registered(), TEST, ENDL);

    display_section();
    write("Global test environment set-up\n");
}

void GoogleTest::test_end(const TestMessage& message) noexcept {
    const auto& msg = get<TestEnd>(message);

    display_section();
    write("Global test environment tear-down\n");

    display_entry();
    write(msg.test_cases(), TEST, FROM, msg.test_suites(),
            TEST_SUITE, " ran\n");

    display_passed();
    write(msg.test_cases_passed(), TEST, ENDL);

    if (msg.test_cases_failed()) {
        display_failed();
        write(msg.test_cases_failed(), TEST, ENDL);
    }
}

void GoogleTest::test_suite_begin(const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteBegin>(message);

    display_section();
    write("Running tests from ", msg.name(), ENDL);
}

void GoogleTest::test_suite_end(const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteEnd>(message);

    display_section();
    write(msg.tests_passed() + msg.tests_failed(), TEST, FROM,
            msg.name(), ENDL);

    display_passed();
    write(msg.tests_passed(), TEST, ENDL);

    if (msg.tests_failed()) {
        display_failed();
        write(msg.tests_failed(), TEST, ENDL);
    }

    write(ENDL);
}

void GoogleTest::test_case_begin(const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseBegin>(message);

    display_run();
    write(msg.name(), ENDL);
}

void GoogleTest::test_case_end(const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseEnd>(message);

    if (TestStatus::PASS == msg.status()) {
        display_ok();
    }
    else {
        display_failed();
    }
    write(msg.name(), ENDL);
}

GoogleTest::~GoogleTest() noexcept { }
