---
tag: YR9XNB
kind: tool
slug: kotest
name: Kotest
aliases: [KotlinTest, Kotest framework, kotest-runner-junit5]
area: testing/test-frameworks
level: advanced
summary: >-
  Kotest is a test tool for Kotlin with three parts: a test framework with eight spec styles, an assertions library of matchers such as shouldBe, and property testing.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/kotest/kotest
homepage: https://kotest.io
see_also: [kotlin, junit-5, test-framework, unit-testing, property-based-tests, xunit, junit]
sources:
  - url: https://github.com/kotest/kotest
    title: Kotest, README
    locator: the introduction
    accessed: 2026-11-15
  - url: https://kotest.io/docs/quickstart
    title: Kotest, Quick Start
    locator: the introduction; Test Framework; Assertions Library
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/project-setup.html
    title: Kotest, Setup
    locator: JVM
    accessed: 2026-11-15
  - url: https://github.com/kotest/kotest/blob/master/LICENSE
    title: Kotest, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/testing-styles.html
    title: Kotest, Testing Styles
    locator: the introduction; Fun Spec
    accessed: 2026-11-15
  - url: https://kotest.io/docs/assertions/assertions.html
    title: Kotest, Assertions
    locator: Multitude of Matchers; Clues; Inspectors
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/isolation-mode.html
    title: Kotest, Isolation Modes
    locator: the introduction; Single Instance; InstancePerRoot
    accessed: 2026-11-15
  - url: https://kotest.io/docs/assertions/soft-assertions.html
    title: Kotest, Soft Assertions
    locator: the whole page
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/datatesting/data-driven-testing.html
    title: Kotest, Data Driven Testing
    locator: Module Changes; Getting Started
    accessed: 2026-11-15
  - url: https://kotest.io/docs/proptest/property-based-testing.html
    title: Kotest, Property-based Testing
    locator: the introduction; What is Property Testing?
    accessed: 2026-11-15
  - url: https://kotest.io/docs/proptest/property-test-functions.html
    title: Kotest, Property Test Functions
    locator: For All; Check All; Iterations
    accessed: 2026-11-15
  - url: https://kotlinlang.org/api/core/kotlin-stdlib/kotlin.math/abs.html
    title: Kotlin standard library, abs
    locator: the special cases
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/lifecycle-hooks.html
    title: Kotest, Lifecycle hooks
    locator: DSL Methods
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/tags.html
    title: Kotest, Grouping Tests with Tags
    locator: the introduction; Running with Tags
    accessed: 2026-11-15
  - url: https://kotest.io/docs/framework/project-config.html
    title: Kotest, Project Level Config
    locator: the introduction; Setup, JVM
    accessed: 2026-11-15
  - url: https://kotest.io/docs/release6/
    title: Kotest 6.0
    locator: New Features
    accessed: 2026-11-15
  - url: https://kotest.io/docs/changelog.html
    title: Kotest, Changelog
    locator: 5.0.0 November 2021; 4.0.0 March 2020
    accessed: 2026-11-15
  - url: https://github.com/kotest/kotest/releases
    title: Kotest releases on GitHub
    locator: v6.2.7; v6.2.0; v6.1.0; v6.0.0
    accessed: 2026-11-15
---

## What it is

Kotest is "a flexible and comprehensive testing tool for Kotlin with multiplatform support".[^1] It has three parts that work alone or together: a test framework, an assertions library and a library for property testing.[^2] A project can use the Kotest assertions with JUnit, or the Kotest framework with another assertions library.[^2]

On the JVM, the framework runs on the JUnit Platform, the same platform as [[junit-5]].[^3] So a build tool that runs JUnit 5 tests, such as Gradle or Maven, also runs Kotest tests. The framework also works on the Kotlin targets for JavaScript, Wasm and Native.[^3] Its licence is the Apache License 2.0.[^4]

## Install and first use

With Gradle, a project turns on the JUnit Platform for the task `test` and adds the dependency `io.kotest:kotest-runner-junit5`.[^3] The examples on this page use Maven, with these dependencies in `pom.xml`:

```text
<dependency>
  <groupId>io.kotest</groupId>
  <artifactId>kotest-runner-junit5-jvm</artifactId>
  <version>6.2.7</version>
  <scope>test</scope>
</dependency>
<dependency>
  <groupId>io.kotest</groupId>
  <artifactId>kotest-assertions-core-jvm</artifactId>
  <version>6.2.7</version>
  <scope>test</scope>
</dependency>
```

A test file is a class that extends one of the spec styles. In the style `FunSpec`, the function `test` takes a name and a lambda with the body of the test.[^5] The function `shouldBe` is the general matcher for equality.[^6]

```text
import io.kotest.assertions.throwables.shouldThrow
import io.kotest.core.spec.style.FunSpec
import io.kotest.matchers.shouldBe
import io.kotest.matchers.string.shouldContain

data class Item(val name: String, val cents: Int, val quantity: Int)

fun total(items: List<Item>): Int {
    require(items.all { it.quantity >= 0 }) { "negative quantity" }
    return items.sumOf { it.cents * it.quantity }
}

class CartTest : FunSpec({
    test("an empty cart costs nothing") {
        total(emptyList()) shouldBe 0
    }

    test("the total adds the lines") {
        total(listOf(Item("pen", 150, 2), Item("book", 1200, 1))) shouldBe 1500
    }

    test("a negative quantity is an error") {
        val error = shouldThrow<IllegalArgumentException> {
            total(listOf(Item("pen", 150, -1)))
        }
        error.message shouldContain "negative"
    }
})
```

```text
$ mvn test
...
[INFO] Running CartTest
[INFO] Tests run: 3, Failures: 0, Errors: 0, Skipped: 0, Time elapsed: 0.345 s -- in CartTest
```

The runner of this site has no Kotlin compiler with Kotest, so the Kotlin code on this page is text. The outputs were recorded with Kotest 6.2.7, Kotlin 2.4.21, Maven Surefire 3.5.4 and Java 25. Kotest 6 needs a JVM target of 11 or later; with the Kotlin default of 1.8, the compiler stops with an error about inline bytecode, so the build sets the property `kotlin.compiler.jvmTarget`.

## Concepts of the tool

### Spec styles

Kotest has eight styles for tests, and "There are no functional differences between the styles".[^5] Some come from other tools: `FunSpec`, `FreeSpec` and `WordSpec` from ScalaTest, `DescribeSpec` from JavaScript frameworks and RSpec, `BehaviorSpec` from Gherkin, and `FeatureSpec` from Cucumber.[^5] `ShouldSpec`, `StringSpec` and `ExpectSpec` are new in Kotest.[^5] The documentation says to use `FunSpec` when in doubt.[^5]

A `BehaviorSpec` has three levels, `given`, `when` and `then`. The word `when` is a keyword of Kotlin, so the code puts it in backticks.

```text
import io.kotest.core.spec.style.BehaviorSpec
import io.kotest.matchers.shouldBe

class Stock {
    private val counts = mutableMapOf<String, Int>()
    fun add(name: String, n: Int) { counts[name] = count(name) + n }
    fun count(name: String): Int = counts[name] ?: 0
    fun take(name: String): Boolean {
        if (count(name) == 0) return false
        counts[name] = count(name) - 1
        return true
    }
}

class StockTest : BehaviorSpec({
    given("a stock with one pen") {
        val stock = Stock()
        stock.add("pen", 1)
        `when`("a customer takes the pen") {
            val taken = stock.take("pen")
            then("the customer gets it") {
                taken shouldBe true
            }
            then("no pen is left") {
                stock.count("pen") shouldBe 0
            }
        }
    }
})
```

The JUnit XML report of Surefire has two tests, one for each `then`, and the name of each test is the path of names:

```text
<testcase name="a stock with one pen/a customer takes the pen/the customer gets it"
<testcase name="a stock with one pen/a customer takes the pen/no pen is left"
```

### Isolation modes

The code of `given` and `when` runs one time, and both `then` blocks see the same `stock`. This is the default isolation mode of Kotest, `SingleInstance`: one instance of the spec runs all its tests.[^7] The documentation says that this default is the same as in ScalaTest and Jest, but different from JUnit.[^7] The mode `InstancePerRoot`, new in Kotest 6.0, makes a new instance for each top-level test.[^7] The older modes `InstancePerTest` and `InstancePerLeaf` are deprecated.[^7]

### Matchers and soft assertions

Kotest has "over 350 matchers spread across multiple modules".[^6] Matchers come in an infix form, such as `list shouldContain 4`, and a regular form.[^6] A failed matcher throws an exception, so the test stops at the first failure.[^8] In the block `assertSoftly`, the test goes on after a failure, and Kotest reports all the failures together at the end of the block.[^8]

```text
import io.kotest.assertions.assertSoftly
import io.kotest.core.spec.style.FunSpec
import io.kotest.matchers.collections.shouldContain
import io.kotest.matchers.shouldBe

// average has a bug: the division is a division of integers.
fun average(values: List<Int>): Double = (values.sum() / values.size).toDouble()

class AverageTest : FunSpec({
    test("the average of two values") {
        assertSoftly {
            average(listOf(2, 3)) shouldBe 2.5
            listOf(2, 3) shouldContain 4
        }
    }
})
```

```text
$ cat target/surefire-reports/AverageTest.txt
...
AverageTest.the average of two values -- Time elapsed: 0.343 s <<< FAILURE!
io.kotest.assertions.MultiAssertionError:
The following 2 assertions failed:
1) expected:<2.5> but was:<2.0>
   at AverageTest$1$1.invokeSuspend(AverageTest.kt:12)
2) Collection should contain element 4 based on object equality; but the collection is [2, 3]
   at AverageTest$1$1.invokeSuspend(AverageTest.kt:13)
...
```

The second assertion fails on purpose, to show two failures in one report. Kotest also has clues, which add context to a failure message, and inspectors, which check how many elements of a collection pass a test.[^6]

### Data-driven tests

A data test runs the same body for each row of data, and each row is a test with its own name.[^9] In Kotest 6, data tests are part of the core framework, and the old module `kotest-framework-datatest` is not necessary.[^9] The function for a data test matches the style: in `FunSpec`, it is `withTests`.[^9]

```text
import io.kotest.core.spec.style.FunSpec
import io.kotest.datatest.withTests
import io.kotest.matchers.shouldBe

fun discount(cents: Int, code: String): Int = when (code) {
    "TEN" -> cents * 90 / 100
    "HALF" -> cents / 2
    else -> cents
}

data class Case(val code: String, val price: Int)

class DiscountTest : FunSpec({
    context("a price of 1000 cents") {
        withTests(Case("TEN", 900), Case("HALF", 500), Case("NONE", 1000)) { (code, price) ->
            discount(1000, code) shouldBe price
        }
    }
})
```

```text
<testcase name="a price of 1000 cents/Case(code=TEN, price=900)"
<testcase name="a price of 1000 cents/Case(code=HALF, price=500)"
<testcase name="a price of 1000 cents/Case(code=NONE, price=1000)"
```

### Property tests

The module `kotest-property` runs a test many times with generated values; it works with Kotest or with another test framework.[^10] The function `forAll` takes a function that returns a Boolean, and `checkAll` takes a function that uses assertions.[^11] By default, a property test runs 1000 times.[^11] The generated values include edge cases that example tests often miss.[^10]

```text
import io.kotest.core.spec.style.FunSpec
import io.kotest.property.forAll
import kotlin.math.abs

class AbsTest : FunSpec({
    test("abs is never negative") {
        forAll<Int> { n -> abs(n) >= 0 }
    }
})
```

```text
$ cat target/surefire-reports/AbsTest.txt
...
AbsTest.abs is never negative -- Time elapsed: 1.057 s <<< FAILURE!
org.opentest4j.AssertionFailedError:
Property failed after 910 attempts

	Arg 0: -2147483648

Repeat this test by using seed 7803908828178315302

Caused by AssertionFailedError: expected:<true> but was:<false>
...
```

The property is false: the value in the report is `Int.MIN_VALUE`, and `abs(Int.MIN_VALUE)` is `Int.MIN_VALUE`.[^12] The number of attempts and the seed change from run to run; the seed in the report lets a developer repeat the same run. See [[property-based-tests]] for the method.

### Hooks

A spec can define hooks with functions such as `beforeTest`, `afterTest`, `beforeSpec` and `afterSpec`, or it can override functions of the same names.[^13] There is `afterProject` but no `beforeProject` in a spec, because the project has started before a spec runs.[^13]

## Common tasks

```text
mvn test                                          run all specs with Maven
gradle test                                       run all specs with Gradle
gradle test -Dkotest.tags="Linux & !Database"     run only the tests with these tags
KOTEST_TAGS="Linux & !Database" gradle check      the same, with an environment variable
xtest("...") { }                                  disable one test in FunSpec
test("...").config(tags = setOf(Linux)) { }       add a tag to a test
```

The tag commands and the `x` prefix come from the documentation.[^14][^5] A tag is an object that extends `Tag`, or a `NamedTag` with a name.[^14]

## Configuration

Project-wide configuration is a class that extends `AbstractProjectConfig`.[^15] On the JVM, Kotest looks for the class `io.kotest.provided.ProjectConfig`, then for the class in the system property `kotest.framework.config.fqn`, then for a class named `ProjectConfig` in a package common to all the tests.[^15] Configuration on a spec or a test overrides the project configuration.[^15] Kotest 6.0 also added package-level configuration with `AbstractPackageConfig`, and modes to run specs and tests concurrently.[^16]

## Alternatives

- [[junit-5]] with Kotlin: the JUnit API works from Kotlin, and Kotest assertions can be used with it.[^2]
- Spock: the Kotest assertions also work with this framework.[^6]
- ScalaTest: the source of the styles `FunSpec`, `FreeSpec` and `WordSpec`.[^5]

## History and versions

| Version | Change |
|---|---|
| 4.0.0, March 2020 | the project KotlinTest is renamed Kotest, with new packages and module names, and becomes multiplatform[^17] |
| 4.6.0 | the new support for data-driven tests[^9] |
| 5.0.0, November 2021 | Kotlin 1.6 is the minimum version[^17] |
| 6.0.0, 19 August 2025 | the isolation mode `InstancePerRoot`; data tests in the core; concurrency modes for specs and tests; package-level configuration[^18][^7][^9][^16] |
| 6.1.0, 20 January 2026 | data tests with functions that match the spec style; a runner for JUnit 6[^18] |
| 6.2.0, 15 June 2026 | a JVM module for test suites that deprecates `JunitXmlReporter`; isolation modes on all Kotlin Multiplatform targets[^18] |

The latest version on 12 November 2026 was 6.2.7, from 11 November 2026.[^18]

## References
