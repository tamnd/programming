```csharp run id=relational-patterns-and-guards
// Relational and logical patterns test ranges in one condition.
static string Range(int x)
{
    if (x is > 0 and < 10)
        return "one digit";
    else if (x is >= 10 and < 100)
        return "two digits";
    else
        return "other";
}
Console.WriteLine($"{Range(7)} {Range(42)} {Range(-3)}");

// A switch expression with a when guard.
static string Classify((int X, int Y) p) => p switch
{
    (0, 0) => "origin",
    (_, 0) => "on the x axis",
    var (x, y) when x == y => "on the diagonal",
    _ => "elsewhere",
};
Console.WriteLine($"{Classify((0, 0))} {Classify((3, 0))} {Classify((2, 2))}");

// ?. and ?? choose a value when something is null.
string? name = null;
Console.WriteLine(name?.ToUpper() ?? "anonymous");

// The condition must be a bool, so an int does not work as a condition.
int count = 3;
Console.WriteLine(count != 0 ? "non-zero" : "zero");
```

```output for=relational-patterns-and-guards
one digit two digits other
origin on the x axis on the diagonal
anonymous
non-zero
```

- Verdict: native. `x is > 0 and < 10` tests a range with relational patterns. A `switch` expression uses a `when` guard for the diagonal. `?.` and `??` choose `"anonymous"` when the name is `null`. A condition must be a `bool`, so the program writes `count != 0`.
