```csharp run id=utf16-char-storage
using System.Text;

// A char is one UTF-16 code unit of 2 bytes.
Console.WriteLine(sizeof(char));

// The same text in three encodings.
foreach (string text in new[] { "hello", "café", "日本", "😀" })
{
    Console.WriteLine($"{text}: chars={text.Length} utf-8={Encoding.UTF8.GetByteCount(text)} utf-16={Encoding.Unicode.GetByteCount(text)}");
}

// Latin-1 replaces characters that it cannot hold with '?'.
byte[] latin = Encoding.Latin1.GetBytes("café 日本");
Console.WriteLine(Encoding.Latin1.GetString(latin));

// A string stores its length, so it can hold a null character.
string s = "a\0b";
Console.WriteLine($"{s.Length} {(int)s[1]}");
```

```output for=utf16-char-storage
2
hello: chars=5 utf-8=5 utf-16=10
café: chars=4 utf-8=5 utf-16=8
日本: chars=2 utf-8=6 utf-16=4
😀: chars=2 utf-8=4 utf-16=4
café ??
3 0
```

- Verdict: native. A `char` takes 2 bytes, and `Length` counts UTF-16 code units. `GetByteCount` gives the size of each text in UTF-8 and UTF-16. `Encoding.Latin1` replaces the characters that it cannot hold with `?`. A string stores its length, so it can hold a null character.
