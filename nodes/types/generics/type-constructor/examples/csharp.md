```csharp run id=unbound-closed
using System;
using System.Collections.Generic;

// typeof(List<>) is the unbound generic type; List<int> is a closed type.
Type unbound = typeof(List<>);
Type closed = typeof(List<int>);
Type map = typeof(Dictionary<,>);

Console.WriteLine($"{unbound.Name} {unbound.IsGenericTypeDefinition} {unbound.GetGenericArguments().Length}");
Console.WriteLine($"{map.Name} {map.GetGenericArguments().Length}");
Console.WriteLine($"{closed.Name} {closed.GetGenericArguments()[0].Name}");

// Apply the constructor to a type when the program runs.
Type made = unbound.MakeGenericType(typeof(string));
Console.WriteLine($"{made == typeof(List<string>)} {made == closed}");
```

```output for=unbound-closed
List`1 True 1
Dictionary`2 2
List`1 Int32
True False
```

- Verdict: native. `typeof(List<>)` names the unbound generic type, which is not a type for a variable. Its .NET name ends in the arity: ``List`1`` takes one type and ``Dictionary`2`` takes two. `MakeGenericType` applies the constructor to `string` when the program runs. .NET keeps the type arguments, so `List<string>` and `List<int>` are different types at run time.
