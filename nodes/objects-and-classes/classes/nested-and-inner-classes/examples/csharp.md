```csharp run id=nested-types-and-outer-access
using System;
using System.Collections.Generic;

var p = new Playlist("road");
p.Add("one");
p.Add("two");
var c = new Playlist.Cursor(p); // the outer object is passed by hand
while (c.MoveNext())
{
    Console.WriteLine(c.Current);
}
Console.WriteLine(Playlist.Describe(p));

class Playlist
{
    private readonly string name;
    private readonly List<string> songs = [];

    public Playlist(string name) => this.name = name;

    public void Add(string song) => songs.Add(song);

    public static string Describe(Playlist p) => new Stats(p).ToString();

    // A public nested type. It has no outer instance; it gets one in its constructor.
    public class Cursor(Playlist owner)
    {
        private int next = -1;
        public bool MoveNext() => ++next < owner.songs.Count; // private members are visible
        public string Current => $"{owner.name}: {owner.songs[next]}";
    }

    // A private nested type: code outside Playlist cannot name it.
    private record Stats(Playlist P)
    {
        public override string ToString() => $"{P.songs.Count} songs";
    }
}
```

```output for=nested-types-and-outer-access
road: one
road: two
2 songs
```

- Verdict: native. A C# nested type is like a Java static nested class: it has no hidden reference to an outer object. Code that needs one passes it, here as a primary constructor parameter.
- A nested type sees the private members of the type that contains it, so `Cursor` reads `owner.songs`. Nested types are private unless they say otherwise, so `Stats` is hidden from the top-level statements.
