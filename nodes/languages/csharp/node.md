---
tag: NEW-csharp
kind: language
slug: csharp
name: "C#"
area: languages
level: intro
summary: >-
  C# is a general-purpose, object-oriented language with static, strong typing and garbage collection. Programs compile to an intermediate language that the .NET runtime runs.
class: programming
tier: 1
paradigms: [object-oriented, imperative, functional, generic, concurrent]
typing: {discipline: static, strength: strong, checking: [static, runtime], inference: local}
memory: garbage-collected
execution: [vm, jit, compiled-native]
appeared: 2000
designers: [anders-hejlsberg, scott-wiltamuth, peter-golde]
influenced_by: [c, cpp, java]
standard: ecma-334
reference: {url: "https://learn.microsoft.com/dotnet/csharp/language-reference/", licence: cc-by-4.0}
licence_of_reference_impl: mit
file_extensions: [cs, cake, cs.pp, csx, linq]
hello_world: examples/csharp.md#hello
same_as: {wikidata: Q2370, wikipedia: C_Sharp_(programming_language), pldb: csharp, linguist: "C#", rosetta: null, hopl: null}
sources:
  - url: https://learn.microsoft.com/dotnet/csharp/tour-of-csharp/overview
    title: Overview, A tour of C#, Microsoft Learn
    locator: the introduction; Hello world; File-based apps
    accessed: 2026-11-10
  - url: https://ecma-international.org/publications-and-standards/standards/ecma-334/
    title: ECMA-334, C# language specification
    locator: the introduction (7th edition, December 2023); the table of Microsoft Visual C# versions with the corresponding Ecma and ISO/IEC standards; Archives
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/introduction.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: Introduction
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/introduction
    title: Introduction to .NET, Microsoft Learn
    locator: Components; Free and open source; Support; .NET ecosystem
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/language-reference/language-versioning
    title: Language versioning, C# reference, Microsoft Learn
    locator: Defaults
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard
    title: dotnet/csharpstandard, the C# language standard
    locator: README, the introduction and C# Language Specification
    accessed: 2026-11-10
  - url: https://github.com/dotnet/docs/blob/main/LICENSE
    title: dotnet/docs, LICENSE
    locator: Attribution 4.0 International
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/fundamentals/program-structure/top-level-statements
    title: Top-level statements, programs without Main methods, Microsoft Learn
    locator: Entry point rules
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/sdk/file-based-apps
    title: File-based apps, .NET, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/fundamentals/types/records
    title: C# record types, Microsoft Learn
    locator: the introduction; Declare a record
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/language-reference/operators/switch-expression
    title: switch expression, C# reference, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/linq/
    title: Language Integrated Query (LINQ), Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/asynchronous-programming/
    title: Asynchronous programming with async and await, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://github.com/github-linguist/linguist/blob/main/lib/linguist/languages.yml
    title: GitHub Linguist, languages.yml
    locator: C#, extensions
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/types.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §8.1 General; §8.3.1 General; §8.3.12 Nullable value types; §8.3.13 Boxing and unboxing; §8.4.3 Open and closed types; §8.7 The dynamic type
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/structs.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §16.1 General
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/statements.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §13.6.2.2 Implicitly typed local variable declarations; §13.13 The lock statement; §13.14 The using statement
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/programming-guide/generics/generics-in-the-run-time
    title: Generics in the runtime, C# programming guide, Microsoft Learn
    locator: the whole article
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/expressions.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §12.8.19 The checked and unchecked operators; §12.19.6.2 Captured outer variables; §12.20.1 General
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/fundamentals/null-safety/nullable-reference-types
    title: Nullable reference types, C#, Microsoft Learn
    locator: the introduction; Express intent with annotations
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/basic-concepts.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §7.9 Automatic memory management
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/unsafe-code.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §23.1 General
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/standard/managed-execution-process
    title: Managed execution process, .NET, Microsoft Learn
    locator: the introduction; Compile to CIL; Compilation by the JIT compiler
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/deploying/native-aot/
    title: Native AOT deployment, .NET, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://github.com/dotnet/csharpstandard/blob/standard-v7/standard/classes.md
    title: C# language specification, ECMA-334 7th edition, Markdown text of the standard-v7 branch
    locator: §15.14 Iterators; §15.15 Async Functions
    accessed: 2026-11-10
  - url: https://dotnet.microsoft.com/platform/support/policy/dotnet-core
    title: .NET and .NET Core official support policy
    locator: Release cadence; Release types (LTS, STS); Go-live; the tables of supported and out-of-support versions
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/whats-new/csharp-14
    title: What's new in C# 14, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/whats-new/csharp-15
    title: What's new in C# 15, Microsoft Learn
    locator: the introduction
    accessed: 2026-11-10
  - url: https://dotnetcli.blob.core.windows.net/dotnet/release-metadata/releases-index.json
    title: .NET release metadata, releases-index.json
    locator: the entries for channels 6.0 to 11.0
    accessed: 2026-11-10
  - url: https://endoflife.date/api/v1/products/dotnet/
    title: .NET, endoflife.date API v1
    locator: the releases 5 to 10
    accessed: 2026-11-10
  - url: https://github.com/dotnet/roslyn
    title: dotnet/roslyn, The .NET Compiler Platform
    locator: README, the introduction; LICENSE (MIT)
    accessed: 2026-11-10
  - url: https://github.com/dotnet/runtime
    title: dotnet/runtime
    locator: LICENSE.TXT (MIT)
    accessed: 2026-11-10
  - url: https://www.nuget.org/packages/Microsoft.CodeAnalysis.CSharp/5.9.0
    title: Microsoft.CodeAnalysis.CSharp 5.9.0, NuGet Gallery
    locator: Version History
    accessed: 2026-11-10
  - url: https://github.com/mono/mono
    title: mono/mono
    locator: README, the introduction and the notice on the stewardship of the project; LICENSE; the tag mono-6.12.0.206
    accessed: 2026-11-10
  - url: https://endoflife.date/api/v1/products/dotnetfx/
    title: .NET Framework, endoflife.date API v1
    locator: the release 4.8.1
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/nuget/what-is-nuget
    title: What is NuGet and what does it do?, Microsoft Learn
    locator: The flow of packages between creators, hosts, and consumers
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/testing/
    title: Testing in .NET, Microsoft Learn
    locator: Testing tools
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/tools/dotnet-build
    title: dotnet build command, .NET CLI, Microsoft Learn
    locator: Description
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/tools/dotnet-test
    title: dotnet test command, .NET CLI, Microsoft Learn
    locator: Description
    accessed: 2026-11-10
  - url: https://xunit.net/
    title: xUnit.net
    locator: the home page
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/core/tools/dotnet-format
    title: dotnet format command, .NET CLI, Microsoft Learn
    locator: Description
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/language-reference/compiler-options/errors-warnings
    title: Compiler options, errors and warnings, C# reference, Microsoft Learn
    locator: TreatWarningsAsErrors
    accessed: 2026-11-10
  - url: https://pldb.io/concepts/csharp.html
    title: C#, PLDB
    locator: the introduction
    accessed: 2026-11-10
  - url: https://www.wikidata.org/wiki/Q2370
    title: C# (Q2370), Wikidata
    locator: inception (P571); designed by (P287); influenced by (P737)
    accessed: 2026-11-10
  - url: https://learn.microsoft.com/dotnet/csharp/whats-new/csharp-version-history
    title: The history of C#, Microsoft Learn
    locator: the sections C# version 1.0 to C# version 14
    accessed: 2026-11-10
  - url: https://dotnetcli.blob.core.windows.net/dotnet/release-metadata/1.0/releases.json
    title: .NET Core 1.0 release metadata, releases.json
    locator: the release 1.0.0
    accessed: 2026-11-10
---

## Overview

C# is a general-purpose, object-oriented language with strong, static typing. Its syntax is in the C family, so it is familiar to programmers of C, C++, Java, JavaScript and TypeScript. It also takes many features from other paradigms, functional programming in particular.[^1] Ecma International publishes the standard as ECMA-334, and ISO/IEC has published editions of it as ISO/IEC 23270 and ISO/IEC 20619.[^2]

The standard says that the language comes from a submission by Hewlett-Packard, Intel and Microsoft. It names Anders Hejlsberg, Scott Wiltamuth and Peter Golde as the principal inventors. Microsoft released the first widely distributed implementation in July 2000, as part of its .NET Framework initiative.[^3]

The Introduction of the standard lists the design goals. C# is a general-purpose, object-oriented language. It supports strong type checking, array bounds checking, the detection of uninitialized variables and automatic garbage collection. It is for software components in distributed environments, and source code must be portable, in particular for programmers who know C and C++. It supports internationalization and suits hosted and embedded systems. The goals also say that C# does not compete directly with C or assembly language on performance and size.[^3]

C# is the primary language of .NET, and much of .NET is written in C#.[^4] Each C# version ships with a .NET release, and the compiler selects the default language version from the target framework.[^5] The 7th edition of ECMA-334, of December 2023, specifies C# 7.[^2] The committee Ecma TC49-TG2 writes the drafts for later versions in a public repository, and language design takes place in a separate repository, csharplang.[^6] Microsoft's C# language reference documents the language as its compiler implements it. Its text is under the Creative Commons Attribution 4.0 licence.[^7]

## A first program

```csharp run id=hello
Console.WriteLine("Hello, world!");
```

```output for=hello
Hello, world!
```

This file has top-level statements and no `Main` method. The compiler generates the method that is the entry point of the program.[^8] From C# 14 and .NET 10, the command `dotnet run hello.cs` builds and runs a single file with no project file.[^1][^9]

The program below models shapes as records. A `switch` expression selects the area formula, a query with lambda expressions filters the shapes, and two asynchronous calls run together.

```csharp run id=shapes
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

Shape[] shapes = [new Circle(1.5), new Rect(2, 3), new Square(4)];

foreach (Shape shape in shapes)
{
    Console.WriteLine($"{shape} has area {Area(shape):F2}");
}

var large = shapes.Where(s => Area(s) > 7).Select(s => s.GetType().Name);
Console.WriteLine(string.Join(", ", large));

Console.WriteLine(new Rect(2, 3) == new Rect(2, 3));
Console.WriteLine(new Square(4) with { Side = 5 });

string[] words = ["alpha", "beta", "gamma"];
int[] lengths = await Task.WhenAll(words.Select(LengthAsync));
Console.WriteLine(string.Join(" ", lengths));

static double Area(Shape shape) => shape switch
{
    Circle(var r) => Math.PI * r * r,
    Rect { Width: var w, Height: var h } => w * h,
    Square s => s.Side * s.Side,
    _ => throw new ArgumentException("unknown shape", nameof(shape)),
};

static async Task<int> LengthAsync(string word)
{
    await Task.Yield();
    return word.Length;
}

abstract record Shape;
record Circle(double Radius) : Shape;
record Rect(double Width, double Height) : Shape;
record Square(double Side) : Shape;
```

```output for=shapes
Circle { Radius = 1.5 } has area 7.07
Rect { Width = 2, Height = 3 } has area 6.00
Square { Side = 4 } has area 16.00
Circle, Square
True
Square { Side = 5 }
5 4 5
```

The `record` modifier tells the compiler to generate value equality, a formatted `ToString` and nondestructive mutation with `with` expressions. So the two `Rect(2, 3)` values are equal.[^10] The `switch` expression returns the value of the first arm whose pattern matches the input. Here the arms use a positional pattern, a property pattern and a type pattern.[^11] `Where` and `Select` come from Language-Integrated Query (LINQ), which puts query capabilities into the language.[^12] The `async` and `await` keywords let code that waits for tasks read as a sequence of statements ([[async-await]]).[^13]

## The language at a glance

A source file has the extension `.cs`. GitHub Linguist counts files with the extensions `.cs`, `.cake`, `.cs.pp`, `.csx` and `.linq` as C#.[^14]

**Types.** Every variable has a type that is known at compile time ([[static-and-dynamic-typing]]).[^1] The types are value types and reference types. A variable of a value type contains its data, and a variable of a reference type stores a reference to an object. The type system is unified: every type derives from `object`, and boxing lets a program treat a value as an object.[^15] A `struct` is a value type, and the types that C# provides, such as `int`, `double` and `bool`, are all struct types.[^16] The keyword `var` declares a local variable whose type the compiler infers from the initializer.[^17] The type `dynamic` uses dynamic binding instead of static binding.[^15]

Generic types take type parameters ([[parametric-polymorphism]]). At run time each closed constructed type has its own static variables.[^15] The runtime makes a specialized version of a generic type for each value type argument, and reflection can find the type argument at run time. So generics in C# are reified, not erased as in Java ([[type-erasure]]).[^18]

```csharp run id=types
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

Point p = new() { X = 1, Y = 2 };
Point q = p;
q.X = 10;
Console.WriteLine($"{p.X} {q.X}");

Box b = new() { Value = 1 };
Box c = b;
c.Value = 10;
Console.WriteLine($"{b.Value} {c.Value}");

int big = int.MaxValue;
Console.WriteLine(big + 1);
try
{
    Console.WriteLine(checked(big + 1));
}
catch (OverflowException e)
{
    Console.WriteLine(e.GetType().Name);
}

Console.WriteLine(typeof(List<string>) == typeof(List<int>));
Console.WriteLine(typeof(List<int>));

int? missing = null;
Console.WriteLine(missing ?? 0);

struct Point
{
    public int X;
    public int Y;
}

class Box
{
    public int Value;
}
```

```output for=types
1 10
10 10
-2147483648
OverflowException
False
System.Collections.Generic.List`1[System.Int32]
0
```

An assignment to a struct variable copies the value, so `q` is a separate point. An assignment of a class variable copies the reference, so `b` and `c` refer to the same box.[^15] An expression that is not constant and not inside `checked` or `unchecked` uses the unchecked context by default, so the addition wraps. In a `checked` context it throws a `System.OverflowException` ([[integer-overflow]]).[^19] The type `int?` is a nullable value type, which can also hold `null`.[^15] For reference types, nullable annotations and warnings help to prevent a `NullReferenceException`. C# uses static analysis and warning diagnostics instead of a separate type ([[null-references]]).[^20]

**Memory.** C# manages memory automatically with a garbage collector, and a finalizer runs at some later time when an object is no longer in use ([[garbage-collection]]).[^21] The .NET collector is a self-tuning, tracing collector. A program can release a resource promptly with `IDisposable` and the `using` statement.[^4][^17] This is the C# form of scope-based cleanup ([[resource-acquisition-is-initialization]]). See [[stack-and-heap]] for value types and reference types. The core language has no pointer types, so an uninitialized variable or an array index past the bounds is not possible. Unsafe code adds pointers for work such as calls into the operating system, and the standard makes this part conditionally normative.[^22]

**Execution.** The compiler translates source code to common intermediate language (CIL) and metadata. At run time, a JIT compiler translates the CIL to native code on demand.[^23] A Native AOT publication compiles the code ahead of time instead, and the app then runs with no JIT compiler.[^24]

**Paradigms.** The type system has single inheritance of classes, interfaces with default method implementations, and virtual method dispatch ([[interfaces]]).[^4] Anonymous functions can capture the local variables of the method around them ([[closures]], [[higher-order-functions]]).[^19] An iterator uses `yield` to produce a sequence ([[iterators]]), and an async function can suspend at `await` ([[async-await]]).[^25] The `lock` statement holds the mutual-exclusion lock of an object while a block runs ([[threads]], [[mutual-exclusion]]).[^17]

## The guide

The guide to C# has the 25 parts of the language template. Only this page exists so far, so the table has no links yet.

| Part | What it covers in C# |
|---|---|
| Overview and history | this page: design goals, the standard, versions, implementations, history |
| Getting started | installing the .NET SDK, `dotnet new`, `dotnet run`, file-based apps |
| Lexical structure and syntax | Unicode source, identifiers, keywords and contextual keywords, literals, raw string literals, preprocessor directives |
| Values and types | value types and reference types, the numeric types, `bool` and `char`, boxing, nullable value types, `var`, `dynamic` |
| Variables, binding and scope | locals, fields, definite assignment, `readonly`, `const`, `ref` locals |
| Control flow | `if`, `switch` statements and expressions, loops, `foreach`, `goto`, patterns |
| Functions | methods, overloading, `ref`, `out` and `in` parameters, local functions, lambda expressions, delegates |
| Built-in collections | arrays, `List<T>`, `Dictionary<TKey,TValue>`, spans, collection expressions, LINQ |
| Strings and text | `string` and `char`, UTF-16, interpolation, `StringBuilder`, culture and formatting, `Regex` |
| User-defined types | classes, structs, records, interfaces, enums, generics, extension members |
| Errors and exceptions | `try`, `catch`, `finally` and `throw`, exception filters, `System.Exception` |
| Memory and resources | the garbage collector, `IDisposable` and `using`, finalizers, `Span<T>`, unsafe code |
| Modules, packages and builds | namespaces, assemblies, project files, MSBuild, NuGet packages |
| Concurrency and parallelism | tasks, `async` and `await`, threads, `lock`, channels, `Parallel` |
| Metaprogramming | attributes, reflection, source generators, expression trees |
| The standard library | a map of the .NET base class library; the api cards hang here |
| I/O, files and the OS | `System.IO`, streams, `Process`, `System.Text.Json`, `HttpClient` |
| Testing | xUnit, NUnit, MSTest, `dotnet test` |
| Tooling | the `dotnet` CLI, Roslyn analyzers, `dotnet format`, debuggers and profilers |
| Ecosystem | NuGet, the .NET Foundation, csharplang and the standard, libraries by need |
| Performance | the JIT and tiered compilation, Native AOT, allocation, BenchmarkDotNet |
| Interoperability | P/Invoke, COM, function pointers, other .NET languages |
| Idioms and style | the .NET naming and coding conventions, records for data, LINQ, `using` declarations |
| Pitfalls | struct copies, closures over loop variables, `async void`, unchecked overflow, null |
| Versions and migration | the yearly release, LTS and STS, language version defaults, preview features |

## Versions and support

A new major release of .NET comes out every November. Even-numbered releases are long-term support (LTS) releases with three years of free support, and odd-numbered releases are standard-term support (STS) releases with two years. Microsoft supports go-live releases, which are typically release candidates, in production. It does not support previews.[^26] .NET gets a patch release each month on the second Tuesday.[^4]

Each .NET version has a default C# version. .NET 10 uses C# 14, .NET 9 uses C# 13, and .NET Framework uses C# 7.3. C# 14 is supported only on .NET 10 and newer versions.[^5] What's new in C# 14 lists extension members, the `field` keyword in properties and null-conditional assignment, among others.[^27] C# 15 is the default for projects that target .NET 11, whose SDK is still a prerelease. It adds union types and closed hierarchies, among other features ([[sum-types]]).[^28]

The guide states facts for C# 14 on .NET 10. The examples run on the .NET 10 SDK.

| C# version | .NET release | Status | First release | Newest release | End of support |
|---|---|---|---|---|---|
| 15 | .NET 11 (STS) | go-live release candidate | not released | 11.0.0-rc.3 (2026-10-25) | two years after the release |
| 14 | .NET 10 (LTS) | active | 2025-11-11 | 10.0.14 (2026-10-25) | 2028-11-14 |
| 13 | .NET 9 (STS) | maintenance | 2024-11-12 | 9.0.22 (2026-10-25) | 2026-11-10 |
| 12 | .NET 8 (LTS) | maintenance | 2023-11-14 | 8.0.33 (2026-10-25) | 2026-11-10 |
| 11 | .NET 7 (STS) | end of support | 2022-11-08 | 7.0.20 (2024-05-28) | 2024-05-14 |
| 10 | .NET 6 (LTS) | end of support | 2021-11-08 | 6.0.36 (2024-11-12) | 2024-11-12 |

The dates and statuses are Microsoft's.[^26] The support for .NET 8 and .NET 9 ends on 2026-11-10, the day these facts were checked. The release metadata gives the newest patches,[^29] and endoflife.date agrees on the end dates.[^30]

## Implementations

The .NET SDK holds the C# compiler, and the .NET runtime runs the compiled code. The source and binaries of .NET are under the MIT licence, and other licences apply on Windows.[^4] Roslyn is the open-source implementation of the C# and Visual Basic compilers, with an API for code analysis tools.[^31] The table gives the newest release of each on 2026-11-10.

| Implementation | Runs on | C# version | Newest release | Licence |
|---|---|---|---|---|
| .NET (SDK and runtime) | several operating systems, on Arm64, x64 and x86 | 14 | SDK 10.0.403 and runtime 10.0.14 (2026-10-25) | MIT |
| Roslyn (Microsoft.CodeAnalysis.CSharp) | the .NET SDK; also a NuGet library | 14 | 5.9.0 (2026-08-17) | MIT |
| Mono | several platforms; an implementation of .NET Framework | not stated | 6.12.0.206 (2024-02-13) | MIT |

Sources: .NET[^4][^29][^32], Roslyn[^31][^33] and Mono[^34]. The .NET column is the only implementation column of the compat tables of the feature pages.

.NET Framework is the original .NET for Windows. It is in maintenance, and its newest version, 4.8.1, came out on 2022-08-09.[^4][^35] Mono is the original community and open-source .NET.[^4] Its last major release was in July 2019 and its last patch release in February 2024. WineHQ is now the steward of the Mono project, and Microsoft keeps a fork of the Mono runtime in the .NET runtime repository.[^34]

## Ecosystem

NuGet is the package manager of .NET, and `dotnet build` also restores the NuGet packages of a project.[^4] The public gallery at nuget.org holds more than 100,000 unique packages.[^36] The test frameworks for .NET include MSTest, NUnit, TUnit and xUnit ([[unit-testing]]).[^37]

| Need | Tool |
|---|---|
| get libraries | NuGet and nuget.org[^36] |
| build a project | `dotnet build`[^38] |
| run tests | `dotnet test`[^39]; xUnit.net, a free, open source unit testing tool for C#, F# and Visual Basic[^40] |
| format | `dotnet format`, which formats code to match the EditorConfig settings[^41] |
| lint | the analyzers of the compiler; `-warnaserror` treats all warnings as errors[^42] |
| run one file | `dotnet run` on a file-based app[^9] |

The guide's Ecosystem part lists the main libraries by need, as library cards.

## History and influence

PLDB gives 2000 as the year C# appeared and names Anders Hejlsberg as its creator. Wikidata gives 2001 as the inception, and it names Microsoft, Hejlsberg and Mads Torgersen as designers.[^43][^44]

| Date | Event |
|---|---|
| July 2000 | the first widely distributed implementation, from Microsoft[^3] |
| December 2001 | ECMA-334, 1st edition[^2] |
| January 2002 | C# 1.0, with Visual Studio .NET 2002[^45] |
| 2003 | ISO/IEC 23270:2003, the ISO standard for C# 1[^2] |
| November 2005 | C# 2.0, with generics, iterators and nullable value types[^45] |
| November 2007 | C# 3.0, with lambda expressions and LINQ, for .NET Framework 3.5[^45] |
| August 2012 | C# 5.0, with `async` and `await`[^45] |
| July 2015 | C# 6.0, with Roslyn, a C# compiler written in C#[^45] |
| 2016-06-27 | .NET Core 1.0[^46] |
| September 2019 | C# 8.0, the first major release that targets .NET Core[^45] |
| November 2020 | C# 9, with .NET 5[^45] |
| December 2023 | ECMA-334, 7th edition, for C# 7[^2] |
| November 2025 | C# 14, with .NET 10[^45][^27] |

.NET is a project of the .NET Foundation. Microsoft and the community maintain it on GitHub.[^4] The committee Ecma TC49-TG2 works on the standard, and the language design has its own repository, csharplang.[^6] The C# 1.0 and 2.0 standards also have ISO/IEC editions. ECMA-334:2017 for C# 5 has the edition ISO/IEC 23270:2018, and ISO/IEC 20619:2023 includes the newest edition of ECMA-334 by reference.[^2]

The history page of Microsoft says that C# 1.0 looked a lot like Java and was an alternative to Java on Windows.[^45] The design goals of the standard name programmers who know C and C++.[^3] Wikidata lists C, C++ and Java among the languages that influenced C#.[^44]

## Coming from another language

- From [[java]]: generics are reified, so a program can ask at run time for the type argument of a `List<int>` ([[type-erasure]]).[^18] Exceptions are the main model of error handling, and error information does not need to be in method signatures ([[exception-handling]]).[^4] A `struct` is a value type that the program can define itself.[^16]
- From [[cpp]]: the core language has no pointers and uses a garbage collector. Pointers are only in unsafe code.[^22] A `using` statement disposes of a resource at the end of a block.[^17]
- From [[typescript]]: the syntax is in the same family.[^1] Nullable reference types give warnings at compile time, not a separate type.[^20] The `async` and `await` keywords work with tasks ([[async-await]]).[^13]
- From [[rust]]: a garbage collector manages memory ([[garbage-collection]]).[^21] A `switch` expression matches patterns, and from C# 8.0, nullable reference types mark the variables that can be null.[^11][^45]

## References
