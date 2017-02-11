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
 * @file utest/test_writer/file.cpp
 *
 * @brief Test writer file implementation
 */

#include <utest/test_writer/file.hpp>

using utest::test_writer::File;

void File::write(const TestString& str) noexcept {
    if (m_file) {
        std::fwrite(str.data(), sizeof(TestString::value_type),
                str.length(), m_file);
    }
}

void File::color(TestColor c) noexcept {
    if (TestWriter::color()) {
        switch (c) {
        case TestColor::BLACK:
            write("\x1B[30m");
            break;
        case TestColor::RED:
            write("\x1B[31m");
            break;
        case TestColor::GREEN:
            write("\x1B[32m");
            break;
        case TestColor::YELLOW:
            write("\x1B[33m");
            break;
        case TestColor::BLUE:
            write("\x1B[34m");
            break;
        case TestColor::MAGENTA:
            write("\x1B[35m");
            break;
        case TestColor::CYAN:
            write("\x1B[36m");
            break;
        case TestColor::WHITE:
            write("\x1B[37m");
            break;
        case TestColor::DEFAULT:
            write("\x1B[39m");
            break;
        default:
            break;
        }
    }
}

File::~File() noexcept {
    if (m_file && m_open) {
        std::fclose(m_file);
    }
    m_file = nullptr;
}
