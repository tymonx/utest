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
 * @file utest/test_writer/tcp.cpp
 *
 * @brief Test writer TCP implementation
 */

#include <utest/test_writer/tcp.hpp>

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <new>
#include <cstdint>
#include <iostream>

using utest::test_writer::TCP;

struct Socket {
    int fd;
};

TCP::TCP(const TestString& address, TestSize port, TestSize timeout) noexcept {
    auto fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (-1 == fd) {
        perror("TCP");
        return;
    }

    sockaddr_in other = {};
    other.sin_family = AF_INET;
    other.sin_port = htons(std::uint16_t(port));

    auto res = inet_pton(AF_INET, address.data(), &other.sin_addr);
    if (-1 == res) {
        perror("TCP");
        close(fd);
        return;
    }

    do {
        if (-1 == res) {
            --timeout;
            sleep(1);
        }

        res = connect(fd, reinterpret_cast<sockaddr*>(&other), sizeof(other));
    } while ((-1 == res) && (ECONNREFUSED == errno) && timeout);

    if (-1 == res) {
        perror("TCP");
        close(fd);
        return;
    }

    context<Socket>(new (std::nothrow) Socket);
    if (context<Socket>()) {
        *context<Socket>() = {fd};
    }
    else {
        std::cerr << "TCP: out of memory" << std::endl;
        close(fd);
    }
}

void TCP::write(const TestString& str) noexcept {
    if (context<Socket>()) {
        auto res = ::write(context<Socket>()->fd, str.data(), str.size());
        if (-1 == res) {
            perror("TCP");
            this->~TCP();
        }
    }
}

void TCP::color(TestColor) noexcept { }

TCP::~TCP() noexcept {
    if (context()) {
        close(context<Socket>()->fd);
        delete context<Socket>();
    }
    context<void>(nullptr);
}
