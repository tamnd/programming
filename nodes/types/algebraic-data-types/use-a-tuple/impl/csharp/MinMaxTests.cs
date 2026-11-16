// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

using System.Linq;
using Xunit;

public sealed class MinMaxTests
{
    public static TheoryData<string, int[], int, int> Cases => new()
    {
        { "mixed", new[] { 3, 1, 2 }, 1, 3 },
        { "longer", new[] { 4, 9, -2, 7, 0 }, -2, 9 },
        { "one", new[] { 5 }, 5, 5 },
        { "negative", new[] { -3, -1, -7 }, -7, -1 },
        { "all-same", new[] { 2, 2, 2 }, 2, 2 },
        { "int32-extremes", new[] { int.MaxValue, 0, int.MinValue }, int.MinValue, int.MaxValue },
        { "sorted", new[] { 1, 2, 3, 4 }, 1, 4 },
        { "reversed", new[] { 4, 3, 2, 1 }, 1, 4 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int[] numbers, int min, int max)
    {
        Assert.NotNull(name);
        var before = numbers.ToArray();
        var (lo, hi) = MinMax.Of(numbers); // deconstruction
        Assert.Equal(min, lo);
        Assert.Equal(max, hi);
        Assert.Equal(before, numbers);
    }

    [Fact]
    public void TuplesCompareByValue()
    {
        Assert.True(MinMax.Of(new[] { 3, 1, 2 }) == (1, 3));
        Assert.Equal(3, MinMax.Of(new[] { 3, 1, 2 }).Max);
    }
}
