// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

using System.Collections.Generic;

public static class Brackets
{
    // Returns true if the brackets of text match and nest correctly.
    public static bool Balanced(string text)
    {
        var stack = new Stack<char>();
        foreach (char c in text)
        {
            switch (c)
            {
                case '(' or '[' or '{':
                    stack.Push(c);
                    break;
                case ')' or ']' or '}':
                    // TryPop returns false on an empty stack; Pop would throw.
                    if (!stack.TryPop(out char top) || top != Opening(c))
                    {
                        return false;
                    }
                    break;
            }
        }
        return stack.Count == 0;
    }

    private static char Opening(char close) => close switch
    {
        ')' => '(',
        ']' => '[',
        _ => '{',
    };
}
