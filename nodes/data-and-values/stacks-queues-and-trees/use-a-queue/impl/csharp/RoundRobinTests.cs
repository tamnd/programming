// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

using System.Linq;
using Xunit;

public sealed class RoundRobinTests
{
    public static TheoryData<string, int[], int, int[]> Cases => new()
    {
        { "example", new[] { 3, 1, 2 }, 1, new[] { 1, 2, 0 } },
        { "slice-two", new[] { 4, 1, 3, 2 }, 2, new[] { 1, 3, 0, 2 } },
        { "empty", new int[0], 3, new int[0] },
        { "one", new[] { 7 }, 2, new[] { 0 } },
        { "big-slice", new[] { 3, 1, 2 }, 10, new[] { 0, 1, 2 } },
        { "slice-one", new[] { 2, 2, 1 }, 1, new[] { 2, 0, 1 } },
        { "equal-work", new[] { 2, 2, 2 }, 1, new[] { 0, 1, 2 } },
        { "long-job-first", new[] { 10, 1, 1 }, 3, new[] { 1, 2, 0 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] work, int slice, int[] want)
    {
        Assert.NotNull(name);
        var before = work.ToArray();
        Assert.Equal(want, RoundRobin.FinishOrder(work, slice));
        Assert.Equal(before, work);
    }
}
