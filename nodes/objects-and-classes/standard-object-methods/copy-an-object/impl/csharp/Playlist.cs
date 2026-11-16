// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

using System;
using System.Collections.Generic;

/// <summary>A playlist with a list field, copied in two ways.</summary>
public sealed class Playlist
{
    public string Name { get; set; }
    public List<string> Songs { get; }

    public Playlist(string name, List<string> songs)
    {
        Name = name;
        Songs = songs;
    }

    // A shallow copy: MemberwiseClone copies the fields, so the copy shares the list.
    public Playlist ShallowCopy() => (Playlist)MemberwiseClone();

    // A deep copy: a new Playlist with a new list.
    public Playlist DeepCopy() => new(Name, new List<string>(Songs));

    public static List<List<string>> CopyAndChange(IReadOnlyList<string> songs, int index, string title)
    {
        if (index < 0 || index >= songs.Count)
        {
            throw new ArgumentOutOfRangeException(nameof(index));
        }
        var original = new Playlist("mix", new List<string>(songs));
        var shallow = original.ShallowCopy();
        var deep = original.DeepCopy();
        original.Songs[index] = title;
        return new List<List<string>> { new(original.Songs), new(shallow.Songs), new(deep.Songs) };
    }
}
