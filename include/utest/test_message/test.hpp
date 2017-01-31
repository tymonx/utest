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
 * @file utest/test_message/test.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_HPP
#define UTEST_TEST_MESSAGE_TEST_HPP

#include <utest/test_message.hpp>
#include <utest/test_size.hpp>

namespace utest {

class Test;

namespace test_message {

class TestBegin : public TestMessage {
public:
    TestSize tests_registered() const noexcept {
        return m_tests_registered;
    }
private:
    friend class utest::Test;

    TestBegin(TestSize registered) noexcept :
        TestMessage{TestMessage::TEST_BEGIN},
        m_tests_registered{registered}
    { }

    TestSize m_tests_registered{0};
};

class TestEnd : public TestMessage {
public:
    TestSize test_suites_passed() const noexcept {
        return m_test_suites_passed;
    }

    TestSize test_suites_failed() const noexcept {
        return m_test_suites_failed;
    }

    TestSize test_cases_passed() const noexcept {
        return m_test_cases_passed;
    }

    TestSize test_cases_failed() const noexcept {
        return m_test_cases_failed;
    }

    TestSize test_suites() const noexcept {
        return m_test_suites_passed + m_test_suites_failed;
    }

    TestSize test_cases() const noexcept {
        return m_test_cases_passed + m_test_cases_failed;
    }
private:
    friend class utest::Test;

    TestEnd(TestSize suites_passed, TestSize suites_failed,
            TestSize cases_passed, TestSize cases_failed) noexcept :
        TestMessage{TestMessage::TEST_END},
        m_test_suites_passed{suites_passed},
        m_test_suites_failed{suites_failed},
        m_test_cases_passed{cases_passed},
        m_test_cases_failed{cases_failed}
    { }

    TestSize m_test_suites_passed{0};
    TestSize m_test_suites_failed{0};
    TestSize m_test_cases_passed{0};
    TestSize m_test_cases_failed{0};
};

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestBegin& {
    return reinterpret_cast<const TestBegin&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestEnd& {
    return reinterpret_cast<const TestEnd&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_HPP */
