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
 * @file utest/test_assert.hpp
 *
 * @brief Test assert interface
 */

#ifndef UTEST_TEST_ASSERT_HPP
#define UTEST_TEST_ASSERT_HPP

#include <utest/test_value.hpp>
#include <utest/test_status.hpp>
#include <utest/test_exception.hpp>

#include <cstddef>

namespace utest {

class TestMessage;
class TestParams;

class TestAssert {
public:
    using TestRun = void(*)(TestParams& params);

    TestAssert(TestParams& params) noexcept;

    ~TestAssert() noexcept;

    TestAssert& line(TestSize test_line) noexcept;

    TestSize line() const noexcept;

    const TestString& file() const noexcept;

    TestStatus status() const noexcept;

    TestAssert& fatal(bool test_fatal = true) noexcept;

    TestAssert& fail() noexcept;

    TestAssert& is_true(bool value) noexcept;

    TestAssert& is_false(bool value) noexcept;

    template<typename T1, typename T2>
    TestAssert& equal(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2>
    TestAssert& not_equal(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2>
    TestAssert& greater_than(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2>
    TestAssert& greater_than_or_equal(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2>
    TestAssert& less_than(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2>
    TestAssert& less_than_or_equal(const T1& lhs, const T2& rhs) noexcept;

    TestAssert& any_throw(TestRun test_run) noexcept;

    TestAssert& no_throw(TestRun test_run) noexcept;

    template<typename T>
    TestAssert& expected_throw(TestRun test_run) noexcept;

    TestAssert& operator<<(std::nullptr_t) noexcept;

    TestAssert& operator<<(const void* ptr) noexcept;

    template<TestSize N>
    TestAssert& operator<<(const char (&arr)[N]) noexcept;

    TestAssert& operator<<(bool value) noexcept;

    TestAssert& operator<<(const TestString& number) noexcept;

    TestAssert& operator<<(int value) noexcept;

    TestAssert& operator<<(unsigned int value) noexcept;

    TestAssert& operator<<(float number) noexcept;

    TestAssert& operator<<(double number) noexcept;

    TestAssert& operator<<(const TestNumber& number) noexcept;
private:
    void report(const TestMessage& test_message) noexcept;

    void equal(const TestValue& lhs, const TestValue& rhs) noexcept;

    void not_equal(const TestValue& lhs, const TestValue& rhs) noexcept;

    void greater_than(const TestValue& lhs, const TestValue& rhs) noexcept;

    void greater_than_or_equal(const TestValue& lhs,
            const TestValue& rhs) noexcept;

    void less_than(const TestValue& lhs, const TestValue& rhs) noexcept;

    void less_than_or_equal(const TestValue& lhs,
            const TestValue& rhs) noexcept;

    void expected_throw(bool throws, const TestString& str = {}) noexcept;

    TestParams& m_params;
    TestSize m_line{0};
    TestStatus m_status{TestStatus::PASS};
    bool m_explanation{false};
    bool m_non_fatal{false};
};

inline auto
TestAssert::line(TestSize test_line) noexcept -> TestAssert& {
    m_line = test_line;
    return *this;
}

inline auto
TestAssert::line() const noexcept -> TestSize {
    return m_line;
}

inline auto
TestAssert::status() const noexcept -> TestStatus {
    return m_status;
}

inline auto
TestAssert::fatal(bool test_fatal) noexcept -> TestAssert& {
    m_non_fatal = !test_fatal;
    return *this;
}

template<TestSize N> inline auto
TestAssert::operator<<(const char (&arr)[N]) noexcept -> TestAssert& {
    return operator<<(TestString{arr});
}

inline auto
TestAssert::operator<<(int value) noexcept -> TestAssert& {
    return operator<<(TestNumber{value});
}

inline auto
TestAssert::operator<<(unsigned int value) noexcept -> TestAssert& {
    return operator<<(TestNumber{value});
}

inline auto
TestAssert::operator<<(float value) noexcept -> TestAssert& {
    return operator<<(TestNumber{value});
}

inline auto
TestAssert::operator<<(double value) noexcept -> TestAssert& {
    return operator<<(TestNumber{value});
}

template<typename T1, typename T2> auto
TestAssert::equal(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs == rhs)) {
        equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::not_equal(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs != rhs)) {
        not_equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::greater_than(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs > rhs)) {
        greater_than(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::greater_than_or_equal(const T1& lhs,
        const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs > rhs)) {
        greater_than_or_equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::less_than(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs < rhs)) {
        less_than(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::less_than_or_equal(const T1& lhs,
        const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs < rhs)) {
        less_than_or_equal(TestValue{lhs}, TestValue{rhs});
    }
    return *this;
}

#if defined(UTEST_USE_EXCEPTIONS)

template<typename T> auto
TestAssert::expected_throw(TestRun test_run) noexcept -> TestAssert& {
    if (test_run) {
        try {
            test_run(m_params);
            expected_throw(false);
        }
        catch (const T&) {
            /* Do nothing */
        }
        catch (const std::exception& e) {
            expected_throw(true, {e.what(), TestString::length(e.what())});
        }
        catch (...) {
            expected_throw(true);
        }
    }
    return *this;
}

template<> auto inline
TestAssert::expected_throw<std::exception>(
        TestRun test_run) noexcept -> TestAssert& {
    if (test_run) {
        try {
            test_run(m_params);
            expected_throw(false);
        }
        catch (const std::exception&) {
            /* Do nothing */
        }
        catch (...) {
            expected_throw(true);
        }
    }
    return *this;
}

#else

template<typename T> auto
TestAssert::expected_throw(TestRun test_run) noexcept -> TestAssert& {
    if (test_run) {
        test_run(m_params);
    }
    return *this;
}

#endif

}

#endif /* UTEST_TEST_ASSERT_HPP */
