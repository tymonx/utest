# ARM

## Toolchain

Download and unpack [gcc-arm-none-eabi][arm-toolchain] toolchain. Add
`<path_to_toolchain>/bin` to PATH environment variable.

## Build examples for QEMU

Build using gcc-arm-none-eabi toolchain:

```
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake -DCMAKE_SYSTEM_PROCESSOR=cortex-m3 -DSEMIHOSTING=ON -DEXAMPLES=ON ..
make
```

Build using Clang compiler and GCC linker from gcc-arm-none-eabi toolchain:

```
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-clang-arm-none-eabi.cmake -DCMAKE_SYSTEM_PROCESSOR=cortex-m3 -DSEMIHOSTING=ON -DEXAMPLES=ON ..
make
```

Run factorial example using QEMU:

```
qemu-arm -cpu cortex-m3 ./bin/factorial
```

## Build examples for embedded target

Download and unpack CMSIS archive from [ARM][arm-cmsis]. Adapt linker script
and startup code to your project. Example for GCC compilers and Cortex-M3
target you can find all necessary files under
`<path_to_cmsis>/Device/ARM/ARMCM3/Source/GCC/` directory.

Build:

```
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake -DCMAKE_SYSTEM_PROCESSOR=cortex-m3 -DSEMIHOSTING=ON -DEXAMPLES=ON -DLINKER_SCRIPT=<linker_script_file> -DSTARTUP_CODE=<startup_code_file> ..
make
```

[arm-toolchain]: https://launchpad.net/gcc-arm-embedded
[arm-cmsis]: https://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php
