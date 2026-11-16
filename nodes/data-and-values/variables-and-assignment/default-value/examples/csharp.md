```csharp run id=default-keyword
using System;

Console.WriteLine($"{Zero<int>()} {Zero<string>() == null} {Zero<bool>()}");
int? maybe = default;
Console.WriteLine(maybe.HasValue);
var p = default(Point);
Console.WriteLine($"{p.X} {p.Y}");
Console.WriteLine($"{Label()} {Label("Ada")}");

static T? Zero<T>() => default;

static string Label(string name = "none") => name;

record struct Point(int X, int Y);
```

```output for=default-keyword
0 True False
False
0 0
none Ada
```

- Verdict: native. `default` gives the default value of a type: 0, `false`, `null`, or a struct with each field set to its default. For a nullable value type, the default has no value. A parameter can have a default value, which the compiler puts into each call that leaves it out.
