```csharp run id=overloading-csharp
using System;

class Program
{
    static string Show(int x)
    {
        return "int " + x;
    }

    static string Show(string s)
    {
        return "string " + s;
    }

    static string Show(int x, int y)
    {
        return "two ints " + x + " " + y;
    }

    static void Main()
    {
        Console.WriteLine(Show(42));
        Console.WriteLine(Show("hi"));
        Console.WriteLine(Show(3, 4));
    }
}
```

```output for=overloading-csharp
int 42
string hi
two ints 3 4
```

- Verdict: native. The three methods have the same name and different signatures. The compiler chooses one at each call.
