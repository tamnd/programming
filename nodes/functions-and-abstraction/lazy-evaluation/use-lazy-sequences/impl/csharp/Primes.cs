// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-lazy-sequences/.

using System.Collections.Generic;
using System.Linq;

public static class Primes
{
    // IsPrime returns true if k is a prime number, by trial division.
    public static bool IsPrime(int k)
    {
        if (k < 2)
        {
            return false;
        }
        for (int d = 2; d * d <= k; d++)
        {
            if (k % d == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Naturals is the infinite sequence start, start + 1, ... It runs only when it is read.
    private static IEnumerable<int> Naturals(int start)
    {
        for (int i = start; ; i++)
        {
            yield return i;
        }
    }

    // All is the infinite lazy sequence of the prime numbers.
    public static IEnumerable<int> All()
    {
        // Where uses deferred execution: it tests a number only when it is read.
        return Naturals(2).Where(IsPrime);
    }

    // First returns the first n prime numbers.
    public static int[] First(int n)
    {
        return All().Take(n).ToArray();
    }
}
