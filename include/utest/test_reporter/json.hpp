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
 * @file utest/test_reporter/json.hpp
 *
 * @brief Test reporter JSON interface
 */

#ifndef UTEST_TEST_REPORTER_JSON_HPP
#define UTEST_TEST_REPORTER_JSON_HPP

#include <utest/test_reporter.hpp>
#include <utest/test_message.hpp>

namespace utest {

class TestValue;

namespace test_message {
    class TestAssertBase;
    class TestAssertCompare;
}

namespace test_reporter {

class JSON final : public TestReporter {
public:
    static constexpr TestSize DEFAULT_INDENT{4};

    using TestReporter::TestReporter;
    using TestReporter::color;

    JSON& compact(bool enable = true) noexcept;

    JSON& indent(TestSize step) noexcept;

    virtual void report(const TestMessage& message) noexcept override;

    virtual ~JSON() noexcept override;
private:
    enum Indent {
        INCREASE,
        DECREASE
    };

    template<Indent T>
    JSON& indent() noexcept;

    JSON& indent() noexcept;

    JSON& endl() noexcept;

    JSON& append() noexcept;

    JSON& key(const TestString& str) noexcept;

    JSON& value(const TestValue& val) noexcept;

    JSON& value(const TestString& str) noexcept;

    template<typename... Args>
    JSON& write(const Args&... args) noexcept;

    template<TestMessage::Type T>
    void report(const TestMessage& message) noexcept;

    void report(const TestString& str,
            const test_message::TestAssertBase& base) noexcept;

    void report(const TestString& str,
            const test_message::TestAssertCompare& compare) noexcept;

    void report_exception(const TestString& str,
            const TestMessage& message) noexcept;

    TestSize m_indent{0};
    TestSize m_indent_step{DEFAULT_INDENT};
    bool m_compact{false};
    bool m_next_test_suite{false};
    bool m_next_test_case{false};
    bool m_next_test_assert{false};
    bool m_test_asserts{false};
    bool m_explanation{false};
};

inline auto
JSON::indent(TestSize step) noexcept -> JSON& {
    m_indent_step = step;
    return *this;
}

inline auto
JSON::compact(bool enable) noexcept -> JSON& {
    m_compact = enable;
    return *this;
}

template<typename... Args> inline auto
JSON::write(const Args&... args) noexcept -> JSON& {
    TestReporter::write(args...);
    return *this;
}

}
}

#endif /* UTEST_TEST_REPORTER_JSON_HPP */
