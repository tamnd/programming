```csharp run id=readline-loop-and-euclid
using System.IO;

// ReadLine returns null at the end of the text.
var reader = new StringReader("one\ntwo\nthree");
string? line;
int count = 0;
while ((line = reader.ReadLine()) != null)
{
    count++;
    Console.WriteLine($"{count}: {line}");
}

// Euclid's algorithm: loop until the remainder is 0.
int a = 1071, b = 462;
while (b != 0)
{
    (a, b) = (b, a % b);
}
Console.WriteLine($"gcd: {a}");
```

```output for=readline-loop-and-euclid
1: one
2: two
3: three
gcd: 21
```

- Verdict: native. The condition assigns the next line and tests it for `null`, the usual read loop of C#. Euclid's algorithm runs while the remainder is not 0, and a tuple assignment updates both variables at once.
