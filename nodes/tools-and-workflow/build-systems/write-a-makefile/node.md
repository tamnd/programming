---
tag: RYM1YZ
kind: task
slug: write-a-makefile
name: Write a Makefile
aliases: [write a makefile, makefile for a C project, makefile with build test and clean targets, phony targets, make clean, make test]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Write a Makefile that builds a small project, runs its tests and removes the build outputs, with phony targets for the actions and correct dependencies on the header files.
see_also: [make, build-a-project, build-graph, incremental-builds, build-system-concept, cmake]
sources:
  - url: https://www.gnu.org/software/make/manual/make.html
    title: GNU Make manual, edition 0.77 for GNU make version 4.4.1, 26 February 2023
    locator: 2.1 What a Rule Looks Like; 3.3 Including Other Makefiles; 4.3 Types of Prerequisites; 4.6 Phony Targets; 4.14 Generating Prerequisites Automatically; 6.5 Setting Variables; 10.5.3 Automatic Variables; 16.6 Standard Targets for Users
  - url: https://gcc.gnu.org/onlinedocs/gcc-13.3.0/gcc/Preprocessor-Options.html
    title: GCC 13.3 manual, Options Controlling the Preprocessor
    locator: -MD; -MMD; -MP
  - url: https://docs.python.org/3/library/zipapp.html
    title: Python documentation, zipapp, Manage executable Python zip archives
    locator: Command-Line Interface (-m and -o)
  - url: https://docs.python.org/3/library/unittest.html
    title: Python documentation, unittest, Unit testing framework
    locator: Test Discovery (-s, --start-directory)
---

## Task

Write a `Makefile` for a small project, so that these commands work:

- `make` builds the program, and a second `make` with no changes does nothing.
- `make test` builds the tests and runs them.
- `make clean` removes all the files that the build made.

A change to any source file or header file must rebuild the files that depend on it, and only those. The build outputs go into a directory `build`, away from the sources.

The first solution is a C project with a header file, the most common use of a makefile. The second solution uses Make as a task runner for a Python project. Each solution is a shell script that makes a temporary directory, writes the project files into it, runs `make` with GNU Make 4.3 on Linux, and removes the directory. The page [[make]] explains the parts of a makefile.

## Solutions

| Part | C project | Python project |
| --- | --- | --- |
| Default goal `all` | links `build/calc` | makes `build/calc.pyz` with `zipapp` |
| `test` | builds and runs `build/test_calc` | runs `python3 -m unittest` |
| `clean` | `rm -rf build` | `rm -rf build` |
| Header dependencies | written by the compiler with `-MMD -MP` | none |

**C.** The project has `src/calc.c`, `src/calc.h`, `src/main.c` and a test program `tests/test_calc.c`. The makefile does these things:

- `.PHONY: all test clean` declares the three actions as phony targets, so that a file with one of these names does not stop the recipe.[^1]
- `all` is the first target, so it is the default goal. The GNU coding standards in the GNU Make manual say that `all` should "Compile the entire program" and that it should be the default target.[^1] They also say that the rules should compile with `-g` by default, so that the programs have debugging symbols.[^1] `CFLAGS ?= -g -O2 -Wall` sets this default, and the user can change it with `make CFLAGS=-O0`.
- Two pattern rules compile `src/%.c` and `tests/%.c` into `build/%.o`. In the recipes, `$@` is the target, `$<` is the first prerequisite and `$^` is all the prerequisites.[^1]
- `| $(BUILD)` makes the directory `build` an order-only prerequisite. Make creates it before it compiles, but a change to the directory does not make the object files out of date, because "Order-only prerequisites are never checked when determining if the target is out of date".[^1]
- `-MMD` makes GCC write a file `build/calc.d` next to each object file, with the user header files that the source includes. `-MMD` is "Like -MD except mention only user header files, not system header files", and `-MD` writes the dependency file "as a side effect of the compilation process".[^2] `-MP` adds a phony target for each header, which works around the errors that make gives if you remove a header file without updating the makefile.[^2]
- `-include $(wildcard $(BUILD)/*.d)` reads these files. The `-` makes Make ignore a file that does not exist, with no error.[^1] On the first build there are no `.d` files, and Make does not need them, because it must compile all the files anyway.

```bash run id=c-makefile
d=$(mktemp -d)
cd "$d"
mkdir src tests
cat >src/calc.h <<'END'
int add(int a, int b);
int mul(int a, int b);
END
cat >src/calc.c <<'END'
#include "calc.h"
int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
END
cat >src/main.c <<'END'
#include <stdio.h>
#include "calc.h"
int main(void) { printf("2 + 3 = %d, 2 * 3 = %d\n", add(2, 3), mul(2, 3)); return 0; }
END
cat >tests/test_calc.c <<'END'
#include <stdio.h>
#include "calc.h"
int main(void) {
    int failed = (add(2, 3) != 5) + (mul(2, 3) != 6);
    printf("%d of 2 tests failed\n", failed);
    return failed != 0;
}
END
cat >Makefile <<'END'
CFLAGS ?= -g -O2 -Wall
CPPFLAGS += -Isrc -MMD -MP
BUILD := build

.PHONY: all test clean
all: $(BUILD)/calc

$(BUILD)/calc: $(BUILD)/main.o $(BUILD)/calc.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/test_calc: $(BUILD)/test_calc.o $(BUILD)/calc.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD)/%.o: tests/%.c | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

test: $(BUILD)/test_calc
	$(BUILD)/test_calc

clean:
	rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d)
END
echo "--- make"
make 2>&1
build/calc
echo "--- make test"
make test 2>&1
echo "--- make again"
make 2>&1
echo "--- after touch src/calc.h"
touch src/calc.h
make 2>&1
echo "--- make clean"
make clean 2>&1
cd /
rm -rf "${d:?}"
```

```output for=c-makefile
--- make
mkdir -p build
cc -Isrc -MMD -MP -g -O2 -Wall -c -o build/main.o src/main.c
cc -Isrc -MMD -MP -g -O2 -Wall -c -o build/calc.o src/calc.c
cc  -o build/calc build/main.o build/calc.o
2 + 3 = 5, 2 * 3 = 6
--- make test
cc -Isrc -MMD -MP -g -O2 -Wall -c -o build/test_calc.o tests/test_calc.c
cc  -o build/test_calc build/test_calc.o build/calc.o
build/test_calc
0 of 2 tests failed
--- make again
make: Nothing to be done for 'all'.
--- after touch src/calc.h
cc -Isrc -MMD -MP -g -O2 -Wall -c -o build/main.o src/main.c
cc -Isrc -MMD -MP -g -O2 -Wall -c -o build/calc.o src/calc.c
cc  -o build/calc build/main.o build/calc.o
--- make clean
rm -rf build
```

After `touch src/calc.h`, Make compiles `main.c` and `calc.c` again, because the `.d` files list `src/calc.h` as a prerequisite of `main.o` and `calc.o`. The GNU Make manual explains why you should not write these rules by hand: you "must always be very careful to update the makefile every time you add or remove an #include".[^1]

**Python.** Python has no compile step to describe, but a makefile can still give a project the same three commands. Here `all` makes a zip application with `python3 -m zipapp app -m calc:main -o build/calc.pyz`. The `-m` option writes a `__main__.py` that calls `calc.main`, and `-o` names the output file.[^3] The rule lists the Python files of the application as prerequisites, without the test files, so a change to a test does not rebuild the archive. `test` runs `python3 -m unittest discover -s app`, where `-s` is the directory where discovery starts.[^4] In the run, a `sed` command removes the run time from the last line of the test output.

```bash run id=python-makefile
d=$(mktemp -d)
cd "$d"
mkdir app
cat >app/calc.py <<'END'
def add(a, b):
    return a + b


def main():
    print("2 + 3 =", add(2, 3))
END
cat >app/test_calc.py <<'END'
import unittest

from calc import add


class AddTest(unittest.TestCase):
    def test_add(self):
        self.assertEqual(add(2, 3), 5)
END
cat >Makefile <<'END'
PYTHON ?= python3
BUILD := build
sources := $(filter-out app/test_%.py,$(wildcard app/*.py))

.PHONY: all test clean
all: $(BUILD)/calc.pyz

$(BUILD)/calc.pyz: $(sources) | $(BUILD)
	$(PYTHON) -m zipapp app -m calc:main -o $@

$(BUILD):
	mkdir -p $@

test:
	$(PYTHON) -m unittest discover -s app

clean:
	rm -rf $(BUILD)
END
echo "--- make"
make 2>&1
python3 build/calc.pyz
echo "--- make test"
make test 2>&1 | sed -E 's/ in [0-9.]+s$//'
echo "--- make again"
make 2>&1
echo "--- after touch app/test_calc.py"
touch app/test_calc.py
make 2>&1
cd /
rm -rf "${d:?}"
```

```output for=python-makefile
--- make
mkdir -p build
python3 -m zipapp app -m calc:main -o build/calc.pyz
2 + 3 = 5
--- make test
python3 -m unittest discover -s app
.
----------------------------------------------------------------------
Ran 1 test

OK
--- make again
make: Nothing to be done for 'all'.
--- after touch app/test_calc.py
make: Nothing to be done for 'all'.
```

## Edge cases

- **A file named like a phony target.** Without `.PHONY`, a file called `clean` or `test` in the directory makes Make say that the target is up to date, and the recipe does not run.[^1]
- **A missing build directory.** The order-only prerequisite `| $(BUILD)` creates `build` before the first compile. A normal prerequisite on the directory would be wrong, because the time stamp of a directory changes whenever a file in it is added, removed or renamed, and Make would then rebuild all the targets.[^1]
- **Other names for the test target.** The GNU coding standards call the target that runs the tests `check`, and they describe it as "Perform self-tests (if any)".[^1] Many other projects use `test`. A makefile can have both, with `check: test`.
- **Settings from the user.** `?=` sets a variable only if it is not yet defined, so a value from the environment or from the command line wins.[^1] Use `+=` to add flags that the build always needs, such as `-Isrc` in `CPPFLAGS`.

## Pitfalls

- **Spaces in place of a tab.** A recipe line must start with a tab.[^1] With spaces, GNU Make stops with the error `missing separator` before it runs any recipe. The run below shows the error and then the same makefile with a tab.

```bash run id=tab-error
d=$(mktemp -d)
cd "$d"
printf 'hello:\n    echo hello\n' >Makefile
make 2>&1
echo "exit status: $?"
printf 'hello:\n\techo hello\n' >Makefile
make 2>&1
echo "exit status: $?"
cd /
rm -rf "${d:?}"
```

```output for=tab-error
Makefile:2: *** missing separator.  Stop.
exit status: 2
echo hello
hello
exit status: 0
```

- **Headers that are not prerequisites.** If the makefile does not list a header, a change to it does not rebuild the object files, and the program can link old code with changed declarations. Let the compiler write the dependencies, as in the C solution.
- **A recipe that does not make its target.** A rule such as `test:` with no file named `test` runs each time. This is correct for a phony target, but for a file target it means that the recipe writes a different file name than the target. Use `$@` in the recipe, so that the recipe writes the file that the rule names.[^1]
- **A clean target that deletes too much.** The GNU coding standards say that `clean` should delete the files that the build normally makes, but not the files that record the configuration.[^1] With all outputs in one directory, `rm -rf build` cannot remove a source file.

## Related tasks

- [[build-a-project]]: build a small project with the usual build tool of each language.

## References
