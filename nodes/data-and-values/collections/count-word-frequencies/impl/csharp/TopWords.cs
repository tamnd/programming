// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

using System.Text.RegularExpressions;

/// <summary>Counts the words of a text and returns the most frequent ones.</summary>
public static partial class WordCounter
{
    // A word is a run of ASCII letters, ASCII digits and UTF-16 units that are not ASCII.
    [GeneratedRegex(@"[A-Za-z0-9\u0080-\uFFFF]+")]
    private static partial Regex Word();

    /// <summary>
    /// Returns the n most frequent words of text, by count from high to low, then by word in
    /// code point order.
    /// </summary>
    public static List<(string Word, long Count)> TopWords(string text, int n)
    {
        var counts = new Dictionary<string, long>();
        foreach (Match m in Word().Matches(text))
        {
            var word = m.Value.ToLowerInvariant();
            counts[word] = counts.GetValueOrDefault(word) + 1;
        }
        // StringComparer.Ordinal compares UTF-16 code units. The default comparer of
        // ThenBy depends on the culture.
        return counts
            .OrderByDescending(e => e.Value)
            .ThenBy(e => e.Key, StringComparer.Ordinal)
            .Take(n)
            .Select(e => (e.Key, e.Value))
            .ToList();
    }
}
