// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

using Xunit;

public sealed class BracketsTests
{
    public static TheoryData<string, string, bool> Cases => new()
    {
        { "pair", "()", true },
        { "nested", "{[()()]}", true },
        { "code", "f(a[0], {b: 1})", true },
        { "crossed", "([)]", false },
        { "empty", "", true },
        { "no-brackets", "abc", true },
        { "unclosed", "((", false },
        { "extra-close", "())", false },
        { "close-first", ")(", false },
        { "wrong-kind", "(]", false },
        { "deep", new string('(', 1000) + new string(')', 1000), true },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string text, bool want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Brackets.Balanced(text));
    }
}
