// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

using System;
using System.Collections.Generic;
using Xunit;

public sealed class PlaylistTests
{
    public static TheoryData<string, string[], int, string, string[][]> Cases => new()
    {
        { "middle", new[] { "Intro", "Song", "Outro" }, 1, "Hit",
          new[] { new[] { "Intro", "Hit", "Outro" }, new[] { "Intro", "Hit", "Outro" }, new[] { "Intro", "Song", "Outro" } } },
        { "two", new[] { "A", "B" }, 0, "C", new[] { new[] { "C", "B" }, new[] { "C", "B" }, new[] { "A", "B" } } },
        { "single", new[] { "Solo" }, 0, "Duet", new[] { new[] { "Duet" }, new[] { "Duet" }, new[] { "Solo" } } },
        { "first", new[] { "a", "b", "c" }, 0, "z", new[] { new[] { "z", "b", "c" }, new[] { "z", "b", "c" }, new[] { "a", "b", "c" } } },
        { "last", new[] { "a", "b", "c" }, 2, "z", new[] { new[] { "a", "b", "z" }, new[] { "a", "b", "z" }, new[] { "a", "b", "c" } } },
        { "same-title", new[] { "a", "b" }, 1, "b", new[] { new[] { "a", "b" }, new[] { "a", "b" }, new[] { "a", "b" } } },
        { "duplicates", new[] { "x", "x", "x" }, 1, "y", new[] { new[] { "x", "y", "x" }, new[] { "x", "y", "x" }, new[] { "x", "x", "x" } } },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void CopyAndChange(string name, string[] songs, int index, string title, string[][] want)
    {
        Assert.NotNull(name);
        var got = Playlist.CopyAndChange(songs, index, title);
        Assert.Equal(want.Length, got.Count);
        for (var i = 0; i < want.Length; i++)
        {
            Assert.Equal(want[i], got[i]);
        }
    }

    [Fact]
    public void CopiesAreNewObjects()
    {
        var original = new Playlist("mix", new List<string> { "a" });
        var shallow = original.ShallowCopy();
        var deep = original.DeepCopy();
        Assert.NotSame(original, shallow);
        Assert.Same(original.Songs, shallow.Songs);
        Assert.NotSame(original.Songs, deep.Songs);
        shallow.Name = "other";
        Assert.Equal("mix", original.Name);
    }

    [Fact]
    public void BadIndex()
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => Playlist.CopyAndChange(new[] { "a" }, 1, "b"));
    }
}
