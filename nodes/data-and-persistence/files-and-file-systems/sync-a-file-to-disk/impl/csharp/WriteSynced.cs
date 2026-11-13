// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Runtime.Versioning;
using System.Text;

// FileStream.Flush(true) syncs a file. .NET cannot open a directory as a
// stream, so SyncDirectory calls open, fsync and close of the C library.
[UnsupportedOSPlatform("windows")]
public static class WriteSynced
{
    [DllImport("libc", EntryPoint = "open", SetLastError = true)]
    private static extern int Open(string path, int flags);

    [DllImport("libc", EntryPoint = "fsync", SetLastError = true)]
    private static extern int Fsync(int fd);

    [DllImport("libc", EntryPoint = "close", SetLastError = true)]
    private static extern int Close(int fd);

    private const int ORdOnly = 0;

    // Syncs the entries of dir, so that a new name in it is on disk.
    public static void SyncDirectory(string dir)
    {
        int fd = Open(dir, ORdOnly);
        if (fd < 0)
        {
            throw new IOException($"open {dir}", new Win32Exception(Marshal.GetLastPInvokeError()));
        }
        int rc = Fsync(fd);
        int err = Marshal.GetLastPInvokeError();
        Close(fd);
        if (rc != 0)
        {
            throw new IOException($"fsync {dir}", new Win32Exception(err));
        }
    }

    // Writes text to path as UTF-8, syncs the file, closes it, and syncs the
    // directory of path.
    public static void Write(string path, string text)
    {
        using (var fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.None))
        {
            byte[] bytes = new UTF8Encoding(false).GetBytes(text);
            fs.Write(bytes);
            // true: write the buffer of the stream, then sync the file.
            fs.Flush(true);
        }
        SyncDirectory(Path.GetDirectoryName(Path.GetFullPath(path))!);
    }
}
