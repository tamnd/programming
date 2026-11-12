```csharp run id=top-level-statements
using System;

Console.WriteLine("Hello, world!");
Console.WriteLine($"{"world"} has {"world".Length} letters");
Console.WriteLine($"arguments: {args.Length}");
return 0;
```

```output for=top-level-statements
Hello, world!
world has 5 letters
arguments: 0
```

- Verdict: native. C# accepts top-level statements: one file in a project can hold statements outside any class, and the compiler wraps them in a generated `Main` method. `args` holds the arguments, and `return 0` sets the exit status. The classic form, a `static void Main(string[] args)` method in a class, still works and is common in existing code. `Console.WriteLine` writes a line, and interpolated strings with `$` format values.
