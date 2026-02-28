# Artificial Will Game and Game Engine

This is a game about a robot Will overcoming obstacles. The game has a very deep meaning I will come up later.

Also, this is my fun project of writing a game engine and a use case for my other projects, so I can test them in real application and improve:

- https://github.com/an-dr/abcmake - my CMake-compatible build system
- https://github.com/an-dr/hyphoria - my messaging framework
- https://github.com/an-dr/microlog - my logging library

## Prerequirements

Most of the deps are resolved via [vcpkg](https://github.com/microsoft/vcpkg) so:

1. Install vcpkg
2. Specify VCPKG_ROOT environment variable
3. Add to your CMake profile `--preset  vcpkg`