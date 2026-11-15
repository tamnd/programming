// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-class/.

using Xunit;

public sealed class RectangleTests
{
    public static TheoryData<string, int, int, int, int[]> Cases => new()
    {
        { "three-by-four", 3, 4, 2, new[] { 12, 14, 48, 28 } },
        { "square", 5, 5, 3, new[] { 25, 20, 225, 60 } },
        { "zero-side", 0, 7, 2, new[] { 0, 14, 0, 28 } },
        { "zero-factor", 3, 4, 0, new[] { 12, 14, 0, 0 } },
        { "identity", 3, 4, 1, new[] { 12, 14, 12, 14 } },
        { "limit", 1000, 1000, 46, new[] { 1000000, 4000, 2116000000, 184000 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Report(string name, int width, int height, int factor, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Rectangle.Report(width, height, factor));
    }

    [Fact]
    public void InstancesHaveTheirOwnFields()
    {
        var a = new Rectangle(1, 2);
        var b = new Rectangle(1, 2);
        a.Scale(3);
        Assert.Equal("Rectangle(3, 6)", a.ToString());
        Assert.Equal("Rectangle(1, 2)", b.ToString());
    }
}
