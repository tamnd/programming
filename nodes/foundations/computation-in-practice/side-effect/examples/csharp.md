```csharp run id=deferred-query
int calls = 0;
int[] data = [1, 2, 3];
var doubled = data.Select(x => { calls++; return x * 2; }); // deferred: nothing runs yet
Console.WriteLine($"after Select: {calls} calls");
Console.WriteLine($"sum {doubled.Sum()}, then {calls} calls");
Console.WriteLine($"max {doubled.Max()}, then {calls} calls"); // the query runs again
var list = doubled.ToList(); // run once and keep the results
Console.WriteLine($"list [{string.Join(", ", list)}], then {calls} calls");

var p = new Point(1, 2);
var q = p with { X = 5 }; // a record makes a changed copy
Console.WriteLine($"{p} {q}");

record Point(int X, int Y);
```

```output for=deferred-query
after Select: 0 calls
sum 12, then 3 calls
max 6, then 6 calls
list [2, 4, 6], then 9 calls
Point { X = 1, Y = 2 } Point { X = 5, Y = 2 }
```

- Verdict: native. C# allows effects anywhere. A LINQ query is deferred: `Select` stores the lambda, and each enumeration runs it again, so `Sum`, `Max` and `ToList` each run the lambda three times. `ToList` or `ToArray` runs the query once and keeps the results. Records and the `with` expression make changed copies, and `readonly` fields and `init` properties prevent writes after construction. `[Pure]` documents a method as pure, and the compiler does not check it.
