```csharp run id=virtual-overload
using System;
using System.Collections.Generic;

Shape[] shapes = { new Square(2), new Circle(1) };
foreach (var s in shapes)
{
    Console.WriteLine($"{s.GetType().Name} {s.Area():F2}");
}

Console.WriteLine($"{Poly.Show(1)} {Poly.Show("a")} {Poly.Show(2.5)}");
Console.WriteLine($"{Poly.First(new List<int> { 3, 4 })} {Poly.First(new List<string> { "a", "b" })}");

// Overloads must be methods of a type; local functions cannot be overloaded.
static class Poly
{
    public static string Show(int x) => $"int {x}";
    public static string Show(string x) => $"string {x}";
    public static string Show(object x) => "object";

    public static T First<T>(List<T> items) => items[0];
}

abstract class Shape
{
    public abstract double Area();
}

class Square(double side) : Shape
{
    public override double Area() => side * side;
}

class Circle(double r) : Shape
{
    public override double Area() => Math.PI * r * r;
}
```

```output for=virtual-overload
Square 4.00
Circle 3.14
int 1 string a object
3 a
```

- Verdict: native. `Area` is abstract in `Shape`, and each class overrides it, so the run-time type of the object picks the code. `Show` has three overloads. `Show(2.5)` calls the `object` version, because no overload takes a `double`. `First<T>` is a generic method.
