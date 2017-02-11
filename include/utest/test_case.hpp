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
 * @file utest/test_case.hpp
 *
 * @brief Test case interface
 */

#ifndef UTEST_TEST_CASE_HPP
#define UTEST_TEST_CASE_HPP

#include <utest/test_string.hpp>
#include <utest/test_status.hpp>
#include <utest/test_span.hpp>
#include <utest/test_size.hpp>

namespace utest {

class Test;
class TestSuite;
class TestParams;
class TestParams;
class TestMessage;
class TestReporter;
class TestThread;

class TestCase {
public:
    using TestReporters = TestSpan<TestReporter*>;
    using TestFunction = void(*)(TestParams&);
    using TestContext = void*;

    TestCase& name(const TestString& str) noexcept;

    template<typename T>
    TestCase& context(T& test_context) noexcept;

    template<typename T = void>
    TestCase& context(T* test_context) noexcept;

    template<typename T = void>
    T* context() noexcept;

    template<typename T = void>
    const T* context() const noexcept;

    TestCase& setup(TestFunction test_setup) noexcept;

    TestFunction setup() noexcept;

    TestFunction setup() const noexcept;

    TestCase& teardown(TestFunction test_setup) noexcept;

    TestFunction teardown() noexcept;

    TestFunction teardown() const noexcept;

    const TestString& name() const noexcept;

    TestCase& file(const TestString& str) noexcept;

    const TestString& file() const noexcept;

    TestCase& line(TestSize number) noexcept;

    TestSize line() const noexcept;

    TestStatus status() const noexcept;

    TestCase& run(TestFunction test_run) noexcept;

    TestCase& fatal(bool test_fatal = true) noexcept;
private:
    friend class TestSuite;
    friend class TestParams;

    TestCase(TestSuite& test_suite) noexcept;

    void report(const TestMessage& test_message) noexcept;

    void run_setup(TestParams& test_params) noexcept;

    void run_teardown(TestParams& test_params) noexcept;

    void run_test(TestParams& test_params) noexcept;

    void test_execute(TestParams& test_params, TestFunction test_run);

    TestSize passed() const noexcept;

    TestSize failed() const noexcept;

    Test& m_test;
    TestContext m_context{};
    TestFunction m_setup{};
    TestFunction m_teardown{};
    TestFunction m_run{};
    TestString m_name{};
    TestString m_file{};
    TestSize m_line{};
    TestSize m_passed{};
    TestSize m_failed{};
    TestStatus m_status{TestStatus::PASS};
    bool m_non_fatal{false};
};

template<typename T> inline auto
TestCase::context(T& test_context) noexcept -> TestCase& {
    m_context = static_cast<void*>(&test_context);
    return *this;
}

template<typename T> inline auto
TestCase::context(T* test_context) noexcept -> TestCase& {
    m_context = static_cast<void*>(test_context);
    return *this;
}

template<typename T> inline auto
TestCase::context() noexcept -> T* {
    return static_cast<T*>(m_context);
}

template<typename T> inline auto
TestCase::context() const noexcept -> const T* {
    return static_cast<const T*>(m_context);
}

inline auto
TestCase::setup(TestFunction test_setup) noexcept -> TestCase& {
    m_setup = test_setup;
    return *this;
}

inline auto
TestCase::setup() noexcept -> TestFunction {
    return m_setup;
}

inline auto
TestCase::setup() const noexcept -> TestFunction {
    return m_setup;
}

inline auto
TestCase::teardown(TestFunction test_setup) noexcept -> TestCase& {
    m_teardown = test_setup;
    return *this;
}

inline auto
TestCase::teardown() noexcept -> TestFunction {
    return m_teardown;
}

inline auto
TestCase::teardown() const noexcept -> TestFunction {
    return m_teardown;
}

inline auto
TestCase::name(const TestString& str) noexcept -> TestCase& {
    m_name = str;
    return *this;
}

inline auto
TestCase::name() const noexcept -> const TestString& {
    return m_name;
}

inline auto
TestCase::file(const TestString& str) noexcept -> TestCase& {
    m_file = str;
    return *this;
}

inline auto
TestCase::file() const noexcept -> const TestString& {
    return m_file;
}

inline auto
TestCase::line(TestSize number) noexcept -> TestCase& {
    m_line = number;
    return *this;
}

inline auto
TestCase::line() const noexcept -> TestSize {
    return m_line;
}

inline auto
TestCase::status() const noexcept -> TestStatus {
    return m_status;
}

inline auto
TestCase::passed() const noexcept -> TestSize {
    return m_passed;
}

inline auto
TestCase::failed() const noexcept -> TestSize {
    return m_failed;
}

inline auto
TestCase::fatal(bool test_fatal) noexcept -> TestCase& {
    m_non_fatal = !test_fatal;
    return *this;
}

}

#endif /* UTEST_TEST_CASE_HPP */
