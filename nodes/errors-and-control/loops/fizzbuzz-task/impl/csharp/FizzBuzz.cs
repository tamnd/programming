// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/fizzbuzz-task/.

using System.Globalization;

public static class FizzBuzz
{
    // Returns FizzBuzz, Fizz, Buzz or the number for each number from 1 to n.
    public static List<string> Of(int n)
    {
        var result = new List<string>();
        for (int i = 1; i <= n; i++)
        {
            // Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
            if (i % 15 == 0)
            {
                result.Add("FizzBuzz");
            }
            else if (i % 3 == 0)
            {
                result.Add("Fizz");
            }
            else if (i % 5 == 0)
            {
                result.Add("Buzz");
            }
            else
            {
                result.Add(i.ToString(CultureInfo.InvariantCulture));
            }
        }
        return result;
    }
}
