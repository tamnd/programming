// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

using System.Linq;
using Xunit;

public sealed class ScoreLinesTests
{
    public static TheoryData<string, string[], int[], string[]> Cases => new()
    {
        { "scores", new string[] { "bob", "amy", "cal" }, new int[] { 7, 9, 4 }, new string[] { "amy=9", "bob=7", "cal=4", "total=20" } },
        { "one", new string[] { "x" }, new int[] { 1 }, new string[] { "x=1", "total=1" } },
        { "empty", new string[0], new int[0], new string[] { "total=0" } },
        { "case", new string[] { "b", "B", "a", "A" }, new int[] { 1, 2, 3, 4 }, new string[] { "A=4", "B=2", "a=3", "b=1", "total=10" } },
        { "negative", new string[] { "a", "b" }, new int[] { -5, 5 }, new string[] { "a=-5", "b=5", "total=0" } },
        { "prefix", new string[] { "abc", "a", "ab" }, new int[] { 3, 2, 1 }, new string[] { "a=2", "ab=1", "abc=3", "total=6" } },
        { "reverse", new string[] { "j", "i", "h", "g", "f", "e", "d", "c", "b", "a" }, new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, new string[] { "a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Check(string name, string[] keys, int[] values, string[] want)
    {
        Assert.NotNull(name);
        var scores = keys.Zip(values).ToDictionary(p => p.First, p => p.Second);
        Assert.Equal(want, ScoreLines.Lines(scores));
    }
}
