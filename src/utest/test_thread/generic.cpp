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
 * @file utest/test_thread/generic.cpp
 *
 * @brief Test thread implementation
 */

#include <utest/test_thread/generic.hpp>
#include <utest/test_exception.hpp>
#include <utest/test_case.hpp>

#if defined(UTEST_USE_THREADS)
#include <thread>
#include <functional>
#endif

using utest::test_thread::Generic;

#if defined(UTEST_USE_THREADS) && defined(UTEST_USE_EXCEPTIONS)

void Generic::run(TestCase& test_case, TestCaseRun test_run,
        TestParams& test_params) noexcept {
    try {
        std::thread test(test_run, std::ref(test_case), std::ref(test_params));
        try {
            test.join();
        } catch (...) { }
    } catch (...) {
        (test_case.*test_run)(test_params);
    }
}

#elif defined(UTEST_USE_THREADS)

void Generic::run(TestCase& test_case, TestCaseRun test_run,
        TestParams& test_params) noexcept {
    std::thread test(test_run, std::ref(test_case), std::ref(test_params));
    test.join();
}

#else

void Generic::run(TestCase& test_case, TestCaseRun test_run,
        TestParams& test_params) noexcept {
    (test_case.*test_run)(test_params);
}

#endif

Generic::~Generic() noexcept { }
