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
 * @file utest/test_reporter/google_test.hpp
 *
 * @brief Test reporter Google Test interface
 */

#ifndef UTEST_TEST_REPORTER_GOOGLE_TEST_HPP
#define UTEST_TEST_REPORTER_GOOGLE_TEST_HPP

#include <utest/test_reporter.hpp>
#include <utest/test_message.hpp>

namespace utest {

class TestNumber;
class TestString;

namespace test_message { class TestAssertValue; }

namespace test_reporter {

class GoogleTest final : public TestReporter {
public:
    using TestReporter::TestReporter;

    virtual void report(const TestMessage& message) noexcept override;

    virtual ~GoogleTest() noexcept;
private:
    using TestReporter::write;
    using TestAssertValue = test_message::TestAssertValue;

    void failure(const TestString& file, const TestNumber& line) noexcept;

    template<typename T>
    void failure(const T& message) noexcept;

    void write_exception(const TestString& message) noexcept;

    void write_address(std::uintptr_t address) noexcept;

    void write(const TestNumber& number, const TestString& str) noexcept;

    void write(const TestString& str, const TestAssertValue& value) noexcept;

    template<typename ...Args>
    void write(const TestString& str, const TestAssertValue& value,
            const Args&... args) noexcept;

    template<bool T>
    void write(const TestString& str) noexcept;

    template<TestMessage::Type T>
    void report(const TestMessage& message) noexcept;
};

template<typename T> inline void
GoogleTest::failure(const T& message) noexcept {
    failure(message.file(), message.line());
}

template<typename ...Args> inline void
GoogleTest::write(const TestString& str, const TestAssertValue& value,
        const Args&... args) noexcept {
    write(str, value);
    write(args...);
}

}
}

#endif /* UTEST_TEST_REPORTER_GOOGLE_TEST_HPP */
