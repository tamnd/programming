// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

using System.Runtime.Versioning;
using System.Security.Cryptography;
using System.Text;

// Path.GetTempFileName makes a file only in the temporary folder of the user,
// with the end .tmp, and Directory.CreateTempSubdirectory makes a directory.
// So MakeTempFile makes the name from random characters, and opens it with
// FileMode.CreateNew, which fails when the path exists, and with the Unix mode
// 0600. It tries a new name when the name is taken.
[UnsupportedOSPlatform("windows")]
public static class TempFile
{
    private const string Chars = "abcdefghijklmnopqrstuvwxyz0123456789";

    private static FileStream Open(string dir, string prefix, out string path)
    {
        if (prefix.Contains('/'))
        {
            throw new ArgumentException($"invalid prefix: {prefix}", nameof(prefix));
        }
        var options = new FileStreamOptions
        {
            Mode = FileMode.CreateNew,
            Access = FileAccess.ReadWrite,
            UnixCreateMode = UnixFileMode.UserRead | UnixFileMode.UserWrite,
        };
        for (int attempt = 0; attempt < 100; attempt++)
        {
            path = Path.Combine(dir, prefix + RandomNumberGenerator.GetString(Chars, 10));
            try
            {
                return new FileStream(path, options);
            }
            catch (IOException) when (File.Exists(path) || Directory.Exists(path))
            {
                // Another file has the name: try a new name.
            }
        }
        throw new IOException("no free temporary name after 100 attempts");
    }

    public static string MakeTempFile(string dir, string prefix)
    {
        using FileStream f = Open(dir, prefix, out string path);
        return path;
    }

    public static string UseTempFile(string dir, string prefix, string data)
    {
        FileStream f = Open(dir, prefix, out string path);
        try
        {
            using (f)
            {
                byte[] bytes = new UTF8Encoding(false).GetBytes(data);
                f.Write(bytes);
                f.Position = 0;
                using var reader = new StreamReader(f, new UTF8Encoding(false));
                return reader.ReadToEnd();
            }
        }
        finally
        {
            File.Delete(path);
        }
    }
}
