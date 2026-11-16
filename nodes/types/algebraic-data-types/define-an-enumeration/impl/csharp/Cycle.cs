// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

using System;
using System.Collections.Generic;

// An enum whose members have the int values 0 to 6.
public enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday }

public static class Cycle
{
    public static List<string> Of(string start, int steps)
    {
        var days = Enum.GetValues<Day>();
        var result = new List<string>();
        foreach (var d in days)
        {
            // Compare with the names, not Enum.TryParse, which also accepts
            // numbers such as "3".
            if (d.ToString().ToLowerInvariant() != start)
            {
                continue;
            }
            for (int k = 0; k < steps; k++)
            {
                var next = (Day)(((int)d + k) % days.Length);
                result.Add(next.ToString().ToLowerInvariant());
            }
            break;
        }
        return result;
    }
}
