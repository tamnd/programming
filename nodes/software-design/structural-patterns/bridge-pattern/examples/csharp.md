```csharp run id=shapes
using System;
using System.Collections.Generic;
using System.Linq;

foreach (IRenderer renderer in new IRenderer[] { new TextRenderer(), new SvgRenderer() })
{
    Console.WriteLine($"{renderer.Name}:");
    foreach (Shape shape in new Shape[] { new Circle(renderer, 1, 1, 2), new Square(renderer, 0, 0, 3) })
    {
        Console.WriteLine($"  {shape.Draw()}");
        shape.Scale(2);
        Console.WriteLine($"  {shape.Draw()}");
    }
}

Shape circle = new Circle(new TextRenderer(), 1, 1, 2);
circle.Renderer = new SvgRenderer(); // the implementation changes at run time
Console.WriteLine($"switched: {circle.Draw()}");

// The implementor: the primitive drawing operations of one output format.
interface IRenderer
{
    string Name { get; }
    string Circle(int x, int y, int radius);
    string Polygon(IEnumerable<(int X, int Y)> points);
}

sealed class TextRenderer : IRenderer
{
    public string Name => "text";
    public string Circle(int x, int y, int radius) => $"circle at ({x}, {y}) radius {radius}";
    public string Polygon(IEnumerable<(int X, int Y)> points) =>
        "polygon " + string.Join(" ", points.Select(p => $"({p.X}, {p.Y})"));
}

sealed class SvgRenderer : IRenderer
{
    public string Name => "svg";
    public string Circle(int x, int y, int radius) => $"<circle cx=\"{x}\" cy=\"{y}\" r=\"{radius}\"/>";
    public string Polygon(IEnumerable<(int X, int Y)> points) =>
        $"<polygon points=\"{string.Join(" ", points.Select(p => $"{p.X},{p.Y}"))}\"/>";
}

// The abstraction: it holds a renderer and draws itself with the primitives.
abstract class Shape(IRenderer renderer)
{
    public IRenderer Renderer { get; set; } = renderer;
    public abstract string Draw();
    public abstract void Scale(int factor);
}

sealed class Circle(IRenderer renderer, int x, int y, int radius) : Shape(renderer)
{
    private int radius = radius;
    public override string Draw() => Renderer.Circle(x, y, radius);
    public override void Scale(int factor) => radius *= factor;
}

sealed class Square(IRenderer renderer, int x, int y, int side) : Shape(renderer)
{
    private int side = side;
    public override string Draw() =>
        Renderer.Polygon([(x, y), (x + side, y), (x + side, y + side), (x, y + side)]);
    public override void Scale(int factor) => side *= factor;
}
```

```output for=shapes
text:
  circle at (1, 1) radius 2
  circle at (1, 1) radius 4
  polygon (0, 0) (3, 0) (3, 3) (0, 3)
  polygon (0, 0) (6, 0) (6, 6) (0, 6)
svg:
  <circle cx="1" cy="1" r="2"/>
  <circle cx="1" cy="1" r="4"/>
  <polygon points="0,0 3,0 3,3 0,3"/>
  <polygon points="0,0 6,0 6,6 0,6"/>
switched: <circle cx="1" cy="1" r="2"/>
```

- Verdict: idiom. An `interface` for the implementor and an `abstract class` with a primary constructor for the abstraction.
- The `Renderer` property has a setter, so a program can switch the implementation of one shape at run time.
- A value tuple `(int X, int Y)` stands in for a point type. The collection expression `[(x, y), ...]` builds the sequence for `Polygon`.
