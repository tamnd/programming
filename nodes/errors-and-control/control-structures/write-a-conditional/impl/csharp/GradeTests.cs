// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-conditional/.

using Xunit;

public sealed class GradeTests
{
    public static TheoryData<string, int, string> Cases => new()
    {
        { "ninety-five", 95, "A" },
        { "eighty-five", 85, "B" },
        { "seventy-five", 75, "C" },
        { "sixty-five", 65, "D" },
        { "forty", 40, "F" },
        { "hundred", 100, "A" },
        { "zero", 0, "F" },
        { "ninety", 90, "A" },
        { "eighty-nine", 89, "B" },
        { "sixty", 60, "D" },
        { "fifty-nine", 59, "F" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int score, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Grade.Of(score));
    }
}
