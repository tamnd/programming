// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

using Xunit;

public sealed class CompareSetsTests
{
    public static TheoryData<string, string[], string[], string[], string[], string[], string[], bool> Cases => new()
    {
        { "colors", new string[] { "red", "blue", "green" }, new string[] { "blue", "yellow" }, new string[] { "blue", "green", "red", "yellow" }, new string[] { "blue" }, new string[] { "green", "red" }, new string[] { "green", "red", "yellow" }, false },
        { "subset", new string[] { "b" }, new string[] { "a", "b", "c" }, new string[] { "a", "b", "c" }, new string[] { "b" }, new string[0], new string[] { "a", "c" }, true },
        { "both-empty", new string[0], new string[0], new string[0], new string[0], new string[0], new string[0], true },
        { "empty-a", new string[0], new string[] { "x" }, new string[] { "x" }, new string[0], new string[0], new string[] { "x" }, true },
        { "empty-b", new string[] { "x" }, new string[0], new string[] { "x" }, new string[0], new string[] { "x" }, new string[] { "x" }, false },
        { "equal", new string[] { "a", "b" }, new string[] { "b", "a" }, new string[] { "a", "b" }, new string[] { "a", "b" }, new string[0], new string[0], true },
        { "disjoint", new string[] { "a" }, new string[] { "b" }, new string[] { "a", "b" }, new string[0], new string[] { "a" }, new string[] { "a", "b" }, false },
        { "duplicates", new string[] { "a", "a", "b" }, new string[] { "b", "b" }, new string[] { "a", "b" }, new string[] { "b" }, new string[] { "a" }, new string[] { "a" }, false },
        { "case", new string[] { "a" }, new string[] { "A" }, new string[] { "A", "a" }, new string[0], new string[] { "a" }, new string[] { "A", "a" }, false },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] a, string[] b, string[] union, string[] intersection, string[] difference, string[] symmetric, bool isSubset)
    {
        Assert.NotNull(name);
        var got = CompareSets.Run(a, b);
        Assert.Equal(union, got.Union);
        Assert.Equal(intersection, got.Intersection);
        Assert.Equal(difference, got.Difference);
        Assert.Equal(symmetric, got.SymmetricDifference);
        Assert.Equal(isSubset, got.IsSubset);
    }
}
