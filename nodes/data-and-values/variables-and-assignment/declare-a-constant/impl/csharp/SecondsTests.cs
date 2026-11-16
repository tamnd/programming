// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

using Xunit;

public sealed class SecondsTests
{
    public static TheoryData<string, long, long> Cases => new()
    {
        { "one-day", 1, 86400 },
        { "one-week", 7, 604800 },
        { "one-year", 365, 31536000 },
        { "zero", 0, 0 },
        { "two-days", 2, 172800 },
        { "leap-year", 366, 31622400 },
        { "last-32-bit-day", 24855, 2147472000 },
        { "past-32-bits", 24856, 2147558400 },
        { "million-days", 1000000, 86400000000 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SecondsIn(string name, long days, long want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Seconds.SecondsIn(days));
    }

    [Fact]
    public void SecondsPerDay()
    {
        Assert.Equal(86400L, Seconds.SecondsPerDay);
    }
}
