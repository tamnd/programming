// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

using Xunit;

public sealed class FilledTests
{
    public static TheoryData<string, int, int, int[]> Cases => new()
    {
        { "three-sevens", 3, 7, new int[] { 7, 7, 7 } },
        { "five-zeros", 5, 0, new int[] { 0, 0, 0, 0, 0 } },
        { "empty", 0, 7, new int[0] },
        { "single", 1, 42, new int[] { 42 } },
        { "zero", 2, 0, new int[] { 0, 0 } },
        { "negative", 2, -1, new int[] { -1, -1 } },
        { "max-int", 2, 2147483647, new int[] { 2147483647, 2147483647 } },
        { "min-int", 2, int.MinValue, new int[] { int.MinValue, int.MinValue } },
        { "ten", 10, 1, new int[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int n, int value, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Filled.Create(n, value));
    }
}
