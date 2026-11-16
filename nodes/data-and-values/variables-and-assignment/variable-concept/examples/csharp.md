```csharp run id=value-and-reference
using System;

var p = new Point { X = 1 };
var q = p;
q.X = 2;
Console.WriteLine($"{p.X} {q.X}");

var r = new Box { X = 1 };
var s = r;
s.X = 2;
Console.WriteLine($"{r.X} {s.X}");

struct Point { public int X; }

class Box { public int X; }
```

```output for=value-and-reference
1 2
2 2
```

- Verdict: native. A C# variable is a storage location with a type. A variable of a struct type holds the value, so `q = p` copies the struct, and a change to `q` does not change `p`. A variable of a class type holds a reference, so `s = r` makes two names for one object.
