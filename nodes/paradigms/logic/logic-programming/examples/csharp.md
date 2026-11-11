```csharp run id=fixpoint
var parent = new[] { ("zeus", "ares"), ("ares", "harmonia"), ("harmonia", "semele"), ("semele", "dionysus") };

var ancestor = new HashSet<(string X, string Y)>(parent);
bool changed = true;
while (changed)
{
    var derived = (from p in parent
                   from a in ancestor
                   where p.Item2 == a.X
                   select (p.Item1, a.Y)).ToList();
    changed = false;
    foreach (var fact in derived)
    {
        changed |= ancestor.Add(fact);
    }
}
Console.WriteLine(ancestor.Count);
Console.WriteLine(string.Join(",", ancestor.Where(a => a.Y == "semele").Select(a => a.X).Order()));
```

```output for=fixpoint
10
ares,harmonia,zeus
```

- Verdict: absent. C# has no logic engine. The LINQ query is the join in the body of the recursive rule, and the loop runs it until the set of facts stops growing.
