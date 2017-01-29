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
 * @file utest/test_suite.hpp
 *
 * @brief Test suite interface
 */

#ifndef UTEST_TEST_SUITE_HPP
#define UTEST_TEST_SUITE_HPP

#include <utest/test_string.hpp>
#include <utest/test_status.hpp>
#include <utest/test_span.hpp>
#include <utest/test_size.hpp>

namespace utest {

class Test;
class TestCase;
class TestRunner;
class TestMessage;
class TestReporter;

class TestSuite {
public:
    using TestReporters = TestSpan<TestReporter*>;
    using TestRun = void (*)(TestCase& test_case);

    TestSuite& name(const TestString& str) noexcept;

    const TestString& name() const noexcept;

    TestSuite& file(const TestString& str) noexcept;

    const TestString& file() const noexcept;

    TestSuite& line(TestSize number) noexcept;

    auto line() const noexcept -> TestSize;

    TestStatus status() const noexcept;

    TestSuite& run(TestRun test_run) noexcept;
private:
    friend class Test;
    friend class TestCase;
    friend class TestRunner;

    TestSuite(const Test& test) noexcept;

    void report(const TestMessage& test_message) noexcept;

    TestSize passed() const noexcept;

    TestSize failed() const noexcept;

    TestSize test_cases_passed() const noexcept;

    TestSize test_cases_failed() const noexcept;

    TestReporters m_reporters{};
    TestString m_name{};
    TestString m_file{};
    TestSize m_line{};
    TestSize m_passed{0};
    TestSize m_failed{0};
    TestSize m_test_cases_passed{0};
    TestSize m_test_cases_failed{0};
    TestStatus m_status{TestStatus::PASS};
};

inline auto
TestSuite::name(const TestString& str) noexcept -> TestSuite& {
    m_name = str;
    return *this;
}

inline auto
TestSuite::name() const noexcept -> const TestString& {
    return m_name;
}

inline auto
TestSuite::file(const TestString& str) noexcept -> TestSuite& {
    m_file = str;
    return *this;
}

inline auto
TestSuite::file() const noexcept -> const TestString& {
    return m_file;
}

inline auto
TestSuite::line(TestSize number) noexcept -> TestSuite& {
    m_line = number;
    return *this;
}

inline auto
TestSuite::line() const noexcept -> TestSize {
    return m_line;
}

inline auto
TestSuite::status() const noexcept -> TestStatus {
    return m_status;
}

inline auto
TestSuite::passed() const noexcept -> TestSize {
    return m_passed;
}

inline auto
TestSuite::failed() const noexcept -> TestSize {
    return m_failed;
}

inline auto
TestSuite::test_cases_passed() const noexcept -> TestSize {
    return m_test_cases_passed;
}

inline auto
TestSuite::test_cases_failed() const noexcept -> TestSize {
    return m_test_cases_failed;
}

}

#endif /* UTEST_TEST_SUITE_HPP */
