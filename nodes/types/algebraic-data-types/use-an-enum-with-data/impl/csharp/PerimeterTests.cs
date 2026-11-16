// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-an-enum-with-data/.
using Xunit;

public sealed class PerimeterTests
{
    public static TheoryData<string, string, int[], int?> Cases => new()
    {
        { "square", "square", new[] { 3 }, 12 },
        { "rectangle", "rectangle", new[] { 2, 5 }, 14 },
        { "triangle", "triangle", new[] { 3, 4, 5 }, 12 },
        { "unknown-kind", "circle", new[] { 2 }, null },
        { "wrong-case", "Square", new[] { 3 }, null },
        { "too-few-sides", "rectangle", new[] { 4 }, null },
        { "too-many-sides", "square", new[] { 3, 3 }, null },
        { "no-sides", "triangle", new int[0], null },
        { "zero-side", "square", new[] { 0 }, null },
        { "negative-side", "rectangle", new[] { -2, 5 }, null },
        { "not-a-triangle", "triangle", new[] { 1, 2, 3 }, null },
        { "largest", "triangle", new[] { 1000000, 1000000, 1000000 }, 3000000 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string kind, int[] sides, int? want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Perimeter.Of(kind, sides));
    }

    [Fact]
    public void VariantsCarryTheirData()
    {
        Assert.Equal(new Shape.Rectangle(2, 5), Perimeter.MakeShape("rectangle", new[] { 2, 5 }));
    }
}
