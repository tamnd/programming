```csharp run id=runes-and-text-elements
using System;
using System.Globalization;
using System.Linq;
using System.Text;

string s = "cafe\u0301 \U0001F44D\U0001F3FD";
Console.WriteLine($"{s.Length} {s.EnumerateRunes().Count()} {new StringInfo(s).LengthInTextElements} {Encoding.UTF8.GetByteCount(s)}");

Console.WriteLine($"{(int)s[6]:X} {Rune.GetRuneAt(s, 6).Value:X} {char.IsSurrogate(s[6])}");

string nfc = s.Normalize(NormalizationForm.FormC);
Console.WriteLine($"{nfc.Length} {nfc == "caf\u00e9 \U0001F44D\U0001F3FD"}");
```

```output for=runes-and-text-elements
10 8 6 15
D83D 1F44D True
9 True
```

- Verdict: native. A .NET `string` holds UTF-16 code units, and `Length` counts them. `EnumerateRunes` gives code points as `Rune` values, `StringInfo` counts text elements, which are extended grapheme clusters, and `Encoding.UTF8` converts to bytes. `Normalize` converts between normalization forms.
