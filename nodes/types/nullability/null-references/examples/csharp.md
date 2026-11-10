```csharp run id=nullable
#nullable enable

string? Find(string[] names, string prefix)
{
    foreach (var name in names)
    {
        if (name.StartsWith(prefix)) return name;
    }
    return null;
}

string[] names = ["Ada", "Alan", "Grace"];
string? hit = Find(names, "Gr");
string? miss = Find(names, "Z");

Console.WriteLine(hit?.Length);
Console.WriteLine(miss?.Length is null);
Console.WriteLine(miss ?? "none");

int? count = null;
Console.WriteLine(count.HasValue);
count ??= 3;
Console.WriteLine(count.Value);
```

```output
5
True
none
False
3
```

- Verdict: native. Every reference type includes `null`. Member access on `null` throws `NullReferenceException`.
- In a nullable context, `string` means "never null" and `string?` means "maybe null". The compiler tracks the null state of each variable and warns, for example with CS8602 "Dereference of a possibly null reference". The warnings do not change the program at run time.
- `?.` and `?[]` return `null` when the operand is `null`. `??` gives a default value, and `??=` assigns only when the variable is `null`.
- For a value type, `int?` is `Nullable<int>`, a struct with `HasValue` and `Value`. It is a different mechanism from `string?`, which is an annotation on a reference type.
