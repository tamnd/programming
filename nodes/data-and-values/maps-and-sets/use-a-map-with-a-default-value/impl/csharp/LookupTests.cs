// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

using System.Collections.Generic;
using System.Linq;
using Xunit;

public sealed class LookupTests
{
    public static TheoryData<string, string[], int[], string[], int, int[]> Cases => new()
    {
        { "prices", new string[] { "apple", "pear" }, new int[] { 3, 5 }, new string[] { "apple", "fig", "pear" }, 0, new int[] { 3, 0, 5 } },
        { "negative-fallback", new string[] { "a" }, new int[] { 1 }, new string[] { "b", "a" }, -1, new int[] { -1, 1 } },
        { "empty-table", new string[0], new int[0], new string[] { "a", "b" }, 7, new int[] { 7, 7 } },
        { "empty-keys", new string[] { "a" }, new int[] { 1 }, new string[0], 0, new int[0] },
        { "zero-value", new string[] { "a" }, new int[] { 0 }, new string[] { "a", "b" }, 5, new int[] { 0, 5 } },
        { "repeat", new string[] { "a" }, new int[] { 1 }, new string[] { "b", "b", "a" }, 2, new int[] { 2, 2, 1 } },
        { "case", new string[] { "a" }, new int[] { 1 }, new string[] { "A" }, 0, new int[] { 0 } },
    };

    private static Dictionary<string, int> Map(string[] keys, int[] values) => keys.Zip(values).ToDictionary(p => p.First, p => p.Second);

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] tableKeys, int[] tableValues, string[] keys, int fallback, int[] want)
    {
        Assert.NotNull(name);
        var table = Map(tableKeys, tableValues);
        Assert.Equal(want, Lookup.Get(table, keys, fallback));
        Assert.Equal(Map(tableKeys, tableValues), table);
    }
}
