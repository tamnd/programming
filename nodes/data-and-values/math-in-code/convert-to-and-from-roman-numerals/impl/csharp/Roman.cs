// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

using System;
using System.Text;

/// <summary>Conversion between integers and Roman numerals in the standard form.</summary>
public static class Roman
{
    private static readonly (int Value, string Symbol)[] Table = new (int, string)[]
    {
        (1000, "M"), (900, "CM"), (500, "D"), (400, "CD"),
        (100, "C"), (90, "XC"), (50, "L"), (40, "XL"),
        (10, "X"), (9, "IX"), (5, "V"), (4, "IV"), (1, "I"),
    };

    /// <summary>Returns the Roman numeral of n, for 1 &lt;= n &lt;= 3999.</summary>
    public static string ToRoman(long n)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(n, 1);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(n, 3999);
        var sb = new StringBuilder();
        foreach (var (value, symbol) in Table)
        {
            while (n >= value)
            {
                sb.Append(symbol);
                n -= value;
            }
        }
        return sb.ToString();
    }

    /// <summary>Returns the value of a Roman numeral in the standard form.</summary>
    public static int FromRoman(string s)
    {
        if (s.Length == 0)
        {
            throw new FormatException("not a Roman numeral");
        }
        int total = 0;
        for (int i = 0; i < s.Length; i++)
        {
            int value = Value(s[i]);
            if (value == 0)
            {
                throw new FormatException("not a Roman numeral");
            }
            int next = i + 1 < s.Length ? Value(s[i + 1]) : 0;
            total += value < next ? -value : value;
        }
        if (total < 1 || total > 3999 || ToRoman(total) != s)
        {
            throw new FormatException("not in the standard form");
        }
        return total;
    }

    private static int Value(char c) => c switch
    {
        'I' => 1,
        'V' => 5,
        'X' => 10,
        'L' => 50,
        'C' => 100,
        'D' => 500,
        'M' => 1000,
        _ => 0,
    };
}
