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

static constexpr TestString NAME{"name"};
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
    indent<INCREASE>().key("test suite").write("[");
    m_indent += m_indent_step;
    m_next_test_suite = false;
}

template<>
void JSON::report<TestMessage::TEST_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestEnd>(message);

    m_indent -= m_indent_step;
    endl();
    indent().write("],").endl();
    indent().key(PASSED).write(msg.test_cases_passed(), ",").endl();
    if (msg.test_cases_failed()) {
        indent().key(FAILED).write(msg.test_cases_failed(), ",").endl();
    }
    indent().key(STATUS);

    if (msg.test_cases_failed()) {
        name(FAILED);
    }
    else {
        name(PASSED);
    }
    endl();
    indent<DECREASE>().write("}");
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
    indent<INCREASE>().key(NAME).name(msg.name()).write(",").endl();
    indent().key("test case").write("[");
    m_indent += m_indent_step;
    m_next_test_case = false;
}

template<>
void JSON::report<TestMessage::TEST_SUITE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestSuiteEnd>(message);

    m_indent -= m_indent_step;
    endl();
    indent().write("],").endl();
    indent().key(PASSED).write(msg.tests_passed(), ",").endl();
    if (msg.tests_failed()) {
        indent().key(FAILED).write(msg.tests_failed(), ",").endl();
    }
    indent().key(STATUS);

    if (msg.tests_failed()) {
        name(FAILED);
    }
    else {
        name(PASSED);
    }
    endl();
    indent<DECREASE>().write("}");
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
    indent<INCREASE>().key(NAME).name(msg.name()).write(",");
    m_next_test_assert = false;
    m_test_asserts = false;
}

template<>
void JSON::report<TestMessage::TEST_CASE_END>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestCaseEnd>(message);

    endl();
    if (m_test_asserts) {
        indent<DECREASE>().write("],").endl();
    }
    indent().key(STATUS);

    if (TestStatus::FAIL == msg.status()) {
        name(FAILED);
    }
    else {
        name(PASSED);
    }
    endl();
    indent<DECREASE>().write("}");
    m_next_test_case = true;
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EXPLANATION>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EXPLANATION_END>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_FAIL>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_TRUE>(
        const TestMessage& message) noexcept {
    const auto& msg = get<TestAssertTrue>(message);

    if (!m_test_asserts) {
        endl();
        indent().key("assert").write("[");
        m_indent += m_indent_step;
        m_test_asserts = true;
    }

    if (m_next_test_assert) {
        write(",");
    }
    endl();

    indent().write("{").endl();
    indent<INCREASE>().key(NAME).name("is true").write(",").endl();

    if (msg.file()) {
        indent().key("file").name(msg.file()).write(",").endl();
    }

    if (msg.line()) {
        indent().key("line").write(msg.line()).write(",").endl();
    }

    indent().key("value").write("false").endl();
    indent<DECREASE>().write("}");

    m_next_test_assert = true;
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_FALSE>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_EQUAL>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_NOT_EQUAL>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_GREATER_THAN>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_GREATER_THAN_OR_EQUAL>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_LESS_THAN>(
        const TestMessage&) noexcept {
}

template<>
void JSON::report<TestMessage::TEST_ASSERT_LESS_THAN_OR_EQUAL>(
        const TestMessage&) noexcept {
}

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
    case TestMessage::TEST_ASSERT_EXPECTED_THROW:
    case TestMessage::TEST_ASSERT_ANY_THROW:
    case TestMessage::TEST_ASSERT_NO_THROW:
    case TestMessage::TEST_RUNNER_EXCEPTION:
    case TestMessage::TEST_SUITE_EXCEPTION:
    case TestMessage::TEST_CASE_EXCEPTION:
    case TestMessage::TEST_CASE_SETUP_EXCEPTION:
    case TestMessage::TEST_CASE_TEARDOWN_EXCEPTION:
    case TestMessage::TEST_CASE_SETUP:
    case TestMessage::TEST_CASE_TEARDOWN:
    default:
        break;
    }
}

JSON& JSON::indent() noexcept {
    if (!m_compress) {
        for (TestSize i = 0; i < m_indent; ++i) {
            write(" ");
        }
    }
    return *this;
}

JSON& JSON::endl() noexcept {
    if (!m_compress) {
        write("\n");
    }
    return *this;
}

JSON& JSON::name(const TestString& str) noexcept {
    return write("\"", str, "\"");
}

JSON& JSON::key(const TestString& str) noexcept {
    name(str).write(":");
    if (!m_compress) {
        write(" ");
    }
    return *this;
}

JSON::~JSON() noexcept { }
