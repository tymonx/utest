# Linux

## Build

```
mkdir -p build && cd build
cmake ..
make
```

Under `build/lib` directory you can find two static libraries `libutest.a` and
`libutest-main.a`.

## Install

Install libraries and headers under given directory:

```
mkdir -p build && cd build
cmake -DCMAKE_INSTALL_PREFIX=<where_to_install_utest> ..
make install
```

Headers are installed under `<utest_install_dir>/include/utest` and libraries
under `<utest_install_dir>/lib`.

## Create and run unit test

Create [factorial.cpp](https://github.com/tymonx/utest#examples) file.
Build example using GCC:

```
g++ -std=c++11 -I<utest_include_dir> -L<utest_libraries_dir> factorial.cpp -o factorial -pthread -lutest-main -lutest
```

Build example using Clang:

```
clang++ -std=c++11 -I<utest_include_dir> -L<utest_libraries_dir> factorial.cpp -o factorial -pthread -lutest-main -lutest
```

`<utest_include_dir>` may point to `<utest_source_dir>/include` or to `<utest_install_dir>/include`.
`<utest_libraries_dir>` may point to `<utest_build_dir>/lib` or to `<utest_install_dir>/lib`.
Flags -I and -L may be omitted when `<utest_install_dir>` is `/usr/` or `/usr/local/`.

Run example:

```
./factorial
```
