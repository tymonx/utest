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
 * @file utest/test_assert.cpp
 *
 * @brief Test assert implementation
 */

#include <utest/test_assert.hpp>
#include <utest/test_params.hpp>
#include <utest/test_reporter.hpp>
#include <utest/test.hpp>

#include <utest/test_message/test_assert.hpp>

using utest::TestSize;
using utest::TestNumber;
using utest::TestAssert;
using utest::TestString;
using namespace utest::test_message;

static constexpr TestString STRING_NULL{"null"};
static constexpr TestString STRING_TRUE{"true"};
static constexpr TestString STRING_FALSE{"false"};

TestAssert::TestAssert(TestParams& params) noexcept :
    m_params(params),
    m_file{params.m_file},
    m_non_fatal{params.m_non_fatal}
{ }

TestAssert::~TestAssert() noexcept {
    if (TestStatus::FAIL == m_status) {
        m_params.status(TestStatus::FAIL);
        if (m_explanation) {
            report(TestAssertExplanationEnd{*this});
        }
        if (!m_non_fatal) {
            m_params.jump();
        }
    }
}

void TestAssert::report(const TestMessage& test_message) noexcept {
    m_params.m_test.report(test_message);
}

TestAssert& TestAssert::operator<<(std::nullptr_t) noexcept {
    if (TestStatus::FAIL == m_status) {
        m_explanation = true;
        report(TestAssertExplanation{*this, STRING_NULL});
    }
    return *this;
}

TestAssert& TestAssert::operator<<(const void* ptr) noexcept {
    if (TestStatus::FAIL == m_status) {
        m_explanation = true;
        if (nullptr != ptr) {
            char buffer[TestNumber::MAX_ADDRESS_BUFFER];
            report(TestAssertExplanation{
                    *this, to_string(ptr, buffer)});
        }
        else {
            report(TestAssertExplanation{*this, STRING_NULL});
        }
    }
    return *this;
}

TestAssert& TestAssert::operator<<(bool value) noexcept {
    if (TestStatus::FAIL == m_status) {
        m_explanation = true;
        if (true == value) {
            report(TestAssertExplanation{*this, STRING_TRUE});
        }
        else {
            report(TestAssertExplanation{*this, STRING_FALSE});
        }
    }
    return *this;
}

TestAssert& TestAssert::operator<<(const TestString& str) noexcept {
    if (TestStatus::FAIL == m_status) {
        m_explanation = true;
        report(TestAssertExplanation{*this, str});
    }
    return *this;
}

TestAssert& TestAssert::operator<<(const TestNumber& number) noexcept {
    if (TestStatus::FAIL == m_status) {
        m_explanation = true;
        char buffer[TestNumber::MAX_STRING_BUFFER];
        auto value = to_string(number, buffer);

        report(TestAssertExplanation{*this, value});
    }
    return *this;
}

TestAssert& TestAssert::fail() noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertFail{*this});
    return *this;
}

TestAssert& TestAssert::is_true(bool value) noexcept {
    if (true != value) {
        m_status = TestStatus::FAIL;
        report(TestAssertTrue{*this});
    }
    return *this;
}

TestAssert& TestAssert::is_false(bool value) noexcept {
    if (false != value) {
        m_status = TestStatus::FAIL;
        report(TestAssertFalse{*this});
    }
    return *this;
}

TestAssert& TestAssert::equal(const TestNumber& lhs,
        const TestNumber& rhs) noexcept {
    if (!(lhs == rhs)) {
        equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

TestAssert& TestAssert::not_equal(const TestNumber& lhs,
        const TestNumber& rhs) noexcept {
    if (!(lhs != rhs)) {
        not_equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

void TestAssert::equal(const TestValue& lhs, const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertEqual{*this, lhs, rhs});
}

void TestAssert::not_equal(const TestValue& lhs,
        const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertNotEqual{*this, lhs, rhs});
}

void TestAssert::greater_than(const TestValue& lhs,
        const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertGreaterThan{*this, lhs, rhs});
}

void TestAssert::greater_than_or_equal(const TestValue& lhs,
        const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertGreaterThanOrEqual{*this, lhs, rhs});
}

void TestAssert::less_than(const TestValue& lhs,
        const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertLessThan{*this, lhs, rhs});
}

void TestAssert::less_than_or_equal(const TestValue& lhs,
        const TestValue& rhs) noexcept {
    m_status = TestStatus::FAIL;
    report(TestAssertLessThanOrEqual{*this, lhs, rhs});
}

TestAssert& TestAssert::any_throw(TestRun test_run) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    if (test_run) {
        try {
            test_run(m_params);
            m_status = TestStatus::FAIL;
            report(TestAssertAnyThrow{*this});
        } catch (...) { }
    }
#else
    (void)test_run;
#endif
    return *this;
}

TestAssert& TestAssert::no_throw(TestRun test_run) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    if (test_run) {
        try {
            test_run(m_params);
        }
        catch (const std::exception& e) {
            m_status = TestStatus::FAIL;
            report(TestAssertNoThrow{*this, e.what()});
        }
        catch (...) {
            m_status = TestStatus::FAIL;
            report(TestAssertNoThrow{*this, {}});
        }
    }
#else
    (void)test_run;
#endif
    return *this;
}

void TestAssert::expected_throw(bool throws, const TestString& str) noexcept {
#if defined(UTEST_USE_EXCEPTIONS)
    m_status = TestStatus::FAIL;
    report(TestAssertExpectedThrow{*this, throws, str});
#else
    (void)throws;
    (void)str;
#endif
}
