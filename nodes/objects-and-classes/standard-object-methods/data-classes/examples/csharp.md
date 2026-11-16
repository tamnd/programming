```csharp run id=record-and-with
using System;
using System.Collections.Generic;

var a = new Version(1, 4);
Console.WriteLine(a);
Console.WriteLine(a == new Version(1, 4));
Console.WriteLine(ReferenceEquals(a, new Version(1, 4)));
var b = a with { Minor = 5 };
Console.WriteLine(b);
var seen = new HashSet<Version> { a, new Version(1, 4) };
Console.WriteLine(seen.Count);

var p = new Point(2, 3);
var (x, y) = p;
Console.WriteLine($"{p} {x + y}");

record Version(int Major, int Minor);

readonly record struct Point(int X, int Y);
```

```output for=record-and-with
Version { Major = 1, Minor = 4 }
True
False
Version { Major = 1, Minor = 5 }
1
Point { X = 2, Y = 3 } 5
```

- Verdict: native. A positional `record` gets init-only properties, a constructor, value equality with `==`, `GetHashCode`, `ToString` and a `Deconstruct` method.
- A `record` is a reference type, so `ReferenceEquals` tells the two objects apart, but `==` compares the values. A `record struct` is a value type.
- `with` makes a copy with some properties changed; the original does not change.
