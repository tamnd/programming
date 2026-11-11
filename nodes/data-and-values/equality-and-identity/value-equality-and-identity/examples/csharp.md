```csharp run id=points
var a = new Point(1, 2);
var b = new Point(1, 2);
var c = a;
Console.WriteLine($"a == b: {a == b}");
Console.WriteLine($"ReferenceEquals(a, b): {ReferenceEquals(a, b)}");
Console.WriteLine($"ReferenceEquals(a, c): {ReferenceEquals(a, c)}");
Console.WriteLine($"distinct points: {new HashSet<Point> { a, b, c }.Count}");

var p = new Box(1, 2);
var q = new Box(1, 2);
Console.WriteLine($"Box p == q: {p == q}");
Console.WriteLine($"Box p.Equals(q): {p.Equals(q)}");

string s = "hello";
string t = new string("hello".ToCharArray());
Console.WriteLine($"s == t: {s == t}");
Console.WriteLine($"ReferenceEquals(s, t): {ReferenceEquals(s, t)}");

double nan = double.NaN;
Console.WriteLine($"nan == nan: {nan == nan}");
Console.WriteLine($"nan.Equals(nan): {nan.Equals(nan)}");

record Point(int X, int Y);

class Box
{
    public int X { get; }
    public int Y { get; }
    public Box(int x, int y) { X = x; Y = y; }
}
```

```output for=points
a == b: True
ReferenceEquals(a, b): False
ReferenceEquals(a, c): True
distinct points: 1
Box p == q: False
Box p.Equals(q): False
s == t: True
ReferenceEquals(s, t): False
nan == nan: False
nan.Equals(nan): True
```

- Verdict: native. A record gets value equality from its fields for `==`, `Equals` and `GetHashCode`. A plain class compares references with both `==` and `Equals`. `ReferenceEquals` always compares references.
- `string` overloads `==` to compare characters, with an ordinal, case-sensitive comparison.
- `double.Equals` treats `NaN` as equal to `NaN`. The `==` operator does not.

```csharp wrong id=boxed
object a = 42;
object b = 42;
Console.WriteLine(a == b);
Console.WriteLine(a.Equals(b));
```

```output for=boxed
False
True
```

Each assignment boxes `42` into a new object. `==` on two `object` operands compares references, so it is false. `Equals` is a virtual call, and `int` overrides it to compare values. The compiler picks `==` by the static types, so the same values give a different answer when they are typed as `object`.
