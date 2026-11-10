```csharp run id=shapes
Shape[] shapes = { new Circle(1), new Rect(2, 3), new Triangle(4, 5) };
foreach (Shape shape in shapes)
{
    Console.WriteLine($"{Area(shape):F2}");
}
Console.WriteLine(shapes[1]);

static double Area(Shape shape) => shape switch
{
    Circle(var r) => Math.PI * r * r,
    Rect(var w, var h) => w * h,
    Triangle(var b, var h) => b * h / 2,
    _ => throw new ArgumentException($"unknown shape {shape}"),
};

abstract record Shape;
sealed record Circle(double Radius) : Shape;
sealed record Rect(double Width, double Height) : Shape;
sealed record Triangle(double Base, double Height) : Shape;
```

```output for=shapes
3.14
6.00
10.00
Rect { Width = 2, Height = 3 }
```

- Verdict: idiom. An abstract record with one sealed record for each case plays the role of a sum type. A `switch` expression with positional patterns takes each case apart.
- Any assembly can derive another class from `Shape`, so the compiler cannot treat the three cases as complete. Without the discard arm `_`, it gives a warning, and a value that matches no arm throws `SwitchExpressionException` (see Pitfalls on the concept page).
- The concept page describes the `union` types and `closed` classes of C# 15, which the machine that recorded the examples does not have.
