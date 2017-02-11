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
class TestParams;
class TestRunner;
class TestMessage;
class TestReporter;
class TestThread;

class TestSuite {
public:
    using TestReporters = TestSpan<TestReporter*>;
    using TestFunction = void(*)(TestParams&);
    using TestRun = void (*)(TestCase& test_case);
    using TestContext = void*;

    template<typename T>
    TestSuite& context(T& test_context) noexcept;

    template<typename T = void>
    TestSuite& context(T* test_context) noexcept;

    template<typename T = void>
    T* context() noexcept;

    template<typename T = void>
    const T* context() const noexcept;

    TestSuite& name(const TestString& str) noexcept;

    const TestString& name() const noexcept;

    TestSuite& file(const TestString& str) noexcept;

    const TestString& file() const noexcept;

    TestSuite& line(TestSize number) noexcept;

    TestSize line() const noexcept;

    TestSuite& setup(TestFunction test_setup) noexcept;

    TestFunction setup() noexcept;

    TestFunction setup() const noexcept;

    TestSuite& teardown(TestFunction test_setup) noexcept;

    TestFunction teardown() noexcept;

    TestFunction teardown() const noexcept;

    TestStatus status() const noexcept;

    TestSuite& run(TestRun test_run) noexcept;

    TestSuite& fatal(bool test_fatal = true) noexcept;
private:
    friend class Test;
    friend class TestCase;
    friend class TestRunner;

    TestSuite(Test& test) noexcept;

    void report(const TestMessage& test_message) noexcept;

    TestSize passed() const noexcept;

    TestSize failed() const noexcept;

    TestSize test_cases_passed() const noexcept;

    TestSize test_cases_failed() const noexcept;

    Test& m_test;
    TestContext m_context{};
    TestFunction m_setup{};
    TestFunction m_teardown{};
    TestString m_name{};
    TestString m_file{};
    TestSize m_line{0};
    TestSize m_passed{0};
    TestSize m_failed{0};
    TestSize m_test_cases_passed{0};
    TestSize m_test_cases_failed{0};
    bool m_non_fatal{false};
};

template<typename T> inline auto
TestSuite::context(T& test_context) noexcept -> TestSuite& {
    m_context = static_cast<void*>(&test_context);
    return *this;
}

template<typename T> inline auto
TestSuite::context(T* test_context) noexcept -> TestSuite& {
    m_context = static_cast<void*>(test_context);
    return *this;
}

template<typename T> inline auto
TestSuite::context() noexcept -> T* {
    return static_cast<T*>(m_context);
}

template<typename T> inline auto
TestSuite::context() const noexcept -> const T* {
    return static_cast<const T*>(m_context);
}

inline auto
TestSuite::setup(TestFunction test_setup) noexcept -> TestSuite& {
    m_setup = test_setup;
    return *this;
}

inline auto
TestSuite::setup() noexcept -> TestFunction {
    return m_setup;
}

inline auto
TestSuite::setup() const noexcept -> TestFunction {
    return m_setup;
}

inline auto
TestSuite::teardown(TestFunction test_setup) noexcept -> TestSuite& {
    m_teardown = test_setup;
    return *this;
}

inline auto
TestSuite::teardown() noexcept -> TestFunction {
    return m_teardown;
}

inline auto
TestSuite::teardown() const noexcept -> TestFunction {
    return m_teardown;
}

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
    return m_failed ? TestStatus::FAIL : TestStatus::PASS;
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

inline auto
TestSuite::fatal(bool test_fatal) noexcept -> TestSuite& {
    m_non_fatal = !test_fatal;
    return *this;
}

}

#endif /* UTEST_TEST_SUITE_HPP */
