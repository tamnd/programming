// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

/// <summary>A text template with $name, ${name} and $$, as in Python string.Template.</summary>
public static class TextTemplate
{
    private static readonly Regex Placeholder =
        new(@"\$(?:(\$)|\{([A-Za-z_][A-Za-z0-9_]*)\}|([A-Za-z_][A-Za-z0-9_]*))?");

    /// <summary>
    /// Returns template with $$ changed to $, and $name and ${name} changed
    /// to the value of name. The text that the evaluator returns is used as it
    /// is. An unknown name throws KeyNotFoundException, and a $ without a
    /// name throws FormatException.
    /// </summary>
    public static string Substitute(string template, string[] names, string[] values)
    {
        var table = new Dictionary<string, string>();
        for (var i = 0; i < names.Length; i++)
        {
            table[names[i]] = values[i];
        }
        return Placeholder.Replace(template, m =>
        {
            if (m.Groups[1].Success)
            {
                return "$";
            }
            var group = m.Groups[2].Success ? m.Groups[2] : m.Groups[3];
            if (!group.Success)
            {
                throw new FormatException($"a $ without a name at {m.Index}");
            }
            return table.TryGetValue(group.Value, out var value)
                ? value
                : throw new KeyNotFoundException($"unknown name: {group.Value}");
        });
    }
}
