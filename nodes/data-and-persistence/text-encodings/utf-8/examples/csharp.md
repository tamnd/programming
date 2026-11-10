```csharp run id=lengths
using System.Globalization;
using System.Text;

string text = "nai\u0308ve \U0001F469\u200D\U0001F4BB";
Console.WriteLine($"UTF-8 bytes {Encoding.UTF8.GetByteCount(text)}");
Console.WriteLine($"UTF-16 chars {text.Length}");
Console.WriteLine($"runes {text.EnumerateRunes().Count()}");
Console.WriteLine($"text elements {new StringInfo(text).LengthInTextElements}");

string composed = text.Normalize(NormalizationForm.FormC);
Console.WriteLine($"after NFC {composed.Length} {Encoding.UTF8.GetByteCount(composed)}");
```

```output for=lengths
UTF-8 bytes 19
UTF-16 chars 12
runes 10
text elements 7
after NFC 11 18
```

```csharp run id=decode
using System.Text;

byte[] bad = { 0x61, 0xC0, 0xAF, 0xED, 0xA0, 0x80, 0x62 };
string replaced = Encoding.UTF8.GetString(bad);
Console.WriteLine($"{replaced.Length} {replaced.Count(c => c == '\uFFFD')}");

try
{
    new UTF8Encoding(encoderShouldEmitUTF8Identifier: false, throwOnInvalidBytes: true).GetString(bad);
}
catch (DecoderFallbackException error)
{
    Console.WriteLine(error.GetType().Name);
}

Console.WriteLine(Convert.ToHexString(Encoding.UTF8.GetBytes("a\uD800b")));
Console.WriteLine(Convert.ToHexString(Encoding.UTF8.GetPreamble()));
Console.WriteLine(new UTF8Encoding(false).GetPreamble().Length);

byte[] withBom = { 0xEF, 0xBB, 0xBF, 0x68, 0x69 };
Console.WriteLine(Encoding.UTF8.GetString(withBom).Length);
using var reader = new StreamReader(new MemoryStream(withBom));
Console.WriteLine(reader.ReadToEnd().Length);
```

```output for=decode
7 5
DecoderFallbackException
61EFBFBD62
EFBBBF
0
3
2
```

- Verdict: stdlib. `Encoding.UTF8` in the namespace `System.Text` converts between strings and UTF-8 bytes.
- A .NET `string` is a sequence of UTF-16 `char` values. `Length` counts code units. `EnumerateRunes` gives the scalar values, and `StringInfo.LengthInTextElements` counts grapheme clusters. `string.Normalize` does the four normalization forms.
- `Encoding.UTF8` replaces bad input with U+FFFD, here one for each maximal subpart. A `UTF8Encoding` made with `throwOnInvalidBytes: true` throws `DecoderFallbackException`.
- `Encoding.UTF8.GetPreamble` returns the byte order mark `EF BB BF`. A `UTF8Encoding` made with `false` has an empty preamble. `GetString` keeps a leading byte order mark in the string. A `StreamReader` detects it and removes it.
