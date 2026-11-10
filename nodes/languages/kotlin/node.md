---
tag: NEW-kotlin
kind: language
slug: kotlin
name: Kotlin
area: languages
level: intro
summary: >-
  Kotlin is a statically typed language from JetBrains that combines object-oriented and functional features. One compiler turns it into JVM bytecode, JavaScript, WebAssembly and native code.
class: programming
tier: 1
paradigms: [object-oriented, functional, imperative, concurrent]
typing: {discipline: static, strength: strong, checking: [static, runtime], inference: local}
memory: garbage-collected
execution: [vm, compiled-native, transpiled]
appeared: 2011
designers: [andrey-breslav, max-shafirov]
influenced_by: [java, scala, csharp, groovy, javascript]
standard: null
reference: {url: "https://kotlinlang.org/spec/", licence: apache-2.0}
licence_of_reference_impl: apache-2.0
file_extensions: [kt, ktm, kts]
hello_world: examples/kotlin.md#hello
same_as: {wikidata: Q3816639, wikipedia: Kotlin, pldb: kotlin, linguist: Kotlin, rosetta: null, hopl: null}
sources:
  - url: https://kotlinlang.org/spec/introduction.html#introduction
    title: Kotlin language specification, version 1.9-rfc+0.1
    locator: Introduction; Compatibility
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/faq.html#what-is-kotlin
    title: FAQ, Kotlin documentation
    locator: What is Kotlin?; Is Kotlin free?; What does Kotlin compile down to?; Which versions of JVM does Kotlin target?; Is Kotlin hard?
    accessed: 2026-11-10
  - url: https://blog.jetbrains.com/kotlin/2016/02/kotlin-1-0-released-pragmatic-language-for-jvm-and-android/
    title: "Kotlin 1.0 Released: Pragmatic Language for the JVM and Android, by Andrey Breslav, The JetBrains Blog (2016-02-15)"
    locator: the opening paragraph; the list of compatibility commitments
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/kotlin-evolution-principles.html#principles-of-pragmatic-evolution
    title: Kotlin evolution principles, Kotlin documentation
    locator: Principles of pragmatic evolution; Decision making; Language feature delivery; Pre-stable features
    accessed: 2026-11-10
  - url: https://github.com/Kotlin/kotlin-spec/blob/release/LICENSE
    title: LICENSE of the Kotlin specification repository (Apache License 2.0)
    locator: the whole file
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/basic-syntax.html#program-entry-point
    title: Basic syntax overview, Kotlin documentation
    locator: Program entry point; Variables; String templates; Type checks and automatic casts
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/command-line.html#create-and-run-an-application
    title: Kotlin command-line compiler, Kotlin documentation
    locator: Create and run an application; Run the REPL; Run scripts
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/sealed-classes.html#use-sealed-classes-with-when-expression
    title: Sealed classes and interfaces, Kotlin documentation
    locator: the introduction; Use sealed classes with when expression
    accessed: 2026-11-10
  - url: https://kotlinlang.org/spec/expressions.html#exhaustive-when-expressions
    title: Kotlin language specification, version 1.9-rfc+0.1, Expressions
    locator: When expressions, Exhaustive when expressions; Elvis operator expressions; Navigation operators; String interpolation expressions
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/data-classes.html
    title: Data classes, Kotlin documentation
    locator: the introduction; Copying
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/extensions.html#extension-functions
    title: Extensions, Kotlin documentation
    locator: the introduction; Extension functions
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/lambdas.html#higher-order-functions
    title: Higher-order functions and lambdas, Kotlin documentation
    locator: "Higher-order functions; it: implicit name of a single parameter; Closures"
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/null-safety.html#nullable-types-and-non-nullable-types
    title: Null safety, Kotlin documentation
    locator: the introduction; Nullable types and non-nullable types; Safe call operator; Elvis operator; Not-null assertion operator
    accessed: 2026-11-10
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: the Kotlin entry, extensions
    accessed: 2026-11-10
  - url: https://kotlinlang.org/spec/type-system.html#nullable-types
    title: Kotlin language specification, version 1.9-rfc+0.1, Type system
    locator: Nullable types; Flexible types
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/java-interop.html#null-safety-and-platform-types
    title: Calling Java from Kotlin, Kotlin documentation
    locator: the introduction; Null-safety and platform types; Java generics in Kotlin; Checked exceptions
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/generics.html#type-erasure
    title: Generics, Kotlin documentation
    locator: Type erasure; Generics type checks and casts
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/native-memory-manager.html#garbage-collector
    title: Kotlin/Native memory management, Kotlin documentation
    locator: the introduction; Garbage collector
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/wasm-overview.html
    title: Kotlin/Wasm, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/js-overview.html
    title: Kotlin/JavaScript, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/native-overview.html#target-platforms
    title: Kotlin/Native, Kotlin documentation
    locator: the introduction; Target platforms; Interoperability
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/whatsnew20.html#kotlin-k2-compiler
    title: What's new in Kotlin 2.0.0, Kotlin documentation
    locator: the header (Released May 21, 2024); Kotlin K2 compiler
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/control-flow.html#iterators
    title: Conditions and loops, Kotlin documentation
    locator: For loops, Iterators
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/coroutines-overview.html#suspending-functions-and-coroutine-builders
    title: Coroutines, Kotlin documentation
    locator: the introduction; Suspending functions and coroutine builders
    accessed: 2026-11-10
  - url: https://kotlinlang.org/spec/asynchronous-programming-with-coroutines.html#suspending-functions
    title: Kotlin language specification, version 1.9-rfc+0.1, Asynchronous programming with coroutines
    locator: Suspending functions; Coroutines
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/releases.html#release-history
    title: Kotlin releases, Kotlin documentation
    locator: the introduction (release types); Upcoming Kotlin releases; Standard library security support; Release history
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/security.html
    title: Security, Kotlin documentation
    locator: the list of recommendations
    accessed: 2026-11-10
  - url: https://endoflife.date/api/v1/products/kotlin/
    title: Kotlin, endoflife.date API v1
    locator: releases 2.4 to 2.0 (eol is labelled Security Support)
    accessed: 2026-11-10
  - url: https://github.com/JetBrains/kotlin/releases
    title: Releases of JetBrains/kotlin on GitHub
    locator: v2.5.0-Beta2; v2.4.21; v2.4.20; v2.4.10; v2.4.0
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/whatsnew24.html#new-stable-features
    title: What's new in Kotlin 2.4.0, Kotlin documentation
    locator: the list of highlights; New features (Stable)
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/components-stability.html#current-stability-of-kotlin-components
    title: Stability of Kotlin components, Kotlin documentation
    locator: Stability levels explained; Current stability of Kotlin components
    accessed: 2026-11-10
  - url: https://github.com/JetBrains/kotlin/blob/v2.4.21/license/README.md
    title: The license README of JetBrains/kotlin at v2.4.21
    locator: the opening paragraph
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/multiplatform/kmp-overview.html
    title: What is Kotlin Multiplatform, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/faq.html#what-build-tools-support-kotlin
    title: FAQ, Kotlin documentation
    locator: What IDEs support Kotlin?; What build tools support Kotlin?
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/compiler-reference.html#werror
    title: Kotlin compiler options, Kotlin documentation
    locator: -Werror
    accessed: 2026-11-10
  - url: https://github.com/pinterest/ktlint
    title: ktlint on GitHub
    locator: the repository description; Key features
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/jvm-test-using-junit.html
    title: Test Java code using Kotlin and JUnit, tutorial, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/kotlin-doc.html
    title: "Document Kotlin code: KDoc, Kotlin documentation"
    locator: the introduction
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/server-overview.html#explore-frameworks
    title: Backend development with Kotlin, Kotlin documentation
    locator: Explore frameworks; Spring; Ktor
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/android-overview.html
    title: Kotlin for Android, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
  - url: https://blog.jetbrains.com/kotlin/2020/11/roman-elizarov-is-the-new-project-lead-for-kotlin/
    title: Roman Elizarov is the new Project Lead for Kotlin, by Andrey Breslav, The JetBrains Blog (2020-11-19)
    locator: the whole post
    accessed: 2026-11-10
  - url: https://blog.jetbrains.com/kotlin/2011/07/hello-world-2/
    title: Hello World, by Dmitry Jemerov, The JetBrains Blog (2011-07-19)
    locator: the first and the third paragraphs
    accessed: 2026-11-10
  - url: https://android-developers.googleblog.com/2017/05/android-announces-support-for-kotlin.html
    title: Android Announces Support for Kotlin, by Mike Cleron, Android Developers Blog (2017-05-17)
    locator: the opening paragraphs
    accessed: 2026-11-10
  - url: https://android-developers.googleblog.com/2018/10/kotlin-momentum-for-android-and-beyond.html
    title: Kotlin Momentum for Android and Beyond, Android Developers Blog (2018-10-04)
    locator: the paragraph on the Kotlin Foundation
    accessed: 2026-11-10
  - url: https://developer.android.com/kotlin/first
    title: Android's Kotlin-first approach, Android Developers
    locator: the introduction; Why is Android development Kotlin-first?; What does Kotlin-first mean?
    accessed: 2026-11-10
  - url: https://kotlinlang.org/docs/exceptions.html
    title: Exception and error handling, Kotlin documentation
    locator: the introduction
    accessed: 2026-11-10
---

## Overview

Kotlin is a multiplatform, statically typed, general-purpose language. Its specification says that it took inspiration from many languages, including Java, Scala, C# and Groovy, and that one main idea behind it is to be pragmatic, a language for day-to-day development.[^1] JetBrains develops Kotlin, and it is open source under the Apache 2.0 licence. It targets the JVM, Android, JavaScript, WebAssembly (Wasm) and native code.[^2]

The announcement of Kotlin 1.0 calls it "a pragmatic programming language for the JVM and Android that combines OO and functional features and is focused on interoperability, safety, clarity, and tooling support."[^3] The evolution principles of the project say: "Kotlin is designed to be a pragmatic tool for programmers". They keep the language up to date with the needs of its users, keep a feedback loop with users, and make updates comfortable, with deprecation and migration tools before an incompatible change.[^4]

No standards body publishes Kotlin. The reference is the Kotlin language specification, version 1.9-rfc+0.1, which describes Kotlin 1.9. It covers Kotlin/Core, the parts of the language that work mostly the same way on every platform. The specification has an experimental stability level, so even its incremental releases can add, remove or change parts without warning.[^1] It is under the Apache License 2.0.[^5] For later versions, the documentation and the "What's new" page of each release describe the changes. The guide states facts for Kotlin 2.4, and the examples run with kotlinc 2.4.21 on JDK 25.

## A first program

```kotlin run id=hello
fun main() {
    println("Hello, world!")
}
```

```output for=hello
Hello, world!
```

The entry point of a Kotlin application is the `main` function. Another form of `main` takes the command-line arguments as an `Array<String>`.[^6] On the JVM, `kotlinc hello.kt -include-runtime -d hello.jar` makes a runnable JAR file that includes the Kotlin runtime, and `java -jar hello.jar` runs it.[^7]

The program below models shapes. A sealed interface lists the kinds of shape, and three data classes implement it.

```kotlin run id=shapes
sealed interface Shape

data class Rect(val width: Int, val height: Int) : Shape

data class Square(val side: Int) : Shape

data class Triangle(val base: Int, val height: Int) : Shape

fun Shape.area(): Int = when (this) {
    is Rect -> width * height
    is Square -> side * side
    is Triangle -> base * height / 2
}

fun main() {
    val shapes = listOf(Rect(3, 4), Square(5), Triangle(6, 2))
    for (shape in shapes) {
        println("$shape has area ${shape.area()}")
    }
    val large = shapes.filter { it.area() > 10 }
    println("Area over 10: $large")
    println("Total area: ${shapes.sumOf { it.area() }}")

    val byName: Map<String, Shape> = mapOf("tile" to Square(2))
    println(byName["tile"]?.area() ?: 0)
    println(byName["door"]?.area() ?: 0)
    println(Square(5) == Square(5))
    println(Square(5).copy(side = 7))
}
```

```output for=shapes
Rect(width=3, height=4) has area 12
Square(side=5) has area 25
Triangle(base=6, height=2) has area 6
Area over 10: [Rect(width=3, height=4), Square(side=5)]
Total area: 43
4
0
true
Square(side=7)
```

The `when` in `area` has no `else` branch. The compiler knows all the direct subtypes of a sealed interface, so it checks that the branches cover them all ([[sum-types]]).[^8][^9] After the check `is Rect`, the compiler casts `this` automatically, so the branch can use `width`.[^6] For a `data` class the compiler generates `equals`, `hashCode`, a `toString` of the form `Rect(width=3, height=4)`, and a `copy` function.[^10] `Shape.area` is an extension function: it makes a new function callable on `Shape` but does not change the interface.[^11] The lambdas given to `filter` and `sumOf` use `it`, the implicit name of a single parameter.[^12] The map has no entry for `"door"`, so the safe call `?.` gives `null` and the Elvis operator `?:` gives the alternative value 0.[^13] A string template puts a value or an expression into a string with `$` and `${}`.[^6][^9]

## The language at a glance

A source file has the extension `.kt`, and a script has the extension `.kts`.[^7] GitHub Linguist counts files with the extensions `.kt`, `.ktm` and `.kts` as Kotlin.[^14]

**Types.** The type system separates nullable types, written with `?`, from non-nullable types at compile time ([[null-references]]).[^1][^15] A variable declared with `val` is assigned once, and the compiler infers its type when the type is left out.[^6] Types that come from Java are platform types (flexible types in the specification), and Kotlin relaxes its null checks on them to the level of Java.[^15][^16] Classes and functions can have type parameters, with bounds and variance ([[parametric-polymorphism]]).[^1] At run time a generic instance holds no information about its type arguments, so the compiler rejects a check such as `is List<Int>` ([[type-erasure]]).[^17]

```kotlin run id=glance
fun nickname(name: String): String? = mapOf("Robert" to "Bob")[name]

fun describe(x: Any): String =
    if (x is String) "a string of length ${x.length}" else "not a string"

fun main() {
    val bob: String? = nickname("Robert")
    val ann: String? = nickname("Ann")
    println(bob?.length)
    println(ann?.length)
    println(ann?.length ?: 0)
    println(describe("hello"))
    println(describe(42))
    val words = ArrayList<String>()
    val numbers = ArrayList<Int>()
    println(words::class == numbers::class)
    try {
        println(ann!!.length)
    } catch (e: NullPointerException) {
        println("caught ${e::class.simpleName}")
    }
}
```

```output for=glance
3
null
0
a string of length 5
not a string
true
caught NullPointerException
```

The compiler reports an error for a call such as `ann.length` on a `String?`, and allows only a safe call or a not-null assertion. The not-null assertion `!!` converts a value to a non-nullable type and throws a `NullPointerException` if the value is null. The documentation lists the only causes of that exception: an explicit `throw`, the `!!` operator, data that is not yet initialized during construction, and Java code.[^13]

**Memory.** A program does not free objects by hand. Kotlin/Native stores objects in a shared heap and runs a tracing garbage collector, a concurrent mark and sweep collector that does not separate the heap into generations ([[garbage-collection]]).[^18] To run Kotlin/Wasm code, a browser must support the garbage collection proposal of WebAssembly.[^19]

**Execution.** For the JVM, the compiler makes Java-compatible bytecode. By default this bytecode runs on Java 8, and a project can select a target from Java 9 to 26. For JavaScript, it makes ES5, ES2015 or ES2020 code.[^2][^20] For native targets, it makes code through LLVM, and the program runs without a virtual machine on Linux, Windows, the Android NDK and Apple platforms.[^2][^21] Kotlin/Wasm compiles to WebAssembly for browsers or for standalone virtual machines with WASI.[^19] From Kotlin 2.0.0 the K2 compiler is the default and is stable for the JVM, Native, Wasm and JS targets.[^22]

**Paradigms.** Kotlin is object-oriented and has many functional elements, with first-class higher-order functions and lambda literals.[^1] A lambda can read and change the variables of its outer scope ([[closures]]).[^12] A `for` loop iterates through anything that provides an iterator ([[iterators]]).[^23] An interface can be sealed, so that all its direct implementations are known at compile time ([[interfaces]]).[^8] For concurrency, a function marked `suspend` can pause without blocking a thread. Coroutines are lightweight alternatives to threads, and most coroutine features, such as the builders `launch` and `async`, come from the kotlinx.coroutines library ([[async-await]], [[threads]]).[^24][^25]

## The guide

The guide to Kotlin has the 25 parts of the language template. Only this page exists so far, so the table has no links yet.

| Part | What it covers in Kotlin |
|---|---|
| Overview and history | this page: design goals, versions, targets, history |
| Getting started | `kotlinc`, the JVM, a Gradle project, the REPL and scripts |
| Lexical structure and syntax | identifiers, keywords, literals, optional semicolons, operator precedence |
| Values and types | `Int` and the other built-in types, `Any`, `Unit`, `Nothing`, nullable types |
| Variables, binding and scope | `val` and `var`, type inference, properties, `lateinit`, shadowing |
| Control flow | `if` and `when` as expressions, `for`, `while`, ranges, labels |
| Functions | top-level functions, default and named arguments, lambdas, extension and inline functions |
| Built-in collections | `List`, `Set`, `Map`, read-only and mutable views, sequences, arrays |
| Strings and text | `String` and `Char`, templates, multiline strings, `Regex` |
| User-defined types | classes, interfaces, data classes, sealed hierarchies, enums, objects, value classes |
| Errors and exceptions | `try` as an expression, `throw`, unchecked exceptions, `Result`, `require` and `check` |
| Memory and resources | garbage collection on each target, `use` for closeable resources |
| Modules, packages and builds | packages, `import`, modules, Gradle and Maven, multiplatform source sets |
| Concurrency and parallelism | suspending functions, coroutines, structured concurrency, flows, channels |
| Metaprogramming | annotations, reflection, compiler plugins, delegated properties |
| The standard library | a map of `kotlin.*` and the kotlinx libraries; the api cards hang here |
| I/O, files and the OS | `java.io` and `java.nio.file` on the JVM, `kotlinx-io`, processes |
| Testing | `kotlin.test`, JUnit, test doubles |
| Tooling | `kotlinc`, IntelliJ IDEA, Gradle, KDoc and Dokka, ktlint |
| Ecosystem | Maven Central, the Kotlin Foundation, KEEP, libraries by need |
| Performance | inline functions, JVM tuning, Kotlin/Native builds, benchmarks |
| Interoperability | Java, JavaScript, C and Swift or Objective-C |
| Idioms and style | the coding conventions, scope functions, null safety, immutability |
| Pitfalls | platform types, `!!`, `==` and `===`, type erasure |
| Versions and migration | the release cycle, stability levels, opt-in, deprecation cycles |

## Versions and support

From Kotlin 2.0.0 there are three kinds of release. A language release (2.x.0) comes every six months. A tooling release (2.x.20) comes three months after it. Bug fix releases (2.x.yz) have no exact schedule.[^26] Language features ship only in language and tooling releases. A pre-stable feature (Experimental, Alpha or Beta) needs an explicit opt-in and can change at any time.[^4]

From Kotlin 2.4.0, the standard library for the JVM has an 18-month support window for each release line, and the window of 2.4 ends on 2027-12-03. If JetBrains finds a security problem in it, it ships a bug fix release for the newest release of each line in its window.[^26][^27] The official pages give no support window for the lines before 2.4. endoflife.date ends each of those lines on the day the next line came out, and it gives no end date for 2.4.[^28] JetBrains plans 2.5.0 for December 2026 and 2.5.20 for March 2027.[^26] 2.5.0-Beta2 came out on 2026-10-21.[^29] Kotlin 2.4 made context parameters and explicit backing fields stable.[^30]

| Version | Status | First release | Newest release | End of support |
|---|---|---|---|---|
| 2.5 | beta | 2026-12 (planned) | 2.5.0-Beta2 (2026-10-21) | none stated |
| 2.4 | supported | 2026-06-03 | 2.4.21 (2026-10-08) | 2027-12-03 |
| 2.3 | superseded | 2025-12-16 | 2.3.21 (2026-04-23) | none stated |
| 2.2 | superseded | 2025-06-23 | 2.2.21 (2025-10-23) | none stated |
| 2.1 | superseded | 2024-11-27 | 2.1.21 (2025-05-13) | none stated |
| 2.0 | superseded | 2024-05-21 | 2.0.21 (2024-10-10) | none stated |

The dates come from the release history of the Kotlin documentation, and the GitHub releases agree.[^26][^29]

## Implementations

There is one compiler, and Kotlin/JVM, Kotlin/Native, Kotlin/JS and Kotlin/Wasm are its targets. Each target has its own stability level.[^31] The Apache 2.0 licence applies to all the code of JetBrains in the repository, and some third-party parts have other licences.[^32] Kotlin Multiplatform shares code between Android, iOS, desktop, web and server, and it became stable in 1.9.20.[^33][^31]

| Target | Runs on | Stability | Newest release | Licence |
|---|---|---|---|---|
| Kotlin/JVM | the JVM, Java 8 bytecode by default, and Android | Stable, from 1.0.0 | 2.4.21 (2026-10-08) | Apache-2.0 |
| Kotlin/Native | Linux, Windows (MinGW), Android NDK, macOS, iOS, tvOS, watchOS | Stable, from 1.9.0 | 2.4.21 (2026-10-08) | Apache-2.0 |
| Kotlin/JS | JavaScript engines (ES5, ES2015, ES2020) | Stable, from 1.3.0 | 2.4.21 (2026-10-08) | Apache-2.0 |
| Kotlin/Wasm | browsers with Wasm garbage collection; WASI | Beta, from 2.2.20 | 2.4.21 (2026-10-08) | Apache-2.0 |

Sources: the stability table[^31], the target pages[^2][^20][^21][^19] and the GitHub releases.[^29] Kotlin/JVM is the main column of the compat tables of the feature pages.

## Ecosystem

IntelliJ IDEA and Android Studio support Kotlin through an official plugin from JetBrains, and an official extension for Visual Studio Code is in Alpha. On the JVM, the main build tools are Gradle and Maven.[^34]

| Need | Tool |
|---|---|
| get libraries | Maven Central, where JetBrains publishes signed Kotlin releases[^27] |
| build a project | Gradle; Maven[^34] |
| run tests | JUnit, with Kotlin tests next to Java tests[^37] |
| format and lint | ktlint, a linter with a built-in formatter[^36]; `kotlinc -Werror` makes all warnings errors[^35] |
| document | KDoc comments, and Dokka to generate documentation[^38] |
| try code interactively | the REPL of `kotlinc -Xrepl`[^7] |

On the server, Spring and the JetBrains framework Ktor support Kotlin.[^39] Android development has been Kotlin-first since Google I/O 2019, and Jetpack Compose is the recommended Android toolkit for native UI in Kotlin.[^40]

## History and influence

Andrey Breslav made the first commit in November 2010. He designed the language with Max Shafirov and others who joined later.[^41]

| Date | Event |
|---|---|
| 2011-07-19 | JetBrains shows Kotlin at the JVM Language Summit, as a statically typed language for the JVM[^42] |
| 2016-02-15 | Kotlin 1.0, with a commitment to backward compatibility of the language and kotlin-stdlib on the JVM[^3] |
| 2017-05-17 | Android announces official support for Kotlin and a plan to move Kotlin into a non-profit foundation[^43] |
| 2018-10-04 | Google says that it has established the Kotlin Foundation with JetBrains[^44] |
| 2019 | at Google I/O, Google announces that Android development will be increasingly Kotlin-first[^45] |
| 2023-11-01 | Kotlin 1.9.20, with Kotlin Multiplatform stable[^26] |
| 2024-05-21 | Kotlin 2.0.0, with the K2 compiler stable[^26][^22] |
| 2026-06-03 | Kotlin 2.4.0[^26] |

In 2020 Roman Elizarov became the project lead, and Breslav described the Kotlin Foundation as created by JetBrains and Google.[^41] JetBrains drives the language with the community and the Kotlin Foundation. The Lead Language Designer, Michail Zarečenskij, has the final say on the evolution of the language. A Language Committee under the Foundation must approve incompatible changes to stable components.[^4] A proposal for a feature is a Kotlin Evolution and Enhancement Process (KEEP) document.[^4]

The specification names Java, Scala, C# and Groovy as sources of inspiration.[^1] The FAQ also names JavaScript.[^2]

## Coming from another language

- From [[java]]: Kotlin can call Java code and Java can call Kotlin code.[^16] All exceptions are unchecked, so the compiler does not force a caller to catch them ([[exception-handling]]).[^16][^46] Java types arrive as platform types without null checks.[^16]
- From [[csharp]]: the keyword `suspend` marks a function that can pause without blocking a thread. The coroutine builders `launch` and `async` are extension functions of the kotlinx.coroutines library, not keywords ([[async-await]]).[^24]
- From [[typescript]]: Kotlin/JS compiles Kotlin and the standard library to JavaScript, and the Kotlin Multiplatform Gradle plugin adds npm dependencies.[^20] Subtyping is nominal: a type gets its supertypes from its declaration.[^1]
- From [[rust]]: the program does not free memory by hand, and Kotlin/Native reclaims unreachable objects with a tracing garbage collector.[^18] A sealed interface with data classes and an exhaustive `when` describes a closed set of cases ([[sum-types]]).[^8]

## References
