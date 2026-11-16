// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

/// <summary>A prime test with trial division.</summary>
public static class Prime
{
    /// <summary>Returns true if n is prime. i &lt;= n / i does not overflow, but i * i can.</summary>
    public static bool IsPrime(long n)
    {
        if (n < 2)
        {
            return false;
        }
        if (n % 2 == 0)
        {
            return n == 2;
        }
        for (long i = 3; i <= n / i; i += 2)
        {
            if (n % i == 0)
            {
                return false;
            }
        }
        return true;
    }
}
