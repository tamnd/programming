```csharp run id=linq-zip
using System;
using System.Linq;

int[] a = { 3, 1, 4, 1, 5 };
int[] b = { 2, 7, 1, 8, 2 };

int[] sum = a.Zip(b, (x, y) => x + y).ToArray();      // element-wise
int dot = a.Zip(b, (x, y) => x * y).Sum();            // reduction
int[] big = a.Where(x => x > 2).ToArray();            // compress by a test
int[][] table = Enumerable.Range(1, 3)
    .Select(x => Enumerable.Range(1, 3).Select(y => x * y).ToArray())
    .ToArray();                                       // outer product

Console.WriteLine($"{string.Join(" ", sum)} | {dot} | {string.Join(" ", big)}");
foreach (int[] row in table) Console.WriteLine(string.Join(" ", row));
```

```output for=linq-zip
5 8 5 9 7 | 35 | 3 4 5
1 2 3
2 4 6
3 6 9
```

- Verdict: idiom. LINQ's `Zip`, `Select`, `Where` and `Sum` give element-wise operations, compression and reductions over any sequence. For numeric work, `System.Numerics.Vector<T>` gives SIMD operations whose width matches the processor, and the System.Numerics.Tensors package gives operations on whole spans of numbers.
