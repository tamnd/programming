---
tag: T78G2P
kind: tool
slug: bazel
name: Bazel
aliases: [bazel, Bazel build, Blaze, BUILD.bazel, MODULE.bazel, Bzlmod, bazel build, bazel test, bazel query]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Bazel is the open-source form of Google's build system. Targets are declared in BUILD files in Starlark, and Bazel runs each action from declared inputs, with caching, sandboxes and remote execution.
role: [build, test]
licence: apache-2.0
repo: https://github.com/bazelbuild/bazel
homepage: https://bazel.build/
history:
  year: 2015
  people: []
  note: Bazel is the open-source form of Blaze, the build tool that Google uses inside the company. Google made it open source in March 2015 and announced the Beta release on 1 September 2015.
see_also: [build-graph, hermetic-builds, remote-caching-and-execution, other-monorepo-build-tools, cmake, ninja, make, build-systems-a-la-carte]
sources:
  - url: https://bazel.build/about/intro
    title: Bazel documentation, Intro to Bazel
    locator: the first paragraph; Benefits
  - url: https://bazel.build/about/faq
    title: Bazel documentation, FAQ
    locator: Where did Bazel come from?; Why did Google build Bazel?; What should I not use Bazel for?; Will Bazel make my builds reproducible automatically?; Why do other Google projects use other build tools?; How do you pronounce Bazel?
  - url: https://bazel.build/concepts/build-files
    title: Bazel documentation, BUILD files
    locator: the introduction (Starlark, no def, for or if, no arbitrary I/O)
  - url: https://github.com/bazelbuild/bazel/blob/9.2.0/LICENSE
    title: Bazel licence, tag 9.2.0
    locator: the whole file (Apache License 2.0)
  - url: https://github.com/bazelbuild/bazel/blob/9.2.0/README.md
    title: Bazel README, tag 9.2.0
    locator: the title and the tag line
  - url: https://bazel.build/about/why
    title: Bazel documentation, Why Bazel?
    locator: Bazel is fast; Bazel is correct; Bazel is extensible
  - url: https://bazel.build/start/cpp
    title: Bazel Tutorial, Build a C++ Project
    locator: Stage 1 (bazel build, bazel-bin); Stage 3 (lib/BUILD, main/BUILD, visibility)
  - url: https://bazel.build/run/build
    title: Bazel documentation, Build programs with Bazel
    locator: Quickstart (the commands); Phases of a build (loading, analysis, execution)
  - url: https://bazel.build/concepts/build-ref
    title: Bazel documentation, Repositories, workspaces, packages, and targets
    locator: Repositories; Packages; Targets
  - url: https://bazel.build/concepts/labels
    title: Bazel documentation, Labels
    locator: canonical and apparent repo names; the short form of a label
  - url: https://bazel.build/rules/language
    title: Bazel documentation, Starlark Language
    locator: Syntax; Differences with Python
  - url: https://bazel.build/remote/caching
    title: Bazel documentation, Remote Caching
    locator: Overview (actions, the cache of action outputs)
  - url: https://bazel.build/remote/rbe
    title: Bazel documentation, Remote Execution Overview
    locator: the introduction (gRPC protocol, benefits)
  - url: https://bazel.build/basics/hermeticity
    title: Bazel documentation, Hermeticity
    locator: the definition; Isolation
  - url: https://bazel.build/concepts/platforms
    title: Bazel documentation, Platforms
    locator: the host, execution and target roles
  - url: https://bazel.build/query/guide
    title: Bazel documentation, Query guide
    locator: deps; somepath and allpaths
  - url: https://bazel.build/external/overview
    title: Bazel documentation, External dependencies overview
    locator: Overview of the system; the Bzlmod and WORKSPACE notes
  - url: https://bazel.build/external/module
    title: Bazel documentation, Bazel modules
    locator: the MODULE.bazel example; Version selection (Minimal Version Selection)
  - url: https://buck2.build/docs/about/why/
    title: Buck2 documentation, Why Buck2
    locator: Why does Buck2 exist? (the open-source dates of Buck and Bazel)
  - url: https://blog.bazel.build/2015/09/01/beta-release.html
    title: Bazel Blog, Bazel Builder Blasts Beyond Beta Barrier (1 September 2015)
    locator: the first two paragraphs
---

## What it is

Bazel is a build and test tool. Its documentation describes it as "an open-source build and test tool similar to Make, Maven, and Gradle".[^1] The FAQ says that Bazel "is a flavor of the tool that Google uses to build its server software internally", and that Bazel shares most of its code with that internal tool.[^2] Inside Google the tool has the name Blaze.[^2] Before it, Google built its software with "large, generated Makefiles", and these gave slow and unreliable builds.[^2]

A project describes its software in BUILD files, as targets such as libraries, programs and tests. The BUILD files use the language Starlark.[^3] Bazel reads these files, makes a [[build-graph]] of the targets, and runs the compile, link and test steps itself. It does not write files for another build tool, as [[cmake]] does. Bazel works with many languages and platforms, and it is made for large code bases in one repository or in many.[^1] It is free software under the Apache License 2.0.[^4]

The README gives the aim of Bazel in one line: "{Fast, Correct} - Choose two".[^5] Bazel knows which input files each build command needs, and it runs a command again only when these inputs change.[^6] It runs each action in a sandbox and tracks every input file of the build, so that "the same source code always results in the same binary".[^6]

## Install and first use

The Bazel documentation gives install steps for Ubuntu, macOS, Windows and other systems.[^7] The command `bazel help` lists the commands.[^8]

The build machine of this site has no Bazel, so the examples on this page were not run with Bazel. The BUILD files below are from stage 3 of the Bazel C++ tutorial, and the commands are shown as the tutorial gives them.[^7] A Python model after them shows how Bazel reads the files. The model ran with Python 3.12.

The project has a file `MODULE.bazel` at the root, a package `lib` and a package `main`.[^7] A package is a directory with a file named `BUILD` or `BUILD.bazel`, and it includes all the files below it, except the directories that have their own BUILD file.[^9] This is `lib/BUILD`:

```starlark
cc_library(
    name = "hello-time",
    srcs = ["hello-time.cc"],
    hdrs = ["hello-time.h"],
    visibility = ["//main:__pkg__"],
)
```

This is `main/BUILD`:

```starlark
cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
        "//lib:hello-time",
    ],
)
```

The command `bazel build //main:hello-world` builds the program. In the label, `//main` is the location of the BUILD file from the root of the workspace, and `hello-world` is the name of the target.[^7] Bazel puts the outputs in the directory `bazel-bin` at the root of the workspace, so the program is `bazel-bin/main/hello-world`.[^7]

The target `hello-world` uses `//lib:hello-time` from another package. The tutorial says that "by default targets are only visible to other targets in the same BUILD file".[^7] The attribute `visibility = ["//main:__pkg__"]` in `lib/BUILD` lets the targets in the package `main` use `hello-time`, and without it the build does not succeed.[^7]

The model below is not Bazel. These two BUILD files use only the part of Starlark that is also Python, so the model runs them as Python, with two functions that record each call to `cc_library` and `cc_binary`. It then gives the label of each target, puts the dependencies of `//main:hello-world` before the target, and checks the visibility of each dependency. Last, it removes the `visibility` line from `lib/BUILD` and tries again.

```python run id=bazel-build-files
# A small model of how Bazel loads BUILD files. It is not Bazel.
LIB_BUILD = """
cc_library(
    name = "hello-time",
    srcs = ["hello-time.cc"],
    hdrs = ["hello-time.h"],
    visibility = ["//main:__pkg__"],
)
"""

MAIN_BUILD = """
cc_library(
    name = "hello-greet",
    srcs = ["hello-greet.cc"],
    hdrs = ["hello-greet.h"],
)

cc_binary(
    name = "hello-world",
    srcs = ["hello-world.cc"],
    deps = [
        ":hello-greet",
        "//lib:hello-time",
    ],
)
"""


def load(build_files):
    targets = {}
    for pkg, text in build_files.items():
        def rule(kind, pkg=pkg):
            def declare(name, srcs=(), hdrs=(), deps=(), visibility=()):
                label = "//%s:%s" % (pkg, name)
                deps = ["//%s%s" % (pkg, d) if d.startswith(":") else d for d in deps]
                targets[label] = dict(kind=kind, pkg=pkg, files=list(srcs) + list(hdrs),
                                      deps=deps, visibility=list(visibility))
            return declare
        names = {"__builtins__": {}, "cc_library": rule("cc_library"), "cc_binary": rule("cc_binary")}
        exec(text, names)
    return targets


def visible(target, pkg):
    vis = target["visibility"]
    return target["pkg"] == pkg or "//visibility:public" in vis or "//%s:__pkg__" % pkg in vis


def build_order(targets, label, order, path=()):
    if label in order:
        return
    for dep in targets[label]["deps"]:
        if dep not in targets:
            raise LookupError("%s: no such target %s" % (label, dep))
        if not visible(targets[dep], targets[label]["pkg"]):
            raise PermissionError("%s: target %s is not visible" % (label, dep))
        build_order(targets, dep, order)
    order.append(label)


targets = load({"lib": LIB_BUILD, "main": MAIN_BUILD})
for label in sorted(targets):
    t = targets[label]
    print("%-20s %-11s %s" % (label, t["kind"], " ".join(t["files"])))
order = []
build_order(targets, "//main:hello-world", order)
print("build order:", " -> ".join(order))

print("--- lib/BUILD without the visibility attribute")
targets = load({"lib": LIB_BUILD.replace('    visibility = ["//main:__pkg__"],\n', ""), "main": MAIN_BUILD})
try:
    build_order(targets, "//main:hello-world", [])
except PermissionError as e:
    print("error:", e)
```

```output for=bazel-build-files
//lib:hello-time     cc_library  hello-time.cc hello-time.h
//main:hello-greet   cc_library  hello-greet.cc hello-greet.h
//main:hello-world   cc_binary   hello-world.cc
build order: //main:hello-greet -> //lib:hello-time -> //main:hello-world
--- lib/BUILD without the visibility attribute
error: //main:hello-world: target //lib:hello-time is not visible
```

The model gives one order. Bazel runs the actions of independent targets in parallel, on one computer or on remote machines.[^6]

## Concepts of the tool

**Repositories, packages and targets.** A repository is a directory tree with a boundary marker file at its root, such as `MODULE.bazel`, `REPO.bazel`, or the legacy `WORKSPACE`.[^9] A package is a container of targets, and the targets are declared in the BUILD file of the package.[^9] Most targets are files or rules. Source files are checked in, and generated files are made by the build.[^9] Each rule gives the relation between a set of input files and a set of output files, and the inputs can be the outputs of other rules.[^9]

**Labels.** A label names a target. `//my/app/main:app_binary` is the target `app_binary` in the package `my/app/main`, and `//my/app/lib` is short for `//my/app/lib:lib`.[^10] A label can start with a repository name: `@myrepo//...` uses an apparent repo name, and `@@myrepo//...` uses a canonical repo name, which is unique in the workspace.[^10] In the commands, target patterns name many targets at once: `//foo/...` is all the rule targets in all the packages below the directory `foo`, and `//foo/bar:all` is all the rule targets in the package `foo/bar`.[^8]

**Starlark.** Starlark was formerly called Skylark, and its syntax comes from Python 3.[^11] It differs from Python in some ways. For example, global variables are immutable and "Recursion is not allowed."[^11] A BUILD file "cannot contain function definitions, for statements or if statements", but list comprehensions and `if` expressions are allowed.[^3] Functions, rules and macros go in `.bzl` files, and `load("//foo/bar:file.bzl", "some_library")` adds the symbol `some_library` from such a file.[^3] Starlark code cannot do arbitrary input and output, so a BUILD file depends only on a known set of inputs. The Bazel documentation calls this hermetic, and it makes the builds reproducible.[^3]

**The phases of a build.** In the loading phase, Bazel loads, parses and evaluates the BUILD files of the targets and of all their dependencies.[^8] In the analysis phase, it checks each rule, makes the dependency graph, and finds the work that each step must do.[^8] In the execution phase, it runs the compilers, linkers and other tools as necessary so that each output agrees with its inputs.[^8] Bazel keeps the dependency graph from one build to the next, and after a change it analyzes again only what it must.[^8]

**Actions and caches.** Bazel divides a build into actions. "Each action has inputs, output names, a command line, and environment variables", and each action declares its inputs and its outputs.[^12] Bazel keeps the outputs of actions in caches in memory, on disk and on a remote build farm, and the documentation says: "At Google, we routinely achieve cache hit rates north of 99%."[^6] A remote cache stores the names of the output files and the hashes of their contents, so that a team and its CI system can share outputs.[^12] With remote execution, Bazel sends build and test actions to many machines. It uses "an open-source gRPC protocol" for remote execution and for remote caching.[^13] The page [[remote-caching-and-execution]] explains these methods for all build tools.

**Hermeticity.** The Bazel documentation says: "When given the same input source code and product configuration, a hermetic build system always returns the same output".[^14] A hermetic build depends on given versions of its tools and libraries, and not on the software that is installed on the machine.[^14] The page [[hermetic-builds]] explains this method. The FAQ says that Bazel makes Java and C++ builds reproducible "assuming you do not change the toolchain", and that build steps that run custom scripts need more care.[^2]

**Platforms.** Bazel gives a platform one of three roles. The host platform runs Bazel, the execution platforms run the build actions, and the target platforms run the code that the build makes.[^15] In a cross-compilation build, the host and the execution platform are the same, and the target platform is different.[^15]

## Common tasks

| Task | Command |
|---|---|
| Build a target | `bazel build //main:hello-world`[^7] |
| Build and run the tests of all packages below `foo` | `bazel test //foo/...`[^8] |
| Build and run a program | `bazel run //main:hello-world`[^8] |
| List all the dependencies of a target | `bazel query "deps(//foo)"`[^16] |
| Find one dependency path between two targets | `bazel query "somepath(//foo:foo, third_party/zlib:zlibonly)"`[^16] |
| Fetch all external dependencies | `bazel fetch`[^8] |
| Remove the outputs | `bazel clean`[^8] |

`bazel query` works on the target graph. The query guide uses `somepath` and `allpaths` to find out why a target depends on a library that its BUILD file does not name.[^16]

## Configuration

**MODULE.bazel.** The external dependencies of a project are Bazel modules. Each module is a versioned Bazel project with a `MODULE.bazel` file at its root.[^17][^18] The Bazel modules page gives this example:[^18]

```starlark
module(name = "my-module", version = "1.0")

bazel_dep(name = "rules_cc", version = "0.0.1")
bazel_dep(name = "protobuf", version = "3.19.0")
```

Bazel reads the `MODULE.bazel` file of the root module, then gets the `MODULE.bazel` files of the dependencies from a Bazel registry until it has the full dependency graph.[^18] It selects one version of each module with the Minimal Version Selection algorithm of Go modules: it takes the highest version that any dependent asks for, and not a later version.[^18] This system is called Bzlmod. The documentation says that Bzlmod "gradually replaced the legacy WORKSPACE system between Bazel 6 and 9."[^17]

**When not to use Bazel.** The FAQ says that Bazel "is not good for running build operations whose outputs should not be cached."[^2] Its examples are a compile step that fetches data from the internet, a test that connects to the QA instance of a site, and a deploy step.[^2] A build that has a few long steps in sequence gets little help from Bazel. The FAQ says to divide such steps into smaller targets that Bazel can run in parallel.[^2]

## Alternatives

- [[other-monorepo-build-tools]]: Buck2, Pants and Please also use BUILD files and a graph of targets. The Bazel FAQ says that former Google engineers made Pants at Twitter and Foursquare and Buck at Facebook, and that both tools "have been modeled after Bazel".[^2]
- [[cmake]] with [[ninja]]: CMake describes a C or C++ project and writes the files for Ninja or another build tool.
- [[make]]: a makefile lists the rules by hand, with no sandbox and no remote cache.
- Maven and Gradle build Java projects. The Bazel FAQ says that Maven is mostly for Java while Bazel handles many languages, and that Bazel build files have more structure than Gradle build files, so Bazel knows what each action does.[^2]

The page [[build-systems-a-la-carte]] compares the way Bazel, Make, Shake and other build tools decide what to rebuild.

## History and versions

Google started its internal tool, Blaze, before Facebook started Buck, but Buck became open source first. Buck was made open source in March 2013, and Bazel in March 2015.[^19] On 1 September 2015, Google announced the Beta release, and the announcement said: "Bazel is what we use to build the large majority of software within Google."[^20] Before the first Alpha release, Bazel was not available outside Google, so open-source projects such as Chromium and Android could not use it.[^2]

Bzlmod and `MODULE.bazel` replaced the `WORKSPACE` file between Bazel 6 and Bazel 9.[^17] The name Bazel is said the same way as the herb basil in US English.[^2]

## References
