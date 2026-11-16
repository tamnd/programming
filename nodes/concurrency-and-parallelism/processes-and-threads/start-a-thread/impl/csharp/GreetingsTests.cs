// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/start-a-thread/.

using System.Linq;
using Xunit;

public sealed class GreetingsTests
{
    private static readonly string[] Many = Enumerable.Range(0, 32).Select(i => "t" + i).ToArray();

    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "two", new[] { "Ada", "Linus" }, new[] { "hello from Ada", "hello from Linus" } },
        { "three", new[] { "main", "worker", "logger" }, new[] { "hello from main", "hello from worker", "hello from logger" } },
        { "empty", new string[] { }, new string[] { } },
        { "single", new[] { "solo" }, new[] { "hello from solo" } },
        { "empty-name", new[] { "" }, new[] { "hello from " } },
        { "duplicate", new[] { "twin", "twin" }, new[] { "hello from twin", "hello from twin" } },
        { "unicode", new[] { "Zoë", "李雷" }, new[] { "hello from Zoë", "hello from 李雷" } },
        { "many", Many, Many.Select(n => "hello from " + n).ToArray() },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ThreadGreetings(string name, string[] names, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Greetings.ThreadGreetings(names));
    }
}
