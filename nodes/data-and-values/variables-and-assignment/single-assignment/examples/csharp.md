```csharp run id=with-expression
using System;

var p = new Point(1, 2);
var q = p with { X = 5 };
Console.WriteLine($"{p} {q}");

var total = 0;
total += 3;
Console.WriteLine(total);

record Point(int X, int Y);
```

```output for=with-expression
Point { X = 1, Y = 2 } Point { X = 5, Y = 2 }
3
```

- Verdict: idiom. C# local variables can always get a new value, except `const` for compile-time constants. Single assignment applies to data: a positional record has `init` properties, which get a value only when the object is made, and `with` makes a copy with some properties changed. `readonly` fields can be assigned only in a constructor.
