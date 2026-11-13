// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

using Xunit;

public sealed class RotateLeftTests
{
    public static TheoryData<string, int[], int, int[]> Cases => new()
    {
        { "by-two", new int[] { 1, 2, 3, 4, 5 }, 2, new int[] { 3, 4, 5, 1, 2 } },
        { "by-one", new int[] { 1, 2, 3 }, 1, new int[] { 2, 3, 1 } },
        { "zero", new int[] { 1, 2, 3 }, 0, new int[] { 1, 2, 3 } },
        { "full-turn", new int[] { 1, 2, 3 }, 3, new int[] { 1, 2, 3 } },
        { "more-than-length", new int[] { 1, 2, 3 }, 4, new int[] { 2, 3, 1 } },
        { "much-larger", new int[] { 1, 2, 3 }, 2000000000, new int[] { 3, 1, 2 } },
        { "empty", new int[0], 5, new int[0] },
        { "empty-zero", new int[0], 0, new int[0] },
        { "single", new int[] { 7 }, 3, new int[] { 7 } },
        { "duplicates", new int[] { 1, 1, 2 }, 1, new int[] { 1, 2, 1 } },
        { "negative-items", new int[] { -1, -2, -3 }, 1, new int[] { -2, -3, -1 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] items, int k, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, RotateLeft.By(items, k));
    }
}
