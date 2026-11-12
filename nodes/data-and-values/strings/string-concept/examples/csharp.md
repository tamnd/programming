```csharp run id=immutable-string-and-builder
using System.Text;

// A string holds UTF-16 code units. Length counts them.
string s = "naïve 😀";
Console.WriteLine($"{s.Length} {s.EnumerateRunes().Count()}");

// A string is immutable. Replace and ToUpperInvariant return new strings.
string t = s.Replace("naïve", "brave");
Console.WriteLine($"{s} | {t} | {s.ToUpperInvariant()}");

// string is a reference type, but == compares the characters.
string a = "ab";
string b = new string(new[] { 'a', 'b' });
Console.WriteLine($"{a == b} {ReferenceEquals(a, b)}");

// StringBuilder is the mutable string type.
var sb = new StringBuilder("cat");
sb[0] = 'b';
sb.Append('s');
Console.WriteLine($"{sb} {sb.Length}");

// Encoding.UTF8 converts between text and bytes.
byte[] utf8 = Encoding.UTF8.GetBytes(s);
Console.WriteLine($"{utf8.Length} {Encoding.UTF8.GetString(utf8) == s}");
```

```output for=immutable-string-and-builder
8 7
naïve 😀 | brave 😀 | NAÏVE 😀
True False
bats 4
11 True
```

- Verdict: native. `Length` counts UTF-16 code units and `EnumerateRunes` counts code points. `Replace` and `ToUpperInvariant` return new strings. `==` compares the characters of two strings, but `ReferenceEquals` shows that they are two objects. A `StringBuilder` changes in place. `Encoding.UTF8` converts between text and bytes.
