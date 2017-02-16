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
 * @file utest/test_writer/udp.cpp
 *
 * @brief Test writer UDP implementation
 */

#include <utest/test_writer/udp.hpp>

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <new>
#include <cstdint>
#include <iostream>

using utest::TestString;
using utest::test_writer::UDP;

constexpr TestString UDP::DEFAULT_ADDRESS;

struct Socket {
    int fd;
    sockaddr_in other;
};

UDP::UDP(const TestString& address, TestSize port) noexcept {
    auto fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == fd) {
        perror("UDP");
        return;
    }

    sockaddr_in other = {};
    other.sin_family = AF_INET;
    other.sin_port = htons(std::uint16_t(port));

    auto res = inet_pton(AF_INET, address.data(), &other.sin_addr);
    if (-1 == res) {
        perror("UDP");
        ::close(fd);
        return;
    }
    else if (!res) {
        std::cerr << "UDP: invalid IP address " << address.data() << std::endl;
        ::close(fd);
        return;
    }

    context<Socket>(new (std::nothrow) Socket);
    if (context<Socket>()) {
        *context<Socket>() = {fd, other};
    }
    else {
        std::cerr << "UDP: out of memory" << std::endl;
        ::close(fd);
    }
}

void UDP::write(const TestString& str) noexcept {
    if (context<Socket>()) {
        auto res = sendto(context<Socket>()->fd, str.data(), str.size(), 0,
                reinterpret_cast<sockaddr*>(&context<Socket>()->other),
                sizeof(sockaddr_in));
        if (-1 == res) {
            perror("UDP");
            close();
        }
    }
}

void UDP::color(TestColor c) noexcept {
    if (TestWriter::color()) {
        write(ansi_escape_code(c));
    }
}

void UDP::close() noexcept {
    if (context()) {
        ::close(context<Socket>()->fd);
        delete context<Socket>();
    }
    context<void>(nullptr);
}

UDP::~UDP() noexcept {
    close();
}
