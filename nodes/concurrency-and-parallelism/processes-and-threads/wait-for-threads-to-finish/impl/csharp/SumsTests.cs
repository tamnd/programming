// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

using System.Linq;
using Xunit;

public sealed class SumsTests
{
    private const long G = 1_000_000_000;

    public static TheoryData<string, long[][], long[]> Cases => new()
    {
        { "two", new[] { new long[] { 1, 2, 3 }, new long[] { 4, 5 } }, new long[] { 6, 9 } },
        { "three", new[] { new long[] { 10 }, new long[] { 20, 30 }, new long[] { -5, 5 } }, new long[] { 10, 50, 0 } },
        { "empty", new long[][] { }, new long[] { } },
        { "empty-chunk", new[] { new long[] { }, new long[] { 7 } }, new long[] { 0, 7 } },
        { "single", new[] { new long[] { 42 } }, new long[] { 42 } },
        { "negative", new[] { new long[] { -1, -2 }, new long[] { -3 } }, new long[] { -3, -3 } },
        { "limit", new[] { new long[] { G, G, G }, new long[] { -G, -G, -G } }, new long[] { 3 * G, -3 * G } },
        { "many", Enumerable.Range(0, 32).Select(i => new long[] { i, i, i }).ToArray(),
          Enumerable.Range(0, 32).Select(i => 3L * i).ToArray() },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ParallelSums(string name, long[][] chunks, long[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Sums.ParallelSums(chunks));
    }
}
