```csharp run id=window-shape
using System;

interface ITitled
{
    string Title { get; }
}

interface IShape
{
    int Area();
    string Describe(ITitled owner);
}

record Rectangle(int Width, int Height) : IShape
{
    public int Area() => Width * Height;
    public string Describe(ITitled owner) => $"{owner.Title}: a rectangle of {Width} by {Height}";
}

record Triangle(int Base, int Height) : IShape
{
    public int Area() => Base * Height / 2;
    public string Describe(ITitled owner) => $"{owner.Title}: a triangle of {Base} by {Height}";
}

class AppWindow : ITitled
{
    public string Title { get; }
    public IShape Shape { get; set; } // the delegate

    public AppWindow(string title, IShape shape)
    {
        Title = title;
        Shape = shape;
    }

    public int Area() => Shape.Area(); // forward
    public string Describe() => Shape.Describe(this); // delegate, and pass the receiver
}

class Program
{
    static void Main()
    {
        var w = new AppWindow("main", new Rectangle(3, 4));
        Console.WriteLine($"area: {w.Area()}");
        Console.WriteLine(w.Describe());
        w.Shape = new Triangle(3, 4); // change the behaviour at run time
        Console.WriteLine($"area: {w.Area()}");
        Console.WriteLine(w.Describe());
    }
}
```

```output
area: 12
main: a rectangle of 3 by 4
area: 6
main: a triangle of 3 by 4
```

- Verdict: idiom. The window holds the delegate in a property of an interface type, and each forwarded method is one expression.
