// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

using Xunit;

public sealed class ToBytesTests
{
    // The encoding is UTF-8. U+00E9 is 2 bytes, U+20AC is 3 bytes and U+1F642 is 4 bytes. In Latin-1, U+00FF would be 1 byte.
    public static TheoryData<string, string, byte[]> Cases => new()
    {
        { "ascii", "abc", new byte[] { 97, 98, 99 } },
        { "letter", "A", new byte[] { 65 } },
        { "punct", "hi!", new byte[] { 104, 105, 33 } },
        { "empty", "", new byte[] {  } },
        { "control", "\t\n", new byte[] { 9, 10 } },
        { "two-byte", "\u00e9", new byte[] { 195, 169 } },
        { "latin-1-max", "\u00ff", new byte[] { 195, 191 } },
        { "three-byte", "\u20ac", new byte[] { 226, 130, 172 } },
        { "bom", "\ufeff", new byte[] { 239, 187, 191 } },
        { "astral", "\U0001f642", new byte[] { 240, 159, 153, 130 } },
        { "combining", "e\u0301", new byte[] { 101, 204, 129 } },
        { "mixed", "a\u00e9\U0001f642", new byte[] { 97, 195, 169, 240, 159, 153, 130 } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ToBytes(string name, string s, byte[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, StringBytes.ToBytes(s));
    }
}
