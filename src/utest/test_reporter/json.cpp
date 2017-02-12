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
 * @file utest/test_reporter/json.cpp
 *
 * @brief Test reporter JSON implementation
 */

#include <utest/test_reporter/json.hpp>

#include <utest/test_string.hpp>
#include <utest/test_status.hpp>

#include <utest/test_message/test.hpp>
#include <utest/test_message/test_suite.hpp>
#include <utest/test_message/test_case.hpp>
#include <utest/test_message/test_runner.hpp>
#include <utest/test_message/test_assert.hpp>

using utest::TestSize;
using utest::TestNumber;
using utest::TestString;
using utest::TestMessage;
using utest::test_reporter::JSON;
using namespace utest::test_message;

static constexpr TestString WHAT{"what"};
static constexpr TestString NAME{"name"};
static constexpr TestString VALUE{"value"};
static constexpr TestString STATUS{"status"};
static constexpr TestString PASSED{"passed"};
static constexpr TestString FAILED{"failed"};

namespace utest {
namespace test_reporter {

template<>
JSON& JSON::indent<JSON::INCREASE>() noexcept {
    m_indent += m_indent_step;
    return indent();
}

template<>
JSON& JSON::indent<JSON::DECREASE>() noexcept {
    m_indent -= m_indent_step;
    return indent();
}

template<>
void JSON::report<TestMessage::TEST_BEGIN>(
        const TestMessage&) noexcept {
    indent().write("{").endl();
    indent<INCREASE>().key("testSuites").write("[");
    m_indent += m_indent_step;
    m_next_test_suite = false;
}

template<>
void JSON::report<TestMessage::TEST_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestEnd>(message);

    m_indent -= m_indent_step;
    endl().indent().write("]");
    append().key(PASSED, msg.test_cases_passed());
    if (msg.test_cases_failed()) {
        append().key(FAILED, msg.test_cases_failed());
    }
    append().key(STATUS, msg.test_cases_failed() ? FAILED : PASSED);
    endl().indent<DECREASE>().write("}");
}

template<>
void JSON::report<TestMessage::TEST_SUITE_BEGIN>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteBegin>(message);

    if (m_next_test_suite) {
        write(",");
    }
    endl();

    indent().write("{").endl();
    indent<INCREASE>().key(NAME, msg.name());
    append().key("testCases").write("[");
    m_indent += m_indent_step;
    m_next_test_case = false;
}

template<>
void JSON::report<TestMessage::TEST_SUITE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteEnd>(message);

    m_indent -= m_indent_step;
    endl().indent().write("]");
    append().key(PASSED, msg.tests_passed());
    if (msg.tests_failed()) {
        append().key(FAILED, msg.tests_failed());
    }
    append().key(STATUS, msg.tests_failed() ? FAILED : PASSED);
    endl().indent<DECREASE>().write("}");
    m_next_test_suite = true;
}

template<>
void JSON::report<TestMessage::TEST_CASE_BEGIN>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseBegin>(message);

    if (m_next_test_case) {
        write(",");
    }
    endl();

    indent().write("{").endl();
    indent<INCREASE>().key(NAME, msg.name());
    m_next_test_assert = false;
    m_test_asserts = false;
    m_explanation = false;
}

template<>
void JSON::report<TestMessage::TEST_CASE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseEnd>(message);

    if (m_test_asserts) {
        endl().indent<DECREASE>().write("}");
        endl().indent<DECREASE>().write("]");
    }
    append().key(STATUS, TestStatus::FAIL == msg.status()
            ? FAILED : PASSED);
    endl().indent<DECREASE>().write("}");
    m_next_test_case = true;
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EXPLANATION>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestAssertExplanation>(message);

    if (!m_explanation) {
        append().key("explanation").write("\"");
        m_explanation = true;
    }

    write(msg.explanation());
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EXPLANATION_END>(
        const TestMessage&) noexcept {
    write("\"");
    m_explanation = false;
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_FAIL>(
        const TestMessage& message) noexcept {
    report("fail", get<TestAssertBase>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_TRUE>(
        const TestMessage& message) noexcept {
    report("isTrue", get<TestAssertBase>(message));
    append().key(VALUE).write("false");
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_FALSE>(
        const TestMessage& message) noexcept {
    report("isFalse", get<TestAssertBase>(message));
    append().key(VALUE).write("true");
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EQUAL>(
        const TestMessage& message) noexcept {
    report("isEqual", get<TestAssertCompare>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_NOT_EQUAL>(
        const TestMessage& message) noexcept {
    report("isNotEqual", get<TestAssertCompare>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_GREATER_THAN>(
        const TestMessage& message) noexcept {
    report("isGreaterThan", get<TestAssertCompare>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_GREATER_THAN_OR_EQUAL>(
        const TestMessage& message) noexcept {
    report("isGreaterThanOrEqual", get<TestAssertCompare>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_LESS_THAN>(
        const TestMessage& message) noexcept {
    report("isLessThan", get<TestAssertCompare>(message));
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_LESS_THAN_OR_EQUAL>(
        const TestMessage& message) noexcept {
    report("isLessThanOrEqual", get<TestAssertCompare>(message));
}

#if defined(UTEST_USE_EXCEPTIONS)

template<>
void JSON::report<TestMessage::TEST_ASSERT_NO_THROW>(
        const TestMessage& message) noexcept {
    report("throwNoException", get<TestAssertBase>(message));
    const auto& msg = get<TestAssertNoThrow>(message);

    append().key(VALUE, "it throws an exception");
    if (msg.what()) {
        append().key(WHAT, msg.what());
    }
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_ANY_THROW>(
        const TestMessage& message) noexcept {
    report("throwAnyException", get<TestAssertBase>(message));
    append().key(VALUE, "it throws nothing");
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EXPECTED_THROW>(
        const TestMessage& message) noexcept {
    report("throwExpectedException", get<TestAssertBase>(message));
    const auto& msg = get<TestAssertExpectedThrow>(message);

    if (msg.throws()) {
        append().key(VALUE, "it throws unexpected exception");
        if (msg.what()) {
            append().key(WHAT, msg.what());
        }
    }
    else {
        append().key(VALUE, "it throws nothing");
    }
}

template<>
void JSON::report<TestMessage::TEST_RUNNER_EXCEPTION>(
        const TestMessage& message) noexcept {
    report_exception("from test runner body", message);
    append().key(WHAT, get<TestRunnerException>(message).what());
}

template<>
void JSON::report<TestMessage::TEST_SUITE_EXCEPTION>(
        const TestMessage& message) noexcept {
    report_exception("from test suite body", message);
    append().key(WHAT, get<TestSuiteException>(message).what());
}

template<>
void JSON::report<TestMessage::TEST_CASE_EXCEPTION>(
        const TestMessage& message) noexcept {
    report_exception("from test case body", message);
    append().key(WHAT, get<TestCaseException>(message).what());
}

template<>
void JSON::report<TestMessage::TEST_CASE_SETUP_EXCEPTION>(
        const TestMessage& message) noexcept {
    report_exception("from test case setup body", message);
    append().key(WHAT, get<TestCaseSetupException>(message).what());
}

template<>
void JSON::report<TestMessage::TEST_CASE_TEARDOWN_EXCEPTION>(
        const TestMessage& message) noexcept {
    report_exception("from test case teardown body", message);
    append().key(WHAT, get<TestCaseTeardownException>(message).what());
}

#endif

}
}

void JSON::report(const TestMessage& message) noexcept {
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
    case TestMessage::TEST_ASSERT_FAIL:
        report<TestMessage::TEST_ASSERT_FAIL>(message);
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
    case TestMessage::TEST_ASSERT_GREATER_THAN:
        report<TestMessage::TEST_ASSERT_GREATER_THAN>(message);
        break;
    case TestMessage::TEST_ASSERT_GREATER_THAN_OR_EQUAL:
        report<TestMessage::TEST_ASSERT_GREATER_THAN_OR_EQUAL>(message);
        break;
    case TestMessage::TEST_ASSERT_LESS_THAN:
        report<TestMessage::TEST_ASSERT_LESS_THAN>(message);
        break;
    case TestMessage::TEST_ASSERT_LESS_THAN_OR_EQUAL:
        report<TestMessage::TEST_ASSERT_LESS_THAN_OR_EQUAL>(message);
        break;
#if defined(UTEST_USE_EXCEPTIONS)
    case TestMessage::TEST_ASSERT_EXPECTED_THROW:
        report<TestMessage::TEST_ASSERT_EXPECTED_THROW>(message);
        break;
    case TestMessage::TEST_ASSERT_ANY_THROW:
        report<TestMessage::TEST_ASSERT_ANY_THROW>(message);
        break;
    case TestMessage::TEST_ASSERT_NO_THROW:
        report<TestMessage::TEST_ASSERT_NO_THROW>(message);
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
#else
    case TestMessage::TEST_ASSERT_EXPECTED_THROW:
    case TestMessage::TEST_ASSERT_ANY_THROW:
    case TestMessage::TEST_ASSERT_NO_THROW:
    case TestMessage::TEST_RUNNER_EXCEPTION:
    case TestMessage::TEST_SUITE_EXCEPTION:
    case TestMessage::TEST_CASE_EXCEPTION:
    case TestMessage::TEST_CASE_SETUP_EXCEPTION:
    case TestMessage::TEST_CASE_TEARDOWN_EXCEPTION:
#endif
    case TestMessage::TEST_CASE_SETUP:
    case TestMessage::TEST_CASE_TEARDOWN:
    default:
        break;
    }
}

void JSON::report_exception(const TestString& str,
        const TestMessage& message) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    report("throwException", get<TestAssertBase>(message));
    append().key(VALUE, str);
#else
    (void)str;
    (void)message;
#endif
}

void JSON::report(const TestString& str,
        const TestAssertCompare& compare) noexcept {
    report(str, static_cast<const TestAssertBase&>(compare));

    append().key(VALUE).write("[").endl();
    indent<INCREASE>().write_value(compare.get<0>());
    append().write_value(compare.get<1>());
    endl().indent<DECREASE>().write("]");
}

JSON& JSON::write_value(const TestValue& value) noexcept {
    switch (value.type()) {
    case TestValue::NUMBER:
        write(value.get<TestNumber>());
        break;
    case TestValue::STRING:
        name(value.get<TestString>());
        break;
    case TestValue::OBJECT:
    case TestValue::POINTER:
        char buffer[TestNumber::MAX_ADDRESS_BUFFER];
        name(to_string(value.get<const void*>(), buffer));
        break;
    default:
        break;
    }

    return *this;
}

void JSON::report(const TestString& str, const TestAssertBase& base) noexcept {
    if (!m_test_asserts) {
        append().key("testAsserts").write("[");
        m_indent += m_indent_step;
        m_test_asserts = true;
    }

    if (m_next_test_assert) {
        endl().indent<DECREASE>().write("},");
    }
    endl().indent().write("{");
    endl().indent<INCREASE>().key(NAME, str);

    if (base.file()) {
        append().key("file", base.file());
    }

    if (base.line()) {
        append().key("line", base.line());
    }

    m_next_test_assert = true;
}

JSON& JSON::indent() noexcept {
    if (!m_compress) {
        for (TestSize i = 0; i < m_indent; ++i) {
            write(" ");
        }
    }
    return *this;
}

JSON& JSON::append() noexcept {
    return write(",").endl().indent();
}

JSON& JSON::endl() noexcept {
    if (!m_compress) {
        write("\n");
    }
    return *this;
}

JSON& JSON::key(const TestString& str, const TestString& value) noexcept {
    return key(str).name(value);
}

JSON& JSON::key(const TestString& str, const TestNumber& value) noexcept {
    return key(str).write(value);
}

JSON& JSON::key(const TestString& str) noexcept {
    name(str).write(":");
    if (!m_compress) {
        write(" ");
    }
    return *this;
}

JSON& JSON::name(const TestString& str) noexcept {
    return write("\"", str, "\"");
}

JSON::~JSON() noexcept { }
