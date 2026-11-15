---
tag: 739K0G
kind: tool
slug: other-monorepo-build-tools
name: Other large-scale build tools
aliases: [Buck2, Buck, Pants, Pantsbuild, Please, plz, Nx, Turborepo, turbo, monorepo build tools]
area: tools-and-workflow/build-systems
level: advanced
summary: >-
  Buck2, Pants and Please build large repositories from a graph of targets, as Bazel does. Nx and Turborepo run the package scripts of JavaScript monorepos in graph order and cache their results.
role: [build]
see_also: [bazel, build-graph, remote-caching-and-execution, hermetic-builds, incremental-builds, make, build-system-concept]
sources:
  - url: https://nx.dev/docs/getting-started/how-nx-works
    title: Nx documentation, How Nx works
    locator: The project graph; The task graph; Caching; Inferred and explicit configuration
  - url: https://turborepo.dev/docs
    title: Turborepo documentation, Introduction
    locator: The monorepo solution
  - url: https://bazel.build/about/faq
    title: Bazel documentation, FAQ
    locator: How does Bazel compare to other build tools? (Pants, Buck)
  - url: https://buck2.build/docs/about/why/
    title: Buck2 documentation, Why Buck2
    locator: Why does Buck2 exist?; What's different about Buck2?
  - url: https://www.pantsbuild.org/stable/docs/introduction/how-does-pants-work
    title: Pants documentation, How does Pants work? (version 2.33)
    locator: The Pants Engine; Caching; Remote Execution; Dependency inference
  - url: https://please.build/
    title: Please, the home page
    locator: the tag line; Why Please?; the footer (Thought Machine, documentation licence)
  - url: https://please.build/basics.html
    title: Please basics
    locator: BUILD files; Build targets; the plz commands
  - url: https://turborepo.dev/docs/crafting-your-repository/caching
    title: Turborepo documentation, Caching
    locator: the introduction; Remote Caching; Providing file outputs; global hash and task hash
  - url: https://github.com/facebook/buck2/blob/main/README.md
    title: Buck2 README
    locator: the introduction (fast, hermetic, multi-language); the licence badge
  - url: https://www.pantsbuild.org/stable/docs/introduction/welcome-to-pants
    title: Pants documentation, Welcome to Pants!
    locator: What does Pants do?; Who is Pants for?
  - url: https://nx.dev/docs/getting-started/intro
    title: Nx documentation, Intro to Nx
    locator: the first paragraph; What Nx does; Start small, grow as needed
  - url: https://turborepo.dev/docs/reference/configuration
    title: Turborepo documentation, Configuring turbo.json
    locator: the tasks example; dependsOn (tasks in the same package)
---

## What it is

A large repository can hold many projects in many languages, with thousands of build and test tasks. This page describes five tools for such repositories, other than [[bazel]]. They fall into two groups:

- **Buck2, Pants and Please** describe the build as targets in BUILD files, and they run the compilers and test runners themselves, as Bazel does. They use the same methods: a graph of targets, [[hermetic-builds]], caches, and [[remote-caching-and-execution]].
- **Nx and Turborepo** come from JavaScript and TypeScript monorepos. They do not replace the build tool of each project. They run the tasks that the projects already have, such as the scripts in `package.json`, in the order of a task graph, and they keep the results in a cache.[^1][^2]

The Bazel FAQ says that former Google engineers made Pants at Twitter and Foursquare and Buck at Facebook, and that both tools "have been modeled after Bazel".[^3]

| Tool | Kind | Rules or tasks | Configuration |
|---|---|---|---|
| Buck2 | build system, from Meta | rules in Starlark, engine in Rust | `BUCK` files[^4] |
| Pants | build system | rules in typed Python 3, engine in Rust | `BUILD` files[^5] |
| Please | build system, from Thought Machine | build rules, tool written in Go | `BUILD` files[^6][^7] |
| Nx | task runner | tasks from `package.json`, `project.json` and plugins | `nx.json`[^1] |
| Turborepo | task runner | scripts from `package.json` | `turbo.json`[^2] |

## Install and first use

The build machine of this site has none of these tools, so the configuration files on this page are shown as source, from the documentation of each tool, and they were not run. A Python model shows the two ideas that the tools share: the task graph and the affected projects. The model is not Nx or Turborepo, and it ran with Python 3.12.

The model has the five projects of the example in the Nx documentation: a web application, an API service and three shared packages.[^1] In the model, `packages/ui` also uses `packages/utils`. A build of `apps/web` first builds each project that it depends on, as the rule `"dependsOn": ["^build"]` asks. A changed file affects the project that holds it and all the projects that depend on that project.

```python run id=monorepo-affected
# A model of two ideas that monorepo tools share: the task graph and
# "affected" projects. It is not Nx or Turborepo.
projects = {
    "apps/web": ["packages/ui", "packages/utils"],
    "apps/api": ["packages/core"],
    "packages/ui": ["packages/utils"],
    "packages/utils": [],
    "packages/core": [],
}


def build_tasks(project, order):
    # "dependsOn": ["^build"]: build each dependency first.
    for dep in projects[project]:
        build_tasks(dep, order)
    task = project + "#build"
    if task not in order:
        order.append(task)
    return order


def affected(changed_files):
    changed = {p for p in projects for f in changed_files if f.startswith(p + "/")}
    result = set(changed)
    grew = True
    while grew:
        grew = False
        for p, deps in projects.items():
            if p not in result and result.intersection(deps):
                result.add(p)
                grew = True
    return sorted(result)


print("tasks for build of apps/web:", ", ".join(build_tasks("apps/web", [])))
for files in (["packages/utils/src/date.ts"], ["packages/core/src/Main.java"], ["README.md"]):
    print("changed", files[0], "->", affected(files) or "no project")
```

```output for=monorepo-affected
tasks for build of apps/web: packages/utils#build, packages/ui#build, apps/web#build
changed packages/utils/src/date.ts -> ['apps/web', 'packages/ui', 'packages/utils']
changed packages/core/src/Main.java -> ['apps/api', 'packages/core']
changed README.md -> no project
```

`apps/api` and `packages/core` are not in the task graph of `apps/web`, because the build of `apps/web` does not need them. The Nx documentation says the same about its example.[^1] A change to `packages/utils` affects three projects, and a change to `packages/core` affects two. Nx can run the tasks of the affected projects only.[^1] In the model, a file outside all projects, such as `README.md`, affects no project. The real tools let you name files outside the projects that are inputs to all tasks: in Turborepo, a change to a file in `globalDependencies` makes all tasks miss the cache.[^8]

## Concepts of the tool

**Buck2.** Buck2 is the build system of Meta. Meta has a large monorepo in many languages, such as C++, Python, Rust, Kotlin, Swift, Haskell and OCaml, and the Buck2 documentation says that such repositories are "generally beyond the capabilities of traditional build systems like make".[^4] Buck2 is a rewrite of Buck, which is now called Buck1.[^4] It keeps the targets in `BUCK` files, and it takes ideas from Bazel, Pants, Shake, Tup and academic research.[^4] It is different from Buck1 in these ways:[^4]

- Buck2 is written in Rust, and Buck1 was written in Java.
- All the rules are written in Starlark, outside the binary, so the binary does not know about any language.
- It is remote execution first: local execution is a special case of remote execution.
- It uses a dynamic graph, and a rule can build a file, read it, and then give the dependencies of the next actions. Languages whose dependencies follow the imports, such as Haskell and OCaml, use this.

The README says that with remote execution, Buck2 is hermetic: a rule must declare all of its inputs, and the build fails if, for example, a `.c` file needs a `.h` file that is not declared.[^9] Buck2 is under the MIT licence or the Apache License 2.0.[^9]

**Pants.** Pants runs standard tools such as compilers, code generators, dependency resolvers, test runners, linters and formatters, and it caches their results and runs them in parallel.[^10] The documentation says that it focuses on Python, Go, Java, Scala, Shell and Docker.[^10] Its engine, the v2 engine, is written in Rust, and its build rules are written in typed Python 3.[^5] The engine caches processes by their inputs, runs them in a sandbox, and can send work to a remote build cluster.[^5]

The main idea of Pants is dependency inference. Pants reads the import statements of the code to find the dependencies of each file, so the BUILD files do not list them.[^5] The Pants documentation compares a BUILD file of an older tool, which lists eight dependencies of a Python library, with this BUILD file of Pants 2:[^5]

```text
python_sources(name="lib")

python_tests(name="tests")
```

**Please.** Please describes itself as "a cross-language build system with an emphasis on high performance, extensibility and correctness".[^6] It is written in Go, and Thought Machine makes it.[^6] The targets are in files named `BUILD`, and a directory with a BUILD file is a package.[^7] `plz init plugin python` adds the rules of a language, here Python, to the project.[^7] This is a part of the example in the Please basics:[^7]

```text
python_library(
    name = "my_library",
    srcs = ["file1.py", "file2.py"],
)

python_binary(
    name = "my_binary",
    main = "my_main.py",
    deps = [":my_library"],
)
```

`plz build //package:my_binary` builds the program and the libraries that it needs, `plz run` builds and runs it, and `plz test` runs the tests.[^7] Each build step runs in a hermetic environment that has only the files and the environment variables that the rule declares.[^6]

**Nx.** The Nx documentation says that Nx is "a build system for JavaScript and TypeScript monorepos, with plugins extending it to any language".[^11] Nx finds each project from its `package.json` or `project.json` file, or from a plugin that reads the configuration of a tool, such as `vite.config.ts` or `build.gradle.kts`.[^1] It then reads the TypeScript imports, the `package.json` dependencies and the Gradle project dependencies to make the project graph.[^1] A task is a target of one project, such as `web:build`. For each command, Nx combines the project graph with the task pipeline into a task graph, and it runs the tasks that do not depend on each other in parallel.[^1] This pipeline in `nx.json` says that the build of a project needs the build of each project that it depends on:[^1]

```json
{
  "targetDefaults": {
    "build": {
      "dependsOn": ["^build"]
    }
  }
}
```

Before Nx runs a task, it makes a hash of the inputs of the task: the source files of the project, the files of the projects that it depends on, the configuration, and the versions of the external dependencies.[^1] If a task with the same hash ran before, Nx gives back its terminal output and its files from the cache.[^1] The core of Nx is a task runner with a local cache and affected commands. Nx Cloud adds remote caching and distributes tasks over many machines.[^11]

**Turborepo.** Turborepo uses the `package.json` scripts and the dependencies that a repository already has, and one file `turbo.json`.[^2] It works with npm, yarn and pnpm.[^2] It runs the tasks in parallel on all the cores of the machine.[^2] This is a shorter form of the example in the Turborepo reference:[^12]

```json
{
  "$schema": "https://turborepo.dev/schema.json",
  "tasks": {
    "build": {
      "dependsOn": ["^build"],
      "outputs": ["dist/**"]
    },
    "test": {
      "outputs": ["coverage/**"],
      "dependsOn": ["build"]
    }
  }
}
```

A task name with no prefix in `dependsOn`, such as `build` in the `test` task, is a task in the same package.[^12] Turborepo keeps the results of tasks in the directory `.turbo/cache`, and a remote cache shares them with the team and with CI.[^8] Each task has a global hash and a task hash, and a change to either one makes the task miss the cache.[^8] The documentation warns: "If you do not declare file outputs for a task, Turborepo will not cache them."[^8] Turborepo also assumes that the tasks are deterministic.[^8]

## Common tasks

| Task | Command |
|---|---|
| Build a program with Please | `plz build //package:my_binary`[^7] |
| Run the tests of a target with Please | `plz test //package:my_library_test`[^7] |
| Add the Python rules to a Please project | `plz init plugin python`[^7] |
| Build a project with Nx | `nx build web`[^1] |
| Build all the projects with Nx | `nx run-many -t build`[^1] |
| Show the project graph of Nx in a browser | `nx graph`[^1] |
| Connect a repository to a remote cache with Turborepo | `npx turbo login`, then `npx turbo link`[^8] |

## Configuration

Each group keeps its configuration in a different place:

- Buck2, Pants and Please keep the targets in a BUILD or BUCK file in each package, as Bazel does. Pants needs fewer lines in the BUILD files, because it infers the dependencies from the imports.[^5]
- Nx takes the tasks from three places, from the lowest to the highest precedence: the tasks that plugins infer, `targetDefaults` in `nx.json`, and the `package.json` or `project.json` of the project.[^1] The command `nx show project web --web` shows the result for one project.[^1]
- Turborepo has one `turbo.json` file at the root, and a package can have its own configuration.[^8][^12] The `outputs` of a task tell Turborepo which files to keep in the cache.[^8]

## Alternatives

- [[bazel]]: the build system of Google, with Starlark BUILD files, remote execution and Bzlmod.
- [[make]]: for one project, a makefile can be enough. The Buck2 documentation says that a large repository in many languages is beyond such tools.[^4]
- The page [[build-graph]] explains the graph of targets that all these tools use, and [[incremental-builds]] explains how they find the work to do again.

## History and versions

Facebook made Buck open source in March 2013, before Google did the same with Bazel in March 2015.[^4] Meta later wrote Buck2 to replace Buck, and Buck1 is no longer used at Meta.[^4] Pants 2 uses the v2 engine, which the Pants team wrote from the start with the lessons of the v1 engine.[^5] The Pants documentation used for this page is for version 2.33.[^5]

## References
