// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

using System.Runtime.Versioning;

// .NET has no lock call for a whole file. The FileShare argument of a
// FileStream does the job: on Unix, .NET enforces FileShare with advisory
// locks on the whole file. FileShare.None asks for an exclusive lock, and read
// access with FileShare.ReadWrite asks for a shared lock. When another open
// file has a lock that conflicts, the constructor throws an IOException. The
// tests check this. FileStream.Lock locks only a byte range.
[UnsupportedOSPlatform("windows")]
public static class LockFile
{
    // Asks for a shared or an exclusive lock on path, without a wait. It
    // returns null when another open file has a lock that conflicts. Dispose
    // the stream to release the lock. The file must exist.
    public static FileStream? TryLockFile(string path, bool exclusive)
    {
        if (!File.Exists(path))
        {
            // FileMode.Open gives the error, but the check makes it clear.
            throw new FileNotFoundException("no such file", path);
        }
        try
        {
            return exclusive
                ? new FileStream(path, FileMode.Open, FileAccess.ReadWrite, FileShare.None)
                : new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite);
        }
        catch (IOException e) when (e is not FileNotFoundException and not DirectoryNotFoundException)
        {
            return null;
        }
    }

    // The entry of the cases: takes held ("none", "shared" or "exclusive"),
    // then asks for want.
    public static bool TryLock(string path, string held, string want)
    {
        using FileStream? first = held == "none" ? null : TryLockFile(path, held == "exclusive");
        using FileStream? second = TryLockFile(path, want == "exclusive");
        return second != null;
    }
}
