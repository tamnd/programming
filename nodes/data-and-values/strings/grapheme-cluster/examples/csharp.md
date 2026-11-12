```csharp run id=string-info-text-elements
using System.Globalization;
using System.Text;

string t = "cafe\u0301 \U0001F1EB\U0001F1F7 \U0001F469\U0001F3FD\u200D\U0001F692";

// Length counts UTF-16 code units, and EnumerateRunes counts scalar values.
Console.WriteLine($"{t.Length} {t.EnumerateRunes().Count()}");

// StringInfo counts text elements, which are extended grapheme clusters.
var info = new StringInfo(t);
Console.WriteLine(info.LengthInTextElements);

// ParseCombiningCharacters gives the index where each text element starts.
Console.WriteLine(string.Join(" ", StringInfo.ParseCombiningCharacters(t)));

// The enumerator gives each text element as a string.
var e = StringInfo.GetTextElementEnumerator(t);
var sb = new StringBuilder();
while (e.MoveNext())
{
    sb.Append(((string)e.Current).Length).Append(' ');
}
Console.WriteLine(sb.ToString().TrimEnd());

// SubstringByTextElements cuts by graphemes, so the accent stays.
Console.WriteLine(info.SubstringByTextElements(0, 4) == "cafe\u0301");
```

```output for=string-info-text-elements
18 13
8
0 1 2 3 5 6 10 11
1 1 1 2 1 4 1 7
True
```

- Verdict: stdlib. `Length` is 18 and the text has 13 runes. `StringInfo` counts 8 text elements. `ParseCombiningCharacters` gives the `char` index where each element starts, and the enumerator gives each element, so the lengths are 1, 2, 4 and 7 `char` values. `SubstringByTextElements(0, 4)` keeps the accent.
