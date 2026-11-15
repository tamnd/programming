---
tag: 2JZSZV
kind: tool
slug: cpp-package-managers
name: C and C++ package managers
aliases: [vcpkg, Conan, conanfile.txt, vcpkg.json, ConanCenter, FetchContent, CMake FetchContent, pkg-config, .pc file, C++ dependencies, C dependencies]
area: tools-and-workflow/package-managers
level: advanced
summary: >-
  C and C++ have no one package manager. vcpkg and Conan get and build libraries for a project, CMake FetchContent gets source code at configure time, and pkg-config finds the libraries on the system.
role: [package-manager]
see_also: [cmake, package-manager-concept, dependency-resolution, lock-files, c, cpp, vendoring, cross-compilation]
sources:
  - url: https://learn.microsoft.com/en-us/vcpkg/get_started/overview
    title: Microsoft Learn, vcpkg overview
    locator: the introduction; Why vcpkg?
  - url: https://docs.conan.io/2/introduction.html
    title: Conan 2 documentation, Introduction
    locator: the introduction
  - url: https://github.com/Kitware/CMake/blob/master/Modules/FetchContent.cmake
    title: CMake source, Modules/FetchContent.cmake (the module documentation)
    locator: Overview; FetchContent_MakeAvailable; the versionadded notes
  - url: https://people.freedesktop.org/~dbn/pkg-config-guide.html
    title: Dan Nicholson, Guide to pkg-config
    locator: Concepts; Writing pkg-config files; Using pkg-config files
  - url: https://learn.microsoft.com/en-us/vcpkg/concepts/manifest-mode
    title: Microsoft Learn, What is manifest mode?
    locator: the introduction; Manifest files in ports
  - url: https://learn.microsoft.com/en-us/vcpkg/users/versioning
    title: Microsoft Learn, Versioning reference (vcpkg)
    locator: Baselines; version>=; overrides
  - url: https://docs.conan.io/2/tutorial/consuming_packages/build_simple_cmake_project.html
    title: Conan 2 documentation, Build a simple CMake project using Conan
    locator: the conanfile.txt; conan profile detect; conan install
  - url: https://docs.conan.io/2/tutorial/versioning/lockfiles.html
    title: Conan 2 documentation, Lockfiles
    locator: the introduction; conan lock create
  - url: https://devblogs.microsoft.com/cppblog/vcpkg-a-tool-to-acquire-and-build-c-open-source-libraries-on-windows/
    title: "Eric Mittelette, Vcpkg: a tool to acquire and build C++ open source libraries on Windows, C++ Team Blog, 19 September 2016"
    locator: the introduction
  - url: https://blog.conan.io/2023/02/22/Conan-2.0.html
    title: Introducing Conan 2.0, Conan blog, 22 February 2023
    locator: the introduction
---

## What it is

C and C++ have no one package manager that all projects use. The vcpkg overview says that vcpkg "is designed to address the unique pain points with managing C/C++ libraries".[^1] This page describes four tools that a C or C++ project can use to get its libraries:

- **vcpkg** is "a free and open-source C/C++ package manager maintained by Microsoft and the C++ community". It runs on Windows, macOS and Linux.[^1] Its registry has over 2300 open-source libraries, and it builds them from source or downloads prebuilt binaries.[^1]
- **Conan** is "a dependency and package manager for C and C++ languages". It is free and open source, and it works with build systems such as CMake, MSBuild and Makefiles.[^2] It manages binaries: it can make and reuse binaries for different architectures and compiler versions of each version of a package.[^2]
- **CMake FetchContent** is a module of [[cmake]]. It gets source code, for example from a Git repository or a URL, when CMake configures the project, and it adds that code to the build.[^3]
- **pkg-config** does not get any code. It reads small `.pc` files that give the compiler and linker flags for a library that is already on the computer.[^4] A system package manager such as [[apt-tool]] or [[homebrew-pm]] installs these libraries.

The general ideas are on other pages: [[package-manager-concept]], [[dependency-resolution]] and [[lock-files]].

## Install and first use

vcpkg and Conan are separate programs. FetchContent is a module of CMake, so a project that uses CMake has it.[^3]

The script below uses FetchContent. First it makes a C library `greet` in a local Git repository with the tag `v1.0`. A program declares the library with `FetchContent_Declare` and gets it with `FetchContent_MakeAvailable`, and then links to the target `greet` as if the library were part of the project.[^3] The output is from CMake 3.28.3 on the build machine of this site.

```bash run timeout=30 id=cpp-fetch
# Get a C library from a Git repository with CMake FetchContent.
# Nothing comes from the network: the repository is a local one.
d=$(mktemp -d)
cd "$d"
cmake --version | head -1
mkdir greet app
cat > greet/CMakeLists.txt <<'CM'
cmake_minimum_required(VERSION 3.24)
project(greet C)
add_library(greet greet.c)
target_include_directories(greet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
CM
echo 'const char *greet(void);' > greet/greet.h
echo 'const char *greet(void) { return "Hello from greet 1.0"; }' > greet/greet.c
git -C greet init -q
git -C greet add .
git -C greet -c user.name=Example -c user.email=example@example.com commit -q -m "greet 1.0"
git -C greet tag v1.0
cat > app/CMakeLists.txt <<'CM'
cmake_minimum_required(VERSION 3.24)
project(app C)
include(FetchContent)
FetchContent_Declare(greet GIT_REPOSITORY ${GREET_REPO} GIT_TAG v1.0)
FetchContent_MakeAvailable(greet)
add_executable(app main.c)
target_link_libraries(app PRIVATE greet)
CM
cat > app/main.c <<'C'
#include <stdio.h>
#include "greet.h"
int main(void) { puts(greet()); return 0; }
C
echo "--- cmake configure and build"
cmake -S app -B build -G Ninja -DGREET_REPO="$d/greet" > configure.log 2>&1 && echo "configure: ok"
cmake --build build > build.log 2>&1 && echo "build: ok"
./build/app
echo "--- the fetched source"
ls build/_deps
git -C build/_deps/greet-src describe --tags
cd / && rm -rf "${d:?}"
```

```output for=cpp-fetch
cmake version 3.28.3
--- cmake configure and build
configure: ok
build: ok
Hello from greet 1.0
--- the fetched source
greet-build
greet-src
greet-subbuild
v1.0
```

FetchContent put the source of `greet` in `build/_deps/greet-src`, at the tag `v1.0`.

The second script writes a `.pc` file for a library that is made up, and asks pkg-config for its version and its flags.[^4] The output is from pkg-config 1.8.1 on the same machine.

```bash run timeout=30 id=cpp-pkgconfig
# Write a .pc file for a made-up library, and ask pkg-config about it.
d=$(mktemp -d)
mkdir "$d/pc"
cd "$d"
pkg-config --version
cat > pc/example-lib.pc <<'PC'
prefix=/opt/example
includedir=${prefix}/include
libdir=${prefix}/lib

Name: example-lib
Description: A made-up library for this example
Version: 1.5.0
Cflags: -I${includedir}
Libs: -L${libdir} -lexample-lib
PC
export PKG_CONFIG_PATH="$d/pc"
pkg-config --modversion example-lib
pkg-config --cflags --libs example-lib | sed 's/ *$//'
cd / && rm -rf "${d:?}"
```

```output for=cpp-pkgconfig
1.8.1
1.5.0
-I/opt/example/include -L/opt/example/lib -lexample-lib
```

pkg-config printed the flags from the `Cflags` and `Libs` lines of the `.pc` file. The variable `PKG_CONFIG_PATH` adds a directory to the search path of pkg-config; without it, pkg-config looks in directories such as `/usr/lib/pkgconfig`.[^4]

## Concepts of the tool

**vcpkg: manifest mode.** vcpkg has two modes, classic mode and manifest mode, and the documentation recommends manifest mode.[^5] In manifest mode, a file `vcpkg.json` in the project lists the dependencies, and `vcpkg install` installs them.[^5] Manifest mode is necessary for versioning and for custom registries.[^5]

**vcpkg: versions.** The reference says: "Baselines define a global version floor for what versions will be considered". So the manifest does not need a version for each dependency.[^6] The field `builtin-baseline` gives the baseline of the built-in registry.[^6] The field `"version>="` sets a minimum version for one dependency. The reference says: "vcpkg selects the lowest version that matches all constraints, so a less-than constraint is not required."[^6] An override "forces vcpkg to ignore all other version constraints and use the version specified in the override".[^6]

```json
{
  "name": "project",
  "version-semver": "1.0.0",
  "dependencies": [
    { "name": "zlib", "version>=": "1.2.11#9" },
    { "name": "fmt", "version>=": "7.1.3#1" }
  ],
  "builtin-baseline": "3426db05b996481ca31e95fff3734cf23e0f51bc"
}
```

In `1.2.11#9`, the part after `#` is the port version: version 1.2.11 of zlib, port version 9.[^6]

**Conan: conanfile, profiles and generators.** A project lists its libraries in a `conanfile.txt`. The `[requires]` section names each library and its version, and the `[generators]` section says which files Conan writes for the build system.[^7]

```ini
[requires]
zlib/1.3.1

[generators]
CMakeDeps
CMakeToolchain
```

`CMakeDeps` writes the information that CMake needs to find the libraries, and `CMakeToolchain` writes a CMake toolchain file.[^7] Conan also needs a profile, which gives the operating system, the architecture, the compiler and the build type. `conan profile detect --force` makes a default profile from the tools on the computer.[^7]

**Conan: lock files.** The Conan documentation says that lockfiles "are a mechanism to achieve reproducible dependencies, even when new versions or revisions of those dependencies are created". `conan lock create .` writes the file `conan.lock`, which records each version with its revision.[^8]

**FetchContent: declare, then make available.** `FetchContent_Declare` gives the details of a dependency, for example a Git repository and a tag, or a URL and a hash. `FetchContent_MakeAvailable` gets the content and adds it to the main build.[^3] FetchContent gets the content when CMake configures the project, so the configure step can already use it. The module `ExternalProject` gets content at build time.[^3]

**pkg-config: .pc files.** The guide says that the primary use of pkg-config "is to provide the necessary details for compiling and linking a program to a library".[^4] A `.pc` file has variables such as `prefix=` and keywords such as `Name:`, `Version:`, `Cflags:` and `Libs:`.[^4]

## Common tasks

| Task | Command |
|---|---|
| Install the dependencies of a vcpkg manifest | `vcpkg install`, in the directory of `vcpkg.json`[^5] |
| Add the first baseline to a vcpkg manifest | `vcpkg x-update-baseline --add-initial-baseline`[^6] |
| Move a vcpkg manifest to a later baseline | `vcpkg x-update-baseline`[^6] |
| Make a default Conan profile | `conan profile detect --force`[^7] |
| Install the Conan dependencies and write the CMake files | `conan install . --output-folder=build --build=missing`[^7] |
| Write a Conan lock file | `conan lock create .`[^8] |
| Get a library from Git at configure time | `FetchContent_Declare(...)` and `FetchContent_MakeAvailable(...)` in `CMakeLists.txt`[^3] |
| Show the version of an installed library | `pkg-config --modversion foo`[^4] |
| Show the compiler and linker flags of a library | `pkg-config --cflags --libs foo`[^4] |

## Configuration

**vcpkg.** `vcpkg.json` is the manifest of a project and of each port.[^5] The fields `builtin-baseline` and `overrides` are in `vcpkg.json`. Each configured registry has its own baseline.[^6]

**Conan.** `conanfile.txt` gives the requirements and the generators, and the profiles in the Conan home folder give the settings of the build.[^7]

**FetchContent.** The settings are the arguments of `FetchContent_Declare` in `CMakeLists.txt`.[^3] The run above sets the repository with a CMake variable, `GREET_REPO`, so that the script can point to a temporary directory.

**pkg-config.** `PKG_CONFIG_PATH` adds directories to the search path.[^4]

## Alternatives

A system package manager can install a C or C++ library with its headers: see [[apt-tool]], [[homebrew-pm]], [[nix]] and [[other-system-package-managers]]. The vcpkg announcement of 2016 named Homebrew on macOS and apt-get and yum on Linux as tools that C++ developers compared with the situation on Windows.[^9] A project can also copy the source of a library into its own repository: see [[vendoring]].

The package managers of other languages are on the pages [[cargo]], [[go-modules]], [[nuget-pm]] and [[other-language-package-managers]].

## History and versions

| Date or version | Event |
|---|---|
| 19 September 2016 | Microsoft announced vcpkg on GitHub, as a tool to get and build open-source libraries on Windows.[^9] |
| CMake 3.11 | CMake added the module FetchContent.[^3] |
| CMake 3.14 | CMake added the command `FetchContent_MakeAvailable`.[^3] |
| 22 February 2023 | Conan 2.0 came out.[^10] |

vcpkg now runs on Windows, macOS and Linux.[^1]

## References
