---
tag: HCQENS
kind: tool
slug: xunit-net
name: xUnit.net releases
aliases: [xUnit.net v2, xUnit.net v3]
area: testing/test-frameworks
level: core
summary: >-
  The versions of xUnit.net: v1, v2 with parallel runs by default, and v3 with test projects that are programs. This page shows the changes from v2 to v3 and how to migrate.
role: [test-framework]
licence: apache-2.0
repo: https://github.com/xunit/xunit
homepage: https://xunit.net
see_also: [xunit, nunit-and-mstest, test-framework, junit]
sources:
  - url: https://xunit.net/releases/
    title: xUnit.net, Release Notes
    locator: Core Framework v3; Core Framework v2; Visual Studio adapter
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/migration
    title: xUnit.net, Migrating Unit Tests from v2 to v3
    locator: Architectural Changes; Migrating to v3 Packages; Convert to Executable Project
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v2/getting-started
    title: xUnit.net, Getting Started with xUnit.net v2
    locator: the introduction; Create the unit test project
    accessed: 2026-11-15
  - url: https://xunit.net/docs/shared-context
    title: xUnit.net, Sharing Context between Tests
    locator: Constructor and Dispose
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/whats-new
    title: xUnit.net, What's New in v3?
    locator: Dynamically skippable tests; Explicit tests; Query filter language; Test context; Capturing Console, Debug, and Trace output
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v2/2.0.0
    title: xUnit.net, Core Framework v2 2.0.0
    locator: Hello, v2!; Theories; Parallelism
    accessed: 2026-11-15
  - url: https://xunit.net/docs/capturing-output
    title: xUnit.net, Capturing Output
    locator: the introduction
    accessed: 2026-11-15
  - url: https://xunit.net/docs/running-tests-in-parallel
    title: xUnit.net, Running Tests in Parallel
    locator: Algorithms; Parallel modes
    accessed: 2026-11-15
  - url: https://xunit.net/docs/nuget-packages-v3
    title: xUnit.net, What NuGet Packages Should I Use? (v3)
    locator: xunit.v3.runner.console; xunit.runner.visualstudio
    accessed: 2026-11-15
  - url: https://xunit.net/xunit.analyzers/rules/xUnit1049
    title: xUnit.net, xUnit1049
    locator: Cause; Reason for rule; How to fix violations
    accessed: 2026-11-15
  - url: https://xunit.net/docs/getting-started/v3/microsoft-testing-platform
    title: xUnit.net, Microsoft Testing Platform (xUnit.net v3)
    locator: the package list
    accessed: 2026-11-15
  - url: https://xunit.net/docs/config-xunit-runner-json
    title: xUnit.net, Config with xunit.runner.json
    locator: parallelizeTestCollections; parallelMode
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v2/2.9.3
    title: xUnit.net, Core Framework v2 2.9.3
    locator: the note
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v3/1.0.0
    title: xUnit.net, Core Framework v3 1.0.0
    locator: Hello, v3!
    accessed: 2026-11-15
  - url: https://xunit.net/releases/v3/4.0.3
    title: xUnit.net, Core Framework v3 4.0.3
    locator: the introduction
    accessed: 2026-11-15
---

## What it is

This is the version page of [[xunit]]. The framework has three major versions, v1, v2 and v3. Each major version changed how tests run, so the version of a test project decides which documentation, packages and runners apply to it. The page [[xunit]] explains the framework itself: facts, theories, fixtures and test collections.

The release notes list the versions by the NuGet package that ships them:[^1]

| Product | NuGet package | Releases on the release notes page |
|---|---|---|
| Core Framework v2 | `xunit` and friends | 2.0.0 to 2.9.3 |
| Core Framework v3 | `xunit.v3` and friends | 1.0.0 to 4.0.3 |
| Visual Studio adapter | `xunit.runner.visualstudio` | 2.5.0 to 4.0.1 |

The page says that it "only lists packages that are available on NuGet.org and have associated release notes", so v1 is not in the table.[^1]

A version such as v3 4.0.3 has two parts. The part v3 is the version of the product, and the part 4.0.3 is the version of the package. The migration guide explains that the new package names let "the v3 product version to live in the package name instead of the major version", so the package versions can follow SemVer.[^2]

## Install and first use

A v2 project is a class library. The v2 guide makes it with the template of the .NET SDK, `dotnet new xunit`, and the project gets three packages:[^3]

```text
<PackageReference Include="Microsoft.NET.Test.Sdk" Version="17.13.0" />
<PackageReference Include="xunit" Version="2.9.3" />
<PackageReference Include="xunit.runner.visualstudio" Version="3.1.1" />
```

A v3 project is a program. The migration guide says: "Test projects in v3 are stand-alone executables now, capable of running themselves."[^2] To move a project from v2 to v3, the guide gives three main steps: update the NuGet packages, make the project an executable, and update the target framework.[^2] For the second step, it says: "Update your project file (i.e., .csproj ) and change OutputType from Library to Exe ."[^2]

```text
<OutputType>Exe</OutputType>
```

The new minimum runtimes of v3 are ".NET Framework 4.7.2 (or later)" and ".NET 8 (or later)".[^2]

The example below is a v2 test class. It runs with the packages `xunit` 2.9.3 and `xunit.runner.visualstudio` 3.1.4. The class needs async setup and cleanup, so it implements `IAsyncLifetime`. In v2, both methods of this interface return `Task`:[^2][^4]

```csharp test id=v2 deps=deps/csharp/V2.Tests.csproj file=CacheTests.cs timeout=30
using Xunit;

public class Cache
{
    private readonly Dictionary<string, string> items = [];

    public bool Open { get; private set; }

    public async Task OpenAsync()
    {
        await Task.Delay(10);
        Open = true;
    }

    public async Task CloseAsync()
    {
        await Task.Delay(10);
        Open = false;
    }

    public void Put(string key, string value) => items[key] = value;

    public string? Get(string key) => items.GetValueOrDefault(key);
}

public class CacheTests : IAsyncLifetime
{
    private readonly Cache cache = new();

    public Task InitializeAsync() => cache.OpenAsync();

    public Task DisposeAsync() => cache.CloseAsync();

    [Fact]
    public void CacheIsOpenInTheTest()
    {
        Assert.True(cache.Open);
    }

    [Fact]
    public async Task StoredValueComesBack()
    {
        cache.Put("tea", "green");
        await Task.Yield();
        Assert.Equal("green", cache.Get("tea"));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/V2.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     2, Skipped:     0, Total:     2, Duration: $TIME - V2.Tests.dll (net10.0)
```

The same test class in v3 uses `ValueTask` for both methods. It also passes the cancellation token of the test context to the code under test:[^2][^5]

```csharp test id=v3 deps=deps/csharp/V3.Tests.csproj file=CacheTests.cs timeout=30
using Xunit;

public class Cache
{
    private readonly Dictionary<string, string> items = [];

    public bool Open { get; private set; }

    public async Task OpenAsync(CancellationToken token)
    {
        await Task.Delay(10, token);
        Open = true;
    }

    public async Task CloseAsync()
    {
        await Task.Delay(10);
        Open = false;
    }

    public void Put(string key, string value) => items[key] = value;

    public string? Get(string key) => items.GetValueOrDefault(key);
}

public class CacheTests : IAsyncLifetime
{
    private readonly Cache cache = new();

    public async ValueTask InitializeAsync() => await cache.OpenAsync(TestContext.Current.CancellationToken);

    public async ValueTask DisposeAsync() => await cache.CloseAsync();

    [Fact]
    public void CacheIsOpenInTheTest()
    {
        Assert.True(cache.Open);
    }

    [Fact]
    public async Task StoredValueComesBack()
    {
        cache.Put("tea", "green");
        await Task.Delay(1, TestContext.Current.CancellationToken);
        Assert.Equal("green", cache.Get("tea"));
    }
}
```

```output
Test run for $WORK/bin/Debug/net10.0/V3.Tests.dll (.NETCoreApp,Version=v10.0)
A total of 1 test files matched the specified pattern.

Passed!  - Failed:     0, Passed:     2, Skipped:     0, Total:     2, Duration: $TIME - V3.Tests.dll (net10.0)
```

## Concepts of the tool

### v1 and v2

The release notes of v2 2.0.0 give the timeline of v2. v1 1.9 shipped in January 2012, the first public alpha of v2 came in August 2012, and the first RTM release of v2 came in March 2015.[^6] The notes say: "Our 2½ year journey included 20 alphas, 5 betas, and 4 release candidates".[^6]

v2 made these main changes:

- Theories moved into the core. The notes say: "One of the very first things we did for 2.0 was to remove the xunit.extensions project, and integrate the theory support directly into xUnit.net core."[^6]
- Tests run in parallel by default. The notes say: "One of the premiere new features in xUnit.net 2.0 is support for parallelism."[^6] Tests in one test class do not run in parallel with each other, but tests in different test classes do.[^6]
- Output from `Console` is not captured. The documentation says: "xUnit.net v2 removed this ability and replaced it with an injectable ITestOutputHelper that could be used to capture output from a test."[^7]

In v2 2.8, the default algorithm of the parallel runs changed. The two algorithms "are called conservative (the new default) and aggressive (the original algorithm)".[^8]

### The architecture of v3

v3 changes how a runner finds and runs the tests. A v2 runner loads the test assembly into its own process. A v3 test project builds to a program, and the program runs its own tests.[^2] The package guide gives one result of this change: "Since xUnit.net v3 projects are stand-alone executables, they will run with whatever version (and bitness) of .NET or .NET Framework that they were built against regardless of the runner's version."[^9]

The packages have new names that start with `xunit.v3`.[^2] For the reason, the migration guide says: "We wanted users to make a conscious choice to upgrade and understand what the scope of that work is, rather than being offered a NuGet package upgrade in Visual Studio and then have everything be broken without being told why."[^2]

The runners of v3 can still run old tests. The console runner `xunit.v3.runner.console` "is capable of running .NET Framework and .NET projects from xUnit.net v3, as well as .NET Framework projects from xUnit.net v1 and v2."[^9] The Visual Studio adapter can run v3 tests only from its version 3.0: "(Support for v3 requires using runner version 3.0 or later.)"[^9]

### Breaking changes from v2 to v3

| Area | v2 | v3 |
|---|---|---|
| Project type | class library | executable, `<OutputType>Exe</OutputType>`[^2] |
| Packages | `xunit`, `xunit.core`, `xunit.assert` | `xunit.v3`, `xunit.v3.core`, `xunit.v3.assert`[^2] |
| Minimum runtimes | lower minimums | .NET Framework 4.7.2 and .NET 8[^2] |
| `IAsyncLifetime` | its own `DisposeAsync`, and methods that return `Task` | inherits `IAsyncDisposable`, and methods that return `ValueTask`[^2] |
| `async void` tests | supported | fail at run time[^2] |
| Output of `Console` | not captured | captured when an assembly attribute asks for it[^5] |

For `async void`, the guide says: "Tests which are async void will be fast-failed at runtime to indicate that their signatures need to be updated from void to either Task or ValueTask ."[^2] An `async void` method gives no task to wait for, so a runner cannot know when such a test ends.

In practice, a v3 project usually does not get as far as the run. The package `xunit.v3` brings the analyzers of xUnit.net,[^9] and the rule xUnit1049 is an error: "Do not use 'async void' for test methods as it is no longer supported". The rule "will only trigger for v3 projects".[^10] So a v2 test such as this one stops the build of a v3 project:

```text
using Xunit;

public class ReportTests
{
    [Fact]
    public async void ReportIsWritten()
    {
        await Task.Delay(10);
        Assert.True(true);
    }
}
```

With xunit.v3.mtp-off 4.0.3 on .NET 10, `dotnet build` stops with this error. The path of the file is shortened here:

```text
ReportTests.cs(6,23): error xUnit1049: Support for 'async void' unit tests has been removed from xUnit.net v3. Convert the test to 'async Task' or 'async ValueTask' instead. (https://xunit.net/xunit.analyzers/rules/xUnit1049)
```

The fix is to change the return type from `void` to `Task` or `ValueTask`:[^10]

```text
[Fact]
public async Task ReportIsWritten()
{
    await Task.Delay(10, TestContext.Current.CancellationToken);
    Assert.True(true);
}
```

For the cleanup, v3 calls only one method. The guide says: "For xUnit.net, that means it will call DisposeAsync but not Dispose ." This is true for every object that implements both `IAsyncDisposable` and `IDisposable`.[^2] A v2 test class that implements both interfaces and needs both calls must move its cleanup into `DisposeAsync`.

### New features of v3

The "What's New" guide lists the features of v3.[^5] The main ones are:

- Tests that skip themselves at run time, with `Assert.Skip`, `Assert.SkipUnless` and `Assert.SkipWhen`.[^5]
- Explicit tests, which a run skips unless it asks for them. Such tests are "reported in runners as "not run"".[^5]
- A query filter language. The guide says that for v3 runners "we have added a new "query filter language" that can be invoked via -filter ."[^5]
- The test context. It is available from anywhere "via the static TestContext.Current ", and it gives a cancellation token.[^5]
- Capture of `Console`, `Debug` and `Trace`. The guide says: "In v2, we do not capture any output calls to Console , Debug , or Trace ."[^5] In v3, the assembly attribute `[assembly: CaptureConsole]` turns on the capture of the console.[^5]
- Assembly fixtures, which are shared by all the tests of an assembly.[^4]

In v3 4.0 and later, there is a third parallel mode. The guide says: "in xUnit.net v3 starting with 4.0, a third mode is supported ( all )". In this mode, "all tests are run in parallel against all other tests."[^8]

### Microsoft Testing Platform

v3 can use the Microsoft Testing Platform (MTP), the newer test platform of Microsoft, in place of VSTest. MTP v1 was the default in the 3.x.y builds, and MTP v2 is the default from 4.0.0.[^11] The package of a project chooses the mode:[^11]

| Package | MTP support |
|---|---|
| `xunit.v3` | the default version |
| `xunit.v3.mtp-v1` | MTP v1, only for the 3.x.y versions |
| `xunit.v3.mtp-v2` | MTP v2 |
| `xunit.v3.mtp-off` | no MTP |

The examples on the page [[xunit]] use `xunit.v3.mtp-off` with the VSTest runner of `dotnet test`.

## Common tasks

Migrate a project from v2 to v3, with the steps of the migration guide:[^2]

1. In the project file, replace the package `xunit` with `xunit.v3`. Remove the packages that v3 does not need, such as `xunit.abstractions`. Replace `xunit.extensibility.core` and `xunit.extensibility.execution` with `xunit.v3.extensibility.core`, which merges them.
2. Set `<OutputType>Exe</OutputType>`.
3. Set the target framework to .NET 8 or later, or to .NET Framework 4.7.2 or later.
4. Use version 3.0 or later of `xunit.runner.visualstudio`.
5. Change `async void` tests to `async Task`, and change the methods of `IAsyncLifetime` to return `ValueTask`.
6. Fix the namespaces. For example, `ITestOutputHelper` moved from `Xunit.Abstractions` to `Xunit`.[^2]

Run a v3 project and write an XML report. The guide shows that these two commands do the same:[^2]

```text
.\bin\Debug\TestProject.exe -xml results.xml
dotnet run -- -xml results.xml
```

## Configuration

Both v2 and v3 read the file `xunit.runner.json`. Some settings are for v3 only. For example, the setting `parallelMode` is for v3 4.0 and later, and the older setting `parallelizeTestCollections` maps to it.[^12] The page [[xunit]] shows a configuration file.

## Alternatives

- Stay on v2. v2 still gets critical fixes, and the last release of v2 is 2.9.3 so far.[^13] But the release notes say that v2 "is in maintenance mode", and that "Developers are encouraged to upgrade from v2 to v3 at their earliest convenience."[^1]
- Move to NUnit or MSTest. See [[nunit-and-mstest]].

## History and versions

| Version | Date | Note |
|---|---|---|
| v1 1.9 | January 2012 | shipped before the first public alpha of v2[^6] |
| v2 2.0.0 | 2015 March 16 | the first RTM release of v2[^6] |
| v3 1.0.0 | 2024 December 16 | "Today we're shipping the first release version of xUnit.net v3!"[^14] |
| v2 2.9.3 | 2025 January 8 | "We anticipate this to be the last release of Core Framework v2, as all effort is focused on v3 now."[^13] |
| v3 4.0.3 | 2026 November 8 | the current release of v3[^15] |

Version 3.0.0 of the Visual Studio adapter shipped on the same day as v3 1.0.0.[^14] The notes of v3 1.0.0 also say: "We have shipped several prerelease versions over the last 6 months".[^14]

## References
