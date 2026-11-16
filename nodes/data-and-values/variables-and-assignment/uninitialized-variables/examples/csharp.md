```csharp run id=out-parameter
using System;

int value;
bool ok = int.TryParse("x", out value);
Console.WriteLine($"{ok} {value}");

string label;
if (DateTime.MinValue.Year == 1)
{
    label = "first year";
}
else
{
    label = "other";
}
Console.WriteLine(label);
```

```output for=out-parameter
False 0
first year
```

- Verdict: native. A local variable is initially unassigned, and the compiler rejects a read that some path reaches without an assignment. A method must assign each `out` parameter before it returns, so `value` has a value after the call, here 0 because the text is not a number. Both branches of the `if` assign `label`.
