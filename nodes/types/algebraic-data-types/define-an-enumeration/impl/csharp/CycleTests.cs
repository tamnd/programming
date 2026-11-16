// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

using Xunit;

public sealed class CycleTests
{
    public static TheoryData<string, string, int, string[]> Cases => new()
    {
        { "weekdays", "monday", 3, new[] { "monday", "tuesday", "wednesday" } },
        { "wrap", "friday", 4, new[] { "friday", "saturday", "sunday", "monday" } },
        { "zero-steps", "monday", 0, new string[0] },
        { "more-than-a-week", "sunday", 9, new[] { "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday" } },
        { "unknown", "funday", 3, new string[0] },
        { "wrong-case", "Friday", 2, new string[0] },
        { "number", "3", 2, new string[0] },
        { "empty-name", "", 1, new string[0] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string start, int steps, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Cycle.Of(start, steps));
    }

    [Fact]
    public void ToAndFromInt()
    {
        Assert.Equal(4, (int)Day.Friday);
        Assert.Equal(Day.Friday, (Day)4);
        Assert.Equal("Friday", Day.Friday.ToString());
    }
}
