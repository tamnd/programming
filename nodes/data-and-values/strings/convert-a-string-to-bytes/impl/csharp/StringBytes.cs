// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

using System.Text;

public static class StringBytes
{
    // ToBytes returns the UTF-8 bytes of s. Encoding.Unicode would give
    // UTF-16 bytes.
    public static byte[] ToBytes(string s)
    {
        return Encoding.UTF8.GetBytes(s);
    }
}
