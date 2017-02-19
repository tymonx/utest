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
 * @file utest/test_command_line.hpp
 *
 * @brief Test command line interface
 */

#ifndef UTEST_TEST_COMMAND_LINE_HPP
#define UTEST_TEST_COMMAND_LINE_HPP

#include <utest/test_string.hpp>
#include <utest/test_utilities.hpp>

namespace utest {

class Test;
class TestWriter;

class TestCommandLine {
public:
    using Arguments = TestSpan<char*>;

    TestCommandLine() noexcept;

    TestCommandLine(const TestCommandLine& other) noexcept = default;

    TestCommandLine(TestCommandLine&& other) noexcept = default;

    TestCommandLine& operator=(const TestCommandLine& other) noexcept = default;

    TestCommandLine& operator=(TestCommandLine&& other) noexcept = default;

    TestCommandLine(int argc, char* argv[]) noexcept;

    TestCommandLine(int argc, char* argv[], TestWriter& writer) noexcept;

    TestCommandLine(const Arguments& arguments) noexcept;

    TestCommandLine(const Arguments& arguments, TestWriter& writer) noexcept;

    bool valid() const noexcept;
private:
    friend class Test;

    enum class Status {
        OK,
        NOT_FOUND,
        END
    };

    using Handler = Status (TestCommandLine::*)(const Arguments&);

    void write(const TestString& str) noexcept;

    template<typename... Args>
    void write(const TestString& str, const Args&... args) noexcept;

    void write() noexcept;

    void parsing() noexcept;

    static TestSize get_arguments_length(const Arguments& arguments) noexcept;

    Status command_help(const Arguments& arguments) noexcept;
    Status command_version(const Arguments& arguments) noexcept;

    struct Command {
        Handler handler;
        TestString long_name;
        TestString short_name;
        TestString explanation;
    };

    static const Command g_commands[];

    TestWriterReference m_writer;
    Arguments m_arguments{};
    bool m_no_valid{false};
};

inline
TestCommandLine::TestCommandLine(int argc, char* argv[]) noexcept :
    TestCommandLine{{argv, TestSize(argc)}}
{ }

inline
TestCommandLine::TestCommandLine(int argc, char* argv[],
        TestWriter& writer) noexcept :
    m_writer{writer}, m_arguments{argv, TestSize(argc)}
{
    parsing();
}

inline
TestCommandLine::TestCommandLine(const Arguments& arguments,
        TestWriter& writer) noexcept :
    m_writer{writer}, m_arguments{arguments}
{
    parsing();
}

template<typename... Args> void
TestCommandLine::write(const TestString& str, const Args&... args) noexcept {
    write(str);
    write(args...);
}

inline void
TestCommandLine::write() noexcept { }

inline bool
TestCommandLine::valid() const noexcept {
    return !m_no_valid;
}

}

#endif /* UTEST_TEST_COMMAND_LINE_HPP */
