```csharp run id=strict
static T Trace<T>(string label, T value)
{
    Console.WriteLine($"evaluating {label}");
    return value;
}

static int Pick(bool flag, int left, int right)
{
    Console.WriteLine("inside pick");
    return flag ? left : right;
}

static int PickLazy(bool flag, Func<int> left, Func<int> right) => flag ? left() : right();

Console.WriteLine(Pick(Trace("flag", true), Trace("left", 1), Trace("right", 2)));
Console.WriteLine(PickLazy(true, () => Trace("left", 1), () => Trace("right", 2)));
```

```output for=strict
evaluating flag
evaluating left
evaluating right
inside pick
1
evaluating left
1
```

- Verdict: native. The C# specification states that the expressions of an argument list are always evaluated in textual order, before the function member runs.
- A `Func<T>` parameter is a thunk. `PickLazy` calls only the delegate that it needs.
