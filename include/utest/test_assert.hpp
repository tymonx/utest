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

#include <utest/test_status.hpp>
#include <utest/test_value.hpp>
#include <utest/test_exception.hpp>

#include <limits>
#include <cstddef>

namespace utest {

class TestMessage;
class TestParams;

class TestAssert {
public:
    template<typename T>
    using enable_signed = typename std::enable_if<
            std::is_integral<T>::value && std::is_signed<T>::value
        , int>::type;

    template<typename T>
    using enable_unsigned = typename std::enable_if<
            std::is_integral<T>::value && std::is_unsigned<T>::value
        , unsigned>::type;

    template<typename T>
    using enable_floating = typename std::enable_if<
            std::is_floating_point<T>::value
        , short>::type;

    template<typename T1, typename T2>
    using enable_compare = typename std::enable_if<
            !((std::is_floating_point<T1>::value ||
            std::is_floating_point<T2>::value) &&
            std::is_convertible<T1, double>::value &&
            std::is_convertible<T2, double>::value)
        , long>::type;

    template<typename T1, typename T2>
    using enable_floating_compare = typename std::enable_if<
            (std::is_floating_point<T1>::value ||
            std::is_floating_point<T2>::value) &&
            std::is_convertible<T1, double>::value &&
            std::is_convertible<T2, double>::value
        , unsigned long>::type;

    using TestRun = void(*)(TestParams& params);

    TestAssert(TestParams& params) noexcept;

    ~TestAssert() noexcept;

    TestAssert& line(TestSize test_line) noexcept;

    TestSize line() const noexcept;

    TestAssert& file(const TestString& test_file) noexcept;

    const TestString& file() const noexcept;

    TestStatus status() const noexcept;

    TestAssert& fatal(bool test_fatal = true) noexcept;

    TestAssert& fail() noexcept;

    template<typename T>
    TestAssert& is_true(const T& value) noexcept;

    template<typename T>
    TestAssert& is_false(const T& value) noexcept;

    template<typename T1, typename T2, enable_compare<T1, T2> = 0>
    TestAssert& equal(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2, enable_compare<T1, T2> = 0>
    TestAssert& not_equal(const T1& lhs, const T2& rhs) noexcept;

    template<typename T1, typename T2, enable_floating_compare<T1, T2> = 0>
    TestAssert& equal(const T1& lhs, const T2& rhs,
            double epsilon = std::numeric_limits<double>::epsilon()) noexcept;

    template<typename T1, typename T2, enable_floating_compare<T1, T2> = 0>
    TestAssert& not_equal(const T1& lhs, const T2& rhs,
            double epsilon = std::numeric_limits<double>::epsilon()) noexcept;

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

    TestAssert& operator<<(bool value) noexcept;

    TestAssert& operator<<(std::nullptr_t) noexcept;

    TestAssert& operator<<(const void* ptr) noexcept;

    template<TestSize N>
    TestAssert& operator<<(const char (&arr)[N]) noexcept;

    TestAssert& operator<<(const TestString& str) noexcept;

    TestAssert& operator<<(std::intmax_t value) noexcept;

    TestAssert& operator<<(std::uintmax_t value) noexcept;

    TestAssert& operator<<(double value) noexcept;

    template<typename T, enable_signed<T> = 0>
    TestAssert& operator<<(T value) noexcept;

    template<typename T, enable_unsigned<T> = 0>
    TestAssert& operator<<(T value) noexcept;

    template<typename T, enable_floating<T> = 0>
    TestAssert& operator<<(T value) noexcept;
private:
    void report(const TestMessage& test_message) noexcept;

    bool equal(double lhs, double rhs, double epsilon) noexcept;

    void report_is_true(const TestValue& value) noexcept;

    void report_is_false(const TestValue& value) noexcept;

    void report_equal(const TestValue& lhs, const TestValue& rhs) noexcept;

    void report_not_equal(const TestValue& lhs, const TestValue& rhs) noexcept;

    void report_greater_than(const TestValue& lhs,
            const TestValue& rhs) noexcept;

    void report_greater_than_or_equal(const TestValue& lhs,
            const TestValue& rhs) noexcept;

    void report_less_than(const TestValue& lhs, const TestValue& rhs) noexcept;

    void report_less_than_or_equal(const TestValue& lhs,
            const TestValue& rhs) noexcept;

    void report_expected_throw(bool throws, const TestString& str = {}) noexcept;

    TestParams& m_params;
    TestString m_file{};
    TestSize m_line{0};
    TestStatus m_status{TestStatus::PASS};
    bool m_explanation{false};
    bool m_non_fatal{false};
};

inline auto
TestAssert::file(const TestString& test_file) noexcept -> TestAssert& {
    m_file = test_file;
    return *this;
}

inline auto
TestAssert::file() const noexcept -> const TestString& {
    return m_file;
}

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

template<typename T, TestAssert::enable_signed<T>> inline auto
TestAssert::operator<<(T value) noexcept -> TestAssert& {
    return operator<<(std::intmax_t(value));
}

template<typename T, TestAssert::enable_unsigned<T>> inline auto
TestAssert::operator<<(T value) noexcept -> TestAssert& {
    return operator<<(std::uintmax_t(value));
}

template<typename T, TestAssert::enable_floating<T>> inline auto
TestAssert::operator<<(T value) noexcept -> TestAssert& {
    return operator<<(double(value));
}

template<typename T> auto
TestAssert::is_true(const T& value) noexcept -> TestAssert& {
    if (!value) {
        report_is_true({value});
    }
    return *this;
}

template<typename T> auto
TestAssert::is_false(const T& value) noexcept -> TestAssert& {
    if (!!value) {
        report_is_false({value});
    }
    return *this;
}

template<typename T1, typename T2, TestAssert::enable_compare<T1, T2>> auto
TestAssert::equal(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs == rhs)) {
        report_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2, TestAssert::enable_compare<T1, T2>> auto
TestAssert::not_equal(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs != rhs)) {
        report_not_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2,
    TestAssert::enable_floating_compare<T1, T2>> auto
TestAssert::equal(const T1& lhs, const T2& rhs,
        double epsilon) noexcept -> TestAssert& {
    if (!equal(double(lhs), double(rhs), epsilon)) {
        report_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2,
    TestAssert::enable_floating_compare<T1, T2>> auto
TestAssert::not_equal(const T1& lhs, const T2& rhs,
        double epsilon) noexcept -> TestAssert& {
    if (equal(double(lhs), double(rhs), epsilon)) {
        report_not_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::greater_than(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs > rhs)) {
        report_greater_than({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::greater_than_or_equal(const T1& lhs,
        const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs >= rhs)) {
        report_greater_than_or_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::less_than(const T1& lhs, const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs < rhs)) {
        report_less_than({lhs}, {rhs});
    }
    return *this;
}

template<typename T1, typename T2> auto
TestAssert::less_than_or_equal(const T1& lhs,
        const T2& rhs) noexcept -> TestAssert& {
    if (!(lhs <= rhs)) {
        report_less_than_or_equal({lhs}, {rhs});
    }
    return *this;
}

template<typename T> auto
TestAssert::expected_throw(TestRun test_run) noexcept -> TestAssert& {
#if defined(UTEST_USE_EXCEPTIONS)
    if (test_run) {
        try {
            test_run(m_params);
            report_expected_throw(false);
        }
        catch (const T&) {
            /* Do nothing */
        }
        catch (const std::exception& e) {
            report_expected_throw(true, e.what());
        }
        catch (...) {
            report_expected_throw(true);
        }
    }
#else
    (void)test_run;
#endif
    return *this;
}

template<> auto inline
TestAssert::expected_throw<std::exception>(
        TestRun test_run) noexcept -> TestAssert& {
#if defined(UTEST_USE_EXCEPTIONS)
    if (test_run) {
        try {
            test_run(m_params);
            report_expected_throw(false);
        }
        catch (const std::exception&) {
            /* Do nothing */
        }
        catch (...) {
            report_expected_throw(true);
        }
    }
#else
    (void)test_run;
#endif
    return *this;
}

}

#endif /* UTEST_TEST_ASSERT_HPP */
