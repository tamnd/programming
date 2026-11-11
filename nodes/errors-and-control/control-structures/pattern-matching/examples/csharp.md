```csharp run id=points
(int X, int Y)[] points = { (0, 0), (3, 0), (0, -2), (4, 4), (1, 5) };
foreach (var point in points)
{
    Console.WriteLine($"{point}: {Describe(point)}");
}

static string Describe((int X, int Y) point) => point switch
{
    (0, 0) => "origin",
    (var x, 0) => $"on the x axis at {x}",
    (0, var y) => $"on the y axis at {y}",
    var (x, y) when x == y => $"on the diagonal at {x}",
    var (x, y) => $"at {x}, {y}",
};
```

```output for=points
(0, 0): origin
(3, 0): on the x axis at 3
(0, -2): on the y axis at -2
(4, 4): on the diagonal at 4
(1, 5): at 1, 5
```

List patterns, from C# 11, take apart an array or a list. A slice pattern `..` matches the remaining elements, and `.. var rest` binds them.

```csharp run id=commands
string[][] commands =
{
    new[] { "go", "north" },
    new[] { "take", "lamp" },
    new[] { "take", "lamp", "key", "map" },
    new[] { "dance" },
    new string[0],
};
foreach (var command in commands)
{
    Console.WriteLine(Run(command));
}

static string Run(string[] command) => command switch
{
    [] => "nothing to do",
    ["go", var direction] => $"you go {direction}",
    ["take", var item] => $"you take the {item}",
    ["take", var first, .. var rest] => $"you take the {first} and {rest.Length} more",
    [var verb, ..] => $"unknown command: {verb}",
};
```

```output for=commands
you go north
you take the lamp
you take the lamp and 2 more
unknown command: dance
nothing to do
```

- Verdict: native. C# has declaration, type, constant, relational, logical (`and`, `or`, `not`), property, positional, `var`, discard and list patterns. They work in `is`, in a `switch` statement and in a `switch` expression.
- A `switch` expression that can miss a value gives a warning, and it throws `SwitchExpressionException` at run time when no arm matches.
- An arm that an earlier arm already covers is an error (CS8510).
