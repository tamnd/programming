```csharp run id=switch-cs
using System;

static string Action(char command)
{
    switch (command)
    {
        case 'q':
        case 'x':
            return "quit";
        case 'h':
            return "help";
        default:
            return "unknown";
    }
}

static string Kind(int day) => day switch
{
    6 or 7 => "weekend",
    >= 1 and <= 5 => "weekday",
    _ => "not a day",
};

foreach (var c in "qhz")
{
    Console.WriteLine($"{c}: {Action(c)}");
}
foreach (var day in new[] { 1, 7, 9 })
{
    Console.WriteLine($"{day}: {Kind(day)}");
}
```

```output for=switch-cs
q: quit
h: help
z: unknown
1: weekday
7: weekend
9: not a day
```

- Verdict: native. A C# `switch` statement allows several labels on one section, but a section cannot fall through into the next one, so each section ends with `return` or `break`. `Kind` is a switch expression. Its arms use the patterns `6 or 7` and `>= 1 and <= 5`, and `_` takes each other value.
