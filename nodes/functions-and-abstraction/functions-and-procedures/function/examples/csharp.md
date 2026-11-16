```csharp run id=function-csharp
using System;

static class Program
{
    static int RectangleArea(int width, int height)
    {
        return width * height;
    }

    static string Greeting(string name) => "Hello, " + name + "!";

    static void Main()
    {
        Console.WriteLine(RectangleArea(3, 4));
        Console.WriteLine(Greeting("Ada"));
    }
}
```

```output for=function-csharp
12
Hello, Ada!
```

- Verdict: native. Each function is a `static` method of a class. `Greeting` has an expression body after `=>`.
