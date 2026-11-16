// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-switch-or-match/.

using Xunit;

public sealed class DaysInMonthTests
{
    public static TheoryData<string, int, int> Cases => new()
    {
        { "january", 1, 31 },
        { "february", 2, 28 },
        { "april", 4, 30 },
        { "july", 7, 31 },
        { "november", 11, 30 },
        { "december", 12, 31 },
        { "zero", 0, 0 },
        { "thirteen", 13, 0 },
        { "minus-one", -1, 0 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int month, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, DaysInMonth.Of(month));
    }
}
