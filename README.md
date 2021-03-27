# Runtime Verification Technical Assessment

This repository implements a solution to the compiler implementation
[task][task] set as part of the RV interview process. When built, the code will
produce an executable (`rvc`) that parses and compiles a single file matching
the required grammar to LLVM bitcode. When compiled and executed, this code
prints the result of the input arithmetic expression to `stdout` and exits.

## Building and Running

The code builds and has been tested on two platforms:
* macOS 11
* Ubuntu 20.04

The easiest way to build it is to use the included `Vagrantfile` to create an
Ubuntu VM with the correct dependencies already installed:
```shell
vagrant up
vagrant ssh
```

Then, inside the VM:
```shell
cd /src
mkdir build
cd build
cmake -DLLVM_DIR=$(llvm-config-11 --cmakedir) ..
make
```

On macOS, first install dependencies with Homebrew:
```shell
brew install flex bison llvm@11
```

Then, to build the compiler:
```shell
mkdir build
cd build
cmake \
  -DLLVM_DIR=$($(brew --prefix llvm@11)/bin/llvm-config --cmakedir) \
  -DFL_LIBRARY=$(brew --prefix flex)/lib/libfl.dylib \
  ..
make
```

## Tests

Included is a small test suite verifying that pairs of expressions produce
equivalent results. Once the compiler is built:
```shell
cd test
./test.sh
```

[task]: https://gist.github.com/dwightguth/d36516734edd8118236162de6d16149a
