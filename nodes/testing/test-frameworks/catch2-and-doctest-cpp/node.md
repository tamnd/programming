---
tag: KQPEKH
kind: tool
slug: catch2-and-doctest-cpp
name: Catch2 and doctest
aliases: [Catch2, Catch, doctest, doctest for C++]
area: testing/test-frameworks
level: advanced
summary: >-
  Catch2 and doctest are C++ test frameworks with free-form test names and plain C++ expressions in assertions. This page shows sections, BDD macros, tags, generators and the single header of doctest.
role: [test-framework]
see_also: [googletest, test-framework, cpp, unit-testing, c-test-frameworks, test-driven-development, doctest-python]
sources:
  - url: https://github.com/catchorg/Catch2
    title: Catch2, README
    locator: What is Catch2?; Catch2 v3 has been released!
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
    title: doctest, Tutorial
    locator: A simple example; Test cases and subcases
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/assertions.md
    title: Catch2, Assertion Macros
    locator: Natural Expressions; Exceptions
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/migrate-v2-to-v3.md
    title: Catch2, Migrating from v2 to v3
    locator: the introduction; How to migrate projects from v2 to v3
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/LICENSE.txt
    title: Catch2, LICENSE.txt
    locator: the whole file
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest/blob/master/doc/markdown/faq.md
    title: doctest, FAQ
    locator: How is doctest different from Catch?; Is doctest thread-aware?
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest/blob/master/LICENSE.txt
    title: doctest, LICENSE.txt
    locator: the whole file
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md
    title: Catch2, CMake integration
    locator: CMake targets; Automatic test registration
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md
    title: Catch2, Command line
    locator: Specifying which tests to run; Choosing a reporter to use; Listing available tests, tags or reporters; Order; Test sharding
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/comparing-floating-point-numbers.md
    title: Catch2, Comparing floating point numbers with Catch2
    locator: Floating point matchers
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
    title: Catch2, Tutorial
    locator: Getting Catch2; Writing tests; Test cases and sections
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/test-cases-and-sections.md
    title: Catch2, Test cases and sections
    locator: Tags; Special Tags; BDD-style test cases
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/generators.md
    title: Catch2, Data Generators
    locator: the introduction; Provided generators
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest/blob/master/doc/markdown/configuration.md
    title: doctest, Configuration
    locator: DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN; DOCTEST_CONFIG_DISABLE
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest
    title: doctest, README
    locator: the introduction
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/blob/devel/docs/reporters.md
    title: Catch2, Reporters
    locator: Using different reporters; Using multiple reporters
    accessed: 2026-11-15
  - url: https://github.com/catchorg/Catch2/releases
    title: Catch2 releases on GitHub
    locator: v3.16.1; v3.0.1; v2.0.1
    accessed: 2026-11-15
  - url: https://github.com/doctest/doctest/releases
    title: doctest releases on GitHub
    locator: v2.5.3; v2.5.0; 2.0.0
    accessed: 2026-11-15
---

## What it is

Catch2 and doctest are two test frameworks for [[cpp]] in the same style. A test case has a free-form name in a string, and an assertion is a plain C++ expression such as `CHECK(total == 1500)`.[^1][^2] When the check fails, the framework breaks the expression into its two sides and prints the value of each side.[^3]

Catch2 is "mainly a unit testing framework for C++", with micro-benchmarks and macros for behaviour-driven development (BDD) as well.[^1] Since version 3 it is a normal library with many headers and a part that is compiled separately, and it needs C++14 or later.[^1][^4] Its licence is the Boost Software License 1.0.[^5]

doctest is a single header.[^2] Its FAQ calls it "a very polished, light, stable and clean subset (or reimplementation)" of Catch, and it names its strengths: the header is much lighter to compile, the assertions are thread-safe, and a macro removes all the tests from a binary.[^6] Its licence is the MIT licence.[^7]

[[googletest]] is the other main C++ framework; it uses fixed macros such as `EXPECT_EQ` and test names that are C++ identifiers.

## Install and first use

Catch2 v3 is a library: CMake projects link the target `Catch2::Catch2WithMain`, which supplies a `main` function, and the function `catch_discover_tests` registers each test case with CTest.[^8] Catch2 also ships one header and one source file with all of the library, `catch_amalgamated.hpp` and `catch_amalgamated.cpp`; the migration guide says that this option is not the main one, and that it compiles slower.[^4]

The file below has two test cases with the tag `[cart]`. `REQUIRE` stops the test case when it fails, and `CHECK` records the failure and goes on.[^3]

```cpp test id=first-use deps=deps/cpp/vcpkg.json
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <vector>

struct Item {
    std::string name;
    int cents;
    int quantity;
};

int total(const std::vector<Item>& items) {
    int sum = 0;
    for (const auto& item : items) {
        if (item.quantity < 0) {
            throw std::invalid_argument("negative quantity");
        }
        sum += item.cents * item.quantity;
    }
    return sum;
}

TEST_CASE("an empty cart costs nothing", "[cart]") {
    REQUIRE(total({}) == 0);
}

TEST_CASE("the total adds the lines", "[cart]") {
    std::vector<Item> cart{{"pen", 150, 2}, {"book", 1200, 1}};
    CHECK(total(cart) == 1500);
    CHECK_THROWS_AS(total({{"pen", 150, -1}}), std::invalid_argument);
}
```

```output
Randomness seeded to: $SEED
===============================================================================
All tests passed (3 assertions in 2 test cases)
```

The runner of this site builds the file with `-lCatch2Main -lCatch2`, and runs the program with `--order decl`. Since Catch2 3.9.0, the default order of the test cases is random; `--order decl` runs them in the order of the file.[^9]

## Concepts of the tool

### Assertions and expression decomposition

Most frameworks have one macro for each comparison. Catch2 has `REQUIRE` and `CHECK` for any expression, and it decomposes the expression to show its parts.[^3] `REQUIRE_FALSE` and `CHECK_FALSE` check that an expression is false, and `CHECK_THROWS_AS` checks that an expression throws an exception of a given type.[^3] An expression with `&&` or `||` cannot be decomposed, and does not compile in an assertion.[^3]

The function `average` below divides two integers, so it loses the fraction. The test case fails on purpose.

```cpp test fails expect=test-failure id=decomposition deps=deps/cpp/vcpkg.json
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <vector>

// average has a bug: the division is a division of integers.
double average(const std::vector<int>& values) {
    int sum = 0;
    for (int v : values) {
        sum += v;
    }
    return sum / static_cast<int>(values.size());
}

TEST_CASE("the average of two values", "[average]") {
    using Catch::Matchers::WithinRel;
    const std::vector<int> values{2, 3};
    REQUIRE_FALSE(values.empty());
    CHECK(average(values) == 2.5);
    CHECK_THAT(average(values), WithinRel(2.5, 0.001));
}
```

```output
Randomness seeded to: $SEED

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main_test.exe is a Catch2 v3.16.1 host application.
Run with -? for options

-------------------------------------------------------------------------------
the average of two values
-------------------------------------------------------------------------------
main_test.cpp:15
...............................................................................

main_test.cpp:19: FAILED:
  CHECK( average(values) == 2.5 )
with expansion:
  2.0 == 2.5

main_test.cpp:20: FAILED:
  CHECK_THAT( average(values), WithinRel(2.5, 0.001) )
with expansion:
  2.0 and 2.5 are within 0.1% of each other

===============================================================================
test cases: 1 | 1 failed
assertions: 3 | 1 passed | 2 failed
```

The line `with expansion:` shows the values of the two sides; for a matcher, it shows the value and the description of the matcher that did not match. The test case has three assertions: `REQUIRE_FALSE` passed, and the two checks failed. Floating-point values need care, so Catch2 has the matchers `WithinAbs`, `WithinRel` and `WithinULP`, which `CHECK_THAT` and `REQUIRE_THAT` use.[^10]

### Sections

A test case can contain sections. For each section, the test case "is executed from the start", so each section sees freshly made objects, and the code before the sections works as the set-up of a fixture.[^11] One run of the test case enters only one leaf section, and sections can be nested.[^11]

`SCENARIO`, `GIVEN`, `WHEN` and `THEN` are the BDD form of the same idea: `SCENARIO` is a `TEST_CASE` whose name has the prefix `Scenario: `, and the other three are sections whose names have the prefixes `given: `, `when: ` and `then: `.[^12]

```cpp test id=sections deps=deps/cpp/vcpkg.json
#include <catch2/catch_test_macros.hpp>

#include <map>
#include <string>

class Stock {
public:
    void add(const std::string& name, int count) { counts_[name] += count; }
    bool take(const std::string& name) {
        auto it = counts_.find(name);
        if (it == counts_.end() || it->second == 0) {
            return false;
        }
        --it->second;
        return true;
    }
    int count(const std::string& name) const {
        auto it = counts_.find(name);
        return it == counts_.end() ? 0 : it->second;
    }

private:
    std::map<std::string, int> counts_;
};

TEST_CASE("taking items from the stock", "[stock]") {
    // Catch2 runs this part again before each section.
    Stock stock;
    stock.add("pen", 2);
    REQUIRE(stock.count("pen") == 2);

    SECTION("take lowers the count") {
        REQUIRE(stock.take("pen"));
        CHECK(stock.count("pen") == 1);
    }
    SECTION("an unknown item cannot be taken") {
        CHECK_FALSE(stock.take("book"));
        CHECK(stock.count("pen") == 2);
    }
}

SCENARIO("the last pen is sold", "[stock]") {
    GIVEN("a stock with one pen") {
        Stock stock;
        stock.add("pen", 1);
        WHEN("a customer takes the pen") {
            REQUIRE(stock.take("pen"));
            THEN("no pen is left") {
                CHECK(stock.count("pen") == 0);
                CHECK_FALSE(stock.take("pen"));
            }
        }
    }
}
```

```output
Randomness seeded to: $SEED
===============================================================================
All tests passed (9 assertions in 2 test cases)
```

The option `-s` prints the passed assertions too, with the path of sections for each test case.[^9] The first part of the output for the tag `[stock]` is below. The first `REQUIRE` passes twice, once before each section.

```text
$ ./main_test.exe "[stock]" --order decl -s
Filters: [stock]
Randomness seeded to: $SEED

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
main_test.exe is a Catch2 v3.16.1 host application.
Run with -? for options

-------------------------------------------------------------------------------
taking items from the stock
-------------------------------------------------------------------------------
main_test.cpp:26
...............................................................................

main_test.cpp:30: PASSED:
  REQUIRE( stock.count("pen") == 2 )
with expansion:
  2 == 2

-------------------------------------------------------------------------------
taking items from the stock
  take lowers the count
-------------------------------------------------------------------------------
main_test.cpp:32
...............................................................................

main_test.cpp:33: PASSED:
  REQUIRE( stock.take("pen") )
with expansion:
  true

main_test.cpp:34: PASSED:
  CHECK( stock.count("pen") == 1 )
with expansion:
  1 == 1

-------------------------------------------------------------------------------
taking items from the stock
-------------------------------------------------------------------------------
main_test.cpp:26
...............................................................................

main_test.cpp:30: PASSED:
  REQUIRE( stock.count("pen") == 2 )
with expansion:
  2 == 2

-------------------------------------------------------------------------------
taking items from the stock
  an unknown item cannot be taken
-------------------------------------------------------------------------------
main_test.cpp:36
...............................................................................

main_test.cpp:37: PASSED:
  CHECK_FALSE( stock.take("book") )
with expansion:
  !false

main_test.cpp:38: PASSED:
  CHECK( stock.count("pen") == 2 )
with expansion:
  2 == 2
...
```

### Tags and test specs

A test case can have tags in square brackets, such as `[cart]` and `[stock]`.[^12] On the command line, a test spec selects the test cases to run: a name, a name with `*` at the start or end, or a tag. Specs written together must all match, specs joined with a comma are alternatives, and `~` negates a spec.[^9] A tag that starts with a dot, such as `[.integration]`, hides a test case, so it runs only when a test spec names it.[^12]

### Generators

`GENERATE` gives a list of values, and Catch2 runs the test case once for each value.[^13] Two generators in one test case give the cartesian product of their values.[^13] `CAPTURE` logs the values of variables, and Catch2 prints them with the result of an assertion.

```cpp test id=generators deps=deps/cpp/vcpkg.json
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <string>

int discount(int total, const std::string& code) {
    if (code == "TEN") {
        return total - total * 10 / 100;
    }
    if (code == "HALF") {
        return total / 2;
    }
    return total;
}

TEST_CASE("a discount never raises the price", "[discount]") {
    // The test case runs once for each value of each generator.
    auto code = GENERATE(as<std::string>{}, "TEN", "HALF", "NONE");
    auto total = GENERATE(0, 99, 1000);
    CAPTURE(code, total);
    CHECK(discount(total, code) <= total);
}
```

```output
Randomness seeded to: $SEED
===============================================================================
All tests passed (9 assertions in 1 test case)
```

The test case ran 9 times, 3 codes times 3 totals, so the summary counts 9 assertions in 1 test case. With `-s`, each assertion shows the captured values under `with messages:`. The second run of the test case gives this part of the output:

```text
-------------------------------------------------------------------------------
a discount never raises the price
-------------------------------------------------------------------------------
main_test.cpp:16
...............................................................................

main_test.cpp:21: PASSED:
  CHECK( discount(total, code) <= total )
with expansion:
  90 <= 99
with messages:
  code := "TEN"
  total := 99
```

### doctest

doctest keeps most of the macros of Catch2, and its FAQ shows how to move tests from Catch to doctest.[^6] `SUBCASE` is its name for a section.[^2] One file defines `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` before it includes `doctest.h`, and this file gets the implementation and a `main` function.[^2][^14] The runner of this site has no doctest package, so the block below is text, and the output was recorded with doctest 2.5.3 and GCC 15.

```text
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <map>
#include <string>

int discount(int total, const std::string& code) {
    if (code == "TEN") {
        return total - total * 10 / 100;
    }
    return total;
}

TEST_CASE("discount codes") {
    CHECK(discount(1000, "TEN") == 900);
    CHECK(discount(1000, "NONE") == 1000);
}

TEST_CASE("a stock of pens") {
    std::map<std::string, int> stock{{"pen", 2}};
    REQUIRE(stock["pen"] == 2);

    SUBCASE("one pen is sold") {
        stock["pen"] -= 1;
        CHECK(stock["pen"] == 1);
    }
    SUBCASE("a delivery adds pens") {
        stock["pen"] += 3;
        CHECK(stock["pen"] == 5);
    }
}

TEST_CASE("a check that fails") {
    CHECK(discount(1000, "HALF") == 500);
}
```

```text
[doctest] doctest version is "2.5.3"
[doctest] run with "--help" for options
===============================================================================
main.cpp:33:
TEST CASE:  a check that fails

main.cpp:34: ERROR: CHECK( discount(1000, "HALF") == 500 ) is NOT correct!
  values: CHECK( 1000 == 500 )

===============================================================================
[doctest] test cases: 3 | 2 passed | 1 failed | 0 skipped
[doctest] assertions: 7 | 6 passed | 1 failed |
[doctest] Status: FAILURE!
```

The function `discount` in this file knows only the code `TEN`, so the third test case fails. doctest has no matchers and no benchmarks; it has no tags either, but its FAQ shows how to emulate them.[^6] Its assertions can be used from many threads that one test case starts, but the test cases themselves run one after another.[^6]

With `DOCTEST_CONFIG_DISABLE` defined, everything for testing is removed from the binary, with every test case, so tests can stay in the production code.[^6][^14] The README compares this with languages such as D and Rust, where tests are written next to the code.[^15]

## Common tasks

```text
./tests                         run all the test cases that are not hidden
./tests "[cart]"                run the test cases with the tag cart
./tests "[cart],[stock]"        run the test cases with either tag
./tests "~[slow]"               run all test cases except those with the tag slow
./tests "the total*"            run the test cases whose names start with the total
./tests --list-tests            list the test cases and their tags
./tests -s                      also print the assertions that pass
./tests --order rand --rng-seed 42   run in a fixed random order
./tests --reporter JUnit::out=report.xml --reporter console::out=-
                                write a JUnit report and print the console output
./tests --shard-count 4 --shard-index 0   run one of four parts of the tests
```

These are options of Catch2.[^9][^16] A doctest program also responds to options on the command line, and with no options it runs all the test cases and returns 0 on success and 1 on a failure.[^2]

## Configuration

Catch2 and doctest have no configuration file. Catch2 is configured with options on the command line.[^9] doctest uses macros with the prefix `DOCTEST_CONFIG_`, such as `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN` and `DOCTEST_CONFIG_DISABLE`.[^14]

Catch2 has 9 reporters, and more than one can be active in a run: for example a JUnit report in a file and the console output on the screen.[^16]

## Alternatives

- [[googletest]]: the C++ framework of Google, with fixtures, death tests and gMock.
- [[c-test-frameworks]]: Unity, CMocka, Check and Criterion for C code.
- [[doctest-python]] is a different tool with a similar name: it runs the examples in the docstrings of Python.

## History and versions

| Version | Change |
|---|---|
| Catch2 2.0.1, 5 November 2017 | the first full release of Catch2; C++98 support is removed[^17] |
| doctest 2.0.0, 23 August 2018 | doctest moves to C++11, and its assertions become thread-safe[^18] |
| Catch2 3.0.1, 17 May 2022 | Catch2 is a static library with many headers, and needs C++14[^17][^4] |
| Catch2 3.9.0 | random order is the default[^9] |
| doctest 2.5.0, 27 March 2026 | support for `volatile` pointers, and documentation of the use with `FetchContent` of CMake[^18] |
| Catch2 3.16.1, 8 October 2026 | fixes of reporters and compile times[^17] |

The latest versions on 12 November 2026 were Catch2 3.16.1, from 8 October 2026, and doctest 2.5.3, from 6 July 2026.[^17][^18]

## References
