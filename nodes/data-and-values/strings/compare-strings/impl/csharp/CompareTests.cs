// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

using Xunit;

public sealed class CompareTests
{
    // U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
    public static TheoryData<string, string, string, int> Order => new()
    {
        { "less", "apple", "banana", -1 },
        { "equal", "apple", "apple", 0 },
        { "greater", "pear", "apple", 1 },
        { "both-empty", "", "", 0 },
        { "empty-first", "", "a", -1 },
        { "prefix", "app", "apple", -1 },
        { "prefix-longer", "apple", "app", 1 },
        { "upper-before-lower", "Zebra", "apple", -1 },
        { "lower-after-upper", "a", "A", 1 },
        { "digits", "10", "9", -1 },
        { "non-ascii", "\u00e9", "z", 1 },
        { "astral-after-bmp", "\uff61", "\U0001f642", -1 },
        { "astral", "\U0001f642", "\U0001f643", -1 },
        { "combining", "cafe\u0301", "caf\u00e9", -1 },
    };

    public static TheoryData<string, string, string, bool> Equal => new()
    {
        { "eq-same", "abc", "abc", true },
        { "eq-case", "abc", "ABC", false },
        { "eq-empty", "", "", true },
        { "eq-combining", "cafe\u0301", "caf\u00e9", false },
        { "eq-astral", "\U0001f642", "\U0001f642", true },
        { "eq-trailing-space", "abc", "abc ", false },
    };

    [Theory]
    [MemberData(nameof(Order))]
    public void Compares(string name, string a, string b, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, StringOrder.Compare(a, b));
    }

    [Theory]
    [MemberData(nameof(Equal))]
    public void EqualsTest(string name, string a, string b, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, StringOrder.AreEqual(a, b));
    }
}
