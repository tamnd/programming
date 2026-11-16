```csharp run id=action-and-func
using System;

int n = Run.Timed("sum", () => 1 + 2);
Console.WriteLine(n);
Run.Timed("greet", () => Run.Greet("Ann"));

// The empty ValueTuple has one value, so it can act as a unit type.
Func<ValueTuple> unit = () =>
{
    Run.Greet("Bo");
    return ValueTuple.Create();
};
ValueTuple u = unit();
Console.WriteLine($"{u} {u.Equals(default(ValueTuple))} {typeof(void)}");

static class Run
{
    public static void Greet(string name) => Console.WriteLine($"hello {name}");

    public static T Timed<T>(string label, Func<T> body)
    {
        T result = body();
        Console.WriteLine($"{label} done");
        return result;
    }

    // A method without a result needs a second overload with Action.
    public static void Timed(string label, Action body)
    {
        body();
        Console.WriteLine($"{label} done");
    }
}
```

```output for=action-and-func
sum done
3
hello Ann
greet done
hello Bo
() True System.Void
```

- Verdict: stdlib. `void` marks a method without a result, but `Func<void>` does not exist, so the library has `Action` for delegates without a result and `Func` for delegates with one. `Timed` needs two overloads for this reason. The empty `ValueTuple` has one value, which prints as `()`, so code can return it from a `Func` when it needs a unit value. `typeof(void)` is `System.Void`.
