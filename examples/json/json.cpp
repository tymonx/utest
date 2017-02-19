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
 * @file examples/json.cpp
 *
 * @brief Main implementation
 */

#include <utest/utest.hpp>

#include <utest/test_writer/file.hpp>
#include <utest/test_reporter/json.hpp>

#include <cstdlib>

using namespace utest;

int main() {
    test_writer::File out{stdout};
    test_writer::File json_file[2]{
        {"pretty.json"},
        {"compact.json"}
    };

    TestWriterReference writer_1[]{
        out,
        json_file[0]
    };

    TestWriterReference writer_2[]{
        out,
        json_file[0]
    };


    test_reporter::JSON json[2]{
        {writer_1},
        {writer_2}
    };

    json[1].compact();

    TestReporterReference reporters[]{
        json[0],
        json[1]
    };

    return TestStatus::PASS == Test{reporters}.run().status()
        ? EXIT_SUCCESS : EXIT_FAILURE;
}

static TestRunner g([] (TestSuite& test_suite) {
    test_suite.file(__FILE__)

    .name("positive").run([] (TestCase& test_case) {
        test_case.fatal(false)
        .name("boolean").run([] (TestParams& p) {
            TestAssert{p}.is_true(true);
            TestAssert{p}.is_false(false);
        })
        .name("compare").run([] (TestParams& p) {
            TestAssert{p}.equal(1, 1);
            TestAssert{p}.not_equal(3, 4);
            TestAssert{p}.greater_than(7, 6);
            TestAssert{p}.greater_than_or_equal(4, 4);
            TestAssert{p}.less_than(3, 5);
            TestAssert{p}.less_than_or_equal(4, 4);
        })
        .name("string").run([] (TestParams& p) {
            TestAssert{p}.equal("Test", "Test");
            TestAssert{p}.not_equal("Test", "TestX");
        });
    })
    .name("negative").run([] (TestCase& test_case) {
        test_case.fatal(false)
        .name("boolean").run([] (TestParams& p) {
            TestAssert{p}.is_true(false);
            TestAssert{p}.is_false(true);
        })
        .name("compare").run([] (TestParams& p) {
            TestAssert{p}.equal(1, 2);
            TestAssert{p}.not_equal(3, 3);
            TestAssert{p}.greater_than(6, 6);
            TestAssert{p}.greater_than_or_equal(4, 7);
            TestAssert{p}.less_than(5, 5);
            TestAssert{p}.less_than_or_equal(4, 3);
        })
        .name("string").run([] (TestParams& p) {
            TestAssert{p}.equal("Test", "TestX");
            TestAssert{p}.not_equal("Test", "Test");
        });
    });
});
