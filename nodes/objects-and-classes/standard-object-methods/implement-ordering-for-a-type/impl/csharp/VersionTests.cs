// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-ordering-for-a-type/.

using System.Collections.Generic;
using Xunit;

public sealed class VersionTests
{
    public static TheoryData<string, string[], string[]> Cases => new()
    {
        { "simple", new[] { "1.2.0", "1.0.0", "1.1.0" }, new[] { "1.0.0", "1.1.0", "1.2.0" } },
        { "fields-in-order", new[] { "2.0.0", "1.9.9", "1.10.0" }, new[] { "1.9.9", "1.10.0", "2.0.0" } },
        { "patch", new[] { "0.1.10", "0.1.2", "0.1.1" }, new[] { "0.1.1", "0.1.2", "0.1.10" } },
        { "empty", new string[] { }, new string[] { } },
        { "single", new[] { "3.4.5" }, new[] { "3.4.5" } },
        { "numeric-not-text", new[] { "10.0.0", "9.0.0", "1.0.0" }, new[] { "1.0.0", "9.0.0", "10.0.0" } },
        { "duplicates", new[] { "1.0.0", "0.9.0", "1.0.0" }, new[] { "0.9.0", "1.0.0", "1.0.0" } },
        { "zero", new[] { "0.0.1", "0.0.0" }, new[] { "0.0.0", "0.0.1" } },
        { "limit", new[] { "1000000.0.0", "999999.1000000.1000000" }, new[] { "999999.1000000.1000000", "1000000.0.0" } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void SortVersions(string name, string[] versions, string[] want)
    {
        Assert.NotNull(name);
        Assert.Equal(want, Version.SortVersions(versions));
    }

    [Fact]
    public void OrderRules()
    {
        var a = new Version(1, 2, 3);
        var b = new Version(1, 10, 0);
        var same = new Version(1, 2, 3);
        Assert.True(a < b && b > a && a <= same && a >= same);
        Assert.True(a.CompareTo(b) < 0 && b.CompareTo(a) > 0);
        Assert.Equal(0, a.CompareTo(new Version(1, 2, 3)));
        Assert.Equal(new Version(1, 2, 3), a);
        Assert.True(a.CompareTo(null) > 0);
    }

    [Fact]
    public void SortedSet()
    {
        var set = new SortedSet<Version> { new(2, 0, 0), new(1, 0, 0), new(1, 0, 0) };
        Assert.Equal(2, set.Count);
        Assert.Equal(new Version(1, 0, 0), set.Min);
    }
}
