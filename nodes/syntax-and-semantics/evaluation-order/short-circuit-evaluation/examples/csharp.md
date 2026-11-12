```csharp run id=null-conditional-operators
int calls = 0;
bool Check(bool v)
{
    calls++;
    return v;
}

// && and || skip the right operand; & and | on bool evaluate both.
bool r1 = Check(false) && Check(true);
Console.WriteLine($"{r1} calls={calls}");
calls = 0;
bool r2 = Check(false) & Check(true);
Console.WriteLine($"{r2} calls={calls}");

// ?. stops at null, and ?? gives a value for null.
string? name = null;
Console.WriteLine(name?.Length ?? -1);
Console.WriteLine(name?.Trim().ToUpper() ?? "anonymous");

// ??= assigns only when the variable is null, so the right side runs once.
List<int>? cache = null;
cache ??= Load();
cache ??= Load();
Console.WriteLine($"{cache.Count} items, loads={loads}");

// A throw expression on the right side of ?? runs only for null.
string Require(string? s) => s ?? throw new ArgumentNullException(nameof(s));
Console.WriteLine(Require("ok"));

partial class Program
{
    static int loads = 0;
    static List<int> Load()
    {
        loads++;
        return [1, 2, 3];
    }
}
```

```output for=null-conditional-operators
False calls=1
False calls=2
-1
anonymous
3 items, loads=1
ok
```

- Verdict: native. `&&` and `||` short-circuit, and `&` and `|` on `bool` evaluate both operands. `?.` stops at null, and `??` supplies a value for null. `??=` calls `Load` only when the variable is null, so it runs once. A `throw` expression on the right of `??` runs only for a null argument.
