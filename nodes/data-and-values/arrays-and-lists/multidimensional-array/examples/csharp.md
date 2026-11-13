```csharp run id=rectangular-and-jagged
using System;
using System.Linq;

class Program
{
    static void Main()
    {
        // int[,] is a true two-dimensional array: one object with a fixed number of rows and columns.
        int[,] m = { { 1, 2, 3 }, { 4, 5, 6 } };
        Console.WriteLine($"{m.Rank} {m.GetLength(0)} {m.GetLength(1)} {m.Length} {m[1, 2]}");

        // foreach visits the rightmost index first, which is row-major order.
        Console.WriteLine(string.Join(",", m.Cast<int>()));

        // int[][] is a jagged array: an array of separate row arrays.
        int[][] jagged = new int[3][];
        for (int i = 0; i < jagged.Length; i++) jagged[i] = new int[i + 1];
        jagged[2][2] = 7;
        Console.WriteLine($"{string.Join(" ", jagged.Select(r => r.Length))} {jagged[2][2]} {jagged.Length}");

        // A new jagged array has null rows until the program creates them.
        int[][] empty = new int[2][];
        Console.WriteLine(empty[0] is null);

        // A row of a jagged array is an array. A two-dimensional array has no row object.
        int[] row = jagged[1];
        row[0] = 9;
        Console.WriteLine($"{jagged[1][0]} {m.GetUpperBound(1)}");
    }
}
```

```output
2 2 3 6 6
1,2,3,4,5,6
1 2 3 7 3
True
9 2
```

- Verdict: native. int[,] is one rectangular array with Rank 2, GetLength for each dimension and Length for all items. Its items come out in row-major order. int[][] is a jagged array of separate rows, and a new jagged array has null rows. A row of a jagged array is an array that the program can hold, and a write through it changes the jagged array.
