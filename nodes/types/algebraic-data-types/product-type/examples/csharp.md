```csharp run id=parts
var t = (3, 4); // parts by position
var (x, y) = t;
Console.WriteLine($"tuple: {t} x = {x} y = {y} Item1 = {t.Item1}");

var p = new Point(3, 4); // parts by name
Console.WriteLine($"record: {p} x = {p.X} y = {p.Y}");
Console.WriteLine("same parts equal: " + Yn(p == new Point(3, 4)));
Console.WriteLine("swapped parts equal: " + Yn(p == new Point(4, 3)));
Console.WriteLine("tuples equal: " + Yn(t == (3, 4)));
Console.WriteLine($"unit: {new Unit()} equal to unit: {Yn(new Unit() == new Unit())}");

static string Yn(bool b) => b ? "yes" : "no";

record Point(int X, int Y);
record Unit;
```

```output for=parts
tuple: (3, 4) x = 3 y = 4 Item1 = 3
record: Point { X = 3, Y = 4 } x = 3 y = 4
same parts equal: yes
swapped parts equal: no
tuples equal: yes
unit: Unit { } equal to unit: yes
```

- Verdict: native. A tuple `(int, int)` is a product by position, and a `record` is a product by name.
- Tuples are value types, and `==` compares their elements. A record class is a reference type, but its `==` compares the members.
