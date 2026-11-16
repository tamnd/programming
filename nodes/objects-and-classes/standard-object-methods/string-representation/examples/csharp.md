```csharp run id=override-tostring-and-record
using System;

var b = new Book("Dune", 1965);
Console.WriteLine(b);
Console.WriteLine(new Film("Alien", 1979));
Console.WriteLine($"Watch {new Film("Alien", 1979)}");
Console.WriteLine(new Plain());

record Book(string Title, int Year);

sealed class Film
{
    public Film(string title, int year) => (Title, Year) = (title, year);

    public string Title { get; }

    public int Year { get; }

    public override string ToString() => $"{Title} ({Year})";
}

sealed class Plain;
```

```output for=override-tostring-and-record
Book { Title = Dune, Year = 1965 }
Alien (1979)
Watch Alien (1979)
Plain
```

- Verdict: native. `Object.ToString` gives every object a string form, and `Console.WriteLine` and string interpolation call it.
- A record generates `ToString` with the type name and its properties in braces. A class keeps the default, the full name of its type, here `Plain`, until it overrides the method.
- `IFormattable` adds a `ToString(format, provider)` overload for types such as numbers and dates, which need format strings and culture rules.
