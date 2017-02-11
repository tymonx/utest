# µTest

[![Join the chat at https://gitter.im/utest-embedded/Lobby](https://badges.gitter.im/utest-embedded/Lobby.svg)](https://gitter.im/utest-embedded/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![License (3-Clause BSD)](https://img.shields.io/badge/license-BSD%203--Clause-blue.svg?style=flat)](http://opensource.org/licenses/BSD-3-Clause)
[![Build Status](https://travis-ci.org/tymonx/utest.svg?branch=master)](https://travis-ci.org/tymonx/utest)

Lightweight unit testing framework mainly for C++ and also for C projects. Suitable for embedded devices.

## Key Features

* Written in C++11
* An XUnit test framework
* The 3-clause BSD license
* No evil defined test macros, programmer API style
* Automatic test registration
* Single registered test can have multi test suites and test cases
* A rich set of assertions
* Fatal and non-fatal assertions
* Support for multi reporters with multi writers
* Working with/without exceptions and RTTI
* Optionally running test case in isolate environment using thread
* Minimal virtual methods usage
* Small footprint, friendly for embedded target like Cortex-Mx microcontrollers
* Warnings free even on the most aggressive levels
* Support for Clang 3.5 and later
* Support for GCC 4.8 and later

## How to use it

* [Build under Linux](docs/build-linux.md)
* [Build for ARM target](docs/build-arm.md)

## Examples

Test source example:

```c++
#include <utest/utest.hpp>

using namespace utest;

static unsigned int Factorial(unsigned int number) {
    return number > 1 ? Factorial(number - 1) * number : 1;
}

static TestRunner g([] (TestSuite& test_suite) {
    test_suite.name("Factorial").run([] (TestCase& test_case) {
        test_case.name("computed").run([] (TestParams& p) {
            TestAssert{p}.equal(Factorial(0), 1u);
            TestAssert{p}.equal(Factorial(1), 1u);
            TestAssert{p}.equal(Factorial(2), 2u);
            TestAssert{p}.equal(Factorial(3), 6u);
            TestAssert{p}.equal(Factorial(10), 3628800u);
        });
    });
});
 ```

Test output example:

```
[==========] Running 1 registered test runner(s).
[----------] Running test(s) from Factorial
[ RUN      ] computed
[       OK ] computed
[----------] 1 test(s) from Factorial
[  PASSED  ] 1 test(s).

[==========] 1 test(s) from 1 test suite(s) ran.
[  PASSED  ] 1 test(s).
```

Test example with evil macros extension:

```
#include <utest/utest.hpp>
#include <utest/test_extension/evil_defines.hpp>

UTEST_RUNNER()
    UTEST_SUITE("integer compare")
        int x = 0;

        UTEST_CASE_CONTEXT(x);

        UTEST_CASE_SETUP()
            UTEST_CONTEXT(int) = 5;
        UTEST_CASE_SETUP_END

        UTEST_CASE("equal")
            UTEST_EXPECT_EQ(UTEST_CONTEXT(int), 5);
        UTEST_CASE_END

        UTEST_CASE_SETUP()
            UTEST_CONTEXT(int) = 4;
        UTEST_CASE_SETUP_END

        UTEST_CASE("not equal")
            UTEST_EXPECT_NE(UTEST_CONTEXT(int), 5);
        UTEST_CASE_END
    UTEST_SUITE_END
UTEST_RUNNER_END
 ```
