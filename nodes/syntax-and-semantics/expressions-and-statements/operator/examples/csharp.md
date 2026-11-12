```csharp run id=operator-declarations
var a = new Money(150);
var b = new Money(275);
Console.WriteLine(a + b);
Console.WriteLine(a < b);
Console.WriteLine(-a);

// A user-defined conversion operator.
decimal d = (decimal)b;
Console.WriteLine(d);

// ??= assigns only when the left side is null.
string? name = null;
name ??= "guest";
Console.WriteLine(name);

// is with a pattern, and the null-conditional operator ?.
object o = 42;
Console.WriteLine(o is int n and > 40 ? $"big {n}" : "other");
int[]? none = null;
Console.WriteLine(none?.Length ?? -1);

readonly record struct Money(long Cents)
{
    public static Money operator +(Money x, Money y) => new(x.Cents + y.Cents);
    public static Money operator -(Money x) => new(-x.Cents);
    public static bool operator <(Money x, Money y) => x.Cents < y.Cents;
    public static bool operator >(Money x, Money y) => x.Cents > y.Cents;
    public static explicit operator decimal(Money m) => m.Cents / 100m;
}
```

```output for=operator-declarations
Money { Cents = 425 }
True
Money { Cents = -150 }
2.75
guest
big 42
-1
```

- Verdict: native. A type declares `+`, unary `-`, `<` and `>` with the `operator` keyword, and C# requires `<` and `>` as a pair. An `explicit operator` gives a conversion to `decimal`. `??=` assigns only when the left side is `null`, and `?.` gives `null` when the array is `null`.
