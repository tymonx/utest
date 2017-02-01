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

#include <utest/test_string.hpp>
#include <utest/test_number.hpp>
#include <utest/test_status.hpp>
#include <utest/test_size.hpp>

#include <cstddef>
#include <type_traits>

namespace utest {

class TestMessage;
class TestParams;

class TestAssert {
public:
    template<typename T>
    using enable_object = typename std::enable_if<
            !std::is_integral<T>::value && !std::is_floating_point<T>::value &&
            !std::is_same<T, TestString>::value,
        int>::type;

    TestAssert(TestParams& params) noexcept;

    ~TestAssert() noexcept;

    TestAssert& line(TestSize test_line) noexcept;

    TestSize line() const noexcept;

    const TestString& file() const noexcept;

    TestStatus status() const noexcept;

    TestAssert& fatal(bool test_fatal = true) noexcept;

    TestAssert& is_true(bool value) noexcept;

    TestAssert& is_false(bool value) noexcept;

    TestAssert& equal(const TestNumber& lhs, const TestNumber& rhs) noexcept;

    TestAssert& not_equal(const TestNumber& lhs,
            const TestNumber& rhs) noexcept;

    TestAssert& equal(const TestString& lhs, const TestString& rhs) noexcept;

    TestAssert& not_equal(const TestString& lhs,
            const TestString& rhs) noexcept;

    template<typename T, enable_object<T> = 0>
    TestAssert& equal(const T& lhs, const T& rhs) noexcept;

    template<typename T, enable_object<T> = 0>
    TestAssert& not_equal(const T& lhs, const T& rhs) noexcept;

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

    void report_equal(const void* lhs, const void* rhs) noexcept;

    void report_not_equal(const void* lhs, const void* rhs) noexcept;

    TestParams& m_params;
    TestSize m_line{0};
    TestStatus m_status{TestStatus::PASS};
    bool m_explanation{false};
    bool m_non_fatal{false};
};

inline
TestAssert::TestAssert(TestParams& params) noexcept :
    m_params{params}
{ }

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

template<typename T, TestAssert::enable_object<T>> inline auto
TestAssert::equal(const T& lhs, const T& rhs) noexcept -> TestAssert& {
    if (!(lhs == rhs)) {
        m_status = TestStatus::FAIL;
        report_equal(&lhs, &rhs);
    }
    return *this;
}

template<typename T, TestAssert::enable_object<T>> inline auto
TestAssert::not_equal(const T& lhs, const T& rhs) noexcept -> TestAssert& {
    if (!(lhs != rhs)) {
        m_status = TestStatus::FAIL;
        report_not_equal(&lhs, &rhs);
    }
    return *this;
}

}

#endif /* UTEST_TEST_ASSERT_HPP */
