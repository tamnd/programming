// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-from-standard-input/.

using System.Text;
using Xunit;

public sealed class StdinLinesTests
{
    private static readonly string LongLine = new('x', 1 << 20);

    public static TheoryData<string, string, string[]> Cases => new()
    {
        { "lf", "alpha\nbeta\ngamma\n", ["alpha", "beta", "gamma"] },
        { "blank-lines", "first\n\n\nlast line\n", ["first", "", "", "last line"] },
        { "empty", "", [] },
        { "only-newline", "\n", [""] },
        { "no-final-newline", "one\ntwo\nthree", ["one", "two", "three"] },
        { "crlf", "one\r\ntwo\r\nthree\r\n", ["one", "two", "three"] },
        { "crlf-no-final-newline", "one\r\ntwo\r\nthree", ["one", "two", "three"] },
        { "mixed-endings", "one\r\ntwo\nthree\r\n", ["one", "two", "three"] },
        { "trailing-blank-line", "last\n\n", ["last", ""] },
        { "whitespace", "  two spaces\ttab \n", ["  two spaces\ttab "] },
        { "backslashes", "C:\\temp\\new\n", ["C:\\temp\\new"] },
        { "non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", ["naïve café", "日本語のテキスト", "🙂🙂🙂🙂🙂🙂"] },
        { "long-line", LongLine + "\nend\n", [LongLine, "end"] },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ReadsStandardInput(string name, string text, string[] want)
    {
        Assert.False(string.IsNullOrEmpty(name));
        TextReader saved = Console.In;
        // The test gives the bytes to a UTF-8 reader and makes it the standard input.
        Console.SetIn(new StreamReader(new MemoryStream(Encoding.UTF8.GetBytes(text)), Encoding.UTF8));
        try
        {
            Assert.Equal(want, StdinLines.ReadStdinLines());
        }
        finally
        {
            Console.SetIn(saved);
        }
    }
}
