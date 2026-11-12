// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

public static class CopyBuffered
{
    /// <summary>
    /// Copies src to dst through buffers of size bytes and returns the number of bytes copied.
    /// </summary>
    /// <remarks>
    /// The explicit Flush writes the last bytes in the buffer, so that an error from that
    /// write comes out of this method and not out of Dispose.
    /// </remarks>
    public static long Copy(string src, string dst, int size)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(size, 1);
        using var input = new FileStream(src, FileMode.Open, FileAccess.Read, FileShare.Read, size);
        using var output = new FileStream(dst, FileMode.Create, FileAccess.Write, FileShare.None, size);
        var chunk = new byte[size];
        long total = 0;
        int n;
        while ((n = input.Read(chunk, 0, chunk.Length)) > 0)
        {
            output.Write(chunk, 0, n);
            total += n;
        }
        output.Flush();
        return total;
    }
}
