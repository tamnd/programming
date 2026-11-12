```csharp run id=the-dynamic-type
using System;
using Microsoft.CSharp.RuntimeBinder;

static dynamic Twice(dynamic x) => x + x; // resolved when it runs

Console.WriteLine(Twice(21));
Console.WriteLine(Twice("ab"));
Console.WriteLine(Twice(1.5));

dynamic d = "text";
int typed = 5;
Console.WriteLine(d.Length + typed); // the binder finds Length on string

try
{
    Console.WriteLine(d.Count); // string has no Count property
}
catch (RuntimeBinderException e)
{
    Console.WriteLine("RuntimeBinderException: " + e.Message);
}

try
{
    int n = d; // a conversion from dynamic is checked when it runs
    Console.WriteLine(n);
}
catch (RuntimeBinderException)
{
    Console.WriteLine("cannot convert the string to int");
}
```

```output for=the-dynamic-type
42
abab
3
9
RuntimeBinderException: 'string' does not contain a definition for 'Count'
cannot convert the string to int
```

- Verdict: native. C# has the static type `dynamic`, added in C# 4.0: the compiler skips checks for a `dynamic` value and records each operation, and the run-time binder resolves the operation with the actual type. `Twice` works on `int`, `string` and `double`, a missing member throws `RuntimeBinderException`, and assigning a `dynamic` value to an `int` variable checks the conversion when it runs. Code that does not use `dynamic` keeps all of its static checks.
