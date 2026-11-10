```csharp run id=reified
static T Make<T>() where T : new() => new T();

static string Describe<T>(T value) =>
    $"{typeof(T).Name}, default {default(T)?.ToString() ?? "null"}";

static bool IsListOf<T>(object value) => value is List<T>;

Console.WriteLine(Describe(42));
Console.WriteLine(Describe("text"));
Console.WriteLine(Make<List<int>>().GetType());
object names = new List<string> { "Ada" };
Console.WriteLine(IsListOf<string>(names));
Console.WriteLine(IsListOf<int>(names));
```

```output for=reified
Int32, default 0
String, default null
System.Collections.Generic.List`1[System.Int32]
True
False
```

- Verdict: native. Type arguments are a part of the run-time type. A `List<string>` and a `List<int>` are different types.
- `typeof(T)` gives the type argument inside the generic method, and `default(T)` gives the default value of that type.
- The constraint `where T : new()` lets the body call `new T()`. The type argument must have a public constructor with no parameters.
- `value is List<T>` tests the type argument at run time. Java rejects the same test at compile time.
