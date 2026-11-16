// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

using Xunit;

public sealed class PrimeTests
{
    public static TheoryData<string, long, bool> Cases => new()
    {
        { "small-prime", 17, true },
        { "small-composite", 15, false },
        { "even", 100, false },
        { "odd-composite", 91, false },
        { "negative", -7, false },
        { "zero", 0, false },
        { "one", 1, false },
        { "two", 2, true },
        { "three", 3, true },
        { "square-of-prime", 25, false },
        { "large-square", 4293001441, false },
        { "carmichael", 561, false },
        { "large-prime", 1000000007, true },
        { "mersenne-prime", 2147483647, true },
        { "large-composite", 2147483649, false },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void IsPrime(string name, long n, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Prime.IsPrime(n));
    }
}
