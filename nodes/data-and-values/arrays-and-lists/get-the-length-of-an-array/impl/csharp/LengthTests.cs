// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

using Xunit;

public sealed class LengthTests
{
    public static TheoryData<string, string[], int> Cases => new()
    {
        { "three", new string[] { "a", "b", "c" }, 3 },
        { "two-words", new string[] { "apple", "banana" }, 2 },
        { "empty", new string[0], 0 },
        { "single", new string[] { "x" }, 1 },
        { "empty-strings", new string[] { "", "" }, 2 },
        { "non-ascii", new string[] { "é", "日本語" }, 2 },
        { "duplicates", new string[] { "a", "a", "a" }, 3 },
        { "ten", new string[] { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" }, 10 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] items, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Length.Of(items));
    }
}
