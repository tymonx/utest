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
 * @file tests/tcp.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>
#include <utest/test_writer/tcp.hpp>
#include <utest/test_writer/file.hpp>
#include <utest/test_reporter/google_test.hpp>

#include <memory>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace utest;

static int g_socket_code{-1};
static int g_inet_pton_code{-1};
static int g_connect_code{-1};
static ssize_t g_write_code{-1};
static int g_close_code{-1};
static bool g_new{false};

int socket(int,int,int) {
    return g_socket_code;
}

int inet_pton(int, const char*, void*) {
    return g_inet_pton_code;
}

int connect(int, const struct sockaddr*, socklen_t) {
    return g_connect_code;
}

ssize_t write(int, const void*, size_t) {
    return g_write_code;
}

int close(int) {
    return g_close_code;
}

void* operator new(std::size_t count, const std::nothrow_t&) noexcept {
    return g_new ? std::malloc(count) : nullptr;
}

void operator delete(void* ptr) noexcept {
    std::free(ptr);
}

int main() {
    {
        test_writer::TCP tcp{"127.0.0.1", 51234};
    }
    {
        g_socket_code = 0;
        g_inet_pton_code = -1;
        test_writer::TCP tcp{"127.0.0.1", 51234};
    }
    {
        g_inet_pton_code = 0;
        test_writer::TCP tcp{"127.0.0.1", 51234};
    }
    {
        g_inet_pton_code = 1;
        errno = ECONNREFUSED;
        test_writer::TCP tcp{"127.0.0.1", 51234, 1};
    }
    {
        g_connect_code = 0;
        errno = 0;
        test_writer::TCP tcp{"127.0.0.1", 51234};
    }
    {
        g_new = true;
        test_writer::TCP tcp{"127.0.0.1", 51234};
    }
    {
        test_writer::TCP tcp{"127.0.0.1"};
        TestWriter* writers[]{&tcp};

        test_reporter::GoogleTest google_test{writers};
        TestReporter* reporters[]{&google_test};

        Test{reporters}.color().run();
    }
    {
        std::unique_ptr<TestWriter> p{new test_writer::TCP{}};
    }

    return EXIT_SUCCESS;
}

static TestRunner g([] (TestSuite&) { });
