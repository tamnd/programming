// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-method-on-a-type/.

using Xunit;

public sealed class PointTests
{
    public static TheoryData<string, int, int, int, int, int[]> Cases => new()
    {
        { "right-and-up", 1, 2, 3, 4, new[] { 4, 6, 7, 10 } },
        { "mixed-signs", 5, -1, -2, 3, new[] { 3, 2, 5, 5 } },
        { "zero-move", 7, -3, 0, 0, new[] { 7, -3, 0, 10 } },
        { "negative", -2, -5, -3, -1, new[] { -5, -6, 4, 11 } },
        { "origin", 2, 3, -2, -3, new[] { 0, 0, 5, 0 } },
        { "limit", 100000000, -100000000, 100000000, -100000000, new[] { 200000000, -200000000, 200000000, 400000000 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Report(string name, int x, int y, int dx, int dy, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Point.Report(x, y, dx, dy));
    }

    [Fact]
    public void TranslateReturnsAnotherPoint()
    {
        var p = new Point(1, 2);
        var q = p.Translate(3, 4);
        Assert.Equal(new Point(1, 2), p);
        Assert.Equal(new Point(4, 6), q);
        Assert.Equal("Point { X = 4, Y = 6 }", q.ToString());
    }
}
