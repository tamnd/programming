// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

using System;
using System.Collections.Generic;

/// <summary>An amount in cents and a currency code. Sealed, so no subclass can break Equals.</summary>
public sealed class Money : IEquatable<Money>
{
    public long Cents { get; }

    public string Currency { get; }

    public Money(long cents, string currency)
    {
        Cents = cents;
        Currency = currency.ToUpperInvariant(); // "eur" and "EUR" are the same currency
    }

    public bool Equals(Money? other) =>
        other is not null && Cents == other.Cents && Currency == other.Currency;

    public override bool Equals(object? obj) => Equals(obj as Money);

    public override int GetHashCode() => HashCode.Combine(Cents, Currency); // the same fields as Equals

    public override string ToString() => $"Money({Cents}, {Currency})";

    // For a class, == compares references unless the class defines it.
    public static bool operator ==(Money? left, Money? right) =>
        left is null ? right is null : left.Equals(right);

    public static bool operator !=(Money? left, Money? right) => !(left == right);

    public static int CountDistinct(IReadOnlyList<long> amounts, IReadOnlyList<string> currencies)
    {
        var seen = new HashSet<Money>();
        for (var i = 0; i < amounts.Count; i++)
        {
            seen.Add(new Money(amounts[i], currencies[i]));
        }
        return seen.Count;
    }
}
