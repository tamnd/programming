// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

using System.Runtime.Versioning;

// The Unix mode does not exist on Windows, so the class says so, and the
// compiler checks each call site.
[UnsupportedOSPlatform("windows")]
public static class SetMode
{
    /// <summary>
    /// Sets the mode of the file at path, and returns the new permission bits.
    /// mode is three or four octal digits, as for the chmod command. The code
    /// checks each character, because in .NET the pattern ^[0-7]{3,4}$ also
    /// matches "640\n": $ matches before a final newline. The values of
    /// UnixFileMode are the Unix bits, so the number casts to the enum.
    /// File.SetUnixFileMode follows a symbolic link, and the umask does not
    /// apply. A missing file gives FileNotFoundException.
    /// </summary>
    public static int Set(string path, string mode)
    {
        if (mode is null || mode.Length is < 3 or > 4 || !mode.All(c => c is >= '0' and <= '7'))
        {
            throw new ArgumentException($"invalid mode: {mode}", nameof(mode));
        }
        File.SetUnixFileMode(path, (UnixFileMode)Convert.ToInt32(mode, 8));
        return (int)File.GetUnixFileMode(path);
    }
}
