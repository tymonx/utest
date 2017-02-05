# ARM

## Toolchain

Download and unpack [gcc-arm-none-eabi][arm-toolchain] toolchain. Add
`<path_to_toolchain>/bin` to PATH environment variable.

## Build examples for QEMU

Build:

```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake -DCMAKE_SYSTEM_PROCESSOR=cortex-m3 -DSEMIHOSTING=ON ..
make
```

Run factorial example using QEMU:

```
qemu-arm -cpu cortex-m3 ./bin/factorial
```

## Build examples for embedded target:

Download and unpack CMSIS archive from [ARM][arm-cmsis]. Adapt linker script
and startup code to your project. Example for GCC compilers and Cortex-M3
target you can find all necessary files under
`<path_to_cmsis>/Device/ARM/ARMCM3/Source/GCC/` directory.

Build:

```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-gcc-arm-none-eabi.cmake -DCMAKE_SYSTEM_PROCESSOR=cortex-m3 -DSEMIHOSTING=OFF -DLINKER_SCRIPT=<path_to_linker_script> -DSTARTUP_CODE=<path_to_startup_code> ..
make
```

[arm-toolchain]: https://launchpad.net/gcc-arm-embedded
[arm-cmsis]: https://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php
