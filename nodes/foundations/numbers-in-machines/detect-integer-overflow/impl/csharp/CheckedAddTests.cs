// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-integer-overflow/.
using Xunit;

public sealed class CheckedAddTests
{
    private const long Max = long.MaxValue;
    private const long Min = long.MinValue;

    public static TheoryData<string, long, long, long?> Cases => new()
    {
        { "small", 2, 3, 5 },
        { "negative", -7, 4, -3 },
        { "beyond-32-bit", 4000000000, 5000000000, 9000000000 },
        { "max-plus-zero", Max, 0, Max },
        { "max-plus-one", Max, 1, null },
        { "max-plus-max", Max, Max, null },
        { "min-plus-zero", Min, 0, Min },
        { "min-minus-one", Min, -1, null },
        { "min-plus-min", Min, Min, null },
        { "min-plus-max", Min, Max, -1 },
        { "max-minus-one", Max, -1, Max - 1 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, long a, long b, long? want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Overflow.CheckedAdd(a, b));
    }

    [Fact]
    public void UncheckedWraps()
    {
        long a = Max;
        Assert.Equal(Min, unchecked(a + 1));
    }
}
