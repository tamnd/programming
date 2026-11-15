```csharp run id=method-groups
using System;

var r = new Rectangle(3, 4);
Func<int> bound = r.Area;                 // a delegate with the receiver fixed
Func<Rectangle, int> unbound = x => x.Area();
r.Scale(2);
Console.WriteLine($"{r.Area()} {bound()} {unbound(r)}");
Console.WriteLine(bound.Target == r);
Console.WriteLine(Rectangle.AreaOf(6, 8));

class Rectangle
{
    private int width, height;

    public Rectangle(int width, int height)
    {
        this.width = width;
        this.height = height;
    }

    public int Area() => width * height;

    public void Scale(int factor)
    {
        width *= factor;
        height *= factor;
    }

    public static int AreaOf(int width, int height) => width * height;
}
```

```output for=method-groups
48 48 48
True
48
```

- Verdict: native. A method is declared in a class or a struct, and an instance method has the hidden receiver `this`. A `static` method has no receiver.
- A method group such as `r.Area` converts to a delegate. The delegate stores the receiver in its `Target` property, so `bound()` sees the change that `Scale` made. For the unbound form, C# code usually writes a lambda that takes the receiver.
