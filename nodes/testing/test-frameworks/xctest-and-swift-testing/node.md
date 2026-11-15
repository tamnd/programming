---
tag: SSDHBW
kind: tool
slug: xctest-and-swift-testing
name: XCTest and Swift Testing
aliases: [XCTest, Swift Testing, swift-testing, swift-corelibs-xctest, XCTestCase]
area: testing/test-frameworks
level: core
summary: >-
  XCTest and Swift Testing are the test frameworks of Swift. XCTest uses XCTestCase classes and XCTAssert functions. Swift Testing uses @Test functions and the #expect macro.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/swiftlang/swift-testing
homepage: https://developer.apple.com/documentation/testing
see_also: [test-framework, unit-testing, xunit, junit-5, kotest, googletest, async-await, test-double]
sources:
  - url: https://developer.apple.com/documentation/xctest
    title: Apple Developer Documentation, XCTest
    locator: the overview
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/testing
    title: Apple Developer Documentation, Swift Testing
    locator: the overview
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/xctest/xctestcase
    title: Apple Developer Documentation, XCTestCase
    locator: the overview
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/testing/migratingfromxctest
    title: Apple Developer Documentation, Migrating a test from XCTest
    locator: all sections
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift-testing
    title: Swift Testing, README
    locator: Overview; Cross-platform support; Documentation
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift-corelibs-xctest
    title: swift-corelibs-xctest, README
    locator: Using XCTest; Standalone Command Line Usage
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift-testing/blob/main/LICENSE.txt
    title: Swift Testing, LICENSE.txt
    locator: the whole file
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift-corelibs-xctest/blob/main/LICENSE
    title: swift-corelibs-xctest, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/testing/expectations
    title: Apple Developer Documentation, Expectations and confirmations
    locator: the overview
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/testing/parameterizedtesting
    title: Apple Developer Documentation, Implementing parameterized tests
    locator: the overview; Test with more than one collection
    accessed: 2026-11-15
  - url: https://developer.apple.com/documentation/testing/parallelization
    title: Apple Developer Documentation, Running tests serially or in parallel
    locator: the overview
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift/releases
    title: Swift releases on GitHub
    locator: swift-6.4.2-RELEASE; swift-6.4.0-RELEASE; swift-6.0-RELEASE
    accessed: 2026-11-15
  - url: https://github.com/swiftlang/swift-testing/releases
    title: Swift Testing releases on GitHub
    locator: swift-6.4.2-RELEASE
    accessed: 2026-11-15
---

## What it is

Swift has two test frameworks. XCTest is the older one. Apple describes it with the words "Create and run unit tests, performance tests, and UI tests for your Xcode project."[^1] Swift Testing is the newer one. Its documentation says "Create and run tests for your Swift packages and Xcode projects."[^2]

Xcode 16 and later includes Swift Testing, and Apple recommends it for new unit tests.[^1] A test target can contain tests of the two frameworks, but one test must not mix the APIs of the two.[^1] For user interface tests and performance tests, Apple tells developers to continue to use XCTest.[^1]

XCTest has the shape of [[xunit]]. A test class is a subclass of `XCTestCase`, and a test is a method whose name starts with `test`.[^3][^4] Swift Testing uses macros. The attribute `@Test` marks a test function, and the macro `#expect` checks a condition.[^5][^4]

On platforms other than those of Apple, XCTest comes from the open-source project swift-corelibs-xctest. This project implements most of the XCTest APIs of Xcode 7 and later.[^6] Swift Testing is in the official Swift toolchains for Apple platforms, Linux and Windows.[^5] Swift Testing and swift-corelibs-xctest both use the Apache License 2.0 with a Runtime Library Exception.[^7][^8]

## Install and first use

Neither framework is a package dependency. Swift Testing is included with the Swift 6 toolchain and Xcode 16.[^5] With the Swift Package Manager, the test files are in the subdirectory `Tests` of the package, and the command `swift test` builds and runs them.[^6]

The site does not run the Swift code on this page, because its build has no Swift toolchain. This XCTest test case tests a class `Cart` in a module `Shop`:

```text
import XCTest
@testable import Shop

final class CartTests: XCTestCase {
    var cart: Cart!

    override func setUp() {
        super.setUp()
        cart = Cart()
    }

    func testEmptyCartIsZero() {
        XCTAssertEqual(cart.total, 0)
    }

    func testAddItem() throws {
        cart.add(price: 250, quantity: 2)
        XCTAssertEqual(cart.total, 500)
        let first = try XCTUnwrap(cart.items.first)
        XCTAssertEqual(first.quantity, 2)
    }
}
```

A test case is a group of related test methods, with optional setup and teardown before and after the tests.[^3] Here `setUp()` makes a new cart for each test method.

These are the same tests in Swift Testing:

```text
import Testing
@testable import Shop

struct CartTests {
    let cart = Cart()

    @Test func emptyCartIsZero() {
        #expect(cart.total == 0)
    }

    @Test func addItem() throws {
        cart.add(price: 250, quantity: 2)
        #expect(cart.total == 500)
        let first = try #require(cart.items.first)
        #expect(first.quantity == 2)
    }
}
```

Swift Testing calls a type that holds tests a suite. A suite does not have to be a class or inherit from `XCTestCase`, and Apple recommends a structure or an actor.[^4] The initializer `init()` replaces `setUp()`, and `deinit` replaces `tearDown()`. A suite that needs `deinit` must be a class or an actor.[^4] A test function can have any name, because the attribute `@Test` identifies it.[^4]

When an expectation fails, Swift Testing shows the values in the expression. The README gives this example, with the message in the comment:[^5]

```text
@Test func helloWorld() {
  let greeting = "Hello, world!"
  #expect(greeting == "Hello") // Expectation failed: (greeting → "Hello, world!") == "Hello"
}
```

## Concepts of the tool

### Assertions and expectations

XCTest has a family of approximately 40 assertion functions, with the collective name `XCTAssert()`.[^4] Swift Testing has two macros in their place. After `#expect` fails, the test continues to run. `#require` throws an error of type `ExpectationFailedError`, and this stops the test.[^9] With an optional value, `try #require(x)` unwraps the value, as `XCTUnwrap` does in XCTest.[^4]

| XCTest | Swift Testing |
|---|---|
| `XCTAssert(x)`, `XCTAssertTrue(x)` | `#expect(x)` |
| `XCTAssertFalse(x)` | `#expect(!x)` |
| `XCTAssertNil(x)` | `#expect(x == nil)` |
| `XCTAssertEqual(x, y)` | `#expect(x == y)` |
| `XCTAssertIdentical(x, y)` | `#expect(x === y)` |
| `XCTAssertLessThan(x, y)` | `#expect(x < y)` |
| `XCTAssertThrowsError(try f())` | `#expect(throws: (any Error).self) { try f() }` |
| `XCTAssertNoThrow(try f())` | `#expect(throws: Never.self) { try f() }` |
| `try XCTUnwrap(x)` | `try #require(x)` |
| `XCTFail(...)` | `Issue.record(...)` |

The table is a part of the migration guide.[^4] Swift Testing has no equivalent of `XCTAssertEqual` with an accuracy argument. To compare two numbers within an accuracy, Apple tells developers to use `isApproximatelyEqual()` from swift-numerics.[^4]

In XCTest, a test case can set `continueAfterFailure` to false, so that a test stops at its first failure. XCTest stops the test with an Objective-C exception, and swift-corelibs-xctest does not fully support this property on platforms other than those of Apple.[^4] Swift Testing does not use exceptions to stop a test. `#require` throws a Swift error.[^4]

### Parameterized tests

```text
@Test("discount codes", arguments: zip(["TEN", "HALF", "NONE"], [900, 500, 1000]))
func discountCodes(code: String, price: Int) {
    #expect(Pricing.apply(code, to: 1000) == price)
}
```

The argument `arguments` makes one test function into many test cases, one for each input.[^10] With two collections and no `zip`, Swift Testing runs every combination of the inputs, the Cartesian product. In the example of the documentation, five foods and 100 order sizes give 500 test cases.[^10] With `zip`, Swift Testing pairs the inputs and gives each pair to the two parameters, so the test above has three test cases.[^10] By default, the test cases of one test function run in parallel.[^10]

### Traits, tags and skips

A trait changes how a test runs. A trait can give a condition for the test, limit the test to some operating system versions, or set a time limit.[^5] The trait `.enabled(if:)` runs a test only when a condition is true, and the trait `.tags(...)` puts a test in a group, so that a run can select it.[^5]

```text
@Test(.enabled(if: AppFeatures.isCommentingEnabled))
func videoCommenting() async throws { ... }

@Test(.tags(.metadata))
func `Check video metadata`() { ... }
```

These two declarations come from the README.[^5] The second test has a raw identifier as its name, so its name can contain spaces.[^5]

To skip a test, XCTest code throws `XCTSkip`, or it calls `XCTSkipIf()` or `XCTSkipUnless()`.[^4] Swift Testing uses a condition trait to skip a test before it starts, and `Test.cancel(...)` to end a running test early without a failure.[^4] For a known bug, XCTest has `XCTExpectFailure`, and Swift Testing has `withKnownIssue`.[^4]

### Asynchronous tests

In both frameworks, a test can be `async`, `throws` or both.[^3][^4] XCTest uses `XCTestExpectation` for an asynchronous condition, and a test waits for the expectation with `XCTWaiter`.[^4] Swift Testing has confirmations in place of expectations. A confirmation does not block or suspend the caller. The code must confirm it before the function `confirmation()` returns, or Swift Testing records an issue.[^4]

XCTest runs synchronous test methods on the main actor by default. Swift Testing runs all test functions on an arbitrary task. A test that must run on the main thread needs the attribute `@MainActor`.[^4] See [[async-await]].

### Parallel runs

Swift Testing runs tests in parallel by default. It uses task groups, and the tests usually run in the same process.[^11] The trait `.serialized` on a test or a suite runs its test cases one at a time. On a suite, the trait applies to all tests and suites in it.[^11] The option `--no-parallel` of `swift test` turns off parallel runs for all tests.[^11]

### Interoperability

With interoperability, the assertions of XCTest work in Swift Testing tests, and the expectations of Swift Testing work in XCTest tests.[^4] Without it, a Swift Testing test passes even when a helper function in the test calls `XCTAssertEqual` and the assertion fails.[^4] The default mode depends on the toolchain and on the swift-tools-version of the package:[^4]

| Toolchain | swift-tools-version | Default mode |
|---|---|---|
| older than 6.4 | any | `none` |
| 6.4 or later | older than 6.4 | `limited` |
| 6.4 or later | 6.4 or later | `complete` |

In the mode `none`, a failure from the other framework gives no message. In `limited`, it gives a warning. In `complete`, it gives a failure and a warning to replace the XCTest API. In `strict`, a call to an XCTest API from Swift Testing is a fatal error.[^4] The environment variable `SWIFT_TESTING_XCTEST_INTEROP_MODE` selects a mode.[^4]

## Common tasks

```text
swift test                                 build the package and run all its tests
swift test --no-parallel                   run all tests one at a time
./FooTests Tests.FooTestCase/testFoo       run one XCTest method, without SwiftPM
./FooTests Tests.FooTestCase               run all methods of one test case
./FooTests --list-tests                    list the tests and do not run them
SWIFT_TESTING_XCTEST_INTEROP_MODE=strict swift test    run with the strict mode
```

The `swift test` commands come from the README of swift-corelibs-xctest and the Swift Testing documentation.[^6][^11] Without SwiftPM, a project with swift-corelibs-xctest makes its own executable, and the function `XCTMain` runs the tests. `XCTMain` does not return. The executable exits with 0 for success or 1 for failure.[^6]

## Configuration

Most Swift Testing configuration is in the code, as traits on tests and suites.[^5] The trait `.serialized` on a suite also applies to the tests in the suite.[^11] The environment variable `SWIFT_TESTING_XCTEST_INTEROP_MODE` sets the interoperability mode.[^4] In XCTest, a test case sets properties such as `continueAfterFailure`, and it overrides `setUp()` and `tearDown()`.[^4]

## Alternatives

- XCTest: for user interface tests and performance tests, and for tests that must build with a toolchain older than Swift 6.[^1][^5]
- Swift Testing: for new unit tests.[^1]
- Frameworks of the same kind for other languages are [[junit-5]] for Java and [[kotest]] for Kotlin.

## History and versions

| Version | Change |
|---|---|
| Xcode 7 | swift-corelibs-xctest implements most of the XCTest APIs of Xcode 7 and later[^6] |
| Swift 6.0, 17 September 2024 | Swift Testing is included in the Swift 6 toolchain and in Xcode 16[^5][^12] |
| Swift 6.4.0, 15 September 2026 | interoperability is on by default, in the mode `limited` or `complete`[^4][^12] |

The latest Swift release on 12 November 2026 was 6.4.2, from 30 October 2026.[^12] Swift Testing has a release with the same name and date.[^13]

## References
