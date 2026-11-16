// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/overload-an-operator/.

using Xunit;

public sealed class Vec2Tests
{
    public static TheoryData<string, long[], long[], long[]> Cases => new()
    {
        { "two", new long[] { 1, 3 }, new long[] { 2, 4 }, new long[] { 4, 6 } },
        { "three", new long[] { 1, 2, 3 }, new long[] { 10, 20, 30 }, new long[] { 6, 60 } },
        { "empty", new long[] { }, new long[] { }, new long[] { 0, 0 } },
        { "single", new long[] { 5 }, new long[] { -7 }, new long[] { 5, -7 } },
        { "negative", new long[] { -1, -2 }, new long[] { -3, -4 }, new long[] { -3, -7 } },
        { "cancel", new long[] { 3, -3 }, new long[] { 5, -5 }, new long[] { 0, 0 } },
        { "limit", new long[] { 1_000_000_000, 1_000_000_000, 1_000_000_000 },
          new long[] { -1_000_000_000, -1_000_000_000, -1_000_000_000 }, new long[] { 3_000_000_000, -3_000_000_000 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void VectorSum(string name, long[] xs, long[] ys, long[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Vec2.VectorSum(xs, ys));
    }

    [Fact]
    public void Operators()
    {
        var a = new Vec2(1, 2);
        var b = new Vec2(3, 4);
        Assert.True(a + b == new Vec2(4, 6));
        Assert.True(a != b);
        Assert.False(a == b);
        var c = a;
        c += b;
        Assert.Equal(new Vec2(4, 6), c);
    }
}
