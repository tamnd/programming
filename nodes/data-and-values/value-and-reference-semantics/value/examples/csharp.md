```csharp run id=struct-class-and-boxing
// A struct variable holds the value; a class variable holds a reference.
var p1 = new PointS(1, 2);
var p2 = p1;
p2.X = 10;
var c1 = new PointC { X = 1 };
var c2 = c1;
c2.X = 10;
Console.WriteLine($"{p1.X} {p2.X} {c1.X} {c2.X}");

// Boxing copies a value into an object; changes to the copy do not reach the box.
object box = p1;
p1.X = 5;
Console.WriteLine($"{((PointS)box).X} {p1.X}");

// Each type has a default value.
Console.WriteLine($"{default(int)} {default(bool)} {default(PointS).X} {default(string) ?? "null"}");

// A record compares by value; a class compares by reference by default.
Console.WriteLine($"{new Money(5, "EUR") == new Money(5, "EUR")} {new PointC { X = 1 } == new PointC { X = 1 }}");

// A lambda is a value of a delegate type.
Func<int, int> square = v => v * v;
Console.WriteLine(square(7));

struct PointS(int x, int y)
{
    public int X = x;
    public int Y = y;
}

class PointC
{
    public int X;
}

record Money(decimal Amount, string Currency);
```

```output for=struct-class-and-boxing
1 10 10 10
1 5
0 False 0 null
True False
49
```

- Verdict: native. A `struct` variable holds the value, so `p2 = p1` copies it, and a `class` variable holds a reference, so `c2 = c1` shares the object. Boxing copies a `struct` into an object, and a later change to the variable does not reach the box. `default` gives the default value of each type. Records compare by value and classes by reference.
