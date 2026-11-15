// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

using Xunit;

public sealed class CountByTests
{
    public static TheoryData<string, int, int, int, int[]> Cases => new()
    {
        { "by-two", 2, 8, 2, new int[] { 2, 4, 6, 8 } },
        { "down", 10, 0, -1, new int[] { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 } },
        { "uneven", 1, 10, 3, new int[] { 1, 4, 7, 10 } },
        { "past-stop", 0, 9, 4, new int[] { 0, 4, 8 } },
        { "down-by-three", 10, 1, -3, new int[] { 10, 7, 4, 1 } },
        { "empty-up", 5, 1, 1, new int[0] },
        { "empty-down", 1, 5, -1, new int[0] },
        { "single", 3, 3, 7, new int[] { 3 } },
        { "negative", -3, 3, 2, new int[] { -3, -1, 1, 3 } },
        { "limit-up", 2147483640, 2147483647, 5, new int[] { 2147483640, 2147483645 } },
        { "limit-down", -2147483640, int.MinValue, -5, new int[] { -2147483640, -2147483645 } },
        { "big-step", 0, 10, 2147483647, new int[] { 0 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int start, int stop, int step, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, CountBy.Of(start, stop, step));
    }
}
