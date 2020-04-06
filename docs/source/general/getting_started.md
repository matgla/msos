# Getting started with MSOS

MSOS is operating system targeting embedded devices with very restricted resources. It's POSIX like system which supports features
+ executable dynamic loading
+ shared library dynamic linking
+ preemptive context switching
+ no MMU

Currently supported architectures are:
+ Cortex-M3

# How to build

Requirements:
+ arm-none-eabi toolchain must be installed in operating system. It's possible also to pass specific version with -DARM_TOOLCHAIN_PATH={path} in CMake configure command.

Firstly clone repository from:

```console
git clone https://github.com/matgla/msos.git
```

**Important**: It's not necessary to fetch submodules. External dependencies are managed by CMake.

After clone project must be configured like normal CMake, but BOARD variable must be passed to provide support for specific processor and its peripherals.

Currently supported boards are:

+ Stm32_Black_Pill

To disable tests while building target please add -DDISABLE_TESTS=ON flag, to CMake.

```console
mkdir build
cd build && cmake .. -DBOARD=Stm32_Black_Pill -DDISABLE_TESTS=ON
```

To build project:
```console
make
```

Artifact files will be created where cmake was called, i.e:
```console
build/msos_test.bin
```
This binary file is ready for upload to real microcontroller flash.

# QEMU

QEMU software can be used to test binary file created in previous step.

There is fork of qemu which supports Stm32_Black_Pill board.

https://github.com/matgla/qemu_stm32

## How to use it

```console
qemu-system-arm -kernel build/msos_test.bin -machine stm32_black_pill -serial stdio
```

# Test running

**Requirements**:
+ arm-none-eabi toolchain in system
+ [qemu](#QEMU)

In future it's planned to include some toolchain package via Yocto or Docker.

In root directory of project
```console
make test
```
Command will build and execute unit tests (X86-64) and system tests (ARM Cortex-M3).

