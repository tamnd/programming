// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

using Xunit;

public sealed class JoinTests
{
    public static TheoryData<string, string[], string, string> Cases => new()
    {
        { "comma", new[] { "a", "b", "c" }, ",", "a,b,c" },
        { "comma-space", new[] { "red", "green", "blue" }, ", ", "red, green, blue" },
        { "words", new[] { "Hello", "world" }, " ", "Hello world" },
        { "empty-list", new string[] { }, ",", "" },
        { "single", new[] { "only" }, ",", "only" },
        { "empty-items", new[] { "", "", "" }, ",", ",," },
        { "empty-middle", new[] { "a", "", "b" }, "-", "a--b" },
        { "empty-sep", new[] { "a", "b", "c" }, "", "abc" },
        { "long-sep", new[] { "x", "y" }, " -> ", "x -> y" },
        { "non-ascii", new[] { "naïve", "日本" }, "·", "naïve·日本" },
        { "astral-sep", new[] { "a", "b" }, "\U0001F642", "a\U0001F642b" },
        { "sep-in-item", new[] { "a,b", "c" }, ",", "a,b,c" },
    };

    public static TheoryData<string, int[], string, string> NumberCases => new()
    {
        { "numbers", new[] { 1, 2, 3 }, ",", "1,2,3" },
        { "negative", new[] { -1, 0, 1 }, " ", "-1 0 1" },
        { "empty", new int[] { }, ",", "" },
        { "single", new[] { 42 }, ",", "42" },
        { "int32-ends", new[] { 2147483647, -2147483648 }, ";", "2147483647;-2147483648" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Strings(string name, string[] items, string sep, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Join.Strings(items, sep));
    }

    [Theory]
    [MemberData(nameof(NumberCases))]
    public void Numbers(string name, int[] items, string sep, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Join.Numbers(items, sep));
    }
}
