// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

/// <summary>The Caesar cipher and ROT13.</summary>
public static class Caesar
{
    /// <summary>
    /// Moves each letter A to Z and a to z shift places along the alphabet,
    /// from z back to a. Other characters do not change. A negative shift
    /// decodes, and Encode(text, 13) is ROT13.
    /// </summary>
    public static string Encode(string text, long shift)
    {
        var k = (int)(((shift % 26) + 26) % 26);
        return string.Create(text.Length, (text, k), static (span, state) =>
        {
            for (var i = 0; i < span.Length; i++)
            {
                var c = state.text[i];
                span[i] = c switch
                {
                    >= 'A' and <= 'Z' => (char)('A' + ((c - 'A' + state.k) % 26)),
                    >= 'a' and <= 'z' => (char)('a' + ((c - 'a' + state.k) % 26)),
                    _ => c,
                };
            }
        });
    }
}
