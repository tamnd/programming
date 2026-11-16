```csharp run id=money
using System;

double total = 0.0;
for (int i = 0; i < 10; i++)
{
    total += 0.1;
}
Console.WriteLine($"loop: {total:R}");
Console.WriteLine($"equals 1.0: {(total == 1.0 ? "yes" : "no")}");
Console.WriteLine($"0.1 as G17: {0.1.ToString("G17")}");

decimal exact = 0m;
for (int i = 0; i < 10; i++)
{
    exact += 0.1m;
}
Console.WriteLine($"decimal sum: {exact}");
Console.WriteLine($"equals 1: {(exact == 1m ? "yes" : "no")}");
Console.WriteLine($"1m / 3m: {1m / 3m}");
```

```output for=money
loop: 0.9999999999999999
equals 1.0: no
0.1 as G17: 0.10000000000000001
decimal sum: 1.0
equals 1: yes
1m / 3m: 0.3333333333333333333333333333
```

- Verdict: native. C# `double` is binary64.
- `decimal` holds 0.1 exactly, so ten additions give 1.0. It still rounds 1/3 to 28 digits.
