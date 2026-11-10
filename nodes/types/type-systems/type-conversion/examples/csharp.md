```csharp run timeout=30 id=conversions
int i = 300;
long wide = i;
byte low = (byte)i;
double d = -3.99;
Console.WriteLine($"{wide} {low} {(int)d}");
Console.WriteLine(int.Parse("42") + 1);
Console.WriteLine(int.TryParse("42abc", out int n) ? n.ToString() : "rejected");

object o = "text";
Console.WriteLine(o as string ?? "null");
Console.WriteLine(o as Uri == null);

try
{
    byte checkedLow = checked((byte)i);
    Console.WriteLine(checkedLow);
}
catch (OverflowException)
{
    Console.WriteLine("OverflowException");
}
```

```output for=conversions
300 44 -3
43
rejected
text
True
OverflowException
```

- Verdict: native. The implicit numeric conversions never lose magnitude. Every other numeric conversion needs a cast.
- In the default unchecked context, `(byte)300` keeps the low 8 bits and gives 44. In a `checked` context the same cast throws `OverflowException`.
- `as` returns `null` when a reference conversion fails, and a cast throws `InvalidCastException`.
- `int.Parse` throws on bad input. `int.TryParse` returns `false` instead.
