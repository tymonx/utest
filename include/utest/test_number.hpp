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
 * @file utest/test_number.hpp
 *
 * @brief Test number interface
 */

#ifndef UTEST_TEST_NUMBER_HPP
#define UTEST_TEST_NUMBER_HPP

#include <utest/test_span.hpp>
#include <utest/test_size.hpp>

#include <cstdint>
#include <limits>
#include <cmath>

namespace utest {

template<typename T>
class TestSpan;

class TestNumber {
public:
    using Double = double;
    using Int = std::intmax_t;
    using Uint = std::uintmax_t;

    static constexpr TestSize MAX_ADDRESS_BUFFER =
        2 + 2*sizeof(std::uintptr_t);

    static constexpr TestSize MAX_INTEGER_BUFFER =
        std::numeric_limits<Int>::digits10 + 1;

    static constexpr TestSize MAX_FLOATING_BUFFER =
        std::numeric_limits<Double>::max_digits10 + 7;

    static constexpr TestSize MAX_STRING_BUFFER =
        (MAX_INTEGER_BUFFER < MAX_FLOATING_BUFFER)
        ? MAX_FLOATING_BUFFER : MAX_INTEGER_BUFFER;

    constexpr TestNumber() noexcept;

    constexpr TestNumber(float value) noexcept;

    constexpr TestNumber(double value) noexcept;

    constexpr TestNumber(signed char value) noexcept;

    constexpr TestNumber(unsigned char value) noexcept;

    constexpr TestNumber(signed short value) noexcept;

    constexpr TestNumber(unsigned short value) noexcept;

    constexpr TestNumber(signed int value) noexcept;

    constexpr TestNumber(unsigned int value) noexcept;

    constexpr TestNumber(signed long value) noexcept;

    constexpr TestNumber(unsigned long value) noexcept;

    constexpr TestNumber(signed long long value) noexcept;

    constexpr TestNumber(unsigned long long value) noexcept;

    constexpr operator Double() const noexcept;

    constexpr operator Int() const noexcept;

    constexpr operator Uint() const noexcept;

    template<typename T>
    T get() const noexcept;

    constexpr TestSize size() const noexcept;

    constexpr bool is_floating() const noexcept;

    constexpr bool is_integer() const noexcept;

    constexpr bool is_signed() const noexcept;

    constexpr bool is_unsigned() const noexcept;
private:
    union {
        Double m_double;
        Int m_int;
        Uint m_uint;
    };

    unsigned m_is_signed : 1;
    unsigned m_is_floating : 1;
    unsigned m_size : 6;
};

TestSpan<char> to_string(const TestNumber& number, const TestSpan<char>& str,
        int base = 10) noexcept;

TestSpan<char> to_string(const void* ptr, const TestSpan<char>& str) noexcept;

inline constexpr
TestNumber::TestNumber() noexcept :
    m_uint{},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(0)}
{ }

inline constexpr
TestNumber::TestNumber(float value) noexcept :
    m_double{Double(value)},
    m_is_signed{std::signbit(value)},
    m_is_floating{true},
    m_size{sizeof(float)}
{ }

inline constexpr
TestNumber::TestNumber(double value) noexcept :
    m_double{Double(value)},
    m_is_signed{std::signbit(value)},
    m_is_floating{true},
    m_size{sizeof(double)}
{ }

inline constexpr
TestNumber::TestNumber(signed char value) noexcept :
    m_int{Int(value)},
    m_is_signed{value < 0},
    m_is_floating{false},
    m_size{sizeof(signed char)}
{ }

inline constexpr
TestNumber::TestNumber(unsigned char value) noexcept :
    m_uint{Uint(value)},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(unsigned char)}
{ }

inline constexpr
TestNumber::TestNumber(signed short value) noexcept :
    m_int{Int(value)},
    m_is_signed{value < 0},
    m_is_floating{false},
    m_size{sizeof(signed short)}
{ }

inline constexpr
TestNumber::TestNumber(unsigned short value) noexcept :
    m_uint{Uint(value)},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(unsigned short)}
{ }

inline constexpr
TestNumber::TestNumber(signed int value) noexcept :
    m_int{Int(value)},
    m_is_signed{value < 0},
    m_is_floating{false},
    m_size{sizeof(signed int)}
{ }

inline constexpr
TestNumber::TestNumber(unsigned int value) noexcept :
    m_uint{Uint(value)},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(unsigned int)}
{ }

inline constexpr
TestNumber::TestNumber(signed long value) noexcept :
    m_int{Int(value)},
    m_is_signed{value < 0},
    m_is_floating{false},
    m_size{sizeof(signed long)}
{ }

inline constexpr
TestNumber::TestNumber(unsigned long value) noexcept :
    m_uint{Uint(value)},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(unsigned long)}
{ }

inline constexpr
TestNumber::TestNumber(signed long long value) noexcept :
    m_int{Int(value)},
    m_is_signed{value < 0},
    m_is_floating{false},
    m_size{sizeof(signed long long)}
{ }

inline constexpr
TestNumber::TestNumber(unsigned long long value) noexcept :
    m_uint{Uint(value)},
    m_is_signed{false},
    m_is_floating{false},
    m_size{sizeof(unsigned long long)}
{ }

inline constexpr
TestNumber::operator Double() const noexcept {
    return m_double;
}

inline constexpr
TestNumber::operator Int() const noexcept {
    return m_int;
}

inline constexpr
TestNumber::operator Uint() const noexcept {
    return m_uint;
}

template<typename T> inline auto
TestNumber::get() const noexcept -> T {
    T value;

    if (is_floating()) {
        value = T(m_double);
    }
    else if (is_signed()) {
        value = T(m_int);
    }
    else {
        value = T(m_uint);
    }

    return value;
}

template<> auto
TestNumber::get<TestNumber::Double>() const noexcept -> Double;

inline constexpr auto
TestNumber::size() const noexcept -> TestSize {
    return TestSize(m_size);
}

inline constexpr auto
TestNumber::is_floating() const noexcept -> bool {
    return m_is_floating;
}

inline constexpr auto
TestNumber::is_integer() const noexcept -> bool {
    return !m_is_floating;
}

inline constexpr auto
TestNumber::is_signed() const noexcept -> bool {
    return m_is_signed;
}

inline constexpr auto
TestNumber::is_unsigned() const noexcept -> bool {
    return !m_is_signed;
}

bool
operator==(const TestNumber& lhs, const TestNumber& rhs) noexcept;

static inline bool
operator!=(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    return !(lhs == rhs);
}

bool
operator<(const TestNumber& lhs, const TestNumber& rhs) noexcept;

static inline bool
operator>(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    return rhs < lhs;
}

static inline bool
operator<=(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    return !(lhs > rhs);
}

static inline bool
operator>=(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    return !(lhs < rhs);
}

}

#endif /* UTEST_TEST_NUMBER_HPP */
