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
 * @file utest/test_message/test_case.hpp
 *
 * @brief Test message interface
 */

#ifndef UTEST_TEST_MESSAGE_TEST_CASE_HPP
#define UTEST_TEST_MESSAGE_TEST_CASE_HPP

#include <utest/test_case.hpp>
#include <utest/test_message.hpp>
#include <utest/test_exception.hpp>
#include <utest/test_string.hpp>
#include <utest/test_status.hpp>
#include <utest/test_size.hpp>

namespace utest {
namespace test_message {

class TestCaseProxy {
protected:
    TestCaseProxy(const utest::TestCase& test_case) noexcept :
        m_test_case{test_case}
    { }

    const TestString& name() const noexcept {
        return m_test_case.name();
    }

    const TestString& file() const noexcept {
        return m_test_case.file();
    }

    TestSize line() const noexcept {
        return m_test_case.line();
    }

    TestStatus status() const noexcept {
        return m_test_case.status();
    }
private:
    const utest::TestCase& m_test_case;
};

class TestCaseBegin : public TestMessage, public TestCaseProxy {
public:
    using TestCaseProxy::name;
    using TestCaseProxy::file;
    using TestCaseProxy::line;
private:
    friend class utest::TestCase;

    TestCaseBegin(const utest::TestCase& test_case) noexcept :
        TestMessage{TestMessage::TEST_CASE_BEGIN},
        TestCaseProxy{test_case}
    { }
};

class TestCaseEnd : public TestMessage, public TestCaseProxy {
public:
    using TestCaseProxy::name;
    using TestCaseProxy::file;
    using TestCaseProxy::line;
    using TestCaseProxy::status;
private:
    friend class utest::TestCase;

    TestCaseEnd(const utest::TestCase& test_case) noexcept :
        TestMessage{TestMessage::TEST_CASE_END},
        TestCaseProxy{test_case}
    { }
};

class TestCaseSetup : public TestMessage, public TestCaseProxy {
public:
    using TestCaseProxy::name;
    using TestCaseProxy::file;
    using TestCaseProxy::line;
    using TestCaseProxy::status;
private:
    friend class utest::TestCase;

    TestCaseSetup(const utest::TestCase& test_case) noexcept :
        TestMessage{TestMessage::TEST_CASE_SETUP},
        TestCaseProxy{test_case}
    { }
};

class TestCaseTeardown : public TestMessage, public TestCaseProxy {
public:
    using TestCaseProxy::name;
    using TestCaseProxy::file;
    using TestCaseProxy::line;
    using TestCaseProxy::status;
private:
    friend class utest::TestCase;

    TestCaseTeardown(const utest::TestCase& test_case) noexcept :
        TestMessage{TestMessage::TEST_CASE_TEARDOWN},
        TestCaseProxy{test_case}
    { }
};

class TestCaseException : public TestMessage, public TestException {
private:
    friend class utest::TestCase;

    TestCaseException() noexcept :
        TestMessage{TestMessage::TEST_CASE_EXCEPTION},
        TestException{}
    { }

    TestCaseException(const TestString& message) noexcept :
        TestMessage{TestMessage::TEST_CASE_EXCEPTION},
        TestException{message}
    { }
};

class TestCaseSetupException : public TestMessage, public TestException {
private:
    friend class utest::TestCase;

    TestCaseSetupException() noexcept :
        TestMessage{TestMessage::TEST_CASE_SETUP_EXCEPTION},
        TestException{}
    { }

    TestCaseSetupException(
            const TestString& message) noexcept :
        TestMessage{TestMessage::TEST_CASE_SETUP_EXCEPTION},
        TestException{message}
    { }
};

class TestCaseTeardownException : public TestMessage, public TestException {
private:
    friend class utest::TestCase;

    TestCaseTeardownException() noexcept :
        TestMessage{TestMessage::TEST_CASE_TEARDOWN_EXCEPTION},
        TestException{}
    { }

    TestCaseTeardownException(
            const TestString& message) noexcept :
        TestMessage{TestMessage::TEST_CASE_TEARDOWN_EXCEPTION},
        TestException{message}
    { }
};

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseBegin& {
    return reinterpret_cast<const TestCaseBegin&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseEnd& {
    return reinterpret_cast<const TestCaseEnd&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseSetup& {
    return reinterpret_cast<const TestCaseSetup&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseTeardown& {
    return reinterpret_cast<const TestCaseTeardown&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseException& {
    return reinterpret_cast<const TestCaseException&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseSetupException& {
    return reinterpret_cast<const TestCaseSetupException&>(msg);
}

template<> inline auto
get(const TestMessage& msg) noexcept -> const TestCaseTeardownException& {
    return reinterpret_cast<const TestCaseTeardownException&>(msg);
}

}
}

#endif /* UTEST_TEST_MESSAGE_TEST_CASE_HPP */
