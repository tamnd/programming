---
tag: NZ7BEA
kind: tool
slug: xunit
name: xUnit.net
aliases: [xUnit]
area: testing/test-frameworks
level: core
summary: >-
  xUnit.net is a test framework for C#, F# and Visual Basic on .NET. This page shows facts and theories, one instance of the test class per test, the fixtures and parallel test collections.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/xunit/xunit
homepage: https://xunit.net
see_also: [xunit-net, nunit-and-mstest, test-framework, unit-testing, csharp, junit-5]
same_as:
  wikidata: Q17048771
  wikipedia: XUnit.net
sources:
  - url: https://xunit.net/
    title: xUnit.net, About xUnit.net
    locator: the home page
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/getting-started
    title: xUnit.net, Getting Started with xUnit.net v3
    locator: Create the unit test project; Write your first tests; Write your first theory; Using dotnet test
    accessed: 2026-11-15
  - url: https://xunit.net/docs/shared-context
    title: xUnit.net, Sharing Context between Tests
    locator: Constructor and Dispose; Class Fixtures; Collection Fixtures; Assembly Fixtures
    accessed: 2026-11-15
  - url: https://xunit.net/docs/running-tests-in-parallel
    title: xUnit.net, Running Tests in Parallel
    locator: Background; Parallel modes
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/microsoft-testing-platform
    title: xUnit.net, Microsoft Testing Platform (xUnit.net v3)
    locator: Microsoft Testing Platform support disabled; Enabling the dotnet test experience
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/theory-data-attributes
    title: xUnit.net, Theory Data Attributes in xUnit.net v3
    locator: InlineData; MemberData; ClassData; Alternative return types
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/whats-new
    title: xUnit.net, What's New in v3?
    locator: Dynamically skippable tests; Explicit tests; Test context; Matrix theory data; Stable randomization
    accessed: 2026-11-15
  - url: https://xunit.net/docs/config-xunit-runner-json
    title: xUnit.net, Config with xunit.runner.json
    locator: Adding the configuration file; parallelizeTestCollections; failSkips
    accessed: 2026-11-15
  - url: https://xunit.net/docs/capturing-output
    title: xUnit.net, Capturing Output
    locator: the introduction
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/migration
    title: xUnit.net, Migrating Unit Tests from v2 to v3
    locator: Stand-alone executables
    accessed: 2026-11-15
  - url: https://xunit.net/governance
    title: xUnit.net, Governance
    locator: Project Lead
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v2/2.0.0
    title: xUnit.net, Core Framework v2 2.0.0
    locator: the timeline; Parallelism
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v3/1.0.0
    title: xUnit.net, Core Framework v3 1.0.0
    locator: Hello, v3!
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v3/4.0.3
    title: xUnit.net, Core Framework v3 4.0.3
    locator: the introduction
    accessed: 2026-11-15
  - url: https://xunit.net/releases/
    title: xUnit.net, Release Notes
    locator: Core Framework v3; Core Framework v2
    accessed: 2026-11-15
---

## What it is

xUnit.net is a [[test-framework]] for .NET. Its home page says: "xUnit.net is a free, open source, community-focused unit testing tool for C#, F#, and Visual Basic. xUnit.net v3 supports .NET 8.0 or later, and .NET Framework 4.7.2 or later."[^1] The project "is part of the .NET Foundation" and "is licensed under Apache 2".[^1] It is a member of the xUnit family of frameworks, but it does not copy the names of [[junit]]. A test is a `[Fact]` or a `[Theory]`, the setup is the constructor of the test class, and the teardown is `Dispose`.[^2][^3]

Two choices of the design make xUnit.net different from the older frameworks. First, xUnit.net "creates a new instance of the test class for every test that is run".[^3] Second, it runs the tests of different test classes in parallel by default.[^4] The other main .NET frameworks are NUnit and MSTest; the page [[nunit-and-mstest]] compares them. The versions v1, v2 and v3 of xUnit.net, and the move from v2 to v3, are on the page [[xunit-net]].

## Install and first use

The project gives templates for the .NET SDK. These commands install the templates and make a test project:[^2]

```text
dotnet new install xunit.v3.templates
mkdir MyFirstUnitTests
cd MyFirstUnitTests
dotnet new xunit3
```

The template makes a project file with one package reference, `xunit.v3.mtp-v2`, and the property `<OutputType>Exe</OutputType>`. The getting started guide explains: "OutputType is set to Exe , because unit test projects in xUnit.net v3 are stand-alone executables that can be directly run."[^2] So `dotnet run` runs the tests of a v3 project, and `dotnet test` also runs them.[^2]

The examples on this page use the VSTest runner of `dotnet test`. For VSTest, a project needs two more packages: "If you choose VSTest as your dotnet test test runner, we will also include references to two additional packages ( xunit.runner.visualstudio and Microsoft.NET.Test.Sdk ) required to enable VSTest support."[^2] The examples use the package `xunit.v3.mtp-off`, because it "explicitly disables MTP support".[^5] This is the project file of the examples:

```text
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <TargetFramework>net10.0</TargetFramework>
    <OutputType>Exe</OutputType>
    <Nullable>enable</Nullable>
    <ImplicitUsings>enable</ImplicitUsings>
    <IsPackable>false</IsPackable>
    <VSTestLogger>console%3Bverbosity=minimal</VSTestLogger>
  </PropertyGroup>
  <ItemGroup>
    <PackageReference Include="Microsoft.NET.Test.Sdk" Version="17.14.1" />
    <PackageReference Include="xunit.v3.mtp-off" Version="4.0.3" />
    <PackageReference Include="xunit.runner.visualstudio" Version="4.0.1" />
  </ItemGroup>
</Project>
```

The property `VSTestLogger` makes the console logger of VSTest print the details of each failed test. The template of xUnit.net adds `<Using Include="Xunit" />` to the project file, so its test files need no `using Xunit;`.[^2] The project file above does not have it, so the examples have the `using` line.

A first test file has a class under test and a test class. A `[Fact]` is a test with no parameters. A `[Theory]` is a test with parameters, and each `[InlineData]` attribute gives one row of arguments:[^2][^6]

```csharp test id=first-use deps=deps/csharp/Shop.Tests.csproj file=BasketTests.cs timeout=30
using Xunit;

public class Basket
{
    private readonly List<(string Name, int Price, int Quantity)> lines = [];

    public void Add(string name, int price, int quantity)
    {
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(quantity);
        lines.Add((name, price, quantity));
    }

    public int Total => lines.Sum(line => line.Price * line.Quantity);
}

public class BasketTests
{
    [Fact]
    public void NewBasketCostsNothing()
    {
        Assert.Equal(0, new Basket().Total);
    }

    [Theory]
    [InlineData(250, 1, 250)]
    [InlineData(250, 3, 750)]
    [InlineData(99, 10, 990)]
    public void TotalIsPriceTimesQuantity(int price, int quantity, int expected)
    {
        var basket = new Basket();
        basket.Add("tea", price, quantity);
        Assert.Equal(expected, basket.Total);
    }

    [Fact]
    public void ZeroQuantityIsRejected()
    {
        var basket = new Basket();
        Assert.Throws<ArgumentOutOfRangeException>(() => basket.Add("tea", 250, 0));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/Shop.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     5, Skipped:     0, Total:     5, Duration: $TIME - Shop.Tests.dll (net10.0)
```

The report counts 5 tests for the 3 methods, because the theory runs once for each of its 3 rows.

## Concepts of the tool

### Facts and theories

The guide gives the difference between the two kinds of test: "Facts are tests which are always true. They test invariant conditions." and "Theories are tests which are only true for a particular set of data."[^2] A theory gets its data from a data source attribute. The documentation names "three built-in data source attributes": `[InlineData]`, `[MemberData]` and `[ClassData]`.[^6]

- `[InlineData]` puts the values in the attribute. Attributes "can accept a very limited number of data types as arguments", so values that are not constants cannot go in it.[^6]
- `[MemberData]` names a public static property, method or field that gives the rows. The documentation calls `IEnumerable<ITheoryDataRow>` "the preferred signature", and recommends `TheoryDataRow<...>` for type safety.[^6]
- `[ClassData]` names a class that gives the rows.[^6]

### One instance of the test class for each test

The documentation says: "xUnit.net creates a new instance of the test class for every test that is run, so any code which is placed into the constructor of the test class will be run for every single test."[^3] For the cleanup, the test class implements `IDisposable` and puts the code in `Dispose()`.[^3] A constructor cannot call asynchronous methods, so a class that needs async setup implements `IAsyncLifetime`, with `InitializeAsync` and `DisposeAsync`.[^3]

So a field of the test class is new in each test. One test cannot change the data of the next test through a field. The documentation calls this structure the "test class as context" pattern.[^3]

### Fixtures

Some setup is too slow to do for each test, for example the start of a database. xUnit.net has three kinds of shared test fixtures:[^3]

| Kind | How a test class gets it | Lifetime |
|---|---|---|
| Class fixture | the class implements `IClassFixture<T>`, and the constructor takes a `T` | created before the first test of the class, cleaned up after its last test |
| Collection fixture | a class with `[CollectionDefinition]` implements `ICollectionFixture<T>`, and each test class has `[Collection]` with the same name | created before the first test of the collection, cleaned up after all its test classes |
| Assembly fixture | `[assembly: AssemblyFixture(typeof(T))]`, and the constructor takes a `T` | created before the first test of the assembly, cleaned up after all tests |

For a class fixture, the documentation says: "Just before the first test in MyDatabaseTests is run, xUnit.net will create an instance of DatabaseFixture . For each test, it will create a new instance of MyDatabaseTests , and pass the shared instance of DatabaseFixture to the constructor."[^3] The assembly fixture is "Newly introduced in Core Framework v3".[^3]

In the next example, the fixture counts how many times it is created. Each test adds an entry to a field of the test class, and then checks that the field has only its own entry:

```csharp test id=fixture deps=deps/csharp/Shop.Tests.csproj file=PriceTests.cs timeout=30
using Xunit;

public class PriceList
{
    public static int Created;

    public PriceList() => Created++;

    public Dictionary<string, int> Prices { get; } = new() { ["tea"] = 250, ["cake"] = 400 };
}

public class PriceTests : IClassFixture<PriceList>
{
    private readonly PriceList priceList;
    private readonly List<string> asked = [];

    public PriceTests(PriceList priceList) => this.priceList = priceList;

    [Fact]
    public void TeaHasAPrice()
    {
        asked.Add("tea");
        Assert.Equal(250, priceList.Prices["tea"]);
        Assert.Single(asked);
        Assert.Equal(1, PriceList.Created);
    }

    [Fact]
    public void CakeHasAPrice()
    {
        asked.Add("cake");
        Assert.Equal(400, priceList.Prices["cake"]);
        Assert.Single(asked);
        Assert.Equal(1, PriceList.Created);
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/Shop.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     2, Skipped:     0, Total:     2, Duration: $TIME - Shop.Tests.dll (net10.0)
```

Both tests pass in either order. The list `asked` has one entry in each test, so each test has its own instance of `PriceTests`. The counter is 1 in both tests, so the two tests share one `PriceList`.

### Test collections and parallel runs

A test collection is a group of tests that do not run in parallel with each other. The documentation says: "By default, there is a test collection per test class . This means that tests in a test class will not be run in parallel against one another, but tests in different test classes will be."[^4] The default parallel mode is `collections`.[^4] To keep the tests of two classes from running at the same time, put both classes in the same collection with `[Collection]`.[^3][^4]

Running tests in parallel was "a new feature in xUnit.net version 2", and it is "enabled by default".[^4] Tests that share a static field, a file or a database must be in one collection, or the parallel run must be turned off. A collection fixture also puts its test classes in one collection, so they do not run in parallel. An assembly fixture does not change the parallel runs: the documentation warns that "fixtures used as assembly fixtures may be used from multiple tests simultaneously".[^3]

### Assertions

The class `Assert` has the assertions. The expected value comes first. In the guide, the call `Assert.Equal(5, Add(2, 2))` gives the message `Expected: 5` and `Actual: 4`.[^2]

`Assert.Throws<T>` passes only when the code throws exactly the type `T`. `Assert.ThrowsAny<T>` also accepts a subclass of `T`. In this example, `ArgumentOutOfRangeException` is a subclass of `ArgumentException`, so the first test fails and the second test passes:

```csharp test fails expect=test-failure id=exact-type deps=deps/csharp/Shop.Tests.csproj file=AccountTests.cs timeout=30
using Xunit;

public class Account
{
    public int Balance { get; private set; } = 100;

    public void Withdraw(int amount)
    {
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(amount);
        Balance -= amount;
    }
}

public class AccountTests
{
    [Fact]
    public void ZeroIsRejected()
    {
        var account = new Account();
        Assert.Throws<ArgumentException>(() => account.Withdraw(0));
    }

    [Fact]
    public void ZeroIsRejectedAsSomeArgumentException()
    {
        var account = new Account();
        Assert.ThrowsAny<ArgumentException>(() => account.Withdraw(0));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/Shop.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.
  Failed AccountTests.ZeroIsRejected [$TIME]
  Error Message:
   Assert.Throws() Failure: Exception type was not an exact match
Expected: typeof(System.ArgumentException)
Actual:   typeof(System.ArgumentOutOfRangeException)
---- System.ArgumentOutOfRangeException : amount ('0') must be a non-negative and non-zero value. (Parameter 'amount')
Actual value was 0.
  Stack Trace:
     at AccountTests.ZeroIsRejected() in $WORK/AccountTests.cs:line 20
----- Inner Stack Trace -----
   at Account.Withdraw(Int32 amount) in $WORK/AccountTests.cs:line 9
   at AccountTests.<>c__DisplayClass0_0.<ZeroIsRejected>b__0() in $WORK/AccountTests.cs:line 20

Failed!  - Failed:     1, Passed:     1, Skipped:     0, Total:     2, Duration: $TIME - Shop.Tests.dll (net10.0)
```

xUnit.net has no `[ExpectedException]` attribute. The check of an exception is a statement in the test, so the test shows which call must throw.

### Theory data rows and skipped tests

A `[MemberData]` source can give `TheoryDataRow<...>` values. A row can have metadata, such as a reason to skip it: `new(6, false) { Skip = "This is a flaky data row, figure out what's wrong later" }`.[^6] In v3, a test can also skip itself at run time with `Assert.Skip`, `Assert.SkipUnless` and `Assert.SkipWhen`, and `[Fact]` and `[Theory]` have the properties `SkipUnless` and `SkipWhen`.[^7]

```csharp test id=theory-data deps=deps/csharp/Shop.Tests.csproj file=ShippingTests.cs timeout=30
using Xunit;

public static class Shipping
{
    public static int Cost(int grams) => grams switch
    {
        <= 0 => throw new ArgumentOutOfRangeException(nameof(grams)),
        <= 500 => 400,
        <= 2000 => 750,
        _ => 950,
    };
}

public class ShippingTests
{
    public static IEnumerable<TheoryDataRow<int, int>> Edges =>
    [
        new(1, 400),
        new(500, 400),
        new(501, 750),
        new(2000, 750),
        new(2001, 950),
        new(30000, 950) { Skip = "the price for freight is not decided" },
    ];

    [Theory]
    [MemberData(nameof(Edges))]
    public void CostAtTheEdges(int grams, int cost)
    {
        Assert.Equal(cost, Shipping.Cost(grams));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/Shop.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.
[xUnit.net $TIME]     ShippingTests.CostAtTheEdges(grams: 30000, cost: 950) [SKIP]
  Skipped ShippingTests.CostAtTheEdges(grams: 30000, cost: 950) [$TIME]

Passed!  - Failed:     0, Passed:     5, Skipped:     1, Total:     6, Duration: $TIME - Shop.Tests.dll (net10.0)
```

The theory has 6 rows. 5 rows run, and the runner reports the last row as skipped. A test that calls `Assert.Skip` with a reason is reported in the same way.[^7] With the setting `failSkips`, a run treats skipped tests as errors.[^8]

The order of the tests is random, but stable for one location of the test assembly. The guide says that "running a test assembly from a different location will generated a different unique ID, and thus run the tests in a different (random) order."[^7] So a test must not depend on the order of the other tests.

### Output from a test

A test can write diagnostic output that the runner shows with the result. The documentation gives the history: "xUnit.net v1 always captured output from Console , Debug , and Trace ." Then "xUnit.net v2 removed this ability and replaced it with an injectable ITestOutputHelper that could be used to capture output from a test." Last, "xUnit.net v3 reintroduced the ability to capture output, which is disabled by default for backward compatibility with v2."[^9] To use `ITestOutputHelper`, a test class takes it as a parameter of the constructor.[^9]

### The test context

xUnit.net v3 adds the class `TestContext`. It gives information about the current state of the test pipeline, and "a cancellation token that can be passed to downstream methods in your test".[^7] A test gets it from anywhere with `TestContext.Current`.[^7]

## Common tasks

Run the tests of a project:

```text
dotnet test
```

Run a v3 test project as a program, and pass options to the runner after `--`:[^10]

```text
dotnet run
dotnet run -- -xml results.xml
```

Run one test class, with the filter of `dotnet test` in VSTest mode:

```text
dotnet test --filter "FullyQualifiedName~BasketTests"
```

Give a test a skip reason that is fixed when the code compiles:

```text
[Fact(Skip = "the tax rules change in March")]
public void TaxForMarch() { }
```

Mark a test as explicit, so that it runs only when a run asks for explicit tests. The documentation says that such tests are "normally not run unless it is requested that all explicit tests run".[^7]

```text
[Fact(Explicit = true)]
public void FullImportOfTheProductFile() { }
```

## Configuration

A test project can have a file `xunit.runner.json`. The build copies it to the output folder with this item:[^8]

```text
<ItemGroup>
  <Content Include="xunit.runner.json" CopyToOutputDirectory="PreserveNewest" />
</ItemGroup>
```

The file starts with a reference to the schema, so that an editor can complete the keys. This file turns off the parallel runs and makes skipped tests fail the run:[^8]

```text
{
  "$schema": "https://xunit.net/schema/current/xunit.runner.schema.json",
  "parallelizeTestCollections": false,
  "failSkips": true
}
```

For `parallelizeTestCollections`, the documentation says: "Set this to false to disable all parallelization within this test assembly."[^8] Its default value is `true`.[^8] In v3 4.0 and later, it maps to the newer setting `parallelMode`, and the mode `all` is new in 4.0.[^8][^4]

The test runner of `dotnet test` can be VSTest or the Microsoft Testing Platform (MTP). The package `xunit.v3` gives the default MTP version, `xunit.v3.mtp-v2` chooses MTP v2, and `xunit.v3.mtp-off` turns MTP off.[^5]

## Alternatives

- NUnit and MSTest: the other two main test frameworks of .NET. NUnit uses one instance of the fixture class for all its tests by default, and it has constraint assertions such as `Assert.That(x, Is.EqualTo(4))`. MSTest is the framework of Microsoft. See [[nunit-and-mstest]].
- [[junit-5]]: the test framework of Java. JUnit Jupiter also makes a new instance of the test class for each test by default, and its parameterized tests are like theories.

## History and versions

The parallel tests page says that work on xUnit.net started in 2006.[^4] The governance page says: "Brad Wilson is the currently self-appointed project lead for xUnit.net."[^11]

| Version | Date | Note |
|---|---|---|
| v1 1.9 | January 2012 | the timeline in the notes of v2 2.0.0[^12] |
| v2 2.0.0 | 2015 March 16 | parallel runs by default; the theories move into the core[^12] |
| v3 1.0.0 | 2024 December 16 | "the first release version of xUnit.net v3"[^13] |
| v3 4.0.3 | 2026 November 8 | the version of the examples on this page[^14] |

Core Framework v2 "is in maintenance mode. Critical bug fixes will be issued, but no new feature work is being done."[^15] The changes from v2 to v3 are on the page [[xunit-net]].

## References
