```csharp run id=query-syntax-and-sugar
int[] nums = [5, 3, 8, 1];

// Query syntax is sugar: the compiler turns it into method calls.
var q1 = from n in nums where n > 2 orderby n select n * 10;
var q2 = nums.Where(n => n > 2).OrderBy(n => n).Select(n => n * 10);
Console.WriteLine($"{string.Join(",", q1)} | {string.Join(",", q2)}");

// String interpolation is sugar for a call that builds the string.
int a = 2, b = 3;
Console.WriteLine($"{a} + {b} = {a + b}");

// A property looks like a field in the syntax, but it calls a method.
var t = new Temp { Celsius = 100 };
Console.WriteLine(t.Fahrenheit);

// A pattern in a switch expression is a short form of tests and casts.
object o = 3.5;
Console.WriteLine(o switch { int i => $"int {i}", double d when d > 3 => $"big {d}", _ => "other" });

class Temp
{
    public double Celsius { get; set; }
    public double Fahrenheit => Celsius * 9 / 5 + 32;
}
```

```output for=query-syntax-and-sugar
30,50,80 | 30,50,80
2 + 3 = 5
212
big 3.5
```

- Verdict: native. The compiler translates query syntax into the method calls `Where`, `OrderBy` and `Select`, so the two queries give the same result. Interpolation builds a string. A property looks like a field, but reading it calls a method. A switch expression with patterns is a short form of tests and casts.
