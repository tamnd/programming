```csharp run id=var
var count = 32;
var ratio = 2.5;
var names = new List<string> { "Ada", "Grace" };
var pairs = names.Select(n => (n, n.Length)).ToList();
var make = () => 1;

Console.WriteLine(count.GetType());
Console.WriteLine(ratio.GetType());
Console.WriteLine(names.GetType());
Console.WriteLine(pairs[1]);
Console.WriteLine(pairs[1].GetType());
Console.WriteLine(make.GetType());
```

```output for=var
System.Int32
System.Double
System.Collections.Generic.List`1[System.String]
(Grace, 5)
System.ValueTuple`2[System.String,System.Int32]
System.Func`1[System.Int32]
```

- Verdict: native. `var` takes the type of the initializer, and the variable is strongly typed.
- The lambda `() => 1` has a natural type, `Func<int>`. A lambda with an untyped parameter, such as `x => x + 1`, has none, and `var` rejects it.
- `var x;` and `var z = null;` do not compile, because the compiler has no type to use.
- The tuple `(n, n.Length)` gets the type `(string, int)` from its parts, which is `ValueTuple<string, int>` at run time.
