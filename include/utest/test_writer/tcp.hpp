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
 * @file utest/test_writer/tcp.hpp
 *
 * @brief Test writer interface
 */

#ifndef UTEST_TEST_WRITER_TCP_HPP
#define UTEST_TEST_WRITER_TCP_HPP

#include <utest/test_writer.hpp>

namespace utest {
namespace test_writer {

class TCP final : public TestWriter {
public:
    static constexpr TestString DEFAULT_ADDRESS{"127.0.0.1"};
    static constexpr TestSize   DEFAULT_PORT{8080};
    static constexpr TestSize   DEFAULT_TIMEOUT{0};

    TCP(TCP&& other) noexcept;

    TCP& operator=(TCP&& other) noexcept;

    TCP(TestSize port, TestSize timeout = DEFAULT_TIMEOUT) noexcept;

    TCP(const TestString& address = DEFAULT_ADDRESS,
            TestSize port = DEFAULT_PORT,
            TestSize timeout = DEFAULT_TIMEOUT) noexcept;

    virtual void write(const TestString& str) noexcept override;

    virtual void color(TestColor c) noexcept override;

    virtual ~TCP() noexcept override;
private:
    TCP(const TCP&) = delete;
    TCP& operator=(const TCP&) = delete;

    template<typename T = void>
    void context(T* ptr) noexcept;

    template<typename T = void>
    T* context() noexcept;

    void* m_context{nullptr};
};

inline
TCP::TCP(TCP&& other) noexcept :
    m_context{other.context()}
{
    other.context<void>(nullptr);
}

inline auto
TCP::operator=(TCP&& other) noexcept -> TCP& {
    if (this != &other) {
        context(other.context());
        other.context<void>(nullptr);
    }
    return *this;
}

inline
TCP::TCP(TestSize port, TestSize timeout) noexcept :
    TCP{DEFAULT_ADDRESS, port, timeout}
{ }

template<typename T> inline void
TCP::context(T* ptr) noexcept {
    m_context = static_cast<void*>(ptr);
}

template<typename T> inline auto
TCP::context() noexcept -> T* {
    return static_cast<T*>(m_context);
}

}
}

#endif /* UTEST_TEST_WRITER_TCP_HPP */
