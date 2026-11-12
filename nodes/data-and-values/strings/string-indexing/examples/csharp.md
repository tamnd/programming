```csharp run id=char-index-and-string-info
using System.Globalization;

string s = "naïve 😀";

// s[i] gives a char, which is one UTF-16 code unit.
Console.WriteLine($"{s[2]} {s.Length} {char.IsHighSurrogate(s[6])}");

// char.ConvertToUtf32 reads a surrogate pair as one code point.
Console.WriteLine($"{char.ConvertToUtf32(s, 6):x}");

// An index out of range throws.
try
{
    _ = s[20];
}
catch (IndexOutOfRangeException e)
{
    Console.WriteLine(e.GetType().Name);
}

// StringInfo indexes text elements, which are grapheme clusters.
string t = "cafe\u0301 🇫🇷";
var info = new StringInfo(t);
Console.WriteLine($"{t.Length} {info.LengthInTextElements} {info.SubstringByTextElements(3, 1)}");

// Index from the end with ^.
Console.WriteLine($"{s[^4]} {s[..5]}");
```

```output for=char-index-and-string-info
ï 8 True
1f600
IndexOutOfRangeException
10 6 é
e naïve
```

- Verdict: native. `s[2]` is `ï`, `Length` is 8, and `s[6]` is a high surrogate. `char.ConvertToUtf32` reads the pair as U+1F600. `s[20]` throws `IndexOutOfRangeException`. `StringInfo` counts 6 text elements in a string of 10 `char` values, and text element 3 is `é`. `s[^4]` indexes from the end, and `s[..5]` is a range.
