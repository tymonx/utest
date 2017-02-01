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
 * @file utest/test_message/test_assert.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_ASSERT_HPP
#define UTEST_TEST_MESSAGE_TEST_ASSERT_HPP

#include <utest/test_assert.hpp>
#include <utest/test_message.hpp>
#include <utest/test_string.hpp>
#include <utest/test_number.hpp>
#include <utest/test_status.hpp>
#include <utest/test_size.hpp>

#include <utility>

namespace utest {
namespace test_message {

class TestAssertProxy {
public:
    const TestString& file() const noexcept;

    TestSize line() const noexcept;

    TestStatus status() const noexcept;
protected:
    TestAssertProxy(const TestAssert& test_assert) noexcept :
        m_test_assert{test_assert}
    { }
private:
    const TestAssert& m_test_assert;
};

inline auto
TestAssertProxy::file() const noexcept -> const TestString& {
    return m_test_assert.file();
}

inline auto
TestAssertProxy::line() const noexcept -> TestSize {
    return m_test_assert.line();
}

inline auto
TestAssertProxy::status() const noexcept -> TestStatus {
    return m_test_assert.status();
}

template<TestMessage::Type T>
class TestAssertMessage : public TestMessage {
protected:
    TestAssertMessage() noexcept :
        TestMessage{T}
    { }
};

class TestAssertValue {
public:
    enum Type {
        NUMBER,
        STRING,
        OBJECT
    };

    Type type() const noexcept {
        return m_type;
    }

    template<typename T>
    const T& get() const noexcept;

    TestAssertValue(const TestNumber& value) noexcept :
        m_type{NUMBER}, m_number{value}
    { }

    TestAssertValue(const TestString& value) noexcept :
        m_type{STRING}, m_string{value}
    { }

    TestAssertValue(const void* value) noexcept :
        m_type{OBJECT}, m_object{value}
    { }
private:
    Type m_type;

    union {
        const TestNumber m_number;
        const TestString m_string;
        const void* m_object;
    };
};

template<> inline auto
TestAssertValue::get<TestString>() const noexcept -> const TestString& {
    return m_string;
}

template<> inline auto
TestAssertValue::get<TestNumber>() const noexcept -> const TestNumber& {
    return m_number;
}

template<> inline auto
TestAssertValue::get<const void*>() const noexcept ->
        const void* const& {
    return m_object;
}

class TestAssertValuePair {
public:
    template<int N>
    const TestAssertValue& get() const noexcept;
protected:
    TestAssertValuePair(const TestAssertValue& lhs,
            const TestAssertValue& rhs) noexcept :
        m_lhs{lhs}, m_rhs{rhs}
    { }
private:
    TestAssertValue m_lhs;
    TestAssertValue m_rhs;
};

template<> inline auto
TestAssertValuePair::get<0>() const noexcept -> const TestAssertValue& {
    return m_lhs;
}

template<> inline auto
TestAssertValuePair::get<1>() const noexcept -> const TestAssertValue& {
    return m_rhs;
}

class TestAssertExplanation :
        public TestAssertMessage<TestMessage::TEST_ASSERT_EXPLANATION>,
        public TestAssertProxy {
public:
    const TestString& explanation() const noexcept {
        return m_message;
    }
private:
    friend utest::TestAssert;

    TestAssertExplanation(const TestAssert& test_assert,
            const TestString& message) noexcept :
        TestAssertProxy{test_assert}, m_message{message}
    { }

    const TestString& m_message;
};

class TestAssertEnd :
        public TestAssertMessage<TestMessage::TEST_ASSERT_END>,
        public TestAssertProxy {
    friend utest::TestAssert;
    using TestAssertProxy::TestAssertProxy;
};

class TestAssertTrue :
        public TestAssertMessage<TestMessage::TEST_ASSERT_TRUE>,
        public TestAssertProxy {
    friend utest::TestAssert;
    using TestAssertProxy::TestAssertProxy;
};

class TestAssertFalse :
        public TestAssertMessage<TestMessage::TEST_ASSERT_FALSE>,
        public TestAssertProxy {
    friend utest::TestAssert;
    using TestAssertProxy::TestAssertProxy;
};

class TestAssertEqual :
        public TestAssertMessage<TestMessage::TEST_ASSERT_EQUAL>,
        public TestAssertProxy,
        public TestAssertValuePair {
    friend class utest::TestAssert;

    TestAssertEqual(const TestAssert& test_assert,
            const TestAssertValue& lhs, const TestAssertValue& rhs) noexcept :
        TestAssertProxy{test_assert}, TestAssertValuePair{lhs, rhs}
    { }
};

class TestAssertNotEqual :
        public TestAssertMessage<TestMessage::TEST_ASSERT_NOT_EQUAL>,
        public TestAssertProxy,
        public TestAssertValuePair {
    friend class utest::TestAssert;

    TestAssertNotEqual(const TestAssert& test_assert,
            const TestAssertValue& lhs, const TestAssertValue& rhs) noexcept :
        TestAssertProxy{test_assert}, TestAssertValuePair{lhs, rhs}
    { }
};

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertEnd& {
    return reinterpret_cast<const TestAssertEnd&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertExplanation& {
    return reinterpret_cast<const TestAssertExplanation&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertTrue& {
    return reinterpret_cast<const TestAssertTrue&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertFalse& {
    return reinterpret_cast<const TestAssertFalse&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertEqual& {
    return reinterpret_cast<const TestAssertEqual&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertNotEqual& {
    return reinterpret_cast<const TestAssertNotEqual&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_ASSERT_HPP */
