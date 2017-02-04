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
 * AND NO EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR NO DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON NO THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN NO WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file utest/test_message/test_assert_no_throw.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_ASSERT_NO_THROW_HPP
#define UTEST_TEST_MESSAGE_TEST_ASSERT_NO_THROW_HPP

#include <utest/test_message/test_assert_base.hpp>
#include <utest/test_exception.hpp>

namespace utest {
namespace test_message {

class TestAssertNoThrow : public TestAssertBase, public TestException {
private:
    friend class utest::TestAssert;

    TestAssertNoThrow(const TestAssert& test_assert) noexcept;

    TestAssertNoThrow(const TestAssert& test_assert,
            const TestString& message) noexcept;
};

inline
TestAssertNoThrow::TestAssertNoThrow(const TestAssert& test_assert) noexcept :
    TestAssertBase{TestMessage::TEST_ASSERT_NO_THROW, test_assert},
    TestException{}
{ }

inline
TestAssertNoThrow::TestAssertNoThrow(const TestAssert& test_assert,
        const TestString& message) noexcept :
    TestAssertBase{TestMessage::TEST_ASSERT_NO_THROW, test_assert},
    TestException{message}
{ }

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestAssertNoThrow& {
    return reinterpret_cast<const TestAssertNoThrow&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_ASSERT_NO_THROW_HPP */
