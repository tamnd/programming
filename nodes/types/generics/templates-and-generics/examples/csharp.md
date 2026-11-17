```csharp run id=reified
using System;
using System.Collections.Generic;

Console.WriteLine(Info.Describe<int>());
Console.WriteLine(Info.Describe<string>());
Console.WriteLine(new List<int>().GetType() == new List<string>().GetType());

static class Info
{
    public static string Describe<T>() =>
        $"{typeof(T).Name} {typeof(T).IsValueType} {default(T)?.ToString() ?? "null"}";
}
```

```output for=reified
Int32 True 0
String False null
False
```

- Verdict: native. The runtime knows the type argument, so `typeof(T)` and `default(T)` work inside the generic method. `List<int>` and `List<string>` are two types when the program runs.
