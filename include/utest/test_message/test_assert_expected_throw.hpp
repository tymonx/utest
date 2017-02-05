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
 * AND EXPECTED EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR EXPECTED DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON EXPECTED THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN EXPECTED WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file utest/test_message/test_assert_expected_throw.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_ASSERT_EXPECTED_THROW_HPP
#define UTEST_TEST_MESSAGE_TEST_ASSERT_EXPECTED_THROW_HPP

#include <utest/test_message/test_assert_base.hpp>
#include <utest/test_exception.hpp>

namespace utest {
namespace test_message {

class TestAssertExpectedThrow : public TestAssertBase, public TestException {
public:
    bool throws() const noexcept;
private:
    friend class utest::TestAssert;

    TestAssertExpectedThrow(const TestAssert& test_assert,
            bool exception_throws, const TestString& str) noexcept;

    bool m_throws{false};
};

inline
TestAssertExpectedThrow::TestAssertExpectedThrow(
        const TestAssert& test_assert, bool exception_throws,
        const TestString& str) noexcept :
    TestAssertBase{TestMessage::TEST_ASSERT_EXPECTED_THROW, test_assert},
    TestException{str},
    m_throws{exception_throws}
{ }

inline auto
TestAssertExpectedThrow::throws() const noexcept -> bool {
    return m_throws;
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertExpectedThrow& {
    return reinterpret_cast<const TestAssertExpectedThrow&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_ASSERT_EXPECTED_THROW_HPP */
