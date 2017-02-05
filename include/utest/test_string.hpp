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
 * @file utest/test_string.hpp
 *
 * @brief Test string interface
 */

#ifndef UTEST_TEST_STRING_HPP
#define UTEST_TEST_STRING_HPP

#include <utest/test_span.hpp>
#include <utest/test_size.hpp>

#include <limits>
#include <type_traits>

namespace utest {

class TestString : public TestSpan<const char> {
public:
    static constexpr TestSize MAX_LENGTH{std::numeric_limits<TestSize>::max()};

    using TestSpan<const char>::TestSpan;
    using TestSpan<const char>::TestSpan::length;

    constexpr TestString() noexcept = default;

    TestString& ignore_case(bool enable = true) noexcept;

    constexpr bool ignoring_case() const noexcept;

    template<TestSize N>
    constexpr TestString(value_type (&arr)[N]) noexcept;

    static TestSize length(const_pointer str) noexcept;
private:
    bool m_ignore_case{false};
};

template<TestSize N> inline constexpr
TestString::TestString(value_type (&arr)[N]) noexcept :
    TestSpan{arr, N - 1}
{ }

inline constexpr auto
TestString::ignoring_case() const noexcept -> bool {
    return m_ignore_case;
}

inline auto
TestString::ignore_case(bool enable) noexcept -> TestString& {
    m_ignore_case = enable;
    return *this;
}

auto
operator==(const TestString& str1, const TestString& str2) noexcept -> bool;

static inline auto
operator!=(const TestString& str1, const TestString& str2) noexcept -> bool {
    return !(str1 == str2);
}

}

#endif /* UTEST_TEST_STRING_HPP */
