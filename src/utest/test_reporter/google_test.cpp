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

using utest::TestSize;
using utest::TestNumber;
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

static constexpr TestString EXCEPTION_BEGIN
    {"C++ exception with description \""};

static constexpr TestString EXCEPTION_END
    {"\" thrown in the "};

namespace utest {
namespace test_reporter {

template<>
void GoogleTest::write<true>(const TestString& str) noexcept {
    color(TestColor::GREEN);
    write(str);
    color(TestColor::DEFAULT);
}

template<>
void GoogleTest::write<false>(const TestString& str) noexcept {
    color(TestColor::RED);
    write(str);
    color(TestColor::DEFAULT);
}

template<>
void GoogleTest::report<TestMessage::TEST_BEGIN>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestBegin>(message);

    write<true>(ENTRY);
    write("Running registered ");
    write(msg.tests_registered(), TEST);
    write(ENDL);

    write<true>(SECTION);
    write("Global test environment set-up\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestEnd>(message);

    write<true>(SECTION);
    write("Global test environment tear-down\n");

    write<true>(ENTRY);
    write(msg.test_cases(), TEST);
    write(FROM);
    write(msg.test_suites(), TEST_SUITE);
    write(" ran\n");

    write<true>(PASSED);
    write(msg.test_cases_passed(), TEST);
    write(ENDL);

    if (msg.test_cases_failed()) {
        write<false>(FAILED);
        write(msg.test_cases_failed(), TEST);
        write(ENDL);
    }
}

template<>
void GoogleTest::report<TestMessage::TEST_SUITE_BEGIN>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteBegin>(message);

    write<true>(SECTION);
    write("Running tests from ", msg.name(), ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_SUITE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteEnd>(message);

    write<true>(SECTION);
    write(msg.tests_passed() + msg.tests_failed(), TEST);
    write(FROM, msg.name(), ENDL);

    write<true>(PASSED);
    write(msg.tests_passed(), TEST);
    write(ENDL);

    if (msg.tests_failed()) {
        write<false>(FAILED);
        write(msg.tests_failed(), TEST);
        write(ENDL);
    }

    write(ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_CASE_BEGIN>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseBegin>(message);

    write<true>(RUN);
    write(msg.name(), ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_CASE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseEnd>(message);

    if (TestStatus::PASS == msg.status()) {
        write<true>(OK);
    }
    else {
        write<false>(FAILED);
    }
    write(msg.name(), ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_EXPLANATION>(
        const TestMessage& message) noexcept {
    write(get<TestAssertExplanation>(message).explanation());
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_EXPLANATION_END>(
        const TestMessage&) noexcept {
    write(ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_TRUE>(
        const TestMessage& message) noexcept {
    failure(get<TestAssertTrue>(message));
    write("  Actual: false\n");
    write("Expected: true\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_FALSE>(
        const TestMessage& message) noexcept {
    failure(get<TestAssertFalse>(message));
    write("  Actual: true\n");
    write("Expected: false\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_EQUAL>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestAssertEqual>(message);

    failure(msg);
    write("      Expected: ", msg.get<0>(), ENDL);
    write("To be equal to: ", msg.get<1>(), ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_ASSERT_NOT_EQUAL>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestAssertNotEqual>(message);

    failure(msg);
    write("    Expected: ", msg.get<0>(), ENDL);
    write("Not equal to: ", msg.get<1>(), ENDL);
}

template<>
void GoogleTest::report<TestMessage::TEST_RUNNER_EXCEPTION>(
        const TestMessage& message) noexcept {
    write_exception(get<TestRunnerException>(message).what());
    write("test runner body\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_SUITE_EXCEPTION>(
        const TestMessage& message) noexcept {
    write_exception(get<TestSuiteException>(message).what());
    write("test suite body\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_CASE_EXCEPTION>(
        const TestMessage& message) noexcept {
    write_exception(get<TestCaseException>(message).what());
    write("test case body\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_CASE_SETUP_EXCEPTION>(
        const TestMessage& message) noexcept {
    write_exception(get<TestCaseSetupException>(message).what());
    write("test case setup body\n");
}

template<>
void GoogleTest::report<TestMessage::TEST_CASE_TEARDOWN_EXCEPTION>(
        const TestMessage& message) noexcept {
    write_exception(get<TestCaseTeardownException>(message).what());
    write("test case teardown body\n");
}

}
}

void GoogleTest::report(const TestMessage& message) noexcept {
    switch (message.type()) {
    case TestMessage::TEST_BEGIN:
        report<TestMessage::TEST_BEGIN>(message);
        break;
    case TestMessage::TEST_END:
        report<TestMessage::TEST_END>(message);
        break;
    case TestMessage::TEST_SUITE_BEGIN:
        report<TestMessage::TEST_SUITE_BEGIN>(message);
        break;
    case TestMessage::TEST_SUITE_END:
        report<TestMessage::TEST_SUITE_END>(message);
        break;
    case TestMessage::TEST_CASE_BEGIN:
        report<TestMessage::TEST_CASE_BEGIN>(message);
        break;
    case TestMessage::TEST_CASE_END:
        report<TestMessage::TEST_CASE_END>(message);
        break;
    case TestMessage::TEST_ASSERT_EXPLANATION:
        report<TestMessage::TEST_ASSERT_EXPLANATION>(message);
        break;
    case TestMessage::TEST_ASSERT_EXPLANATION_END:
        report<TestMessage::TEST_ASSERT_EXPLANATION_END>(message);
        break;
    case TestMessage::TEST_ASSERT_TRUE:
        report<TestMessage::TEST_ASSERT_TRUE>(message);
        break;
    case TestMessage::TEST_ASSERT_FALSE:
        report<TestMessage::TEST_ASSERT_FALSE>(message);
        break;
    case TestMessage::TEST_ASSERT_EQUAL:
        report<TestMessage::TEST_ASSERT_EQUAL>(message);
        break;
    case TestMessage::TEST_ASSERT_NOT_EQUAL:
        report<TestMessage::TEST_ASSERT_NOT_EQUAL>(message);
        break;
    case TestMessage::TEST_RUNNER_EXCEPTION:
        report<TestMessage::TEST_RUNNER_EXCEPTION>(message);
        break;
    case TestMessage::TEST_SUITE_EXCEPTION:
        report<TestMessage::TEST_SUITE_EXCEPTION>(message);
        break;
    case TestMessage::TEST_CASE_EXCEPTION:
        report<TestMessage::TEST_CASE_EXCEPTION>(message);
        break;
    case TestMessage::TEST_CASE_SETUP_EXCEPTION:
        report<TestMessage::TEST_CASE_SETUP_EXCEPTION>(message);
        break;
    case TestMessage::TEST_CASE_TEARDOWN_EXCEPTION:
        report<TestMessage::TEST_CASE_TEARDOWN_EXCEPTION>(message);
        break;
    case TestMessage::TEST_CASE_SETUP:
    case TestMessage::TEST_CASE_TEARDOWN:
    default:
        break;
    }
}

void GoogleTest::write_exception(const TestString& message) noexcept {
    write(EXCEPTION_BEGIN, message, EXCEPTION_END);
}

void GoogleTest::write(const TestNumber& number,
        const TestString& str) noexcept {
    write(number);
    write(str);
    if (number != TestNumber{1}) { write("s"); }
}

void GoogleTest::failure(const TestString& file,
        const TestNumber& line) noexcept {
    if (!file.empty()) {
        write(file);
    }
    else {
        write("unknown file");
    }

    write(":");

    if (line > TestNumber{0}) {
        write(line);
        write(":");
    }

    write(" Failure\n");
}

void GoogleTest::write(const TestString& str,
        const TestAssertValue& value) noexcept {
    write(str);

    switch (value.type()) {
    case TestAssertValue::NUMBER:
        write(value.get<TestNumber>());
        break;
    case TestAssertValue::STRING:
        write("\"", value.get<TestString>(), "\"");
        break;
    case TestAssertValue::OBJECT:
        char buffer[TestNumber::MAX_POINTER_SIZE];
        write(to_string(value.get<const void*>(), buffer));
        break;
    default:
        break;
    }
}

GoogleTest::~GoogleTest() noexcept { }
