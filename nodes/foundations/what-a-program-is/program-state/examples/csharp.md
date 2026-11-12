```csharp run id=iterator-state
IEnumerable<int> Evens()
{
    int n = 0; // the compiler moves this local into a hidden state object
    while (true)
    {
        yield return n;
        n += 2;
    }
}

var seq = Evens();
using var e1 = seq.GetEnumerator();
using var e2 = seq.GetEnumerator();
e1.MoveNext(); e1.MoveNext(); e1.MoveNext();
e2.MoveNext();
Console.WriteLine($"e1 at {e1.Current}, e2 at {e2.Current}"); // each enumerator has its own state
Console.WriteLine(string.Join(", ", seq.Take(4)));
```

```output for=iterator-state
e1 at 4, e2 at 0
0, 2, 4, 6
```

- Verdict: native. The compiler turns an iterator method into a class that implements the enumerator, with fields for the local `n` and for the position after the last `yield return`. Each call of `GetEnumerator` makes a new object with its own state, so `e1` and `e2` move independently, and `Take(4)` starts again from 0.
