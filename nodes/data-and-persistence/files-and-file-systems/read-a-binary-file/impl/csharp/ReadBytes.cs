// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

public static class ReadBytes
{
    /// <summary>Returns all of the bytes of a file, with no decoding.</summary>
    public static byte[] Read(string path) => File.ReadAllBytes(path);

    /// <summary>Returns the first n bytes of a file.</summary>
    /// <exception cref="EndOfStreamException">The file has fewer than n bytes.</exception>
    public static byte[] ReadExact(string path, int n)
    {
        using var stream = new FileStream(path, FileMode.Open, FileAccess.Read);
        var data = new byte[n];
        // ReadExactly reads again until the buffer is full. Read can stop
        // after fewer bytes, so its result must not be taken as n bytes.
        stream.ReadExactly(data);
        return data;
    }
}
