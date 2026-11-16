```csharp run id=enum-cast
using System;

Color c = Color.Green;
Console.WriteLine($"{c} {(int)c}");

Color odd = (Color)42; // compiles: any int converts to Color
Console.WriteLine($"{odd} {Enum.IsDefined(odd)}");

Console.WriteLine(Describe(Color.Red));
Console.WriteLine(Describe(odd));

static string Describe(Color c) => c switch
{
    Color.Red => "red",
    Color.Green => "green",
    Color.Blue => "blue",
    _ => "not a color", // needed: Color can hold other values
};

enum Color { Red, Green, Blue }
```

```output for=enum-cast
Green 1
42 False
red
not a color
```

- Verdict: idiom. A C# `enum` names constants of an integer type, but a cast turns any `int` into a `Color`. `(Color)42` has no name, and `Enum.IsDefined` reports that. A `switch` on an enum needs a default arm, because the compiler cannot assume that the value is one of the names.
