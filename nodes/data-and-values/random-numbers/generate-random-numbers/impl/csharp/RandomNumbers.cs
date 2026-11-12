// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/generate-random-numbers/.

using System.Security.Cryptography;

/// <summary>Random integers in a range, a shuffle, a seeded generator and tokens.</summary>
public static class RandomNumbers
{
    private const ulong Words = 1UL << 32;

    /// <summary>
    /// Returns an integer from lo to hi, each with equal probability. A word below 2^32 % n is
    /// rejected, so that each remainder comes from the same number of words.
    /// </summary>
    public static long IntInRange(Func<uint> nextWord, long lo, long hi)
    {
        // hi - lo can overflow a long, but as a ulong it is the true difference.
        ulong span = unchecked((ulong)(hi - lo));
        if (hi < lo || span >= Words)
        {
            throw new ArgumentException("need lo <= hi and at most 2^32 values");
        }
        ulong n = span + 1;
        ulong least = Words % n;
        while (true)
        {
            uint word = nextWord();
            if (word >= least)
            {
                return unchecked(lo + (long)(word % n));
            }
        }
    }

    /// <summary>Shuffles items in place with the Fisher-Yates method.</summary>
    public static void Shuffle<T>(Func<uint> nextWord, IList<T> items)
    {
        for (int i = items.Count - 1; i > 0; i--)
        {
            int j = (int)IntInRange(nextWord, 0, i);
            (items[i], items[j]) = (items[j], items[i]);
        }
    }

    /// <summary>Returns a word source that gives the same words for the same seed.</summary>
    public static Func<uint> SeededWords(int seed)
    {
        var random = new Random(seed);
        return () => (uint)random.NextInt64(1L << 32);
    }

    /// <summary>Returns a word source from the secure generator of the platform.</summary>
    public static Func<uint> SecureWords() =>
        () => BitConverter.ToUInt32(RandomNumberGenerator.GetBytes(sizeof(uint)));

    /// <summary>Returns nbytes secure random bytes as 2 * nbytes lowercase hexadecimal digits.</summary>
    public static string TokenHex(int nbytes) =>
        RandomNumberGenerator.GetHexString(2 * nbytes, lowercase: true);
}
