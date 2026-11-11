```csharp run id=shapes
using System;

Shape[] shapes = { new Circle(1), new Square(2) };
foreach (Shape s in shapes)
    Console.WriteLine(s.Describe());

// dynamic picks the overload at run time from the types of both arguments
foreach (Shape a in shapes)
    foreach (Shape b in shapes)
        Console.WriteLine(Geometry.Meet((dynamic)a, (dynamic)b));

abstract class Shape
{
    public abstract double Area();
    public virtual string Name() => "shape";
    public string Describe() => $"{Name()} with area {Area():F2}";
}

sealed class Circle(double r) : Shape
{
    public override double Area() => Math.PI * r * r;
    public override string Name() => "circle";
}

sealed class Square(double side) : Shape
{
    public override double Area() => side * side;
}

static class Geometry
{
    public static string Meet(Shape a, Shape b) => "two shapes";
    public static string Meet(Circle a, Circle b) => "two circles";
    public static string Meet(Circle a, Square b) => "a circle and a square";
}
```

```output for=shapes
circle with area 3.14
shape with area 4.00
two circles
a circle and a square
two shapes
two shapes
```

```csharp wrong id=hiding
using System;

Square q = new Square();
Shape s = q;
Console.WriteLine(q.Name());
Console.WriteLine(s.Name()); // the same object

class Shape
{
    public string Name() => "shape"; // not virtual
}

class Square : Shape
{
    public new string Name() => "square"; // hides Shape.Name; overrides nothing
}
```

```output for=hiding
square
shape
```

- Verdict: native. A C# method dispatches at run time only when the base declares it `virtual` or `abstract` and the derived class declares it `override`. Methods are static by default, and interface methods always dispatch. The cast to `dynamic` moves overload resolution to run time, so `Meet` chooses its overload from the types of both arguments, which gives multiple dispatch.
- The wrong block hides a method instead of overriding it. `Shape.Name` is not virtual, so `new` in `Square` declares a second method with the same name, and the static type of the reference chooses between them. Declare the base method `virtual` and the new one `override`.
