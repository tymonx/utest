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
 * @file utest/test_writer/udp.hpp
 *
 * @brief Test writer interface
 */

#ifndef UTEST_TEST_WRITER_UDP_HPP
#define UTEST_TEST_WRITER_UDP_HPP

#include <utest/test_writer.hpp>

namespace utest {
namespace test_writer {

class UDP final : public TestWriter {
public:
    using TestWriter::color;

    static constexpr TestString DEFAULT_ADDRESS{"127.0.0.1"};
    static constexpr TestSize   DEFAULT_PORT{8080};

    UDP(UDP&& other) noexcept;

    UDP& operator=(UDP&& other) noexcept;

    UDP(TestSize port) noexcept;

    UDP(const TestString& address = DEFAULT_ADDRESS,
            TestSize port = DEFAULT_PORT) noexcept;

    virtual ~UDP() noexcept override;
private:
    UDP(const UDP&) = delete;
    UDP& operator=(const UDP&) = delete;

    virtual void write(const TestString& str) noexcept override;

    virtual void color(TestColor c) noexcept override;

    template<typename T = void>
    void context(T* ptr) noexcept;

    template<typename T = void>
    T* context() noexcept;

    void* m_context{nullptr};
};

inline
UDP::UDP(UDP&& other) noexcept :
    m_context{other.context()}
{
    other.context<void>(nullptr);
}

inline auto
UDP::operator=(UDP&& other) noexcept -> UDP& {
    if (this != &other) {
        context(other.context());
        other.context<void>(nullptr);
    }
    return *this;
}

inline
UDP::UDP(TestSize port) noexcept :
    UDP{DEFAULT_ADDRESS, port}
{ }

template<typename T> inline void
UDP::context(T* ptr) noexcept {
    m_context = static_cast<void*>(ptr);
}

template<typename T> inline auto
UDP::context() noexcept -> T* {
    return static_cast<T*>(m_context);
}

}
}

#endif /* UTEST_TEST_WRITER_UDP_HPP */
