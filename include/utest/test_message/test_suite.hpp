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
 * @file utest/test_message/test_suite.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_SUITE_HPP
#define UTEST_TEST_MESSAGE_TEST_SUITE_HPP

#include <utest/test_suite.hpp>
#include <utest/test_message.hpp>
#include <utest/test_exception.hpp>
#include <utest/test_string.hpp>
#include <utest/test_status.hpp>
#include <utest/test_size.hpp>

namespace utest {
namespace test_message {

class TestSuiteProxy {
protected:
    TestSuiteProxy(const utest::TestSuite& test_suite) noexcept :
        m_test_suite{test_suite}
    { }

    const TestString& name() const noexcept {
        return m_test_suite.name();
    }

    const TestString& file() const noexcept {
        return m_test_suite.file();
    }

    TestSize line() const noexcept {
        return m_test_suite.line();
    }

    TestStatus status() const noexcept {
        return m_test_suite.status();
    }
private:
    const utest::TestSuite& m_test_suite;
};

class TestSuiteBegin : public TestMessage, public TestSuiteProxy {
public:
    using TestSuiteProxy::name;
    using TestSuiteProxy::file;
    using TestSuiteProxy::line;
private:
    friend class utest::TestSuite;

    TestSuiteBegin(const utest::TestSuite& test_suite) noexcept :
        TestMessage{TestMessage::TEST_SUITE_BEGIN},
        TestSuiteProxy{test_suite}
    { }
};

class TestSuiteEnd : public TestMessage, public TestSuiteProxy {
public:
    using TestSuiteProxy::name;
    using TestSuiteProxy::file;
    using TestSuiteProxy::line;
    using TestSuiteProxy::status;

    TestSize tests_passed() const noexcept {
        return m_passed;
    }

    TestSize tests_failed() const noexcept {
        return m_failed;
    }
private:
    friend class utest::TestSuite;

    TestSuiteEnd(const utest::TestSuite& test_suite,
            TestSize passed, TestSize failed) noexcept :
        TestMessage{TestMessage::TEST_SUITE_END},
        TestSuiteProxy{test_suite},
        m_passed{passed},
        m_failed{failed}
    { }

    TestSize m_passed{};
    TestSize m_failed{};
};

class TestSuiteException : public TestMessage, public TestException {
private:
    friend class utest::TestSuite;

    TestSuiteException() noexcept :
        TestMessage{TestMessage::TEST_SUITE_EXCEPTION},
        TestException{}
    { }

    TestSuiteException(const TestString& message) noexcept :
        TestMessage{TestMessage::TEST_SUITE_EXCEPTION},
        TestException{message}
    { }
};

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestSuiteBegin& {
    return reinterpret_cast<const TestSuiteBegin&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestSuiteEnd& {
    return reinterpret_cast<const TestSuiteEnd&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestSuiteException& {
    return reinterpret_cast<const TestSuiteException&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_SUITE_HPP */
