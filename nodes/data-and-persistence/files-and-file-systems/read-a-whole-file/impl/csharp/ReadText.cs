// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

using System.Text;

public static class ReadText
{
    // false: no byte order mark is written; true: bad bytes throw.
    private static readonly UTF8Encoding Strict = new(false, true);

    /// <summary>Returns all of the text of a UTF-8 file, with no change to it.</summary>
    /// <remarks>File.ReadAllText would drop a byte order mark and put U+FFFD for bad bytes.</remarks>
    public static string Read(string path) => Strict.GetString(File.ReadAllBytes(path));
}
