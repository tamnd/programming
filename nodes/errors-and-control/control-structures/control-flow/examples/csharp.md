```csharp run id=goto-case-and-iterators
using System.Collections.Generic;

// C# forbids fallthrough between cases; goto case jumps to another case.
static string Describe(int n)
{
    switch (n)
    {
        case 0:
            return "zero";
        case 1:
            goto case 2;
        case 2:
            return "small";
        default:
            return "large";
    }
}
Console.WriteLine($"{Describe(0)} {Describe(1)} {Describe(9)}");

// An iterator runs lazily: its body runs only when the caller asks for the next value.
static IEnumerable<int> Numbers()
{
    Console.WriteLine("start");
    yield return 1;
    Console.WriteLine("after 1");
    yield return 2;
}
foreach (var n in Numbers())
{
    Console.WriteLine($"got {n}");
}

// finally runs when control leaves the try block by return.
static int WithFinally()
{
    try
    {
        return 1;
    }
    finally
    {
        Console.WriteLine("finally");
    }
}
Console.WriteLine(WithFinally());

// A switch expression and a conditional operator choose a value.
int t = 25;
Console.WriteLine(t switch { < 10 => "cold", < 25 => "mild", _ => "warm" });
Console.WriteLine(t % 2 == 0 ? "even" : "odd");
```

```output for=goto-case-and-iterators
zero small large
start
got 1
after 1
got 2
finally
1
warm
odd
```

- Verdict: native. C# forbids fallthrough between cases, so `case 1` uses `goto case 2` to share its code. The body of an iterator runs only when `foreach` asks for the next value, so the lines interleave. `finally` runs when `return` leaves the `try` block. A `switch` expression with relational patterns and the conditional operator choose values.
