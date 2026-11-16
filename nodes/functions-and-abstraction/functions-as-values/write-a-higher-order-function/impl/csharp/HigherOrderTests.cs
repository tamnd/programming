// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-higher-order-function/.

using Xunit;

public sealed class HigherOrderTests
{
    public static TheoryData<string, int, int, int> Cases => new()
    {
        { "one-step", 6, 1, 3 },
        { "three-steps", 6, 3, 5 },
        { "ten-steps", 27, 10, 214 },
        { "zero-times", 7, 0, 7 },
        { "start-one", 1, 1, 4 },
        { "cycle", 1, 3, 1 },
        { "past-one", 16, 10, 1 },
        { "peak-27", 27, 77, 9232 },
        { "peak-9663", 9663, 48, 27114424 },
        { "limit", 10000, 1000, 2 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CollatzAfter(string name, int start, int times, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, HigherOrder.CollatzAfter(start, times));
    }

    [Fact]
    public void RepeatTakesALambda()
    {
        Assert.Equal(1024, HigherOrder.Repeat(x => x * 2, 10)(1));
    }

    [Fact]
    public void RepeatZeroTimesIsTheIdentity()
    {
        Assert.Equal(27, HigherOrder.Repeat(HigherOrder.CollatzStep, 0)(27));
    }
}
