---
tag: 39Y3B4
kind: tool
slug: nunit-and-mstest
name: NUnit and MSTest
aliases: [NUnit, MSTest, MSTest.TestFramework]
area: testing/test-frameworks
level: advanced
summary: >-
  NUnit and MSTest are the two other main test frameworks of .NET. This page shows their attributes, their constraint and classic assertions, and how they make test class instances.
role: [test-framework]
see_also: [xunit, xunit-net, test-framework, junit-5, csharp]
sources:
  - url: https://docs.nunit.org/articles/nunit/intro.html
    title: NUnit Docs, NUnit Documentation
    locator: the introduction
    accessed: 2026-11-15
  - url: https://www.wikidata.org/wiki/Q1747342
    title: Wikidata, NUnit (Q1747342)
    locator: copyright license; source code repository
    accessed: 2026-11-15
  - url: https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-mstest-intro
    title: Microsoft Learn, MSTest overview
    locator: the introduction
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/writing-tests/attributes/testcase.html
    title: NUnit Docs, TestCase
    locator: ExpectedResult
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/writing-tests/assertions/assertion-models/constraint.html
    title: NUnit Docs, Constraint Model (Assert.That)
    locator: the introduction
    accessed: 2026-11-15
  - url: https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-mstest-writing-tests
    title: Microsoft Learn, Write tests with MSTest
    locator: TestMethod requirements; Attributes
    accessed: 2026-11-15
  - url: https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-mstest-writing-tests-data-driven
    title: Microsoft Learn, Data-driven testing in MSTest
    locator: DataRowAttribute; DynamicData
    accessed: 2026-11-15
  - url: https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-mstest-writing-tests-assertions
    title: Microsoft Learn, MSTest assertions
    locator: Overview; The Assert class; Soft assertions with Assert.Scope()
    accessed: 2026-11-15
  - url: https://learn.microsoft.com/en-us/dotnet/core/testing/unit-testing-mstest-writing-tests-lifecycle
    title: Microsoft Learn, MSTest lifecycle
    locator: ClassInitialize and ClassCleanup; Complete test-level order
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/writing-tests/attributes.html
    title: NUnit Docs, Attributes
    locator: the list of attributes
    accessed: 2026-11-15
  - url: https://xunit.net/docs/shared-context
    title: xUnit.net, Sharing Context between Tests
    locator: Constructor and Dispose
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/writing-tests/attributes/fixturelifecycle.html
    title: NUnit Docs, FixtureLifeCycle
    locator: LifeCycle enumeration; Notes
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/writing-tests/assertions/assertion-models/classic.html
    title: NUnit Docs, Classic Model
    locator: the introduction
    accessed: 2026-11-15
  - url: https://docs.nunit.org/articles/nunit/release-notes/framework.html
    title: NUnit Docs, Framework Release Notes
    locator: NUnit 5.0.0; NUnit 4.0.0; NUnit 3.13
    accessed: 2026-11-15
---

## What it is

.NET has three main test frameworks: [[xunit]], NUnit and MSTest. This page covers NUnit and MSTest, and compares the three.

NUnit describes itself in one sentence: "NUnit is the open-source unit-testing framework for all .NET languages."[^1] Its documentation covers "NUnit 3 up to the current version, NUnit 5".[^1] NUnit uses the MIT licence, and its code is at github.com/nunit/nunit.[^2]

MSTest is the test framework of Microsoft. The documentation of Microsoft says: "MSTest, Microsoft Testing Framework, is a fully supported, open-source, and cross-platform test framework for .NET applications."[^3]

All three frameworks are in the xUnit family, and all three mark tests with attributes. They differ in the names of the attributes, in the style of the assertions, and in one choice of the design: how many instances of the test class a run makes.

## Install and first use

The examples on this page use these packages, with the VSTest runner of `dotnet test`:

```text
<!-- NUnit -->
<PackageReference Include="Microsoft.NET.Test.Sdk" Version="17.14.1" />
<PackageReference Include="NUnit" Version="5.0.0" />
<PackageReference Include="NUnit3TestAdapter" Version="6.3.1" />

<!-- MSTest -->
<PackageReference Include="Microsoft.NET.Test.Sdk" Version="17.14.1" />
<PackageReference Include="MSTest.TestFramework" Version="4.5.3" />
<PackageReference Include="MSTest.TestAdapter" Version="4.5.3" />
```

For new MSTest projects, Microsoft says: "For new projects, we recommend using MTP with MSTest.Sdk ."[^3] MTP is the Microsoft Testing Platform. The examples here use VSTest, so that all the .NET pages use the same runner.

### A first NUnit test

An NUnit test is a method with `[Test]`. A parameterized test has one `[TestCase]` attribute for each case. When the method returns a value, "ExpectedResult lets NUnit compare the return value instead of asserting inside the body".[^4] The checks use the constraint model, with `Assert.That` and an object such as `Is.EqualTo(4)`:[^5]

```csharp test id=nunit-first deps=deps/csharp/NUnit.Tests.csproj file=LoanTests.cs timeout=30
using NUnit.Framework;

public static class Loan
{
    // The monthly payment, rounded up so that the payments cover the amount.
    public static int MonthlyPayment(int amount, int months)
    {
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(months);
        return (amount + months - 1) / months;
    }
}

[TestFixture]
public class LoanTests
{
    [TestCase(1200, 12, ExpectedResult = 100)]
    [TestCase(1000, 3, ExpectedResult = 334)]
    [TestCase(0, 6, ExpectedResult = 0)]
    public int PaymentRoundsUp(int amount, int months) => Loan.MonthlyPayment(amount, months);

    [Test]
    public void ZeroMonthsIsRejected()
    {
        Assert.That(() => Loan.MonthlyPayment(1000, 0), Throws.TypeOf<ArgumentOutOfRangeException>());
    }

    [Test]
    public void PaymentsCoverTheAmount()
    {
        int total = Loan.MonthlyPayment(1000, 3) * 3;
        Assert.That(total, Is.GreaterThanOrEqualTo(1000).And.LessThan(1003));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/NUnit.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     5, Skipped:     0, Total:     5, Duration: $TIME - NUnit.Tests.dll (net10.0)
```

### A first MSTest test

An MSTest test is a method with `[TestMethod]` in a class with `[TestClass]`. The test methods must be public instance methods, not static methods.[^6] A parameterized test has one `[DataRow]` attribute for each case.[^7] The assertions are static methods of the class `Assert`, with the expected value first:[^8]

```csharp test id=mstest-first deps=deps/csharp/MSTest.Tests.csproj file=LoanTests.cs timeout=30
using Microsoft.VisualStudio.TestTools.UnitTesting;

public static class Loan
{
    // The monthly payment, rounded up so that the payments cover the amount.
    public static int MonthlyPayment(int amount, int months)
    {
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(months);
        return (amount + months - 1) / months;
    }
}

[TestClass]
public class LoanTests
{
    private static int classSetups;
    private int testSetups;

    [ClassInitialize]
    public static void SetUpClass(TestContext context) => classSetups++;

    [TestInitialize]
    public void SetUpTest() => testSetups++;

    [TestMethod]
    [DataRow(1200, 12, 100)]
    [DataRow(1000, 3, 334)]
    [DataRow(0, 6, 0)]
    public void PaymentRoundsUp(int amount, int months, int expected)
    {
        Assert.AreEqual(expected, Loan.MonthlyPayment(amount, months));
    }

    [TestMethod]
    public void ZeroMonthsIsRejected()
    {
        Assert.ThrowsExactly<ArgumentOutOfRangeException>(() => Loan.MonthlyPayment(1000, 0));
    }

    [TestMethod]
    public void ZeroMonthsIsSomeArgumentException()
    {
        Assert.Throws<ArgumentException>(() => Loan.MonthlyPayment(1000, 0));
    }

    [TestMethod]
    public void EachTestHasItsOwnInstance()
    {
        Assert.AreEqual(1, classSetups);
        Assert.AreEqual(1, testSetups);
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/MSTest.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     6, Skipped:     0, Total:     6, Duration: $TIME - MSTest.Tests.dll (net10.0)
```

The last test passes, because the field `testSetups` is 1 in each test. MSTest makes a new instance of the class for each test, and runs the `[TestInitialize]` method on it. The static field `classSetups` is also 1, because `[ClassInitialize]` runs once for the class. Its method must be "public static", and it takes one `TestContext` parameter.[^9]

## Concepts of the tool

### The attributes compared

The three frameworks have the same main parts with different names. The sources are the attribute lists of NUnit and MSTest, and the page [[xunit]] for xUnit.net.[^10][^6][^9][^7]

| Task | xUnit.net | NUnit | MSTest |
|---|---|---|---|
| Mark a test class | no attribute | `[TestFixture]` | `[TestClass]` |
| Mark a test | `[Fact]` | `[Test]` | `[TestMethod]` |
| One case of a parameterized test | `[Theory]` and `[InlineData]` | `[TestCase]` | `[DataRow]` |
| Cases from code | `[MemberData]` | `[TestCaseSource]` | `[DynamicData]` |
| Setup before each test | the constructor | `[SetUp]` | `[TestInitialize]` or the constructor |
| Cleanup after each test | `Dispose` | `[TearDown]` | `[TestCleanup]` or `Dispose` |
| Setup once for a class | `IClassFixture<T>` | `[OneTimeSetUp]` | `[ClassInitialize]` |
| Setup once for more classes | collection and assembly fixtures | `[SetUpFixture]` | `[AssemblyInitialize]` |
| Skip a test | `Skip = "reason"` | `[Ignore("reason")]` | `[Ignore]` |
| Run only on request | `Explicit = true` | `[Explicit]` | not in the attribute list |

NUnit describes `[SetUp]` as "a method of a TestFixture called just before each test method", and `[SetUpFixture]` as a class "with one-time setup or teardown methods for all the test fixtures in a namespace".[^10] NUnit also has `[DependsOnTest]` and `[DependsOnFixture]`, which make one test or fixture depend on another.[^10]

### Instances of the test class

This is the main difference of the design. xUnit.net "creates a new instance of the test class for every test that is run".[^11] MSTest also makes a new instance for each test: the first step of its test-level order is "Create instance of test class (constructor)".[^9] NUnit makes one instance of the fixture class, and all its tests share it. The documentation of `FixtureLifeCycle` says that with `LifeCycle.SingleInstance`, "A single instance is created and shared for all test cases. This is the default."[^12]

So in NUnit, a field of the fixture keeps its value from one test to the next. A test can then depend on an earlier test, which is a source of order bugs. The attribute `[FixtureLifeCycle(LifeCycle.InstancePerTestCase)]`, added in NUnit 3.13, makes a new instance for each test.[^12] The example shows both behaviors:

```csharp test id=nunit-instance deps=deps/csharp/NUnit.Tests.csproj file=PageTests.cs timeout=30
using NUnit.Framework;

[TestFixture]
public class SharedInstanceTests
{
    private readonly List<string> pages = [];

    [Test, Order(1)]
    public void FirstTestAddsAPage()
    {
        pages.Add("home");
        Assert.That(pages, Has.Count.EqualTo(1));
    }

    [Test, Order(2)]
    public void SecondTestSeesTheFirstPage()
    {
        pages.Add("shop");
        Assert.That(pages, Has.Count.EqualTo(2));
    }
}

[TestFixture]
[FixtureLifeCycle(LifeCycle.InstancePerTestCase)]
public class NewInstanceTests
{
    private readonly List<string> pages = [];

    [Test, Order(1)]
    public void FirstTestAddsAPage()
    {
        pages.Add("home");
        Assert.That(pages, Has.Count.EqualTo(1));
    }

    [Test, Order(2)]
    public void SecondTestStartsEmpty()
    {
        pages.Add("shop");
        Assert.That(pages, Has.Count.EqualTo(1));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/NUnit.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     4, Skipped:     0, Total:     4, Duration: $TIME - NUnit.Tests.dll (net10.0)
```

All four tests pass. In `SharedInstanceTests`, the list has 2 entries in the second test, because the first test added an entry to the same list. In `NewInstanceTests`, each test has a new list. With `InstancePerTestCase`, the `[OneTimeSetUp]` and `[OneTimeTearDown]` methods "must be static".[^12] In NUnit, reset the shared state in a `[SetUp]` method, or use `InstancePerTestCase`.

### The assertion models of NUnit

NUnit has two assertion models. The constraint model is the main one: "The constraint-based Assert model uses a single method of the Assert class for all assertions."[^5] A constraint object, such as `Is.EqualTo`, `Has.Count` or `Throws.TypeOf`, describes the check, and constraints combine with `And` and `Or`.

The classic model has one method for each check, such as `AreEqual`. In NUnit 4, the classic methods moved to the class `ClassicAssert`. The documentation says: "From version 4.5.0, using C# 14, you don't need to use the ClassicAssert class, nor the NUnit.Framework.Legacy" namespace.[^13]

When a constraint fails, the message gives the expression of the call, the expected value and the actual value:

```csharp test fails expect=test-failure id=nunit-message deps=deps/csharp/NUnit.Tests.csproj file=ReceiptTests.cs timeout=30
using NUnit.Framework;

public class ReceiptTests
{
    [Test]
    public void TotalOfTheReceipt()
    {
        int[] prices = [250, 400, 99];
        Assert.That(prices.Sum(), Is.EqualTo(750));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/NUnit.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.
  Failed TotalOfTheReceipt [$TIME]
  Error Message:
     Assert.That(prices.Sum(), Is.EqualTo(750))
  Expected: 750
  But was:  749

  Stack Trace:
     at ReceiptTests.TotalOfTheReceipt() in $WORK/ReceiptTests.cs:line 9

1)    at ReceiptTests.TotalOfTheReceipt() in $WORK/ReceiptTests.cs:line 9



Failed!  - Failed:     1, Passed:     0, Skipped:     0, Total:     1, Duration: $TIME - NUnit.Tests.dll (net10.0)
```

### The assertions of MSTest

MSTest has three assertion classes: `Assert`, `StringAssert` and `CollectionAssert`. Microsoft says: "For new code, always use the Assert class. The StringAssert and CollectionAssert classes are likely to be deprecated in a future release."[^8] Since MSTest 4.0, "all Assert APIs capture the argument expression and include it in failure messages."[^8]

For exceptions, `Assert.Throws` accepts a subclass of the exception type, and `Assert.ThrowsExactly` needs the exact type. The first example shows both. Microsoft recommends them "over the ExpectedException attribute".[^8] MSTest also has soft assertions with `Assert.Scope()`: "while a scope is active, assertion failures are collected instead of thrown". This API is experimental.[^8]

The same failing test in MSTest:

```csharp test fails expect=test-failure id=mstest-message deps=deps/csharp/MSTest.Tests.csproj file=ReceiptTests.cs timeout=30
using Microsoft.VisualStudio.TestTools.UnitTesting;

[TestClass]
public class ReceiptTests
{
    [TestMethod]
    public void TotalOfTheReceipt()
    {
        int[] prices = [250, 400, 99];
        Assert.AreEqual(750, prices.Sum());
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/MSTest.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.
  Failed TotalOfTheReceipt [$TIME]
  Error Message:
   Assertion failed. Expected values to be equal.

expected: 750
actual:   749

Assert.AreEqual(750, prices.Sum())
  Stack Trace:
     at ReceiptTests.TotalOfTheReceipt() in $WORK/ReceiptTests.cs:line 10

Failed!  - Failed:     1, Passed:     0, Skipped:     0, Total:     1, Duration: $TIME - MSTest.Tests.dll (net10.0)
```

## Common tasks

Run the tests of a project, for either framework:

```text
dotnet test
```

Run only the tests of one category. NUnit marks them with `[Category("slow")]`, and MSTest with `[TestCategory("slow")]`:

```text
dotnet test --filter "TestCategory=slow"
```

Run the tests of NUnit in parallel. The attribute `[Parallelizable]` "Indicates whether test and/or its descendants can be run in parallel."[^10] The attribute can go on the assembly:

```text
[assembly: Parallelizable(ParallelScope.Fixtures)]
```

Run the tests of MSTest in parallel, with the attribute `Parallelize` on the assembly. The attribute `DoNotParallelize` keeps a test out of the parallel run.[^6]

```text
[assembly: Parallelize(Workers = 0, Scope = ExecutionScope.MethodLevel)]
```

## Configuration

MSTest can read its settings from a `.runsettings` file or from a `testconfig.json` file.[^9] In both frameworks, the attributes on the assembly, such as `Parallelizable` and `Parallelize`, are the other main way to configure a run.[^10][^6]

## Alternatives

- [[xunit]]: the framework of the .NET Foundation, with a new instance of the test class for each test and parallel runs by default.
- [[junit-5]]: the test framework of Java. The attribute `[Test]` of NUnit matches the annotation `@Test` of JUnit.

## History and versions

| Version | Date | Note |
|---|---|---|
| NUnit 3.13 | January 7, 2021 | added `FixtureLifeCycle`[^14][^12] |
| NUnit 4.0.0 | November 26, 2023 | the minimum target frameworks became .NET Framework 4.6.2 and .NET 6.0[^14] |
| NUnit 5.0.0 | Sept 27, 2026 | "a major release, with a series of breaking changes"[^14] |

The documentation of NUnit says that "NUnit 2 is a different product, with its own design and its own documentation."[^1] The notes of NUnit 5.0.0 say that the breaking changes "follow up on version 4's move towards a more type-safe NUnit."[^14]

## References
