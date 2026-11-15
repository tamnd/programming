// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

using Xunit;

public sealed class FizzBuzzTests
{
    public static TheoryData<string, int, string[]> Cases => new()
    {
        { "fifteen", 15, new[] { "1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz" } },
        { "thirty", 30, new[] { "1", "2", "Fizz", "4", "Buzz", "Fizz", "7", "8", "Fizz", "Buzz", "11", "Fizz", "13", "14", "FizzBuzz", "16", "17", "Fizz", "19", "Buzz", "Fizz", "22", "23", "Fizz", "Buzz", "26", "Fizz", "28", "29", "FizzBuzz" } },
        { "zero", 0, new string[0] },
        { "one", 1, new[] { "1" } },
        { "three", 3, new[] { "1", "2", "Fizz" } },
        { "five", 5, new[] { "1", "2", "Fizz", "4", "Buzz" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, int n, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, FizzBuzz.Of(n));
    }
}
