```csharp run id=pair
var pair = MinMax(new[] { 7, 2, 9, 4 });
Console.WriteLine($"pair: {pair}");
var (lo, hi) = pair;
Console.WriteLine($"lo: {lo} hi: {hi}");
Console.WriteLine($"second part: {pair.Item2}");
Console.WriteLine("equal pairs: " + Yn(pair == (2, 9)));
Console.WriteLine($"by name: hi = {pair.Hi}");

static (int Lo, int Hi) MinMax(int[] values) => (values.Min(), values.Max());

static string Yn(bool b) => b ? "yes" : "no";
```

```output for=pair
pair: (2, 9)
lo: 2 hi: 9
second part: 9
equal pairs: yes
by name: hi = 9
```

- Verdict: native. A value tuple such as `(int Lo, int Hi)` is a value type with public fields.
- The names `Lo` and `Hi` are extra names for `Item1` and `Item2`.
- `==` compares the elements in order and ignores the names.
