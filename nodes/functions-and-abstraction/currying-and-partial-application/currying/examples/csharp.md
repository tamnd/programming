```csharp run id=functions
Func<int, Func<int, int>> add = a => b => a + b;
var addTen = add(10);
Console.WriteLine(addTen(5));

static Func<B, R> Partial<A, B, R>(Func<A, B, R> f, A a) => b => f(a, b);

Func<string, int, string> pad = (s, width) => s.PadLeft(width, '.');
var padAbc = Partial(pad, "abc");
Console.WriteLine(padAbc(6));
```

```output for=functions
15
...abc
```

- Verdict: idiom. Lambdas and the `Func` delegate types can express curried functions and partial application. A helper such as `Partial` is a one-line generic method.
- The standard library has no partial application helper.
