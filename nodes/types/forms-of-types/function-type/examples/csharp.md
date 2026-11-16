```csharp run id=delegates
using System;
using System.Linq;

Func<int, int> inc = n => n + 1;
Func<Func<int, int>, int, int> twice = (f, x) => f(f(x));
Console.WriteLine(twice(inc, 5));

// A method group converts to a delegate type.
Func<string, int> parse = int.Parse;
Console.WriteLine(new[] { "1", "2", "3" }.Select(parse).Sum());

// Action is the delegate type for a function with no result.
Action<string> say = s => Console.WriteLine("say " + s);
say("hi");

// A lambda with var gets a Func or Action type.
var square = (int n) => n * n;
Console.WriteLine(square.GetType().Name + " " + square(7));

// A function returns a function.
Func<int, Func<int, int>> adder = a => b => a + b;
Console.WriteLine(adder(10)(5));
```

```output for=delegates
7
6
say hi
Func`2 49
15
```

- Verdict: native. `Func<T, TResult>` and `Action<T>` are delegate types from the library, and a lambda or a method group such as `int.Parse` converts to them. `twice` takes a delegate as a parameter, so its type is `Func<Func<int, int>, int, int>`. With `var`, the compiler gives the lambda a natural type, here ``Func`2``, which is `Func<int, int>`.
