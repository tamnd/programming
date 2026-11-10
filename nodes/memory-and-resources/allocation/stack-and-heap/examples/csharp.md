```csharp run id=copies
using System;

var a = new PointValue(1, 2);
var b = a;
b.X = 10;
Console.WriteLine($"struct: {a.X} {b.X}");

var c = new PointObject(1, 2);
var d = c;
d.X = 10;
Console.WriteLine($"class: {c.X} {d.X}");

object boxed = a;
a.X = 99;
Console.WriteLine($"boxed copy: {((PointValue)boxed).X}");

Span<int> totals = stackalloc int[3];
for (var i = 0; i < totals.Length; i++)
{
    totals[i] = i * 10;
}
Console.WriteLine(totals[2]);

struct PointValue(int x, int y)
{
    public int X = x;
    public int Y = y;
}

class PointObject(int x, int y)
{
    public int X = x;
    public int Y = y;
}
```

```output
struct: 1 10
class: 10 10
boxed copy: 1
20
```

- Verdict: native. A variable of a `struct` type holds the value, and assignment copies it. A variable of a `class` type holds a reference, so `c` and `d` are one object.
- Converting a struct to `object` boxes it: the runtime allocates an object on the managed heap and copies the value into it. A later change to `a` does not reach the boxed copy.
- `stackalloc` puts the array in the frame, and it goes away when the method returns. A `Span<int>` reads it safely.
- A `StackOverflowException` cannot be caught. The process ends.
