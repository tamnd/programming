```csharp run id=override-object-members
using System;
using System.Collections.Generic;

var a = new Money(1250, "EUR");
var b = new Money(1250, "EUR");
Console.WriteLine($"{a.Equals(b)} {a == b} {ReferenceEquals(a, b)}");
var names = new Dictionary<Money, string> { [a] = "lunch" };
Console.WriteLine($"{names[b]} {a}");
Console.WriteLine(new Plain());

sealed class Plain { }

sealed class Money : IEquatable<Money>
{
    public long Cents { get; }
    public string Currency { get; }

    public Money(long cents, string currency)
    {
        Cents = cents;
        Currency = currency;
    }

    public bool Equals(Money? other) => other is not null && Cents == other.Cents && Currency == other.Currency;

    public override bool Equals(object? obj) => Equals(obj as Money);

    public override int GetHashCode() => HashCode.Combine(Cents, Currency);

    public override string ToString() => $"{Cents / 100}.{Cents % 100:D2} {Currency}";

    public static bool operator ==(Money? left, Money? right) => left is null ? right is null : left.Equals(right);

    public static bool operator !=(Money? left, Money? right) => !(left == right);
}
```

```output for=override-object-members
True True False
lunch 12.50 EUR
Plain
```

- Verdict: native. `System.Object` declares `Equals`, `GetHashCode` and `ToString`, and every type inherits them.
- `IEquatable<Money>` adds an `Equals` without a cast, and the override of `Equals(object)` calls it. The `==` operator does not call `Equals` by itself, so the class defines `==` and `!=` too.
- `HashCode.Combine` mixes the fields that `Equals` compares. The default `ToString` returns the name of the type, as `Plain` shows.
