# µTest
Lightweight unit tests framework mainly for C++ and also for C projects. Target for embedded devices.

## Key Features

* C++11 support
* An XUnit test framework
* No evil defined test macros, programmer API style
* Automatic test registration
* A rich set of assertions
* Fatal and non-fatal assertions
* Support for multi reporters with multi writers
* Working with/without exceptions and RTTI
* Optionally running test case in isolate environment using thread
* Friendly for embedded target like Cortex-Mx microcontrollers
* Small memory footprint

## Build

```
mkdir build
cd build
cmake ..
make
```

## Examples

```
./bin/simple
 ```
