---
tag: HMJXQW
kind: tool
slug: meson
name: Meson
aliases: [meson, Meson build, meson.build, meson setup, meson compile, meson test, Meson wrap]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Meson is a build system for C, C++ and other languages. A project describes its targets in meson.build files, in a small language that is not Turing-complete, and Meson writes the files for Ninja.
role: [build]
licence: apache-2.0
repo: https://github.com/mesonbuild/meson
homepage: https://mesonbuild.com/
history:
  year: 2013
  people: [Jussi Pakkanen]
  note: Jussi Pakkanen started Meson in the Christmas holidays of 2012 as an experiment in build system design, and the first release came out on 2 March 2013. GNOME, GTK, GLib and the X.org server build with Meson.
see_also: [ninja, cmake, autotools, make, build-system-concept, build-graph, incremental-builds]
sources:
  - url: https://mesonbuild.com/FAQ.html
    title: The Meson Build System, FAQ
    locator: Why is it called Meson?; Why can't I specify target files with a wildcard?; Why is there not a Make backend?; Why is Meson not just a Python module
  - url: https://mesonbuild.com/Running-Meson.html
    title: The Meson Build System, Running Meson
    locator: Configuring the build directory (the buildtype values); Building from the source
  - url: https://mesonbuild.com/Builtin-options.html
    title: The Meson Build System, Built-in options
    locator: Core options (buildtype, optimization, debug, warning_level, werror, default_library)
  - url: https://mesonbuild.com/Syntax.html
    title: The Meson Build System, Syntax
    locator: the introduction; Variables (immutable objects); Foreach statements; Feature not available (user-defined functions)
  - url: https://github.com/mesonbuild/meson/blob/1.3.2/README.md
    title: Meson README, tag 1.3.2
    locator: Dependencies; Installing from source; Running
  - url: https://github.com/mesonbuild/meson/blob/1.3.2/COPYING
    title: Meson licence, tag 1.3.2
    locator: the whole file (Apache License 2.0)
  - url: https://mesonbuild.com/Unit-tests.html
    title: The Meson Build System, Unit tests
    locator: the introduction
  - url: https://mesonbuild.com/Dependencies.html
    title: The Meson Build System, Dependencies
    locator: the introduction (dependency, required, pkg-config)
  - url: https://mesonbuild.com/Wrap-dependency-system-manual.html
    title: The Meson Build System, Wrap dependency system manual
    locator: the introduction; How it works; wrap format
  - url: https://mesonbuild.com/Design-rationale.html
    title: The Meson Build System, Design rationale
    locator: the requirements list; Overview of the solution
  - url: https://en.wikipedia.org/wiki/Meson_(software)
    title: Wikipedia, Meson (software)
    locator: the infobox (developer, first release); Adopters
  - url: https://mesonbuild.com/Users.html
    title: The Meson Build System, Users
    locator: the list of notable projects
---

## What it is

Meson is a build system for C, C++ and other languages. A project describes its programs, libraries and tests in files named `meson.build`. Meson reads these files and writes the files for a build back end, which does the build. Its FAQ says: "The main backend of Meson is Ninja".[^1] On Windows and macOS, Meson can also write project files for Visual Studio and Xcode.[^2][^3]

The language of `meson.build` is small and declarative, and it is not a general programming language. The Syntax page says that the language "is strongly typed so no object is ever converted to another under the covers", and that all objects are immutable.[^4] Meson is written in Python, and it is free software under the Apache License 2.0.[^1][^5][^6]

## Install and first use

Meson needs Python 3 and Ninja. The README of Meson 1.3.2 gives the command `python3 -m pip install meson`, and many systems also have a package for it.[^5] The examples on this page ran with Meson 1.3.2, Ninja 1.11.1 and GCC 13 on Ubuntu 24.04.

A build with Meson has three commands:

1. `meson setup build` reads `meson.build` in the current directory and writes a build directory `build`. Meson always uses a build directory that is separate from the source directory, and the README says that "these two are different".[^5]
2. `meson compile -C build` builds the project in that directory.[^2]
3. `meson test -C build` runs the tests that `meson.build` defines.[^7]

The run below has a static library `calc`, a program `app` that uses it, and a test. It configures, builds, runs the program and the test, and builds again. The run passes `-j 1` to `meson compile` only to keep the order of the output the same in each run, and it removes the run times from the test output.

```bash run id=meson-project
d=$(mktemp -d)
cd "$d"
mkdir src tests
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
cat >tests/test_calc.c <<'END'
#include "calc.h"
int main(void) { return add(2, 3) == 5 ? 0 : 1; }
END
cat >meson.build <<'END'
project('calc', 'c',
  version : '1.0',
  default_options : ['warning_level=2', 'buildtype=release'])

inc = include_directories('src')
calc = static_library('calc', 'src/calc.c', include_directories : inc)
executable('app', 'src/main.c', link_with : calc, include_directories : inc)
test_calc = executable('test_calc', 'tests/test_calc.c',
  link_with : calc, include_directories : inc)
test('add', test_calc)
END
echo "--- meson setup build"
meson setup build >setup.log 2>&1
echo "exit status: $?"
grep -E '^(Project name|Project version|Build targets in project):' setup.log
echo "--- meson compile -C build -j 1"
meson compile -C build -j 1 2>&1 | grep '^\['
build/app
echo "--- meson test -C build"
meson test -C build 2>&1 | grep -E '^ *1/1 |^(Ok|Fail):' | sed -E 's/ +/ /g; s/ [0-9.]+s$//; s/ +$//'
echo "--- meson compile again"
meson compile -C build 2>&1 | grep -E '^ninja: no work'
cd /
rm -rf "${d:?}"
```

```output for=meson-project
--- meson setup build
exit status: 0
Project name: calc
Project version: 1.0
Build targets in project: 3
--- meson compile -C build -j 1
[1/6] Compiling C object libcalc.a.p/src_calc.c.o
[2/6] Linking static target libcalc.a
[3/6] Compiling C object app.p/src_main.c.o
[4/6] Linking target app
[5/6] Compiling C object test_calc.p/tests_test_calc.c.o
[6/6] Linking target test_calc
2 + 3 = 5
--- meson test -C build
1/1 add OK
Ok: 1
Fail: 0
--- meson compile again
ninja: no work to do.
```

The second `meson compile` calls Ninja, and Ninja finds no output that is older than its inputs. The names of the object files, such as `libcalc.a.p/src_calc.c.o`, come from Meson, and the user does not choose them.

## Concepts of the tool

**Targets.** A function call such as `executable`, `static_library` or `shared_library` declares a target, and its return value is an object that other calls can use. In the example, `link_with : calc` links `app` to the library. The function `test` adds a test, and `meson test` runs it.[^7] Meson captures the output of the tests in the file `meson-logs/testlog.txt` in the build directory.[^7]

**The language.** The language has variables, integers, booleans, strings, arrays, dictionaries, function calls, method calls, `if` and `foreach`.[^4] It has three properties that make it different from the files of many other build tools:

- **It is strongly typed.** An operation on values of the wrong types is an error, and Meson does not convert one value to another.[^4]
- **Objects are immutable.** When a statement such as `more += ['mul']` seems to change an array, Meson makes a different array and gives it the name, and other names that point to the old array do not change.[^4]
- **It is not Turing-complete.** The language has no user-defined functions. The Syntax page says that functions "would make Meson Turing-complete which would make it harder to reason about and more difficult to integrate with tools like IDEs."[^4]

The run below shows each of these properties. `message` prints a line while Meson reads the file. The last line adds a number to a string, and `meson setup` stops with an error that gives the line and the column.

```bash run id=meson-language
d=$(mktemp -d)
cd "$d"
cat >meson.build <<'END'
project('lang', version : '1.0')

names = ['add', 'sub']
more = names
more += ['mul']
message('names has', names.length(), 'items, more has', more.length())

foreach n : more
  if n.startswith('s')
    message(n, 'starts with s')
  endif
endforeach

x = '1' + 1
END
meson setup build >setup.log 2>&1
echo "exit status: $?"
grep -E '^(Message|meson.build:)' setup.log
cd /
rm -rf "${d:?}"
```

```output for=meson-language
exit status: 1
Message: names has 2 items, more has 3
Message: sub starts with s
meson.build:14:10: ERROR: The `+` operator of str does not accept objects of type int (1)
```

The array `names` still has 2 items after `more += ['mul']`, because `more` is now a different array.

**Dependencies.** `dependency('zlib', version : '>=1.2.8')` finds a library outside the project, and a target that lists it in `dependencies` gets the compiler and linker flags that it needs.[^8] Meson finds a library through its pkg-config file, and it has other ways to find some libraries that do not have one.[^8] With `required : false`, a missing dependency is not an error, and the method `found()` tells the build if Meson found it.[^8]

**Subprojects and wraps.** A subproject is a Meson project inside the directory `subprojects` of another project. A wrap file, such as `subprojects/zlib.wrap`, tells Meson where to download the source of a subproject and which hash it must have. Meson downloads and extracts the source when the build needs it.[^9] This lets a project build a dependency from source on a system that does not have it.

## Common tasks

| Task | Command or code |
|---|---|
| Configure a build directory | `meson setup build`[^2] |
| Configure a release build | `meson setup build --buildtype=release`[^2] |
| Build | `meson compile -C build`[^2] |
| Run the tests | `meson test -C build`[^7] |
| Add a program | `executable('app', 'main.c')` |
| Use a library from the system | `zdep = dependency('zlib')`, then `dependencies : zdep` on the target[^8] |
| Add a test | `test('add', test_calc)`[^7] |

## Configuration

**Build types.** By default, `meson setup` makes a debug build, with debug information and no optimization.[^2] The option `buildtype` has the values `plain`, `debug`, `debugoptimized`, `release`, `minsize` and `custom`.[^3] The Running Meson page says that `release` means "full optimization, no debug info", and that `plain` adds no flags at all, which is useful for distribution packagers.[^2]

**Options.** Each built-in option, such as `buildtype`, `optimization`, `debug`, `warning_level`, `werror` or `default_library`, has a default value.[^3] A project can change the defaults with `default_options` in `project()`, as the first run does with `warning_level=2` and `buildtype=release`. The user can change an option with `-D` when they run `meson setup`, or later with `meson configure`. The run below shows that a change to `buildtype` also changes `optimization` and `debug`, and that Meson rejects the name of an option that does not exist.

```bash run id=meson-options
d=$(mktemp -d)
cd "$d"
cat >hello.c <<'END'
#include <stdio.h>
int main(void) { puts("hello"); return 0; }
END
cat >meson.build <<'END'
project('hello', 'c', version : '1.0')
executable('hello', 'hello.c')
END
show() {
  meson introspect --buildoptions build | python3 -c '
import json, sys
o = {x["name"]: x["value"] for x in json.load(sys.stdin)}
print("buildtype =", o["buildtype"], "| optimization =", o["optimization"], "| debug =", o["debug"])'
}
meson setup build >/dev/null 2>&1
echo "--- after meson setup build"
show
meson configure build -Dbuildtype=release >/dev/null 2>&1
echo "--- after meson configure build -Dbuildtype=release"
show
echo "--- a typo in an option name"
meson configure build -Dbuildtyp=debug 2>&1 | grep -i error
cd /
rm -rf "${d:?}"
```

```output for=meson-options
--- after meson setup build
buildtype = debug | optimization = 0 | debug = True
--- after meson configure build -Dbuildtype=release
buildtype = release | optimization = 3 | debug = False
--- a typo in an option name
ERROR: Unknown options: "buildtyp"
```

**Many build directories.** Because Meson never writes into the source directory, one source tree can have many build directories, for example one for each compiler or build type.[^2] To use Clang in one of them, set `CC=clang` when you run `meson setup` for it.[^2]

## Alternatives

- [[cmake]] also generates files for [[ninja]] and for IDEs. Its language is larger and its use is wider.
- [[autotools]] generates a `configure` script and makefiles. The Meson FAQ says that Meson does not expose Python in `meson.build`, so that Meson itself can move to a different language if Python becomes too slow. It says that this problem caused complications for GNU Autotools and SCons.[^1]
- [[make]] with a makefile written by hand. Meson has no back end for Make. Its FAQ says: "Because Make is slow."[^1]

## History and versions

Jussi Pakkanen started Meson as an experiment in the Christmas holidays of 2012.[^10][^11] His design rationale gives the requirements: the syntax must be clean, the defaults must suit the developer, keep the source and build directories apart, support Visual Studio and Xcode natively, and be fast.[^10] It also explains why Meson uses Ninja in place of Make on Unix: "It is not uncommon on large code bases for Make to take several minutes just to determine that nothing needs to be done."[^10] The first release came out on 2 March 2013.[^11]

The name comes from physics. The FAQ says that the name could not be the name of a Debian package or of a Sourceforge project. Gluon was taken, and Meson, the name of another subatomic particle, was free.[^1]

GNOME made it a goal to port its projects to Meson.[^11] The Users page lists GTK, GLib, GStreamer, the X.org server, FreeType and GIMP among the projects that build with Meson.[^12] Meson 1.3.2 needs Python 3.7 or later and Ninja 1.8.2 or later.[^5]

## References
