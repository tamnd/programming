// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

using System.Linq;
using Xunit;

public sealed class CountPointsTests
{
    public static TheoryData<string, int[], int[], int[], int[], int[]> Cases => new()
    {
        { "clicks", new int[] { 1, 3, 1 }, new int[] { 2, 4, 2 }, new int[] { 1, 3 }, new int[] { 2, 4 }, new int[] { 2, 1 } },
        { "swap", new int[] { 2, 1 }, new int[] { 1, 2 }, new int[] { 1, 2 }, new int[] { 2, 1 }, new int[] { 1, 1 } },
        { "empty", new int[0], new int[0], new int[0], new int[0], new int[0] },
        { "one", new int[] { 0 }, new int[] { 0 }, new int[] { 0 }, new int[] { 0 }, new int[] { 1 } },
        { "negative", new int[] { -1, 0, -1 }, new int[] { 0, -1, 0 }, new int[] { -1, 0 }, new int[] { 0, -1 }, new int[] { 2, 1 } },
        { "joined-digits", new int[] { 1, 12 }, new int[] { 23, 3 }, new int[] { 1, 12 }, new int[] { 23, 3 }, new int[] { 1, 1 } },
        { "all-same", new int[] { 5, 5, 5 }, new int[] { 5, 5, 5 }, new int[] { 5 }, new int[] { 5 }, new int[] { 3 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] xs, int[] ys, int[] wantX, int[] wantY, int[] wantCount)
    {
        Assert.NotNull(name);
        var got = CountPoints.Count(xs.Zip(ys, (x, y) => new Point(x, y)));
        Assert.Equal(wantX, got.Select(t => t.X));
        Assert.Equal(wantY, got.Select(t => t.Y));
        Assert.Equal(wantCount, got.Select(t => t.Count));
    }

    [Fact]
    public void EqualPointsHaveEqualHashCodes()
    {
        Assert.Equal(new Point(1, 2), new Point(1, 2));
        Assert.Equal(new Point(1, 2).GetHashCode(), new Point(1, 2).GetHashCode());
    }
}
