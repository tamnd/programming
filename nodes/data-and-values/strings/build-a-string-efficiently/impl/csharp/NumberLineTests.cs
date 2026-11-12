// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

using Xunit;

public sealed class NumberLineTests
{
    public static TheoryData<string, int, string> Cases => new()
    {
        { "zero", 0, "" },
        { "one", 1, "1" },
        { "three", 3, "1,2,3" },
        { "five", 5, "1,2,3,4,5" },
        { "seven", 7, "1,2,3,4,5,6,7" },
        { "nine", 9, "1,2,3,4,5,6,7,8,9" },
        { "ten", 10, "1,2,3,4,5,6,7,8,9,10" },
    };

    // Long results: the length, the start and the end.
    public static TheoryData<string, int, int, string> LongCases => new()
    {
        { "hundred", 100, 291, ",99,100" },
        { "hundred-and-one", 101, 295, ",100,101" },
        { "thousand", 1000, 3892, ",999,1000" },
        { "large", 100000, 588894, ",99999,100000" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Exact(string name, int n, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, NumberLine.Build(n));
    }

    [Theory]
    [MemberData(nameof(LongCases))]
    public void LongResult(string name, int n, int size, string end)
    {
        Assert.NotNull(name);
        string got = NumberLine.Build(n);
        Assert.Equal(size, got.Length);
        Assert.StartsWith("1,2,3,", got);
        Assert.EndsWith(end, got);
    }
}
