```csharp run id=tuples
using System;

int a, b;
a = b = 3;
(a, b) = (1, 2);
(a, b) = (b, a);
Console.WriteLine($"{a} {b}");
var point = (X: 4, Y: 5);
point.X += 10;
Console.WriteLine(point);
```

```output for=tuples
2 1
(14, 5)
```

- Verdict: native. `=` is an expression, as in C, so it chains. A tuple on the left takes the parts of a tuple on the right, and `(a, b) = (b, a)` swaps two variables. `+=` is a compound assignment.
