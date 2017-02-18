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

class TestValue;
class TestString;

namespace test_message { class TestAssertCompare; }

namespace test_reporter {

class GoogleTest final : public TestReporter {
public:
    using TestReporter::TestReporter;
    using TestReporter::color;

    virtual void report(const TestMessage& message) noexcept override;

    virtual ~GoogleTest() noexcept override;
private:
    using TestReporter::write;

    void failure(const TestString& file, TestSize line) noexcept;

    template<typename T>
    void failure(const T& message) noexcept;

    void write_exception(const TestString& message) noexcept;

    template<TestMessage::Type T>
    void report(const TestMessage& message) noexcept;

    void report(const test_message::TestAssertCompare& message,
            const TestString& str) noexcept;

    template<bool T>
    void write(const TestString& str) noexcept;

    void write_type(const TestValue& value) noexcept;

    void write_value(const TestValue& value) noexcept;
};

template<typename T> void
GoogleTest::failure(const T& message) noexcept {
    failure(message.file(), message.line());
}

}
}

#endif /* UTEST_TEST_REPORTER_GOOGLE_TEST_HPP */
