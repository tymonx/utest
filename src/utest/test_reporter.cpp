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
#include <utest/test_writer.hpp>

#include <array>

using utest::TestReporter;

static std::array<utest::TestWriter*, 1> g_default{{
    &utest::TestWriter::get_default()
}};

TestReporter& TestReporter::get_default() noexcept {
    static test_reporter::GoogleTest instance;
    return instance;
}

TestReporter::TestReporter() noexcept :
    m_writers{g_default}
{ }

void TestReporter::write_ex(const TestString& str) noexcept {
    for (auto writer : m_writers) {
        if (writer) {
            writer->write(str);
        }
    }
}

void TestReporter::write_ex(TestValue::Type value) noexcept {
    write_ex(to_string(value));
}

void TestReporter::write_ex(const TestValue& value) noexcept {
    TestString::Buffer buffer;
    write(to_string(value, buffer));
}

void TestReporter::write_ex(std::intmax_t value) noexcept {
    TestString::Buffer buffer;
    write(to_string(value, buffer));
}

void TestReporter::write_ex(std::uintmax_t value) noexcept {
    TestString::Buffer buffer;
    write(to_string(value, buffer));
}

void TestReporter::write_ex(const void* ptr) noexcept {
    TestString::Buffer buffer;
    write(to_string(ptr, buffer));
}

void TestReporter::color(bool enable) noexcept {
    for (auto writer : m_writers) {
        if (writer) {
            writer->color(enable);
        }
    }
}

void TestReporter::color(TestColor c) noexcept {
    for (auto writer : m_writers) {
        if (writer) {
            writer->color(c);
        }
    }
}

TestReporter::~TestReporter() noexcept { }
