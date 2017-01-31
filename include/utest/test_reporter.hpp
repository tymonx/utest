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
 * @file utest/test_repoter.hpp
 *
 * @brief Test reporter interface
 */

#ifndef UTEST_TEST_REPORTER_HPP
#define UTEST_TEST_REPORTER_HPP

#include <utest/test_span.hpp>
#include <utest/test_string.hpp>
#include <utest/test_color.hpp>

namespace utest {

class TestMessage;
class TestWritter;
class TestNumber;

class TestReporter {
public:
    using TestWritters = TestSpan<TestWritter*>;

    static TestReporter& get_default() noexcept;

    TestReporter() noexcept;

    TestReporter(const TestWritters& test_writters) noexcept;

    virtual void report(const TestMessage& test_message) noexcept = 0;

    virtual ~TestReporter() noexcept;
protected:
    void write(const TestString& str) noexcept;

    void write(const TestNumber& number, const TestString& str) noexcept;

    template<TestSize... N>
    void write(const TestNumber& number, const TestString& str,
            const char (&...args)[N]) noexcept;

    template<typename... Args>
    void write(const TestNumber& number, const TestString& str,
            const Args&... args) noexcept;

    template<TestSize... N>
    void write(const TestString& str, const char (&...args)[N]) noexcept;

    template<typename... Args>
    void write(const TestString& str, const Args&... args) noexcept;

    void color(TestColor c) noexcept;

    TestWritters m_writters{};
};

inline
TestReporter::TestReporter(const TestWritters& test_writters) noexcept :
    m_writters{test_writters}
{ }

template<TestSize... N>
void TestReporter::write(const TestString& str, const char (&...args)[N]) noexcept {
    write(str);
    write(args...);
}

template<TestSize... N>
void TestReporter::write(const TestNumber& number, const TestString& str,
        const char (&...args)[N]) noexcept {
    write(number, str);
    write(args...);
}

template<typename... Args>
void TestReporter::write(const TestNumber& number, const TestString& str,
        const Args&... args) noexcept {
    write(number, str);
    write(args...);
}

template<typename... Args>
void TestReporter::write(const TestString& str, const Args&... args) noexcept {
    write(str);
    write(args...);
}

}

#endif /* UTEST_TEST_REPORTER_HPP */
