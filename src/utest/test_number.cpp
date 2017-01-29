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
 * @file utest/test_number.cpp
 *
 * @brief Test number implementation
 */

#include <utest/test_number.hpp>
#include <utest/test_string.hpp>
#include <utest/test_span.hpp>

#include <cmath>
#include <limits>

using utest::TestNumber;
using utest::TestString;
using utest::TestSpan;
using utest::TestSize;

using Int = TestNumber::Int;
using Uint = TestNumber::Uint;
using Double = TestNumber::Double;

static_assert(2*sizeof(std::uintptr_t) <= TestNumber::MAX_STRING_BUFFER,
        "TestNumber::MAX_STRING_BUFFER smaller that address width");

static constexpr TestString STRING_BASE{"0123456789abcdef"};

static bool equal(Double lhs, Double rhs) noexcept {
    return std::abs(lhs - rhs) < std::numeric_limits<Double>::epsilon();
}

static bool is_signed(const TestNumber& number) noexcept {
    return number.is_signed() || (number.is_unsigned()
        && (Uint(number) <= std::numeric_limits<Int>::max()));
}

static bool is_unsigned(const TestNumber& number) noexcept {
    return number.is_unsigned() || (number.is_signed() && (Int(number) >= 0));
}

static bool overflow(const TestNumber& number) noexcept {
    return number.is_unsigned()
        && (Uint(number) > std::numeric_limits<Int>::max());
}

static bool underflow(const TestNumber& number) noexcept {
    return number.is_signed() && (Int(number) < 0);
}

template<>
Double TestNumber::get<Double>() const noexcept {
    Double value;

    if (is_floating()) {
        value = m_double;
    }
    else if (is_signed()) {
        value = Double(m_int);
    }
    else {
        value = Double(m_uint);
    }

    return value;
}

bool utest::operator==(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    bool ok{false};

    if (lhs.is_floating() || rhs.is_floating()) {
        ok = equal(lhs.get<Double>(), rhs.get<Double>());
    }
    else if (is_signed(lhs) && is_signed(rhs)) {
        ok = Int(lhs) == Int(rhs);
    }
    else if (is_unsigned(lhs) && is_unsigned(rhs)) {
        ok = Uint(lhs) == Uint(rhs);
    }

    return ok;
}

bool utest::operator<(const TestNumber& lhs, const TestNumber& rhs) noexcept {
    bool ok{false};

    if (lhs.is_floating() || rhs.is_floating()) {
        ok = lhs.get<Double>() < rhs.get<Double>();
    }
    else if (is_signed(lhs)) {
        ok = (Int(lhs) < Int(rhs)) || overflow(rhs);
    }
    else {
        ok = (Uint(lhs) < Uint(rhs)) && !underflow(rhs);
    }

    return ok;
}

static void integer_to_string_base(const TestNumber& number,
        TestSpan<char>& str, Uint base = 10) noexcept {
    auto value = number.is_signed() ? Uint(-Int(number)) : Uint(number);
    auto ptr = str.begin();
    TestSize size = 0;

    do {
        *(ptr++) = STRING_BASE[TestString::size_type(value % base)];
        value /= base;
        ++size;
    } while (0 != value);

    if (number.is_signed()) {
        *(ptr++) = '-';
        ++size;
    }

    auto it1 = str.begin();
    auto it2 = str.begin() + TestSpan<char>::difference_type(size) - 1;

    while (it1 < it2) {
        auto ch = *it2;
        *(it2--) = *it1;
        *(it1++) = ch;
    }

    str = {str.data(), size};
}

static void integer_to_string(const TestNumber& number,
        TestSpan<char>& str, Uint base = 10) noexcept {
    if (str.size() >= TestNumber::MAX_INTEGER_BUFFER && (0 != base)
            && (base <= STRING_BASE.size())) {
        integer_to_string_base(number, str, base);
    }
    else {
        str = {str.data(), 0};
    }
}

static void floating_to_string(const TestNumber& number,
        TestSpan<char>& str) noexcept {
    TestSize size = 0;

    if (str.size() >= TestNumber::MAX_FLOATING_BUFFER) {
        int exp = 0;
        auto value = Double(number);
        auto ptr = str.begin();

        if (std::signbit(value)) {
            value = std::abs(value);
            *(ptr++) = '-';
            ++size;
        }

        if (value < std::numeric_limits<Double>::epsilon()) {
            *(ptr++) = '0';
            *(ptr++) = '.';
            *(ptr++) = '0';
            size += 3;
        }
        else {
            Double integral;
            Double fractional;

            while (value >= 10.0) {
                value /= 10;
                ++exp;
            }

            while (value < 1.0) {
                value *= 10;
                --exp;
            }

            *(ptr++) = STRING_BASE[TestString::size_type(value)];
            ++size;

            *(ptr++) = '.';
            ++size;

            do {
                value *= 10;
                fractional = std::modf(value, &integral);
                *(ptr++) = STRING_BASE[TestString::size_type(integral) % 10];
                ++size;
            } while (fractional >= std::numeric_limits<Double>::epsilon());

            if (0 != exp) {
                *(ptr++) = 'e';
                ++size;

                TestSpan<char> tmp{&(*ptr), 0};
                integer_to_string_base(exp, tmp);
                size += tmp.size();
            }
        }
    }

    str = {str.data(), size};
}

void utest::to_string(const TestNumber& number, TestSpan<char>& str,
        int base) noexcept {
    if (number.is_floating()) {
        floating_to_string(number, str);
    }
    else {
        integer_to_string(number, str, Uint(base));
    }
}
