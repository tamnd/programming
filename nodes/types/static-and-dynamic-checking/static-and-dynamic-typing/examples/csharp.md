```csharp run id=types
using Microsoft.CSharp.RuntimeBinder;

var count = 41;
object boxed = count;
Console.WriteLine(boxed.GetType().Name);
if (boxed is int n)
{
    Console.WriteLine(n + 1);
}

dynamic value = "abc";
Console.WriteLine(value.Length);
value = 42;
try
{
    Console.WriteLine(value.Length);
}
catch (RuntimeBinderException error)
{
    Console.WriteLine($"RuntimeBinderException: {error.Message}");
}
```

```output
Int32
42
3
RuntimeBinderException: 'int' does not contain a definition for 'Length'
```

- Verdict: native. C# is statically typed. Each variable has a compile-time type, and each object has a run-time type, which `GetType()` gives.
- `var` infers the compile-time type. `is` with a pattern tests the run-time type and binds a variable.
- An expression of type `dynamic` skips the static check. The runtime binds `value.Length` at each run, so the error for an `int` appears only then, as a `RuntimeBinderException`.
