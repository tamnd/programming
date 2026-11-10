```csharp run id=constants
using System;

class Program
{
    const int Rows = 3;
    const int Cols = 4;
    const int Cells = Rows * Cols;
    const string Name = "grid";
    const string Label = $"{Name} cells";
    static readonly long Started = DateTime.Now.Ticks;

    static string Kind(int n) => n switch
    {
        Cells => "full",
        0 => "empty",
        _ => "partial",
    };

    static void Main()
    {
        Console.WriteLine($"{Label}: {Cells}");
        Console.WriteLine($"{Kind(12)} {Kind(0)} {Kind(5)}");
        Console.WriteLine($"readonly set at run time: {Started > 0}");
    }
}
```

```output for=constants
grid cells: 12
full empty partial
readonly set at run time: True
```

`const` values are computed by the compiler, so `Cells` can be a pattern in a `switch`. An interpolated string whose parts are constant strings is also a constant. A value that needs a call at run time, such as the time, must be a `static readonly` field.

```csharp fails expect=compile id=overflow
using System;

class Program
{
    const int Big = int.MaxValue + 1;

    static void Main()
    {
        Console.WriteLine(Big);
    }
}
```

The compiler evaluates constant expressions in a checked context. `int.MaxValue + 1` overflows, so the program does not compile. The same addition on variables wraps at run time in the default unchecked context.

```csharp fails expect=compile id=call
using System;

class Program
{
    const int Larger = Math.Max(10, 20);

    static void Main()
    {
        Console.WriteLine(Larger);
    }
}
```

`Math.Max` is a method call, and the compiler does not run methods to compute a constant. The program does not compile.

- Verdict: native. A `const` holds a number, a Boolean value, a string or `null`, and the compiler evaluates its initializer. The language has no compile-time functions and no static assertion.
- Other assemblies copy the value of a public `const` when they compile. If the value changes, those assemblies must be compiled again to see it.
