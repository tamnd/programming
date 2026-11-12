```csharp run id=null-coalescing-and-patterns
// Shift binds less tightly than +, so this is 1 << 3.
Console.WriteLine(1 << 2 + 1);

// ?? binds less tightly than +, so this is have ?? (10 + 1).
int? have = 5;
Console.WriteLine(have ?? 10 + 1);
Console.WriteLine((have ?? 10) + 1);

// == binds more tightly than &, so x & 1 == 0 does not compile for an int x.
int x = 6;
Console.WriteLine((x & 1) == 0);

// A pattern after `is` uses its own `and`, `or` and `not`, with `not` binding most tightly.
object o = 7;
Console.WriteLine(o is int and > 0 and not 5);

// The conditional operator and ?? group to the right.
string? a = null, b = null, c = "c";
Console.WriteLine(a ?? b ?? c);
int n = 9;
Console.WriteLine(n % 15 == 0 ? "FizzBuzz" : n % 3 == 0 ? "Fizz" : n % 5 == 0 ? "Buzz" : n.ToString());

// Casts bind more tightly than binary operators.
double d = 7.9;
Console.WriteLine((int)d * 2 + " " + (int)(d * 2));
```

```output for=null-coalescing-and-patterns
8
5
6
True
True
c
Fizz
14 15
```

- Verdict: native. `+` binds more tightly than `<<` and `??`, so `have ?? 10 + 1` is `have ?? (10 + 1)`. `==` binds more tightly than `&`. Patterns after `is` have their own `and`, `or` and `not`. `??` and `?:` group to the right, so a chain of them reads from left to right. A cast binds more tightly than `*`, so `(int)d * 2` converts first.
