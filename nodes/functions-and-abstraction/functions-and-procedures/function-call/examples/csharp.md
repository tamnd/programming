```csharp run id=function-call-csharp
using System;

class Program
{
    static int RectangleArea(int width, int height)
    {
        Console.WriteLine("computing " + width + " by " + height);
        return width * height;
    }

    static void Main()
    {
        int area = RectangleArea(3, 4);
        Console.WriteLine("area " + area);
    }
}
```

```output for=function-call-csharp
computing 3 by 4
area 12
```

- Verdict: native. A call of a static method writes the arguments in parentheses after the method name.
