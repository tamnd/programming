```csharp run id=hierarchy
Shape[] shapes = { new Square(2), new Circle(1) };
foreach (Shape shape in shapes)
{
    Console.WriteLine($"{shape.GetType().Name} {shape.Area():F2}");
}

IEnumerable<string> names = new List<string> { "Ada", "Grace" };
IEnumerable<object> items = names;
Console.WriteLine(string.Join(", ", items));

Action<object> print = value => Console.WriteLine($"got {value}");
Action<string> printName = print;
printName("Ada");

object any = new Square(3);
if (any is Square square)
{
    Console.WriteLine(square.Side);
}

abstract class Shape
{
    public abstract double Area();
}

class Square(double side) : Shape
{
    public double Side => side;
    public override double Area() => side * side;
}

class Circle(double radius) : Shape
{
    public override double Area() => Math.PI * radius * radius;
}
```

```output for=hierarchy
Square 4.00
Circle 3.14
Ada, Grace
got Ada
3
```

- Verdict: native. Subtyping is nominal, through base classes and interfaces. A method must be `virtual` or `abstract` to be overridden.
- `IEnumerable<out T>` is covariant, so a sequence of strings is a sequence of objects. `Action<in T>` is contravariant, so an action that takes any object can act on strings.
- Variance of generic types applies only to interfaces and delegates, and only to reference types.
- Arrays of reference types are covariant, with a check on each store. The concept page shows the `ArrayTypeMismatchException`.
