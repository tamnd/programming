// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

using System.Text;

public static class Utf8
{
    // The second argument makes the decoder throw for bytes that are not
    // well-formed UTF-8. Encoding.UTF8 would replace them with U+FFFD.
    private static readonly UTF8Encoding Strict = new(false, true);

    // EncodeUtf8 returns the UTF-8 bytes of the code point cp.
    public static byte[] EncodeUtf8(int cp)
    {
        return Strict.GetBytes(char.ConvertFromUtf32(cp));
    }

    // DecodeUtf8 returns the code point of the one UTF-8 sequence in b, or -1.
    public static int DecodeUtf8(byte[] b)
    {
        string s;
        try
        {
            s = Strict.GetString(b);
        }
        catch (DecoderFallbackException)
        {
            return -1;
        }
        int count = 0;
        int cp = -1;
        foreach (Rune r in s.EnumerateRunes())
        {
            count++;
            cp = r.Value;
        }
        return count == 1 ? cp : -1;
    }
}
