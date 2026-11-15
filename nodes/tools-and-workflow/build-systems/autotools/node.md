---
tag: KW7FR0
kind: tool
slug: autotools
name: Autotools
aliases: [GNU Autotools, autoconf, automake, libtool, GNU Build System, configure script, configure.ac, Makefile.am, autoreconf, ./configure && make]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  The GNU Autotools are Autoconf, Automake and Libtool. They generate a portable configure script and makefiles, so that a user builds a package with ./configure && make && make install.
role: [build]
homepage: https://www.gnu.org/software/autoconf/
history:
  year: 1991
  people: [David J. MacKenzie, Tom Tromey, Gordon Matzigkeit]
  note: David J. MacKenzie wrote the first configure script in June 1991 and released Autoconf 1.0 in July 1992. He started Automake in 1994, and Tom Tromey took it over in 1995. Gordon Matzigkeit started Libtool in 1996.
see_also: [make, write-a-makefile, cmake, meson, build-system-concept, build-a-project]
sources:
  - url: https://www.gnu.org/software/autoconf/manual/autoconf.html
    title: Autoconf manual, version 2.73
    locator: 1 Introduction; 2 The GNU Build System; 3 Making configure Scripts; AC_ARG_ENABLE; AC_CHECK_PROG; AC_SUBST; AC_CONFIG_FILES; AC_OUTPUT; autoreconf; 20.1 Distributing configure Scripts; 21 History of Autoconf
  - url: https://www.gnu.org/software/automake/manual/automake.html
    title: GNU Automake manual
    locator: 1 Introduction; 2.1 Introducing the GNU Build System; 2.2 Use Cases for the GNU Build System; 2.3 How Autotools Help; 2.4 A Small Hello World
  - url: https://www.gnu.org/software/libtool/manual/libtool.html
    title: GNU Libtool manual
    locator: 1 Introduction; 3.3.1 Wrapper executables for uninstalled programs
  - url: https://www.gnu.org/software/autoconf/
    title: GNU Autoconf home page
    locator: Introduction to Autoconf
  - url: https://mesonbuild.com/Design-rationale.html
    title: The Meson Build System, Design rationale
    locator: the introduction (running a program built with GNU Autotools under GDB)
  - url: https://www.gnu.org/software/automake/history/automake-history.html
    title: Brief History of Automake
    locator: Timeline (1994-09-19, 1995-11-12, 1996-03-15)
---

## What it is

The GNU Autotools are three tools that make the build system of a package portable across Unix-like systems:

- **Autoconf** makes a shell script `configure` from a file `configure.ac`. The Autoconf manual says that Autoconf "is a tool for producing shell scripts that automatically configure software source code packages to adapt to many kinds of POSIX-like systems."[^1] The script tests the system for each feature that the package needs, and it writes the makefiles and a header file with the results.[^1]
- **Automake** makes a template `Makefile.in` from a file `Makefile.am`. A `Makefile.am` is mostly a list of variable definitions, and the generated `Makefile.in` has the standard targets of the GNU Coding Standards.[^2]
- **Libtool** builds shared libraries in the same way on each platform. Its manual says that it puts both the code for each platform and the user interface "in a single script".[^3]

Together they make what the Automake manual calls the GNU Build System.[^2] A user who builds a package unpacks it and runs `./configure && make && make install`.[^2] The user does not need the Autotools for this: the scripts that Autoconf makes "are independent of Autoconf when they are run", and the packages that Automake makes do not need Perl to build.[^1][^2] Only the maintainer of the package runs the Autotools.

## Install and first use

Most Linux distributions have packages for `autoconf`, `automake` and `libtool`. Autoconf needs GNU M4.[^4] The examples on this page ran with Autoconf 2.71, Automake 1.16.5 and GCC 13 on Ubuntu 24.04. The Autoconf manual that this page cites is the manual of version 2.73.

The first run recreates the package `amhello` from section 2.4 of the Automake manual.[^2] The maintainer writes five files: the source file `src/main.c`, a `README`, a `Makefile.am` in each of the two directories, and `configure.ac`. Then `autoreconf --install` runs the Autotools.[^2] The run shows the files that it makes, and the options that `./configure --help` gives for the installation directory and for silent rules.

```bash run id=autotools-amhello
d=$(mktemp -d)
cd "$d"
mkdir src
cat >src/main.c <<'END'
#include <config.h>
#include <stdio.h>

int
main (void)
{
  puts ("Hello World!");
  puts ("This is " PACKAGE_STRING ".");
  return 0;
}
END
echo 'This is a demonstration package.' >README
cat >src/Makefile.am <<'END'
bin_PROGRAMS = hello
hello_SOURCES = main.c
END
cat >Makefile.am <<'END'
SUBDIRS = src
dist_doc_DATA = README
END
cat >configure.ac <<'END'
AC_INIT([amhello], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_OUTPUT
END
echo "--- autoreconf --install"
autoreconf --install 2>&1
ls configure config.h.in Makefile.in src/Makefile.in
echo "--- ./configure --help"
./configure --help | grep -E '^ +--(prefix|enable-silent-rules)[ =]'
cd /
rm -rf "${d:?}"
```

```output for=autotools-amhello
--- autoreconf --install
configure.ac:3: installing './compile'
configure.ac:2: installing './install-sh'
configure.ac:2: installing './missing'
src/Makefile.am: installing './depcomp'
Makefile.in
config.h.in
configure
src/Makefile.in
--- ./configure --help
  --prefix=PREFIX         install architecture-independent files in PREFIX
  --enable-silent-rules   less verbose build output (undo: "make V=1")
```

`autoreconf` copies the helper scripts `install-sh`, `missing`, `compile` and `depcomp` into the package. It also makes `configure` and three templates: `config.h.in`, `Makefile.in` and `src/Makefile.in`. When the user runs `./configure`, it makes `config.h`, `Makefile` and `src/Makefile` from these templates.[^2] After that, `make` builds `src/hello`, and the program prints `Hello World!` and `This is amhello 1.0.`.[^2]

The files that the maintainer writes are short:

- `configure.ac` has a list of M4 macros. `AC_INIT` gives the name of the package, its version and an address for bug reports. `AM_INIT_AUTOMAKE` starts Automake, and `AC_PROG_CC` makes `configure` find a C compiler. `AC_CONFIG_HEADERS` and `AC_CONFIG_FILES` give the files that `configure` makes, and `AC_OUTPUT` makes them.[^2] The macros that start with `AC_` belong to Autoconf, and the macros that start with `AM_` belong to Automake.[^2]
- `src/Makefile.am` has `bin_PROGRAMS = hello` and `hello_SOURCES = main.c`. From the variable `bin_PROGRAMS`, Automake makes the rules that compile and link the program.[^2]

## Concepts of the tool

**Feature tests, not system lists.** A `configure` script does not look up the name of the system in a table. It tests for each feature, and before each test it prints a line that says what it checks.[^1] David MacKenzie, who wrote Autoconf, explains this choice in the history of Autoconf: to keep a database of the features of each variant of each system up to date was "a hopeless cause", and it was more reliable to check for most features when `configure` runs.[^1]

**Templates and output variables.** `configure` makes each output file from a template, such as `Makefile` from `Makefile.in`.[^1] `AC_SUBST` makes a shell variable an output variable, and `AC_OUTPUT` replaces each `@variable@` in the templates with its value.[^1] `AC_ARG_ENABLE` adds an option `--enable-feature` to `configure`, and `AC_CHECK_PROG` checks for a program in the `PATH`.[^1] The run below uses only Autoconf. It writes a `configure.ac` with an option `--enable-shout` and a `Makefile.in`, runs `autoconf`, and runs `configure` twice with different options.

```bash run id=autoconf-substitution
d=$(mktemp -d)
cd "$d"
cat >configure.ac <<'END'
AC_INIT([greet], [1.0])
AC_ARG_ENABLE([shout],
  [AS_HELP_STRING([--enable-shout], [print the greeting in capitals])],
  [], [enable_shout=no])
AC_CHECK_PROG([HAVE_SH], [sh], [yes], [no])
AC_SUBST([SHOUT], [$enable_shout])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
END
printf 'prefix = @prefix@\nexec_prefix = @exec_prefix@\nbindir = @bindir@\nSHOUT = @SHOUT@\n\nshow:\n\t@echo "bindir=$(bindir) shout=$(SHOUT)"\n' >Makefile.in
echo "--- autoconf"
autoconf 2>&1
echo "configure has $(wc -l <configure) lines"
echo "--- ./configure"
./configure 2>&1
make show
echo "--- ./configure --prefix=/opt/greet --enable-shout"
./configure --prefix=/opt/greet --enable-shout >/dev/null 2>&1
make show
grep -E '^(SHOUT|bindir) ' Makefile
echo "--- ./configure --help"
./configure --help | grep -E '^ +--(prefix|enable-shout)'
cd /
rm -rf "${d:?}"
```

```output for=autoconf-substitution
--- autoconf
configure has 2932 lines
--- ./configure
checking for sh... yes
configure: creating ./config.status
config.status: creating Makefile
bindir=/usr/local/bin shout=no
--- ./configure --prefix=/opt/greet --enable-shout
bindir=/opt/greet/bin shout=yes
bindir = ${exec_prefix}/bin
SHOUT = yes
--- ./configure --help
  --prefix=PREFIX         install architecture-independent files in PREFIX
  --enable-shout          print the greeting in capitals
```

The second `configure` writes `/opt/greet` into the variable `prefix`, and `bindir` follows from it. The small `configure.ac` gives a `configure` script with more than 2900 lines, because each macro expands to portable shell code.[^1]

**The files that `configure` makes.** `configure` makes the makefiles, an optional C header file with `#define` lines, a script `config.status` that can make the same files again, and a file `config.log` with the output of the compilers for debugging.[^1] With the option `--config-cache`, it also writes `config.cache` with the results of its tests.[^1]

**Shared libraries with Libtool.** When a program links to a shared library in the same package, Libtool puts the real program in the hidden directory `.libs` and writes a wrapper script in its place. The wrapper script sets up the environment, so that the program finds the library before it is installed.[^3] To debug such a program, run `libtool --mode=execute gdb hell`, and not `gdb hell`, because the debugger cannot read the wrapper script.[^3] The design rationale of Meson gives this behavior as an example of the problems of build systems.[^5]

## Common tasks

| Task | Command |
|---|---|
| Make `configure` and the templates from a checkout | `autoreconf --install`[^1][^2] |
| Configure, build and install | `./configure && make && make install`[^2] |
| Install below another prefix | `./configure --prefix ~/usr`[^2] |
| Build outside the source tree | `mkdir build && cd build && ../configure && make`[^2] |
| Run the tests | `make check`[^2] |
| Make a source tarball | `make dist`, or `make distcheck` to also check it[^2] |
| Install into a staging directory for a package | `make DESTDIR=$HOME/inst install`[^2] |

**Standard targets.** The GNU Coding Standards define the targets of the makefiles, and Automake makes all of them: `all`, `install`, `uninstall`, `clean`, `distclean`, `check`, `installcheck` and `dist`.[^2] `make distclean` also removes the files that `./configure` made.[^2]

**Build trees.** The source tree is the directory that holds `configure`, and the build tree is the directory where `configure` ran. When `configure` runs in its own directory, the two trees are the same, and the build writes its files next to the sources.[^2] Run `../configure` from another directory to keep all the built files apart. The Automake manual calls these "Parallel Build Trees (a.k.a. VPATH Builds)".[^2]

## Configuration

**Options of `configure`.** The user configures a package with options of `configure`. `--prefix` sets the directory where `make install` puts the files, and the default is `/usr/local`.[^2] The GNU Coding Standards define directory variables such as `bindir`, which is `${exec_prefix}/bin` by default.[^2] Options such as `--enable-feature` and `--with-package` come from the `configure.ac` of each package.[^1] `./configure --help` lists all of them.

**Cross compilation.** `--build` gives the system that does the build, and `--host` gives the system where the built programs will run.[^2] With `--host`, `configure` looks for tools whose names start with the name of the host system, such as `i586-mingw32msvc-gcc`.[^2]

**Variables for the build.** The user can set variables such as `CC` and `CFLAGS` on the command line of `configure`, as in `./configure CC=gcc-3`.[^2] `DESTDIR` makes `make install` copy the files into a staging directory, so that a package manager can see which files the package installs.[^2]

## Alternatives

- [[cmake]] and [[meson]] describe the build in their own language and generate the files for a build tool such as [[ninja]]. They also support Windows and IDE projects directly.
- A [[make|makefile written by hand]] is enough for a package that runs on a small number of systems. The Automake manual says that it is possible to write a GNU Build System by hand, "but it is rather burdensome and error-prone".[^2]

The Automake manual also lists reasons not to use the Autotools: a project may already use another build system, and "People who have their own idea of how a build system should work will feel frustrated by the Autotools."[^2]

## History and versions

In June 1991, David J. MacKenzie maintained many GNU utilities. The number of `-D` options that users had to select in the makefiles, about 20, had become a burden, so he wrote a shell script that guessed some of the settings for the package fileutils.[^1] The Automake manual describes the same event: he "got tired of customizing Makefile for the 20 platforms he had to deal with".[^2] As he adapted more packages, it became impractical to update all the scripts by hand, so he wrote a tool that generates them from M4 macros. He first named it Autoconfig, but the name was too long for old Unix file systems.[^1] He released Autoconf 1.0 in July 1992.[^1] Roland McGrath wrote the script `autoheader`, which makes the template for the C header file.[^1]

David MacKenzie also started Automake: its first commit in the version control system is from 19 September 1994.[^6] Tom Tromey took over Automake in November 1995, after a year with no activity.[^6] Gordon Matzigkeit started to write Libtool in March 1996.[^6]

The Autotools are still common, for two reasons that come from their design. The user of a package needs only a shell and `make`, because the generated `configure` script and makefiles do not need the Autotools.[^1][^2] Also, the interface `./configure && make && make install` is the same in many packages, and the Automake manual says that the result is a build system that makes all packages "almost indistinguishable by the installer".[^2] The Autoconf manual also says that there are no restrictions on how the `configure` scripts that it makes may be distributed or used.[^1]

## References
