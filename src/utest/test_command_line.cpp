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
 * @file utest/test_command_line.cpp
 *
 * @brief Test command line implementation
 */

#include <utest/test_command_line.hpp>
#include <utest/test_writer.hpp>

using utest::TestSize;
using utest::TestCommandLine;

static constexpr TestSize EXPLANATION_INDENT{28};

const TestCommandLine::Command TestCommandLine::g_commands[]{
    {
        &TestCommandLine::command_help, "--help", "-h",
        "Display this information"
    },
    {
        &TestCommandLine::command_version, "--version", "-v",
        "Display µTest version information"
    }
};

TestCommandLine::TestCommandLine(const Arguments& arguments) noexcept :
    m_writer{&TestWriter::get_default()},
    m_arguments{arguments}
{
    parsing();
}

void TestCommandLine::write(const TestString& str) noexcept {
    m_writer->write(str);
}

TestSize TestCommandLine::get_arguments_length(
        const Arguments& arguments) noexcept {
    TestSize index{0};

    for (const auto& arg : arguments) {
        if (!arg || ('-' == arg[0]))  {
            break;
        }
        ++index;
    }

    return index;
}

void TestCommandLine::parsing() noexcept {
    auto status = Status::OK;

    for (TestSize i = 1; i < m_arguments.size(); ++i) {
        const auto& arg = m_arguments[i];

        status = Status::NOT_FOUND;
        for (const auto& cmd : g_commands) {
            if ((arg == cmd.short_name) || (arg == cmd.long_name)) {
                auto len = get_arguments_length(m_arguments.subspan(i + 1));
                status = (*this.*cmd.handler)(m_arguments.subspan(i + 1, len));
                i += len;
                break;
            }
        }
        if (Status::OK != status) { break; }
    }

    switch (status) {
    case Status::OK:
    case Status::END:
        break;
    case Status::NOT_FOUND:
    default:
        command_help({});
        break;
    }

    m_no_valid = (Status::OK != status);
}

auto TestCommandLine::command_help(const Arguments&) noexcept -> Status {
    write("Usage: ", m_arguments[0], " [options]\nOptions:\n");

    for (const auto& cmd : g_commands) {
        auto length = 2 + cmd.short_name.length() + cmd.long_name.length();

        write("  ", cmd.short_name);
        if (cmd.short_name && cmd.long_name) {
            length += 2;
            write(", ");
        }
        write(cmd.long_name);

        for (auto i = length; i < EXPLANATION_INDENT; ++i) {
            write(" ");
        }

        write(cmd.explanation, ".\n");
    }

    return Status::END;
}

auto TestCommandLine::command_version(const Arguments&) noexcept -> Status {
    write(R"(µTest 0.1.0
Copyright (C) 2017, Tymoteusz Blazejczyk
License BSD 3-Clause <https://opensource.org/licenses/BSD-3-Clause>.
)");

    return Status::END;
}
