```csharp run id=deconstruct
using System;

var (q, r) = DivRem(17, 5);
Console.WriteLine($"{q} {r}");

var p = new Point(3, 4);
var (x, _) = p;
Console.WriteLine(x);

var (value, unit) = new Temperature(21, "C");
Console.WriteLine($"{value} {unit}");

int a = 1, b = 2;
(a, b) = (b, a);
Console.WriteLine($"{a} {b}");

static (int, int) DivRem(int n, int d) => (n / d, n % d);

record Point(int X, int Y);

class Temperature(int degrees, string unit)
{
    public void Deconstruct(out int value, out string scale)
    {
        value = degrees;
        scale = unit;
    }
}
```

```output for=deconstruct
3 2
3
21 C
2 1
```

- Verdict: native. A tuple is deconstructed into new variables or into existing ones. A positional record gets a `Deconstruct` method from the compiler, and any class can define one. The discard `_` drops a part.
