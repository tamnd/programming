// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

using System.Text;

/// <summary>
/// The lines, words and bytes of a UTF-8 file, as wc counts them. Lines is the number of LF bytes,
/// and a word is a run of one or more code points that are not White_Space.
/// </summary>
public readonly record struct WordCount(long Lines, long Words, long Bytes)
{
    // throwOnInvalidBytes: true makes the decoder throw on a bad byte sequence.
    private static readonly UTF8Encoding StrictUtf8 = new(encoderShouldEmitUTF8Identifier: false, throwOnInvalidBytes: true);

    /// <summary>True for the 25 code points that have the Unicode property White_Space.</summary>
    public static bool IsWhiteSpace(char c) =>
        c is (>= '\u0009' and <= '\u000d') or ' ' or '\u0085' or '\u00a0' or '\u1680'
            or (>= '\u2000' and <= '\u200a') or '\u2028' or '\u2029' or '\u202f' or '\u205f' or '\u3000';

    /// <summary>Counts the lines, words and bytes of the UTF-8 file at path.</summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    /// <exception cref="DecoderFallbackException">The bytes are not valid UTF-8.</exception>
    public static WordCount Read(string path)
    {
        long lines = 0, words = 0;
        bool inWord = false;
        using FileStream stream = File.OpenRead(path);
        // detectEncodingFromByteOrderMarks: false keeps a leading U+FEFF as a character.
        using var reader = new StreamReader(stream, StrictUtf8, detectEncodingFromByteOrderMarks: false, bufferSize: 64 * 1024);
        var buffer = new char[32 * 1024];
        int n;
        while ((n = reader.Read(buffer, 0, buffer.Length)) > 0)
        {
            foreach (char c in buffer.AsSpan(0, n))
            {
                // Each White_Space code point is one char. A surrogate is never white space.
                if (IsWhiteSpace(c))
                {
                    inWord = false;
                    if (c == '\n') lines++;
                }
                else if (!inWord)
                {
                    inWord = true;
                    words++;
                }
            }
        }
        // The reader has read the whole stream, so Position is the number of bytes.
        return new WordCount(lines, words, stream.Position);
    }
}
