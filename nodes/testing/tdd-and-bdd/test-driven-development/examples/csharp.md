```csharp run id=leap
static bool IsLeap(int year) =>
    year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);

// The tests in the order of the to-do list. Each one added one clause.
(string Name, Func<bool> Check)[] tests =
[
    ("2024 is a leap year", () => IsLeap(2024)),
    ("2023 is a common year", () => !IsLeap(2023)),
    ("1900 is a common year", () => !IsLeap(1900)),
    ("2000 is a leap year", () => IsLeap(2000)),
];

var failed = 0;
foreach (var (name, check) in tests)
{
    var ok = check();
    if (!ok) failed++;
    Console.WriteLine($"{(ok ? "pass" : "FAIL")}  {name}");
}
Console.WriteLine(failed == 0 ? "green" : "red");
```

```output
pass  2024 is a leap year
pass  2023 is a common year
pass  1900 is a common year
pass  2000 is a leap year
green
```

- Verdict: library. The usual runners, xUnit.net, NUnit and MSTest, are packages from NuGet, and `dotnet test` runs them. This example uses an array of named checks in a top-level program, so that it runs with the standard library only.
- Each test is a tuple of a name and a `Func<bool>`. The lambda delays the check until the loop calls it.
- A test framework adds what this runner lacks: it catches exceptions per test, it reports the values that differed, and it finds tests by attribute, such as `[Fact]` in xUnit.net.

The first red in C# is a compile error, because `IsLeap` does not exist yet.

```csharp fails expect=compile id=red
// Red: the test comes first, so IsLeap does not exist yet.
if (!IsLeap(2024))
{
    throw new Exception("2024");
}
```

- The compiler reports error CS0103: the name `IsLeap` does not exist in the current context. A stub such as `static bool IsLeap(int year) => false;` lets the program compile, and then the check fails.
