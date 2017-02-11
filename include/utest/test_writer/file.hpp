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
 * @file utest/test_writer/file.hpp
 *
 * @brief Test writer interface
 */

#ifndef UTEST_TEST_WRITER_FILE_HPP
#define UTEST_TEST_WRITER_FILE_HPP

#include <utest/test_writer.hpp>

#include <cstdio>

namespace utest {
namespace test_writer {

class File final : public TestWriter {
public:
    enum Mode {
        WRITE,
        APPEND
    };

    File(File&&) noexcept;

    File& operator=(File&&) noexcept;

    File(FILE* file = stdout) noexcept;

    File(const TestString& file, Mode mode = WRITE) noexcept;

    virtual void write(const TestString& str) noexcept override;

    virtual void color(TestColor c) noexcept override;

    virtual ~File() noexcept override;
private:
    File(const File&) = delete;

    File& operator=(const File&) = delete;

    FILE* m_file{nullptr};
    bool m_open{false};
};

inline
File::File(File&& other) noexcept :
    m_file{other.m_file},
    m_open{other.m_open}
{
    other.m_file = nullptr;
}

inline auto
File::operator=(File&& other) noexcept -> File& {
    if (this != &other) {
        m_file = other.m_file;
        m_open = other.m_open;
        other.m_file = nullptr;
    }
    return *this;
}

inline
File::File(FILE* file) noexcept :
    m_file{file}
{ }

inline
File::File(const TestString& file, Mode mode) noexcept :
    m_open{true}
{
    switch (mode) {
    case WRITE:
        m_file = std::fopen(file.data(), "w");
        break;
    case APPEND:
        m_file = std::fopen(file.data(), "a");
        break;
    default:
        break;
    }
}

}
}

#endif /* UTEST_TEST_WRITER_FILE_HPP */
