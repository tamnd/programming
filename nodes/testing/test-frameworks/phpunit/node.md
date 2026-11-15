---
tag: J4WGC3
kind: tool
slug: phpunit
name: PHPUnit
aliases: [phpunit/phpunit]
area: testing/test-frameworks
level: advanced
summary: >-
  PHPUnit is the xUnit test framework of PHP. A test is a public method of a subclass of TestCase, and PHP attributes give the data providers, groups and code coverage targets.
role: [test-framework]
licence: bsd-3-clause
repo: https://github.com/sebastianbergmann/phpunit
homepage: https://phpunit.de
see_also: [test-framework, unit-testing, xunit, junit, test-double, test-driven-development, rspec-and-minitest]
sources:
  - url: https://github.com/sebastianbergmann/phpunit
    title: phpunit, README
    locator: the introduction; Installation
    accessed: 2026-11-15
  - url: https://github.com/sebastianbergmann/phpunit/blob/main/LICENSE
    title: phpunit, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/writing-tests-for-phpunit.html
    title: PHPUnit 13.4 Manual, Writing Tests for PHPUnit
    locator: the introduction; Asserting return values; Data providers; Examples 2.3 and 2.8
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/installation.html
    title: PHPUnit 13.4 Manual, Installing PHPUnit
    locator: Requirements; PHP Archive (PHAR); Composer; Phive
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/assertions.html
    title: PHPUnit 13.4 Manual, Assertions
    locator: the introduction; assertSame(); assertEquals()
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/fixtures.html
    title: PHPUnit 13.4 Manual, Fixtures
    locator: setUp() and tearDown(); Template Methods
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/test-doubles.html
    title: PHPUnit 13.4 Manual, Test Doubles
    locator: the introduction; Test Stubs; Mock Objects
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/attributes.html
    title: PHPUnit 13.4 Manual, Attributes
    locator: the introduction; Test; TestDox
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/code-coverage.html
    title: PHPUnit 13.4 Manual, Code Coverage
    locator: the introduction; Targeting Units of Code; The Code Coverage Report in HTML Format
    accessed: 2026-11-15
  - url: https://phpunit.de/announcements/phpunit-12.html
    title: PHPUnit, Release Announcement for Version 12
    locator: Metadata
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/cli-options.html
    title: PHPUnit 13.4 Manual, CLI options
    locator: --filter; --group; --testsuite; --coverage-html; --generate-configuration
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/textui.html
    title: PHPUnit 13.4 Manual, The Command-Line Test Runner
    locator: TestDox; Logging; Stopping early
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/configuration.html
    title: PHPUnit 13.4 Manual, Configuration
    locator: the introduction
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/xml-configuration-file.html
    title: PHPUnit 13.4 Manual, The XML Configuration File
    locator: The testsuites Element; The failOnRisky Attribute
    accessed: 2026-11-15
  - url: https://docs.phpunit.de/en/13.4/organizing-tests.html
    title: PHPUnit 13.4 Manual, Organizing Tests
    locator: Composing a Test Suite Using XML Configuration
    accessed: 2026-11-15
  - url: https://phpunit.de/supported-versions.html
    title: PHPUnit, Supported Versions of PHPUnit
    locator: the table; the support policy
    accessed: 2026-11-15
  - url: https://repo.packagist.org/p2/phpunit/phpunit.json
    title: Packagist, the metadata of phpunit/phpunit
    locator: 13.4.7; 13.4.6; 13.0.0
    accessed: 2026-11-15
  - url: https://phpunit.de/announcements/phpunit-10.html
    title: PHPUnit, Release Announcement for Version 10
    locator: Event System
    accessed: 2026-11-15
  - url: https://phpunit.de/announcements/phpunit-13.html
    title: PHPUnit, Release Announcement for Version 13
    locator: Major Changes in PHPUnit 13
    accessed: 2026-11-15
---

## What it is

PHPUnit "is a programmer-oriented testing framework for PHP. It is an instance of the xUnit architecture for unit testing frameworks."[^1] Sebastian Bergmann develops it, and its licence is the BSD 3-Clause License.[^1][^2]

A test is a public method of a class that inherits from `PHPUnit\Framework\TestCase`. The name of the method starts with `test`, or the method has the attribute `#[Test]`.[^3] Assertion methods such as `assertSame()` compare an actual value with an expected value. "A test passes when none of its assertions fail, none of its expectations are violated, and no unexpected exception is thrown."[^3]

## Install and first use

PHPUnit 13 requires PHP 8.4.[^4] The project recommends the PHP Archive (PHAR), one file that has all the dependencies of PHPUnit. It can also be installed with Composer, as a development dependency of the project, or with the tool Phive.[^4][^1]

```text
composer require --dev phpunit/phpunit
wget -O phpunit.phar https://phar.phpunit.de/phpunit-13.phar
phive install phpunit
```

`composer require --dev phpunit/phpunit` adds a version constraint with the caret operator, for example `"phpunit/phpunit": "^13.3"`.[^4] The examples in the manual run PHPUnit as `./tools/phpunit`, which is where Phive puts it.[^4][^3]

This test from the manual uses a data provider. The static method `additionProvider()` returns named data sets, and the attribute `#[DataProvider]` runs `testAdd()` one time for each data set. The last data set is wrong on purpose: 1 + 1 is not 3.[^3]

```text
<?php declare(strict_types=1);
use PHPUnit\Framework\Attributes\DataProvider;
use PHPUnit\Framework\TestCase;

final class NamedDataSetsTest extends TestCase
{
    public static function additionProvider(): array
    {
        return [
            'adding zeros'  => [0, 0, 0],
            'zero plus one' => [0, 1, 1],
            'one plus zero' => [1, 0, 1],
            'one plus one'  => [1, 1, 3],
        ];
    }

    #[DataProvider('additionProvider')]
    public function testAdd(int $a, int $b, int $expected): void
    {
        $this->assertSame($expected, $a + $b);
    }
}
```

The manual shows this output from PHPUnit 13.4.0:[^3]

```text
./tools/phpunit tests/NamedDataSetsTest.php
PHPUnit 13.4.0 by Sebastian Bergmann and contributors.

Runtime:       PHP 8.5.11

...F                                                                4 / 4 (100%)

Time: 00:00.001, Memory: 32.33 MB

There was 1 failure:

1) NamedDataSetsTest::testAdd@one plus one with data (1, 1, 3)
Failed asserting that 2 is identical to 3.

/path/to/tests/NamedDataSetsTest.php:20

FAILURES!
Tests: 4, Assertions: 4, Failures: 1.
```

Each data set is a separate test, so the run has 4 tests. The name of the failing test includes the name of the data set, `one plus one`.[^3]

## Concepts of the tool

### Assertions

The assertion methods are static methods of `PHPUnit\Framework\Assert`, which `TestCase` inherits from. A test can call them as `$this->assertTrue()` or `self::assertTrue()`.[^5] `assertSame()` checks identity with the operator `===`, and `assertEquals()` checks equality with the operator `==`, with special comparisons for some types.[^5] So `assertSame(2204, '2204')` fails with the message "Failed asserting that 2204 is identical to '2204'."[^5]

An assertion that fails ends the test method. The assertions after it do not run, so only the first failure is reported:[^3][^5]

```text
<?php declare(strict_types=1);
use PHPUnit\Framework\TestCase;

final class FailedAssertionTest extends TestCase
{
    public function testAssertionsAfterAFailedAssertionAreNotExecuted(): void
    {
        $this->assertSame(1, 1);
        $this->assertSame(2, 3);
        $this->assertSame(4, 5);
    }
}
```

```text
./tools/phpunit tests/FailedAssertionTest.php
PHPUnit 13.4.0 by Sebastian Bergmann and contributors.

Runtime:       PHP 8.5.11

F                                                                   1 / 1 (100%)

Time: 00:00, Memory: 32.33 MB

There was 1 failure:

1) FailedAssertionTest::testAssertionsAfterAFailedAssertionAreNotExecuted
Failed asserting that 3 is identical to 2.

/path/to/tests/FailedAssertionTest.php:9

FAILURES!
Tests: 1, Assertions: 2, Failures: 1.
```

The run counts 2 assertions, because the third one never runs.[^3] To check each value on its own, the manual recommends a data provider, because each data set is reported as a separate test.[^3] An exception is checked with `$this->expectException(InvalidArgumentException::class)` before the code that throws it.[^3]

### Fixtures

The template method `setUp()` runs before each test method, and `tearDown()` runs after it, whether the test passed or failed. They run for each test method, on a fresh instance of the test class.[^6] `setUpBeforeClass()` runs one time before the first test of the class.[^6]

```text
protected function setUp(): void
{
    $this->example = new Example(
        $this->createStub(Collaborator::class)
    );
}

protected function tearDown(): void
{
    $this->example = null;
}
```

### Test stubs and mock objects

PHPUnit makes test doubles from an interface or a class. A test stub replaces a component that the system under test depends on, and it controls the indirect inputs of the system under test.[^7] `createStub()` makes a stub whose methods return default values unless the test configures them.[^7] "A mock object is a test stub that can additionally be configured with expectations about how it should be called."[^7]

These examples from the manual use an interface `Database` with the methods `execute()` and `query()`, and a class `Service` that uses it:[^7]

```text
public function testDoSomethingReturnsTrueWhenQueryReturnsRows(): void
{
    $database = $this->createStub(Database::class);

    $database
        ->method('query')
        ->willReturn([['foo' => 'bar']]);

    $service = new Service($database);

    $this->assertTrue($service->doSomething());
}

public function testDoSomethingElseExecutesInsertQuery(): void
{
    $database = $this->createMock(Database::class);

    $database
        ->expects($this->once())
        ->method('execute')
        ->with(
            'INSERT INTO bar (foo, baz) VALUES (?, ?);',
            'value',
            'another value',
        );

    $service = new Service($database);

    $service->doSomethingElse();
}
```

The second test has no assertion. It fails if `Service` does not call `execute()` exactly one time with these arguments.[^7]

### Attributes

PHPUnit reads metadata from PHP attributes in the namespace `PHPUnit\Framework\Attributes`, for example `#[Test]`, `#[DataProvider]`, `#[Group]`, `#[TestDox]` and `#[CoversClass]`.[^8][^9] PHPUnit 10 added the attributes. Before that, the only way to add metadata was annotations in doc comments. PHPUnit 11 deprecated the annotations, and PHPUnit 12 removed them.[^10]

## Common tasks

```text
./tools/phpunit tests/EmailTest.php          run the tests in one file
./tools/phpunit --filter testAdd             run the tests whose names match a pattern
./tools/phpunit --group slow                 run the tests in a group
./tools/phpunit --testsuite unit             run one test suite from phpunit.xml
./tools/phpunit --testdox                    print the tests as sentences
./tools/phpunit --stop-on-failure            stop after the first failure
./tools/phpunit --log-junit junit.xml        write the results as JUnit XML
./tools/phpunit --coverage-html coverage     write an HTML code coverage report
./tools/phpunit --generate-configuration     make phpunit.xml with a wizard
```

The options come from the manual pages on CLI options and on the test runner.[^11][^12] For the results, the manual recommends `--log-otr`, the Open Test Reporting format, and it calls `--log-junit` legacy.[^12]

## Configuration

PHPUnit reads the XML file `phpunit.xml`, or the file given with `--configuration`.[^13] The command line options override the XML file, and the XML file overrides the built-in defaults.[^13] The element `testsuites` groups the test directories and files into named suites:[^14]

```text
<phpunit bootstrap="vendor/autoload.php">
  <testsuites>
    <testsuite name="unit">
      <directory>tests/unit</directory>
    </testsuite>

    <testsuite name="integration" bootstrap="tests/integration/bootstrap.php">
      <directory>tests/integration</directory>
    </testsuite>
  </testsuites>
</phpunit>
```

With this file, `./tools/phpunit` with no arguments runs all the suites, and `--testsuite unit` runs one suite. The output then has a line `Configuration:` with the path of the file.[^15] With the attribute `failOnRisky="true"`, the exit code shows failure when all tests pass but some tests are marked as risky.[^14]

Code coverage needs the PHP extension PCOV or Xdebug. Without one of them, PHPUnit warns that no code coverage driver is available.[^9] The attribute `#[CoversClass]` on a test class says which class the test is for.[^9]

## Alternatives

- [[junit]] and [[xunit]]: the xUnit family that PHPUnit follows.
- [[rspec-and-minitest]]: the main test frameworks of Ruby.

## History and versions

Since PHPUnit 6 in 2017, each major version of PHPUnit has come out in February. Bugfix support for a major version ends when the major version two steps later comes out, two years after the first release.[^16][^17]

| Version | First release | Change |
|---|---|---|
| PHPUnit 1 | 15 March 2004 | for PHP 4[^16] |
| PHPUnit 3 | 19 November 2006 | for PHP 5.1 to PHP 5.6[^16] |
| PHPUnit 10 | 3 February 2023 | PHP 8.1 or later; an event system for all output; attributes for metadata[^16][^18][^10] |
| PHPUnit 11 | 2 February 2024 | PHP 8.2 or later; annotations deprecated[^16][^10] |
| PHPUnit 12 | 7 February 2025 | PHP 8.3 or later; annotations removed[^16][^10] |
| PHPUnit 13 | 6 February 2026 | PHP 8.4 or later; eight new array assertions, for example `assertArraysAreIdenticalIgnoringOrder()`; sealed test doubles with `seal()`; `withParameterSetsInOrder()`[^16][^19] |

PHPUnit 1, for PHP 4, and PHPUnit 2, for PHP 5, were developed in parallel.[^16] On 12 November 2026, the latest release was 13.4.6, from 2 November 2026. Version 13.4.7 followed on 13 November 2026.[^17]

## References
