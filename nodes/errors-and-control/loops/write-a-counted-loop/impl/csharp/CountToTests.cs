// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

using Xunit;

public sealed class CountToTests
{
    public static TheoryData<string, int, int[]> Cases => new()
    {
        { "ten", 10, new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } },
        { "three", 3, new int[] { 1, 2, 3 } },
        { "zero", 0, new int[0] },
        { "one", 1, new int[] { 1 } },
        { "two", 2, new int[] { 1, 2 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int n, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, CountTo.Of(n));
    }
}
