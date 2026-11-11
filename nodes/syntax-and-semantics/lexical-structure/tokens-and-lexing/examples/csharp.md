```csharp run id=verbatim-raw-and-escaped-names
int i = 1, j = 10;
int k = i+++j; // lexed as i ++ + j
Console.WriteLine($"{k} {i}");

Console.WriteLine($"{1_000_000} {0b1010} {0xff} {10L} {1.5m}");
string path = @"C:\temp\new";       // verbatim: no escapes
string raw = """He said "hi".""";   // raw string literal
Console.WriteLine(path);
Console.WriteLine(raw);
int @class = 3;                     // @ turns a keyword into a name
var var = @class * 2;               // var is a contextual keyword
Console.WriteLine(var);
```

```output for=verbatim-raw-and-escaped-names
11 2
1000000 10 255 10 1.5
C:\temp\new
He said "hi".
6
```

- Verdict: library. The Roslyn compiler, in the `Microsoft.CodeAnalysis.CSharp` package, gives programs the tokens and trees of C# source. C# has three kinds of string literal: regular strings with escapes, verbatim strings with `@` that keep backslashes, and raw strings with three or more quotes. An `@` before a keyword makes it a name, and contextual keywords such as `var` can be names where the grammar allows.
