// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

using System.Linq;
using Xunit;

public sealed class BuildMapTests
{
    public static TheoryData<string, string[], int[], string[], string[], int[]> Cases => new()
    {
        { "fruits", new string[] { "apple", "pear", "fig" }, new int[] { 3, 5, 2 }, new string[0], new string[] { "apple", "fig", "pear" }, new int[] { 3, 2, 5 } },
        { "update-and-delete", new string[] { "a", "b", "a", "c" }, new int[] { 1, 2, 7, 3 }, new string[] { "b" }, new string[] { "a", "c" }, new int[] { 7, 3 } },
        { "empty", new string[0], new int[0], new string[0], new string[0], new int[0] },
        { "replace", new string[] { "k", "k", "k" }, new int[] { 1, 2, 3 }, new string[0], new string[] { "k" }, new int[] { 3 } },
        { "delete-absent", new string[] { "a" }, new int[] { 1 }, new string[] { "z" }, new string[] { "a" }, new int[] { 1 } },
        { "delete-all", new string[] { "a", "b" }, new int[] { 1, 2 }, new string[] { "a", "b" }, new string[0], new int[0] },
        { "delete-twice", new string[] { "a", "b" }, new int[] { 1, 2 }, new string[] { "a", "a" }, new string[] { "b" }, new int[] { 2 } },
        { "zero-value", new string[] { "a", "b" }, new int[] { 0, -4 }, new string[0], new string[] { "a", "b" }, new int[] { 0, -4 } },
        { "case", new string[] { "b", "B", "a", "A" }, new int[] { 1, 2, 3, 4 }, new string[0], new string[] { "A", "B", "a", "b" }, new int[] { 4, 2, 3, 1 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] keys, int[] values, string[] remove, string[] wantKeys, int[] wantValues)
    {
        Assert.NotNull(name);
        var got = BuildMap.Build(keys.Zip(values).ToArray(), remove);
        Assert.Equal(wantKeys, got.Select(p => p.Key));
        Assert.Equal(wantValues, got.Select(p => p.Value));
    }
}
