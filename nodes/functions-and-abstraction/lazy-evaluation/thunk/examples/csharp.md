```csharp run id=thunks
static int Load()
{
    Console.WriteLine("loading");
    return 21;
}

Func<int> plain = Load;
Console.WriteLine(plain() + plain());

var memo = new Lazy<int>(Load);
Console.WriteLine(memo.IsValueCreated);
Console.WriteLine(memo.Value + memo.Value);
Console.WriteLine(memo.IsValueCreated);
```

```output for=thunks
loading
loading
42
False
loading
42
True
```

- Verdict: stdlib. A `Func<T>` delegate is a plain thunk. `Lazy<T>` is a memoized thunk: `Value` runs the factory on the first access, and `IsValueCreated` tells if the force has happened. By default it is safe for threads.
