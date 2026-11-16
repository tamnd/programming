// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compose-functions/.

using System.Linq;
using Xunit;

public sealed class CompositionTests
{
    private static string[] Times(string[] names, int n) =>
        Enumerable.Repeat(names, n).SelectMany(x => x).ToArray();

    public static TheoryData<string, string[], int, int> Cases => new()
    {
        { "inc-after-double", new[] { "inc", "double" }, 3, 7 },
        { "double-after-inc", new[] { "double", "inc" }, 3, 8 },
        { "three-functions", new[] { "negate", "dec", "double" }, 5, -9 },
        { "empty", System.Array.Empty<string>(), 42, 42 },
        { "single-zero", new[] { "negate" }, 0, 0 },
        { "negative", new[] { "dec", "negate" }, -5, 4 },
        { "low", new[] { "inc" }, -1000, -999 },
        { "duplicates", new[] { "inc", "inc", "inc" }, 7, 10 },
        { "limit-high", Times(new[] { "double" }, 20), 1000, 1048576000 },
        { "limit-low", Times(new[] { "double" }, 20), -1000, -1048576000 },
        { "mixed-20", Times(new[] { "double", "inc" }, 10), 1000, 1026046 },
        { "mixed-20-swapped", Times(new[] { "inc", "double" }, 10), 1000, 1025023 },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void ComposeAll(string name, string[] ops, int x, int want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Composition.ComposeAll(ops, x));
    }

    [Fact]
    public void ComposeIsGeneric()
    {
        var shout = Composition.Compose<string>(s => s + "!", s => s.ToUpperInvariant(), s => s.Trim());
        Assert.Equal("HELLO!", shout("  hello "));
    }

    [Fact]
    public void ComposeOfNothingIsTheIdentity()
    {
        Assert.Equal("same", Composition.Compose<string>()("same"));
    }
}
