// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

using System.Text.RegularExpressions;
using Xunit;

public sealed class RandomNumbersTests
{
    public static TheoryData<string, uint[], long, long, int, long[]> RangeCases => new()
    {
        { "coin", new uint[] {5u, 2u, 7u}, 0L, 1L, 3, [1L, 0L, 1L] },
        { "no-results", new uint[] {}, 1L, 6L, 0, [] },
        { "die", new uint[] {7u, 12u, 1000000u, 4294967295u}, 1L, 6L, 4, [2L, 1L, 5L, 4L] },
        { "negative-range", new uint[] {100u, 2000000000u, 4294967294u}, -10L, 10L, 3, [6L, -5L, -8L] },
        { "one-value", new uint[] {0u, 99u}, 5L, 5L, 2, [5L, 5L] },
        { "reject-low-words", new uint[] {0u, 3u, 4u}, 1L, 6L, 1, [5L] },
        { "accept-least", new uint[] {3u, 4u, 9u}, 0L, 5L, 2, [4L, 3L] },
        { "power-of-two", new uint[] {0u, 4294967295u}, 0L, 7L, 2, [0L, 7L] },
        { "worst-case", new uint[] {2147483646u, 2147483647u}, 0L, 2147483648L, 1, [2147483647L] },
        { "full-unsigned-32", new uint[] {0u, 4294967295u, 123456789u}, 0L, 4294967295L, 3, [0L, 4294967295L, 123456789L] },
        { "full-signed-32", new uint[] {0u, 2147483648u, 4294967295u}, -2147483648L, 2147483647L, 3, [-2147483648L, 0L, 2147483647L] },
        { "near-int64-max", new uint[] {4294967295u, 6u}, 9223372036854775798L, 9223372036854775807L, 2, [9223372036854775803L, 9223372036854775804L] },
        { "near-int64-min", new uint[] {4294967295u, 9u}, long.MinValue, -9223372036854775799L, 2, [-9223372036854775803L, -9223372036854775799L] },
        { "wide-int64", new uint[] {4294967295u, 0u}, 9223372032559808512L, 9223372036854775807L, 2, [9223372036854775807L, 9223372032559808512L] },
    };

    public static TheoryData<string, uint[], string[], string[]> ShuffleCases => new()
    {
        { "empty", new uint[] {}, [], [] },
        { "one-item", new uint[] {}, ["a"], ["a"] },
        { "two-items-swap", new uint[] {0u}, ["a", "b"], ["b", "a"] },
        { "two-items-keep", new uint[] {1u}, ["a", "b"], ["a", "b"] },
        { "five-items", new uint[] {7u, 3u, 4u, 1u}, ["a", "b", "c", "d", "e"], ["a", "e", "b", "d", "c"] },
        { "reject-in-shuffle", new uint[] {0u, 2u, 0u}, ["a", "b", "c"], ["b", "a", "c"] },
        { "duplicates", new uint[] {1u, 1u}, ["x", "x", "y"], ["x", "y", "x"] },
        { "non-ascii", new uint[] {4294967295u, 4294967294u, 0u}, ["é", "日本", "🙂", "a"], ["日本", "é", "🙂", "a"] },
    };

    /// <summary>A word source that gives the words in order.</summary>
    private sealed class FakeWords(uint[] words)
    {
        private int next;

        public uint Next() => words[next++];

        public bool UsedAll => next == words.Length;
    }

    [Theory]
    [MemberData(nameof(RangeCases))]
    public void IntInRange(string name, uint[] words, long lo, long hi, int count, long[] want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        var source = new FakeWords(words);
        long[] got = [.. Enumerable.Range(0, count).Select(_ => RandomNumbers.IntInRange(source.Next, lo, hi))];
        Assert.Equal(want, got);
        Assert.True(source.UsedAll);
    }

    [Theory]
    [MemberData(nameof(ShuffleCases))]
    public void Shuffle(string name, uint[] words, string[] items, string[] want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        var source = new FakeWords(words);
        string[] got = [.. items];
        RandomNumbers.Shuffle(source.Next, got);
        Assert.Equal(want, got);
        Assert.True(source.UsedAll);
    }

    [Fact]
    public void BadRangeThrows()
    {
        Assert.Throws<ArgumentException>(() => RandomNumbers.IntInRange(() => 0, 1, 0));
        Assert.Throws<ArgumentException>(() => RandomNumbers.IntInRange(() => 0, 0, 1L << 32));
        Assert.Throws<ArgumentException>(() => RandomNumbers.IntInRange(() => 0, long.MinValue, long.MaxValue));
    }

    [Fact]
    public void SameSeedSameWords()
    {
        Func<uint> a = RandomNumbers.SeededWords(42), b = RandomNumbers.SeededWords(42), c = RandomNumbers.SeededWords(43);
        uint[] first = [.. Enumerable.Range(0, 10).Select(_ => a())];
        Assert.Equal(first, Enumerable.Range(0, 10).Select(_ => b()));
        Assert.NotEqual(first, Enumerable.Range(0, 10).Select(_ => c()));
    }

    [Fact]
    public void SecureWordsInRange()
    {
        var source = RandomNumbers.SecureWords();
        for (int i = 0; i < 100; i++)
        {
            Assert.InRange(RandomNumbers.IntInRange(source, 1, 6), 1, 6);
        }
    }

    [Theory]
    [InlineData(0)]
    [InlineData(1)]
    [InlineData(16)]
    [InlineData(32)]
    public void TokenHex(int nbytes)
    {
        Assert.Matches(new Regex($"^[0-9a-f]{{{2 * nbytes}}}$"), RandomNumbers.TokenHex(nbytes));
    }

    [Fact]
    public void TokensDiffer()
    {
        Assert.NotEqual(RandomNumbers.TokenHex(16), RandomNumbers.TokenHex(16));
    }
}
