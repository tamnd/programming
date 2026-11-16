```csharp run id=operator-keyword
using System;
using System.Linq;

var a = new Money(150);
var b = new Money(250);
Console.WriteLine(a + b);
Console.WriteLine(a + b == new Money(400));
Console.WriteLine(a < b);

var total = new Money(0);
foreach (var m in new[] { a, b }) total += m;
Console.WriteLine(total);
Console.WriteLine(new[] { b, a }.Max());

readonly record struct Money(long Cents) : IComparable<Money>
{
    public static Money operator +(Money x, Money y) => new(x.Cents + y.Cents);

    // < and > must be defined as a pair.
    public static bool operator <(Money x, Money y) => x.Cents < y.Cents;
    public static bool operator >(Money x, Money y) => x.Cents > y.Cents;

    public int CompareTo(Money other) => Cents.CompareTo(other.Cents);
}
```

```output for=operator-keyword
Money { Cents = 400 }
True
True
Money { Cents = 400 }
Money { Cents = 250 }
```

- Verdict: native. An operator is a `public static` method declared with the `operator` keyword. At least one parameter has the type that declares it.
- The record struct generates `==` and `!=` from the fields. `<` and `>` must be declared together.
- With `+` defined, `total += m` works too. `Max` uses `IComparable<Money>`, not the `<` operator.
