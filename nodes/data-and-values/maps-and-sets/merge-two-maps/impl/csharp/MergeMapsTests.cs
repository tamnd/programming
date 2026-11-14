// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

using System.Collections.Generic;
using System.Linq;
using Xunit;

public sealed class MergeMapsTests
{
    public static TheoryData<string, string[], int[], string[], int[], string[], int[]> Cases => new()
    {
        { "prices", new string[] { "apple", "pear" }, new int[] { 3, 5 }, new string[] { "pear", "fig" }, new int[] { 6, 2 }, new string[] { "apple", "fig", "pear" }, new int[] { 3, 2, 6 } },
        { "disjoint", new string[] { "a" }, new int[] { 1 }, new string[] { "b" }, new int[] { 2 }, new string[] { "a", "b" }, new int[] { 1, 2 } },
        { "empty-base", new string[0], new int[0], new string[] { "a" }, new int[] { 1 }, new string[] { "a" }, new int[] { 1 } },
        { "empty-override", new string[] { "a" }, new int[] { 1 }, new string[0], new int[0], new string[] { "a" }, new int[] { 1 } },
        { "both-empty", new string[0], new int[0], new string[0], new int[0], new string[0], new int[0] },
        { "same-keys", new string[] { "a", "b" }, new int[] { 1, 2 }, new string[] { "a", "b" }, new int[] { 9, 8 }, new string[] { "a", "b" }, new int[] { 9, 8 } },
        { "zero-wins", new string[] { "a" }, new int[] { 5 }, new string[] { "a" }, new int[] { 0 }, new string[] { "a" }, new int[] { 0 } },
        { "case", new string[] { "a" }, new int[] { 1 }, new string[] { "A" }, new int[] { 2 }, new string[] { "A", "a" }, new int[] { 2, 1 } },
    };

    private static Dictionary<string, int> Map(string[] keys, int[] values) => keys.Zip(values).ToDictionary(p => p.First, p => p.Second);

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] baseKeys, int[] baseValues, string[] overKeys, int[] overValues, string[] wantKeys, int[] wantValues)
    {
        Assert.NotNull(name);
        var baseMap = Map(baseKeys, baseValues);
        var over = Map(overKeys, overValues);
        var got = MergeMaps.Merge(baseMap, over);
        Assert.Equal(wantKeys, got.Select(p => p.Key));
        Assert.Equal(wantValues, got.Select(p => p.Value));
        Assert.Equal(Map(baseKeys, baseValues), baseMap);
        Assert.Equal(Map(overKeys, overValues), over);
    }
}
