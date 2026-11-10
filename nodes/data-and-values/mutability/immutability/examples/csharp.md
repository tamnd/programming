```csharp run id=record
using System.Collections.Immutable;
using System.Collections.ObjectModel;

var p = new Point(1, 2);
var q = p with { Y = 5 };
Console.WriteLine($"{p} {q}");

var source = new List<int> { 1, 2 };
var view = new ReadOnlyCollection<int>(source);
var frozen = ImmutableList.CreateRange(source);
source.Add(3);
Console.WriteLine($"{view.Count} {frozen.Count}");

var more = frozen.Add(4);
Console.WriteLine($"{frozen.Count} {more.Count}");

record Point(int X, int Y);
```

```output for=record
Point { X = 1, Y = 2 } Point { X = 1, Y = 5 }
3 2
2 3
```

- Verdict: native. `readonly` fields, `init` accessors and records give immutability that the compiler checks.
- A positional record class has init-only properties. A `with` expression makes a copy with some properties changed.
- The immutability is shallow. A `readonly` field or an init-only property that holds a `List<T>` fixes only the reference.
- `ReadOnlyCollection<T>` is a wrapper. Changes to the wrapped list show through it, as the count 3 shows.
- `System.Collections.Immutable` has immutable collections. `Add` returns a fresh collection and leaves the original as it was.
