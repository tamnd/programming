// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

using Xunit;

public sealed class ToggleTests
{
    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "colors", new string[] { "red", "blue", "red", "green" }, new string[] { "blue", "green" } },
        { "fresh", new string[] { "b", "a" }, new string[] { "a", "b" } },
        { "empty", new string[0], new string[0] },
        { "two-times", new string[] { "a", "a" }, new string[0] },
        { "three-times", new string[] { "a", "a", "a" }, new string[] { "a" } },
        { "case", new string[] { "a", "A" }, new string[] { "A", "a" } },
        { "prefix", new string[] { "ab", "a" }, new string[] { "a", "ab" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] words, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Toggle.Run(words));
    }
}
