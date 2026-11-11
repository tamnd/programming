```csharp run id=switch
foreach (int code in new[] { 1, 2, 3, 4 })
{
    Console.WriteLine($"{code}: {Describe(code)}");
}

static string Describe(int code)
{
    string text = "";
    switch (code)
    {
        case 1:
            text += "minor ";
            goto case 2;
        case 2:
            text += "warning";
            break;
        case 3:
            text = "error";
            break;
        default:
            return "unknown";
    }
    return text;
}
```

```output for=switch
1: minor warning
2: warning
3: error
4: unknown
```

- Verdict: native. C# has `goto` to a label, `goto case` and `goto default`. A `switch` section cannot fall through to the next section, so the jump must be explicit.
- C# has no labeled `break`. To leave nested loops, use `return` from a method or a local function.
