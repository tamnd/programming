// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-sha-256-hash/.

using System.Security.Cryptography;
using System.Text;

public static class Sha256Hash
{
    // Returns the SHA-256 of the UTF-8 bytes of text, as lowercase hex.
    public static string Sha256Text(string text)
    {
        byte[] digest = SHA256.HashData(Encoding.UTF8.GetBytes(text));
        // Convert.ToHexString gives uppercase digits.
        return Convert.ToHexStringLower(digest);
    }

    // Returns the SHA-256 of the bytes of the file at path, as lowercase hex.
    // HashData reads the stream in chunks, so a large file does not fill the memory.
    public static string Sha256File(string path)
    {
        using FileStream stream = File.OpenRead(path);
        return Convert.ToHexStringLower(SHA256.HashData(stream));
    }
}
