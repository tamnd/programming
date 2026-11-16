// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

using System;
using Xunit;

public sealed class ColorTests
{
    public static TheoryData<string, int[], int[], int[], string[]> Cases => new()
    {
        { "orange", new[] { 255 }, new[] { 128 }, new[] { 0 }, new[] { "#FF8000" } },
        { "three", new[] { 255, 0, 0 }, new[] { 0, 255, 0 }, new[] { 0, 0, 255 }, new[] { "#FF0000", "#00FF00", "#0000FF" } },
        { "mixed", new[] { 18, 171 }, new[] { 52, 205 }, new[] { 86, 239 }, new[] { "#123456", "#ABCDEF" } },
        { "empty", new int[] { }, new int[] { }, new int[] { }, new string[] { } },
        { "single", new[] { 1 }, new[] { 2 }, new[] { 3 }, new[] { "#010203" } },
        { "zero", new[] { 0 }, new[] { 0 }, new[] { 0 }, new[] { "#000000" } },
        { "max", new[] { 255 }, new[] { 255 }, new[] { 255 }, new[] { "#FFFFFF" } },
        { "padding", new[] { 10, 0 }, new[] { 11, 15 }, new[] { 12, 16 }, new[] { "#0A0B0C", "#000F10" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void FormatColors(string name, int[] reds, int[] greens, int[] blues, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Color.FormatColors(reds, greens, blues));
    }

    [Fact]
    public void Forms()
    {
        var c = new Color(255, 128, 0);
        Assert.Equal("#FF8000", c.ToString());
        Assert.Equal("color #FF8000", $"color {c}");
        Assert.Equal("#FF8000, #000000", string.Join(", ", c, new Color(0, 0, 0)));
    }

    [Fact]
    public void BadValue()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => new Color(256, 0, 0));
    }
}
