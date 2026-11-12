// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

using Xunit;

public sealed class RepeatTests
{
    // The case many has the 200 chars of 100 copies of ab in one literal.
    public static TheoryData<string, string, int, string> Cases => new()
    {
        { "word", "ab", 3, "ababab" },
        { "one", "abc", 1, "abc" },
        { "dash", "-", 5, "-----" },
        { "zero", "abc", 0, "" },
        { "empty", "", 5, "" },
        { "empty-zero", "", 0, "" },
        { "many", "ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab" },
        { "astral", "\U0001f642", 3, "\U0001f642\U0001f642\U0001f642" },
        { "combining", "e\u0301", 2, "e\u0301e\u0301" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Repeat(string name, string s, int n, string want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, StringRepeat.Repeat(s, n));
    }
}
