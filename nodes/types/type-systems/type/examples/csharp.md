```csharp run id=value-and-reference-types
// A struct is a value type: assignment copies the value.
var a = new Point(1, 2);
var b = a;
b.X = 10;
Console.WriteLine($"{a.X} {b.X}");

// A class is a reference type: assignment copies the reference.
var c = new Box { Value = 1 };
var d = c;
d.Value = 10;
Console.WriteLine($"{c.Value} {d.Value}");

// typeof gives the type at compile time, and GetType gives the run-time type.
object o = 42;
Console.WriteLine($"{typeof(int)} {o.GetType()} {o.GetType() == typeof(int)} {typeof(Point).IsValueType} {typeof(Box).IsValueType}");

// int is an alias of System.Int32, and each type has a default value.
Console.WriteLine($"{typeof(int) == typeof(System.Int32)} {default(int)} {default(bool)} {default(string) ?? "null"}");

// A type pattern tests the run-time type.
foreach (object v in new object[] { 1, "two", 3.0, new Point(4, 5) })
{
    string s = v switch
    {
        int n => $"int {n}",
        string t => $"string of length {t.Length}",
        Point p => $"Point {p.X},{p.Y}",
        _ => $"other {v.GetType().Name}",
    };
    Console.WriteLine(s);
}

struct Point(int x, int y)
{
    public int X = x;
    public int Y = y;
}

class Box
{
    public int Value;
}
```

```output for=value-and-reference-types
1 10
10 10
System.Int32 System.Int32 True True False
True 0 False null
int 1
string of length 3
other Double
Point 4,5
```

- Verdict: native. A `struct` is a value type, so `b = a` copies the point, and a `class` is a reference type, so `d = c` copies the reference. `typeof(int)` and `o.GetType()` give the same `System.Int32`. `int` is an alias of `System.Int32`, and `default` gives the zero value of each type. A `switch` expression with type patterns tests the run-time type.
