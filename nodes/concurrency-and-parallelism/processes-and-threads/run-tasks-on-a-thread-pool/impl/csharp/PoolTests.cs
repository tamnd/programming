// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

using System.Linq;
using Xunit;

public sealed class PoolTests
{
    public static TheoryData<string, int, int[], int[]> Cases => new()
    {
        { "four-jobs", 2, new[] { 1, 2, 3, 6 }, new[] { 0, 1, 7, 8 } },
        { "long-runs", 3, new[] { 27, 97, 871 }, new[] { 111, 118, 178 } },
        { "empty", 2, new int[] { }, new int[] { } },
        { "one-worker", 1, new[] { 7, 8, 9 }, new[] { 16, 3, 19 } },
        { "more-workers-than-jobs", 8, new[] { 5, 6 }, new[] { 5, 8 } },
        { "duplicates", 3, new[] { 27, 27, 27, 1 }, new[] { 111, 111, 111, 0 } },
        { "wide-intermediate", 2, new[] { 113383, 837799 }, new[] { 247, 524 } },
        { "many", 4, Enumerable.Range(1, 200).ToArray(),
          Enumerable.Range(1, 200).Select(n => Pool.CollatzSteps(n)).ToArray() },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void StepsOnPool(string name, int workers, int[] numbers, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Pool.StepsOnPool(workers, numbers));
    }
}
