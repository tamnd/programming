```csharp run id=invariant-culture-and-ordinal
using System.Globalization;

// .NET maps one char to one char, so "ß" stays "ß" and the length does not change.
string s = "straße";
Console.WriteLine($"{s.ToUpperInvariant()} {s.Length} {s.ToUpperInvariant().Length}");

// A CultureInfo gives the Turkish rules.
var tr = new CultureInfo("tr-TR");
Console.WriteLine($"{"title".ToUpperInvariant()} {"title".ToUpper(tr)}");
Console.WriteLine($"{"TITLE".ToLowerInvariant()} {"TITLE".ToLower(tr)}");

// OrdinalIgnoreCase compares char by char after a simple case mapping.
Console.WriteLine(string.Equals("ΌΣΟΣ", "όσος", StringComparison.OrdinalIgnoreCase));
Console.WriteLine(string.Equals("straße", "STRASSE", StringComparison.OrdinalIgnoreCase));

// A culture-aware comparison that ignores case.
Console.WriteLine(string.Compare("Résumé", "RÉSUMÉ", CultureInfo.InvariantCulture, CompareOptions.IgnoreCase) == 0);

// TextInfo.ToTitleCase capitalizes each word, but it keeps words in all capitals.
TextInfo ti = CultureInfo.InvariantCulture.TextInfo;
Console.WriteLine(ti.ToTitleCase("hello WORLD from nasa"));
```

```output for=invariant-culture-and-ordinal
STRAßE 6 6
TITLE TİTLE
title tıtle
True
False
True
Hello WORLD From Nasa
```

- Verdict: native. `ToUpperInvariant` maps one `char` to one `char`, so `ß` stays `ß` and the length stays 6. The `tr-TR` culture gives `TİTLE` and `tıtle`. `OrdinalIgnoreCase` finds the two sigma forms equal, but not `straße` and `STRASSE`. A comparison with `InvariantCulture` and `IgnoreCase` finds `Résumé` and `RÉSUMÉ` equal. `ToTitleCase` capitalizes each word and keeps `WORLD` in capitals.
