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
 * @file utest/test_message.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_HPP
#define UTEST_TEST_MESSAGE_HPP

namespace utest {

class TestMessage {
public:
    enum Type {
        TEST_BEGIN,
        TEST_SUITE_BEGIN,
        TEST_CASE_BEGIN,
        TEST_CASE_SETUP,
        TEST_ASSERT_EXPLANATION,
        TEST_ASSERT_EXPLANATION_END,
        TEST_ASSERT_TRUE,
        TEST_ASSERT_FALSE,
        TEST_ASSERT_EQUAL,
        TEST_ASSERT_NOT_EQUAL,
        TEST_CASE_TEARDOWN,
        TEST_CASE_END,
        TEST_SUITE_END,
        TEST_END,
        TEST_RUNNER_EXCEPTION,
        TEST_SUITE_EXCEPTION,
        TEST_CASE_SETUP_EXCEPTION,
        TEST_CASE_EXCEPTION,
        TEST_CASE_TEARDOWN_EXCEPTION
    };

    Type type() const noexcept;
protected:
    TestMessage(Type type) noexcept;
private:
    Type m_type;
};

inline
TestMessage::TestMessage(Type type) noexcept :
    m_type{type}
{ }

inline auto
TestMessage::type() const noexcept -> Type {
    return m_type;
}

namespace test_message {

template<typename U>
const U& get(const TestMessage& test_message) noexcept;

}

}

#endif /* UTEST_TEST_MESSAGE_HPP */
