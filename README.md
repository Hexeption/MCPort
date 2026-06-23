# MCPort

A small C++ port of early Minecraft/RubyDung rd-161348 code.

## Build

Needs CMake, Xcode command line tools, and Git.

```sh
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target MCPort
```

For release:

```sh
cmake -S . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --target MCPort
```

## Run

Debug build:

```sh
./bin/debug/MCPort
```

Release build:

```sh
./bin/release/MCPort
```

Assets are copied into the output folder after building.
