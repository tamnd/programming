```csharp fails expect=exception:IndexOutOfRangeException id=index
static void SetScore(int[] scores, int player, int score)
{
    scores[player] = score;
}

var scores = new int[4];
SetScore(scores, 2, 10);
Console.WriteLine($"player 2: {scores[2]}");
SetScore(scores, 4, 99);
```

```csharp run id=check
static bool SetScore(Span<int> scores, int player, int score)
{
    if ((uint)player >= (uint)scores.Length)
    {
        return false;
    }
    scores[player] = score;
    return true;
}

var scores = new int[4];
SetScore(scores, 2, 10);
Console.WriteLine($"player 2: {scores[2]}");
if (!SetScore(scores, 4, 99))
{
    Console.WriteLine($"refused: player 4 is out of range, the size is {scores.Length}");
}
```

```output for=check
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: native. The runtime checks each index into an array, and a bad index throws `IndexOutOfRangeException`. `Span<T>` carries its length and checks too.
- The cast to `uint` makes one comparison reject both a negative index and an index past the end.
- CWE-787 says that C# typically provides overflow protection, but that the programmer can turn it off. An `unsafe` block with pointers, `stackalloc` into a pointer and native code through P/Invoke have no check.
