// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.

using Xunit;

public sealed class Utf8Tests
{
    // Each byte is a number from 0 to 255. A decode result of -1 means that the bytes are not one well-formed sequence.
    public static TheoryData<string, int, byte[]> EncodeCases => new()
    {
        { "ascii-a", 0x41, new byte[] { 65 } },
        { "euro", 0x20AC, new byte[] { 226, 130, 172 } },
        { "emoji", 0x1F642, new byte[] { 240, 159, 153, 130 } },
        { "nul", 0x0, new byte[] { 0 } },
        { "one-byte-max", 0x7F, new byte[] { 127 } },
        { "two-byte-min", 0x80, new byte[] { 194, 128 } },
        { "two-byte", 0xE9, new byte[] { 195, 169 } },
        { "two-byte-max", 0x7FF, new byte[] { 223, 191 } },
        { "three-byte-min", 0x800, new byte[] { 224, 160, 128 } },
        { "three-byte-max", 0xFFFF, new byte[] { 239, 191, 191 } },
        { "four-byte-min", 0x10000, new byte[] { 240, 144, 128, 128 } },
        { "max", 0x10FFFF, new byte[] { 244, 143, 191, 191 } },
    };

    public static TheoryData<string, byte[], int> DecodeCases => new()
    {
        { "ascii-a", new byte[] { 65 }, 65 },
        { "e-acute", new byte[] { 195, 169 }, 233 },
        { "emoji", new byte[] { 240, 159, 153, 130 }, 128578 },
        { "max", new byte[] { 244, 143, 191, 191 }, 1114111 },
        { "replacement", new byte[] { 239, 191, 189 }, 65533 },
        { "empty", new byte[] {  }, -1 },
        { "stray-continuation", new byte[] { 128 }, -1 },
        { "truncated", new byte[] { 195 }, -1 },
        { "bad-continuation", new byte[] { 195, 40 }, -1 },
        { "overlong-two", new byte[] { 192, 129 }, -1 },
        { "overlong-three", new byte[] { 224, 128, 128 }, -1 },
        { "surrogate", new byte[] { 237, 160, 128 }, -1 },
        { "above-max", new byte[] { 244, 144, 128, 128 }, -1 },
        { "two-sequences", new byte[] { 65, 66 }, -1 },
    };

    [Theory]
    [MemberData(nameof(EncodeCases))]
    public void EncodeUtf8(string name, int cp, byte[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Utf8.EncodeUtf8(cp));
    }

    [Theory]
    [MemberData(nameof(DecodeCases))]
    public void DecodeUtf8(string name, byte[] b, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Utf8.DecodeUtf8(b));
    }
}
