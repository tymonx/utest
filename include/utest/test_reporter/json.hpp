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
#include <utest/test_value.hpp>

namespace utest {

class TestNumber;
class TestString;

namespace test_message { class TestAssertCompare; }

namespace test_reporter {

class JSON final : public TestReporter {
public:
    using TestReporter::TestReporter;
    using TestReporter::color;

    static constexpr TestSize DEFAULT_INDENT{4};

    JSON& compress(bool enable = true) noexcept;

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

    JSON& key(const TestString& str) noexcept;

    JSON& name(const TestString& str) noexcept;

    JSON& write(const TestString& str) noexcept;

    JSON& write(const TestNumber& number) noexcept;

    template<TestSize... N>
    JSON& write(const TestNumber& number,
            const char (&...args)[N]) noexcept;

    template<typename... Args>
    JSON& write(const TestNumber& number, const Args&... args) noexcept;

    template<TestSize... N>
    JSON& write(const TestString& str,
            const char (&...args)[N]) noexcept;

    template<typename... Args>
    JSON& write(const TestString& str, const Args&... args) noexcept;

    template<TestMessage::Type T>
    void report(const TestMessage& message) noexcept;

    TestSize m_indent{0};
    TestSize m_indent_step{DEFAULT_INDENT};
    bool m_compress{false};
    bool m_next_test_suite{false};
    bool m_next_test_case{false};
    bool m_next_test_assert{false};
    bool m_test_asserts{false};
};

inline auto
JSON::compress(bool enable) noexcept -> JSON& {
    m_compress = enable;
    return *this;
}

inline auto
JSON::write(const TestString& str) noexcept -> JSON& {
    TestReporter::write(str);
    return *this;
}

inline auto
JSON::write(const TestNumber& number) noexcept -> JSON& {
    TestReporter::write(number);
    return *this;
}

template<TestSize... N> auto
JSON::write(const TestString& str,
        const char (&...args)[N]) noexcept -> JSON& {
    write(str);
    write(args...);
    return *this;
}

template<TestSize... N> auto
JSON::write(const TestNumber& number,
        const char (&...args)[N]) noexcept -> JSON& {
    write(number);
    write(args...);
    return *this;
}

template<typename... Args> auto
JSON::write(const TestNumber& number,
        const Args&... args) noexcept -> JSON& {
    write(number);
    write(args...);
    return *this;
}

template<typename... Args> auto
JSON::write(const TestString& str,
        const Args&... args) noexcept -> JSON& {
    write(str);
    write(args...);
    return *this;
}

}
}

#endif /* UTEST_TEST_REPORTER_JSON_HPP */
