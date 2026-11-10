```csharp run id=glyphs
using System;
using System.Collections.Generic;
using System.Linq;

var factory = new GlyphFactory();
List<Glyph> line = "abracadabra".Select(factory.Get).ToList();

int column = 0;
var drawn = new List<string>();
foreach (Glyph glyph in line)
{
    drawn.Add(glyph.Draw(column));
    column += glyph.Width;
}
Console.WriteLine(string.Join(" ", drawn));
Console.WriteLine($"characters: {line.Count}, glyph objects: {factory.Count}");
Console.WriteLine($"first and last a are one object: {ReferenceEquals(line[0], line[^1])}");

// A flyweight holds only the intrinsic state: the character and its width.
sealed record Glyph(char Symbol, int Width)
{
    // the column is extrinsic: the caller passes it in
    public string Draw(int column) => $"{Symbol}@{column}";
}

// The flyweight factory: one Glyph for each character.
sealed class GlyphFactory
{
    private static readonly Dictionary<char, int> Widths = new() { ['i'] = 1, ['l'] = 1, ['r'] = 1, ['m'] = 3, ['w'] = 3 };
    private readonly Dictionary<char, Glyph> glyphs = [];

    public int Count => glyphs.Count;

    public Glyph Get(char symbol)
    {
        if (!glyphs.TryGetValue(symbol, out Glyph? glyph))
        {
            glyph = new Glyph(symbol, Widths.GetValueOrDefault(symbol, 2));
            glyphs[symbol] = glyph;
        }
        return glyph;
    }
}
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: True
```

- Verdict: idiom. A `Dictionary` from the key to the shared object, and a positional `record` with init-only properties, so a shared glyph cannot change.
- The test uses `ReferenceEquals`. A record's `==` compares the values of its properties, so `==` would be true for two separate glyphs with the same character.
- `string.Intern` returns the one string in the intern pool with a given value. The documentation says that this memory is not likely to be released until the runtime terminates, so interning strings from input can hold memory for the life of the process.
- A `struct` is the other way to avoid many small objects. A value type has no object header and no reference, and a `List<T>` of them stores the values inline.
