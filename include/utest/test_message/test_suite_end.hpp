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
 * @file utest/test_message/test_suite_end.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_SUITE_END_HPP
#define UTEST_TEST_MESSAGE_TEST_SUITE_END_HPP

#include <utest/test_message/test_suite_base.hpp>

namespace utest {
namespace test_message {

class TestSuiteEnd : protected TestSuiteBase {
public:
    using TestSuiteBase::name;
    using TestSuiteBase::file;
    using TestSuiteBase::line;
    using TestSuiteBase::status;

    TestSize tests_passed() const noexcept;

    TestSize tests_failed() const noexcept;
private:
    friend class utest::TestSuite;

    TestSuiteEnd(const utest::TestSuite& test_suite,
            TestSize passed, TestSize failed) noexcept;

    TestSize m_tests_passed{};
    TestSize m_tests_failed{};
};

inline
TestSuiteEnd::TestSuiteEnd(const utest::TestSuite& test_suite,
        TestSize passed, TestSize failed) noexcept :
    TestSuiteBase{TestMessage::TEST_SUITE_END, test_suite},
    m_tests_passed{passed}, m_tests_failed{failed}
{ }

inline auto
TestSuiteEnd::tests_passed() const noexcept -> TestSize {
    return m_tests_passed;
}

inline auto
TestSuiteEnd::tests_failed() const noexcept -> TestSize {
    return m_tests_failed;
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestSuiteEnd& {
    return reinterpret_cast<const TestSuiteEnd&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_SUITE_END_HPP */
