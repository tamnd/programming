---
tag: 27B56T
kind: tool
slug: googletest
name: GoogleTest
aliases: [gtest, Google Test, Google C++ Testing Framework, GoogleMock, gMock]
area: testing/test-frameworks
level: core
summary: >-
  GoogleTest is the C++ test framework of Google. This page shows TEST and TEST_F, assertions, fixtures, parameterized and death tests, the flags of a test program, and mocks with gMock.
role: [test-framework]
licence: bsd-3-clause
repo: https://github.com/google/googletest
homepage: https://google.github.io/googletest/
see_also: [test-framework, cpp, unit-testing, test-double, xunit, catch2-and-doctest-cpp, c-test-frameworks, junit]
sources:
  - url: https://github.com/google/googletest
    title: GoogleTest, README
    locator: Welcome to GoogleTest; Announcements; Features; Supported Platforms
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/primer.html
    title: GoogleTest Primer
    locator: Introduction; Beware of the Nomenclature; Assertions; Simple Tests; Test Fixtures; Invoking the Tests; Writing the main() Function
    accessed: 2026-11-15
  - url: https://github.com/google/googletest/blob/main/LICENSE
    title: Google Inc., GoogleTest LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/quickstart-cmake.html
    title: GoogleTest, Quickstart, Building with CMake
    locator: Set up a project; Create and run a binary
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/
    title: GoogleTest User's Guide
    locator: the navigation, Get Started
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/reference/assertions.html
    title: GoogleTest, Assertions Reference
    locator: Generalized Assertion; Boolean Conditions; Binary Comparison; String Comparison; Floating-Point Comparison; Exception Assertions; Death Assertions
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/advanced.html
    title: GoogleTest, Advanced GoogleTest Topics
    locator: Skipping test execution; Death Tests; Value-Parameterized Tests; Typed Tests; Running Test Programs, Advanced Options; Generating an XML Report; Generating a JSON Report
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/reference/testing.html
    title: GoogleTest, Testing Reference
    locator: TEST; TEST_F; TEST_P; INSTANTIATE_TEST_SUITE_P; Classes and types, Test
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/gmock_for_dummies.html
    title: GoogleTest, gMock for Dummies
    locator: What Is gMock?; Setting Expectations
    accessed: 2026-11-15
  - url: https://google.github.io/googletest/reference/matchers.html
    title: GoogleTest, Matchers Reference
    locator: Generic Comparison; String Matchers; Container Matchers
    accessed: 2026-11-15
  - url: https://github.com/google/googletest/releases
    title: GoogleTest releases on GitHub
    locator: v1.18.0; v1.17.0; v1.16.0; v1.13.0; release-1.12.1; release-1.11.0; release-1.10.0; release-1.8.1
    accessed: 2026-11-15
---

## What it is

GoogleTest is a test framework for [[cpp]] from Google.[^1] The same repository holds GoogleMock (gMock), a library for mock objects; the README says that the repository "is a merger of the formerly separate GoogleTest and GoogleMock projects".[^1]

GoogleTest is based on the [[xunit]] architecture, so its words are close to those of [[junit]]: a test belongs to a test suite, and tests in a suite can share a fixture.[^2][^1] GoogleTest finds the tests by itself, so a test program does not need a list of its tests.[^1][^2]

GoogleTest needs C++17 or a later standard.[^1] It is open source under a BSD-style licence with three clauses, and the licence names Google Inc. and the year 2008.[^3]

## Install and first use

GoogleTest is a library that a project builds together with its tests. The quickstart for CMake gets the source with `FetchContent`, links the test program to the target `GTest::gtest_main`, and registers the tests with `gtest_discover_tests`, so that `ctest` runs them.[^4] The user's guide also has a quickstart for Bazel, and a page about pkg-config.[^5]

The test file below has three tests. `TEST(TotalTest, AddsTheLines)` defines the test `AddsTheLines` in the test suite `TotalTest`.[^2] The names must be valid C++ identifiers, and they should not contain an underscore (`_`).[^2]

```cpp test id=first-use deps=deps/cpp/vcpkg.json
#include <gtest/gtest.h>

#include <stdexcept>
#include <string>
#include <vector>

struct Item {
    std::string name;
    int cents;
    int quantity;
};

// total gives the sum of the lines of a cart in cents.
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

TEST(TotalTest, EmptyCartIsZero) {
    EXPECT_EQ(total({}), 0);
}

TEST(TotalTest, AddsTheLines) {
    std::vector<Item> cart{{"pen", 150, 2}, {"book", 1200, 1}};
    EXPECT_EQ(total(cart), 1500);
}

TEST(TotalTest, RejectsANegativeQuantity) {
    EXPECT_THROW(total({{"pen", 150, -1}}), std::invalid_argument);
}
```

```output
Running main() from gtest_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from TotalTest
[ RUN      ] TotalTest.EmptyCartIsZero
[       OK ] TotalTest.EmptyCartIsZero ($TIME)
[ RUN      ] TotalTest.AddsTheLines
[       OK ] TotalTest.AddsTheLines ($TIME)
[ RUN      ] TotalTest.RejectsANegativeQuantity
[       OK ] TotalTest.RejectsANegativeQuantity ($TIME)
[----------] 3 tests from TotalTest ($TIME total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. ($TIME total)
[  PASSED  ] 3 tests.
```

The file has no `main` function. The program is linked to the library `gtest_main`, which supplies a `main` that runs all the tests with `RUN_ALL_TESTS()`.[^2] `RUN_ALL_TESTS()` returns 0 when all the tests pass and 1 when a test fails, and a `main` function must return this value, because a test service reads the exit code and not the text.[^2] The runner of this site builds the program with a command of this form:

```text
c++ -std=c++23 -Wall -o main_test.exe main_test.cpp -lgtest_main -lgtest -pthread
./main_test.exe
```

## Concepts of the tool

### ASSERT and EXPECT

An assertion is a macro. Each check has two forms: `ASSERT_*` makes a fatal failure and returns from the current function, and `EXPECT_*` makes a nonfatal failure and lets the test go on.[^2] The primer recommends `EXPECT_*`, because a test can then report more than one failure in a run.[^2] A test can stream its own failure message into an assertion with `<<`.[^2]

The function `average` below has a bug: it divides two integers, so the result loses its fraction. The test fails on purpose.

```cpp test fails expect=test-failure id=assertions deps=deps/cpp/vcpkg.json
#include <gtest/gtest.h>

#include <vector>

// average has a bug: the division is a division of integers.
double average(const std::vector<int>& values) {
    int sum = 0;
    for (int v : values) {
        sum += v;
    }
    return sum / static_cast<int>(values.size());
}

TEST(AverageTest, OfTwoValues) {
    std::vector<int> values{2, 3};
    ASSERT_FALSE(values.empty());
    EXPECT_DOUBLE_EQ(average(values), 2.5) << "for the values 2 and 3";
    EXPECT_EQ(average({4, 4}), 4.0);
}
```

```output
Running main() from gtest_main.cc
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from AverageTest
[ RUN      ] AverageTest.OfTwoValues
main_test.cpp:17: Failure
Expected equality of these values:
  average(values)
    Which is: 2
  2.5
for the values 2 and 3

[  FAILED  ] AverageTest.OfTwoValues ($TIME)
[----------] 1 test from AverageTest ($TIME total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. ($TIME total)
[  PASSED  ] 0 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] AverageTest.OfTwoValues

 1 FAILED TEST
```

The failure shows the file, the line, the two expressions and the value of each. `EXPECT_DOUBLE_EQ` compares two `double` values and accepts a difference of 4 ULPs (units in the last place) or less; `EXPECT_NEAR` takes an explicit bound.[^6] The second `EXPECT_EQ` passes, because the average of 4 and 4 is 4 with or without the fraction.

| Macro | It passes when |
|---|---|
| `EXPECT_TRUE(c)`, `EXPECT_FALSE(c)` | the condition is true or false[^6] |
| `EXPECT_EQ(a, b)`, `EXPECT_NE`, `EXPECT_LT`, `EXPECT_LE`, `EXPECT_GT`, `EXPECT_GE` | the comparison of the two values is true[^6] |
| `EXPECT_STREQ(a, b)` | two C strings have the same contents; `EXPECT_EQ` compares the pointers[^6] |
| `EXPECT_DOUBLE_EQ(a, b)`, `EXPECT_NEAR(a, b, bound)` | two floating-point values are almost equal[^6] |
| `EXPECT_THROW(s, T)`, `EXPECT_NO_THROW(s)` | the statement throws an exception of type `T`, or throws no exception[^6] |
| `EXPECT_THAT(value, matcher)` | the value matches a matcher of gMock[^6] |
| `EXPECT_DEATH(s, regex)` | the statement ends the process with an exit status that is not 0, and its standard error matches the regular expression[^6] |

Each of these has an `ASSERT_` form.[^6]

### Fixtures with TEST_F

When tests use the same objects, a class that derives from `testing::Test` holds them. This class is a test fixture: `SetUp()` prepares the objects and `TearDown()` releases them, and a test that uses the fixture is defined with `TEST_F`.[^2] For each `TEST_F`, GoogleTest makes a fresh fixture object, calls `SetUp()`, runs the test, calls `TearDown()` and deletes the object, so one test cannot change the data of another test.[^2]

The fixture below puts two pens in a stock. The test `EachTestGetsANewFixture` sees two pens, although the test before it took one.

```cpp test id=fixture deps=deps/cpp/vcpkg.json
#include <gtest/gtest.h>

#include <cstdlib>
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

// The fixture: GoogleTest makes a new StockTest object for each test.
class StockTest : public testing::Test {
protected:
    void SetUp() override { stock_.add("pen", 2); }
    Stock stock_;
};

TEST_F(StockTest, TakeLowersTheCount) {
    ASSERT_TRUE(stock_.take("pen"));
    EXPECT_EQ(stock_.count("pen"), 1);
}

TEST_F(StockTest, EachTestGetsANewFixture) {
    EXPECT_EQ(stock_.count("pen"), 2);
}

TEST_F(StockTest, CannotTakeAnUnknownItem) {
    EXPECT_FALSE(stock_.take("book"));
}

TEST_F(StockTest, NeedsTheWarehouse) {
    if (std::getenv("WAREHOUSE_URL") == nullptr) {
        GTEST_SKIP() << "set WAREHOUSE_URL to run this test";
    }
}

TEST_F(StockTest, DISABLED_CountsAllItems) {
    // Disabled: the test compiles, but it does not run.
}
```

```output
Running main() from gtest_main.cc
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from StockTest
[ RUN      ] StockTest.TakeLowersTheCount
[       OK ] StockTest.TakeLowersTheCount ($TIME)
[ RUN      ] StockTest.EachTestGetsANewFixture
[       OK ] StockTest.EachTestGetsANewFixture ($TIME)
[ RUN      ] StockTest.CannotTakeAnUnknownItem
[       OK ] StockTest.CannotTakeAnUnknownItem ($TIME)
[ RUN      ] StockTest.NeedsTheWarehouse
main_test.cpp:49: Skipped
set WAREHOUSE_URL to run this test

[  SKIPPED ] StockTest.NeedsTheWarehouse ($TIME)
[ DISABLED ] StockTest.DISABLED_CountsAllItems
[----------] 4 tests from StockTest ($TIME total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. ($TIME total)
[  PASSED  ] 3 tests.
[  SKIPPED ] 1 test, listed below:
[  SKIPPED ] StockTest.NeedsTheWarehouse

  YOU HAVE 1 DISABLED TEST
```

`GTEST_SKIP()` stops a test at run time and marks it as skipped, and it can also be in `SetUp()`.[^7] A test whose name starts with `DISABLED_` compiles but does not run, and GoogleTest prints a banner as a reminder, here `YOU HAVE 1 DISABLED TEST`; the flag `--gtest_also_run_disabled_tests` runs it.[^7]

### Parameterized tests and death tests

A value-parameterized test runs the same test with different values.[^7] The fixture derives from `testing::TestWithParam<T>`, the test is defined with `TEST_P` and reads its value with `GetParam()`, and `INSTANTIATE_TEST_SUITE_P` gives the values.[^7][^8] Typed tests run the same test with a list of types.[^7]

A death test checks that a statement ends the process. `EXPECT_DEATH` runs the statement in a child process, and checks the exit status and the standard error of the child.[^6] The documentation asks that the name of a test suite with death tests ends in `DeathTest`, and GoogleTest runs these suites before all other suites.[^7]

```cpp test id=parameterized-and-death deps=deps/cpp/vcpkg.json
#include <gtest/gtest.h>

#include <cstdlib>
#include <iostream>
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

struct Case {
    std::string code;
    int want;
};

class DiscountTest : public testing::TestWithParam<Case> {};

TEST_P(DiscountTest, GivesThePrice) {
    const Case& c = GetParam();
    EXPECT_EQ(discount(1000, c.code), c.want);
}

INSTANTIATE_TEST_SUITE_P(Codes, DiscountTest,
                         testing::Values(Case{"TEN", 900}, Case{"HALF", 500},
                                         Case{"NONE", 1000}));

void check_quantity(int quantity) {
    if (quantity < 0) {
        std::cerr << "negative quantity\n";
        std::abort();
    }
}

TEST(QuantityDeathTest, NegativeQuantityStopsTheProgram) {
    EXPECT_DEATH(check_quantity(-1), "negative quantity");
}
```

```output
Running main() from gtest_main.cc
[==========] Running 4 tests from 2 test suites.
[----------] Global test environment set-up.
[----------] 1 test from QuantityDeathTest
[ RUN      ] QuantityDeathTest.NegativeQuantityStopsTheProgram
[       OK ] QuantityDeathTest.NegativeQuantityStopsTheProgram ($TIME)
[----------] 1 test from QuantityDeathTest ($TIME total)

[----------] 3 tests from Codes/DiscountTest
[ RUN      ] Codes/DiscountTest.GivesThePrice/0
[       OK ] Codes/DiscountTest.GivesThePrice/0 ($TIME)
[ RUN      ] Codes/DiscountTest.GivesThePrice/1
[       OK ] Codes/DiscountTest.GivesThePrice/1 ($TIME)
[ RUN      ] Codes/DiscountTest.GivesThePrice/2
[       OK ] Codes/DiscountTest.GivesThePrice/2 ($TIME)
[----------] 3 tests from Codes/DiscountTest ($TIME total)

[----------] Global test environment tear-down
[==========] 4 tests from 2 test suites ran. ($TIME total)
[  PASSED  ] 4 tests.
```

The output shows the suite `QuantityDeathTest` first, although it is last in the file. The name of each parameterized test has the prefix from `INSTANTIATE_TEST_SUITE_P` and the index of the value: `Codes/DiscountTest.GivesThePrice/0` is the first value.[^7]

### Mocks with gMock

gMock makes mock classes. The gMock documentation separates two kinds of [[test-double]]: a fake has a working implementation that takes a shortcut, and a mock is an object with expectations about the calls that it will receive.[^9] `MOCK_METHOD` defines a method of a mock class, and `EXPECT_CALL` sets an expectation with matchers for the arguments, a number of calls (`Times`) and actions such as `WillOnce(Return(value))`.[^9] The expectations must be set before the code under test calls the mock.[^9]

The test below checks that `notify` sends one mail to each customer. `HasSubstr`, `ElementsAre` and `Contains` are matchers; `EXPECT_THAT` uses them on any value.[^10] The program is linked to `gmock_main` and `gmock` in place of `gtest_main`, and the runner of this site does not link these libraries, so the block is text and the output was recorded with GoogleTest 1.18.0.

```text
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

// Mailer is the interface of a service that sends mail.
class Mailer {
public:
    virtual ~Mailer() = default;
    virtual bool send(const std::string& to, const std::string& text) = 0;
};

class MockMailer : public Mailer {
public:
    MOCK_METHOD(bool, send, (const std::string& to, const std::string& text), (override));
};

// notify sends one mail to each customer and gives the number of mails sent.
int notify(Mailer& mailer, const std::vector<std::string>& customers) {
    int sent = 0;
    for (const auto& c : customers) {
        if (mailer.send(c, "Your order has shipped")) {
            ++sent;
        }
    }
    return sent;
}

using testing::HasSubstr;
using testing::Return;

TEST(NotifyTest, SendsOneMailToEachCustomer) {
    MockMailer mailer;
    EXPECT_CALL(mailer, send("ana", HasSubstr("shipped")))
        .WillOnce(Return(true));
    EXPECT_CALL(mailer, send("bo", HasSubstr("shipped")))
        .WillOnce(Return(false));

    EXPECT_EQ(notify(mailer, {"ana", "bo"}), 1);
}

TEST(NotifyTest, MatchersCheckContainers) {
    std::vector<int> totals{900, 500, 1000};
    EXPECT_THAT(totals, testing::ElementsAre(900, 500, 1000));
    EXPECT_THAT(totals, testing::Contains(500));
}
```

```text
Running main() from gmock_main.cc
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from NotifyTest
[ RUN      ] NotifyTest.SendsOneMailToEachCustomer
[       OK ] NotifyTest.SendsOneMailToEachCustomer ($TIME)
[ RUN      ] NotifyTest.MatchersCheckContainers
[       OK ] NotifyTest.MatchersCheckContainers ($TIME)
[----------] 2 tests from NotifyTest ($TIME total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. ($TIME total)
[  PASSED  ] 2 tests.
```

When a call does not match an expectation, or when an expected call does not come, the test fails, and gMock prints the expected and the actual calls.[^9]

### The flags of a test program

A GoogleTest program takes flags on the command line, and each flag also has an environment variable, such as `GTEST_FILTER` for `--gtest_filter`.[^7]

| Flag | Effect |
|---|---|
| `--gtest_filter=StockTest.Take*` | runs only the tests whose full names match the pattern; a part after `-` excludes tests[^7] |
| `--gtest_list_tests` | lists the tests and runs none[^7] |
| `--gtest_repeat=N` | runs the tests N times; -1 repeats them without end[^7] |
| `--gtest_shuffle` | runs the tests in a random order, and prints the seed; `--gtest_random_seed=SEED` takes a seed from 0 to 99999[^7] |
| `--gtest_fail_fast` | stops at the first failure[^7] |
| `--gtest_brief=1` | prints only the failures and the summary[^7] |
| `--gtest_output=xml:report.xml` | writes a report in XML; `json:report.json` writes JSON[^7] |

The filter below runs one test of the fixture example, with brief output:

```text
$ ./main_test.exe --gtest_filter='StockTest.Take*' --gtest_brief=1
Running main() from gtest_main.cc
[==========] 1 test from 1 test suite ran. (0 ms total)
[  PASSED  ] 1 test.
```

The XML report "is based on the `junitreport` Ant task", so continuous integration servers that read JUnit reports can read it.[^7] For sharding, a runner sets `GTEST_TOTAL_SHARDS` and `GTEST_SHARD_INDEX` on each machine, and each machine runs a different part of the tests.[^7]

## Common tasks

```text
cmake -S . -B build && cmake --build build      build the tests with CMake
ctest --test-dir build                          run the tests that gtest_discover_tests found
./main_test.exe                                 run all the tests of one program
./main_test.exe --gtest_filter=TotalTest.*      run the tests of one suite
./main_test.exe --gtest_filter=-*DeathTest.*    run all tests except the death tests
./main_test.exe --gtest_list_tests              list the tests
./main_test.exe --gtest_repeat=100 --gtest_shuffle   find tests that depend on order
./main_test.exe --gtest_output=xml:report.xml   write a JUnit-style report
```

These are the commands of the quickstart for CMake and the flags of the advanced guide.[^4][^7]

## Configuration

GoogleTest has no configuration file. A program sets its options with the flags above or with the `GTEST_*` environment variables.[^7] A program that needs code to run before the tests writes its own `main`, calls `testing::InitGoogleTest(&argc, argv)` and returns `RUN_ALL_TESTS()`; it links to `gtest` and not to `gtest_main`.[^2]

## Alternatives

- [[catch2-and-doctest-cpp]]: Catch2 and doctest are C++ test frameworks with free-form test names, sections and a single header for doctest.
- [[c-test-frameworks]]: Unity, CMocka, Check and Criterion test code in C.
- [[junit]] and [[xunit]]: the family of frameworks that GoogleTest follows.

## History and versions

| Version | Change |
|---|---|
| 1.8.1, 31 August 2018 | the last release for compilers before C++11[^11] |
| 1.10.0, 3 October 2019 | the `*_TEST_SUITE` names replace the `*_TEST_CASE` names, and the new macro `MOCK_METHOD` replaces `MOCK_METHOD1` and the others[^11] |
| 1.11.0, 12 June 2021 | the documentation moves to GitHub Pages[^11] |
| 1.12.1, 30 June 2022 | the last release for C++11[^11] |
| 1.13.0, 17 January 2023 | C++14 is required[^11] |
| 1.16.0, 7 February 2025 | the last branch for C++14[^11] |
| 1.17.0, 30 April 2025 | C++17 is required; the flag `--gtest_fail_if_no_test_linked` and the matcher `DistanceFrom`[^11] |
| 1.18.0, 10 August 2026 | `GTEST_INTERNAL_HAS_STRING_VIEW` is always 1, and many bug fixes[^11] |

The latest version on 12 November 2026 was 1.18.0, from 10 August 2026.[^11] The release notes ask users to build from the latest commit, because the release branches take no new features.[^11]

## References
