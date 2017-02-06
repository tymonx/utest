# µTest

[![License (3-Clause BSD)](https://img.shields.io/badge/license-BSD%203--Clause-blue.svg?style=flat-square)](http://opensource.org/licenses/BSD-3-Clause)
[![Build Status](https://travis-ci.org/tymonx/utest.svg?branch=master)](https://travis-ci.org/tymonx/utest)

Lightweight unit tests framework mainly for C++ and also for C projects. Suitable for embedded devices.

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
* Friendly for embedded target like Cortex-Mx microcontrollers
* Small footprint, single self-registered method takes very small memory

## Build

```
mkdir -p build && cd build
cmake ..
make
```

## Install

Install libraries and headers under given directory:

```
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=<where_to_install_utest> ..
make install
```

## How to use it

* [Build for ARM targets](docs/arm-target.md)

## Examples

Link example with utest and utest-main libraries.

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
