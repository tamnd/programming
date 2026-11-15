---
tag: 3CPWDK
kind: tool
slug: cmake
name: CMake
aliases: [cmake, CMakeLists.txt, CMake presets, CMakePresets.json, ctest, meta-build system, build system generator]
area: tools-and-workflow/build-systems
level: core
summary: >-
  CMake is a build system generator for C, C++ and other languages. A project describes its targets in CMakeLists.txt files, and CMake writes the files for a build tool such as Ninja or Make.
role: [build]
licence: bsd-3-clause
repo: https://github.com/Kitware/CMake
homepage: https://cmake.org/
history:
  year: 2000
  people: []
  note: Kitware made CMake for the Insight Segmentation and Registration Toolkit (ITK), in work that the National Library of Medicine started in 1999. The copyright notice of CMake starts in 2000.
see_also: [make, ninja, build-system-concept, build-a-project, write-a-makefile, build-graph, incremental-builds]
sources:
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/README.rst
    title: CMake 3.28.3 source, README.rst
    locator: Introduction; License
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake.1.rst
    title: CMake 3.28 documentation, cmake(1)
    locator: Description; Introduction to CMake Buildsystems (Source Tree, Build Tree, Generator); Generate a Project Buildsystem; Build a Project
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/ctest.1.rst
    title: CMake 3.28 documentation, ctest(1)
    locator: --test-dir
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake-buildsystem.7.rst
    title: CMake 3.28 documentation, cmake-buildsystem(7)
    locator: Binary Targets; Build Specification and Usage Requirements; Transitive Usage Requirements
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake-generators.7.rst
    title: CMake 3.28 documentation, cmake-generators(7)
    locator: Introduction
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/variable/CMAKE_BUILD_TYPE.rst
    title: CMake 3.28 documentation, CMAKE_BUILD_TYPE
    locator: the whole page
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/command/cmake_minimum_required.rst
    title: CMake 3.28 documentation, cmake_minimum_required
    locator: the introduction
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/manual/cmake-presets.7.rst
    title: CMake 3.28 documentation, cmake-presets(7)
    locator: Introduction; Format (version)
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Help/command/find_package.rst
    title: CMake 3.28 documentation, find_package
    locator: Search Modes (Module mode, Config mode)
  - url: https://aosabook.org/en/v1/cmake.html
    title: Bill Hoffman and Kenneth Martin, CMake, chapter 5 of The Architecture of Open Source Applications, Volume 1
    locator: the introduction; 5.1 CMake History and Requirements
  - url: https://github.com/Kitware/CMake/blob/v3.28.3/Copyright.txt
    title: CMake 3.28.3 source, Copyright.txt
    locator: the first two lines
---

## What it is

CMake is a build system generator. Its README says: "CMake is a cross-platform, open-source build system generator."[^1] A project does not write a makefile or an IDE project file by hand. It describes its targets in files named `CMakeLists.txt`, in the CMake language, and "From these files CMake generates a preferred buildsystem locally for each user through a backend called a generator."[^2] The generated files are then run by a native build tool, such as [[ninja]], [[make]] or Visual Studio.

CMake is used most for C and C++. It is free software under the BSD 3-clause licence, and Kitware maintains it with a community of contributors.[^1] The tool `ctest` runs the tests of a CMake project, and `cpack` makes packages.[^2]

## Install and first use

Install CMake with the package manager of the system, or with the installers from the CMake project. The command `cmake --version` shows the version. The examples on this page ran with CMake 3.28.3, Ninja 1.11.1 and GCC 13 on Ubuntu 24.04.

A build with CMake has two steps:

1. **Configure and generate.** `cmake -S src -B build` uses `src` as the source tree and `build` as the build tree, and "The build tree will be created automatically if it does not already exist."[^2] The source tree must contain a `CMakeLists.txt` file.[^2] The option `-G` selects the generator.[^2]
2. **Build.** `cmake --build build` builds the project. This command "abstracts a native build tool's command-line interface", so the same command works with each generator.[^2]

CMake writes a `CMakeCache.txt` file in the build tree, which marks the directory as a build tree and keeps settings such as the options of the build.[^2] The manual recommends a build tree that is separate from the source tree: "To maintain a pristine source tree, perform an out-of-source build by using a separate dedicated build tree."[^2]

The run below has a static library `calc`, a program `app` that uses it, and a test. It configures the project with the Ninja generator, builds it, runs the program and the test, and builds again. The configure step prints many lines about the compiler checks, so the run hides them and shows two settings from `CMakeCache.txt`. The option `-j 1` sets "The maximum number of concurrent processes to use when building", and the run uses it only to keep the order of the output the same in each run.[^2] The run also removes the run time from the test output.

```bash run id=cmake-project
d=$(mktemp -d)
cd "$d"
mkdir src
cat >src/calc.h <<'END'
int add(int a, int b);
END
cat >src/calc.c <<'END'
#include "calc.h"
int add(int a, int b) { return a + b; }
END
cat >src/main.c <<'END'
#include <stdio.h>
#include "calc.h"
int main(void) { printf("2 + 3 = %d\n", add(2, 3)); return 0; }
END
cat >src/test_calc.c <<'END'
#include "calc.h"
int main(void) { return add(2, 3) == 5 ? 0 : 1; }
END
cat >CMakeLists.txt <<'END'
cmake_minimum_required(VERSION 3.20)
project(calc VERSION 1.0 LANGUAGES C)

add_library(calc STATIC src/calc.c)
target_include_directories(calc PUBLIC src)

add_executable(app src/main.c)
target_link_libraries(app PRIVATE calc)

enable_testing()
add_executable(test_calc src/test_calc.c)
target_link_libraries(test_calc PRIVATE calc)
add_test(NAME add COMMAND test_calc)
END
echo "--- configure"
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release >/dev/null 2>&1
echo "exit status: $?"
grep -E '^(CMAKE_BUILD_TYPE|CMAKE_GENERATOR):' build/CMakeCache.txt
echo "--- build"
cmake --build build -j 1 2>&1
build/app
echo "--- test"
ctest --test-dir build 2>&1 | grep -E 'Test #|tests passed' | sed -E 's/ +[0-9.]+ sec$//'
echo "--- build again"
cmake --build build 2>&1
cd /
rm -rf "${d:?}"
```

```output for=cmake-project
--- configure
exit status: 0
CMAKE_BUILD_TYPE:STRING=Release
CMAKE_GENERATOR:INTERNAL=Ninja
--- build
[1/6] Building C object CMakeFiles/calc.dir/src/calc.c.o
[2/6] Linking C static library libcalc.a
[3/6] Building C object CMakeFiles/app.dir/src/main.c.o
[4/6] Linking C executable app
[5/6] Building C object CMakeFiles/test_calc.dir/src/test_calc.c.o
[6/6] Linking C executable test_calc
2 + 3 = 5
--- test
1/1 Test #1: add ..............................   Passed
100% tests passed, 0 tests failed out of 1
--- build again
ninja: no work to do.
```

The second `cmake --build` finds nothing to do, because Ninja checks each output against its inputs. `ctest --test-dir build` runs the tests that `add_test` defines, and `--test-dir` gives "the directory in which to look for tests, typically a CMake project build directory."[^3]

## Concepts of the tool

**Targets.** A target is an executable or a library that the build makes. "Executables and libraries are defined using the add_executable and add_library commands."[^4] The command `target_link_libraries` sets the dependencies between targets.[^4] Each target is a node of the [[build-graph]], and CMake writes the edges for the build tool.

**Usage requirements.** Commands such as `target_include_directories`, `target_compile_definitions` and `target_compile_options` set the settings that a target needs to compile, and the settings that the targets that use it need. Each of these commands has a `PRIVATE`, a `PUBLIC` and an `INTERFACE` mode.[^4] `PRIVATE` applies a setting only to the target, `INTERFACE` applies it only to the targets that use it, and `PUBLIC` applies it to both.[^4] In the example, `target_include_directories(calc PUBLIC src)` lets both `calc` and `app` find `calc.h`. The CMake manual says that a dependency used only by the implementation of a library should be `PRIVATE`, and that a dependency also used in the header files of the library should be `PUBLIC`.[^4]

**Generators.** "A CMake Generator is responsible for writing the input files for a native build system."[^5] "Exactly one of the CMake Generators must be selected for a build tree".[^5] The Makefile generators and the Ninja generators are for command-line build tools, and other generators write project files for IDEs such as Visual Studio and Xcode.[^5]

**Build types.** `CMAKE_BUILD_TYPE` "Specifies the build type on single-configuration generators (e.g. Makefile Generators or Ninja)." The typical values are `Debug`, `Release`, `RelWithDebInfo` and `MinSizeRel`.[^6] The default value is often an empty string, and the CMake documentation says that this "is usually not desirable".[^6] Set the build type when you configure, as with `-DCMAKE_BUILD_TYPE=Release` in the run above.

**Policies.** `cmake_minimum_required(VERSION 3.20)` sets the minimum version of CMake, and an older CMake stops with an error.[^7] The manual says to call it at the beginning of the top `CMakeLists.txt`, before the `project` command.[^7] It also sets the policies. Each policy that the given version or an older version introduced uses its `NEW` behavior, and the policies of later versions are unset, so that a later CMake warns about them.[^7]

## Common tasks

| Task | Command or code |
|---|---|
| Configure a build tree with Ninja | `cmake -S . -B build -G Ninja`[^2] |
| Build | `cmake --build build`[^2] |
| Run the tests | `ctest --test-dir build`[^3] |
| Use a configure preset | `cmake --preset release`[^8] |
| Add a library | `add_library(calc STATIC src/calc.c)`[^4] |
| Link a target to a library | `target_link_libraries(app PRIVATE calc)`[^4] |
| Use a package that is installed on the system | `find_package(ZLIB REQUIRED)`, then `target_link_libraries(app PRIVATE ZLIB::ZLIB)` |

**Packages.** `find_package` finds a package outside the project and loads its details.[^9] In Module mode, CMake looks for a file `Find<PackageName>.cmake`, which the operating system, CMake or the project provides. The CMake manual says that these find modules "tend to be heuristic in nature and are susceptible to becoming out-of-date."[^9] In Config mode, CMake looks for a file `<PackageName>Config.cmake` or `<lowercasePackageName>-config.cmake`, which the package installs.[^9] Both modes give the project targets with names such as `ZLIB::ZLIB` that carry their own usage requirements.

## Configuration

**Presets.** The files `CMakePresets.json` and `CMakeUserPresets.json` in the root of the project store the options of common builds, so that people can share them.[^8] `CMakePresets.json` holds the settings of the project and goes into version control. `CMakeUserPresets.json` holds the settings of one developer, and the manual says that it "should NOT be checked in".[^8] The field `version` gives the version of the format: version 6 needs CMake 3.25, and CMake 3.28 reads up to version 8.[^8] A preset can inherit from another preset. The run below lists the presets of a small project.

```bash run id=cmake-presets
d=$(mktemp -d)
cd "$d"
cat >CMakeLists.txt <<'END'
cmake_minimum_required(VERSION 3.20)
project(hello LANGUAGES C)
add_executable(hello hello.c)
END
cat >CMakePresets.json <<'END'
{
  "version": 6,
  "configurePresets": [
    {
      "name": "debug",
      "displayName": "Debug build with Ninja",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/debug",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug" }
    },
    {
      "name": "release",
      "inherits": "debug",
      "displayName": "Release build with Ninja",
      "binaryDir": "${sourceDir}/build/release",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Release" }
    }
  ],
  "buildPresets": [
    { "name": "release", "configurePreset": "release" }
  ]
}
END
cmake --list-presets 2>&1
cmake --build --list-presets 2>&1
cd /
rm -rf "${d:?}"
```

```output for=cmake-presets
Available configure presets:

  "debug"   - Debug build with Ninja
  "release" - Release build with Ninja
Available build presets:

  "release"
```

**The cache.** A value given with `-D` on the command line, such as `-DCMAKE_BUILD_TYPE=Release`, is stored in `CMakeCache.txt` and stays for the later runs of CMake in the same build tree.[^2] To start again with other settings, delete the build tree.

## Alternatives

- [[make]]: a makefile written by hand gives full control, but the author must write the compile commands and the header dependencies for each platform.
- Meson also generates the files for [[ninja]] from a description of the project in its own language.
- Bazel and Buck2 run the build themselves, with no separate build tool. They use [[hermetic-builds]] and [[remote-caching-and-execution]].
- Autotools generates a `configure` script and makefiles. This was the usual way to build on Unix before CMake.[^10]

## History and versions

In 1999, the National Library of Medicine engaged Kitware to make a better way to configure, build and deploy software on many platforms, as part of the Insight Segmentation and Registration Toolkit (ITK).[^10] At that time, a project usually had a configure script and makefiles for Unix, and Visual Studio project files for Windows.[^10] CMake replaced the two systems with one description. Its first requirements included that it needs only a C++ compiler, and that it can generate Visual Studio project files.[^10] The copyright notice of CMake starts in the year 2000.[^11]

CMake runs in two steps. In the configure step, it reads `CMakeCache.txt` and the `CMakeLists.txt` files, and it runs their commands. In the generate step, it writes the build files for the build tool that the user selected.[^10] CMake 2.6 added the policy system: a change that would break the build of a project gives a warning, and CMake keeps the old behavior.[^10] CMake 3.13 added the `-S` and `-B` options, and CMake 3.19 added presets.[^2][^8]

## References
