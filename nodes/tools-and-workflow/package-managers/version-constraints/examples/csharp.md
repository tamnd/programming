```csharp run id=caret-and-tilde-ranges
// Turn caret, tilde and exact constraints into a range [low, high), then test versions against it.
static (Version Low, Version High) ToRange(string constraint)
{
    var op = constraint[0] is '^' or '~' ? constraint[..1] : "";
    var low = Version.Parse(constraint[op.Length..]);
    var high = op switch
    {
        "^" when low.Major > 0 => new Version(low.Major + 1, 0, 0), // keep the left-most part that is not zero
        "^" when low.Minor > 0 => new Version(0, low.Minor + 1, 0),
        "^" => new Version(0, 0, low.Build + 1),
        "~" => new Version(low.Major, low.Minor + 1, 0), // allow patch changes only
        _ => new Version(low.Major, low.Minor, low.Build + 1), // an exact version
    };
    return (low, high);
}

string[] candidates = ["0.0.4", "0.0.5", "0.2.3", "0.2.4", "0.2.9", "0.3.0",
    "1.2.2", "1.2.3", "1.2.9", "1.4.0", "2.0.0"];
Console.WriteLine("versions: " + string.Join(" ", candidates));
foreach (var constraint in new[] { "^1.2.3", "^0.2.4", "^0.0.4", "~1.2.3", "~0.2.4", "1.2.3" })
{
    var (low, high) = ToRange(constraint);
    var matches = candidates.Where(c => Version.Parse(c) >= low && Version.Parse(c) < high);
    Console.WriteLine($"{constraint} means >={low} <{high}, matches {string.Join(" ", matches)}");
}
```

```output
versions: 0.0.4 0.0.5 0.2.3 0.2.4 0.2.9 0.3.0 1.2.2 1.2.3 1.2.9 1.4.0 2.0.0
^1.2.3 means >=1.2.3 <2.0.0, matches 1.2.3 1.2.9 1.4.0
^0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
^0.0.4 means >=0.0.4 <0.0.5, matches 0.0.4
~1.2.3 means >=1.2.3 <1.3.0, matches 1.2.3 1.2.9
~0.2.4 means >=0.2.4 <0.3.0, matches 0.2.4 0.2.9
1.2.3 means >=1.2.3 <1.2.4, matches 1.2.3
```

- Verdict: idiom. The built-in class `System.Version` parses `1.2.3` and has the operators `<` and `>=`.
- `System.Version` calls the third part `Build`, not patch, and it has no prerelease tags. A SemVer library is needed for versions such as 1.2.3-beta.
- A switch expression with `when` clauses picks the upper bound.
