```csharp run id=keep
using System;

static P Shift<P>(P p) where P : Point
{
    Point q = p;
    Point moved = q with { X = q.X + 1 };
    return (P)moved;
}

Point a = Shift(new Point(1, 2));
Labelled b = Shift(new Labelled(1, 2, "home"));
Console.WriteLine($"point: x={a.X} y={a.Y}");
Console.WriteLine($"labelled: x={b.X} y={b.Y} label={b.Label}");
Console.WriteLine($"run time type: {b.GetType().Name}");

record Point(int X, int Y);
record Labelled(int X, int Y, string Label) : Point(X, Y);
```

```output for=keep
point: x=2 y=2
labelled: x=2 y=2 label=home
run time type: Labelled
```

- Verdict: idiom. `Labelled` derives from `Point`, and `Shift` accepts any type derived from `Point`. The type parameter `P` keeps the full type in the result.
- A `with` expression copies the record with its run time type, so the copy of a `Labelled` is still a `Labelled` with its `Label`. The cast back to `P` is safe for that reason.
- Every type that `Shift` accepts must derive from `Point`. A row type would accept any record with the fields.
