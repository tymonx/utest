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
 * @file test_utilities.hpp
 *
 * @brief Common utilities for tests
 */

#ifndef UTEST_TEST_UTILITIES_HPP
#define UTEST_TEST_UTILITIES_HPP

#include <utest/test_size.hpp>
#include <utest/test_span.hpp>
#include <utest/test_exception.hpp>

#include <algorithm>

namespace utest {

template<typename T, TestSize N> bool
operator==(const TestSpan<T>& lhs, const T (&rhs)[N]) {
    return lhs.size() == N ? std::equal(lhs.cbegin(), lhs.cend(), rhs) : false;
}

template<typename T, TestSize N> bool
operator!=(const TestSpan<T>& lhs, const T (&rhs)[N]) {
    return !(lhs == rhs);
}

union DummyUnion {
    int value;

    operator bool() const noexcept { return !!value; }

    bool operator==(int v) const noexcept { return value == v; }

    bool operator!=(int v) const noexcept { return value != v; }
};

}

#if defined(UTEST_USE_EXCEPTIONS)
#define THROW(_throw_exception)   do {throw _throw_exception;} while(0)
#else
#define THROW(_throw_exception)   do { } while(0)
#endif

#endif /* UTEST_TEST_UTILITIES_HPP */
