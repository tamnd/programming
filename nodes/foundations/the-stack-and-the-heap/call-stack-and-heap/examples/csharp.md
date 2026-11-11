```csharp run id=stackalloc-and-values
using System;

Span<int> buffer = stackalloc int[4]; // memory in the current frame
for (int i = 0; i < buffer.Length; i++) buffer[i] = i * 10;
Console.WriteLine(Sum(buffer));

var p = MakePoint(); // a class instance outlives the call that made it
var v = new Vec(1, 2); // a struct is a value; where it lives is up to the runtime
var w = v;
w.X = 9;
Console.WriteLine($"{p.X} {v.X} {w.X}");

int Down(int n) => n == 0 ? 0 : 1 + Down(n - 1);
Console.WriteLine(Down(10_000)); // ten thousand frames fit in the default stack

static int Sum(ReadOnlySpan<int> xs)
{
    int total = 0;
    foreach (var x in xs) total += x;
    return total;
}

static Point MakePoint() => new Point { X = 5 };

class Point
{
    public int X;
}

struct Vec(int x, int y)
{
    public int X = x;
    public int Y = y;
}
```

```output for=stackalloc-and-values
60
5 1 9
10000
```

```csharp fails expect=compile id=return-stack-span
using System;

Console.WriteLine(Make()[0]);

static Span<int> Make()
{
    Span<int> buffer = stackalloc int[4];
    buffer[0] = 42;
    return buffer; // the frame and its memory end here
}
```

- Verdict: native. The .NET runtime keeps a stack for each thread and a heap that the garbage collector manages. A class instance lives on the heap and outlives the method that made it. A struct is a value that copies on assignment, so changing `w` leaves `v` alone; where the runtime stores it, in a frame, in a register or inside another object, is an implementation detail.
- `stackalloc` takes memory from the current frame, and a `Span<int>` gives safe access to it. The compiler tracks the scope of each span, so the fails block, which returns a span over memory in a frame that is about to end, does not compile.
