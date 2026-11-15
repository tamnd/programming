---
tag: 39P480
kind: tool
slug: gradle
name: Gradle
aliases: [Gradle Build Tool, gradlew, Gradle Wrapper, build.gradle, build.gradle.kts, settings.gradle, Kotlin DSL, Groovy DSL]
area: tools-and-workflow/build-systems
level: core
summary: >-
  Gradle is a build tool for Java, Kotlin and Android. Build scripts in Kotlin or Groovy declare tasks, and Gradle skips tasks whose inputs did not change or takes their outputs from a cache.
role: [build, package-manager]
licence: apache-2.0
repo: https://github.com/gradle/gradle
homepage: https://gradle.org/
history:
  year: 2009
  people: []
  note: The oldest release on the Gradle releases page is 0.7, of 20 July 2009. Gradle 1.0 came out on 12 June 2012, 2.0 on 1 July 2014 and 9.0.0 on 31 July 2025.
see_also: [maven, build-system-concept, build-a-project, incremental-builds, remote-caching-and-execution, build-graph, bazel, dotnet]
sources:
  - url: https://docs.gradle.org/current/userguide/userguide.html
    title: Gradle User Manual, Gradle Build Tool (version 9.9.1)
    locator: Gradle Build Tool; Supported Languages and Frameworks; Licenses
  - url: https://developer.android.com/build
    title: Android Developers, Configure your build
    locator: Android build glossary; Build types
  - url: https://docs.gradle.org/current/userguide/gradle_basics.html
    title: Gradle User Manual, Core Concepts
    locator: Core concepts; Project structure; Invoking Gradle
  - url: https://docs.gradle.org/current/userguide/build_lifecycle.html
    title: Gradle User Manual, Build Lifecycle
    locator: Build Phases; The Phases in Build Scripts
  - url: https://docs.gradle.org/current/userguide/incremental_build.html
    title: Gradle User Manual, Incremental build
    locator: the introduction; Task inputs and outputs
  - url: https://github.com/gradle/gradle/blob/v9.9.1/LICENSE
    title: Gradle licence, tag v9.9.1
    locator: the whole file (Apache License 2.0)
  - url: https://docs.gradle.org/current/userguide/gradle_wrapper.html
    title: Gradle User Manual, Gradle Wrapper
    locator: the introduction; 1. Adding the Gradle Wrapper; 3. Upgrading the Gradle Wrapper
  - url: https://docs.gradle.org/current/userguide/building_java_projects.html
    title: Gradle User Manual, Building Java and JVM projects
    locator: Introduction; Managing dependencies
  - url: https://docs.gradle.org/current/userguide/more_about_tasks.html
    title: Gradle User Manual, Understanding Tasks
    locator: Task outcomes
  - url: https://docs.gradle.org/current/userguide/build_cache.html
    title: Gradle User Manual, Build Cache
    locator: Overview; Enable the Build Cache; What does it look like
  - url: https://docs.gradle.org/current/userguide/gradle_daemon.html
    title: Gradle User Manual, Gradle Daemon
    locator: the introduction; Stop Daemon
  - url: https://docs.gradle.org/current/userguide/configuration_cache.html
    title: Gradle User Manual, Configuration Cache
    locator: the introduction; Configuration Cache Basics; Cache Hit
  - url: https://docs.gradle.org/current/userguide/declaring_dependencies.html
    title: Gradle User Manual, Declaring dependencies
    locator: Adding a dependency; Understanding types of dependencies
  - url: https://docs.gradle.org/current/userguide/version_catalogs.html
    title: Gradle User Manual, Version Catalogs
    locator: the introduction; Using a catalog
  - url: https://docs.gradle.org/current/userguide/kotlin_dsl.html
    title: Gradle User Manual, Kotlin DSL Primer
    locator: the introduction; Script file names
  - url: https://gradle.org/releases/
    title: Gradle, Releases
    locator: the release list (0.7, 1.0, 2.0, 5.0, 7.0, 8.0, 9.0.0, 9.9.1)
---

## What it is

Gradle is a build tool. Its user manual describes it as "a fast, dependable, and adaptable open-source build automation tool with an elegant and extensible declarative build language".[^1] It supports Android, Java, Kotlin Multiplatform, Groovy, Scala, JavaScript and C/C++, and the common IDEs, such as Android Studio, IntelliJ IDEA, Visual Studio Code, Eclipse and NetBeans, can run it.[^1] Android apps are built with Gradle and the Android Gradle plugin.[^2]

A Gradle build is made of projects and tasks. A project is a part of the software that can be built, such as an application or a library. A task is a unit of work, such as compiling code or running tests.[^3] Build scripts, named `build.gradle` in Groovy or `build.gradle.kts` in Kotlin, declare the tasks and the dependencies, and plugins add more tasks and conventions.[^3]

Gradle and [[maven]] do the same job for Java projects, but they work in different ways. A Maven build runs a fixed list of phases. A Gradle build runs the tasks that the command asks for and the tasks that these depend on, in the order of a task graph.[^4] Gradle also checks the inputs and outputs of each task, so that it does not do work that it did before.[^5]

The source code of Gradle is free software under the Apache License 2.0.[^1][^6]

## Install and first use

Most projects do not need an installed Gradle. A project that uses Gradle has the scripts `gradlew` and `gradlew.bat` in its root directory: the Gradle Wrapper.[^3] The wrapper runs a declared version of Gradle, and it downloads that version first if necessary. The Gradle manual says: "The recommended way to execute any Gradle build is with the help of the Gradle Wrapper".[^7] An installed Gradle makes the wrapper files with the task `wrapper`, and the command `gradle init` makes them when it makes a project.[^7]

```text
./gradlew build
```

A Gradle project has this structure:[^3]

```text
project
├── gradle                  the wrapper files
├── gradlew                 the wrapper scripts
├── gradlew.bat
├── settings.gradle.kts     the name of the root project and the subprojects
├── subproject-a
│   ├── build.gradle.kts    the build script of subproject-a
│   └── src/
└── subproject-b
    ├── build.gradle.kts
    └── src/
```

The build machine of this site has no Gradle, so the build scripts on this page are from the Gradle manual and were not run. This is the build script of a Java library in the Kotlin DSL, from the guide to Java projects:[^8]

```text
plugins {
    `java-library`
}

java {
    toolchain {
        languageVersion = JavaLanguageVersion.of(17)
    }
}

version = "1.2.1"
```

The plugin `java-library` adds these tasks: `compileJava` compiles the sources in `src/main/java`, `compileTestJava` compiles the tests in `src/test/java`, `test` runs the tests, `jar` makes a JAR file with the name `<project>-<version>.jar`, and `javadoc` makes the Javadoc.[^8] The toolchain block selects Java 17, and the manual says that this protects the project against builds with different Java versions. The version is also a part of the name of the JAR file.[^8] The plugin attaches `jar` to the task `assemble` and `test` to the task `check`.[^8]

## Concepts of the tool

**The three phases of a build.** A Gradle build has three phases, in this order:[^4]

1. **Initialization.** Gradle runs the init scripts and the settings file, `settings.gradle(.kts)`, and finds the projects of the build.
2. **Configuration.** Gradle runs the build script of each project. The scripts register the tasks, and Gradle makes the task graph from the dependencies between the tasks.
3. **Execution.** Gradle runs the tasks that the command selected, in the order of the task graph. It can run tasks that do not depend on each other in parallel.

The code in a build script runs in the configuration phase, except the code in the `doFirst` and `doLast` blocks of a task, which runs when the task runs.[^4] Gradle configures only the tasks that the build needs.[^4]

**Task outcomes.** When a task runs, Gradle shows an outcome next to its name:[^9]

| Label | Meaning |
|---|---|
| (no label) or `EXECUTED` | The task ran its actions. A task with no actions shows this when one of its dependencies ran. |
| `UP-TO-DATE` | The inputs and the outputs of the task did not change since the last run, so Gradle did not run it. |
| `FROM-CACHE` | Gradle took the outputs of the task from the build cache. |
| `SKIPPED` | The command excluded the task, or a predicate of the task returned false. |
| `NO-SOURCE` | The task has inputs and outputs, but no source files. |

**Incremental build.** Each task declares its inputs and its outputs. Before Gradle runs a task, it checks whether the inputs or the outputs changed since the last run. If they did not change, the task is up to date and Gradle skips it.[^5] A task with no declared output cannot be up to date, so the manual says that incremental build "won't work unless a task has at least one task output".[^5]

**The build cache.** The up-to-date check uses only the last build in the same directory. The build cache keeps the outputs of tasks from earlier builds, in a local directory or on a remote server, and Gradle can take them from there when the inputs are the same.[^10] Gradle makes a cache key for each task from its inputs.[^10] The build cache is off by default.[^10] The manual shows a Java project that is built with the cache, then cleaned and built again:[^10]

```text
$ ./gradlew clean
:clean

BUILD SUCCESSFUL

$ ./gradlew --build-cache assemble
:compileJava FROM-CACHE
:processResources
:classes
:jar
:assemble

BUILD SUCCESSFUL
```

Only `compileJava` came from the cache. The manual says that `classes` and `assemble` are lifecycle tasks, and that `processResources` and `jar` copy files, so Gradle does not cache them, because it is generally faster to run them again.[^10]

The model below is not Gradle. It has the five tasks of that example, and it gives the outcome of each task in four builds. The model makes a key for each task from the contents of its input files and the outputs of its dependencies. A task is up to date when its key is the same as in the last build in the same workspace. A cacheable task is `FROM-CACHE` when its key is in the cache. In the model, only `compileJava` is cacheable, as in the example. The model ran with Python 3.12.

```python run id=gradle-up-to-date
# A model of Gradle up-to-date checks and the build cache. It is not Gradle.
import hashlib

# task: (dependencies, input files, cacheable, lifecycle task)
TASKS = {
    "compileJava": ([], ["src/main/java/App.java"], True, False),
    "processResources": ([], ["src/main/resources/app.properties"], False, False),
    "classes": (["compileJava", "processResources"], [], False, True),
    "jar": (["classes"], [], False, False),
    "assemble": (["jar"], [], False, True),
}


def key(*parts):
    return hashlib.sha256(repr(parts).encode()).hexdigest()[:12]


def order(task, done):
    for dep in TASKS[task][0]:
        order(dep, done)
    if task not in done:
        done.append(task)
    return done


def run(goal, files, workspace, cache, build_cache):
    outputs, states = {}, {}
    for task in order(goal, []):
        deps, inputs, cacheable, lifecycle = TASKS[task]
        k = key(task, [files[f] for f in inputs], [outputs[d] for d in deps])
        if lifecycle:
            up = all(states[d] in ("UP-TO-DATE", "FROM-CACHE", "SKIPPED") for d in deps)
            state = "UP-TO-DATE" if up else ""
        elif workspace.get(task) == k:
            state = "UP-TO-DATE"
        elif build_cache and cacheable and k in cache:
            state = "FROM-CACHE"
        else:
            state = ""
            if build_cache and cacheable:
                cache.add(k)
        workspace[task] = k
        outputs[task], states[task] = key("out", k), state
        print((":" + task + " " + state).rstrip())


files = {"src/main/java/App.java": "class App {}", "src/main/resources/app.properties": "greeting=hi"}
workspace, cache = {}, set()
print("--- assemble, the first time")
run("assemble", files, workspace, cache, True)
print("--- assemble again, with no change")
run("assemble", files, workspace, cache, True)
print("--- assemble after a change to app.properties")
files["src/main/resources/app.properties"] = "greeting=hello"
run("assemble", files, workspace, cache, True)
print("--- clean, then assemble with the build cache")
workspace.clear()
run("assemble", files, workspace, cache, True)
```

```output for=gradle-up-to-date
--- assemble, the first time
:compileJava
:processResources
:classes
:jar
:assemble
--- assemble again, with no change
:compileJava UP-TO-DATE
:processResources UP-TO-DATE
:classes UP-TO-DATE
:jar UP-TO-DATE
:assemble UP-TO-DATE
--- assemble after a change to app.properties
:compileJava UP-TO-DATE
:processResources
:classes
:jar
:assemble
--- clean, then assemble with the build cache
:compileJava FROM-CACHE
:processResources
:classes
:jar
:assemble
```

The last part of the output is the same as the output in the manual. A change to `app.properties` makes `processResources` run again, and `jar` and `assemble` too, because their inputs changed. `compileJava` stays up to date, because its input is only the Java source.

**The daemon.** Gradle runs on the Java virtual machine, and the start of a JVM and of the Gradle libraries takes time. The Gradle Daemon is "a long-lived background process" that keeps Gradle in memory between builds.[^11] It caches project information, and the JVM can optimize the code that runs in it from one build to the next.[^11] The daemon is on by default from Gradle 3.0, and `gradle --stop` stops the daemons of the same Gradle version.[^11]

**The configuration cache.** The build cache saves work in the execution phase. The configuration cache saves the result of the configuration phase. When no input of the configuration changed, Gradle skips the configuration phase, loads the task graph from the cache, and goes straight to execution.[^12] The manual says that the configuration cache "will be enabled by default in Gradle 10".[^12]

**Dependencies.** A build script declares dependencies in the block `dependencies { }`. Each line gives a configuration, such as `implementation` or `testImplementation`, and the coordinates `group:name:version`.[^13] Gradle also gets the transitive dependencies, the dependencies of the dependencies.[^13] The block `repositories { }` gives the repositories, such as Maven Central with `mavenCentral()`.[^8] A dependency on another project of the same build uses the path of the project:[^13]

```text
dependencies {
    implementation("org.codehaus.groovy:groovy:3.0.5")
    implementation(project(":utils"))
}
```

**Version catalogs.** A version catalog keeps the coordinates and the versions of the dependencies in one file, `gradle/libs.versions.toml`, for all the projects of a build.[^14] This catalog is from the manual:[^14]

```toml
[versions]
groovy = "3.0.5"

[libraries]
groovy-core = { module = "org.codehaus.groovy:groovy", version.ref = "groovy" }
groovy-json = { module = "org.codehaus.groovy:groovy-json", version.ref = "groovy" }
```

A build script then uses `implementation(libs.groovy.core)`. Gradle makes type-safe accessors such as `libs.groovy.core` from the catalog, so an IDE can complete them.[^14]

## Common tasks

| Task | Command |
|---|---|
| Build the project | `./gradlew build`[^3] |
| Run the tests | `./gradlew test`[^3] |
| Remove the outputs, then build | `./gradlew clean build`[^3] |
| List the tasks of a project | `./gradlew :app:tasks`[^9] |
| Show the dependency tree | `./gradlew dependencies`[^13] |
| Build with the build cache | `./gradlew --build-cache assemble`[^10] |
| Add or update the wrapper files | `gradle :wrapper`[^7] |
| Stop the daemons | `gradle --stop`[^11] |

## Configuration

**Settings and build scripts.** `settings.gradle(.kts)` gives the name of the root project and includes the subprojects. Each project has a `build.gradle(.kts)` script.[^3] Groovy DSL scripts have the extension `.gradle`, and Kotlin DSL scripts have the extension `.gradle.kts`. One build can mix the two DSLs.[^15] The manual says that the Kotlin DSL gives better content assist, refactoring and documentation in IDEs that support it.[^15]

**gradle.properties.** The file `gradle.properties` in the project or in the Gradle User Home sets the properties of the build. For example, `org.gradle.caching=true` turns on the build cache for all builds.[^10][^11]

**The wrapper properties.** The file `gradle/wrapper/gradle-wrapper.properties` gives the Gradle distribution that the wrapper uses, such as `distributionUrl=https\://services.gradle.org/distributions/gradle-9.9.1-bin.zip`.[^7] The wrapper files, also the small JAR file, go into version control.[^7] To upgrade Gradle, run the `wrapper` task with the version to use.[^7]

**Android builds.** In an Android project, the Android Gradle plugin adds the Android concepts to the build. Build types, such as `debug` and `release`, give the properties of a build for a stage of development. The `debug` build type signs the app with a debug key, and the `release` build type can shrink and obfuscate the app and sign it with a release key.[^2] Android Studio makes these two build types by default.[^2]

## Alternatives

- [[maven]]: the other common build tool of Java, with a POM in XML and a fixed lifecycle.
- [[bazel]]: a build system for large repositories in many languages, with BUILD files in Starlark.
- [[dotnet]]: the command line of the .NET SDK, which builds .NET projects with MSBuild.

The page [[incremental-builds]] explains up-to-date checks for all build tools, and [[remote-caching-and-execution]] explains shared build caches.

## History and versions

The Gradle releases page lists all the releases from 0.7, of 20 July 2009.[^16] These are some of them:[^16]

| Version | Release date |
|---|---|
| 0.7 | 20 July 2009 |
| 1.0 | 12 June 2012 |
| 2.0 | 1 July 2014 |
| 5.0 | 26 November 2018 |
| 7.0 | 9 April 2021 |
| 8.0 | 13 February 2023 |
| 9.0.0 | 31 July 2025 |
| 9.9.1 | 10 November 2026 |

This page uses the user manual of version 9.9.1.[^1] The user manual is under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 licence, and the source code is under the Apache License 2.0.[^1]

## References
