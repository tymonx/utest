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
 * @file utest/test_runner.cpp
 *
 * @brief Test runner implementation
 */

#include <utest/test_runner.hpp>
#include <utest/test_suite.hpp>
#include <utest/test_exception.hpp>

#include <utest/test_message/test_runner.hpp>

#include "test_register.hpp"

using utest::TestRunner;

TestRunner::TestRunner(Run run) noexcept :
        m_run{run} {
    TestRegister::get_instance().add(this);
}

auto TestRunner::operator()(TestSuite& test_suite) noexcept -> TestStatus {
    auto status{TestStatus::PASS};

    if (m_run) {
#if defined(UTEST_USE_EXCEPTIONS)
        try {
            m_run(test_suite);
            status = test_suite.status();
        }
        catch (const std::exception& e) {
            status = TestStatus::FAIL;
            test_suite.report(test_message::TestRunnerException{
                {e.what(), TestString::length(e.what())}
            });
        }
        catch (...) {
            status = TestStatus::FAIL;
            test_suite.report(test_message::TestRunnerException{});
        }
#else
        m_run(test_suite);
        status = test_suite.status();
#endif
    }

    return status;
}
