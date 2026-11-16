// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

using System.Linq;
using Xunit;

public sealed class PrimesTests
{
    public static TheoryData<string, int, int[]> Cases => new()
    {
        { "five", 5, new[] { 2, 3, 5, 7, 11 } },
        { "ten", 10, new[] { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 } },
        { "twenty-five", 25, new[] { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97 } },
        { "zero", 0, new int[0] },
        { "one", 1, new[] { 2 } },
        { "two", 2, new[] { 2, 3 } },
        { "past-a-gap", 31, new[] { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void First(string name, int n, int[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Primes.First(n));
    }

    [Fact]
    public void AllIsLazy()
    {
        Assert.Equal(7, Primes.All().Skip(3).First());
    }
}
