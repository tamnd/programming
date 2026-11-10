// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

using System.Buffers.Text;

/// <summary>Standard is RFC 4648 section 4 with padding; Url is section 5 with no padding.</summary>
public enum Base64Alphabet
{
    Standard,
    Url,
}

/// <summary>Encodes bytes as Base64 text and decodes the text strictly.</summary>
public static class Base64Codec
{
    public static string Encode(byte[] data, Base64Alphabet alphabet) => alphabet switch
    {
        Base64Alphabet.Standard => Convert.ToBase64String(data),
        Base64Alphabet.Url => Base64Url.EncodeToString(data),
        _ => throw new ArgumentOutOfRangeException(nameof(alphabet)),
    };

    /// <summary>Decodes text, and throws a FormatException when it is not valid.</summary>
    public static byte[] Decode(string text, Base64Alphabet alphabet)
    {
        byte[] data;
        try
        {
            data = alphabet switch
            {
                Base64Alphabet.Standard => Convert.FromBase64String(text),
                Base64Alphabet.Url => Base64Url.DecodeFromChars(text),
                _ => throw new ArgumentOutOfRangeException(nameof(alphabet)),
            };
        }
        catch (FormatException e)
        {
            throw new FormatException($"invalid base64 text: {text}", e);
        }

        // Both decoders skip white space, and FromBase64String accepts
        // non-zero pad bits. Only the canonical text encodes back to itself,
        // so this check rejects them.
        if (Encode(data, alphabet) != text)
        {
            throw new FormatException($"invalid base64 text: {text}");
        }
        return data;
    }
}
