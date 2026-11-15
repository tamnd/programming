---
tag: NKMTDM
kind: tool
slug: pester
name: Pester
aliases: [Pester framework, Invoke-Pester, Pester 6]
area: testing/test-frameworks
level: core
summary: >-
  Pester is the test and mock framework of PowerShell. Tests are It blocks inside Describe and Context blocks, Should checks the values, and Mock replaces a command.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/pester/Pester
homepage: https://pester.dev
see_also: [test-framework, unit-testing, test-double, xunit, nunit-and-mstest, junit-5, bats]
sources:
  - url: https://github.com/pester/Pester
    title: Pester, README
    locator: the introduction
    accessed: 2026-11-15
  - url: https://pester.dev/docs/quick-start
    title: Pester, Quick Start
    locator: What is Pester?; Installing Pester; Creating a Pester Test; Other keywords
    accessed: 2026-11-15
  - url: https://github.com/pester/Pester/blob/main/LICENSE
    title: Pester, LICENSE
    locator: the whole file
    accessed: 2026-11-15
  - url: https://pester.dev/docs/introduction/installation
    title: Pester, Installation and Update
    locator: Installing from PSGallery (Windows 10 or later)
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/discovery-and-run
    title: Pester, Discovery and Run
    locator: the introduction; Discovery
    accessed: 2026-11-15
  - url: https://github.com/pester/Pester/releases
    title: Pester releases on GitHub
    locator: 6.2.0; 6.0.0; 5.0.0; 3.4.0
    accessed: 2026-11-15
  - url: https://pester.dev/docs/assertions/
    title: Pester, Assertions
    locator: the introduction; Should Operators
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/mocking
    title: Pester, Mocking with Pester
    locator: Mocking Functions; Mocks are scoped based on their placement; Counting mocks depends on placement
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/modules
    title: Pester, Unit Testing within Modules
    locator: the examples with Mock -ModuleName
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/data-driven-tests
    title: Pester, Data driven tests
    locator: Using -ForEach and -TestCases
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/tags
    title: Pester, Tags
    locator: the introduction
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/configuration
    title: Pester, Configuration
    locator: Run; Output; TestResult
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/test-results
    title: Pester, Test results
    locator: the introduction
    accessed: 2026-11-15
  - url: https://pester.dev/docs/usage/code-coverage
    title: Pester, Code Coverage
    locator: the introduction; Coverage format
    accessed: 2026-11-15
---

## What it is

Pester is "the ubiquitous test and mock framework for PowerShell".[^1] It is used mostly for unit tests and integration tests, and also as a base for tools that check whole environments, such as deployments and database configurations.[^2] Its licence is the Apache License 2.0.[^3]

A test file has a name that ends in `.Tests.ps1`. Pester inspects all files with this name.[^2] The tests use a small set of functions: `Describe`, `Context`, `It`, `Should` and `Mock`.[^2] A `Describe` block is a group of tests, and an `It` block is one test.[^2] The shape is close to [[xunit]]. Pester can also write code coverage files and test result files for a CI pipeline.[^2]

## Install and first use

Windows 10, Windows Server 2016 and later ship with Pester 3.4.0, and this built-in version cannot be updated with `Update-Module`.[^4] The usual installation is from the PowerShell Gallery:[^2][^4]

```text
Install-Module Pester -Force
Import-Module Pester -PassThru
```

The site does not run the PowerShell code on this page, because its build has no PowerShell. The examples and outputs come from the Pester documentation. This is the first test of the Quick Start, in the file `Get-Planet.Tests.ps1`:[^2]

```text
BeforeAll {
    function Get-Planet ([string]$Name = '*') {
        $planets = @(
            @{ Name = 'Mercury' }
            @{ Name = 'Venus' }
            @{ Name = 'Earth' }
            @{ Name = 'Mars' }
            @{ Name = 'Jupiter' }
            @{ Name = 'Saturn' }
            @{ Name = 'Uranus' }
            @{ Name = 'Neptune' }
        ) | ForEach-Object { [PSCustomObject] $_ }
        $planets | Where-Object { $_.Name -like $Name }
    }
}

Describe 'Get-Planet' {
    It 'Given no parameters, it lists all 8 planets' {
        $allPlanets = Get-Planet
        $allPlanets.Count | Should-Be 8
    }
}
```

The command `Invoke-Pester -Output Detailed` with the path of the file runs the test. The documentation shows this output:[^2]

```text
Running tests from 1 files.
Running tests from 'C:\t\Planets\Get-Planet.Tests.ps1'
Describing Get-Planet
 [+] Given no parameters, it lists all 8 planets 20ms
Tests completed in 179ms
Tests Passed: 1, Failed: 0, Skipped: 0, Inconclusive: 0, NotRun: 0
```

When the function returns a ninth planet, the test fails, and the message gives the expected value and the actual value:[^2]

```text
Describing Get-Planet
 [-] Given no parameters, it lists all 8 planets 19ms
 Expected [int] 8, but got [int] 9.
 at $allPlanets.Count | Should-Be 8, C:\t\Planets\Get-Planet.Tests.ps1:22
Tests completed in 183ms
Tests Passed: 0, Failed: 1, Skipped: 0, Inconclusive: 0, NotRun: 0
```

In a real project, the function is in its own file, such as `Get-Planet.ps1`, and the `BeforeAll` block of the test file dot-sources that file.[^2]

## Concepts of the tool

### Blocks and setup

A file can have one or more `Describe` blocks, and they can nest. `Context` can be used in place of `Describe` in almost all cases. Usually the top `Describe` has the name of the function under test, and each `Context` is one aspect of the function.[^2] A `BeforeAll` block runs at the start of the block that contains it, or at the start of the file. `AfterAll` runs at the end of the block, and `BeforeEach` and `AfterEach` run around each test of the block.[^2]

### Discovery and Run

Pester runs test files in two phases, Discovery and Run. During Discovery, it finds all the `Describe`, `Context` and `It` blocks. It saves the body of each test, but it does not run it yet.[^5] So code that is not inside a Pester block runs during Discovery, and its results are not available during Run.[^5] Code that makes tests dynamically goes in a `BeforeDiscovery` block.[^5]

Pester 5 discovered all the files first, then ran all the files. Pester 6 discovers one file and runs it before it goes to the next file. So each test file must import its own modules and do its own setup for Discovery.[^6]

### Assertions

Pester has two syntaxes for assertions. The classic syntax is the command `Should` with an operator, such as `Should -Be` and `Should -BeExactly`. Each operator becomes negative with `Not`, as in `Should -Not -Be`.[^7] Pester 6 added a new family of commands with a dash in the name, such as `Should-Be`, `Should-BeString`, `Should-BeCollection` and `Should-Throw`.[^6] The documentation recommends the new commands for new tests. The classic operators still work in Pester 6.[^7]

```text
1, 2, 3 | Should-BeCollection @(1, 2, 3)
'  hello ' | Should-BeString 'hello' -TrimWhitespace
{ throw 'kaboom' } | Should-Throw -ExceptionMessage 'kaboom'
$actual | Should -Be "actual value"
```

The first three lines use the Pester 6 syntax, from the release notes. The last line uses the classic syntax.[^6][^7] The pipeline unwraps a collection, so a value assertion sees `1` and `@(1)` as the same value. The parameter `-Actual` gives the value without the pipeline, and keeps its type.[^6]

The configuration `Should.ErrorAction = 'Continue'` makes the failed assertions in one test into soft assertions: all the assertions run, and Pester reports every failure at the end of the test.[^6] The configuration `Should.DisableV5 = $true` turns off the classic syntax.[^6]

### Mocks

`Mock` replaces the behavior of any PowerShell command with another implementation. `Should -Invoke` and `Should-Invoke` check how many times a mocked command was called, and with which parameters.[^8] This is a form of [[test-double]].

```text
Describe 'Order processing' {
    It 'emails alice exactly twice' {
        Mock Send-Email { }
        Send-Email -To 'alice' -Subject 'Welcome'
        Send-Email -To 'bob' -Subject 'Receipt'
        Should -Invoke Send-Email -Times 2 -Exactly -ParameterFilter { $To -eq 'alice' }
    }
}
```

This test is based on an example in the Pester 6 release notes. Only one call matches the filter, so the test fails. Pester 6 then prints each recorded call, with `[*]` for a call that matches the filter and `[ ]` for a call that does not match.[^6]

A mock is active only in the block where the test defines it. A mock in an `It` block is for that test only, and a mock in a `BeforeAll` block is for all the tests of the `Describe` block.[^8] To mock a command that a script module calls, the test gives the module with `Mock -ModuleName`.[^9]

### Data-driven tests

The parameter `-ForEach` of `It` makes one test for each item of an array. Each item is a hashtable, and each key of the hashtable becomes a variable in the test. A name in angle brackets, such as `<name>`, is a template for the name of the test.[^10] On `It`, the alias of `-ForEach` is `-TestCases`.[^10]

```text
Describe "Get-Emoji" {
    It "Returns <expected> (<name>)" -ForEach @(
        @{ Name = "cactus"; Expected = '🌵' }
        @{ Name = "giraffe"; Expected = '🦒' }
    ) {
        Get-Emoji -Name $name | Should-Be $expected
    }
}
```

This example makes two tests, with the names `Returns 🌵 (cactus)` and `Returns 🦒 (giraffe)`.[^10] `Describe` and `Context` also take `-ForEach`.[^10]

### Tags

The parameter `-Tag` puts tags on a `Describe`, a `Context` or an `It` block. The parameters `-TagFilter` and `-ExcludeTagFilter` of `Invoke-Pester` select the tests to run.[^11] In Pester 6, the filter value `None` selects the tests that have no tags.[^6]

## Common tasks

```text
Invoke-Pester -Output Detailed ./Get-Planet.Tests.ps1   run one file and show each test
Invoke-Pester $path -TagFilter "Acceptance" -ExcludeTagFilter "Flaky", "Slow"   select tests by tag
Get-ShouldOperator                              list the operators of the classic Should
```

The commands come from the Quick Start, the page on tags and the page on assertions.[^2][^11][^7]

## Configuration

`New-PesterConfiguration` makes a configuration object. Its sections include `Run`, `Filter`, `CodeCoverage`, `TestResult`, `Should`, `Debug`, `Output`, `TestDrive` and `TestRegistry`.[^12] `Run.Path` gives the directories to search for tests, or the paths of test files.[^12]

```text
$config = New-PesterConfiguration
$config.Run.Path = "."
$config.Run.Exit = $true
$config.Output.Verbosity = 'Detailed'
$config.TestResult.OutputFormat = "JUnitXml"
$config.CodeCoverage.Enabled = $true
Invoke-Pester -Configuration $config
```

`Run.Exit` makes Pester exit with a non-zero exit code when the run fails, which a CI job needs.[^12] The values of `Output.Verbosity` are `None`, `Normal`, `Detailed` and `Diagnostic`.[^12] Pester writes test results as XML in the NUnit 2.5, NUnit 3 or JUnit 4 schema.[^13] Code coverage files are in the JaCoCo format by default, and Pester 6 can also write the Cobertura format.[^14]

## Alternatives

- [[nunit-and-mstest]] and [[xunit-net]]: the test frameworks of .NET, for code in C#.
- [[bats]]: a test framework of the same kind for Bash scripts.

## History and versions

| Version | Change |
|---|---|
| 3.4.0, 21 March 2016 | the version that Windows 10 and Windows Server 2016 ship with[^6][^4] |
| 5.0.0, 27 May 2020 | the two phases Discovery and Run; the configuration object[^6][^5] |
| 6.0.0, 7 July 2026 | the new `Should-*` assertions; Discovery and Run for each file; an experimental parallel runner; profiler-based code coverage by default; the minimum is Windows PowerShell 5.1 or PowerShell 7.4[^6] |

Pester 6.0.0 also removed `Assert-MockCalled`, the test status `Pending` and the switch `-Focus`.[^6] The latest version on 12 November 2026 was 6.2.0, from 9 September 2026.[^6]

## References
