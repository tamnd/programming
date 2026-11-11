```csharp run id=ref
int[] data = { 1, 2, 3, 4, 5 };
for (int i = 0, j = data.Length - 1; i < j; i++, j--)
{
    (data[i], data[j]) = (data[j], data[i]);
}
Console.WriteLine(string.Join(",", data));

int total = 0;
AddTo(ref total, 5);
AddTo(ref total, 7);
Console.WriteLine(total);

int count = 0;
foreach (var ch in "imperative")
{
    if ("aeiou".Contains(ch)) count++;
}
Console.WriteLine(count);

static void AddTo(ref int target, int amount) => target += amount;
```

```output for=ref
5,4,3,2,1
12
5
```

- Verdict: native. C# has assignment, compound assignment, `for`, `foreach`, `while` and `do`, and tuple assignment swaps values.
- A `ref` parameter lets a method assign to a variable of its caller. The call site must also write `ref`, so the reader can see that the variable may change.
