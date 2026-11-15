```csharp run id=count-down-and-multiply-steps
// Count down from 5 to 1.
var down = new List<int>();
for (int i = 5; i >= 1; i--)
{
    down.Add(i);
}
Console.WriteLine(string.Join(" ", down));

// The update can be any expression: here each pass doubles p.
var powers = new List<int>();
for (int p = 1; p <= 100; p *= 2)
{
    powers.Add(p);
}
Console.WriteLine(string.Join(" ", powers));

// Several expressions in the initializer and iterator sections.
for (int i = 0, j = 3; i < j; i++, j--)
{
    Console.WriteLine($"{i} {j}");
}
```

```output for=count-down-and-multiply-steps
5 4 3 2 1
1 2 4 8 16 32 64
0 3
1 2
```

- Verdict: native. The `for` statement of C# has an initializer, a condition and an iterator section. The first loop counts down. The second loop doubles its counter, so the update is not always `++`. The third loop declares two counters and steps both in the iterator section.
