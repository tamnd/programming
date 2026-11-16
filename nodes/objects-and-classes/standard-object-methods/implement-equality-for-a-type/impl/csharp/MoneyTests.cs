// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-equality-for-a-type/.

using System.Collections.Generic;
using Xunit;

public sealed class MoneyTests
{
    public static TheoryData<string, long[], string[], int> Cases => new()
    {
        { "two-equal", new long[] { 1250, 1250 }, new[] { "EUR", "EUR" }, 1 },
        { "different-amounts", new long[] { 100, 200, 100 }, new[] { "USD", "USD", "USD" }, 2 },
        { "mixed", new long[] { 500, 500, 500, 700 }, new[] { "EUR", "USD", "EUR", "EUR" }, 3 },
        { "empty", new long[] { }, new string[] { }, 0 },
        { "case", new long[] { 999, 999, 999 }, new[] { "gbp", "GBP", "Gbp" }, 1 },
        { "other-currency", new long[] { 0, 0, 0 }, new[] { "EUR", "USD", "JPY" }, 3 },
        { "negative", new long[] { -5, 5, -5 }, new[] { "EUR", "EUR", "EUR" }, 2 },
        { "limit", new long[] { 1_000_000_000_000, -1_000_000_000_000, 1_000_000_000_000 }, new[] { "usd", "USD", "USD" }, 2 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CountDistinct(string name, long[] amounts, string[] currencies, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Money.CountDistinct(amounts, currencies));
    }

    [Fact]
    public void EqualityRules()
    {
        var a = new Money(1250, "EUR");
        var b = new Money(1250, "eur");
        Assert.True(a.Equals(a));
        Assert.True(a.Equals(b) && b.Equals(a));
        Assert.True(a == b);
        Assert.False(a != b);
        Assert.False(ReferenceEquals(a, b));
        Assert.Equal(a.GetHashCode(), b.GetHashCode());
        Assert.NotEqual(a, new Money(1251, "EUR"));
        Assert.NotEqual(a, new Money(1250, "USD"));
        Assert.False(a.Equals(null));
        Assert.False(a.Equals((object)"1250 EUR"));
        Assert.False(a == null);
    }

    [Fact]
    public void DictionaryKey()
    {
        var names = new Dictionary<Money, string> { [new Money(500, "USD")] = "lunch" };
        Assert.Equal("lunch", names[new Money(500, "usd")]);
    }
}
