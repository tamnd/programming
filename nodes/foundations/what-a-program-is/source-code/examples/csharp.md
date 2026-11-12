```csharp run id=caller-argument-text
using System.Runtime.CompilerServices;

int x = -2;
Check(x > 0);
Check(x * x == 4);
Check(x   <   0);

// The compiler puts the text of the argument and the line of the call into each call.
static void Check(bool ok,
    [CallerArgumentExpression(nameof(ok))] string text = "",
    [CallerLineNumber] int line = 0)
{
    Console.WriteLine($"line {line}: {text} is {(ok ? "true" : "false")}");
}
```

```output for=caller-argument-text
line 4: x > 0 is false
line 5: x * x == 4 is true
line 6: x   <   0 is true
```

- Verdict: native. The compiler fills the optional parameters at each call: `CallerArgumentExpression` gets the source text of the argument and `CallerLineNumber` gets the line. The text is exactly what the programmer wrote, spaces included, as the third call shows. Assertion libraries use this to print the failed condition without a macro.
