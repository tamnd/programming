```csharp run id=interpolated-strings
using System;
using System.Globalization;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;
string name = "Ada";
double price = 4.5;
string[] items = { "tea", "milk" };

Console.WriteLine($"{name} pays {price * 2:F2} for {items.Length} items.");
Console.WriteLine($"[{name,6}] [{name,-6}]");
Console.WriteLine($"{{name}} is text, {{{name}}} is not.");
Console.WriteLine($$"""{"name": "{{name}}"}""");
FormattableString f = $"{price:F1} and {items.Length}";
Console.WriteLine($"{f.Format} {f.ArgumentCount}");
```

```output for=interpolated-strings
Ada pays 9.00 for 2 items.
[   Ada] [Ada   ]
{name} is text, {Ada} is not.
{"name": "Ada"}
{0:F1} and {1} 2
```

- Verdict: native. A string with the prefix `$` accepts any expression in braces, an alignment after a comma and a format after a colon. The conversion uses the current culture, so the program sets the invariant culture first. Two braces give one brace. In a raw string with `$$`, a placeholder needs two braces, so single braces are text, which helps for JSON. Assigned to `FormattableString`, the compiler keeps the composite format and the values apart.
