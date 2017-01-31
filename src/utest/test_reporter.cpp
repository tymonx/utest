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
 * @file utest/test_reporter.cpp
 *
 * @brief Test repoter implementation
 */

#include <utest/test_reporter.hpp>
#include <utest/test_reporter/google_test.hpp>
#include <utest/test_writter.hpp>
#include <utest/test_number.hpp>

using utest::TestReporter;

static utest::TestWritter* g_default[1] {&utest::TestWritter::get_default()};

TestReporter& TestReporter::get_default() noexcept {
    static test_reporter::GoogleTest instance;
    return instance;
}

TestReporter::TestReporter() noexcept :
    m_writters{g_default}
{ }

void TestReporter::write(const TestString& str) noexcept {
    for (auto writter : m_writters) {
        if (writter) {
            writter->write(str);
        }
    }
}

void TestReporter::write(const TestNumber& number) noexcept {
    char buffer[TestNumber::MAX_STRING_BUFFER];
    write(to_string(number, buffer));
}

void TestReporter::color(TestColor c) noexcept {
    for (auto writter : m_writters) {
        if (writter) {
            writter->color(c);
        }
    }
}

TestReporter::~TestReporter() noexcept { }
