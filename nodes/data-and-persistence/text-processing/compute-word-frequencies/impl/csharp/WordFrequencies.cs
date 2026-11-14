// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

using System.Runtime.InteropServices;
using System.Text;

/// <summary>Finds the most frequent words of a text file.</summary>
public static class WordFrequencies
{
    /// <summary>
    /// Returns the n most frequent words of the file at path. A word is a run of ASCII letters,
    /// changed to lower case. The pairs are sorted by count from high to low, then by word.
    /// </summary>
    /// <exception cref="FileNotFoundException">The file does not exist.</exception>
    public static List<(string Word, long Count)> Read(string path, int n)
    {
        var counts = new Dictionary<string, long>();
        // The current word stays in word across the reads of the buffer.
        var word = new StringBuilder();
        void Add()
        {
            if (word.Length == 0)
            {
                return;
            }
            // One lookup finds the count, or adds a count of 0.
            CollectionsMarshal.GetValueRefOrAddDefault(counts, word.ToString(), out _)++;
            word.Clear();
        }
        using (FileStream stream = File.OpenRead(path))
        {
            var buffer = new byte[64 * 1024];
            int read;
            while ((read = stream.Read(buffer, 0, buffer.Length)) > 0)
            {
                foreach (byte b in buffer.AsSpan(0, read))
                {
                    if (b is >= (byte)'A' and <= (byte)'Z')
                    {
                        word.Append((char)(b - 'A' + 'a'));
                    }
                    else if (b is >= (byte)'a' and <= (byte)'z')
                    {
                        word.Append((char)b);
                    }
                    else
                    {
                        Add();
                    }
                }
            }
        }
        Add();
        // The order of a Dictionary is not fixed, so the word is the second key.
        return counts
            .OrderByDescending(e => e.Value)
            .ThenBy(e => e.Key, StringComparer.Ordinal)
            .Take(n)
            .Select(e => (e.Key, e.Value))
            .ToList();
    }
}
