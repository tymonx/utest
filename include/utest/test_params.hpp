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
 * @file utest/test_params.hpp
 *
 * @brief Test params interface
 */

#ifndef UTEST_TEST_PARAMS_HPP
#define UTEST_TEST_PARAMS_HPP

#include <utest/test_string.hpp>
#include <utest/test_status.hpp>
#include <utest/test_span.hpp>
#include <utest/test_utilities.hpp>

#include <cstddef>
#include <csetjmp>

namespace utest {

class Test;
class TestCase;
class TestAssert;
class TestReporter;

class TestParams {
public:
    template<typename T>
    using context_pointer = typename std::enable_if<
        std::is_pointer<T>::value, T>::type;

    template<typename T>
    using context_reference = typename std::enable_if<
        !std::is_pointer<T>::value && std::is_object<T>::value,
        typename std::add_lvalue_reference<T>::type>::type;

    void context(std::nullptr_t test_context) noexcept;

    template<typename T>
    void context(T& test_context) noexcept;

    template<typename T = void>
    void context(T* test_context) noexcept;

    template<typename T = void*>
    context_pointer<T> context() noexcept;

    template<typename T = void*>
    const context_pointer<T> context() const noexcept;

    template<typename T>
    context_reference<T> context() noexcept;

    template<typename T>
    const context_reference<T> context() const noexcept;
private:
    friend class Test;
    friend class TestCase;
    friend class TestAssert;

    using TestContext = void*;
    using TestJumpBuffer = std::jmp_buf;
    using TestReporters = TestSpan<TestReporter*>;

    TestParams(TestCase& test_case) noexcept;

    void status(TestStatus test_status) noexcept;

    TestStatus status() const noexcept;

    const TestString& file() const noexcept;

    bool set_jump() noexcept;

    [[noreturn]] void jump() noexcept;

    TestReference<Test> m_test;
    TestJumpBuffer m_jump_buffer{};
    TestContext* m_context{};
    TestString m_file{};
    TestStatus m_status{TestStatus::PASS};
    bool m_non_fatal{false};
};

inline void
TestParams::context(std::nullptr_t) noexcept {
    *m_context = nullptr;
}

template<typename T> inline void
TestParams::context(T& test_context) noexcept {
    *m_context = static_cast<TestContext&>(&test_context);
}

template<typename T> inline void
TestParams::context(T* test_context) noexcept {
    *m_context = static_cast<TestContext>(test_context);
}

template<typename T> inline auto
TestParams::context() noexcept -> context_pointer<T> {
    return static_cast<T>(*m_context);
}

template<typename T> inline auto
TestParams::context() const noexcept -> const context_pointer<T> {
    return static_cast<T>(*m_context);
}

template<typename T> inline auto
TestParams::context() noexcept -> context_reference<T> {
    return *static_cast<T*>(*m_context);
}

template<typename T> inline auto
TestParams::context() const noexcept -> const context_reference<T> {
    return *static_cast<const T*>(*m_context);
}

inline auto
TestParams::file() const noexcept -> const TestString& {
    return m_file;
}

inline auto
TestParams::status() const noexcept -> TestStatus {
    return m_status;
}

inline void
TestParams::status(TestStatus test_status) noexcept {
    m_status = test_status;
}

}

#endif /* UTEST_TEST_PARAMS_HPP */
