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
 * @file utest/test_string.cpp
 *
 * @brief Test string implementation
 */

#include <utest/test_string.hpp>

#include <cctype>
#include <limits>
#include <cmath>

using utest::TestSpan;
using utest::TestSize;
using utest::TestString;

static constexpr TestString STRING_BASE{"0123456789abcdef"};

static constexpr TestSize MAX_ADDRESS_BUFFER =
    2 + 2*sizeof(std::uintptr_t);

static constexpr TestSize MAX_INTEGER_BUFFER =
    std::numeric_limits<std::intmax_t>::digits10 + 1;

static constexpr TestSize MAX_FLOATING_WRITE =
    std::numeric_limits<double>::max_digits10 + 2;

static constexpr TestSize MAX_FLOATING_BUFFER = MAX_FLOATING_WRITE + 5;

static_assert(sizeof(TestString::Buffer) >= MAX_ADDRESS_BUFFER,
        "TestString::Buffer must be bigger");

static_assert(sizeof(TestString::Buffer) >= MAX_INTEGER_BUFFER,
        "TestString::Buffer must be bigger");

static_assert(sizeof(TestString::Buffer) >= MAX_FLOATING_BUFFER,
        "TestString::Buffer must be bigger");

TestSize TestString::length(const_pointer str) noexcept {
    TestSize size{0};

    if (str) {
        while (('\0' != *str) && (size < MAX_LENGTH)) {
            ++str;
            ++size;
        }
    }

    return size;
}

bool utest::operator==(const TestString& str1,
        const TestString& str2) noexcept {
    bool status{false};

    if (str1.size() == str2.size()) {
        if (str1.ignoring_case()) {
            status = std::equal(str1.cbegin(), str1.cend(), str2.cbegin(),
                [] (const TestString::value_type& ch1,
                        const TestString::value_type& ch2) {
                    return std::tolower(ch1) == std::tolower(ch2);
                }
            );
        }
        else {
            status = std::equal(str1.cbegin(), str1.cend(), str2.cbegin());
        }
    }

    return status;
}

static TestString integer_to_string_base(std::uintmax_t value,
        const TestSpan<char>& buffer, int base = 10) noexcept {
    auto ptr = const_cast<char*>(buffer.data());
    TestSize size = 0;

    do {
        *(ptr++) = STRING_BASE[TestString::size_type(value % unsigned(base))];
        value /= unsigned(base);
        ++size;
    } while (0 != value);

    TestSpan<char> str = {const_cast<char*>(buffer.data()), buffer.size()};
    auto it1 = str.begin();
    auto it2 = str.begin() + TestSpan<char>::difference_type(size) - 1;

    while (it1 < it2) {
        auto ch = *it2;
        *(it2--) = *it1;
        *(it1++) = ch;
    }

    return {buffer.data(), size};
}

static TestString integer_to_string_base(std::intmax_t value,
        const TestSpan<char>& buffer, int base = 10) noexcept {
    TestString str;

    if (value < 0) {
        auto ptr = const_cast<char*>(buffer.data());

        ptr[0] = '-';
        str = {
            buffer.data(), integer_to_string_base(std::uintmax_t(-value),
                    {ptr + 1, 0}, base).size() + 1
        };
    }
    else {
        str = integer_to_string_base(std::uintmax_t(value), buffer, base);
    }

    return str;
}

TestString utest::to_string(std::intmax_t value, const TestSpan<char>& buffer,
        int base) noexcept {
    TestString str;

    if (buffer.size() >= MAX_INTEGER_BUFFER && (0 != base)
            && (base <= int(STRING_BASE.size()))) {
        str = integer_to_string_base(value, buffer, base);
    }

    return str;
}

TestString utest::to_string(std::uintmax_t value, const TestSpan<char>& buffer,
        int base) noexcept {
    TestString str;

    if (buffer.size() >= MAX_INTEGER_BUFFER && (0 != base)
            && (base <= int(STRING_BASE.size()))) {
        str = integer_to_string_base(value, buffer, base);
    }

    return str;
}

TestString utest::to_string(double value,
        const TestSpan<char>& buffer) noexcept {
    TestSize size = 0;

    if (buffer.size() >= MAX_FLOATING_BUFFER) {
        int exp = 0;
        auto ptr = const_cast<char*>(buffer.data());

        if (std::signbit(value)) {
            value = std::abs(value);
            *(ptr++) = '-';
            ++size;
        }

        if (value < std::numeric_limits<double>::epsilon()) {
            *(ptr++) = '0';
            *(ptr++) = '.';
            *(ptr++) = '0';
            size += 3;
        }
        else {
            double integral;
            double fractional;

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
            } while ((fractional >= std::numeric_limits<double>::epsilon())
                    && (size < MAX_FLOATING_WRITE));

            if (0 != exp) {
                *(ptr++) = 'e';
                ++size;

                TestSpan<char> tmp{&(*ptr), 0};
                size += integer_to_string_base(std::intmax_t(exp), tmp).size();
            }
        }
    }

    return {buffer.data(), size};
}

TestString utest::to_string(const void* ptr,
        const TestSpan<char>& buffer) noexcept {
    TestSize size = 0;

    if (buffer.size() >= MAX_ADDRESS_BUFFER) {
        TestSpan<char> str = {const_cast<char*>(buffer.data()), buffer.size()};

        str[0] = '0';
        str[1] = 'x';
        str = {str.data() + 2, MAX_ADDRESS_BUFFER - 2};

        auto it1 = integer_to_string_base(std::uintmax_t(ptr),
                str, 16).cend() - 1;
        auto it2 = str.begin() +
            TestString::difference_type(2*sizeof(std::uintptr_t)) - 1;

        if (it1 < it2) {
            while (it1 >= str.cbegin()) {
                *it2-- = *it1--;
            }

            while (it2 >= str.cbegin()) {
                *it2-- = '0';
            }
        }

        size = MAX_ADDRESS_BUFFER;
    }

    return {buffer.data(), size};
}
